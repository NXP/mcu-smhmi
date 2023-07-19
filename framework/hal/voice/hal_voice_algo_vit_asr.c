/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board_define.h"
#ifdef ENABLE_VIT_ASR
#include "hal_voice_algo_asr_local.h"

#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_voice_algo_manager.h"
#include "hal_event_descriptor_voice.h"
#include "PL_platformTypes_CortexM.h"
#include "hal_audio_defs.h"
#include "VIT.h"

#include "IndexCommands.h"
#include "local_voice_model_vit.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if ENABLE_COFFEE_MACHINE
#define CURRENT_DEMO ASR_CMD_COFFEE_MACHINE
#elif ENABLE_ELEVATOR
#define CURRENT_DEMO ASR_CMD_ELEVATOR
#elif ENABLE_HOME_PANEL
#define CURRENT_DEMO ASR_CMD_HP_MAIN_MENU
#else
#error "CURRENT_DEMO is not defined"
#endif

#define NUMBER_OF_CHANNELS ASR_INPUT_CHANNELS
#if (NUMBER_OF_CHANNELS == 1)
#define VIT_OPERATING_MODE_WW  (VIT_WAKEWORD_ENABLE)
#define VIT_OPERATING_MODE_CMD (VIT_VOICECMD_ENABLE)
#else
#define VIT_OPERATING_MODE_WW  (VIT_WAKEWORD_ENABLE | VIT_AFE_ENABLE)
#define VIT_OPERATING_MODE_CMD (VIT_VOICECMD_ENABLE | VIT_AFE_ENABLE)
#endif /* NUMBER_OF_CHANNELS */

#define VIT_MIC1_MIC2_DISTANCE 60
#define VIT_MIC1_MIC3_DISTANCE 0

#define DEVICE_ID VIT_IMXRT1170

#define MEMORY_ALIGNMENT 8 // in bytes

/* VIT memory pools sizes */
#define FAST_MEMORY_SIZE_BYTES  (45000)
#define SLOW_MEMORY_SIZE_BYTES  (225000)
#define MODEL_MEMORY_SIZE_BYTES (450000)

/* Configure the detection period in second for each command
   VIT will return UNKNOWN if no command is recognized during this time span.
   When only activating one mode, WWD or CMD, even timeout event trigger,
   no model switching happens automatically. Set it to 60s aligned with UI session.
 */
#define VIT_COMMAND_TIME_SPAN 8

typedef enum _asr_session
{
    ASR_SESSION_STOPPED,
    ASR_SESSION_WAKE_WORD,
    ASR_SESSION_VOICE_COMMAND,
} asr_session_t;
static asr_session_t s_asrSession = ASR_SESSION_STOPPED;

static VIT_Handle_t VITHandle = PL_NULL;      // VIT handle pointer
static VIT_InstanceParams_st VITInstParams;   // VIT instance parameters structure
static VIT_ControlParams_st VITControlParams; // VIT control parameters structure
static PL_MemoryTable_st VITMemoryTable;      // VIT memory table descriptor
static PL_BOOL InitPhase_Error = PL_FALSE;
static PL_INT8 *pMemory[PL_NR_MEMORY_REGIONS];

#if SELF_WAKE_UP_PROTECTION
static VIT_Handle_t VITHandleSelfWake = PL_NULL; // VIT handle pointer for self wake up engine
static PL_MemoryTable_st VITMemoryTableSelfWake;
static PL_INT8 *pMemorySelfWake[PL_NR_MEMORY_REGIONS];
#endif /* SELF_WAKE_UP_PROTECTION */

VIT_StatusParams_st VIT_StatusParams_Buffer;
/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t s_vitFastMemoryUsed = 0;
AT_NONCACHEABLE_SECTION_ALIGN_DTC(static int8_t s_vitFastMemory[FAST_MEMORY_SIZE_BYTES], 8);
static uint32_t s_vitSlowMemoryUsed = 0;
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static int8_t s_vitSlowMemory[SLOW_MEMORY_SIZE_BYTES], 8);
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static int8_t s_vitModelMemory[MODEL_MEMORY_SIZE_BYTES], 64);

#if SELF_WAKE_UP_PROTECTION
static uint32_t s_vitFastMemoryUsedSelfWake = 0;
AT_NONCACHEABLE_SECTION_ALIGN_DTC(static int8_t s_vitFastMemorySelfWake[FAST_MEMORY_SIZE_BYTES], 8);
static uint32_t s_vitSlowMemoryUsedSelfWake = 0;
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static int8_t s_vitSlowMemorySelfWake[SLOW_MEMORY_SIZE_BYTES], 8);
#endif /* SELF_WAKE_UP_PROTECTION */

typedef enum _cmd_state
{
    kWwConfirmed,
    kWwRejected,
    kWwNotSure,
} cmd_state_t;

typedef struct _asr_voice_param
{
    asr_voice_control_t voiceControl;
    asr_voice_config_t voiceConfig;
    asr_inference_result_t voiceResult;
} asr_voice_param_t;

static asr_voice_param_t s_AsrEngine;

static void voice_algo_asr_result_notify(asr_inference_result_t *result, uint32_t utteranceLength);

/* Performance Statistics. */
extern volatile uint32_t s_afeDataProcessed;

