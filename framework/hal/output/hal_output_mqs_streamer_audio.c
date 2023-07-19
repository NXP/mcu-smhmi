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
#ifdef ENABLE_OUTPUT_DEV_MqsStreamerAudio
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
#include "smart_tlhmi_event_descriptor.h"
#include "hal_output_dev.h"

#include "app_config.h"

#include "fsl_os_abstraction.h"
#include "streamer_pcm.h"
#include "streamer_api.h"
#include "streamer_element_properties.h"
#include "sounds.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief PCM interface structure */
struct _pcm_rtos_t
{
    sai_transfer_t saiTx;
    sai_edma_handle_t saiTxHandle;
    edma_handle_t dmaTxHandle;

    uint32_t sample_rate;
    uint32_t bit_width;
    uint8_t num_channels;

    SemaphoreHandle_t MqsSemFreeSlots;
    bool dummy_tx_enable;
    uint8_t justLoadMetadata;
    uint8_t changeVolume;
    uint8_t changeTrack;
    int8_t promptId;
};

#define APP_STREAMER_MSG_QUEUE     "app_queue"
#define STREAMER_TASK_NAME         "Streamer"
#define STREAMER_MESSAGE_TASK_NAME "StreamerMessage"

#define STREAMER_TASK_STACK_SIZE         (20 * 1024)
#define STREAMER_MESSAGE_TASK_STACK_SIZE (2 * 1024)

#define MQS_SAI_DMA_IRQ_PRIO (configMAX_SYSCALL_INTERRUPT_PRIORITY - 1)

/* MQS_AUDIO_CHUNK_CNT should be 2 or more in order to be able to play audio without pauses */
#define MQS_AUDIO_CHUNK_CNT (2)
/* Local audio prompts are mono/16kHz/16bit */
#define PROMPTS_AUDIO_CHUNK_SIZE_1MS (16 * 2)
/* PROMPTS_AUDIO_CHUNK_SIZE = 1600 * 2 => 100ms at 16KHz/16bit/mono */
#define PROMPTS_AUDIO_CHUNK_MS   (100)
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
static status_t _GetVolume(char *valueToString);
#if defined(__cplusplus)
}
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

OSA_TASK_HANDLE_DEFINE(msg_thread);

static AT_NONCACHEABLE_SECTION_ALIGN_OCRAM(uint8_t s_MqsStreamPool[MQS_AUDIO_CHUNK_CNT][PROMPTS_AUDIO_CHUNK_SIZE], 4);
#if !AMP_LOOPBACK_DISABLED
static AT_NONCACHEABLE_SECTION_ALIGN_OCRAM(uint8_t s_MqsAfeFeedback[MQS_FEEDBACK_CHUNK_CNT][MQS_FEEDBACK_CHUNK_SIZE],
                                           4);
#endif /* !AMP_LOOPBACK_DISABLED */

/* Used to notify AFE (and ASR) that speaker is streaming.
 * AFE will decide which is the right course of actions.
 * Current approach will disable ASR during MQS playback (barge-in disabled). */
volatile bool g_MQSPlaying = false;

/*
 * For prompt playback, barge-in feature not needed. Will leverage UI event
 * to configure asrEnable. Set asrEnable as false when prompt starts
 * and revert it to true when prompt ends.
 * For music/song playback, barge-in feature is mandatory.
 */
static uint8_t asrEnabled = true;

static AT_NONCACHEABLE_SECTION_ALIGN(pcm_rtos_t pcmHandle, 4);
static STREAMER_T *streamer;

static uint8_t s_PromptPlayingRef = 0;

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

typedef struct
{
    int32_t promptId;
    const uint8_t *buffer;
    int32_t size;
    uint8_t asrEnabled;
} sound_info_t;

static asr_language_t s_Language = ASR_ENGLISH;

static MEMSRC_SET_BUFFER_T s_StreamerMemBuff;

static track_element *s_trackList;
static uint32_t s_trackNumber = 0;
static uint8_t s_trackIndex   = 0;

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
                                    .value         = STREAMER_AUDIO_DEFAULT_VOLUME,
                                    .get           = _GetVolume},
        },
    .cap.callback = NULL,
};

