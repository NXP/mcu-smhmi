/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief Ui coffee machine output HAL device implementation.
 */

#include "board_define.h"
#ifdef ENABLE_OUTPUT_DEV_UiElevator

#include "FreeRTOS.h"
#include "board.h"

#include "app_config.h"

#include "fwk_graphics.h"
#include "fwk_log.h"
#include "fwk_timer.h"
#include "fwk_output_manager.h"
#include "fwk_lpm_manager.h"
#include "hal_output_dev.h"
#include "hal_vision_algo.h"
#include "hal_voice_algo_asr_local.h"
#include "hal_event_descriptor_common.h"
#include "hal_event_descriptor_face_rec.h"
#include "hal_event_descriptor_voice.h"

#include "smart_tlhmi_event_descriptor.h"

#include "custom.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*
     |<------------640------------------>|
___  ____________________________________   ____
 |   |                                   |    |
 |   |-----------------------------------|  --|<--- UI_TOPINFO_Y
 |   |       Top Info                    |    |
 |   |-----------------------------------|  --|<--- UI_MAINWINDOW_Y = UI_TOPINFO_Y+UI_TOPINFO_H
 |   |                                   |    |
 |   |                                   |    |
480  |      Main Window                  |    |
 |   |                                   |    |
 |   |                                   |    |
 |   |                                   |    |
 |   |-----------------------------------|  --|<--- UI_BOTTOMINFO_H = UI_MAINWINDOW_Y+UI_MAINWINDOW_H
 |   |        Bottom Info                |    |
_|_  |___________________________________|  __|_

*/

#define UI_BUFFER_WIDTH  640
#define UI_BUFFER_HEIGHT 480
#define UI_BUFFER_BPP    2
#define UI_BUFFER_PITCH  (UI_BUFFER_WIDTH * UI_BUFFER_BPP)
#define UI_SCALE_W       (640 / UI_BUFFER_WIDTH)
#define UI_SCALE_H       (480 / UI_BUFFER_HEIGHT)

#define UI_GUIDE_RECT_W 360
#define UI_GUIDE_RECT_H 300

#define UI_TOPINFO_W    UI_BUFFER_WIDTH
#define UI_TOPINFO_H    30
#define UI_TOPINFO_X    0
#define UI_TOPINFO_Y    (4 / UI_SCALE_H)
#define UI_BOTTOMINFO_W UI_BUFFER_WIDTH
#define UI_BOTTOMINFO_H 20
#define UI_MAINWINDOW_W UI_BUFFER_WIDTH
#define UI_MAINWINDOW_H (UI_BUFFER_HEIGHT - UI_TOPINFO_H - UI_BOTTOMINFO_H - UI_TOPINFO_Y)
#define UI_MAINWINDOW_X 0
#define UI_MAINWINDOW_Y (UI_TOPINFO_Y + UI_TOPINFO_H)
#define UI_BOTTOMINFO_X 0
#define UI_BOTTOMINFO_Y (UI_MAINWINDOW_Y + UI_MAINWINDOW_H)

#define UI_MAINWINDOW_PROCESS_FG_X_OFFSET ((PROCESS_BAR_BG_W - PROCESS_BAR_FG_W) / 2)
#define UI_MAINWINDOW_PROCESS_FG_Y_OFFSET ((PROCESS_BAR_BG_H - PROCESS_BAR_FG_H) / 2)

#define SESSION_TIMER_IN_MS           (15000)
#define ELEVATOR_MOTION_TIMEOUT_IN_MS (1000)

#if LVGL_MULTITHREAD_LOCK
#define LVGL_LOCK()   _takeLVGLMutex()
#define LVGL_UNLOCK() _giveLVGLMutex()
#else
#define LVGL_LOCK()
#define LVGL_UNLOCK()
#endif /* LVGL_MULTITHREAD_LOCK */

typedef enum _wake_up_source
{
    WAKE_UP_SOURCE_INVALID = 0,
    WAKE_UP_SOURCE_BUTTON,
    WAKE_UP_SOURCE_TOUCH,
    WAKE_UP_SOURCE_VOICE,
    WAKE_UP_SOURCE_COUNT
} wake_up_source_t;

typedef enum _elevator_action_t
{
    kElevatorAction_FloorOne   = 0,
    kElevatorAction_FloorTwo   = 1,
    kElevatorAction_FloorThree = 2,
    kElevatorAction_FloorFour  = 3,
    kElevatorAction_FloorFive  = 4,
    kElevatorAction_FloorSix   = 5,
    kElevatorAction_Confirm    = 6,
    kElevatorAction_Cancel     = 7,
    kElevatorAction_Deregister = 8,
    kElevatorAction_Invalid
} elevator_action_t;

