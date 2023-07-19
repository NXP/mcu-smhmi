/*
 * Copyright 2021-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board_define.h"
#ifdef ENABLE_DSMT_ASR
#include "hal_voice_algo_asr_local.h"

#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_voice_algo_manager.h"
#include "hal_event_descriptor_voice.h"

/* local voice includes */
#include "IndexCommands.h"
#include "local_voice_model.h"
#include "hal_audio_defs.h"

#if ENABLE_COFFEE_MACHINE
#define CURRENT_DEMO ASR_CMD_COFFEE_MACHINE
#elif ENABLE_ELEVATOR
#define CURRENT_DEMO ASR_CMD_ELEVATOR
#elif ENABLE_HOME_PANEL
#define CURRENT_DEMO ASR_CMD_HP_MAIN_MENU
#else
#error "CURRENT_DEMO is not defined"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define WAKE_WORD_MEMPOOL_SIZE    (60 * 1024)
#define CN_WAKE_WORD_MEMPOOL_SIZE (105 * 1024)
#define COMMAND_MEMPOOL_SIZE      (150 * 1024)

/* Groups: base, ww, voice commands etc. */
#define NUM_GROUPS (3 + CMD_MODELS_COUNT)

#if NUM_GROUPS > MAX_GROUPS
#error "NUM_GROUPS must be smaller than MAX_GROUPS. Increase MAX_GROUPS."
#endif /* NUM_GROUPS > MAX_GROUPS */

typedef enum _cmd_state
{
    kWwConfirmed,
    kWwRejected,
    kWwNotSure,
} cmd_state_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static uint8_t s_memPoolWLangEn[WAKE_WORD_MEMPOOL_SIZE], 8);
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static uint8_t s_memPoolWLangCn[CN_WAKE_WORD_MEMPOOL_SIZE], 8);
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static uint8_t s_memPoolWLangDe[WAKE_WORD_MEMPOOL_SIZE], 8);
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static uint8_t s_memPoolWLangFr[WAKE_WORD_MEMPOOL_SIZE], 8);
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static uint8_t s_memPoolCmd[COMMAND_MEMPOOL_SIZE], 8);

#if (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_WW)
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static uint8_t s_memPoolWLangEnSelfWake[WAKE_WORD_MEMPOOL_SIZE], 8);
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static uint8_t s_memPoolWLangCnSelfWake[CN_WAKE_WORD_MEMPOOL_SIZE], 8);
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static uint8_t s_memPoolWLangDeSelfWake[WAKE_WORD_MEMPOOL_SIZE], 8);
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static uint8_t s_memPoolWLangFrSelfWake[WAKE_WORD_MEMPOOL_SIZE], 8);
#endif /* (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_WW) */
#if (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_VC)
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static uint8_t s_memPoolCmdSelfWake[COMMAND_MEMPOOL_SIZE], 8);
#endif /* (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_VC) */

typedef struct _asr_voice_param
{
    struct asr_language_model langModel[MAX_INTERFACES_LANG];
    struct asr_inference_engine infWW[MAX_INTERFACES_WW];
    struct asr_inference_engine infCMD[MAX_INTERFACES_CMD];
    asr_voice_control_t voiceControl;
    asr_voice_config_t voiceConfig;
    asr_inference_result_t voiceResult;
} asr_voice_param_t;

static asr_voice_param_t s_AsrEngine;

static void voice_algo_asr_result_notify(asr_inference_result_t *result, uint32_t utteranceLength);
static void voice_algo_asr_run_notify(hal_valgo_status_t status);

typedef enum _asr_session
{
    ASR_SESSION_STOPPED,
    ASR_SESSION_WAKE_WORD,
    ASR_SESSION_VOICE_COMMAND,
    ASR_SESSION_TIMEOUT,
    //    ASR_CANCELLED,
} asr_session_t;

static asr_session_t s_asrSession = ASR_SESSION_STOPPED;
static bool s_afeCalibrated       = false;

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

#if SELF_WAKE_UP_PROTECTION
/*!
 * @brief Get self wake up protection language.
 */
static asr_language_t get_self_protection_language(asr_language_t lang)
{
    asr_language_t selfProtectionLang = UNDEFINED_LANGUAGE;

    switch (lang)
    {
        case ASR_ENGLISH:
            selfProtectionLang = ASR_ENGLISH_SELF;
            break;

        case ASR_CHINESE:
            selfProtectionLang = ASR_CHINESE_SELF;
            break;

        case ASR_GERMAN:
            selfProtectionLang = ASR_GERMAN_SELF;
            break;

        case ASR_FRENCH:
            selfProtectionLang = ASR_FRENCH_SELF;
            break;

        default:
            selfProtectionLang = UNDEFINED_LANGUAGE;
            break;
    }

    return selfProtectionLang;
}
#endif /* SELF_WAKE_UP_PROTECTION */

#ifdef ASR_TO_AFE_PROCESSED_NOTIFY
/*!
 * @brief Notify the AFE about the finish of the run.
 */
static void voice_algo_asr_run_notify(hal_valgo_status_t status);
#endif /* ASR_TO_AFE_PROCESSED_NOTIFY */

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
 * @brief Utility function to extract indices from bitwise variable. this is used for asr_inference_t.
 */
static unsigned int decode_bitshift(unsigned int x)
{
    unsigned int y = 1; // starting from index 1 (not 0)
    while (x >>= 1)
        y++;
    return y;
}

/*!
 * @brief Utility function to break D-Spotter model binary pack into multiple groups where each group will represent an
 * inference engine combined with base model.
 */
static signed int unpackBin(unsigned char lpbyBin[], unsigned char *lppbyModel[], int32_t nMaxNumModel)
{
    unsigned int *lpnBin     = (unsigned int *)lpbyBin;
    signed int nNumBin       = lpnBin[0];
    unsigned int *lpnBinSize = lpnBin + 1;
    signed int i;

    lppbyModel[0] = (unsigned char *)(lpnBinSize + nNumBin);
    for (i = 1; i < nNumBin; i++)
    {
        if (i >= nMaxNumModel)
            break;
        lppbyModel[i] = lppbyModel[i - 1] + lpnBinSize[i - 1];
    }

    return i;
}

/*!
 * @brief Language model installation.
 */