static void _AdjustVolumeMonoToStereo(int16_t *src_mono, int16_t *dst_stereo, uint32_t sample_cnt);
#if !AMP_LOOPBACK_DISABLED
static void _AdjustVolumeMonoToMono(int16_t *src_mono, int16_t *dst_mono, uint32_t sample_cnt);
static void _SpeakerToAfeNotify(int16_t *buffer, uint32_t length);
#endif /* AMP_LOOPBACK_DISABLED */

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

static void _SendTrackEvent(event_smart_tlhmi_t *trackInfoEvent, event_info_t eventInfo)
{
    output_event_t output_event = {0};
    uint8_t fromISR             = __get_IPSR();

    output_event.eventId   = kOutputEvent_OutputInputNotify;
    output_event.data      = trackInfoEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(event_smart_tlhmi_t);
    output_event.eventInfo = eventInfo;

    s_OutputDev_MqsAudio.cap.callback(s_OutputDev_MqsAudio.id, output_event, fromISR);
}
static void _NextTrack()
{
    PipelineState curState = STATE_NULL;
    ELEMENT_PROPERTY_T prop;
    streamer_get_state(streamer, 0, &curState, false);

    if (curState <= STATE_READY)
    {
        LOGD("Streamer needs to be in playing/pause state. Curr state: %d", curState);
        return;
    }

    pcmHandle.changeTrack = true;
    streamer_set_state(streamer, 0, STATE_NULL, true);
    s_trackIndex = (s_trackIndex + 1) % s_trackNumber;

    s_StreamerMemBuff.location = (int8_t *)s_trackList[s_trackIndex].track;
    s_StreamerMemBuff.size     = s_trackList[s_trackIndex].length;

    prop.prop = PROP_MEMSRC_SET_BUFF;
    prop.val  = (uintptr_t)&s_StreamerMemBuff;
    streamer_set_property(streamer, prop, true);

    if (curState == STATE_PAUSED)
    {
        pcmHandle.justLoadMetadata = true;
    }

    streamer_set_state(streamer, 0, STATE_PLAYING, true);
}

static void _PreviousTrack()
{
    PipelineState curState = STATE_NULL;
    ELEMENT_PROPERTY_T prop;
    streamer_get_state(streamer, 0, &curState, false);

    if (curState <= STATE_READY)
    {
        LOGD("Streamer needs to be in playing/pause state. Curr state: %d", curState);
        return;
    }

    pcmHandle.changeTrack = true;
    streamer_set_state(streamer, 0, STATE_NULL, true);

    if (s_trackIndex == 0)
    {
        s_trackIndex = s_trackNumber - 1;
    }
    else
    {
        s_trackIndex = s_trackIndex - 1;
    }

    s_StreamerMemBuff.location = (int8_t *)s_trackList[s_trackIndex].track;
    s_StreamerMemBuff.size     = s_trackList[s_trackIndex].length;

    prop.prop = PROP_MEMSRC_SET_BUFF;
    prop.val  = (uintptr_t)&s_StreamerMemBuff;
    streamer_set_property(streamer, prop, true);

    if (curState == STATE_PAUSED)
    {
        pcmHandle.justLoadMetadata = true;
    }

    streamer_set_state(streamer, 0, STATE_PLAYING, true);
}

static void _SaiCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    BaseType_t xHigherPriorityTaskWoken, result;

    result = xSemaphoreGiveFromISR(pcmHandle.MqsSemFreeSlots, &xHigherPriorityTaskWoken);
    if (result != pdFAIL)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/*!
 * @brief Streamer task for communicating messages
 *
 * This function is the entry point of a task that is manually created
 *It listens on a message queue and receives status updates
 * about errors, audio playback state and position.  The application can make
 * use of this data.
 *
 * @param arg Data to be passed to the task
 */
