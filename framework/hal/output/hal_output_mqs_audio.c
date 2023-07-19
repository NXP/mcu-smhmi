/*
 * Copyright 2019-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief MQS (medium quality sound) audio HAL output device implementation. Used for playing audio clips over the
 * board's speakers.
 */

#include "board_define.h"
#ifdef ENABLE_OUTPUT_DEV_MqsAudio
#include "board.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "task.h"
#include "math.h"

#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_cache.h"

#include "fwk_log.h"
#include "fwk_output_manager.h"
#include "fwk_platform.h"
#include "fwk_task.h"

#include "hal_event_descriptor_common.h"
#include "hal_event_descriptor_voice.h"
#include "hal_output_dev.h"

#include "app_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* DMA */
#define MQS_DMA            DMA0
#define MQS_DMAMUX         DMAMUX0
#define MQS_SAI_EDMA_TX_CH 0
#define MQS_SAI_TX_SOURCE  kDmaRequestMuxSai3Tx

#define MQS_SAI_DMA_IRQ_PRIO (configMAX_SYSCALL_INTERRUPT_PRIORITY - 1)
/* MQS_AUDIO_CHUNK_SIZE = 1600 * 2 * 2 => 100ms at 16kHz/16bit/stereo */
#define MQS_AUDIO_CHUNK_SIZE (1600 * 2 * 2)
/* MQS_AUDIO_CHUNK_CNT should be 2 or more in order to be able to play audio without pauses */
#define MQS_AUDIO_CHUNK_CNT (2)
/* Local audio prompts are mono/16kHz/16bit */
#define PROMPTS_AUDIO_CHUNK_SIZE_1MS (16 * 2)
/* PROMPTS_AUDIO_CHUNK_SIZE = 1600 * 2 => 100ms at 16KHz/16bit/mono */
#define PROMPTS_AUDIO_CHUNK_MS   100
#define PROMPTS_AUDIO_CHUNK_SIZE (PROMPTS_AUDIO_CHUNK_MS * PROMPTS_AUDIO_CHUNK_SIZE_1MS)

#if !AMP_LOOPBACK_DISABLED
/* MQS_FEEDBACK_CHUNK_SIZE should be PROMPTS_AUDIO_CHUNK_SIZE because prompt audio and Mics are 16KHz
 * MQS_FEEDBACK_CHUNK_CNT should be (MQS_AUDIO_CHUNK_CNT + 1) in order to give AFE time to process all chunks without
 * overlapping */
#define MQS_FEEDBACK_CHUNK_SIZE (PROMPTS_AUDIO_CHUNK_SIZE)
#define MQS_FEEDBACK_CHUNK_CNT  (MQS_AUDIO_CHUNK_CNT + 1)
#endif /* !AMP_LOOPBACK_DISABLED */

#if defined(__cplusplus)
extern "C" {
#endif
void BOARD_InitMqsResource(void);
int HAL_OutputDev_MqsAudio_Register();
static hal_output_status_t HAL_OutputDev_MqsAudio_Init(output_dev_t *dev, output_dev_callback_t callback);
static hal_output_status_t HAL_OutputDev_MqsAudio_Start(const output_dev_t *dev);
static void _PlaySound(int PromptId, const uint8_t *buffer, int32_t size, uint8_t asrEnabled);
static void _StopPlayingSound(void);
static status_t _GetVolume(char *valueToString);
#if defined(__cplusplus)
}
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

static volatile SemaphoreHandle_t s_MqsSemFreeSlots = NULL;
static AT_NONCACHEABLE_SECTION_ALIGN_OCRAM(uint8_t s_MqsStreamPool[MQS_AUDIO_CHUNK_CNT][MQS_AUDIO_CHUNK_SIZE], 4);
#if !AMP_LOOPBACK_DISABLED
static AT_NONCACHEABLE_SECTION_ALIGN_OCRAM(uint8_t s_MqsAfeFeedback[MQS_FEEDBACK_CHUNK_CNT][MQS_FEEDBACK_CHUNK_SIZE],
                                           4);
#endif /* !AMP_LOOPBACK_DISABLED */

/* Used to notify AFE (and ASR) that speaker is streaming.
 * AFE will decide which is the right course of actions.
 * Current approach will disable ASR during MQS playback (barge-in disabled). */