static char *s_PromptName[PROMPT_INVALID + 1] = {"Confirm Tone",
                                                 "Timeout Tone",
                                                 "Alarm Tone",
                                                 "Open Door",
                                                 "Floor One",
                                                 "Floor Two",
                                                 "Floor Three",
                                                 "Floor Four",
                                                 "Floor Five",
                                                 "Floor Six",
                                                 "Floor One, Confirm or Cancel?",
                                                 "Floor Two, Confirm or Cancel?",
                                                 "Floor Three, Confirm or Cancel?",
                                                 "Floor Four, Confirm or Cancel?",
                                                 "Floor Five, Confirm or Cancel?",
                                                 "Floor Six, Confirm or Cancel?",
                                                 "Register your selection floor?",
                                                 "Which floor?",
                                                 "PROMPT INVALID"};

static gfx_surface_t s_UiSurface;

SDK_ALIGN(static char s_AsBuffer[UI_BUFFER_WIDTH * UI_BUFFER_HEIGHT * UI_BUFFER_BPP], 32);

static int s_IsWaitingUsersFloor       = 0;
static int s_IsWaitingRegisterFloor    = 0;
static int s_Recognized                = 0;
static wake_up_source_t s_WakeUpSource = WAKE_UP_SOURCE_INVALID;
static bool s_VoiceSessionStarted      = false;
static int s_UserId                    = 0;
static uint32_t s_UserFloorNum         = 0;
static event_voice_t s_VoiceEvent      = {0};
static TimerHandle_t s_SessionTimer    = NULL;
static bool s_FaceRecOSDEnable         = false;
static bool s_BlockDeleteUser          = false;
static elevator_state_t s_ElevatorState;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

static hal_output_status_t _InferComplete_Vision(const output_dev_t *dev, void *inferResult);
static hal_output_status_t _InferComplete_Voice(const output_dev_t *dev, void *inferResult);

