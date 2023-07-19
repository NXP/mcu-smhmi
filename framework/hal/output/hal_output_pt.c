/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief Performance Test output HAL device implementation.
 */

#include "board_define.h"
#ifdef ENABLE_OUTPUT_DEV_PT

#include "FreeRTOS.h"
#include "board.h"

#include "app_config.h"

#include "fwk_log.h"
#include "fwk_timer.h"
#include "fwk_output_manager.h"
#include "fwk_lpm_manager.h"
#include "hal_output_dev.h"
#include "hal_voice_algo_asr_local.h"
#include "hal_event_descriptor_voice.h"
#include "hal_event_descriptor_face_rec.h"
#include "smart_tlhmi_event_descriptor.h"

#define VOICE_SESSION_TIMER_IN_MS   (15000)
#define FACEREC_SESSION_TIMER_IN_MS (2000)

static asr_language_t s_CurLanguage = ASR_ENGLISH;
static asr_inference_t s_CurDemo    = ASR_CMD_COFFEE_MACHINE;
static event_voice_t s_VoiceEvent;
static TimerHandle_t s_VoiceSessionTimer   = NULL;
static TimerHandle_t s_FaceRecSessionTimer = NULL;

static const char *s_CoffeeMachineCmds[] = {"Start",      "Cancel", "Confirm", "Espresso",  "Americano",
                                            "Cappuccino", "Latte",  "Small",   "Medium",    "Large",
                                            "Soft",       "Mild",   "Strong",  "DeleteUser"};

static const char *s_ElevatorCmds[] = {"FloorOne", "FloorTwo", "FloorThree", "FloorFour", "FloorFive",
                                       "FloorSix", "Confirm",  "Cancel",     "DeleteUser"};