#ifdef ENABLE_OUTPUT_DEV_AudioDump
extern void postMarkCleanStreamSKIPPED(uint32_t *seq, uint8_t cnt);
extern void postMarkCleanStreamConsumedByWW(uint32_t *seq, uint8_t cnt);
extern void postMarkCleanStreamConsumedByCMD(uint32_t *seq, uint8_t cnt);
#endif /* ENABLE_OUTPUT_DEV_AudioDump */

/*******************************************************************************
 * Code
 ******************************************************************************/
static int VIT_Deinit()
{
    VIT_ReturnStatus_en VIT_Status;

    VIT_Status = VIT_GetMemoryTable(VITHandle, &VITMemoryTable, &VITInstParams);

    if (VIT_Status != VIT_SUCCESS)
    {
        LOGD("VIT_GetMemoryTable error: %d\r\n", VIT_Status);
        return VIT_Status;
    }

    // Free the MEM tables
    for (int i = 0; i < PL_NR_MEMORY_REGIONS; i++)
    {
        if (VITMemoryTable.Region[i].Size != 0)
        {
            memset(pMemory[i], 0, VITMemoryTable.Region[i].Size);
            pMemory[i] = NULL;
        }

#if SELF_WAKE_UP_PROTECTION
        if (VITMemoryTableSelfWake.Region[i].Size != 0)
        {
            memset(pMemorySelfWake[i], 0, VITMemoryTableSelfWake.Region[i].Size);
            pMemorySelfWake[i] = NULL;
        }
#endif /* SELF_WAKE_UP_PROTECTION */
    }
    return VIT_Status;
}

static void asr_set_state(asr_session_t state)
{
    VIT_ReturnStatus_en VIT_Status = VIT_ERROR_UNDEFINED;
    s_asrSession                   = state;

    switch (state)
    {
        case ASR_SESSION_STOPPED:
            LOGD("[ASR] Session stopped");
            VITControlParams.OperatingMode = VIT_LPVAD_ENABLE;
            break;

        case ASR_SESSION_WAKE_WORD:
            LOGD("[ASR] Session waiting for Wake Word");
            VITControlParams.OperatingMode = VIT_OPERATING_MODE_WW;
            break;

        case ASR_SESSION_VOICE_COMMAND:
            LOGD("[ASR] Session waiting for Voice Command");
            VITControlParams.OperatingMode = VIT_OPERATING_MODE_CMD;
            break;

        default:
            LOGE("Unknown state %d", state);
            break;
    }

    if (s_asrSession != ASR_SESSION_STOPPED)
    {
        VITControlParams.Feature_LowRes    = PL_FALSE;
        VITControlParams.Command_Time_Span = VIT_COMMAND_TIME_SPAN;

        VIT_Status = VIT_SetControlParameters(VITHandle, &VITControlParams);
#if SELF_WAKE_UP_PROTECTION
        VIT_Status = VIT_SetControlParameters(VITHandleSelfWake, &VITControlParams);
#endif /* SELF_WAKE_UP_PROTECTION */

        if (VIT_Status != VIT_SUCCESS)
        {
            LOGE("[ASR] %d state failed %d", state, VIT_Status);
        }
    }
}

#if SELF_WAKE_UP_PROTECTION
static VIT_ReturnStatus_en initialize_asr_self_wake_up()
{
    VIT_ReturnStatus_en VIT_Status = VIT_SUCCESS;
    VIT_Status                     = VIT_SetModel((const PL_UINT8 *)s_vitModelMemory, VIT_MODEL_IN_FAST_MEM);
    /*
     *   VIT get memory table: Get size info per memory type
     */
    VIT_Status = VIT_GetMemoryTable(PL_NULL, // VITHandle param should be NULL
                                    &VITMemoryTableSelfWake, &VITInstParams);
    if (VIT_Status != VIT_SUCCESS)
    {
        LOGE("VIT_GetMemoryTable error: %d\r\n", VIT_Status);
        return VIT_Status;
    }
    /*
     *   Reserve memory space: Malloc for each memory type
     */
    s_vitSlowMemoryUsedSelfWake = 0;
    s_vitFastMemoryUsedSelfWake = 0;
    for (int i = 0; i < PL_NR_MEMORY_REGIONS; i++)
    {
        /* Log the memory size */
        if (VITMemoryTableSelfWake.Region[i].Size != 0)
        {
            /* reserve memory space
               NB: VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_FAST_DATA] should be allocated
               in the fastest memory of the platform (when possible) - this is not the case in this example.
             */
            if (VITMemoryTableSelfWake.Region[i].Type == PL_PERSISTENT_SLOW_DATA)
            {
                pMemorySelfWake[i] = &s_vitSlowMemorySelfWake[s_vitSlowMemoryUsedSelfWake];
                s_vitSlowMemoryUsedSelfWake += VITMemoryTableSelfWake.Region[i].Size;
                s_vitSlowMemoryUsedSelfWake += MEMORY_ALIGNMENT - (s_vitSlowMemoryUsedSelfWake % MEMORY_ALIGNMENT);
            }
            else
            {
                pMemorySelfWake[i] = &s_vitFastMemorySelfWake[s_vitFastMemoryUsedSelfWake];
                s_vitFastMemoryUsedSelfWake += VITMemoryTableSelfWake.Region[i].Size;
                s_vitFastMemoryUsedSelfWake += MEMORY_ALIGNMENT - (s_vitFastMemoryUsedSelfWake % MEMORY_ALIGNMENT);
            }
            VITMemoryTableSelfWake.Region[i].pBaseAddress = (void *)pMemorySelfWake[i];
        }
    }
    /*
     *    Create VIT Instance for self wake engine
     */
    VITHandleSelfWake = PL_NULL; // force to null address for correct memory initialization
    VIT_Status        = VIT_GetInstanceHandle(&VITHandleSelfWake, &VITMemoryTableSelfWake, &VITInstParams);
    if (VIT_Status != VIT_SUCCESS)
    {
        InitPhase_Error = PL_TRUE;
        LOGD("VIT_GetInstanceHandle error: %d\r\n", VIT_Status);
    }

    /*
     *    Test the reset (OPTIONAL)
     */
    if (!InitPhase_Error)
    {
        VIT_Status = VIT_ResetInstance(VITHandleSelfWake);
        if (VIT_Status != VIT_SUCCESS)
        {
            InitPhase_Error = PL_TRUE;
            LOGD("VIT_ResetInstance error: %d\r\n", VIT_Status);
        }
    }

    return VIT_Status;
}
#endif /* SELF_WAKE_UP_PROTECTION */