static void STREAMER_MessageTask(void *arg)
{
    static event_smart_tlhmi_t PlayPromptTrackInfo = {0};
    static TrackInfo info                          = {0};

    STREAMER_MSG_T msg;
    bool exit_thread = false;
    osa_status_t ret;

    while (!streamer)
    {
        OSA_TimeDelay(1);
    }

    if (streamer->mq_out == NULL)
    {
        PRINTF("[EAP STREAMER] osa_mq_open failed: %d\r\n");
        return;
    }

    do
    {
        ret = OSA_MsgQGet(&streamer->mq_out, (void *)&msg, osaWaitForever_c);
        if (ret != KOSA_StatusSuccess)
        {
            PRINTF("[EAP STREAMER] OSA_MsgQGet error: %d\r\n", ret);
            continue;
        }

        switch (msg.id)
        {
            case STREAM_MSG_DESTROY_PIPELINE:
                break;
            case STREAM_MSG_ERROR:
                break;
            case STREAM_MSG_EOS:
            {
                LOGI("EOF Recv. Play next song");

                if (s_PromptPlayingRef > 0)
                {
                    event_common_t playPromptDoneEvent = {0};
                    output_event_t output_event        = {0};

                    output_event.eventId   = kOutputEvent_OutputInputNotify;
                    output_event.data      = &playPromptDoneEvent;
                    output_event.copy      = 1;
                    output_event.size      = sizeof(playPromptDoneEvent);
                    output_event.eventInfo = kEventInfo_Remote;

                    playPromptDoneEvent.eventBase.eventId = kEventID_PlayPromptDone;
                    playPromptDoneEvent.data              = (void *)((uint32_t)pcmHandle.promptId);
                    uint8_t fromISR                       = __get_IPSR();
                    if (s_OutputDev_MqsAudio.cap.callback)
                    {
                        s_OutputDev_MqsAudio.cap.callback(s_OutputDev_MqsAudio.id, output_event, fromISR);
                    }

                    s_PromptPlayingRef--;
                    if (s_PromptPlayingRef == 0)
                    {
                        streamer_set_state(streamer, 0, STATE_NULL, true);
                    }
                }
                else
                {
                    vTaskDelay(portTICK_PERIOD_MS * 1000);
                    _NextTrack();
                }
            }
            break;
            case STREAM_MSG_UPDATE_POSITION:
            {
                if (s_PromptPlayingRef <= 0)
                {
                    track_info_t *track_info;
                    PlayPromptTrackInfo.eventBase.eventId = kEventID_MediaPlayer_TrackPosition;
                    track_info                            = &PlayPromptTrackInfo.trackInfo;
                    track_info->offset                    = msg.event_data / 1000;

                    LOGI("Offset is %d from %d", track_info->offset, track_info->songDurationS);
                    _SendTrackEvent(&PlayPromptTrackInfo, kEventInfo_Remote);
                }
            }
            break;
            case STREAM_MSG_UPDATE_DURATION:
                if (s_PromptPlayingRef > 1)
                {
                    /* We destroyed last pipeline before EOF*/
                    s_PromptPlayingRef--;
                }
                break;
            case STREAM_MSG_UPDATE_TRACK_INFO:
            {
                if(s_PromptPlayingRef <= 0)
                {
                    track_info_t *track_info;
                    PlayPromptTrackInfo.eventBase.eventId = kEventID_MediaPlayer_TrackInfo;
                    track_info                            = &PlayPromptTrackInfo.trackInfo;

                    streamer_get_track_info(streamer, 0, &info, false);

                    memset(track_info, 0, sizeof(track_info_t));
                    memcpy(track_info->songArtist, info.artist, 32);
                    memcpy(track_info->songName, info.title, 32);
                    track_info->offset        = 0;
                    track_info->songDurationS = info.duration / 1000;

                    LOGI("New song Name: %s, Artist %s, Duration %d", track_info->songName, track_info->songArtist,
                         track_info->songDurationS);
                    _SendTrackEvent(&PlayPromptTrackInfo, kEventInfo_Remote);
                }
            }
            break;
            case STREAM_MSG_CLOSE_TASK:
                break;
            default:
                break;
        }

    } while (!exit_thread);
}

void streamer_pcm_getparams(pcm_rtos_t *pcm, uint32_t *sample_rate, uint32_t *bit_width, uint8_t *num_channels)
{
#if 1
    *sample_rate  = 48000;
    *bit_width    = 16;
    *num_channels = 1;
#else
    *sample_rate  = pcm->sample_rate;
    *bit_width    = pcm->bit_width;
    *num_channels = pcm->num_channels;
#endif
}