static hal_output_status_t HAL_OutputDev_UiElevator_Init(output_dev_t *dev, output_dev_callback_t callback);
static hal_output_status_t HAL_OutputDev_UiElevator_Deinit(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_UiElevator_Start(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_UiElevator_Stop(const output_dev_t *dev);
static void _StopFaceRec(int stop);
static void _SetVoiceModel(asr_inference_t modelId, asr_language_t lang, uint8_t ptt);

__attribute__((weak)) uint32_t APP_OutputDev_UiElevator_InferCompleteDecode(output_algo_source_t source,
                                                                            void *inferResult)
{
    return 0;
}
__attribute__((weak)) uint32_t APP_OutputDev_UiElevator_InputNotifyDecode(event_base_t *inputData)
{
    return 0;
}

static hal_output_status_t HAL_OutputDev_UiElevator_InferComplete(const output_dev_t *dev,
                                                                  output_algo_source_t source,
                                                                  void *inferResult);
static hal_output_status_t HAL_OutputDev_UiElevator_InputNotify(const output_dev_t *dev, void *data);

#if defined(__cplusplus)
}
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

const static output_dev_operator_t s_OutputDev_UiElevatorOps = {
    .init   = HAL_OutputDev_UiElevator_Init,
    .deinit = HAL_OutputDev_UiElevator_Deinit,
    .start  = HAL_OutputDev_UiElevator_Start,
    .stop   = HAL_OutputDev_UiElevator_Stop,
};

static output_dev_t s_OutputDev_UiElevator = {
    .name          = "UiElevator",
    .attr.type     = kOutputDevType_UI,
    .attr.pSurface = &s_UiSurface,
    .ops           = &s_OutputDev_UiElevatorOps,
};

const static output_dev_event_handler_t s_OutputDev_UiElevatorHandler = {
    .inferenceComplete = HAL_OutputDev_UiElevator_InferComplete,
    .inputNotify       = HAL_OutputDev_UiElevator_InputNotify,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void _NotifyFaceRecDebugOption(bool option)
{
    static event_face_rec_t s_FaceRecEvent;
    LOGD("[UI] Notify face rec debug option:%d", option);
    output_event_t output_event = {0};
    unsigned int data           = option;

    output_event.eventId   = kOutputEvent_VisionAlgoInputNotify;
    output_event.data      = &s_FaceRecEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_FaceRecEvent);
    output_event.eventInfo = kEventInfo_Remote;

    s_FaceRecEvent.eventBase.eventId = kEventFaceRecID_OasisDebugOption;
    s_FaceRecEvent.data              = (void *)data;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_UiElevator.cap.callback(s_OutputDev_UiElevator.id, output_event, fromISR);
}

bool FaceRecDebugOptionTrigger(void)
{
#define MAGIC_COUNT 3

    static int clicked_count = 0;

    clicked_count++;
    if (clicked_count == MAGIC_COUNT)
    {
        clicked_count      = 0;
        s_FaceRecOSDEnable = !s_FaceRecOSDEnable;
        _NotifyFaceRecDebugOption(s_FaceRecOSDEnable);
    }

    LOGD("[UI] FaceRec debug option clicked:%d OSD enable:%d", clicked_count, s_FaceRecOSDEnable);

    return s_FaceRecOSDEnable;
}

static language_type_t _ConvertASRLanguageToUILanguage(asr_language_t language)
{
    language_type_t uiLanguage;

    switch (language)
    {
        case ASR_ENGLISH:
            uiLanguage = kLanguageType_English;
            break;
        case ASR_CHINESE:
            uiLanguage = kLanguageType_Chinese;
            break;
        case ASR_GERMAN:
            uiLanguage = kLanguageType_German;
            break;
        case ASR_FRENCH:
            uiLanguage = kLanguageType_French;
            break;
        default:
            uiLanguage = kLanguageType_Invalid;
            break;
    }

    return uiLanguage;
}

static asr_language_t _ConvertUILanguageToASRLanguage(language_type_t language)
{
    asr_language_t asrLanguage;

    switch (language)
    {
        case kLanguageType_English:
            asrLanguage = ASR_ENGLISH;
            break;
        case kLanguageType_Chinese:
            asrLanguage = ASR_CHINESE;
            break;
        case kLanguageType_German:
            asrLanguage = ASR_GERMAN;
            break;
        case kLanguageType_French:
            asrLanguage = ASR_FRENCH;
            break;
        default:
            asrLanguage = UNDEFINED_LANGUAGE;
            break;
    }

    return asrLanguage;
}

static void _CleanFaceInfo()
{
    s_UserId                 = 0;
    s_Recognized             = false;
    s_IsWaitingRegisterFloor = false;
    s_IsWaitingUsersFloor    = false;
    s_BlockDeleteUser        = false;
    s_UserFloorNum           = 0;
    gui_home_update_face_rec_state(kFaceRec_NoUser);
    gui_enable_confirm_cancel(false);
}

static void _SessionTimer_Stop()
{
    if (s_SessionTimer != NULL)
    {
        xTimerStop(s_SessionTimer, 0);
        s_VoiceSessionStarted = false;
        gui_enable_mic(false);
    }
}

static void _CoolbackTimer_Callback(TimerHandle_t xTimer)
{
    asr_language_t language = _ConvertUILanguageToASRLanguage(gui_get_language_type());
    LOGD("[UI] Voice ended face rec:%d language:%d", s_Recognized, language)

    if (s_Recognized == true)
    {
        _CleanFaceInfo();
    }

    _SetVoiceModel(ASR_WW, language, 0);
    s_VoiceSessionStarted = false;
    gui_enable_mic(false);

    _StopFaceRec(0);
}

static void _SessionTimer_Callback(TimerHandle_t xTimer)
{
    LVGL_LOCK();
    LOGD("[UI] Session ended voice:%d face rec:%d", s_VoiceSessionStarted, s_Recognized)

    asr_language_t language = _ConvertUILanguageToASRLanguage(gui_get_language_type());

    _SetVoiceModel(ASR_WW, language, 0);
    s_WakeUpSource        = WAKE_UP_SOURCE_INVALID;
    s_VoiceSessionStarted = false;
    gui_enable_mic(false);

    if (s_Recognized == true)
    {
        _CleanFaceInfo();
    }

    /* restart the face rec */
    _StopFaceRec(0);

    _idle_callback();

    LVGL_UNLOCK();
}

static void _CooldownTimer_Start()
{
    static TimerHandle_t s_cooldownTimer = NULL;
    if (s_cooldownTimer == NULL)
    {
        // create the timer
        s_cooldownTimer = xTimerCreate("SessionTimer", (TickType_t)pdMS_TO_TICKS(2000), pdFALSE, NULL,
                                       (TimerCallbackFunction_t)_CoolbackTimer_Callback);
        if (s_cooldownTimer == NULL)
        {
            LOGE("[UI] Failed to create \"CooldownTimer\" timer.");
            return;
        }
    }

    if (xTimerStart(s_cooldownTimer, 0) != pdPASS)
    {
        LOGE("[UI] Failed to start \"CooldownTimer\" timer.");
    }
}

static void _SessionTimer_Start()
{
    if (s_SessionTimer == NULL)
    {
        // create the timer
        s_SessionTimer = xTimerCreate("SessionTimer", (TickType_t)pdMS_TO_TICKS(SESSION_TIMER_IN_MS), pdFALSE, NULL,
                                      (TimerCallbackFunction_t)_SessionTimer_Callback);
        if (s_SessionTimer == NULL)
        {
            LOGE("[UI] Failed to start \"SessionTimer\" timer.");
            return;
        }
    }

    if (xTimerStart(s_SessionTimer, 0) != pdPASS)
    {
        LOGE("[UI] Failed to start \"SessionTimer\" timer.");
    }
}

/**
 * Set session timer period. The value is in ms
 */
static void _SessionTimer_SetPeriod(uint32_t newPeriodMs)
{
    if (s_SessionTimer != NULL)
    {
        LOGD("[UI] Set session timer period newPeriod %d ms", newPeriodMs);
        xTimerChangePeriod(s_SessionTimer, (TickType_t)pdMS_TO_TICKS(newPeriodMs), 0);
    }
}

static void _StartVoiceSesion()
{
    LOGD("[UI] StartVoiceSesion");

    _SessionTimer_Start();
    gui_enable_mic(true);
    s_VoiceSessionStarted = true;
}

static void _SetPromptLanguage(asr_language_t language)
{
    static event_voice_t prompt_event;
    LOGD("[UI] Set prompt language %d", language);

    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_OutputInputNotify;
    output_event.data      = &prompt_event;
    output_event.copy      = 1;
    output_event.size      = sizeof(prompt_event);
    output_event.eventInfo = kEventInfo_DualCore;

    /* Prepare message for MQS */
    prompt_event.event_base.eventId                = SET_MULTILINGUAL_CONFIG;
    prompt_event.set_multilingual_config.languages = language;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_UiElevator.cap.callback(s_OutputDev_UiElevator.id, output_event, fromISR);
}

static void _SetVoiceModel(asr_inference_t modelId, asr_language_t lang, uint8_t ptt)
{
    LOGD("[UI] Set voice model:%d, language %d, ptt %d", modelId, lang, ptt);

    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_VoiceAlgoInputNotify;
    output_event.data      = &s_VoiceEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_VoiceEvent);
    output_event.eventInfo = kEventInfo_Remote;

    s_VoiceEvent.event_base.eventId  = SET_VOICE_MODEL;
    s_VoiceEvent.set_asr_config.demo = modelId;
    s_VoiceEvent.set_asr_config.lang = lang;
    s_VoiceEvent.set_asr_config.ptt  = ptt;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_UiElevator.cap.callback(s_OutputDev_UiElevator.id, output_event, fromISR);
}