#ifdef ASR_TO_AFE_PROCESSED_NOTIFY
/*!
 * @brief Notify the AFE about the finish of the run.
 */
static void voice_algo_asr_run_notify(hal_valgo_status_t status);
#endif /* ASR_TO_AFE_PROCESSED_NOTIFY */

static VIT_ReturnStatus_en initialize_asr()
{
    VIT_ReturnStatus_en VIT_Status = VIT_ERROR_UNDEFINED;

    uint8_t *asrModelAddr = NULL;
    uint32_t asrModelSize = 0;

    get_voice_model(&asrModelAddr, &asrModelSize, s_AsrEngine.voiceConfig.multilingual, s_AsrEngine.voiceConfig.demo);

    if ((asrModelAddr == NULL) || (asrModelSize == 0) || (asrModelSize > MODEL_MEMORY_SIZE_BYTES))
    {
        LOGE("VIT get model for %d failed. Pointer %p size %d for demo %d.", s_AsrEngine.voiceConfig.demo, asrModelAddr,
             asrModelSize, s_AsrEngine.voiceConfig.demo);
        return VIT_DUMMY_ERROR;
    }
    else
    {
        memcpy(s_vitModelMemory, asrModelAddr, asrModelSize);
        VIT_Status = VIT_SetModel((const PL_UINT8 *)s_vitModelMemory, VIT_MODEL_IN_FAST_MEM);
        if (VIT_Status != VIT_SUCCESS)
        {
            LOGE("VIT_SetModel error: %d\r\n", VIT_Status);
            return VIT_Status;
        }
    }

    /*
     *   Configure VIT Instance Parameters
     */
    VITInstParams.SampleRate_Hz   = VIT_SAMPLE_RATE;
    VITInstParams.SamplesPerFrame = VIT_SAMPLES_PER_30MS_FRAME;
    VITInstParams.NumberOfChannel = NUMBER_OF_CHANNELS;
    VITInstParams.DeviceId        = DEVICE_ID;
    VITInstParams.APIVersion      = VIT_API_VERSION;

    /*
     *   VIT get memory table: Get size info per memory type
     */
    VIT_Status = VIT_GetMemoryTable(PL_NULL, // VITHandle param should be NULL
                                    &VITMemoryTable, &VITInstParams);
    if (VIT_Status != VIT_SUCCESS)
    {
        LOGE("VIT_GetMemoryTable error: %d\r\n", VIT_Status);
        return VIT_Status;
    }

    /*
     *   Reserve memory space: Malloc for each memory type
     */
    s_vitSlowMemoryUsed = 0;
    s_vitFastMemoryUsed = 0;
    for (int i = 0; i < PL_NR_MEMORY_REGIONS; i++)
    {
        /* Log the memory size */
        if (VITMemoryTable.Region[i].Size != 0)
        {
            /* reserve memory space
               NB: VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_FAST_DATA] should be allocated
               in the fastest memory of the platform (when possible) - this is not the case in this example.
             */
            if (VITMemoryTable.Region[i].Type == PL_PERSISTENT_SLOW_DATA)
            {
                pMemory[i] = &s_vitSlowMemory[s_vitSlowMemoryUsed];
                s_vitSlowMemoryUsed += VITMemoryTable.Region[i].Size;
                s_vitSlowMemoryUsed += MEMORY_ALIGNMENT - (s_vitSlowMemoryUsed % MEMORY_ALIGNMENT);
            }
            else
            {
                pMemory[i] = &s_vitFastMemory[s_vitFastMemoryUsed];
                s_vitFastMemoryUsed += VITMemoryTable.Region[i].Size;
                s_vitFastMemoryUsed += MEMORY_ALIGNMENT - (s_vitFastMemoryUsed % MEMORY_ALIGNMENT);
            }
            VITMemoryTable.Region[i].pBaseAddress = (void *)pMemory[i];
        }
    }

    if (s_vitSlowMemoryUsed > SLOW_MEMORY_SIZE_BYTES)
    {
        LOGE("VIT slow memory buffer is too small %d < %d.", SLOW_MEMORY_SIZE_BYTES, s_vitSlowMemoryUsed);
        vTaskDelay(pdMS_TO_TICKS(100));
        while (1)
            ;
    }
    if (s_vitFastMemoryUsed > FAST_MEMORY_SIZE_BYTES)
    {
        LOGE("VIT fast memory buffer is too small %d < %d.", FAST_MEMORY_SIZE_BYTES, s_vitFastMemoryUsed);
        vTaskDelay(pdMS_TO_TICKS(100));
        while (1)
            ;
    }

    /*
     *    Create VIT Instance
     */
    VITHandle  = PL_NULL; // force to null address for correct memory initialization
    VIT_Status = VIT_GetInstanceHandle(&VITHandle, &VITMemoryTable, &VITInstParams);
    if (VIT_Status != VIT_SUCCESS)
    {
        InitPhase_Error = PL_TRUE;
        LOGE("VIT_GetInstanceHandle error: %d\r\n", VIT_Status);
        return VIT_Status;
    }

    /*
     *    Test the reset (OPTIONAL)
     */
    if (!InitPhase_Error)
    {
        VIT_Status = VIT_ResetInstance(VITHandle);
        if (VIT_Status != VIT_SUCCESS)
        {
            InitPhase_Error = PL_TRUE;
            LOGD("VIT_ResetInstance error: %d\r\n", VIT_Status);
        }
    }
#if SELF_WAKE_UP_PROTECTION
    VIT_Status = initialize_asr_self_wake_up();
#endif /* SELF_WAKE_UP_PROTECTION */

    /*
     *   Set and Apply VIT control parameters
     */
    asr_set_state(ASR_SESSION_STOPPED);

    return VIT_Status;
}