volatile bool g_MQSPlaying = false;

static AT_NONCACHEABLE_SECTION_ALIGN(sai_edma_handle_t s_SaiTxHandle, 4);
static AT_NONCACHEABLE_SECTION_ALIGN(edma_handle_t s_SaiDmaHandle, 4);

#define DEFER_PLAYBACK_TO_TASK 1
#if DEFER_PLAYBACK_TO_TASK
typedef struct
{
    fwk_task_data_t commonData;
    const output_dev_t *dev;
} mqs_task_data_t;

typedef struct
{
    fwk_task_t task;
    mqs_task_data_t data;
} mqs_task_t;

typedef enum _mqs_configs
{
    kMQSConfigs_Volume = 0,
} mqs_configs;

static mqs_task_t s_MqsAudioTask;

#define MQS_TASK_NAME     "mqs_audio"
#define MQS_TASK_STACK    1024
#define MQS_TASK_PRIORITY 2
#if FWK_SUPPORT_STATIC_ALLOCATION
FWKDATA static StackType_t s_MqsAudioTaskStack[MQS_TASK_STACK];
FWKDATA static StaticTask_t s_MqsAudioTaskTcb;
static void *s_MqsAudioTaskTcbReference = (void *)&s_MqsAudioTaskTcb;
#else
static void *s_MqsAudioTaskStack        = NULL;
static void *s_MqsAudioTaskTcbReference = NULL;
#endif

static asr_language_t s_Language = ASR_ENGLISH;
static bool s_IsStopPlayingSound = false;

typedef struct
{
    int32_t promptId;
    const uint8_t *buffer;
    int32_t size;
    uint8_t asrEnabled;
} sound_info_t;

const static output_dev_operator_t s_OutputDev_MqsAudioOps = {
    .init   = HAL_OutputDev_MqsAudio_Init,
    .deinit = NULL,
    .start  = HAL_OutputDev_MqsAudio_Start,
    .stop   = NULL,
};

static output_dev_t s_OutputDev_MqsAudio = {
    .name         = "sound",
    .attr.type    = kOutputDevType_Audio,
    .attr.reserve = NULL,
    .ops          = &s_OutputDev_MqsAudioOps,
    .configs =
        {
            [kMQSConfigs_Volume] = {.name          = "volume",
                                    .expectedValue = "<0-100>",
                                    .description   = "% volume of the speaker",
                                    .get           = _GetVolume},
        },
    .cap.callback = NULL,
};

static void _postSoundPlayRequest(int32_t promptId, const uint8_t *buffer, int32_t size, uint8_t asrEnabled)
{
    fwk_message_t *pMsg = (fwk_message_t *)FWK_MALLOC(sizeof(fwk_message_t));

    if (pMsg != NULL)
    {
        memset(pMsg, 0, sizeof(fwk_message_t));
        pMsg->freeAfterConsumed  = 1;
        pMsg->id                 = kFWKMessageID_Raw;
        sound_info_t *pSoundInfo = FWK_MALLOC(sizeof(sound_info_t));
        if (pSoundInfo != NULL)
        {
            pSoundInfo->promptId            = promptId;
            pSoundInfo->buffer              = buffer;
            pSoundInfo->size                = size;
            pSoundInfo->asrEnabled          = asrEnabled;
            pMsg->payload.data              = pSoundInfo;
            pMsg->payload.freeAfterConsumed = 1;
            FWK_Message_Put(MQS_AUDIO_TASK_ID, &pMsg);
        }
        else
        {
            LOGE("Failed to allocate memory for mqs message info.");
            FWK_FREE(pMsg);
        }
    }
    else
    {
        LOGE("Failed to allocate memory for mqs message.");
    }
}

static void HAL_OutputDev_MqsAudio_MsgHandle(fwk_message_t *pMsg, fwk_task_data_t *pTaskData)
{
    LOGI("HAL_OutputDev_MqsAudio_MsgHandle\r");
    if ((pMsg == NULL) || (pTaskData == NULL) || (pMsg->id != kFWKMessageID_Raw) || (pMsg->payload.data == NULL))
    {
        return;
    }

    sound_info_t *pSoundInfo = (sound_info_t *)pMsg->payload.data;
    int32_t currentPromptId  = pSoundInfo->promptId;
    _PlaySound(currentPromptId, pSoundInfo->buffer, pSoundInfo->size, pSoundInfo->asrEnabled);

    if (pMsg->payload.freeAfterConsumed)
    {
        pMsg->payload.freeAfterConsumed = 0;
        FWK_FREE(pMsg->payload.data);
        pMsg->payload.data = NULL;
    }
}

