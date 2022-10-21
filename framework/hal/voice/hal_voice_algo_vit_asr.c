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

#define MODEL_LOCATION VIT_MODEL_IN_ROM
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
#define FAST_MEMORY_SIZE_BYTES    (45000)
#define SLOW_MEMORY_SIZE_BYTES    (225000)
#define MODEL_MEMORY_SIZE_BYTES   (sizeof(VIT_Model_en))
#define VIT_AFE_MEMORY_SIZE_BYTES (290000)

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
static PL_BOOL InitPhase_Error        = PL_FALSE;
static VIT_DataIn_st VIT_InputBuffers = {PL_NULL, PL_NULL,
                                         PL_NULL}; // Resetting Input Buffer addresses provided to VIT_process() API
static PL_INT8 *pMemory[PL_NR_MEMORY_REGIONS];

VIT_StatusParams_st VIT_StatusParams_Buffer;
/*******************************************************************************
 * Variables
 ******************************************************************************/

/* TODO: Delete these variables and the code where they are used.
 * Currently VIT library calls OSA_MemoryAllocate and OSA_MemoryFree.
 * For the best CPU Usage performance, OSA_MemoryAllocate should allocate
 * memory from a fast memory.
 * In this case we want to allocate it from OCRAM instead of SDRAM. */
uint8_t g_vitAfeInitializing = 0;
uint32_t g_vitAfeMemoryUsed  = 0;
AT_CACHEABLE_SECTION_ALIGN_OCRAM(int8_t g_vitAfeMemory[VIT_AFE_MEMORY_SIZE_BYTES], 8);

static uint32_t s_vitFastMemoryUsed = 0;
AT_NONCACHEABLE_SECTION_ALIGN_DTC(static int8_t s_vitFastMemory[FAST_MEMORY_SIZE_BYTES], 8);
static uint32_t s_vitSlowMemoryUsed = 0;
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static int8_t s_vitSlowMemory[SLOW_MEMORY_SIZE_BYTES], 8);
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static int8_t s_vitModelMemory[MODEL_MEMORY_SIZE_BYTES], 64);

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

/*******************************************************************************
 * Code
 ******************************************************************************/
static int VIT_Deinit()
{
    VIT_ReturnStatus_en VIT_Status;

    g_vitAfeInitializing = 1;
    VIT_Status           = VIT_GetMemoryTable(VITHandle, &VITMemoryTable, &VITInstParams);
    g_vitAfeInitializing = 0;

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
            VITControlParams.OperatingMode = VIT_AFE_ENABLE;
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
        VITControlParams.MIC1_MIC2_Distance = VIT_MIC1_MIC2_DISTANCE;
        VITControlParams.MIC1_MIC3_Distance = VIT_MIC1_MIC3_DISTANCE;
        g_vitAfeInitializing                = 1;
        VIT_Status                          = VIT_SetControlParameters(VITHandle, &VITControlParams);
        g_vitAfeInitializing                = 0;
        if (VIT_Status != VIT_SUCCESS)
        {
            LOGE("[ASR] %d state failed %d", state, VIT_Status);
        }
        if (g_vitAfeMemoryUsed > VIT_AFE_MEMORY_SIZE_BYTES)
        {
            LOGE("VIT AFE memory buffer is too small %d < %d.", VIT_AFE_MEMORY_SIZE_BYTES, g_vitAfeMemoryUsed);
            vTaskDelay(100);
            while (1)
                ;
        }
    }
}