static void VIT_ModelInfo(void)
{
    VIT_ReturnStatus_en VIT_Status;
    VIT_ModelInfo_st Model_Info;

    LOGD("  ");

    /* VIT Model information */
    VIT_Status = VIT_GetModelInfo(&Model_Info);
    if (VIT_Status == VIT_SUCCESS)
    {
        LOGD("--- VIT Model info ---");
        if (Model_Info.pLanguage != PL_NULL)
        {
            LOGD("Language: %s\r\n", Model_Info.pLanguage);
        }

        if (Model_Info.WW_VoiceCmds_Strings)
        {
            const char *ptr;

            ptr = Model_Info.pWakeWord_List;

            LOGD("Wake Words count: %d", Model_Info.NbOfWakeWords);
            if (ptr != PL_NULL)
            {
                for (uint16_t i = 0; i < Model_Info.NbOfWakeWords; i++)
                {
                    LOGD("    '%s'", ptr);
                    ptr += strlen(ptr) + 1;
                }
            }

            LOGD("Voice Commands count: %d", Model_Info.NbOfVoiceCmds);
            ptr = Model_Info.pVoiceCmds_List;
            if (ptr != PL_NULL)
            {
                for (uint16_t i = 0; i < Model_Info.NbOfVoiceCmds; i++)
                {
                    LOGD("    '%s'", ptr);
                    ptr += strlen(ptr) + 1;
                }
            }
        }
        else
        {
            LOGE("Number of Commands: Not available");
        }
    }
    else
    {
        LOGE("VIT_GetModelInfo error: %d\r\n", VIT_Status);
    }

    LOGD("  ");

    /* Public call to VIT_GetStatusParameters */
    VIT_StatusParams_st *pVIT_StatusParam_Buffer = (VIT_StatusParams_st *)&VIT_StatusParams_Buffer;

    VIT_GetStatusParameters(VITHandle, pVIT_StatusParam_Buffer, sizeof(VIT_StatusParams_Buffer));
    LOGD(" VIT Status Params");
    LOGD(" VIT LIB Release   = 0x%04x", pVIT_StatusParam_Buffer->VIT_LIB_Release);
    LOGD(" VIT Model Release = 0x%04x", pVIT_StatusParam_Buffer->VIT_MODEL_Release);
    LOGD(" VIT Features supported by the lib = 0x%04x", pVIT_StatusParam_Buffer->VIT_Features_Supported);
    LOGD(" VIT Features Selected             = 0x%04x", pVIT_StatusParam_Buffer->VIT_Features_Selected);
    LOGD(" Number of channels supported by VIT lib = %d", pVIT_StatusParam_Buffer->NumberOfChannels_Supported);
    LOGD(" Device Selected : device id = %d", pVIT_StatusParam_Buffer->Device_Selected);
    LOGD("  ");
}

static const char *get_language_str(asr_language_t language)
{
    const char *language_str = NULL;

    switch (language)
    {
        case ASR_ENGLISH:
            language_str = "ENGLISH";
            break;
        case ASR_CHINESE:
            language_str = "CHINESE";
            break;
        case ASR_GERMAN:
            language_str = "GERMAN";
            break;
        case ASR_FRENCH:
            language_str = "FRENCH";
            break;

        default:
            language_str = "-----";
            break;
    }

    return language_str;
}

static asr_language_t getFirstAvailableLanguage(void)
{
    asr_language_t lang = UNDEFINED_LANGUAGE;

    if ((ASR_ENGLISH & s_AsrEngine.voiceConfig.multilingual) != 0)
    {
        lang = ASR_ENGLISH;
    }
    else if ((ASR_CHINESE & s_AsrEngine.voiceConfig.multilingual) != 0)
    {
        lang = ASR_CHINESE;
    }
    else if ((ASR_GERMAN & s_AsrEngine.voiceConfig.multilingual) != 0)
    {
        lang = ASR_GERMAN;
    }
    else if ((ASR_FRENCH & s_AsrEngine.voiceConfig.multilingual) != 0)
    {
        lang = ASR_FRENCH;
    }
    else
    {
        lang = UNDEFINED_LANGUAGE;
        LOGE("No available languages");
    }

    return lang;
}