void streamer_pcm_init(void)
{
    sai_config_t saiConfig                               = {0};
    edma_channel_Preemption_config_t dmaPreemptionConfig = {0};
    sai_transceiver_t edmaTxConfig                       = {0};

    BOARD_InitMqsResource();

    BOARD_InitEDMA(MQS_DMA);
    EDMA_CreateHandle(&pcmHandle.dmaTxHandle, MQS_DMA, MQS_SAI_EDMA_TX_CH);

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
    SAI_TransferTxCreateHandleEDMA(MQS_SAI, &pcmHandle.saiTxHandle, _SaiCallback, (void *)&pcmHandle,
                                   &pcmHandle.dmaTxHandle);
    SAI_TransferTxSetConfigEDMA(MQS_SAI, &pcmHandle.saiTxHandle, &edmaTxConfig);

    /* Force bit clock to override standard enablement */
    SAI_TxSetBitClockRate(MQS_SAI, MQS_SAI_CLK_FREQ, kSAI_SampleRate16KHz, kSAI_WordWidth16bits, 2);
}

/* STREAMER API implementation*/
int streamer_pcm_setparams(pcm_rtos_t *pcm,
                           uint32_t sample_rate,
                           uint32_t bit_width,
                           uint8_t num_channels,
                           bool transfer,
                           bool dummy_tx,
                           int volume)
{
    if (NULL != pcm)
    {
        /* NOTE don't support change of parameter because of barge-in */
    }

    pcm->changeTrack = false;

    return 0;
}

int streamer_pcm_mute(pcm_rtos_t *pcm, bool mute)
{
    if (mute)
    {
        /* Disable output of Audio amplifier */
        GPIO_PinWrite(BOARD_MQS_OE_GPIO_PORT, BOARD_MQS_OE_GPIO_PIN, 0);
        g_MQSPlaying = false;
        if (pcm->justLoadMetadata == true)
        {
            /* Finished loading the metadata. Paused */
            pcm->justLoadMetadata = false;
        }

        if (pcm->changeTrack == false)
        {
            event_smart_tlhmi_t trackInfoEvent;
            trackInfoEvent.mediaPlayerInfo.isPlaying = false;
            trackInfoEvent.mediaPlayerInfo.volume    = s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value;
            trackInfoEvent.eventBase.eventId         = kEventID_MediaPlayerInfo;
            _SendTrackEvent(&trackInfoEvent, kEventInfo_Remote);
        }
    }

    return 0;
}

int streamer_pcm_set_volume(pcm_rtos_t *pcm, int volume)
{
    /* Enable output of Audio amplifier */

    if ((pcm == NULL) || (pcm->justLoadMetadata == true))
    {
        return 0;
    }

    if (pcm->changeTrack == true)
    {
        /* Enable MQS output. */
        GPIO_PinWrite(BOARD_MQS_OE_GPIO_PORT, BOARD_MQS_OE_GPIO_PIN, 1);
    }
    else if (pcm->changeVolume == true)
    {
        /* Change the volume only, no need to enable the MQS output. */
        s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value = volume;
        pcm->changeVolume                                      = false;
    }
    else
    {
        /* We were muted */
        s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value = volume;
        GPIO_PinWrite(BOARD_MQS_OE_GPIO_PORT, BOARD_MQS_OE_GPIO_PIN, 1);
    }

    return 0;
}

pcm_rtos_t *streamer_pcm_open(uint32_t num_buffers)
{
    return &pcmHandle;
}

pcm_rtos_t *streamer_pcm_rx_open(uint32_t num_buffers)
{
    return 0;
}

void streamer_pcm_start(pcm_rtos_t *pcm)
{
    /* Interrupts already enabled - nothing to do.
     * App/streamer can begin writing data to SAI. */
    GPIO_PinWrite(BOARD_MQS_OE_GPIO_PORT, BOARD_MQS_OE_GPIO_PIN, 1);
}

void streamer_pcm_clean(pcm_rtos_t *pcm)
{
    /* Stop playback. This will flush the SAI transmit buffers. */
    SAI_TransferTerminateSendEDMA(MQS_SAI, &pcm->saiTxHandle);
}

void streamer_pcm_close(pcm_rtos_t *pcm)
{
    /* Stop playback.  This will flush the SAI transmit buffers. */
    // SAI_TransferAbortSendEDMA(MQS_SAI, &pcm->saiTxHandle);

    /* Disable output of Audio amplifier */
    GPIO_PinWrite(BOARD_MQS_OE_GPIO_PORT, BOARD_MQS_OE_GPIO_PIN, 0);
}

void streamer_pcm_rx_close(pcm_rtos_t *pcm)
{
}