#define MQS_SOUND_PLAY_FUNC(promptId, buffer, size, asrEnabled) \
    _postSoundPlayRequest((promptId), (buffer), (size), (asrEnabled))
#else
#define MQS_SOUND_PLAY_FUNC(promptId, buffer, size, asrEnabled) _PlaySound((promptId), (buffer), (size), (asrEnabled))
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

__attribute__((weak)) int APP_OutputDev_MqsAudio_InferCompleteDecode(output_algo_source_t source,
                                                                     void *inferResult,
                                                                     void **audio,
                                                                     uint32_t *len)
{
    return 0;
}

__attribute__((weak)) int APP_OutputDev_MqsAudio_InputNotifyDecode(
    const output_dev_t *dev, void *inputData, void **audio, uint32_t *len, asr_language_t language)
{
    return 0;
}

static void _SaiCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    BaseType_t xHigherPriorityTaskWoken, result;

    result = xSemaphoreGiveFromISR(s_MqsSemFreeSlots, &xHigherPriorityTaskWoken);
    if (result != pdFAIL)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

static void _ConfigMqs(void)
{
    CLOCK_EnableClock(kCLOCK_Mqs); /* Enable MQS hmclk. */

    IOMUXC_MQSEnterSoftwareReset(IOMUXC_GPR, true);                   /* Reset MQS. */
    IOMUXC_MQSEnterSoftwareReset(IOMUXC_GPR, false);                  /* Release reset MQS. */
    IOMUXC_MQSEnable(IOMUXC_GPR, true);                               /* Enable MQS. */
    IOMUXC_MQSConfig(IOMUXC_GPR, kIOMUXC_MqsPwmOverSampleRate64, 1u); /* 65.54MHz/64/16/2 = 16000Hz
                                                                        Higher frequency PWM involves
                                                                        less low frequency harmonic.*/
}

/*!
 * brief set audio volume for this amp.
 *
 * param volume volume value, support 0 ~ 100, only in multiples of 10, 0 is mute, 100 is the maximum volume value.
 */
static float _AdaptVolume(uint32_t volume)
{
    assert(volume <= 100);

    volume /= 10;

    /*
     * This is the function used for generating a nice polynomial curve for the volume levels.
     *
     *                  y = -0.0018 * x ^ 3 + 0.028 * x ^ 2
     *
     * In this case it's more like a linear function with the lower and upper ends slightly curved.
     *
     * The levels go from 0 to 1, making sure that level 1 stays low at 0.0262
     * while still being able to reach the value 1 at level 10.
     *
     * This function is called once for every volume change, so these operations shouldn't be
     * that much of a burden
     */
    return (-0.0018 * pow(volume, 3) + 0.028 * pow(volume, 2));
}

/*!
 * Expand audio mono to stereo in difference. And adjust the volume simultaneously.
 *
 */
static void _AdjustVolumeMonoToStereo(int16_t *src_mono, int16_t *dst_stereo, uint32_t sample_cnt)
{
    uint32_t i;
    float volume;

    /* The volume is decreased by multiplying the samples with values between 0 and 1 */
    volume = _AdaptVolume(s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value);

    for (i = 0; i < sample_cnt; i++)
    {
        *dst_stereo       = (*src_mono) * volume;
        *(dst_stereo + 1) = -(*dst_stereo);

        dst_stereo += 2;
        src_mono++;
    }
}

#if !AMP_LOOPBACK_DISABLED
static void _AdjustVolumeMonoToMono(int16_t *src_mono, int16_t *dst_mono, uint32_t sample_cnt)
{
    uint32_t i;
    float volume;

    volume = _AdaptVolume(s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value);

    for (i = 0; i < sample_cnt; i++)
    {
        /* The volume is decreased by multiplying the samples with values between 0 and 1 */
        *dst_mono = (*src_mono) * volume;

        dst_mono++;
        src_mono++;
    }
}