static void print_enabled_languages(asr_language_t multilingual)
{
    char buffer_str[MAX_NUM_LANGUAGES * 3 + 1] = {0};
    uint8_t buffer_idx                         = 0;

    if (multilingual & ASR_ENGLISH)
    {
        strcpy(&buffer_str[buffer_idx], " en");
        buffer_idx += strlen(" en");
    }
    if (multilingual & ASR_CHINESE)
    {
        strcpy(&buffer_str[buffer_idx], " cn");
        buffer_idx += strlen(" cn");
    }
    if (multilingual & ASR_GERMAN)
    {
        strcpy(&buffer_str[buffer_idx], " de");
        buffer_idx += strlen(" de");
    }
    if (multilingual & ASR_FRENCH)
    {
        strcpy(&buffer_str[buffer_idx], " fr");
        buffer_idx += strlen(" fr");
    }

    LOGD("[ASR]: Enabled languages:%s", buffer_str);
}

static bool isLanguageSupported(asr_language_t language)
{
    bool ret = false;

    switch (language)
    {
        case ASR_ENGLISH:
            ret = true;
            break;
        case ASR_CHINESE:
            ret = true;
            break;
        case ASR_GERMAN:
            ret = true;
            break;
        case ASR_FRENCH:
            ret = true;
            break;

        default:
            ret = false;
            break;
    }

    return ret;
}

static hal_valgo_status_t voice_algo_dev_asr_init(voice_algo_dev_t *dev, valgo_dev_callback_t callback, void *param)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;

    dev->cap.callback = callback;

    s_AsrEngine.voiceConfig.demo            = CURRENT_DEMO;
    s_AsrEngine.voiceConfig.multilingual    = DEFAULT_ACTIVE_LANGUAGE;
    s_AsrEngine.voiceConfig.currentLanguage = getFirstAvailableLanguage();

    /* Initialize the ASR engine */
    if (initialize_asr() == VIT_SUCCESS)
    {
        LOGD("[ASR] VIT initialized");
    }
    else
    {
        ret = kStatus_HAL_ValgoInitError;
    }

    return ret;
}

/*!
 * @brief Check if the detected command was not a "fake" one triggered by the device's speaker.
 */
cmd_state_t confirmDetectedCommand(bool realCmdDetected, bool fakeCmdDetected, void *ampPlaying, uint32_t *delayMs)
{
    static uint32_t realWakeWordDuration = 0;

    cmd_state_t cmdConfirmed = kWwNotSure;

    /* This function is called once per 30ms. */
    *delayMs = realWakeWordDuration * 30;

#if SELF_WAKE_UP_PROTECTION
    if (ampPlaying != NULL)
    {
        if (realWakeWordDuration == 0)
        {
            if (realCmdDetected)
            {
                realWakeWordDuration = 1;
            }
        }
        else
        {
            realWakeWordDuration++;
        }

        if (fakeCmdDetected)
        {
            cmdConfirmed         = kWwRejected;
            realWakeWordDuration = 0;
        }
        else if (realWakeWordDuration > 20)
        {
            cmdConfirmed         = kWwConfirmed;
            realWakeWordDuration = 0;
        }
        else
        {
            cmdConfirmed = kWwNotSure;
        }
    }
    else
#endif /* SELF_WAKE_UP_PROTECTION */
    {
        if (realCmdDetected || (realWakeWordDuration > 0))
        {
            cmdConfirmed         = kWwConfirmed;
            realWakeWordDuration = 0;
        }
        else
        {
            cmdConfirmed = kWwNotSure;
        }
    }

    return cmdConfirmed;
}

/*!
 * @brief ASR main task
 */