void PlayPrompt(int id, uint8_t asrEnabled)
{
    static event_common_t s_PlayPromptEvent;
    LOGD("[UI] Play prompt \"%s\"", s_PromptName[id]);
    if (id >= PROMPT_CONFIRM_TONE && id < PROMPT_INVALID)
    {
        output_event_t output_event = {0};

        output_event.eventId   = kOutputEvent_OutputInputNotify;
        output_event.data      = &s_PlayPromptEvent;
        output_event.copy      = 1;
        output_event.size      = sizeof(s_PlayPromptEvent);
        output_event.eventInfo = kEventInfo_Remote;

        s_PlayPromptEvent.eventBase.eventId     = kEventID_PlayPrompt;
        s_PlayPromptEvent.eventBase.eventInfo   = kEventInfo_Remote;
        s_PlayPromptEvent.promptInfo.id         = id;
        s_PlayPromptEvent.promptInfo.asrEnabled = asrEnabled;

        uint8_t fromISR = __get_IPSR();
        s_OutputDev_UiElevator.cap.callback(s_OutputDev_UiElevator.id, output_event, fromISR);
    }
}

void StopPrompt(void)
{
    static event_common_t s_StopPromptEvent;
    LOGD("[UI] Stop prompt");

    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_OutputInputNotify;
    output_event.data      = &s_StopPromptEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_StopPromptEvent);
    output_event.eventInfo = kEventInfo_Remote;

    s_StopPromptEvent.eventBase.eventId = kEventID_StopPrompt;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_UiElevator.cap.callback(s_OutputDev_UiElevator.id, output_event, fromISR);
}

static int _NeedToAskRegister(void)
{
    uint32_t floorNum = get_current_floor();
    if ((s_Recognized) && (s_UserId == -1) && (floorNum != 1))
    {
        /* new user */
        return 1;
    }

    return 0;
}

static void _StopVoiceCmd(void)
{
    LOGD("[UI] Stop voice command");
    output_event_t output_event = {0};
    s_WakeUpSource              = WAKE_UP_SOURCE_INVALID;

    output_event.eventId   = kOutputEvent_VoiceAlgoInputNotify;
    output_event.data      = &s_VoiceEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_VoiceEvent);
    output_event.eventInfo = kEventInfo_Remote;

    s_VoiceEvent.event_base.eventId = STOP_VOICE_CMD_SESSION;

    uint8_t fromISR = __get_IPSR();

    s_OutputDev_UiElevator.cap.callback(s_OutputDev_UiElevator.id, output_event, fromISR);
}

static void _StopFaceRec(int stop)
{
    static event_face_rec_t s_FaceRecEvent;
    LOGD("[UI] Stop face rec:%d", stop);
    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_VisionAlgoInputNotify;
    output_event.data      = &s_FaceRecEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_FaceRecEvent);
    output_event.eventInfo = kEventInfo_Remote;

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
    s_OutputDev_UiElevator.cap.callback(s_OutputDev_UiElevator.id, output_event, fromISR);
}