static VIT_ReturnStatus_en initialize_asr()
{
    VIT_ReturnStatus_en VIT_Status = VIT_ERROR_UNDEFINED;

    const uint8_t *asrModelAddr = get_model_address(s_AsrEngine.voiceConfig.multilingual, s_AsrEngine.voiceConfig.demo);
    uint32_t asrModelSize       = get_model_size(s_AsrEngine.voiceConfig.multilingual, s_AsrEngine.voiceConfig.demo);

    if ((asrModelAddr == NULL) || (asrModelSize == 0) || (asrModelSize > MODEL_MEMORY_SIZE_BYTES))
    {
        LOGE("VIT get model for %d failed. Pointer %p size %d for demo %d.", s_AsrEngine.voiceConfig.demo, asrModelAddr,
             asrModelSize);
        return VIT_DUMMY_ERROR;
    }
    else
    {
        memcpy(s_vitModelMemory, asrModelAddr, asrModelSize);
        VIT_Status = VIT_SetModel((const PL_UINT8 *)s_vitModelMemory, VIT_MODEL_IN_RAM);
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
    VITInstParams.SamplesPerFrame = VIT_SAMPLES_PER_FRAME;
    VITInstParams.NumberOfChannel = NUMBER_OF_CHANNELS;
    VITInstParams.DeviceId        = DEVICE_ID;

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
            // reserve memory space
            // NB: VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_FAST_DATA] should be allocated
            //      in the fastest memory of the platform (when possible) - this is not the case in this example.
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
        vTaskDelay(100);
        while (1)
            ;
    }
    if (s_vitFastMemoryUsed > FAST_MEMORY_SIZE_BYTES)
    {
        LOGE("VIT fast memory buffer is too small %d < %d.", FAST_MEMORY_SIZE_BYTES, s_vitFastMemoryUsed);
        vTaskDelay(100);
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
        LOGD("VIT_GetInstanceHandle error: %d\r\n", VIT_Status);
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
    VIT_LibInfo_st Lib_Info;

    LOGD("  ");

    /* VIT Model information */
    VIT_Status = VIT_GetModelInfo(&Model_Info);
    if (VIT_Status == VIT_SUCCESS)
    {
        LOGD("--- VIT Model info ---");
        LOGD("Model Release: 0x%04x\r\n", Model_Info.VIT_Model_Release);
        if (Model_Info.pLanguage != PL_NULL)
        {
            LOGD("Language: %s\r\n", Model_Info.pLanguage);
        }

        if (Model_Info.WW_VoiceCmds_Strings)
        {
            const char *ptr;

            ptr = Model_Info.pWakeWord;
            if (ptr != PL_NULL)
            {
                LOGD("Wake Word:");
                LOGD("    '%s'\r\n", ptr);
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

    /* VIT Library information */
    VIT_Status = VIT_GetLibInfo(&Lib_Info);
    if (VIT_Status == VIT_SUCCESS)
    {
        LOGD("--- VIT Library info ---");
        LOGD("LIB Release: 0x%04x", Lib_Info.VIT_LIB_Release);
        LOGD("Features:    0x%04x", Lib_Info.VIT_Features_Supported);
        LOGD("Channels:    %d", Lib_Info.NumberOfChannels_Supported);
    }
    else
    {
        LOGD("VIT_GetLibInfo error: %d", VIT_Status);
    }

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
            /* For now French is not supported */
            ret = false;
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
 * @brief ASR main task
 */
static hal_valgo_status_t voice_algo_dev_asr_run(const voice_algo_dev_t *dev, void *data)
{
    hal_valgo_status_t status = kStatus_HAL_ValgoSuccess;
    VIT_ReturnStatus_en VIT_Status;
    VIT_WakeWord_st WakeWord;
    VIT_VoiceCommand_st VoiceCommand;
    VIT_DetectionStatus_en VIT_DetectionResults = VIT_NO_DETECTION;

    msg_payload_t *audioIn = (msg_payload_t *)data;
    if ((audioIn->data == NULL) || (audioIn->size != VIT_SAMPLES_PER_FRAME * NUMBER_OF_CHANNELS))
    {
        status = kStatus_HAL_ValgoError;
        LOGE("[ASR] Received invalid audio packet: addr=%d, size = %d", (uint32_t)audioIn->data, audioIn->size);
    }
    else
    {
        if (s_asrSession == ASR_SESSION_STOPPED)
        {
            /* Skip Voice Processing for Stopped state. */
        }
        else
        {
            if (VITInstParams.NumberOfChannel == _1CHAN)
            {
                VIT_InputBuffers.pBuffer_Chan1 = (PL_INT16 *)audioIn->data; // PCM buffer: 16-bit - 16kHz - mono
                VIT_InputBuffers.pBuffer_Chan2 = PL_NULL;
                VIT_InputBuffers.pBuffer_Chan3 = PL_NULL;
            }
            else if (VITInstParams.NumberOfChannel == _2CHAN)
            {
                VIT_InputBuffers.pBuffer_Chan1 = (PL_INT16 *)audioIn->data; // PCM buffer: 16-bit - 16kHz
                VIT_InputBuffers.pBuffer_Chan2 =
                    (PL_INT16 *)(audioIn->data + AUDIO_PCM_SINGLE_CH_SMPL_COUNT * AFE_INPUT_MIC_SAMPLE_BYTES);
                VIT_InputBuffers.pBuffer_Chan3 = PL_NULL;
            }

            VIT_Status = VIT_Process(VITHandle, &VIT_InputBuffers, &VIT_DetectionResults);
            if (VIT_Status != VIT_SUCCESS)
            {
                LOGE("VIT_Process error: %d\r\n", VIT_Status);
                status = kStatus_HAL_ValgoError;
            }
            else if (VIT_DetectionResults == VIT_WW_DETECTED)
            {
                VIT_Status = VIT_GetWakeWordFound(VITHandle, &WakeWord);
                if (VIT_Status == VIT_SUCCESS)
                {
                    LOGD("[ASR] Wake Word: %s(%d)", WakeWord.pWW_Name, WakeWord.WW_Id);
                }
                s_AsrEngine.voiceResult.status = ASR_WW_DETECT;
                /* VIT supports only one language at a time so it does not offer the detected
                 * language because it is the one used during initialisation. */
                s_AsrEngine.voiceResult.language = s_AsrEngine.voiceConfig.multilingual;

                asr_set_state(ASR_SESSION_STOPPED);

                voice_algo_asr_result_notify(&s_AsrEngine.voiceResult, 0);
            }
            else if (VIT_DetectionResults == VIT_VC_DETECTED)
            {
                /* Retrieve id of the Voice Command detected
                   String of the Command can also be retrieved (when WW and CMDs strings are integrated in Model) */
                VIT_Status = VIT_GetVoiceCommandFound(VITHandle, &VoiceCommand);
                if (VIT_Status != VIT_SUCCESS)
                {
                    LOGE("VIT_GetVoiceCommandFound error: %d\r\n", VIT_Status);
                    status = kStatus_HAL_ValgoError;
                }
                else
                {
                    if ((VoiceCommand.Cmd_Id > 0) && (VoiceCommand.pCmd_Name != PL_NULL))
                    {
                        LOGD("[ASR] Command: %s(%d)", VoiceCommand.pCmd_Name, VoiceCommand.Cmd_Id);

                        s_AsrEngine.voiceResult.keywordID = get_action_index_from_keyword(
                            s_AsrEngine.voiceConfig.multilingual, s_AsrEngine.voiceConfig.demo, VoiceCommand.Cmd_Id);

                        s_AsrEngine.voiceResult.status = ASR_CMD_DETECT;
                        /* VIT supports only one language at a time so it does not offer the detected
                         * language because it is the one used during initialization. */
                        s_AsrEngine.voiceResult.language = s_AsrEngine.voiceConfig.multilingual;

                        voice_algo_asr_result_notify(&s_AsrEngine.voiceResult, 0);
                    }
                }
            }
        }

        s_afeDataProcessed++;
    }
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
                s_AsrEngine.voiceConfig.multilingual    = event.set_multilingual_config.languages;
                s_AsrEngine.voiceConfig.currentLanguage = event.set_multilingual_config.languages;
                VIT_Deinit();
                initialize_asr();

                asr_set_state(asrSession);
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

            LOGD("[ASR] Set Voice Model: demo %d, language %d(%s)", config.demo,
                    config.lang, get_language_str(config.lang));

            /* (config.demo == UNDEFINED_INFERENCE) means use the same current demo. */
            if (config.demo != UNDEFINED_INFERENCE)
            {
                /* Wake Word is a part of the current demo so we should not update "demo" field. */
                if (config.demo != ASR_WW)
                {
                    s_AsrEngine.voiceConfig.demo = config.demo;
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

                        /* Reinitialize VIT engine in case the current language has been changed => model changed. */
                        LOGD("VIT engine re-initialization");
                        VIT_Deinit();
                        initialize_asr();
                    }
                }
                else
                {
                    LOGE("Language %s (%d) is not supported. Using %s instead.", get_language_str(config.lang),
                         config.lang, get_language_str(s_AsrEngine.voiceConfig.currentLanguage));
                    print_enabled_languages(s_AsrEngine.voiceConfig.multilingual);
                }
            }

            if (config.demo == UNDEFINED_INFERENCE)
            {
                asr_set_state(asrSession);
            }
            else if (config.demo == ASR_WW)
            {
                asr_set_state(ASR_SESSION_WAKE_WORD);
            }
            else
            {
                asr_set_state(ASR_SESSION_VOICE_COMMAND);
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

int HAL_VoiceAlgoDev_Asr_VIT_Register()
{
    int error = 0;
    LOGD("HAL_VoiceAlgoDev_Asr_Register");
    error = FWK_VoiceAlgoManager_DeviceRegister(&voice_algo_dev_asr);
    return error;
}
#endif /* ENABLE_VIT_ASR */