int streamer_pcm_write(pcm_rtos_t *pcm, uint8_t *data, uint32_t size)
{
    bool statusOk                      = true;
    status_t transferStatus            = kStatus_Success;
    static uint8_t mqsAudioPoolSlotIdx = 0;

    if (pcm->justLoadMetadata == true)
    {
        streamer_set_state(streamer, 0, STATE_PAUSED, false);
        return 0;
    }

#if !AMP_LOOPBACK_DISABLED
    static uint8_t mqsFeedbackPoolSlotIdx = 0;
#endif /* !AMP_LOOPBACK_DISABLED */

#if !AMP_LOOPBACK_DISABLED
    if (asrEnabled == 0)
    {
        g_MQSPlaying = true;
    }
#else
    g_MQSPlaying  = true;
#endif /* AMP_LOOPBACK_DISABLED */

    /* A new slot should be available in one PROMPTS_AUDIO_CHUNK_MS.
     * In case there is no empty slot in two PROMPTS_AUDIO_CHUNK_MS, something is wrong. */
    if (xSemaphoreTake(pcmHandle.MqsSemFreeSlots, (TickType_t)(2 * PROMPTS_AUDIO_CHUNK_MS)) != pdTRUE)
    {
        LOGE("[MQS] Playing Failed, played %d samples (%d ms)", size / 2, (size / PROMPTS_AUDIO_CHUNK_MS));
        statusOk = false;
    }

    assert(size <= PROMPTS_AUDIO_CHUNK_SIZE);

    if (statusOk == true)
    {
        _AdjustVolumeMonoToStereo((int16_t *)data, (int16_t *)s_MqsStreamPool[mqsAudioPoolSlotIdx], size / 2);

#if !AMP_LOOPBACK_DISABLED
        if (asrEnabled == 1)
        {
            _AdjustVolumeMonoToMono((int16_t *)(data), (int16_t *)(s_MqsAfeFeedback[mqsFeedbackPoolSlotIdx]), size / 2);
        }
#endif /* !AMP_LOOPBACK_DISABLED */

        pcm->saiTx.data     = s_MqsStreamPool[mqsAudioPoolSlotIdx];
        pcm->saiTx.dataSize = size << 1;

        /* Play this chunk */
        transferStatus = SAI_TransferSendEDMA(MQS_SAI, &pcm->saiTxHandle, &pcm->saiTx);
        if (transferStatus != kStatus_Success)
        {
            LOGE("[MQS] SAI_TransferSendEDMA failed %d for %d samples", transferStatus, pcm->saiTx.dataSize);
            statusOk = false;
        }

#if !AMP_LOOPBACK_DISABLED
        if (asrEnabled == 1)
        {
            /* Notify AFE in order to perform AEC */
            _SpeakerToAfeNotify((int16_t *)s_MqsAfeFeedback[mqsFeedbackPoolSlotIdx], size / 2);
            mqsFeedbackPoolSlotIdx = (mqsFeedbackPoolSlotIdx + 1) % MQS_FEEDBACK_CHUNK_CNT;
        }
#endif /* !AMP_LOOPBACK_DISABLED */

        mqsAudioPoolSlotIdx = (mqsAudioPoolSlotIdx + 1) % MQS_AUDIO_CHUNK_CNT;
    }

    return 0;
}

int streamer_pcm_read(pcm_rtos_t *pcm, uint8_t *data, uint32_t size)
{
    return 0;
}

/* Framework Code */

static void _ConfigMqs(void)
{
    CLOCK_EnableClock(kCLOCK_Mqs); /* Enable MQS hmclk. */

    IOMUXC_MQSEnterSoftwareReset(IOMUXC_GPR, true);  /* Reset MQS. */
    IOMUXC_MQSEnterSoftwareReset(IOMUXC_GPR, false); /* Release reset MQS. */
    IOMUXC_MQSEnable(IOMUXC_GPR, true);              /* Enable MQS. */
    IOMUXC_MQSConfig(IOMUXC_GPR, kIOMUXC_MqsPwmOverSampleRate64,
                     1u); /* 65.54MHz/64/16/2 = 16000Hz
                            Higher frequency PWM involves less low frequency harmonic.*/
}

/*!
 * brief set audio volume for this amp.
 *
 * param volume volume value, support 0 ~ 100, only in multiples of 10, 0 is mute, 100 is the maximum volume value.
 */