static void _SpeakerToAfeNotify(int16_t *buffer, uint32_t length)
{
    event_voice_t feedbackEvent = {0};
    output_event_t output_event = {0};

    if (s_OutputDev_MqsAudio.cap.callback != NULL)
    {
        output_event.eventId   = kOutputEvent_SpeakerToAfeFeedback;
        output_event.data      = &feedbackEvent;
        output_event.copy      = 1;
        output_event.size      = sizeof(feedbackEvent);
        output_event.eventInfo = kEventInfo_Local;

        feedbackEvent.event_base.eventId         = SPEAKER_TO_AFE_FEEDBACK;
        feedbackEvent.speaker_audio.start_time   = FWK_CurrentTimeUs();
        feedbackEvent.speaker_audio.audio_stream = buffer;
        feedbackEvent.speaker_audio.audio_length = length;

        s_OutputDev_MqsAudio.cap.callback(s_OutputDev_MqsAudio.id, output_event, 0);
    }
}
#endif /* !AMP_LOOPBACK_DISABLED */

/*!
 * @brief play audio clip
 *
 * @param buffer pointer to audio clip
 * @param size size of audio buffer
 */
static void _PlaySound(int PromptId, const uint8_t *buffer, int32_t size, uint8_t asrEnabled)
{
    sai_transfer_t xfer         = {0};
    status_t tansferStatus      = kStatus_Success;
    uint32_t audioSize          = 0;
    uint32_t audioPlayedSize    = 0;
    uint32_t audioChunkSize     = 0;
    uint8_t mqsAudioPoolSlotIdx = 0;
    bool statusOk               = true;

#if !AMP_LOOPBACK_DISABLED
    uint8_t mqsFeedbackPoolSlotIdx = 0;
#endif /* !AMP_LOOPBACK_DISABLED */

#if !AMP_LOOPBACK_DISABLED
    if (asrEnabled == 0)
    {
        g_MQSPlaying = true;
    }
#else
    g_MQSPlaying = true;
#endif /* AMP_LOOPBACK_DISABLED */

    /* Enable output of Audio amplifier */
    GPIO_PinWrite(BOARD_MQS_OE_GPIO_PORT, BOARD_MQS_OE_GPIO_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    audioSize = size - (size % PROMPTS_AUDIO_CHUNK_SIZE_1MS);

    LOGD("[MQS] Playing Audio of %d samples (%d ms)", audioSize / 2, (audioSize / PROMPTS_AUDIO_CHUNK_SIZE_1MS));

    while (audioPlayedSize < audioSize)
    {
        if (s_IsStopPlayingSound == true)
        {
            s_IsStopPlayingSound = false;
            break;
        }

        /* A new slot should be available in one PROMPTS_AUDIO_CHUNK_MS.
         * In case there is no empty slot in two PROMPTS_AUDIO_CHUNK_MS, something is wrong. */
        if (xSemaphoreTake(s_MqsSemFreeSlots, (TickType_t)(2 * PROMPTS_AUDIO_CHUNK_MS)) != pdTRUE)
        {
            LOGE("[MQS] Playing Failed, played %d samples (%d ms)", audioPlayedSize / 2,
                 (audioPlayedSize / PROMPTS_AUDIO_CHUNK_MS));
            statusOk = false;
            break;
        }

        if ((audioSize - audioPlayedSize) >= PROMPTS_AUDIO_CHUNK_SIZE)
        {
            audioChunkSize = PROMPTS_AUDIO_CHUNK_SIZE;
        }
        else
        {
            audioChunkSize = audioSize - audioPlayedSize;
        }

        _AdjustVolumeMonoToStereo((int16_t *)&buffer[audioPlayedSize], (int16_t *)s_MqsStreamPool[mqsAudioPoolSlotIdx],
                                  audioChunkSize / 2);

#if !AMP_LOOPBACK_DISABLED
        if (asrEnabled == 1)
        {
            _AdjustVolumeMonoToMono((int16_t *)(&buffer[audioPlayedSize]),
                                    (int16_t *)(s_MqsAfeFeedback[mqsFeedbackPoolSlotIdx]), audioChunkSize / 2);
        }
#endif /* !AMP_LOOPBACK_DISABLED */

        xfer.data     = s_MqsStreamPool[mqsAudioPoolSlotIdx];
        xfer.dataSize = audioChunkSize << 1; /* Prompts data is expanded into stereo. Data length becomes 2 times */

        /* Play this chunk */
        tansferStatus = SAI_TransferSendEDMA(MQS_SAI, &s_SaiTxHandle, &xfer);
        if (tansferStatus != kStatus_Success)
        {
            LOGE("[MQS] SAI_TransferSendEDMA failed %d for %d samples", tansferStatus, xfer.dataSize);
            statusOk = false;
            break;
        }

#if !AMP_LOOPBACK_DISABLED
        if (asrEnabled == 1)
        {
            /* Notify AFE in order to perform AEC */
            _SpeakerToAfeNotify((int16_t *)s_MqsAfeFeedback[mqsFeedbackPoolSlotIdx], audioChunkSize / 2);
            mqsFeedbackPoolSlotIdx = (mqsFeedbackPoolSlotIdx + 1) % MQS_FEEDBACK_CHUNK_CNT;
        }
#endif /* !AMP_LOOPBACK_DISABLED */

        mqsAudioPoolSlotIdx = (mqsAudioPoolSlotIdx + 1) % MQS_AUDIO_CHUNK_CNT;
        audioPlayedSize += audioChunkSize;
    }

    if (statusOk)
    {
        /* s_MqsSemFreeSlots will not be free at least MQS_AUDIO_CHUNK_CNT slots
         * Give one extra slot timeout to let AFE process everything. */
        vTaskDelay(PROMPTS_AUDIO_CHUNK_MS * (MQS_AUDIO_CHUNK_CNT + 1));

        if (uxSemaphoreGetCount(s_MqsSemFreeSlots) != MQS_AUDIO_CHUNK_CNT)
        {
            LOGE("[MQS] Playing Failed, not all slots are free %d", uxSemaphoreGetCount(s_MqsSemFreeSlots));
            statusOk = false;
        }
    }

    if (statusOk)
    {
        LOGD("[MQS] Playing Done");
    }
    else
    {
        SAI_TransferTerminateSendEDMA(MQS_SAI, &s_SaiTxHandle);

        for (uint8_t i = 0; i < MQS_AUDIO_CHUNK_CNT; i++)
        {
            if (uxSemaphoreGetCount(s_MqsSemFreeSlots) == MQS_AUDIO_CHUNK_CNT)
            {
                break;
            }
            else
            {
                xSemaphoreGive(s_MqsSemFreeSlots);
            }
        }
        LOGE("[MQS] Playing Failed, MQS stopped, semaphore value is %d", uxSemaphoreGetCount(s_MqsSemFreeSlots));
    }

    /* Disable output of Audio amplifier */
    GPIO_PinWrite(BOARD_MQS_OE_GPIO_PORT, BOARD_MQS_OE_GPIO_PIN, 0);

    g_MQSPlaying = false;

    static event_common_t s_PlayPromptDoneEvent;
    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_OutputInputNotify;
    output_event.data      = &s_PlayPromptDoneEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_PlayPromptDoneEvent);
    output_event.eventInfo = kEventInfo_DualCore;

    s_PlayPromptDoneEvent.eventBase.eventId = kEventID_PlayPromptDone;
    s_PlayPromptDoneEvent.data              = (void *)PromptId;
    uint8_t fromISR                         = __get_IPSR();

    s_OutputDev_MqsAudio.cap.callback(s_OutputDev_MqsAudio.id, output_event, fromISR);
}