static hal_valgo_status_t voice_algo_dev_asr_run(const voice_algo_dev_t *dev, void *data)
{
    hal_valgo_status_t status = kStatus_HAL_ValgoSuccess;
    VIT_ReturnStatus_en VIT_Status;
    static VIT_WakeWord_st s_WakeWord;
    static VIT_VoiceCommand_st s_VoiceCommand;
    static VIT_WakeWord_st s_WakeWordSelfWake;
    static VIT_VoiceCommand_st s_VoiceCommandSelfWake;
    VIT_DetectionStatus_en VIT_DetectionResults         = VIT_NO_DETECTION;
    VIT_DetectionStatus_en VIT_DetectionResultsSelfWake = VIT_NO_DETECTION;
    uint32_t cmdConfirmedDelayMs                        = 0;
    cmd_state_t cmdConfirmed                            = kWwNotSure;
    int16_t *cleanSound                                 = NULL;
    int16_t *speakerSound                               = NULL;
    bool realCmdDetected                                = false;
    bool fakeCmdDetected                                = false;

    msg_payload_t *audioIn = (msg_payload_t *)data;
    if ((audioIn->data != NULL) && (audioIn->size == ASR_INPUT_SAMPLES))
    {
        cleanSound   = audioIn->data;
        speakerSound = NULL;
    }
#if SELF_WAKE_UP_PROTECTION
    else if ((audioIn->data != NULL) && (audioIn->size == (ASR_INPUT_SAMPLES * 2)))
    {
        cleanSound   = audioIn->data;
        speakerSound = &(((int16_t *)audioIn->data)[ASR_INPUT_SAMPLES]);
    }
#endif /* SELF_WAKE_UP_PROTECTION */
    else
    {
        status = kStatus_HAL_ValgoError;
        LOGE("[ASR] Received invalid audio packet: addr=0x%X, size=%d", (uint32_t)audioIn->data, audioIn->size);
    }

    if (status == kStatus_HAL_ValgoSuccess)
    {
        uint32_t *dumpMark = audioIn->data_marks;
        if (s_asrSession == ASR_SESSION_STOPPED)
        {
            /* Skip Voice Processing for Stopped state. */
#ifdef ENABLE_OUTPUT_DEV_AudioDump
            postMarkCleanStreamSKIPPED(dumpMark, ASR_INPUT_FRAMES);

#endif /* ENABLE_OUTPUT_DEV_AudioDump */
        }
        else
        {
#ifdef ENABLE_OUTPUT_DEV_AudioDump
            if (VITControlParams.OperatingMode == VIT_OPERATING_MODE_WW)
            {
                postMarkCleanStreamConsumedByWW(dumpMark, ASR_INPUT_FRAMES);
            }
            else
            {
                postMarkCleanStreamConsumedByCMD(dumpMark, ASR_INPUT_FRAMES);
            }
#endif /* ENABLE_OUTPUT_DEV_AudioDump */
            VIT_Status = VIT_Process(VITHandle, cleanSound, &VIT_DetectionResults);

            if (VIT_Status != VIT_SUCCESS)
            {
                LOGE("VIT_Process error: %d\r\n", VIT_Status);
                status = kStatus_HAL_ValgoError;
            }
            else if ((VIT_DetectionResults == VIT_WW_DETECTED) || (VIT_DetectionResults == VIT_VC_DETECTED))
            {
                if (VIT_DetectionResults == VIT_WW_DETECTED)
                {
                    VIT_Status = VIT_GetWakeWordFound(VITHandle, &s_WakeWord);
                    if (VIT_Status != VIT_SUCCESS)
                    {
                        LOGE("VIT_GetWakeWordFound error: %d\r\n", VIT_Status);
                        status = kStatus_HAL_ValgoError;
                    }
                    else if (s_WakeWord.Id > 0)
                    {
                        realCmdDetected                = true;
                        s_AsrEngine.voiceResult.status = ASR_WW_DETECT;
                        /* VIT supports only one language at a time so it does not offer the detected
                         * language because it is the one used during initialisation. */
                        s_AsrEngine.voiceResult.language = s_AsrEngine.voiceConfig.multilingual;
                    }
                }
                else if (VIT_DetectionResults == VIT_VC_DETECTED)
                {
                    /* Retrieve id of the Voice Command detected
                       String of the Command can also be retrieved (when WW and CMDs strings are integrated in Model) */
                    VIT_Status = VIT_GetVoiceCommandFound(VITHandle, &s_VoiceCommand);
                    if (VIT_Status != VIT_SUCCESS)
                    {
                        LOGE("VIT_GetVoiceCommandFound error: %d\r\n", VIT_Status);
                        status = kStatus_HAL_ValgoError;
                    }
                    else if (s_VoiceCommand.Id > 0)
                    {
                        realCmdDetected                   = true;
                        s_AsrEngine.voiceResult.keywordID = get_action_index_from_keyword(
                            s_AsrEngine.voiceConfig.multilingual, s_AsrEngine.voiceConfig.demo, s_VoiceCommand.Id);

                        s_AsrEngine.voiceResult.status = ASR_CMD_DETECT;
                        /* VIT supports only one language at a time so it does not offer the detected
                         * language because it is the one used during initialization. */
                        s_AsrEngine.voiceResult.language = s_AsrEngine.voiceConfig.multilingual;
                    }
                }
            }
#if SELF_WAKE_UP_PROTECTION
            if (speakerSound != NULL)
            {
                VIT_Status = VIT_Process(VITHandleSelfWake, speakerSound, &VIT_DetectionResultsSelfWake);

                if (VIT_Status != VIT_SUCCESS)
                {
                    LOGE("VIT_Process error: %d\r\n", VIT_Status);
                    status = kStatus_HAL_ValgoError;
                }
                else if ((VIT_DetectionResultsSelfWake == VIT_WW_DETECTED) ||
                         (VIT_DetectionResultsSelfWake == VIT_VC_DETECTED))
                {
                    if (VIT_DetectionResultsSelfWake == VIT_WW_DETECTED)
                    {
                        VIT_Status = VIT_GetWakeWordFound(VITHandleSelfWake, &s_WakeWordSelfWake);
                        if (VIT_Status != VIT_SUCCESS)
                        {
                            LOGE("VIT_GetWakeWordFound error: %d\r\n", VIT_Status);
                            status = kStatus_HAL_ValgoError;
                        }
                        else if (s_WakeWordSelfWake.Id > 0)
                        {
                            fakeCmdDetected = true;
                        }
                    }
                    else if (VIT_DetectionResultsSelfWake == VIT_VC_DETECTED)
                    {
                        VIT_Status = VIT_GetVoiceCommandFound(VITHandleSelfWake, &s_VoiceCommandSelfWake);
                        if (VIT_Status != VIT_SUCCESS)
                        {
                            LOGE("VIT_GetVoiceCommandFound error: %d\r\n", VIT_Status);
                            status = kStatus_HAL_ValgoError;
                        }
                        else if (s_VoiceCommandSelfWake.Id > 0)
                        {
                            fakeCmdDetected = true;
                        }
                    }
                }
            }
#endif /* SELF_WAKE_UP_PROTECTION */
            cmdConfirmed = confirmDetectedCommand(realCmdDetected, fakeCmdDetected, speakerSound, &cmdConfirmedDelayMs);

            if (cmdConfirmed == kWwConfirmed)
            {
                if (s_AsrEngine.voiceResult.status == ASR_WW_DETECT)
                {
                    LOGD("[ASR] Wake Word: %s(%d)", (s_WakeWord.pName == PL_NULL) ? "UNDEF" : s_WakeWord.pName,
                         s_WakeWord.Id);

                    voice_algo_asr_result_notify(&s_AsrEngine.voiceResult, s_WakeWord.StartOffset);
                }
                else if (s_AsrEngine.voiceResult.status == ASR_CMD_DETECT)
                {
                    LOGD("[ASR] Command: %s(%d)", (s_VoiceCommand.pName == PL_NULL) ? "UNDEF" : s_VoiceCommand.pName,
                         s_VoiceCommand.Id);
                    s_AsrEngine.voiceResult.demo = s_AsrEngine.voiceConfig.demo;
                    voice_algo_asr_result_notify(&s_AsrEngine.voiceResult, 0);
#ifdef SMART_TLHMI_HOMEPANEL
                    asr_set_state(ASR_SESSION_STOPPED);
#endif /*SMART_TLHMI_HOMEPANEL*/
                }
            }
#if SELF_WAKE_UP_PROTECTION
            else if (cmdConfirmed == kWwRejected)
            {
                if (VIT_DetectionResultsSelfWake == VIT_WW_DETECTED)

                {
                    LOGD("[ASR] REJECTED Wake Word: %s(%d)",
                         (s_WakeWordSelfWake.pName == PL_NULL) ? "UNDEF" : s_WakeWordSelfWake.pName,
                         s_WakeWordSelfWake.Id);
                }
                else if (VIT_DetectionResultsSelfWake == VIT_VC_DETECTED)
                {
                    LOGD("[ASR] REJECTED Command: %s(%d)",
                         (s_VoiceCommandSelfWake.pName == PL_NULL) ? "UNDEF" : s_VoiceCommandSelfWake.pName,
                         s_VoiceCommandSelfWake.Id);
                }
            }
#endif /* SELF_WAKE_UP_PROTECTION */
        }

        s_afeDataProcessed++;
    }

#ifdef ASR_TO_AFE_PROCESSED_NOTIFY
    voice_algo_asr_run_notify(status);
#endif /* ASR_TO_AFE_PROCESSED_NOTIFY */

    return status;
}