static float _AdaptVolume(uint32_t volume)
{
    float volume_adjusted;

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
    volume_adjusted = (-0.0018 * pow(volume, 3) + 0.028 * pow(volume, 2));

    /* Limit speaker volume less than 64dBc when play back calculation song */
    volume_adjusted  *= 0.4;

    return volume_adjusted;
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
        // MQS_SOUND_PLAY_FUNC(promptId, audioBuffer, audioLen, asrEnabled);
    }

    return error;
}

static hal_output_status_t HAL_OutputDev_MqsAudio_InputNotify(const output_dev_t *dev, void *data)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    uint8_t *audioBuffer      = NULL;
    uint32_t audioLen         = 0;
    event_base_t eventBase    = *(event_base_t *)data;
    static MEMSRC_SET_BUFFER_T buff;

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
            PipelineState curState = STATE_NULL;
            streamer_get_state(streamer, 0, &curState, false);

            if (curState > STATE_READY)
            {
                pcmHandle.changeVolume = true;
            }

            s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value = event.speakerVolume.volume;

            ELEMENT_PROPERTY_T prop;
            prop.prop = PROP_AUDIOSINK_SET_VOLUME;
            prop.val  = event.speakerVolume.volume;
            streamer_set_property(streamer, prop, true);
        }
        if (eventBase.respond != NULL)
        {
            eventBase.respond(kEventID_SetSpeakerVolume, &event.speakerVolume, eventResponseStatus, true);
        }
    }
    else if (eventBase.eventId == kEventID_StreamerStop)
    {
        streamer_set_state(streamer, 0, STATE_NULL, true);
    }
    else if (eventBase.eventId == kEventID_StreamerLoad)
    {
        PipelineState curState = STATE_NULL;
        ELEMENT_PROPERTY_T prop;

        streamer_get_state(streamer, 0, &curState, false);

        if (curState <= STATE_READY)
        {
            pcmHandle.justLoadMetadata = true;
            buff.location              = (int8_t *)s_trackList[s_trackIndex].track;
            buff.size                  = s_trackList[s_trackIndex].length;

            prop.prop = PROP_MEMSRC_SET_BUFF;
            prop.val  = (uintptr_t)&buff;
            streamer_set_property(streamer, prop, true);
            streamer_set_state(streamer, 0, STATE_PLAYING, true);
        }
    }
    else if (eventBase.eventId == kEventID_StreamerPlay)
    {
        PipelineState curState = STATE_NULL;
        ELEMENT_PROPERTY_T prop;

        streamer_get_state(streamer, 0, &curState, false);

        LOGI("Streamer: Start playing event");

        if (curState <= STATE_READY)
        {
            buff.location = (int8_t *)s_trackList[s_trackIndex].track;
            buff.size     = s_trackList[s_trackIndex].length;

            prop.prop = PROP_MEMSRC_SET_BUFF;
            prop.val  = (uintptr_t)&buff;

            streamer_set_property(streamer, prop, true);
        }

        streamer_set_state(streamer, 0, STATE_PLAYING, true);
    }
    else if (eventBase.eventId == kEventID_StreamerPause)
    {
        LOGI("Streamer: Stop playing event");
        streamer_set_state(streamer, 0, STATE_PAUSED, true);
    }
    else if (eventBase.eventId == kEventID_StreamerNext)
    {
        LOGI("Streamer: Next song event");
        _NextTrack();
    }
    else if (eventBase.eventId == kEventID_StreamerPrevious)
    {
        LOGI("Streamer: Previous song event");
        _PreviousTrack();
    }
    else if (eventBase.eventId == SET_MULTILINGUAL_CONFIG)
    {
        event_voice_t *pVoiceEvent = (event_voice_t *)data;
        s_Language                 = pVoiceEvent->set_multilingual_config.languages;
        LOGD("[MQS]: Set language %d", s_Language);
    }
    else if (eventBase.eventId == kEventID_PlayPrompt)
    {
        APP_OutputDev_MqsAudio_InputNotifyDecode(dev, data, (void *)&audioBuffer, &audioLen, s_Language);
        if (audioBuffer != NULL && audioLen != 0)
        {
            const event_common_t *eventCommon = ((event_common_t *)data);
            asrEnabled                        = eventCommon->promptInfo.asrEnabled;

            PipelineState curState = STATE_NULL;
            ELEMENT_PROPERTY_T prop;

            streamer_set_state(streamer, 0, STATE_NULL, true);
            streamer_get_state(streamer, 0, &curState, false);

            if (curState <= STATE_READY)
            {
                s_PromptPlayingRef++;
                LOGI("Streamer: Start playing event");
                buff.location = (int8_t *)audioBuffer;
                buff.size     = audioLen;

                prop.prop = PROP_MEMSRC_SET_BUFF;
                prop.val  = (uintptr_t)&buff;

                pcmHandle.promptId = eventCommon->promptInfo.id;
                streamer_set_property(streamer, prop, true);
                streamer_set_state(streamer, 0, STATE_PLAYING, true);
            }
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
    STREAMER_CREATE_PARAM params;
    ELEMENT_PROPERTY_T prop;
    osa_task_def_t thread_attr;
    int ret;
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    if (FWK_OutputManager_RegisterEventHandler(dev, &s_MqsAudioHandler) != 0)
    {
        LOGE("HAL_OutputDev_MqsAudio_Start failed - FWK_OutputManager_RegisterEventHandler");
        error = kStatus_HAL_OutputError;
    }

    if (error == kStatus_HAL_OutputSuccess)
    {
        _ConfigMqs();

        pcmHandle.MqsSemFreeSlots = xSemaphoreCreateCounting(MQS_AUDIO_CHUNK_CNT, MQS_AUDIO_CHUNK_CNT);
        if (pcmHandle.MqsSemFreeSlots == NULL)
        {
            LOGE("HAL_OutputDev_MqsAudio_Start failed - xSemaphoreCreateCounting");
            error = kStatus_HAL_OutputError;
        }
    }

    /* Create message process thread */
    thread_attr.tpriority = OSA_PRIORITY_HIGH + 1;
    thread_attr.tname     = (uint8_t *)STREAMER_MESSAGE_TASK_NAME;
    thread_attr.pthread   = &STREAMER_MessageTask;
    thread_attr.stacksize = STREAMER_MESSAGE_TASK_STACK_SIZE;
    ret                   = OSA_TaskCreate(&msg_thread, &thread_attr, NULL);

    if (KOSA_StatusSuccess != ret)
    {
        return kStatus_Fail;
    }

    strcpy(params.out_mq_name, APP_STREAMER_MSG_QUEUE);
    params.stack_size    = STREAMER_TASK_STACK_SIZE;
    params.pipeline_type = STREAM_PIPELINE_MEM;
    params.task_name     = STREAMER_TASK_NAME;
    params.in_dev_name   = "";
    params.out_dev_name  = "";

    streamer = streamer_create(&params);
    if (!streamer)
    {
        return kStatus_HAL_OutputError;
    }

    prop.prop = PROP_DECODER_DECODER_TYPE;
    prop.val  = DECODER_TYPE_MP3;
    streamer_set_property(streamer, prop, true);

    prop.prop = PROP_MEMSRC_SET_CHUNK_SIZE;
    prop.val  = PROMPTS_AUDIO_CHUNK_SIZE / 2;
    streamer_set_property(streamer, prop, true);

    prop.prop = PROP_AUDIOSINK_SET_VOLUME;
    prop.val  = s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value;
    streamer_set_property(streamer, prop, true);

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

__attribute__((weak)) int APP_OutputDev_MqsAudio_GetSongs(track_element **songs, uint32_t *number)
{
    return 0;
}

static hal_output_status_t HAL_OutputDev_MqsAudio_Init(output_dev_t *dev, output_dev_callback_t callback)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    dev->cap.callback = callback;
    /* Initialize OSA*/
    OSA_Init();

    /* Initialize streamer PCM management library. */
    streamer_pcm_init();

    s_trackList   = NULL;
    s_trackNumber = 0;
    APP_OutputDev_MqsAudio_GetSongs(&s_trackList, &s_trackNumber);

    if ((s_trackList == NULL) || (s_trackNumber == 0))
    {
        LOGE("[MQS Streamer]:Failed to load songs");
        error = kStatus_HAL_OutputError;
    }

    return error;
}

static status_t _GetVolume(char *valueToString)
{
    itoa(s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value, valueToString, 10);
    strcat(valueToString, "%");
    return kStatus_Success;
}

int HAL_OutputDev_MqsAudio_Streamer_Register()
{
    int error = 0;
    LOGD("HAL_OutputDev_MqsAudio_Register");
    error = FWK_OutputManager_DeviceRegister(&s_OutputDev_MqsAudio);
    return error;
}
#endif /* ENABLE_OUTPUT_DEV_MqsAudio */