static void _StopPlayingSound(void)
{
    s_IsStopPlayingSound = true;
    LOGD("[MQS] MQS stopped");
}

static hal_output_status_t HAL_OutputDev_MqsAudio_InferComplete(const output_dev_t *dev,
                                                                output_algo_source_t source,
                                                                void *inferResult)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    uint8_t *audioBuffer      = NULL;
    uint32_t audioLen         = 0;

    APP_OutputDev_MqsAudio_InferCompleteDecode(source, inferResult, (void *)&audioBuffer, &audioLen);
    if (audioBuffer != NULL && audioLen != 0)
    {
        int promptId       = (int)((event_common_t *)inferResult)->promptInfo.id;
        uint8_t asrEnabled = ((event_common_t *)inferResult)->promptInfo.asrEnabled;
        MQS_SOUND_PLAY_FUNC(promptId, audioBuffer, audioLen, asrEnabled);
    }

    return error;
}

static hal_output_status_t HAL_OutputDev_MqsAudio_InputNotify(const output_dev_t *dev, void *data)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    uint8_t *audioBuffer      = NULL;
    uint32_t audioLen         = 0;
    event_base_t eventBase    = *(event_base_t *)data;

    LOGI("MQS:Input Notify Event %d", eventBase.eventId);
    if (eventBase.eventId == kEventID_GetSpeakerVolume)
    {
        speaker_volume_event_t speaker;
        speaker.volume =
            s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value /*HAL_OutputDev_SmartLockConfig_GetSpeakerVolume()*/;
        LOGD("Current volume is: %d", speaker.volume);
        if (eventBase.respond != NULL)
        {
            eventBase.respond(kEventID_GetSpeakerVolume, &speaker, kEventStatus_Ok, true);
        }
    }
    else if (eventBase.eventId == kEventID_SetSpeakerVolume)
    {
        event_common_t event               = *(event_common_t *)data;
        event_status_t eventResponseStatus = kEventStatus_Ok;
        if (kSLNConfigStatus_Success /*!= HAL_OutputDev_SmartLockConfig_SetSpeakerVolume(event.speakerVolume.volume)*/)
        {
            error               = kStatus_HAL_OutputError;
            eventResponseStatus = kEventStatus_Error;
            LOGE("Failed to save speaker volume to flash");
        }
        else
        {
            LOGD("Volume set. Value is %d", event.speakerVolume.volume);
            s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value = event.speakerVolume.volume;
        }
        if (eventBase.respond != NULL)
        {
            eventBase.respond(kEventID_SetSpeakerVolume, &event.speakerVolume, eventResponseStatus, true);
        }
    }
    else if (eventBase.eventId == SET_MULTILINGUAL_CONFIG)
    {
        event_voice_t *pVoiceEvent = (event_voice_t *)data;
        s_Language                 = pVoiceEvent->set_multilingual_config.languages;
        LOGD("[MQS]: Set language %d", s_Language);
    }
    else if (eventBase.eventId == kEventID_StopPrompt)
    {
        _StopPlayingSound();
    }
    else
    {
        APP_OutputDev_MqsAudio_InputNotifyDecode(dev, data, (void *)&audioBuffer, &audioLen, s_Language);
        if (audioBuffer != NULL && audioLen != 0)
        {
            int promptId       = (int)((event_common_t *)data)->promptInfo.id;
            uint8_t asrEnabled = ((event_common_t *)data)->promptInfo.asrEnabled;
            MQS_SOUND_PLAY_FUNC(promptId, audioBuffer, audioLen, asrEnabled);
        }
    }
    return error;
}