static int32_t install_language(asr_voice_control_t *pAsrCtrl,
                                struct asr_language_model *pLangModel,
                                asr_language_t lang,
                                unsigned char *pAddrBin,
                                uint8_t nGroups)
{
    sln_asr_local_states_t status = kAsrLocalSuccess;
    uint8_t nGroupsMapID          = nGroups - 2; // -1 cause there is no MapID group for base model,
                                        // -1 cause the nGroups contains 1 more section with the no of mapID binaries

    if (lang && pAddrBin && nGroups)
    {
        pLangModel->iWhoAmI = lang;
        pLangModel->addrBin = pAddrBin;
        pLangModel->nGroups = nGroups;
        pLangModel->next    = pAsrCtrl->langModel;
        pAsrCtrl->langModel = pLangModel;

        if ((status = unpackBin(pAddrBin, pLangModel->addrGroup, nGroups)) <
            nGroups) // unpack group addresses from model binary
        {
            LOGD("Invalid bin. Error Code: %d.\r\n", status);
        }
        else
        {
            if ((status = unpackBin(pLangModel->addrGroup[nGroups - 1], pLangModel->addrGroupMapID, nGroupsMapID)) <
                (nGroupsMapID)) // unpack group addresses from mapID binary
            {
                LOGD("Invalid bin. Error Code: %d.\r\n", status);
            }
        }
    }
    else
        status = 1;

    return status;
}

/*!
 * @brief Inference engine installation.
 */
static uint32_t install_inference_engine(asr_voice_control_t *pAsrCtrl,
                                         struct asr_inference_engine *pInfEngine,
                                         asr_language_t lang,
                                         asr_inference_t infType,
                                         char **idToString,
                                         unsigned char *addrMemPool,
                                         uint32_t sizeMemPool)
{
    sln_asr_local_states_t status = kAsrLocalSuccess;

    if (pAsrCtrl && pInfEngine && lang && infType && addrMemPool && sizeMemPool)
    {
        pInfEngine->iWhoAmI_inf  = infType;
        pInfEngine->iWhoAmI_lang = lang;
        pInfEngine->handler      = NULL;
        pInfEngine->nGroups      = 2;
        pInfEngine->idToKeyword  = idToString;
        pInfEngine->memPool      = addrMemPool;
        pInfEngine->memPoolSize  = sizeMemPool;
        if (infType == ASR_WW)
        {                                                  // linked list for WW engines
            pInfEngine->next      = pAsrCtrl->infEngineWW; // the end of pInfEngine->next should be NULL
            pAsrCtrl->infEngineWW = pInfEngine;
        }
        else
        { // linked list for CMD engines. Dialog demo needs a linked list of CMD engines.
            pInfEngine->next       = pAsrCtrl->infEngineCMD;
            pAsrCtrl->infEngineCMD = pInfEngine;
        }
    }
    else
        status = kAsrLocalInstallFailed;

    return status;
}

/*!
 * @brief Checks memory pool size for WW / CMD engines.
 */
static void verify_inference_handler(struct asr_inference_engine *p)
{
    sln_asr_local_states_t status = kAsrLocalSuccess;
    int32_t mem_usage;

    mem_usage = SLN_ASR_LOCAL_Verify(p->addrGroup[0], (unsigned char **)&p->addrGroup[1], 1, k_nMaxTime);

    if (mem_usage > p->memPoolSize)
    {
        if (p->iWhoAmI_inf == ASR_WW)
        {
            LOGE("Memory size %d for WW exceeds the memory pool %d!\r\n", mem_usage, p->memPoolSize);
        }
        else
        {
            LOGE("Memory size %d for CMD exceeds the memory pool %d!\r\n", mem_usage, p->memPoolSize);
        }
        status = kAsrLocalOutOfMemory;
    }

    if (status != kAsrLocalSuccess)
    {
        // TODO show an LED
    }
}

/*!
 * @brief Handler should be set with valid
 *  p->addrGroup[0] (D-Spotter base model address) and
 *  p->addrGroup[1] (D-Spotter application group such as WW, CMD for IoT, etc)
 */
static void set_inference_handler(struct asr_inference_engine *p)
{
    int status = kAsrLocalSuccess;

    p->handler = SLN_ASR_LOCAL_Init(p->addrGroup[0], (unsigned char **)&p->addrGroup[1], 1, k_nMaxTime, p->memPool,
                                    p->memPoolSize, (int32_t *)&status);

    if (status != kAsrLocalSuccess)
    {
        // TODO show an LED
        LOGD("Could not initialize ASR engine. Please check language settings or license limitations!\r\n");
    }

    if ((status = SLN_ASR_LOCAL_Set_CmdMapID(p->handler, &p->addrGroupMapID, 1)) != kAsrLocalSuccess)
    {
        LOGD("Fail to set map id! - %d\r\n", status);
    }
}

/*!
 * @brief Handler should be reset after WW or CMDs are detected.
 */
static void reset_inference_handler(struct asr_inference_engine *p)
{
    SLN_ASR_LOCAL_Reset(p->handler);
}

/*!
 * @brief Initialize WW inference engine from the installed language models.
 *  After, pInfEngine should be a linked list of the installed languages.
 */
static void init_WW_engine(asr_voice_control_t *pAsrCtrl, asr_inference_t infType)
{
    struct asr_inference_engine *pInfEngine;
    struct asr_language_model *pLang;
    int idx       = decode_bitshift(ASR_WW); // decode the bitwise ASR_WW which is 1.
    int idx_mapID = idx - 1;                 // the index for mapIDs starts from 0 instead of 1

    pInfEngine = pAsrCtrl->infEngineWW;
    for (pLang = pAsrCtrl->langModel; pLang != NULL; pLang = pLang->next)
    {
        if (pInfEngine == NULL)
        {
            /* For (SELF_WAKE_UP_PROTECTION == SELF_WAKE_UP_VC), number of installed languages
             * is not equal to number of wake word interfaces. */
            break;
        }

        pInfEngine->addrGroup[0]   = pLang->addrGroup[0];              // language model's base
        pInfEngine->addrGroup[1]   = pLang->addrGroup[idx];            // language model's wake word group
        pInfEngine->addrGroupMapID = pLang->addrGroupMapID[idx_mapID]; // language model's wake word mapID group

        verify_inference_handler(pInfEngine); // verify inference handler, checking mem pool size
        set_inference_handler(pInfEngine);    // set inf engine to ww mode for each language.

        pInfEngine = pInfEngine->next; // the end of pInfEngine->next should be NULL.
    }
}