static hal_valgo_status_t voice_algo_dev_input_notify(const voice_algo_dev_t *dev, void *data)
{
    hal_valgo_status_t error = kStatus_HAL_ValgoSuccess;
    event_voice_t event      = *(event_voice_t *)data;

    /* s_asrSession will be modified by initialize_asr().
     * Use asrSession to save the current state to restore it after ASR re-initialisation. */
    asr_session_t asrSession = s_asrSession;

    switch (event.event_base.eventId)
    {
        case (GET_TIMEOUT_DURATION):
        case (SET_TIMEOUT_DURATION):
        case (GET_FOLLOWUP_STATUS):
        case (SET_FOLLOWUP_STATUS):
        case (SET_VOICE_DEMO):
            LOGE("%d event handler not implemented", event.event_base.eventId);
            break;

        case (GET_MULTILINGUAL_CONFIG):
            print_enabled_languages(s_AsrEngine.voiceConfig.multilingual);
            break;

        case (SET_MULTILINGUAL_CONFIG):
            if (isLanguageSupported(event.set_multilingual_config.languages))
            {
                if (s_AsrEngine.voiceConfig.currentLanguage != event.set_multilingual_config.languages)
                {
                    s_AsrEngine.voiceConfig.multilingual    = event.set_multilingual_config.languages;
                    s_AsrEngine.voiceConfig.currentLanguage = event.set_multilingual_config.languages;
                    VIT_Deinit();
                    initialize_asr();

                    asr_set_state(asrSession);
                }
            }
            else
            {
                LOGE("Language %s (%d) is not supported.", get_language_str(event.set_multilingual_config.languages),
                     event.set_multilingual_config.languages);
            }
            print_enabled_languages(s_AsrEngine.voiceConfig.multilingual);
            break;

        case (GET_VOICE_DEMO):
            VIT_ModelInfo();
            break;

        case (STOP_VOICE_CMD_SESSION):
            LOGD("[ASR] Stop current voice command session.");
            asr_set_state(ASR_SESSION_STOPPED);
            break;

        case (SET_VOICE_MODEL):
        {
            set_asr_config_event_t config = (set_asr_config_event_t)event.set_asr_config;

            LOGD("[ASR] Set Voice Model: demo %d, language %d(%s)", config.demo, config.lang,
                 get_language_str(config.lang));

            uint8_t model_initialization_flag = 0, voice_session_update_flag = 0;

            if ((config.demo != UNDEFINED_INFERENCE) && (config.demo != s_AsrEngine.voiceConfig.demo))
            {
                switch (config.demo)
                {
                    case ASR_WW: /* Only need to update voice session */
                    {
                        if (s_asrSession != ASR_SESSION_WAKE_WORD)
                        {
                            voice_session_update_flag = 1;
                            asrSession                = ASR_SESSION_WAKE_WORD;
                        }
                    }
                    break;

                    default: /* Model is updated. Need to re-initialize voice engine */
                    {
                        s_AsrEngine.voiceConfig.demo = config.demo;
                        model_initialization_flag    = 1;
                    }
                    break;
                }
            }

            if ((config.demo != UNDEFINED_INFERENCE) && (config.demo == s_AsrEngine.voiceConfig.demo))
            {
                if (s_asrSession != ASR_SESSION_VOICE_COMMAND)
                {
                    voice_session_update_flag = 1;
                    asrSession                = ASR_SESSION_VOICE_COMMAND;
                }
            }

            /* (config.lang != UNDEFINED_LANGUAGE) means use the same language. */
            if (config.lang != UNDEFINED_LANGUAGE)
            {
                if (isLanguageSupported(config.lang))
                {
                    if ((s_AsrEngine.voiceConfig.multilingual != config.lang) ||
                        (s_AsrEngine.voiceConfig.currentLanguage != config.lang))
                    {
                        s_AsrEngine.voiceConfig.multilingual    = config.lang;
                        s_AsrEngine.voiceConfig.currentLanguage = config.lang;
                        model_initialization_flag = 1; /*Language is changed. Need to re-initialize voice engine */
                    }
                }
                else
                {
                    LOGE("Language %s (%d) is not supported. Using %s instead.", get_language_str(config.lang),
                         config.lang, get_language_str(s_AsrEngine.voiceConfig.currentLanguage));
                    print_enabled_languages(s_AsrEngine.voiceConfig.multilingual);
                }
            }

            if (model_initialization_flag)
            {
                VIT_Deinit();
                initialize_asr();
                LOGD("VIT engine re-initialization");
                asr_set_state(
                    asrSession); /* After voice engine re-initialized, need to set voice session to previous one */
            }
            else if (voice_session_update_flag)
            {
                asr_set_state(asrSession);
            }
        }
        break;

        default:
            LOGE("%d event handler not supported", event.event_base.eventId);
            break;
    }

    return error;
}