static void _RegisterUser(uint32_t floorNum, asr_language_t language)
{
    static event_smart_tlhmi_t s_TlhmiEvent;
    LOGD("[UI] Register user's floor:%d, language:%d", floorNum, language);
    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_VisionAlgoInputNotify;
    output_event.data      = &s_TlhmiEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_TlhmiEvent);
    output_event.eventInfo = kEventInfo_Remote;

    s_TlhmiEvent.eventBase.eventId = kEventElevatorId_RegisterFloor;

    s_TlhmiEvent.elevatorInfo.id       = s_UserId;
    s_TlhmiEvent.elevatorInfo.floorNum = floorNum;
    s_TlhmiEvent.elevatorInfo.language = language;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_UiElevator.cap.callback(s_OutputDev_UiElevator.id, output_event, fromISR);
}

void DeregisterUser(void)
{
    static event_face_rec_t s_FaceRecEvent;
    LOGD("[UI] Deregister user:%d %d %d", s_UserId, s_Recognized, s_BlockDeleteUser);

    if (s_BlockDeleteUser)
    {
        return;
    }

    if (s_Recognized && s_UserId >= 0)
    {
        _CleanFaceInfo();

        output_event_t output_event = {0};

        output_event.eventId   = kOutputEvent_VisionAlgoInputNotify;
        output_event.data      = &s_FaceRecEvent;
        output_event.copy      = 1;
        output_event.size      = sizeof(s_FaceRecEvent);
        output_event.eventInfo = kEventInfo_Remote;

        s_FaceRecEvent.eventBase.eventId = kEventFaceRecID_DelUser;
        s_FaceRecEvent.delFace.hasID     = 0;
        s_FaceRecEvent.delFace.hasName   = 0;

        uint8_t fromISR = __get_IPSR();
        s_OutputDev_UiElevator.cap.callback(s_OutputDev_UiElevator.id, output_event, fromISR);
    }
}

void _VoiceWakeUp(wake_up_source_t source)
{
    LOGD("[UI] Voice wakeup:%d:%d", source, s_VoiceSessionStarted);

    if ((source == WAKE_UP_SOURCE_INVALID) || (source == WAKE_UP_SOURCE_COUNT))
    {
        return;
    }

    if (source == WAKE_UP_SOURCE_VOICE)
    {
        s_WakeUpSource          = source;
        asr_language_t language = _ConvertUILanguageToASRLanguage(gui_get_language_type());
        _SetVoiceModel(ASR_CMD_ELEVATOR, language, 0);
        _StartVoiceSesion();
    }
}

uint8_t UI_ElevatorStart_Callback(void)
{
    LOGD("[UI] ElevatorStart %d:%d", s_IsWaitingRegisterFloor, s_IsWaitingUsersFloor);

    _SessionTimer_Stop();
    gui_enable_mic(false);
    s_VoiceSessionStarted = false;

    if (s_IsWaitingRegisterFloor || s_IsWaitingUsersFloor)
    {
        asr_language_t language = _ConvertUILanguageToASRLanguage(gui_get_language_type());
        // clean confirm/cancel
        gui_enable_confirm_cancel(false);
        _SetVoiceModel(ASR_WW, language, 0);
    }
    else
    {
        _StopVoiceCmd();
    }

    return 0;
}

void UI_ElevatorArrived_Callback(void)
{
    LOGD("[UI] ElevatorArrived");
    asr_language_t language = _ConvertUILanguageToASRLanguage(gui_get_language_type());

    if (_NeedToAskRegister())
    {
        // gui_enable_confirm_cancel();
        _SetVoiceModel(ASR_CMD_ELEVATOR, language, 0);
        PlayPrompt(PROMPT_REGISTER_SELECTION, 0);
        s_IsWaitingRegisterFloor = true;
        s_BlockDeleteUser        = true;
    }
    else
    {
        _CooldownTimer_Start();
        _SessionTimer_Start();
    }
}

void UI_SetLanguage_Callback(language_type_t language)
{
    asr_language_t asrUserLanguage = _ConvertUILanguageToASRLanguage(language);

    LOGD("[UI] Set language:%d voice:%d", asrUserLanguage, s_VoiceSessionStarted);

    _SetPromptLanguage(asrUserLanguage);
    _SetVoiceModel(UNDEFINED_INFERENCE, asrUserLanguage, 0);
}