/*!
 * @brief Initialize CMD inference engine from the installed language models.
 *  After, pInfEngine does not need to be a linked list for Demo #1 and #2 but does for Demo #3 (dialog).
 */
static void init_CMD_engine(asr_voice_control_t *pAsrCtrl, asr_inference_t infType)
{
    struct asr_inference_engine *pInfEngine;
    struct asr_language_model *pLang;
    int idx       = decode_bitshift(infType); // decode the bitwise infType variable.
    int idx_mapID = idx - 1;                  // the index for mapIDs starts from 0 instead of 1

    pInfEngine = pAsrCtrl->infEngineCMD;

    pLang                      = pAsrCtrl->langModel;   // langModel for CMD inf engine is selected when WW is detected.
    pInfEngine->addrGroup[0]   = pLang->addrGroup[0];   // the selected language model's base
    pInfEngine->addrGroup[1]   = pLang->addrGroup[idx]; // the selected language model's infType group
    pInfEngine->addrGroupMapID = pLang->addrGroupMapID[idx_mapID]; // the selected language model's mapID group

    verify_inference_handler(pInfEngine); // verify inference handler, checking mem pool size
    set_inference_handler(pInfEngine);    // set inf engine to ww mode for each language.

#if (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_VC)
    pInfEngine = pInfEngine->next; // the end of pInfEngine->next should be NULL.

    pLang                      = pAsrCtrl->langModel;   // langModel for CMD inf engine is selected when WW is detected.
    pInfEngine->addrGroup[0]   = pLang->addrGroup[0];   // the selected language model's base
    pInfEngine->addrGroup[1]   = pLang->addrGroup[idx]; // the selected language model's infType group
    pInfEngine->addrGroupMapID = pLang->addrGroupMapID[idx_mapID]; // the selected language model's mapID group

    verify_inference_handler(pInfEngine); // verify inference handler, checking mem pool size
    set_inference_handler(pInfEngine);    // set inf engine to ww mode for each language.

#endif /* (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_VC) */
}

/*!
 * @brief Set language WW recognition engines.
 */
static void set_WW_engine(asr_voice_control_t *pAsrCtrl)
{
    struct asr_inference_engine *pInf = pAsrCtrl->infEngineWW;

    for (pInf = pAsrCtrl->infEngineWW; pInf != NULL; pInf = pInf->next)
    {
        set_inference_handler(pInf);
    }
}

/*!
 * @brief Reset language WW recognition engines.
 */
static void reset_WW_engine(asr_voice_control_t *pAsrCtrl)
{
    struct asr_inference_engine *pInf = pAsrCtrl->infEngineWW;

    for (pInf = pAsrCtrl->infEngineWW; pInf != NULL; pInf = pInf->next)
    {
        reset_inference_handler(pInf);
    }
}

/*!
 * @brief Set specific language CMD recognition engine, post WW detection.
 */
static void set_CMD_engine(asr_voice_control_t *pAsrCtrl,
                           asr_language_t langType,
                           asr_inference_t infCMDType,
                           char **cmdString)
{
    struct asr_language_model *pLang;
    struct asr_inference_engine *pInf = pAsrCtrl->infEngineCMD;
    int idx                           = decode_bitshift(infCMDType); // decode the bitwise infType variable
    int idx_mapID                     = idx - 1;                     // the index for mapIDs starts from 0 instead of 1

    for (pLang = pAsrCtrl->langModel; pLang != NULL; pLang = pLang->next)
    {
        if (pLang->iWhoAmI == langType)
        {
            pInf->iWhoAmI_inf  = infCMDType;
            pInf->iWhoAmI_lang = langType;
            pInf->addrGroup[0] = pLang->addrGroup[0]; // base model. should be same with WW's base
            if (pLang->addrGroup[idx] != NULL)
            {
                pInf->addrGroup[1]   = pLang->addrGroup[idx];
                pInf->addrGroupMapID = pLang->addrGroupMapID[idx_mapID];
            }
            set_inference_handler(pInf);
            pInf->idToKeyword = cmdString;

#if (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_VC)
            pInf               = pInf->next;
            pInf->iWhoAmI_inf  = infCMDType;
            pInf->iWhoAmI_lang = get_self_protection_language(langType);
            pInf->addrGroup[0] = pLang->addrGroup[0]; // base model. should be same with WW's base
            if (pLang->addrGroup[idx] != NULL)
            {
                pInf->addrGroup[1]   = pLang->addrGroup[idx];
                pInf->addrGroupMapID = pLang->addrGroupMapID[idx_mapID];
            }
            set_inference_handler(pInf);
            pInf->idToKeyword = cmdString;
#endif /* (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_VC) */

            break; // exit for loop, once pInf is set with the intended language
        }
    }
}

/*!
 * @brief Reset specific language CMD recognition engine.
 */
static void reset_CMD_engine(asr_voice_control_t *pAsrCtrl)
{
    struct asr_inference_engine *pInf = pAsrCtrl->infEngineCMD;

    for (pInf = pAsrCtrl->infEngineCMD; pInf != NULL; pInf = pInf->next)
    {
        reset_inference_handler(pInf);
    }
}

/*!
 * @brief Process audio stream to detect wake words or commands.
 */
static int asr_process_audio_buffer(void *handler, int16_t *audBuff, uint16_t bufSize, asr_inference_t infType)
{
    int status = 0;
    // reset values
    s_AsrEngine.voiceControl.result.keywordID[0] = 0xFFFF;
    s_AsrEngine.voiceControl.result.keywordID[1] = 0xFFFF;
    s_AsrEngine.voiceControl.result.cmdMapID     = 0xFF;

    status = SLN_ASR_LOCAL_Process(handler, audBuff, bufSize, &s_AsrEngine.voiceControl.result);

    return status;
}

static char *asr_get_string_by_id(struct asr_inference_engine *pInfEngine, int32_t id)
{
    return pInfEngine->idToKeyword[id];
}