const static output_dev_event_handler_t s_MqsAudioHandler = {
    .inferenceComplete = HAL_OutputDev_MqsAudio_InferComplete,
    .inputNotify       = HAL_OutputDev_MqsAudio_InputNotify,
};

static hal_output_status_t HAL_OutputDev_MqsAudio_Start(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    if (FWK_OutputManager_RegisterEventHandler(dev, &s_MqsAudioHandler) != 0)
    {
        LOGE("HAL_OutputDev_MqsAudio_Start failed - FWK_OutputManager_RegisterEventHandler");
        error = kStatus_HAL_OutputError;
    }

    if (error == kStatus_HAL_OutputSuccess)
    {
        _ConfigMqs();

        s_MqsSemFreeSlots = xSemaphoreCreateCounting(MQS_AUDIO_CHUNK_CNT, MQS_AUDIO_CHUNK_CNT);
        if (s_MqsSemFreeSlots == NULL)
        {
            LOGE("HAL_OutputDev_MqsAudio_Start failed - xSemaphoreCreateCounting");
            error = kStatus_HAL_OutputError;
        }
    }

    if (error == kStatus_HAL_OutputSuccess)
    {
#if DEFER_PLAYBACK_TO_TASK
        s_MqsAudioTask.task.msgHandle  = HAL_OutputDev_MqsAudio_MsgHandle;
        s_MqsAudioTask.task.taskInit   = NULL;
        s_MqsAudioTask.task.data       = (fwk_task_data_t *)&(s_MqsAudioTask.data);
        s_MqsAudioTask.task.taskId     = MQS_AUDIO_TASK_ID;
        s_MqsAudioTask.task.delayMs    = 1;
        s_MqsAudioTask.task.taskStack  = s_MqsAudioTaskStack;
        s_MqsAudioTask.task.taskBuffer = s_MqsAudioTaskTcbReference;
        s_MqsAudioTask.data.dev        = dev;
        FWK_Task_Start((fwk_task_t *)&s_MqsAudioTask, MQS_TASK_NAME, MQS_TASK_STACK, MQS_TASK_PRIORITY);
#endif

        /* TODO: Update 'HAL_OutputDev_SmartLockConfig_Get...' function to be one getter/setter */
        s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value =
            100 /*HAL_OutputDev_SmartLockConfig_GetSpeakerVolume()*/;
    }

    return error;
}