uint8_t UI_EnterScreen_Callback(screen_t screenId)
{
    LOGD("[UI] Enter screen:%s", get_screen_name(screenId));
    uint8_t changeScreen = 0;

    switch (screenId)
    {
        case kScreen_Home:
        {
            // start the face rec if no user detected
            face_rec_state_t faceRecState = get_current_face_rec_state();
            if ((faceRecState == kFaceRec_NoUser) || (faceRecState == kFaceRec_NumStates))
            {
                _StopFaceRec(0);
            }

            if (s_VoiceSessionStarted == false)
            {
                asr_language_t language = _ConvertUILanguageToASRLanguage(gui_get_language_type());
                _SetVoiceModel(ASR_WW, language, 0);
            }

            changeScreen = 1;
        }
        break;
        case kScreen_Help:
        {
            changeScreen = 1;
        }
        break;
        default:
            break;
    }

    return changeScreen;
}

void UI_Confirm_Callback(void)
{
    LOGD("[UI] Confirm callback %d:%d", s_IsWaitingRegisterFloor, s_IsWaitingUsersFloor);

    if (s_IsWaitingRegisterFloor)
    {
        s_IsWaitingRegisterFloor = 0;
        asr_language_t language  = _ConvertUILanguageToASRLanguage(gui_get_language_type());
        uint32_t floorNum        = get_current_floor();
        _RegisterUser(floorNum, language);
        _CleanFaceInfo();

        _StopVoiceCmd();
        s_VoiceSessionStarted = false;
        gui_enable_mic(false);

        _CooldownTimer_Start();
    }

    if (s_IsWaitingUsersFloor)
    {
        s_IsWaitingUsersFloor = 0;

        _SessionTimer_Stop();
        _StopVoiceCmd();

        go_to_floor(s_UserFloorNum);
        // disable the confirm/cancel
        gui_enable_confirm_cancel(false);
    }
}

void UI_Cancel_Callback(void)
{
    LOGD("[UI] Cancel callback %d:%d", s_IsWaitingRegisterFloor, s_IsWaitingUsersFloor);

    if (s_IsWaitingRegisterFloor)
    {
        s_IsWaitingRegisterFloor = 0;
        // disable the confirm/cancel
        gui_enable_confirm_cancel(false);

        /* Stop voice commands */
        _StopVoiceCmd();
        gui_enable_mic(false);

        _CooldownTimer_Start();
    }

    if (s_IsWaitingUsersFloor)
    {
        s_IsWaitingUsersFloor = 0;
        // disable the confirm/cancel
        gui_enable_confirm_cancel(false);
        _SessionTimer_Start();
    }
}

static hal_output_status_t HAL_OutputDev_UiElevator_Init(output_dev_t *dev, output_dev_callback_t callback)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_UiElevator_Init");

    dev->cap.callback = callback;

    /* Add initialization code here */
    s_UiSurface.left   = 0;
    s_UiSurface.top    = 0;
    s_UiSurface.right  = UI_BUFFER_WIDTH - 1;
    s_UiSurface.bottom = UI_BUFFER_HEIGHT - 1;
    s_UiSurface.height = UI_BUFFER_HEIGHT;
    s_UiSurface.width  = UI_BUFFER_WIDTH;
    s_UiSurface.pitch  = UI_BUFFER_WIDTH * 2;
    s_UiSurface.format = kPixelFormat_RGB565;
    s_UiSurface.buf    = s_AsBuffer;
    s_UiSurface.lock   = xSemaphoreCreateMutex();

    LOGD("--HAL_OutputDev_UiElevator_Init");
    return error;
}

static hal_output_status_t HAL_OutputDev_UiElevator_Deinit(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_UiLvgl_Deinit");

    /* Add de-initialization code here */

    LOGD("--HAL_OutputDev_UiLvgl_Deinit");
    return error;
}

static hal_output_status_t HAL_OutputDev_UiElevator_Start(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_UiElevator_Start");

    /* Add start code here */
    if (FWK_OutputManager_RegisterEventHandler(dev, &s_OutputDev_UiElevatorHandler) != 0)
    {
        error = kStatus_HAL_OutputError;
    }

    LOGD("--HAL_OutputDev_UiElevator_Start");
    return error;
}

static hal_output_status_t HAL_OutputDev_UiElevator_Stop(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_UiElevator_Stop");

    /* Add stop code here */

    LOGD("--HAL_OutputDev_UiElevator_Stop");
    return error;
}