static void asr_set_state(asr_session_t state)
{
    s_asrSession = state;

    reset_WW_engine(&s_AsrEngine.voiceControl);
    reset_CMD_engine(&s_AsrEngine.voiceControl);

    switch (state)
    {
        case ASR_SESSION_STOPPED:
            LOGD("[ASR] Session stopped");
            break;

        case ASR_SESSION_WAKE_WORD:
            LOGD("[ASR] Session waiting for Wake Word");
            set_WW_engine(&s_AsrEngine.voiceControl);
            break;

        case ASR_SESSION_VOICE_COMMAND:
            LOGD("[ASR] Session waiting for Voice Command");
            set_CMD_engine(&s_AsrEngine.voiceControl, s_AsrEngine.voiceConfig.currentLanguage,
                           s_AsrEngine.voiceConfig.demo,
                           get_cmd_strings(s_AsrEngine.voiceConfig.currentLanguage, s_AsrEngine.voiceConfig.demo));
            break;

        default:
            LOGE("Unknown state %d", state);
            break;
    }
}

static void initialize_asr(void)
{
    asr_language_t lang[MAX_NUM_LANGUAGES] = {UNDEFINED_LANGUAGE};

    /* Check enabled languages. lang[i] equal to 0 means language is disabled. */
    lang[0] = s_AsrEngine.voiceConfig.multilingual & ASR_ENGLISH;
    lang[1] = s_AsrEngine.voiceConfig.multilingual & ASR_CHINESE;
    lang[2] = s_AsrEngine.voiceConfig.multilingual & ASR_GERMAN;
    lang[3] = s_AsrEngine.voiceConfig.multilingual & ASR_FRENCH;

    /* Reset ASR modules.
     * NULL to ensure the end of linked list. */
    s_AsrEngine.voiceControl.langModel    = NULL;
    s_AsrEngine.voiceControl.infEngineWW  = NULL;
    s_AsrEngine.voiceControl.infEngineCMD = NULL;

    /* Install enabled languages. */
    install_language(&s_AsrEngine.voiceControl, &s_AsrEngine.langModel[3], lang[3], (unsigned char *)&oob_demo_fr_begin,
                     NUM_GROUPS);
    install_language(&s_AsrEngine.voiceControl, &s_AsrEngine.langModel[2], lang[2], (unsigned char *)&oob_demo_de_begin,
                     NUM_GROUPS);
    install_language(&s_AsrEngine.voiceControl, &s_AsrEngine.langModel[1], lang[1], (unsigned char *)&oob_demo_cn_begin,
                     NUM_GROUPS);
    install_language(&s_AsrEngine.voiceControl, &s_AsrEngine.langModel[0], lang[0], (unsigned char *)&oob_demo_en_begin,
                     NUM_GROUPS);

    /* Install enabled languages' Wake Words. */
    install_inference_engine(&s_AsrEngine.voiceControl, &s_AsrEngine.infWW[3], lang[3], ASR_WW, ww_fr, s_memPoolWLangFr,
                             WAKE_WORD_MEMPOOL_SIZE);
    install_inference_engine(&s_AsrEngine.voiceControl, &s_AsrEngine.infWW[2], lang[2], ASR_WW, ww_de, s_memPoolWLangDe,
                             WAKE_WORD_MEMPOOL_SIZE);
    install_inference_engine(&s_AsrEngine.voiceControl, &s_AsrEngine.infWW[1], lang[1], ASR_WW, ww_cn, s_memPoolWLangCn,
                             CN_WAKE_WORD_MEMPOOL_SIZE);
    install_inference_engine(&s_AsrEngine.voiceControl, &s_AsrEngine.infWW[0], lang[0], ASR_WW, ww_en, s_memPoolWLangEn,
                             WAKE_WORD_MEMPOOL_SIZE);

    /* Install English Voice Commands.
     * Reconfigure Voice Commands to appropriate language after Wake Word in that language is uttered. */
    install_inference_engine(&s_AsrEngine.voiceControl, &s_AsrEngine.infCMD[0], s_AsrEngine.voiceConfig.currentLanguage,
                             s_AsrEngine.voiceConfig.demo,
                             get_cmd_strings(s_AsrEngine.voiceConfig.currentLanguage, s_AsrEngine.voiceConfig.demo),
                             s_memPoolCmd, COMMAND_MEMPOOL_SIZE);

#if SELF_WAKE_UP_PROTECTION
    /* Install enabled languages. */
    install_language(&s_AsrEngine.voiceControl, &s_AsrEngine.langModel[MAX_NUM_LANGUAGES + 3], lang[3],
                     (unsigned char *)&oob_demo_fr_begin, NUM_GROUPS);
    install_language(&s_AsrEngine.voiceControl, &s_AsrEngine.langModel[MAX_NUM_LANGUAGES + 2], lang[2],
                     (unsigned char *)&oob_demo_de_begin, NUM_GROUPS);
    install_language(&s_AsrEngine.voiceControl, &s_AsrEngine.langModel[MAX_NUM_LANGUAGES + 1], lang[1],
                     (unsigned char *)&oob_demo_cn_begin, NUM_GROUPS);
    install_language(&s_AsrEngine.voiceControl, &s_AsrEngine.langModel[MAX_NUM_LANGUAGES + 0], lang[0],
                     (unsigned char *)&oob_demo_en_begin, NUM_GROUPS);
#endif /* SELF_WAKE_UP_PROTECTION */

#if (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_WW)
    /* Install enabled languages' Wake Words self wake up protection mechanism. */
    install_inference_engine(&s_AsrEngine.voiceControl, &s_AsrEngine.infWW[MAX_NUM_LANGUAGES + 3],
                             get_self_protection_language(lang[3]), ASR_WW, ww_fr, s_memPoolWLangFrSelfWake,
                             WAKE_WORD_MEMPOOL_SIZE);
    install_inference_engine(&s_AsrEngine.voiceControl, &s_AsrEngine.infWW[MAX_NUM_LANGUAGES + 2],
                             get_self_protection_language(lang[2]), ASR_WW, ww_de, s_memPoolWLangDeSelfWake,
                             WAKE_WORD_MEMPOOL_SIZE);
    install_inference_engine(&s_AsrEngine.voiceControl, &s_AsrEngine.infWW[MAX_NUM_LANGUAGES + 1],
                             get_self_protection_language(lang[1]), ASR_WW, ww_cn, s_memPoolWLangCnSelfWake,
                             CN_WAKE_WORD_MEMPOOL_SIZE);
    install_inference_engine(&s_AsrEngine.voiceControl, &s_AsrEngine.infWW[MAX_NUM_LANGUAGES + 0],
                             get_self_protection_language(lang[0]), ASR_WW, ww_en, s_memPoolWLangEnSelfWake,
                             WAKE_WORD_MEMPOOL_SIZE);
#endif /* (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_WW) */

#if (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_VC)
    /* Install English Voice Commands self wake up protection mechanism..
     * Reconfigure Voice Commands to appropriate language after Wake Word in that language is uttered. */
    install_inference_engine(&s_AsrEngine.voiceControl, &s_AsrEngine.infCMD[1],
                             get_self_protection_language(s_AsrEngine.voiceConfig.currentLanguage),
                             s_AsrEngine.voiceConfig.demo,
                             get_cmd_strings(s_AsrEngine.voiceConfig.currentLanguage, s_AsrEngine.voiceConfig.demo),
                             s_memPoolCmdSelfWake, COMMAND_MEMPOOL_SIZE);
#endif /* (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_VC) */

    /* Initialize WW and CMD engines. */
    init_WW_engine(&s_AsrEngine.voiceControl, s_AsrEngine.voiceConfig.demo);
    init_CMD_engine(&s_AsrEngine.voiceControl, s_AsrEngine.voiceConfig.demo);

    asr_set_state(ASR_SESSION_STOPPED);
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

static const bool isLanguageSupported(asr_language_t language)
{
    bool supported = false;

    switch (language)
    {
        case ASR_ENGLISH:
            supported = true;
            break;
        case ASR_CHINESE:
            supported = true;
            break;
        case ASR_GERMAN:
            supported = true;
            break;
        case ASR_FRENCH:
            supported = true;
            break;

        default:
            supported = false;
            break;
    }

    return supported;
}

static bool isMultilingualSupported(asr_language_t language)
{
    bool supported = true;

    for (uint8_t i = 0; i < (sizeof(asr_language_t) * 8); i++)
    {
        asr_language_t lang = ((1 < i) & language);
        if (lang != 0)
        {
            if (isLanguageSupported(lang) == false)
            {
                supported = false;
                break;
            }
        }
    }

    return supported;
}

static bool isLanguageAvailable(asr_language_t language)
{
    bool ret = false;

    if ((language & s_AsrEngine.voiceConfig.multilingual) != 0)
    {
        ret = true;
    }

    return ret;
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

hal_valgo_status_t voice_algo_dev_asr_init(voice_algo_dev_t *dev, valgo_dev_callback_t callback, void *param)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;

    dev->cap.callback = callback;

    memset(&s_AsrEngine.langModel, 0, sizeof(s_AsrEngine.langModel));
    memset(&s_AsrEngine.infWW, 0, sizeof(s_AsrEngine.infWW));
    memset(&s_AsrEngine.infCMD, 0, sizeof(s_AsrEngine.infCMD));
    memset(&s_AsrEngine.voiceControl, 0, sizeof(asr_voice_control_t));
    s_AsrEngine.voiceResult.status    = ASR_NONE;
    s_AsrEngine.voiceResult.keywordID = ASR_KEYWORDID_INVALID;

    /* TODO: Replace all HAL_OutputDev_SmartLockConfig */
    /*s_AsrEngine.voiceConfig = HAL_OutputDev_SmartLockConfig_GetAsrConfig();*/
    s_AsrEngine.voiceConfig.status = WRITE_FAIL;
    if (s_AsrEngine.voiceConfig.status != WRITE_SUCCESS)
    {
        s_AsrEngine.voiceConfig.demo            = CURRENT_DEMO;
        s_AsrEngine.voiceConfig.followup        = ASR_FOLLOWUP_OFF;
        s_AsrEngine.voiceConfig.multilingual    = DEFAULT_ACTIVE_LANGUAGE;
        s_AsrEngine.voiceConfig.currentLanguage = ASR_ENGLISH;
        s_AsrEngine.voiceConfig.mute            = ASR_MUTE_OFF;
        s_AsrEngine.voiceConfig.ptt             = ASR_PTT_OFF;
        s_AsrEngine.voiceConfig.timeout         = TIMEOUT_TIME_IN_MS;
        s_AsrEngine.voiceConfig.status          = WRITE_SUCCESS;
        s_AsrEngine.voiceConfig.asrCfg          = ASR_CFG_DEMO_NO_CHANGE;
        /*HAL_OutputDev_SmartLockConfig_SetAsrConfig(s_AsrEngine.voiceConfig);*/
    }

    /* Initialize the ASR engine */
    initialize_asr();
    LOGD("[ASR] DSMT initialized");

    return ret;
}

/*!
 * @brief ASR main task
 */
hal_valgo_status_t voice_algo_dev_asr_run(const voice_algo_dev_t *dev, void *data)
{
    hal_valgo_status_t status = kStatus_HAL_ValgoSuccess;
    struct asr_inference_engine *pInfWW;
    struct asr_inference_engine *pInfCMD;
    int16_t *cleanSound   = NULL;
    int16_t *speakerSound = NULL;

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
        static char *s_cmdName           = NULL;
        static asr_language_t s_cmdLang  = UNDEFINED_LANGUAGE;
        static uint32_t s_cmdLength      = 0;
        static asr_result_t s_cmdDetails = {0};

        bool realCmdDetected         = false;
        bool fakeCmdDetected         = false;
        cmd_state_t cmdConfirmed     = kWwNotSure;
        uint32_t cmdConfirmedDelayMs = 0;
        uint32_t *dumpMark = audioIn->data_marks;


        if (s_asrSession == ASR_SESSION_WAKE_WORD)
        {
#ifdef ENABLE_OUTPUT_DEV_AudioDump
            postMarkCleanStreamConsumedByWW(dumpMark, ASR_INPUT_FRAMES);
#endif /* ENABLE_OUTPUT_DEV_AudioDump */

#if (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_WW)
            if (speakerSound != NULL)
            {
                /* Fake Wake Word detection. Check all enabled languages, but stop on first match.
                 * Skip actual languages. Example: Process ASR_ENGLISH_SELF, but skip ASR_ENGLISH. */
                for (pInfWW = s_AsrEngine.voiceControl.infEngineWW; pInfWW != NULL; pInfWW = pInfWW->next)
                {
                    if (pInfWW->iWhoAmI_lang < LAST_LANGUAGE)
                    {
                        continue;
                    }

                    if (asr_process_audio_buffer(pInfWW->handler, speakerSound, ASR_INPUT_SAMPLES,
                                                 pInfWW->iWhoAmI_inf) == kAsrLocalDetected)
                    {
                        s_cmdName = asr_get_string_by_id(pInfWW, s_AsrEngine.voiceControl.result.keywordID[0]);
                        s_cmdLang = pInfWW->iWhoAmI_lang;
                        memcpy(&s_cmdDetails, &s_AsrEngine.voiceControl.result, sizeof(asr_result_t));

                        fakeCmdDetected = true;
                        break;
                    }
                }
            }
#endif /* (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_WW) */

            /* Wake Word detection. Check all enabled languages, but stop on first match.
             * Skip fake languages. Example: Process ASR_ENGLISH, but skip ASR_ENGLISH_SELF. */
            for (pInfWW = s_AsrEngine.voiceControl.infEngineWW; pInfWW != NULL; pInfWW = pInfWW->next)
            {
                if (pInfWW->iWhoAmI_lang >= LAST_LANGUAGE)
                {
                    continue;
                }

                if (asr_process_audio_buffer(pInfWW->handler, cleanSound, ASR_INPUT_SAMPLES,
                                             pInfWW->iWhoAmI_inf) == kAsrLocalDetected)
                {
                    s_cmdName   = asr_get_string_by_id(pInfWW, s_AsrEngine.voiceControl.result.keywordID[0]);
                    s_cmdLang   = pInfWW->iWhoAmI_lang;
                    s_cmdLength = SLN_ASR_LOCAL_GetDetectedCommandDuration(pInfWW->handler);
                    memcpy(&s_cmdDetails, &s_AsrEngine.voiceControl.result, sizeof(asr_result_t));

                    realCmdDetected = true;
                    break;
                }
                else
                {
                    s_cmdLength += ASR_INPUT_SAMPLES;
                }
            }

            cmdConfirmed = confirmDetectedCommand(realCmdDetected, fakeCmdDetected, speakerSound, &cmdConfirmedDelayMs);
            if (cmdConfirmed == kWwConfirmed)
            {
                asr_set_state(ASR_SESSION_STOPPED);

                LOGD("[ASR] Wake Word: %s(%d) - MapID(%d), delay %d [ms]", s_cmdName, s_cmdDetails.keywordID[0],
                     s_cmdDetails.cmdMapID, cmdConfirmedDelayMs);

                s_AsrEngine.voiceConfig.currentLanguage = s_cmdLang;

                /* Send Feedback to AFE regarding detected Wake Word length.
                 * Based on this feedback, AFE calibrates itself for a better performance. */
                s_AsrEngine.voiceResult.status   = ASR_WW_DETECT;
                s_AsrEngine.voiceResult.language = s_cmdLang;

                voice_algo_asr_result_notify(&s_AsrEngine.voiceResult, s_cmdLength);
                s_cmdLength     = 0;
                s_afeCalibrated = true;
            }
#if (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_WW)
            else if (cmdConfirmed == kWwRejected)
            {
                reset_WW_engine(&s_AsrEngine.voiceControl);
                LOGD("[ASR] REJECTED Wake Word: %s(%d) - MapID(%d), delay %d [ms]", s_cmdName,
                     s_cmdDetails.keywordID[0], s_cmdDetails.cmdMapID, cmdConfirmedDelayMs);
            }
#endif /* (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_WW) */
        }
        else if (s_asrSession == ASR_SESSION_VOICE_COMMAND)
        {
#ifdef ENABLE_OUTPUT_DEV_AudioDump
            postMarkCleanStreamConsumedByCMD(dumpMark, ASR_INPUT_FRAMES);
#endif /* ENABLE_OUTPUT_DEV_AudioDump */

#if (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_VC)
            if (speakerSound != NULL)
            {
                /* Fake Command detection. Check all enabled languages, but stop on first match.
                 * Skip actual languages. Example: Process ASR_ENGLISH_SELF, but skip ASR_ENGLISH. */
                for (pInfCMD = s_AsrEngine.voiceControl.infEngineCMD; pInfCMD != NULL; pInfCMD = pInfCMD->next)
                {
                    if (pInfCMD->iWhoAmI_lang < LAST_LANGUAGE)
                    {
                        continue;
                    }

                    if (asr_process_audio_buffer(pInfCMD->handler, speakerSound, ASR_INPUT_SAMPLES,
                                                 pInfCMD->iWhoAmI_inf) == kAsrLocalDetected)
                    {
                        s_cmdName = asr_get_string_by_id(pInfCMD, s_AsrEngine.voiceControl.result.keywordID[1]);
                        s_cmdLang = pInfCMD->iWhoAmI_lang;
                        memcpy(&s_cmdDetails, &s_AsrEngine.voiceControl.result, sizeof(asr_result_t));

                        fakeCmdDetected = true;
                        break;
                    }
                }
            }
#endif /* (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_VC) */

            /* Command detection. Check all enabled languages, but stop on first match.
             * Skip fake languages. Example: Process ASR_ENGLISH, but skip ASR_ENGLISH_SELF. */
            for (pInfCMD = s_AsrEngine.voiceControl.infEngineCMD; pInfCMD != NULL; pInfCMD = pInfCMD->next)
            {
                if (pInfCMD->iWhoAmI_lang >= LAST_LANGUAGE)
                {
                    continue;
                }

                if (asr_process_audio_buffer(pInfCMD->handler, cleanSound, ASR_INPUT_SAMPLES,
                                             pInfCMD->iWhoAmI_inf) == kAsrLocalDetected)
                {
                    s_cmdName   = asr_get_string_by_id(pInfCMD, s_AsrEngine.voiceControl.result.keywordID[1]);
                    s_cmdLang   = pInfCMD->iWhoAmI_lang;
                    s_cmdLength = SLN_ASR_LOCAL_GetDetectedCommandDuration(pInfCMD->handler);
                    memcpy(&s_cmdDetails, &s_AsrEngine.voiceControl.result, sizeof(asr_result_t));

                    realCmdDetected = true;
                    break;
                }
                else
                {
                    s_cmdLength += ASR_INPUT_SAMPLES;
                }
            }

            cmdConfirmed = confirmDetectedCommand(realCmdDetected, fakeCmdDetected, speakerSound, &cmdConfirmedDelayMs);
            if (cmdConfirmed == kWwConfirmed)
            {
                reset_CMD_engine(&s_AsrEngine.voiceControl);

                LOGD("[ASR] Command: %s(%d) - MapID(%d), delay %d [ms]", s_cmdName, s_cmdDetails.keywordID[1],
                     s_cmdDetails.cmdMapID, cmdConfirmedDelayMs);

                /* Send Feedback to AFE regarding detected Voice Command length.
                 * Based on this feedback, AFE calibrates itself for a better performance */
                s_AsrEngine.voiceResult.status   = ASR_CMD_DETECT;
                s_AsrEngine.voiceResult.language = s_cmdLang;
                s_AsrEngine.voiceResult.keywordID =
                    get_action_index_from_keyword(s_cmdLang, s_AsrEngine.voiceConfig.demo, s_cmdDetails.keywordID[1]);

                if (s_afeCalibrated == true)
                {
                    /* In case AFE was already calibrated based on Wake Word,
                     * skip AFE calibration based on detected Voice Command. */
                    s_cmdLength = 0;
                }

                voice_algo_asr_result_notify(&s_AsrEngine.voiceResult, s_cmdLength);
                s_cmdLength     = 0;
                s_afeCalibrated = true;
            }
#if (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_VC)
            else if (cmdConfirmed == kWwRejected)
            {
                reset_CMD_engine(&s_AsrEngine.voiceControl);
                LOGD("[ASR] REJECTED Command: %s(%d) - MapID(%d), delay %d [ms]", s_cmdName, s_cmdDetails.keywordID[1],
                     s_cmdDetails.cmdMapID, cmdConfirmedDelayMs);
            }
#endif /* (SELF_WAKE_UP_PROTECTION & SELF_WAKE_UP_VC) */
        }
#if ENABLE_OUTPUT_DEV_AudioDump
        else
        {
            postMarkCleanStreamSKIPPED(dumpMark, ASR_INPUT_FRAMES);
        }
#endif /* ENABLE_OUTPUT_DEV_AudioDump */
        s_afeDataProcessed++;
    }

#ifdef ASR_TO_AFE_PROCESSED_NOTIFY
    voice_algo_asr_run_notify(status);
#endif /* ASR_TO_AFE_PROCESSED_NOTIFY */

    return status;
}