static hal_output_status_t HAL_OutputDev_PT_Init(output_dev_t *dev, output_dev_callback_t callback);
static hal_output_status_t HAL_OutputDev_PT_Deinit(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_PT_Start(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_PT_Stop(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_PT_InferComplete(const output_dev_t *dev,
                                                          output_algo_source_t source,
                                                          void *inferResult);

/*******************************************************************************
 * Variables
 ******************************************************************************/

const static output_dev_operator_t s_OutputDev_PTOps = {
    .init   = HAL_OutputDev_PT_Init,
    .deinit = HAL_OutputDev_PT_Deinit,
    .start  = HAL_OutputDev_PT_Start,
    .stop   = HAL_OutputDev_PT_Stop,
};

static output_dev_t s_OutputDev_PT = {
    .name      = "PerformanceTest",
    .attr.type = kOutputDevType_Other,
    .ops       = &s_OutputDev_PTOps,
};

const static output_dev_event_handler_t s_OutputDev_PTHandler = {
    .inferenceComplete = HAL_OutputDev_PT_InferComplete,
};

static const char *_VoiceLanguageName_Get(asr_language_t language)
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

static const char *_VoiceCmdName_Get(int32_t keywordID)
{
    const char *cmd_name = NULL;

#if ENABLE_COFFEE_MACHINE
    int32_t cmd_count = sizeof(s_CoffeeMachineCmds) / sizeof(s_CoffeeMachineCmds[0]);
    if (keywordID < cmd_count)
    {
        cmd_name = s_CoffeeMachineCmds[keywordID];
    }
#elif ENABLE_ELEVATOR
    int32_t cmd_count = sizeof(s_ElevatorCmds) / sizeof(s_ElevatorCmds[0]);
    if (keywordID < cmd_count)
    {
        cmd_name = s_ElevatorCmds[keywordID];
    }
#else
#error "CURRENT_DEMO is not defined"
#endif

    return cmd_name;
}

static void _VoiceModel_Set(asr_inference_t modelId, asr_language_t lang, uint8_t ptt)
{
    LOGD("[PT] Set voice model:%d, language:%d, ptt:%d", modelId, lang, ptt);

    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_VoiceAlgoInputNotify;
    output_event.data      = &s_VoiceEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_VoiceEvent);
    output_event.eventInfo = kEventInfo_Local;

    s_VoiceEvent.event_base.eventId   = SET_VOICE_MODEL;
    s_VoiceEvent.event_base.eventInfo = kEventInfo_Local;
    s_VoiceEvent.set_asr_config.demo  = modelId;
    s_VoiceEvent.set_asr_config.lang  = lang;
    s_VoiceEvent.set_asr_config.ptt   = ptt;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_PT.cap.callback(s_OutputDev_PT.id, output_event, fromISR);
}

static void _VoiceSessionTimer_Callback(TimerHandle_t xTimer)
{
    _VoiceModel_Set(ASR_WW, UNDEFINED_LANGUAGE, 0);
}

static void _VoiceSessionTimer_Start()
{
    if (s_VoiceSessionTimer == NULL)
    {
        // create the timer
        s_VoiceSessionTimer = xTimerCreate("VoiceSessionTimer", (TickType_t)pdMS_TO_TICKS(VOICE_SESSION_TIMER_IN_MS),
                                           pdFALSE, NULL, (TimerCallbackFunction_t)_VoiceSessionTimer_Callback);
        if (s_VoiceSessionTimer == NULL)
        {
            LOGE("[PT] Failed to start \"VoiceSessionTimer\" timer.");
            return;
        }
    }

    if (xTimerStart(s_VoiceSessionTimer, 0) != pdPASS)
    {
        LOGE("[PT] Failed to start \"VoiceSessionTimer\" timer.");
    }
}

static void _VoiceSessionTimer_Stop()
{
    if (s_VoiceSessionTimer != NULL)
    {
        xTimerStop(s_VoiceSessionTimer, 0);
    }
}

static void _VoiceSession_Start()
{
    _VoiceSessionTimer_Start();
}

static void _VoiceSession_Stop()
{
    _VoiceSessionTimer_Stop();
}

static void _FaceRec_Stop(int stop)
{
    static event_face_rec_t s_FaceRecEvent;
    LOGD("[PT] Stop face rec:%d", stop);
    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_VisionAlgoInputNotify;
    output_event.data      = &s_FaceRecEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_FaceRecEvent);
    output_event.eventInfo = kEventInfo_Local;

    // notify the face rec to start
    s_FaceRecEvent.eventBase.eventId = kEventFaceRecID_OasisSetState;

    if (stop)
    {
        s_FaceRecEvent.oasisState.state = kOASISLiteState_Stopped;
    }
    else
    {
        s_FaceRecEvent.oasisState.state = kOASISLiteState_Running;
    }
    uint8_t fromISR = __get_IPSR();
    s_OutputDev_PT.cap.callback(s_OutputDev_PT.id, output_event, fromISR);
}

static void _FaceRecSessionTimer_Callback(TimerHandle_t xTimer)
{
    _FaceRec_Stop(0);
}

static void _FaceRecSessionTimer_Start()
{
    if (s_FaceRecSessionTimer == NULL)
    {
        // create the timer
        s_FaceRecSessionTimer =
            xTimerCreate("FaceRecSessionTimer", (TickType_t)pdMS_TO_TICKS(FACEREC_SESSION_TIMER_IN_MS), pdFALSE, NULL,
                         (TimerCallbackFunction_t)_FaceRecSessionTimer_Callback);
        if (s_FaceRecSessionTimer == NULL)
        {
            LOGE("[PT] Failed to start \"FaceRecSessionTimer\" timer.");
            return;
        }
    }

    if (xTimerStart(s_FaceRecSessionTimer, 0) != pdPASS)
    {
        LOGE("[PT] Failed to start \"FaceRecSessionTimer\" timer.");
    }
}

static void _FaceRecSessionTimer_Stop()
{
    if (s_FaceRecSessionTimer != NULL)
    {
        xTimerStop(s_FaceRecSessionTimer, 0);
    }
}

static void _FaceRecSession_Start()
{
    _FaceRecSessionTimer_Start();
}

static void _FaceRec_Register()
{
    static event_smart_tlhmi_t s_TlhmiEvent;
    LOGD("[PT] Register user");

    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_VisionAlgoInputNotify;
    output_event.data      = &s_TlhmiEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_TlhmiEvent);
    output_event.eventInfo = kEventInfo_Local;

    s_TlhmiEvent.eventBase.eventId                            = kEventFaceRecId_RegisterCoffeeSelection;
    s_TlhmiEvent.eventBase.eventInfo                          = kEventInfo_Local;
    s_TlhmiEvent.regCoffeeSelection.id                        = -1;
    s_TlhmiEvent.regCoffeeSelection.coffeeInfo.coffeeType     = 0;
    s_TlhmiEvent.regCoffeeSelection.coffeeInfo.coffeeSize     = 0;
    s_TlhmiEvent.regCoffeeSelection.coffeeInfo.coffeeStrength = 0;
    s_TlhmiEvent.regCoffeeSelection.coffeeInfo.language       = 0;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_PT.cap.callback(s_OutputDev_PT.id, output_event, fromISR);
}

static hal_output_status_t HAL_OutputDev_PT_Init(output_dev_t *dev, output_dev_callback_t callback)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_PT_Init");

    dev->cap.callback = callback;

    LOGD("--HAL_OutputDev_PT_Init");
    return error;
}