static int _GetFloorNum(elevator_action_t action)
{
    int floorNum = -1;
    switch (action)
    {
        case (kElevatorAction_FloorOne):
        {
            floorNum = 1;
        }
        break;
        case (kElevatorAction_FloorTwo):
        {
            floorNum = 2;
        }
        break;
        case (kElevatorAction_FloorThree):
        {
            floorNum = 3;
        }
        break;
        case (kElevatorAction_FloorFour):
        {
            floorNum = 4;
        }
        break;
        case (kElevatorAction_FloorFive):
        {
            floorNum = 5;
        }
        break;
        case (kElevatorAction_FloorSix):
        {
            floorNum = 6;
        }
        break;
        default:
            break;
    }

    return floorNum;
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
        elevator_state_t eState = get_elevator_state();
        LOGD("[UI] Face recognized:%d face_id:%d eState:%d", pResult->face_recognized, pResult->face_id, eState);

        if (s_FaceRecOSDEnable)
        {
            gui_set_algo_debuginfo(pResult->face_count, pResult->debug_info.isSmallFace, pResult->debug_info.isBlurry,
                                   pResult->debug_info.isSideFace, pResult->debug_info.rgbBrightness,
                                   pResult->face_recognized, pResult->face_id);
        }

        if (pResult->face_recognized && pResult->face_id == INVALID_FACE_ID)
        {
            // new user
            s_Recognized = 1;
            s_UserId     = -1;
            gui_home_update_face_rec_state(kFaceRec_NewUser);
            _SessionTimer_Start();
        }
        else if (pResult->face_recognized && pResult->face_id != INVALID_FACE_ID)
        {
            // face recognized (known user)
            // store the user's selection

            // ignore the user's registered floor if the elevator is not in idle
            if (eState != kElevatorState_Idle)
            {
                return error;
            }

            elevator_info_t *pAttr = (elevator_info_t *)pResult->userData;

            LOGD("[UI] User info id: %d, floor: %d, language: %d", pAttr->id, pAttr->floorNum, pAttr->language);
            asr_language_t language = _ConvertUILanguageToASRLanguage(gui_get_language_type());

            s_UserFloorNum = pAttr->floorNum;
            s_Recognized   = 1;
            s_UserId       = pResult->face_id;

            if (s_WakeUpSource == WAKE_UP_SOURCE_VOICE)
            {
                if (language != pAttr->language)
                {
                    /* Need to update language selection in database */
                    _RegisterUser(s_UserFloorNum, language);
                }
            }
            else if (language != pAttr->language)
            {
                /* Need to update the voice prompt */
                _SetPromptLanguage(language);
                gui_home_set_language(_ConvertASRLanguageToUILanguage(pAttr->language));

                language = pAttr->language;
            }

            gui_home_update_face_rec_state(kFaceRec_KnownUser);

            uint32_t curFloor = get_current_floor();
            LOGD("[UI] user's floor:%d, current floor:%d", s_UserFloorNum, curFloor);

            // go to user's registered selection?
            int promptId = PROMPT_INVALID;

            if (curFloor == s_UserFloorNum)
            {
                s_IsWaitingUsersFloor = 0;
                promptId              = PROMPT_WHICH_FLOOR;
                /* Update the language in ASR*/
                _SetVoiceModel(ASR_CMD_ELEVATOR, language, 0);
            }
            else
            {
                s_IsWaitingUsersFloor = 1;
                switch (s_UserFloorNum)
                {
                    case 1:
                        promptId = PROMPT_ONE_CONFIRM_CANCEL;
                        break;
                    case 2:
                        promptId = PROMPT_TWO_CONFIRM_CANCEL;
                        break;
                    case 3:
                        promptId = PROMPT_THREE_CONFIRM_CANCEL;
                        break;
                    case 4:
                        promptId = PROMPT_FOUR_CONFIRM_CANCEL;
                        break;
                    case 5:
                        promptId = PROMPT_FIVE_CONFIRM_CANCEL;
                        break;
                    case 6:
                        promptId = PROMPT_SIX_CONFIRM_CANCEL;
                        break;
                    default:
                        break;
                }
                _SetVoiceModel(ASR_CMD_ELEVATOR, language, 0);
                s_BlockDeleteUser = true;
            }

            _StartVoiceSesion();

            if (promptId != PROMPT_INVALID)
            {
                PlayPrompt(promptId, 0);
            }
        }
    }

    return error;
}