hal_valgo_status_t voice_algo_dev_input_notify(const voice_algo_dev_t *dev, void *data)
{
    hal_valgo_status_t error = kStatus_HAL_ValgoSuccess;
    event_voice_t event      = *(event_voice_t *)data;

    /* s_asrSession will be modified by initialize_asr().
     * Use asrSession to save the current state to restore it after ASR re-initialisation. */
    asr_session_t asrSession = s_asrSession;

    switch (event.event_base.eventId)
    {
        case (GET_TIMEOUT_DURATION):
            /*LOGD("\r\nCurrent timeout duration is: %d\r\n",
             * HAL_OutputDev_SmartLockConfig_GetAsrTimeoutDuration());*/
            break;

        case (SET_TIMEOUT_DURATION):

            if (0/*kSLNConfigStatus_Success ==
                HAL_OutputDev_SmartLockConfig_SetAsrTimeoutDuration(event.set_timeout_duration.timeout)*/)
            {
                s_AsrEngine.voiceConfig.timeout = event.set_timeout_duration.timeout;
            }
            else
            {
                error = kStatus_HAL_ValgoError;
                LOGE("[ERROR] Failed to save Timeout config to flash.");
            }
            break;

        case (GET_FOLLOWUP_STATUS):
            /*LOGD("\r\nFollowup is currently: %s\r\n",
                   HAL_OutputDev_SmartLockConfig_GetAsrFollowupStatus() == ASR_FOLLOWUP_ON ? "enabled" : "disabled");*/
            break;

        case (SET_FOLLOWUP_STATUS):
            if (0/*kSLNConfigStatus_Success ==
                HAL_OutputDev_SmartLockConfig_SetAsrFollowupStatus(event.set_followup_status.followup)*/)
            {
                s_AsrEngine.voiceConfig.followup = event.set_followup_status.followup;
            }
            else
            {
                error = kStatus_HAL_ValgoError;
                LOGE("[ERROR] Failed to save Followup config to flash.");
            }
            break;

        case (GET_MULTILINGUAL_CONFIG):
            print_enabled_languages(s_AsrEngine.voiceConfig.multilingual);
            break;

        case (SET_MULTILINGUAL_CONFIG):
            if (isMultilingualSupported(event.set_multilingual_config.languages))
            {
                if(s_AsrEngine.voiceConfig.multilingual != event.set_multilingual_config.languages)
                {
                    s_AsrEngine.voiceConfig.multilingual = event.set_multilingual_config.languages;
                    initialize_asr();
                    asr_set_state(asrSession);
                    print_enabled_languages(s_AsrEngine.voiceConfig.multilingual);
                }
            }
            else
            {
                LOGE("Language %s (%d) is not supported.", get_language_str(event.set_multilingual_config.languages),
                     event.set_multilingual_config.languages);
            }
            break;

        case (GET_VOICE_DEMO):

            LOGD("********************************\r\n");
            LOGD("\r\nCurrent voice demo set to: ");
            switch (s_AsrEngine.voiceConfig.demo)
            {
                case ASR_CMD_COFFEE_MACHINE:
                    LOGD("\"IoT\"\r\n");
                    break;
                default:
                    break;
            }
            for (int i = 0; i < MAX_NUM_LANGUAGES; i++)
            {
                if (s_AsrEngine.voiceConfig.multilingual & (1 << i))
                {
                    LOGD("********************************\r\n");
                    switch (1 << i)
                    {
                        case ASR_ENGLISH:
                            LOGD("English\r\n");
                            LOGD("********************************\r\n");
                            LOGD("Wake Word:\r\n\t\"%s\"\r\n", ww_en[0]);
                            break;
                        case ASR_CHINESE:
                            LOGD("Chinese\r\n");
                            LOGD("********************************\r\n");
                            LOGD("Wake Word:\r\n\t\"%s\"\r\n", ww_cn[0]);
                            break;
                        case ASR_GERMAN:
                            LOGD("German\r\n");
                            LOGD("********************************\r\n");
                            LOGD("Wake Word:\r\n\t\"%s\"\r\n", ww_de[0]);
                            break;
                        case ASR_FRENCH:
                            LOGD("French\r\n");
                            LOGD("********************************\r\n");
                            LOGD("Wake Word:\r\n\t\"%s\"\r\n", ww_fr[0]);
                            break;
                        default:
                            break;
                    }
                    LOGD("Commands:\r\n");

                    int num_commands = get_cmd_number((1 << i), s_AsrEngine.voiceConfig.demo);
                    char **commands  = get_cmd_strings((1 << i), s_AsrEngine.voiceConfig.demo);
                    for (int j = 0; j < num_commands; j++)
                    {
                        LOGD("\t\"%s\"\r\n", commands[j]);
                    }
                }
            }

            break;

        case (SET_VOICE_DEMO):
            break;

        case (STOP_VOICE_CMD_SESSION):
            LOGD("[ASR] Stop current voice command session.");
            asr_set_state(ASR_SESSION_STOPPED);
            break;

        case (SET_VOICE_MODEL):
        {
            set_asr_config_event_t config = (set_asr_config_event_t)event.set_asr_config;
            uint8_t model_initialization_flag = 0, voice_session_update_flag = 0;

            LOGD("[ASR] Set Voice Model: demo %d, language %d(%s), ptt %d", config.demo, config.lang,
                 get_language_str(config.lang), config.ptt);

            /* In case the device was awaken via Wake Word (config.ptt == 0), it means AFE is already calibrated
             * based on the length of the previously detected Wake Word.
             * In case the device was awaken via other source (config.ptt == 1), it means AFE is NOT yet calibrated,
             * so it should be calibrated based on the next detected voice command.
             * In case the Wake Word listening phase is just being started, it means the AFE should be re-calibrated
             * based on the next detected Wake Word or Voice Command (in case the Wake Word will be skipped). */
            if ((config.ptt == 1) || (config.demo == ASR_WW))
            {
                s_afeCalibrated = false;
            }

            /* (config.demo == UNDEFINED_INFERENCE) means use the same current demo. */
            if ((config.demo != UNDEFINED_INFERENCE) && (config.demo !=  s_AsrEngine.voiceConfig.demo))
            {
                switch(config.demo)
                {
                    case ASR_WW: /* Only need to update voice session */
                    {
                        if (s_asrSession != ASR_SESSION_WAKE_WORD)
                        {
                            voice_session_update_flag = 1;
                            asrSession = ASR_SESSION_WAKE_WORD;                        }
                    }
                    break;

                    default:  /* Model is updated. Need to re-initialize voice engine */
                    {
                        s_AsrEngine.voiceConfig.demo = config.demo;
                        model_initialization_flag = 1;
                    }
                    break;
                }
            }

            if ((config.demo != UNDEFINED_INFERENCE) && (config.demo == s_AsrEngine.voiceConfig.demo))
            {
                if (s_asrSession != ASR_SESSION_VOICE_COMMAND)
                {
                    voice_session_update_flag = 1;
                    asrSession = ASR_SESSION_VOICE_COMMAND;
                }
            }

            /* (config.lang != UNDEFINED_LANGUAGE) means use the same language. */
            if (config.lang != UNDEFINED_LANGUAGE)
            {
                if (isLanguageSupported(config.lang))
                {
                    if (!isLanguageAvailable(config.lang))
                    {   /* Language is changed. Need to re-initialize voice model */
                        print_enabled_languages(s_AsrEngine.voiceConfig.multilingual);
                        LOGD("Language %s (%d) is not enabled. Enabling it.", get_language_str(config.lang),
                             config.lang);

                        s_AsrEngine.voiceConfig.multilingual |= config.lang;
                        s_AsrEngine.voiceConfig.currentLanguage = config.lang;
                        model_initialization_flag = 1;
                        print_enabled_languages(s_AsrEngine.voiceConfig.multilingual);
                    }
                    else if(s_AsrEngine.voiceConfig.currentLanguage != config.lang)
                    {
                        s_AsrEngine.voiceConfig.currentLanguage = config.lang;
                        voice_session_update_flag = 1;
                    }
                }
                else
                {
                    s_AsrEngine.voiceConfig.currentLanguage = getFirstAvailableLanguage();
                    LOGE("Language %s (%d) is not supported. Using %s instead.", get_language_str(config.lang),
                         config.lang, get_language_str(s_AsrEngine.voiceConfig.currentLanguage));
                    print_enabled_languages(s_AsrEngine.voiceConfig.multilingual);
                }

            }

            if(model_initialization_flag)
            {
                initialize_asr();
                asr_set_state(asrSession);
            }
            else if(voice_session_update_flag)
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

const static voice_algo_dev_operator_t voice_algo_dev_asr_ops = {.init        = voice_algo_dev_asr_init,
                                                                 .deinit      = NULL,
                                                                 .run         = voice_algo_dev_asr_run,
                                                                 .inputNotify = voice_algo_dev_input_notify};

static voice_algo_dev_t voice_algo_dev_asr = {
    .id  = 0,
    .ops = (voice_algo_dev_operator_t *)&voice_algo_dev_asr_ops,
    .cap = {.param = NULL},
};

void voice_algo_asr_result_notify(asr_inference_result_t *result, uint32_t utteranceLength)
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

int HAL_VoiceAlgoDev_Asr_Register()
{
    int error = 0;
    LOGD("HAL_VoiceAlgoDev_Asr_Register");
    error = FWK_VoiceAlgoManager_DeviceRegister(&voice_algo_dev_asr);
    return error;
}
#endif /* ENABLE_DSMT_ASR */