void BOARD_InitMqsResource(void)
{
    clock_root_config_t mqsRootClock = {
        .clockOff = false, .mux = MQS_SAI_CLOCK_SOURCE_SELECT, .div = MQS_SAI_CLOCK_SOURCE_DIVIDER};

    const clock_audio_pll_config_t mqsAudioPllConfig = {
        .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
        .postDivider = 1,   /* Divider after the PLL, should only be 0, 1, 2, 3, 4, 5 */
        .numerator   = 77,  /* 30 bit numerator of fractional loop divider. */
        .denominator = 100, /* 30 bit denominator of fractional loop divider */
    };

    CLOCK_InitAudioPll(&mqsAudioPllConfig);
    CLOCK_SetRootClock(MQS_SAI_CLOCK, &mqsRootClock);
}

static hal_output_status_t HAL_OutputDev_MqsAudio_Init(output_dev_t *dev, output_dev_callback_t callback)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    sai_config_t saiConfig                               = {0};
    edma_channel_Preemption_config_t dmaPreemptionConfig = {0};
    sai_transceiver_t edmaTxConfig                       = {0};

    dev->cap.callback = callback;

    BOARD_InitMqsResource();

    BOARD_InitEDMA(MQS_DMA);
    EDMA_CreateHandle(&s_SaiDmaHandle, MQS_DMA, MQS_SAI_EDMA_TX_CH);

    dmaPreemptionConfig.enableChannelPreemption = true;
    dmaPreemptionConfig.enablePreemptAbility    = false;
    dmaPreemptionConfig.channelPriority         = 0;
    EDMA_SetChannelPreemptionConfig(MQS_DMA, MQS_SAI_EDMA_TX_CH, &dmaPreemptionConfig);

    DMAMUX_Init(MQS_DMAMUX);
    DMAMUX_SetSource(MQS_DMAMUX, MQS_SAI_EDMA_TX_CH, MQS_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(MQS_DMAMUX, MQS_SAI_EDMA_TX_CH);

    NVIC_SetPriority(DMA0_DMA16_IRQn, MQS_SAI_DMA_IRQ_PRIO);

    /* Initialize SAI TX */
    SAI_TxGetDefaultConfig(&saiConfig);
    saiConfig.protocol = kSAI_BusLeftJustified;
    SAI_TxInit(MQS_SAI, &saiConfig);

    SAI_GetLeftJustifiedConfig(&edmaTxConfig, kSAI_WordWidth16bits, kSAI_Stereo, kSAI_Channel0Mask);
    SAI_TransferTxCreateHandleEDMA(MQS_SAI, &s_SaiTxHandle, _SaiCallback, NULL, &s_SaiDmaHandle);
    SAI_TransferTxSetConfigEDMA(MQS_SAI, &s_SaiTxHandle, &edmaTxConfig);

    /* Force bit clock to override standard enablement */
    SAI_TxSetBitClockRate(MQS_SAI, MQS_SAI_CLK_FREQ, kSAI_SampleRate16KHz, kSAI_WordWidth16bits, 2);

    return error;
}

static status_t _GetVolume(char *valueToString)
{
    itoa(s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value, valueToString, 10);
    strcat(valueToString, "%");
    return kStatus_Success;
}

int HAL_OutputDev_MqsAudio_Register()
{
    int error = 0;
    LOGD("HAL_OutputDev_MqsAudio_Register");
    error = FWK_OutputManager_DeviceRegister(&s_OutputDev_MqsAudio);
    return error;
}
#endif /* ENABLE_OUTPUT_DEV_MqsAudio */