static hal_valgo_status_t voice_algo_dev_asr_deinit(voice_algo_dev_t *dev)
{
    VIT_Deinit();
    return kStatus_HAL_ValgoSuccess;
}

const static voice_algo_dev_operator_t voice_algo_dev_asr_ops = {.init        = voice_algo_dev_asr_init,
                                                                 .deinit      = voice_algo_dev_asr_deinit,
                                                                 .run         = voice_algo_dev_asr_run,
                                                                 .inputNotify = voice_algo_dev_input_notify};

static voice_algo_dev_t voice_algo_dev_asr = {
    .id  = 0,
    .ops = (voice_algo_dev_operator_t *)&voice_algo_dev_asr_ops,
    .cap = {.param = NULL},
};

static void voice_algo_asr_result_notify(asr_inference_result_t *result, uint32_t utteranceLength)
{
    if (voice_algo_dev_asr.cap.callback != NULL)
    {
        valgo_event_t valgo_event = {0};
        valgo_event.eventId       = kVAlgoEvent_VoiceResultUpdate;
        valgo_event.eventInfo     = kEventInfo_DualCore;
        valgo_event.data          = result;
        valgo_event.size          = sizeof(asr_inference_result_t);
        valgo_event.copy          = 1;
        voice_algo_dev_asr.cap.callback(voice_algo_dev_asr.id, valgo_event, 0);
        LOGD("[ASR] Result Status:%d Command:%d Utterance Length:%d Language:%s\r\n", result->status, result->keywordID,
             utteranceLength, get_language_str(result->language));

        if (utteranceLength != 0)
        {
            event_voice_t feedbackEvent = {0};
            memset(&valgo_event, 0, sizeof(valgo_event));
            feedbackEvent.event_base.eventId         = ASR_TO_AFE_FEEDBACK;
            feedbackEvent.asr_feedback.utterance_len = utteranceLength;

            /* Build Valgo event */
            valgo_event.eventId   = kVAlgoEvent_AsrToAfeFeedback;
            valgo_event.eventInfo = kEventInfo_Local;
            valgo_event.data      = &feedbackEvent;
            valgo_event.size      = sizeof(event_voice_t);
            valgo_event.copy      = 1;
            voice_algo_dev_asr.cap.callback(voice_algo_dev_asr.id, valgo_event, 0);
        }
    }
}

#ifdef ASR_TO_AFE_PROCESSED_NOTIFY
/*!
 * @brief Notify the AFE about the finish of the run. */

static void voice_algo_asr_run_notify(hal_valgo_status_t status)
{
    valgo_event_t valgo_event;
    static event_voice_t runNotifyEvent;

    if (voice_algo_dev_asr.cap.callback != NULL)
    {
        runNotifyEvent.event_base.eventId   = ASR_TO_AFE_PROCESSED;
        runNotifyEvent.event_base.eventInfo = kEventInfo_Local;
        runNotifyEvent.data                 = (void *)status;

        /* Build Valgo event */
        valgo_event.eventId = kVAlgoEvent_AsrToAfeProcessed;
        valgo_event.data    = &runNotifyEvent;
        valgo_event.size    = sizeof(event_voice_t);
        valgo_event.copy    = 0;
        voice_algo_dev_asr.cap.callback(voice_algo_dev_asr.id, valgo_event, 0);
    }
}
#endif /* ASR_TO_AFE_PROCESSED_NOTIFY */

int HAL_VoiceAlgoDev_Asr_VIT_Register()
{
    int error = 0;
    LOGD("HAL_VoiceAlgoDev_Asr_Register");
    error = FWK_VoiceAlgoManager_DeviceRegister(&voice_algo_dev_asr);
    return error;
}
#endif /* ENABLE_VIT_ASR */