static hal_output_status_t _InferComplete_Voice(const output_dev_t *dev, void *inferResult)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    asr_inference_result_t *voiceAlgoResult = (asr_inference_result_t *)inferResult;
    bool elevatorInMotion                   = (get_elevator_state() == kElevatorState_Start) ? true : false;
    screen_t screenId                       = gui_get_screen_id();

    LOGD("[UI] Screen:%d voice status:%d cmd:%d state:%d:%d:%d:%d", screenId, voiceAlgoResult->status,
         voiceAlgoResult->keywordID, s_VoiceSessionStarted, elevatorInMotion, s_IsWaitingUsersFloor,
         s_IsWaitingRegisterFloor);

    if (elevatorInMotion == true)
    {
        /* Discard new messages */
        return error;
    }
    language_type_t language = _ConvertASRLanguageToUILanguage(voiceAlgoResult->language);

    if ((s_VoiceSessionStarted == false) && (voiceAlgoResult->status == ASR_WW_DETECT))
    {
        gui_home_set_language(language);
        // wake word detected
        _VoiceWakeUp(WAKE_UP_SOURCE_VOICE);
    }
    else if ((voiceAlgoResult->status == ASR_CMD_DETECT) && (voiceAlgoResult->keywordID > -1))
    {
        if (s_IsWaitingRegisterFloor)
        {
            uint32_t floorNum = get_current_floor();

            if (voiceAlgoResult->keywordID == kElevatorAction_Confirm)
            {
                UI_Confirm_Callback();
            }
            else if (voiceAlgoResult->keywordID == kElevatorAction_Cancel)
            {
                UI_Cancel_Callback();
            }
        }
        else if (s_IsWaitingUsersFloor)
        {
            if (voiceAlgoResult->keywordID == kElevatorAction_Confirm)
            {
                PlayPrompt(PROMPT_CONFIRM_TONE, 1);
                UI_Confirm_Callback();
            }
            else if (voiceAlgoResult->keywordID == kElevatorAction_Cancel)
            {
                PlayPrompt(PROMPT_CONFIRM_TONE, 1);
                UI_Cancel_Callback();
            }
        }

        // voice command detected
        switch (voiceAlgoResult->keywordID)
        {
            case (kElevatorAction_FloorOne):
            case (kElevatorAction_FloorTwo):
            case (kElevatorAction_FloorThree):
            case (kElevatorAction_FloorFour):
            case (kElevatorAction_FloorFive):
            case (kElevatorAction_FloorSix):
            {
                int floorNum = _GetFloorNum(voiceAlgoResult->keywordID);
                if (floorNum >= 0)
                {
                    go_to_floor(floorNum);
                }
            }
            break;
            case (kElevatorAction_Deregister):
            {
                PlayPrompt(PROMPT_CONFIRM_TONE, 1);
                DeregisterUser();
            }
            break;
            default:
                break;
        }
    }

    return error;
}

static hal_output_status_t HAL_OutputDev_UiElevator_InferComplete(const output_dev_t *dev,
                                                                  output_algo_source_t source,
                                                                  void *inferResult)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    if (inferResult == NULL)
    {
        return error;
    }

    LVGL_LOCK();

#if AQT_TEST
    if (source == kOutputAlgoSource_Voice)
    {
        _InferComplete_Voice(dev, inferResult);
    }
#else
    if (source == kOutputAlgoSource_Vision)
    {
        _InferComplete_Vision(dev, inferResult);
    }
    else if (source == kOutputAlgoSource_Voice)
    {
        _InferComplete_Voice(dev, inferResult);
    }
#endif /* AQT_TEST */

    LVGL_UNLOCK();

    return error;
}

static hal_output_status_t HAL_OutputDev_UiElevator_InputNotify(const output_dev_t *dev, void *data)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    event_base_t *pEventBase  = (event_base_t *)data;

    if (pEventBase->eventId == kEventID_SessionTimeout)
    {
        uint32_t sessionTimeoutMs = (uint32_t)((event_common_t *)data)->data;
        if (sessionTimeoutMs != 0)
        {
            _SessionTimer_SetPeriod(sessionTimeoutMs);
        }
    }
    else if (pEventBase->eventId == kEventID_WakeUp)
    {
        // WakeUp(WAKE_UP_SOURCE_BUTTON);
    }
    else if (pEventBase->eventId == kEventID_PlayPromptDone)
    {
        int promptId = (int)((event_common_t *)data)->data;
        if ((promptId == PROMPT_REGISTER_SELECTION) ||
            ((promptId >= PROMPT_ONE_CONFIRM_CANCEL) && (promptId <= PROMPT_SIX_CONFIRM_CANCEL)))
        {
            LVGL_LOCK();
            s_BlockDeleteUser = false;
            gui_enable_confirm_cancel(true);
            _StartVoiceSesion();
            LVGL_UNLOCK();
        }
        else if (promptId == PROMPT_ALARM_TONE)
        {
            static int alarmPlayTime = 1;
            if ((alarmPlayTime % 2) && (gui_get_alarm_status() == true))
            {
                PlayPrompt(PROMPT_ALARM_TONE, 1);
            }
            else if ((alarmPlayTime % 2) && (gui_get_alarm_status() == false))
            {
                ++alarmPlayTime;
            }
            else
            {
                gui_set_alarm_status(false);
            }
            ++alarmPlayTime;
        }
    }
    /* Add 'inputNotify' event handler code here */
    APP_OutputDev_UiElevator_InputNotifyDecode(pEventBase);

    return error;
}

int HAL_OutputDev_UiElevator_Register()
{
    int error = 0;
    LOGD("++HAL_OutputDev_UiElevator_Register");

    error = FWK_OutputManager_DeviceRegister(&s_OutputDev_UiElevator);

    LOGD("--HAL_OutputDev_UiElevator_Register");
    return error;
}

#endif /* ENABLE_OUTPUT_DEV_UiElevator */