static hal_output_status_t HAL_OutputDev_PT_Deinit(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_AQT_Deinit");

    /* Add de-initialization code here */

    LOGD("--HAL_OutputDev_PT_Deinit");
    return error;
}

static hal_output_status_t HAL_OutputDev_PT_Start(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_PT_Start");

    /* Add start code here */
    if (FWK_OutputManager_RegisterEventHandler(dev, &s_OutputDev_PTHandler) != 0)
    {
        error = kStatus_HAL_OutputError;
    }

    /* Set the voice to wake word state */
    _VoiceModel_Set(ASR_WW, UNDEFINED_LANGUAGE, 0);

    LOGD("--HAL_OutputDev_PT_Start");
    return error;
}

static hal_output_status_t HAL_OutputDev_PT_Stop(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_PT_Stop");

    /* Add stop code here */

    LOGD("--HAL_OutputDev_PT_Stop");
    return error;
}

static hal_output_status_t _InferComplete_Voice(const output_dev_t *dev, void *inferResult)
{
    hal_output_status_t error               = kStatus_HAL_OutputSuccess;
    asr_inference_result_t *voiceAlgoResult = (asr_inference_result_t *)inferResult;

    // LOGD("[PT] voice status:%d cmd:%d", voiceAlgoResult->status, voiceAlgoResult->keywordID);
    if ((voiceAlgoResult->status == ASR_WW_DETECT))
    {
        s_CurLanguage             = voiceAlgoResult->language;
        const char *language_name = _VoiceLanguageName_Get(s_CurLanguage);
        LOGD(":VOICE:WakeWord:%s:[OK]", (language_name == NULL) ? "" : language_name);
        _VoiceModel_Set(s_CurDemo, s_CurLanguage, 0);
        _VoiceSession_Start();
    }
    else if ((voiceAlgoResult->status == ASR_CMD_DETECT) && (voiceAlgoResult->keywordID > -1))
    {
        const char *cmd_name      = _VoiceCmdName_Get(voiceAlgoResult->keywordID);
        const char *language_name = _VoiceLanguageName_Get(s_CurLanguage);
        LOGD(":VOICE:Command:%s:%s:%d:[OK]", (language_name == NULL) ? "" : language_name,
             (cmd_name == NULL) ? "" : cmd_name, voiceAlgoResult->keywordID);
        _VoiceSession_Stop();
        _VoiceModel_Set(ASR_WW, UNDEFINED_LANGUAGE, 0);
    }

    return error;
}

static hal_output_status_t _InferComplete_Vision(const output_dev_t *dev, void *inferResult)
{
    hal_output_status_t error              = kStatus_HAL_OutputSuccess;
    vision_algo_result_t *visionAlgoResult = (vision_algo_result_t *)inferResult;
    oasis_lite_result_t *pResult           = NULL;

    if (visionAlgoResult != NULL)
    {
        if (visionAlgoResult->id == kVisionAlgoID_OasisLite)
        {
            pResult = (oasis_lite_result_t *)&(visionAlgoResult->oasisLite);
        }
    }

    if (pResult != NULL)
    {
        // LOGD("[PT] face rec state:%d face_id:%d", pResult->face_recognized, pResult->face_id);
        if (pResult->face_recognized && pResult->face_id < 0)
        {
            // new user
            _FaceRec_Register();
            LOGD("[FACE] [NEW_USER] [OK]");
        }
        else if (pResult->face_recognized && pResult->face_id >= 0)
        {
            // known user
            LOGD("[FACE] [USER_%03d] [OK]", pResult->face_id);
        }
        _FaceRecSession_Start();
    }

    return error;
}

static hal_output_status_t HAL_OutputDev_PT_InferComplete(const output_dev_t *dev,
                                                          output_algo_source_t source,
                                                          void *inferResult)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    if (inferResult == NULL)
    {
        return error;
    }

    if (source == kOutputAlgoSource_Vision)
    {
        _InferComplete_Vision(dev, inferResult);
    }
    else if (source == kOutputAlgoSource_Voice)
    {
        _InferComplete_Voice(dev, inferResult);
    }

    return error;
}

int HAL_OutputDev_PT_Register()
{
    int error = 0;
    LOGD("++HAL_OutputDev_PT_Register");

    error = FWK_OutputManager_DeviceRegister(&s_OutputDev_PT);

    LOGD("--HAL_OutputDev_PT_Register");
    return error;
}

#endif /* ENABLE_OUTPUT_DEV_PT */
