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
#ifdef ENABLE_OUTPUT_DEV_UiCoffeeMachine

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

#define VIRTUAL_FACE_W 420
#define VIRTUAL_FACE_H 426

#define PROCESS_BAR_BG_W 240
#define PROCESS_BAR_BG_H 14
#define PROCESS_BAR_FG_W 230
#define PROCESS_BAR_FG_H 10

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

#define RGB565_RED      0xf800
#define RGB565_GREEN    0x07e0
#define RGB565_BLUE     0x001f
#define RGB565_BLACK    0x0001
#define RGB565_NXPGREEN 0xBEA6
#define RGB565_NXPRED   0xFD83
#define RGB565_NXPBLUE  0x6D5B

#if LVGL_MULTITHREAD_LOCK
#define LVGL_LOCK()   _takeLVGLMutex()
#define LVGL_UNLOCK() _giveLVGLMutex()
#else
#define LVGL_LOCK()
#define LVGL_UNLOCK()
#endif /* LVGL_MULTITHREAD_LOCK */

typedef enum _face_rec_indicator
{
    FACE_REC_INDICATOR_INIT = 0,
    FACE_REC_INDICATOR_KNOWN,
    FACE_REC_INDICATOR_UNKNOWN,
    FACE_REC_INDICATOR_INVALID
} face_rec_indicator_type;

enum
{
    VOICE_CMD_START      = 0,
    VOICE_CMD_CANCEL     = 1,
    VOICE_CMD_CONFIRM    = 2,
    VOICE_CMD_ESPRESSO   = 3,
    VOICE_CMD_AMERICANO  = 4,
    VOICE_CMD_CAPPUCCINO = 5,
    VOICE_CMD_CAFE_LATTE = 6,
    VOICE_CMD_SMALL      = 7,
    VOICE_CMD_MEDIUM     = 8,
    VOICE_CMD_LARGE      = 9,
    VOICE_CMD_SOFT       = 10,
    VOICE_CMD_MILD       = 11,
    VOICE_CMD_STRONG     = 12,
    VOICE_CMD_DEREGISTER = 13,
    VOICE_CMD_INVALID
};

enum
{
    PROMPT_CONFIRM_TONE = 0,
    PROMPT_TONE_TIMEOUT,
    PROMPT_ANOTHER_ESPRESSO,
    PROMPT_ANOTHER_AMERICANO,
    PROMPT_ANOTHER_CAPPUCCINO,
    PROMPT_ANOTHER_CAFE_LATTE,
    PROMPT_REGISTER_SELECTION,

    PROMPT_INVALID
};

static char *s_PromptName[PROMPT_INVALID + 1] = {
    "Confirm Tone",       "Timeout",          "Another Expresso",   "Another Americano",
    "Another Cappuccino", "Another CafeLate", "Register Selection", "Invalid"};

enum
{
    ICON_PROGRESS_BAR = 0,
    ICON_VIRTUAL_FACE_BLUE,
    ICON_VIRTUAL_FACE_GREEN,
    ICON_VIRTUAL_FACE_RED,
    ICON_INVALID
};

typedef enum _wake_up_source
{
    WAKE_UP_SOURCE_INVALID = 0,
    WAKE_UP_SOURCE_BUTTON,
    WAKE_UP_SOURCE_TOUCH,
    WAKE_UP_SOURCE_VOICE,
    WAKE_UP_SOURCE_COUNT
} wake_up_source_t;

static char *s_Icons[ICON_INVALID];
static int s_GuiderColor[FACE_REC_INDICATOR_INVALID] = {RGB565_BLUE, RGB565_GREEN, RGB565_RED};

static gfx_surface_t s_UiSurface;
static gfx_surface_t s_VirtualFaceSurface;

SDK_ALIGN(static char s_AsBuffer[UI_BUFFER_WIDTH * UI_BUFFER_HEIGHT * UI_BUFFER_BPP], 32);
SDK_ALIGN(static char s_VirtualFaceBuffer[UI_BUFFER_WIDTH * UI_BUFFER_HEIGHT * UI_BUFFER_BPP], 32);

static lv_img_dsc_t s_VirtualFaceImage = {
    .header.always_zero = 0,
    .header.w           = UI_BUFFER_WIDTH,
    .header.h           = UI_BUFFER_HEIGHT,
    .data_size          = UI_BUFFER_WIDTH * UI_BUFFER_HEIGHT * LV_COLOR_SIZE / 8,
    .header.cf          = LV_IMG_CF_TRUE_COLOR,
};

static float s_FaceRecProgress              = 0.0f;
static TimerHandle_t s_FaceRecProgressTimer = NULL;
static bool s_FaceRecOSDEnable              = false;

#if AQT_TEST
#define SESSION_TIMER_IN_MS (15000)
#else
#define SESSION_TIMER_IN_MS (60000)
#endif /* AQT_TEST */
#define SESSION_UPDATE_INTERVALS (60)
#define FACE_REC_UPDATE_INTERVAL (SESSION_TIMER_IN_MS / SESSION_UPDATE_INTERVALS)

static TimerHandle_t s_SessionTimer = NULL;

static int s_IsWaitingAnotherSelection            = 0;
static int s_IsWaitingRegisterSelection           = 0;
static int s_Recognized                           = 0;
static int s_UserId                               = -1;
static wake_up_source_t s_WakeUpSource            = WAKE_UP_SOURCE_INVALID;
static uint8_t s_UserCoffeeType                   = kCoffeeType_NumTypes;
static uint8_t s_UserCoffeeSize                   = kCoffeeSize_NumSizes;
static uint8_t s_UserCoffeeStrength               = kCoffeeStrength_NumStrengths;
static asr_language_t s_UserLanguage              = ASR_ENGLISH;
static face_rec_indicator_type s_FaceRecIndicator = FACE_REC_INDICATOR_INIT;
static bool s_StandbyEnabled                      = true;
preview_mode_t g_PreviewMode                      = PREVIEW_MODE_CAMERA;
static event_voice_t s_VoiceEvent;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

static hal_output_status_t HAL_OutputDev_UiCoffeeMachine_Init(output_dev_t *dev, output_dev_callback_t callback);
static hal_output_status_t HAL_OutputDev_UiCoffeeMachine_Deinit(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_UiCoffeeMachine_Start(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_UiCoffeeMachine_Stop(const output_dev_t *dev);

static void _StopVoiceCmd(void);

__attribute__((weak)) uint32_t APP_OutputDev_UiCoffeeMachine_InferCompleteDecode(output_algo_source_t source,
                                                                                 void *inferResult)
{
    return 0;
}
__attribute__((weak)) uint32_t APP_OutputDev_UiCoffeeMachine_InputNotifyDecode(event_base_t *inputData)
{
    return 0;
}

static hal_output_status_t HAL_OutputDev_UiCoffeeMachine_InferComplete(const output_dev_t *dev,
                                                                       output_algo_source_t source,
                                                                       void *inferResult);
static hal_output_status_t HAL_OutputDev_UiCoffeeMachine_InputNotify(const output_dev_t *dev, void *data);

#if defined(__cplusplus)
}
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

const static output_dev_operator_t s_OutputDev_UiCoffeeMachineOps = {
    .init   = HAL_OutputDev_UiCoffeeMachine_Init,
    .deinit = HAL_OutputDev_UiCoffeeMachine_Deinit,
    .start  = HAL_OutputDev_UiCoffeeMachine_Start,
    .stop   = HAL_OutputDev_UiCoffeeMachine_Stop,
};

static output_dev_t s_OutputDev_UiCoffeeMachine = {
    .name          = "UiCoffeeMachine",
    .attr.type     = kOutputDevType_UI,
    .attr.pSurface = &s_UiSurface,
    .ops           = &s_OutputDev_UiCoffeeMachineOps,
};

const static output_dev_event_handler_t s_OutputDev_UiCoffeeMachineHandler = {
    .inferenceComplete = HAL_OutputDev_UiCoffeeMachine_InferComplete,
    .inputNotify       = HAL_OutputDev_UiCoffeeMachine_InputNotify,
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
    s_OutputDev_UiCoffeeMachine.cap.callback(s_OutputDev_UiCoffeeMachine.id, output_event, fromISR);
}

bool FaceRecDebugOptionTrigger()
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

void LoadIcons(void *base)
{
    s_Icons[ICON_PROGRESS_BAR] = (base + 0);

    s_Icons[ICON_VIRTUAL_FACE_BLUE]  = (base + 6720);
    s_Icons[ICON_VIRTUAL_FACE_GREEN] = (base + 364608);
    s_Icons[ICON_VIRTUAL_FACE_RED]   = (base + 722496);
    // Icons Total: 0x00107c40  1080384
}

static void _DrawVirtualFaceIcon(gfx_surface_t *pSurface, char *pIcon)
{
    if (pIcon)
    {
        int x = (UI_BUFFER_WIDTH - VIRTUAL_FACE_W) / 2;
        gfx_drawPicture(&s_VirtualFaceSurface, x, 0, VIRTUAL_FACE_W, VIRTUAL_FACE_H, 0xFFFF, pIcon);
    }
}

static void _DrawGuideRect(int color)
{
    // guide rect
    int w = UI_GUIDE_RECT_W / UI_SCALE_W;
    int h = UI_GUIDE_RECT_H / UI_SCALE_H;
    int x = (UI_BUFFER_WIDTH - w) / 2;
    int y = (UI_BUFFER_HEIGHT - h) / 2;
    int l = 100 / UI_SCALE_W;
    int d = 4 / UI_SCALE_W;
    gfx_drawRect(&s_UiSurface, x, y, l, d, color);
    gfx_drawRect(&s_UiSurface, x, y, d, l, color);
    gfx_drawRect(&s_UiSurface, x + w - l, y, l, d, color);
    gfx_drawRect(&s_UiSurface, x + w, y, d, l, color);
    gfx_drawRect(&s_UiSurface, x, y + h, l, d, color);
    gfx_drawRect(&s_UiSurface, x, y + h - l, d, l, color);
    gfx_drawRect(&s_UiSurface, x + w - l, y + h, l, d, color);
    gfx_drawRect(&s_UiSurface, x + w, y + h - l, d, l, color);
}

static void _DrawProgressBar(preview_mode_t previewMode, float percent)
{
    /* process bar background */
    int x = (UI_BUFFER_WIDTH - PROCESS_BAR_BG_W / UI_SCALE_W) / 2;
    int y;
    gfx_surface_t *pSurface;
    if (previewMode == PREVIEW_MODE_CAMERA)
    {
        y        = (UI_BUFFER_HEIGHT + UI_GUIDE_RECT_H / UI_SCALE_H) / 2 + 36;
        pSurface = &s_UiSurface;
    }
    else
    {
        y        = (UI_BUFFER_HEIGHT + VIRTUAL_FACE_H) / 2;
        pSurface = &s_VirtualFaceSurface;
    }
    gfx_drawPicture(pSurface, x, y, PROCESS_BAR_BG_W, PROCESS_BAR_BG_H, 0xFFFF, s_Icons[ICON_PROGRESS_BAR]);

    /* process bar foreground */
    gfx_drawRect(pSurface, x + UI_MAINWINDOW_PROCESS_FG_X_OFFSET, y + UI_MAINWINDOW_PROCESS_FG_Y_OFFSET,
                 (int)(PROCESS_BAR_FG_W * percent), PROCESS_BAR_FG_H, RGB565_NXPBLUE);
}

static void _DrawPreviewUI(preview_mode_t previewMode,
                           int drawIndicator,
                           face_rec_indicator_type faceRecIndicator,
                           int drawProgressBar,
                           float progressPercent)
{
    if (faceRecIndicator == FACE_REC_INDICATOR_KNOWN)
    {
        gui_enable_deregister_button(true);
    }
    else
    {
        gui_enable_deregister_button(false);
    }

    if (previewMode == PREVIEW_MODE_CAMERA)
    {
        if (s_UiSurface.lock)
        {
            xSemaphoreTake(s_UiSurface.lock, portMAX_DELAY);
        }

        if (drawIndicator)
        {
            _DrawGuideRect(s_GuiderColor[faceRecIndicator]);
        }

        if (drawProgressBar)
        {
            _DrawProgressBar(previewMode, progressPercent);
        }

        if (s_UiSurface.lock)
        {
            xSemaphoreGive(s_UiSurface.lock);
        }
    }
    else
    {
        char *pIcon = NULL;
        if (drawIndicator)
        {
            switch (faceRecIndicator)
            {
                case FACE_REC_INDICATOR_KNOWN:
                    pIcon = s_Icons[ICON_VIRTUAL_FACE_GREEN];
                    break;
                case FACE_REC_INDICATOR_UNKNOWN:
                    pIcon = s_Icons[ICON_VIRTUAL_FACE_RED];
                    break;
                default:
                    pIcon = s_Icons[ICON_VIRTUAL_FACE_BLUE];
                    break;
            };
        }
        s_VirtualFaceImage.data = s_VirtualFaceSurface.buf;
        _DrawVirtualFaceIcon(&s_VirtualFaceSurface, pIcon);
        if (drawProgressBar)
        {
            _DrawProgressBar(previewMode, progressPercent);
        }
        gui_set_virtual_face(&s_VirtualFaceImage);
    }
}

static language_t _ConvertASRLanguageToUILanguage(asr_language_t language)
{
    language_t uiLanguage;

    switch (language)
    {
        case ASR_ENGLISH:
            uiLanguage = kLanguage_EN;
            break;
        case ASR_CHINESE:
            uiLanguage = kLanguage_CN;
            break;
        case ASR_GERMAN:
            uiLanguage = kLanguage_DE;
            break;
        case ASR_FRENCH:
            uiLanguage = kLanguage_FR;
            break;
        default:
            uiLanguage = kLanguage_EN;
            break;
    }

    return uiLanguage;
}

static asr_language_t _ConvertUILanguageToASRLanguage(language_t language)
{
    asr_language_t asrLanguage;

    switch (language)
    {
        case kLanguage_EN:
            asrLanguage = ASR_ENGLISH;
            break;
        case kLanguage_CN:
            asrLanguage = ASR_CHINESE;
            break;
        case kLanguage_DE:
            asrLanguage = ASR_GERMAN;
            break;
        case kLanguage_FR:
            asrLanguage = ASR_FRENCH;
            break;
        default:
            asrLanguage = UNDEFINED_LANGUAGE;
            break;
    }

    return asrLanguage;
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
    s_OutputDev_UiCoffeeMachine.cap.callback(s_OutputDev_UiCoffeeMachine.id, output_event, fromISR);
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

    s_VoiceEvent.event_base.eventId   = SET_VOICE_MODEL;
    s_VoiceEvent.event_base.eventInfo = kEventInfo_Remote;
    s_VoiceEvent.set_asr_config.demo  = modelId;
    s_VoiceEvent.set_asr_config.lang  = lang;
    s_VoiceEvent.set_asr_config.ptt   = ptt;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_UiCoffeeMachine.cap.callback(s_OutputDev_UiCoffeeMachine.id, output_event, fromISR);
}

static void _PlayPrompt(int id, uint8_t asrEnabled)
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
        s_OutputDev_UiCoffeeMachine.cap.callback(s_OutputDev_UiCoffeeMachine.id, output_event, fromISR);
    }
}

static void _SessionTimer_Stop()
{
    if (s_SessionTimer != NULL)
    {
        xTimerStop(s_SessionTimer, 0);
    }
}

static void _SessionTimer_Callback(TimerHandle_t xTimer)
{
    screen_t currentScreenId = get_current_screen();
    LOGD("[UI] Screen:%s \"SessionTimer\" callback %d:%d", get_screen_name(currentScreenId), s_StandbyEnabled,
         s_IsWaitingRegisterSelection);

    _PlayPrompt(PROMPT_TONE_TIMEOUT, 0);
    s_IsWaitingRegisterSelection = 0;
    s_IsWaitingAnotherSelection  = 0;

    if (s_StandbyEnabled == false)
    {
        LVGL_LOCK();
        gui_set_standby();
        LVGL_UNLOCK();
    }

    _SessionTimer_Stop();
}

static void _SessionTimer_Start()
{
    if (s_SessionTimer == NULL)
    {
        // create the timer
        s_SessionTimer = xTimerCreate("SessionTimer", (TickType_t)pdMS_TO_TICKS(SESSION_TIMER_IN_MS), pdTRUE, NULL,
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

void WakeUp(wake_up_source_t source)
{
    LOGD("[UI] Wakeup:%d:%d", source, s_StandbyEnabled);

    s_WakeUpSource = source;

    if ((s_StandbyEnabled == true) || (get_current_screen() == kScreen_Standby))
    {
        // wake up word detected
        if (source == WAKE_UP_SOURCE_VOICE)
        {
            // wake up word detected
            _SetVoiceModel(ASR_CMD_COFFEE_MACHINE, s_UserLanguage, 0);
        }
        else
        {
            s_UserLanguage = _ConvertUILanguageToASRLanguage(get_language());
            _SetPromptLanguage(s_UserLanguage);
            // push to talk event detected
            _SetVoiceModel(ASR_CMD_COFFEE_MACHINE, s_UserLanguage, 1);
        }

        // go to home screen
        gui_set_home();
        gui_home_set_language(_ConvertASRLanguageToUILanguage(s_UserLanguage));

        _SessionTimer_Start();
    }
}

static void _StandBy(void)
{
    LOGD("[UI] Standby:%d", s_StandbyEnabled);
    // voice command session timeout
    if (s_StandbyEnabled == false)
    {
        s_IsWaitingRegisterSelection = 0;
        s_IsWaitingAnotherSelection  = 0;
        _PlayPrompt(PROMPT_TONE_TIMEOUT, 0);
        gui_set_standby();
    }
}

static int _NeedToAskRegister(void)
{
    if (s_Recognized && s_UserId == -1)
    {
        // new user
        return 1;
    }

    if (s_Recognized && s_UserId >= 0)
    {
        coffee_type_t newCoffeeType         = get_coffee_type();
        coffee_size_t newCoffeeSize         = get_coffee_size();
        coffee_strength_t newCoffeeStrength = get_coffee_strength();

        if ((newCoffeeType != s_UserCoffeeType) || (newCoffeeSize != s_UserCoffeeSize) ||
            (newCoffeeStrength != s_UserCoffeeStrength))
        {
            // user's new selection is different with registered selection
            return 1;
        }
    }

    return 0;
}

static void _FaceRecProgressTimer_Start()
{
    if (s_FaceRecProgressTimer != NULL)
    {
        s_FaceRecProgress = 0;

        if (xTimerStart(s_FaceRecProgressTimer, 0) != pdPASS)
        {
            LOGE("[UI] Failed to start \"FaceRecProgress\" timer.");
        }
    }
}

static void _FaceRecProgressTimer_Stop()
{
    if (s_FaceRecProgressTimer != NULL)
    {
        xTimerStop(s_FaceRecProgressTimer, 0);
    }
}

static void _FaceRecProgressTimer_Callback(TimerHandle_t xTimer)
{
    int drawUpdate = 0;
    if (s_FaceRecProgress < 1.0f)
    {
        s_FaceRecProgress += (float)1 / SESSION_UPDATE_INTERVALS;
        drawUpdate = 1;
    }
    else
    {
        xTimerStop(xTimer, 0);
    }

    if (drawUpdate)
    {
        _DrawPreviewUI(g_PreviewMode, 0, s_FaceRecIndicator, 1, s_FaceRecProgress);
    }
}

static void _StopVoiceCmd()
{
    LOGD("[UI] Stop voice command");
    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_VoiceAlgoInputNotify;
    output_event.data      = &s_VoiceEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_VoiceEvent);
    output_event.eventInfo = kEventInfo_Remote;

    s_VoiceEvent.event_base.eventId   = STOP_VOICE_CMD_SESSION;
    s_VoiceEvent.event_base.eventInfo = kEventInfo_Remote;

    uint8_t fromISR = __get_IPSR();

    s_OutputDev_UiCoffeeMachine.cap.callback(s_OutputDev_UiCoffeeMachine.id, output_event, fromISR);
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
    s_OutputDev_UiCoffeeMachine.cap.callback(s_OutputDev_UiCoffeeMachine.id, output_event, fromISR);
}

static void _RegisterCoffeeSelection(coffee_type_t type,
                                     coffee_size_t size,
                                     coffee_strength_t strength,
                                     asr_language_t language)
{
    static event_smart_tlhmi_t s_TlhmiEvent;
    LOGD("[UI] Register user:%d coffee selection %d:%d:%d:%d", s_UserId, type, size, strength, language);

    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_VisionAlgoInputNotify;
    output_event.data      = &s_TlhmiEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_TlhmiEvent);
    output_event.eventInfo = kEventInfo_Remote;

    s_TlhmiEvent.eventBase.eventId                            = kEventFaceRecId_RegisterCoffeeSelection;
    s_TlhmiEvent.eventBase.eventInfo                          = kEventInfo_Remote;
    s_TlhmiEvent.regCoffeeSelection.id                        = s_UserId;
    s_TlhmiEvent.regCoffeeSelection.coffeeInfo.coffeeType     = type;
    s_TlhmiEvent.regCoffeeSelection.coffeeInfo.coffeeSize     = size;
    s_TlhmiEvent.regCoffeeSelection.coffeeInfo.coffeeStrength = strength;
    s_TlhmiEvent.regCoffeeSelection.coffeeInfo.language       = language;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_UiCoffeeMachine.cap.callback(s_OutputDev_UiCoffeeMachine.id, output_event, fromISR);
}

void DeregisterCoffeeSelection()
{
    static event_face_rec_t s_FaceRecEvent;
    LOGD("[UI] Deregister user:%d %d", s_UserId, s_Recognized);
    if (s_Recognized && s_UserId >= 0)
    {
        output_event_t output_event = {0};

        output_event.eventId   = kOutputEvent_VisionAlgoInputNotify;
        output_event.data      = &s_FaceRecEvent;
        output_event.copy      = 1;
        output_event.size      = sizeof(s_FaceRecEvent);
        output_event.eventInfo = kEventInfo_Remote;

        s_FaceRecEvent.eventBase.eventId   = kEventFaceRecID_DelUser;
        s_FaceRecEvent.eventBase.eventInfo = kEventInfo_Remote;
        s_FaceRecEvent.delFace.hasID       = 0;
        s_FaceRecEvent.delFace.hasName     = 0;
        uint8_t fromISR                    = __get_IPSR();

        s_OutputDev_UiCoffeeMachine.cap.callback(s_OutputDev_UiCoffeeMachine.id, output_event, fromISR);
        s_Recognized = 0;
        s_UserId     = -1;

        s_FaceRecIndicator = FACE_REC_INDICATOR_INIT;
        _DrawPreviewUI(g_PreviewMode, 1, s_FaceRecIndicator, 0, s_FaceRecProgress);
    }
}

void UI_Finished_Callback()
{
    int ret = _NeedToAskRegister();
    LOGD("[UI] Finished Callback %d:%d:%d", ret, s_IsWaitingRegisterSelection, s_UserLanguage);
    if (ret)
    {
        // register selection?
        if (s_IsWaitingRegisterSelection == 0)
        {
            _SessionTimer_SetPeriod(SESSION_TIMER_IN_MS / 2);
            _SetVoiceModel(ASR_CMD_COFFEE_MACHINE, s_UserLanguage, 0);
            _PlayPrompt(PROMPT_REGISTER_SELECTION, 0);

            s_IsWaitingRegisterSelection = 1;
        }
    }
    else
    {
        _StandBy();
    }
}

void UI_WidgetInteraction_Callback()
{
#if AQT_TEST
#else
    _SessionTimer_Start();

    if (s_Recognized == 0)
    {
        /* The User has not been recognized */
        _FaceRecProgressTimer_Start();
        _DrawPreviewUI(g_PreviewMode, 1, s_FaceRecIndicator, 1, s_FaceRecProgress);
    }
#endif /* AQT_TEST */
}

void UI_SetLanguage_Callback(language_t language)
{
    asr_language_t uiUserLanguage = _ConvertUILanguageToASRLanguage(language);

    LOGD("[UI] Set old_language:%d language:%d standby:%d", s_UserLanguage, uiUserLanguage, s_StandbyEnabled);

    if (s_UserLanguage != uiUserLanguage)
    {
        s_UserLanguage = uiUserLanguage;
        _SetPromptLanguage(s_UserLanguage);
        _SetVoiceModel(UNDEFINED_INFERENCE, s_UserLanguage, 0);
        if (s_UserId != -1)
        {
            /* Need to update coffee Selection in database */
            _RegisterCoffeeSelection(s_UserCoffeeType, s_UserCoffeeSize, s_UserCoffeeStrength, s_UserLanguage);
        }
    }
}

uint8_t UI_EnterScreen_Callback(screen_t screenId)
{
    LOGD("[UI] Enter screen:%s", get_screen_name(screenId));
    uint8_t changeScreen = 0;

    switch (screenId)
    {
        case kScreen_Home:
        {
            // draw overlay UI
            s_StandbyEnabled            = false;
            s_FaceRecProgress           = 0.0f;
            s_IsWaitingAnotherSelection = 0;
            s_Recognized                = 0;
            s_UserId                    = -1;
            s_UserCoffeeType            = kCoffeeType_NumTypes;
            s_UserCoffeeSize            = kCoffeeSize_NumSizes;
            s_UserCoffeeStrength        = kCoffeeStrength_NumStrengths;
            s_FaceRecIndicator          = FACE_REC_INDICATOR_INIT;
            changeScreen                = 1;
            _DrawPreviewUI(g_PreviewMode, 1, s_FaceRecIndicator, 1, s_FaceRecProgress);

            // disable the lpm
            // FWK_LpmManager_EnableSleepMode(0);
            // start the face rec

            _StopFaceRec(0);
            _FaceRecProgressTimer_Start();

            _SessionTimer_SetPeriod(SESSION_TIMER_IN_MS);
            _SessionTimer_Start();
        }
        break;
        case kScreen_Brewing:
        {
            if (s_StandbyEnabled == false)
            {
                _SessionTimer_Stop();
                _StopVoiceCmd();
                changeScreen = 1;
            }
        }
        break;
        case kScreen_Finished:
        {
            changeScreen = 1;
        }
        break;
        case kScreen_Standby:
        {
            changeScreen       = 1;
            s_Recognized       = 0;
            s_UserId           = -1;
            s_StandbyEnabled   = true;
            s_FaceRecIndicator = FACE_REC_INDICATOR_INIT;
            _DrawPreviewUI(g_PreviewMode, 1, s_FaceRecIndicator, 0, 0);
            _StopFaceRec(1);
            _SetVoiceModel(ASR_WW, UNDEFINED_LANGUAGE, 0);
            _SessionTimer_Stop();
            // FWK_LpmManager_EnableSleepMode(1);
        }
        break;
        default:
            break;
    }

    return changeScreen;
}

static hal_output_status_t HAL_OutputDev_UiCoffeeMachine_Init(output_dev_t *dev, output_dev_callback_t callback)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_UiCoffeeMachine_Init");

    dev->cap.callback = callback;

    LoadIcons((unsigned char *)APP_ICONS_BASE);

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

    s_FaceRecProgressTimer =
        xTimerCreate("FaceRecProgress", (TickType_t)pdMS_TO_TICKS(FACE_REC_UPDATE_INTERVAL), pdTRUE,
                     (void *)&s_FaceRecProgress, (TimerCallbackFunction_t)_FaceRecProgressTimer_Callback);
    if (s_FaceRecProgressTimer == NULL)
    {
        LOGE("[UI] Failed to start \"FaceRecProgress\" timer.");
    }

    s_VirtualFaceSurface.left   = 0;
    s_VirtualFaceSurface.top    = 0;
    s_VirtualFaceSurface.right  = UI_BUFFER_WIDTH - 1;
    s_VirtualFaceSurface.bottom = UI_BUFFER_HEIGHT - 1;
    s_VirtualFaceSurface.height = UI_BUFFER_HEIGHT;
    s_VirtualFaceSurface.width  = UI_BUFFER_WIDTH;
    s_VirtualFaceSurface.pitch  = UI_BUFFER_WIDTH * 2;
    s_VirtualFaceSurface.format = kPixelFormat_RGB565;
    s_VirtualFaceSurface.buf    = s_VirtualFaceBuffer;
    s_VirtualFaceSurface.lock   = NULL;

    _DrawVirtualFaceIcon(&s_VirtualFaceSurface, s_Icons[ICON_VIRTUAL_FACE_BLUE]);

    LOGD("--HAL_OutputDev_UiCoffeeMachine_Init");
    return error;
}

static hal_output_status_t HAL_OutputDev_UiCoffeeMachine_Deinit(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_UiLvgl_Deinit");

    /* Add de-initialization code here */

    LOGD("--HAL_OutputDev_UiLvgl_Deinit");
    return error;
}

static hal_output_status_t HAL_OutputDev_UiCoffeeMachine_Start(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_UiCoffeeMachine_Start");

    /* Add start code here */
    if (FWK_OutputManager_RegisterEventHandler(dev, &s_OutputDev_UiCoffeeMachineHandler) != 0)
    {
        error = kStatus_HAL_OutputError;
    }

    LOGD("--HAL_OutputDev_UiCoffeeMachine_Start");
    return error;
}

static hal_output_status_t HAL_OutputDev_UiCoffeeMachine_Stop(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_UiCoffeeMachine_Stop");

    /* Add stop code here */

    LOGD("--HAL_OutputDev_UiCoffeeMachine_Stop");
    return error;
}

static hal_output_status_t _InferComplete_Vision(const output_dev_t *dev, void *inferResult, screen_t currentScreenId)
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
        LOGI("[UI] Screen:%s face recognized:%d face_id:%d", get_screen_name(currentScreenId), pResult->face_recognized,
             pResult->face_id);
        if (currentScreenId == kScreen_Home)
        {
            int drawUpdate     = 0;
            s_FaceRecIndicator = FACE_REC_INDICATOR_INIT;

            if (s_FaceRecOSDEnable)
            {
                gui_set_algo_debuginfo(pResult->face_count, pResult->debug_info.isSmallFace,
                                       pResult->debug_info.isBlurry, pResult->debug_info.isSideFace,
                                       pResult->debug_info.rgbBrightness, pResult->face_recognized, pResult->face_id);
            }

            if ((pResult->face_recognized) && (pResult->face_id != INVALID_FACE_ID))
            {
                // known user
                _FaceRecProgressTimer_Stop();

                /* Restart timer if face recognize */
                _SessionTimer_Start();

                s_IsWaitingAnotherSelection = 1;
                s_FaceRecProgress           = 1.0f;
                s_FaceRecIndicator          = FACE_REC_INDICATOR_KNOWN;
                drawUpdate                  = 1;

                // store the user's selection
                coffee_result_t *pAttr = (coffee_result_t *)pResult->userData;
                s_Recognized           = 1;
                s_UserId               = pResult->face_id;
                s_UserCoffeeType       = pAttr->coffeeType;
                s_UserCoffeeSize       = pAttr->coffeeSize;
                s_UserCoffeeStrength   = pAttr->coffeeStrength;

                if ((s_WakeUpSource == WAKE_UP_SOURCE_BUTTON) || (s_WakeUpSource == WAKE_UP_SOURCE_TOUCH))
                {
                    if (s_UserLanguage != pAttr->language)
                    {
                        /* Need to update the voice prompt */
                        s_UserLanguage = pAttr->language;
                        _SetPromptLanguage(s_UserLanguage);
                    }
                }
                else if (s_WakeUpSource == WAKE_UP_SOURCE_VOICE)
                {
                    if (s_UserLanguage != pAttr->language)
                    {
                        /* Need to update coffee Selection in database */
                        _RegisterCoffeeSelection(s_UserCoffeeType, s_UserCoffeeSize, s_UserCoffeeStrength,
                                                 s_UserLanguage);
                    }
                }

                LOGD("[UI] CoffeeType %d, CoffeeSize %d, CoffeeStrength %d, Language %d", s_UserCoffeeType,
                     s_UserCoffeeSize, s_UserCoffeeStrength, s_UserLanguage);

                // update the UI to user's coffee selection
                gui_home_set_language(_ConvertASRLanguageToUILanguage(s_UserLanguage));
                gui_set_home_coffee_type(s_UserCoffeeType);
                gui_set_home_coffee_size(s_UserCoffeeSize);
                gui_set_home_coffee_strength(s_UserCoffeeStrength);

                // ask another selection
                int promptId = PROMPT_INVALID;
                switch (s_UserCoffeeType)
                {
                    case kCoffeeType_Americano:
                        promptId = PROMPT_ANOTHER_AMERICANO;
                        break;
                    case kCoffeeType_Cappuccino:
                        promptId = PROMPT_ANOTHER_CAPPUCCINO;
                        break;
                    case kCoffeeType_Espresso:
                        promptId = PROMPT_ANOTHER_ESPRESSO;
                        break;
                    case kCoffeeType_Latte:
                        promptId = PROMPT_ANOTHER_CAFE_LATTE;
                        break;
                    default:
                        break;
                }

                if (promptId != PROMPT_INVALID)
                {
                    /* TODO: Add correct language */
                    _SetVoiceModel(ASR_CMD_COFFEE_MACHINE, s_UserLanguage, 0);
                    _PlayPrompt(promptId, 0);
                }
            }
            else if ((pResult->face_recognized) && (pResult->face_id == INVALID_FACE_ID))
            {
                // new user
                LOGD("[UI] Coffee machine: New user found");
                _FaceRecProgressTimer_Stop();

                s_Recognized       = 1;
                s_UserId           = -1;
                s_FaceRecProgress  = 1.0f;
                drawUpdate         = 1;
                s_FaceRecIndicator = FACE_REC_INDICATOR_UNKNOWN;
            }

            if (drawUpdate)
            {
                _DrawPreviewUI(g_PreviewMode, 1, s_FaceRecIndicator, 1, s_FaceRecProgress);
            }
        }
    }

    return error;
}

static hal_output_status_t _InferComplete_Voice(const output_dev_t *dev, void *inferResult, screen_t currentScreenId)
{
    hal_output_status_t error               = kStatus_HAL_OutputSuccess;
    asr_inference_result_t *voiceAlgoResult = (asr_inference_result_t *)inferResult;
    LOGD("[UI] Screen:%s voice command status:%d  cmd:%d %d:%d", get_screen_name(currentScreenId),
         voiceAlgoResult->status, voiceAlgoResult->keywordID, s_IsWaitingAnotherSelection,
         s_IsWaitingRegisterSelection);

    if (currentScreenId == kScreen_Standby)
    {
        if (voiceAlgoResult->status == ASR_WW_DETECT)
        {
            s_UserLanguage = voiceAlgoResult->language;
            _SetPromptLanguage(s_UserLanguage);
            WakeUp(WAKE_UP_SOURCE_VOICE);
        }
    }
    else if (currentScreenId == kScreen_Home)
    {
        if (voiceAlgoResult->status == ASR_CMD_DETECT && voiceAlgoResult->keywordID > -1)
        {
            // voice command detected
            switch (voiceAlgoResult->keywordID)
            {
                case (VOICE_CMD_START):
                {
                    _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                    gui_set_brewing();
                }
                break;
                case (VOICE_CMD_CONFIRM):
                {
                    if (s_IsWaitingAnotherSelection)
                    {
                        _SessionTimer_Start();
                        _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                        s_IsWaitingAnotherSelection = 0;
                        gui_set_home_coffee_type(s_UserCoffeeType);
                        gui_set_home_coffee_size(s_UserCoffeeSize);
                        gui_set_home_coffee_strength(s_UserCoffeeStrength);
                        gui_set_brewing();
                    }
                }
                break;
                case (VOICE_CMD_CANCEL):
                {
                    if (s_IsWaitingAnotherSelection)
                    {
                        _SetVoiceModel(ASR_CMD_COFFEE_MACHINE, s_UserLanguage, 0);
                        _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                        s_IsWaitingAnotherSelection = 0;
                    }
                }
                break;
                case (VOICE_CMD_ESPRESSO):
                {
                    if (!s_IsWaitingAnotherSelection)
                    {
                        _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                        coffee_type_t curType = get_coffee_type();
                        coffee_type_t newType = kCoffeeType_Espresso;
                        if (curType != newType)
                        {
                            LOGD("[UI] CurType:%d newType:%d", curType, newType);
                            gui_set_home_coffee_type(newType);
                        }
                    }
                }
                break;
                case (VOICE_CMD_AMERICANO):
                {
                    if (!s_IsWaitingAnotherSelection)
                    {
                        _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                        coffee_type_t curType = get_coffee_type();
                        coffee_type_t newType = kCoffeeType_Americano;
                        if (curType != newType)
                        {
                            LOGD("[UI] CurType:%d newType:%d", curType, newType);
                            gui_set_home_coffee_type(newType);
                        }
                    }
                }
                break;
                case (VOICE_CMD_CAPPUCCINO):
                {
                    if (!s_IsWaitingAnotherSelection)
                    {
                        _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                        coffee_type_t curType = get_coffee_type();
                        coffee_type_t newType = kCoffeeType_Cappuccino;
                        if (curType != newType)
                        {
                            LOGD("[UI] CurType:%d newType:%d", curType, newType);
                            gui_set_home_coffee_type(newType);
                        }
                    }
                }
                break;
                case (VOICE_CMD_CAFE_LATTE):
                {
                    if (!s_IsWaitingAnotherSelection)
                    {
                        _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                        coffee_type_t curType = get_coffee_type();
                        coffee_type_t newType = kCoffeeType_Latte;
                        if (curType != newType)
                        {
                            LOGD("[UI] CurType:%d newType:%d", curType, newType);
                            gui_set_home_coffee_type(newType);
                        }
                    }
                }
                break;
                case (VOICE_CMD_SMALL):
                {
                    if (!s_IsWaitingAnotherSelection)
                    {
                        _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                        coffee_size_t curSize = get_coffee_size();
                        coffee_size_t newSize = kCoffeeSize_Small;
                        if (curSize != newSize)
                        {
                            LOGD("[UI] CurSize:%d newSize:%d", curSize, newSize);
                            gui_set_home_coffee_size(newSize);
                        }
                    }
                }
                break;
                case (VOICE_CMD_MEDIUM):
                {
                    if (!s_IsWaitingAnotherSelection)
                    {
                        _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                        coffee_size_t curSize = get_coffee_size();
                        coffee_size_t newSize = kCoffeeSize_Medium;
                        if (curSize != newSize)
                        {
                            LOGD("[UI] CurSize:%d newSize:%d", curSize, newSize);
                            gui_set_home_coffee_size(newSize);
                        }
                    }
                }
                break;
                case (VOICE_CMD_LARGE):
                {
                    if (!s_IsWaitingAnotherSelection)
                    {
                        _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                        coffee_size_t curSize = get_coffee_size();
                        coffee_size_t newSize = kCoffeeSize_Large;
                        if (curSize != newSize)
                        {
                            LOGD("[UI] CurSize:%d newSize:%d", curSize, newSize);
                            gui_set_home_coffee_size(newSize);
                        }
                    }
                }
                break;
                case (VOICE_CMD_SOFT):
                {
                    if (!s_IsWaitingAnotherSelection)
                    {
                        _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                        coffee_strength_t curStrength = get_coffee_strength();
                        coffee_strength_t newStrength = kCoffeeStrength_Soft;
                        LOGD("[UI] CurStrength:%d newStrength:%d", curStrength, newStrength);
                        gui_set_home_coffee_strength(newStrength);
                    }
                }
                break;
                case (VOICE_CMD_MILD):
                {
                    if (!s_IsWaitingAnotherSelection)
                    {
                        _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                        coffee_strength_t curStrength = get_coffee_strength();
                        coffee_strength_t newStrength = kCoffeeStrength_Medium;
                        LOGD("[UI] CurStrength:%d newStrength:%d", curStrength, newStrength);
                        gui_set_home_coffee_strength(newStrength);
                    }
                }
                break;
                case (VOICE_CMD_STRONG):
                {
                    if (!s_IsWaitingAnotherSelection)
                    {
                        _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                        coffee_strength_t curStrength = get_coffee_strength();
                        coffee_strength_t newStrength = kCoffeeStrength_Strong;
                        LOGD("[UI] CurStrength:%d newStrength:%d", curStrength, newStrength);
                        gui_set_home_coffee_strength(newStrength);
                    }
                }
                break;
                case (VOICE_CMD_DEREGISTER):
                {
                    _SessionTimer_Start();
                    _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                    DeregisterCoffeeSelection();
                }
                break;
                default:
                    break;
            }
            // cancel the user's confirmation
            s_IsWaitingAnotherSelection = 0;
        }
    }
    else if (currentScreenId == kScreen_Finished)
    {
        if (voiceAlgoResult->status == ASR_CMD_DETECT && voiceAlgoResult->keywordID > -1)
        {
            switch (voiceAlgoResult->keywordID)
            {
                case (VOICE_CMD_CONFIRM):
                {
                    if (s_IsWaitingRegisterSelection)
                    {
                        _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                        _FaceRecProgressTimer_Stop();
                        s_IsWaitingRegisterSelection = 0;

                        coffee_type_t curType         = get_coffee_type();
                        coffee_size_t curSize         = get_coffee_size();
                        coffee_strength_t curStrength = get_coffee_strength();
                        asr_language_t language       = _ConvertUILanguageToASRLanguage(get_language());
                        _RegisterCoffeeSelection(curType, curSize, curStrength, language);
                        s_Recognized = 0;
                        s_UserId     = -1;

                        // go to standby
                        _StandBy();
                    }
                }
                break;
                case (VOICE_CMD_CANCEL):
                {
                    _PlayPrompt(PROMPT_CONFIRM_TONE, 0);
                    if (s_IsWaitingRegisterSelection)
                    {
                        s_Recognized = 0;
                        s_UserId     = -1;
                        _FaceRecProgressTimer_Stop();
                        s_IsWaitingRegisterSelection = 0;

                        // go to standby
                        _StandBy();
                    }
                }
                break;
                default:
                    break;
            }
        }
    }
    else if (currentScreenId == kScreen_Brewing)
    {
        if (voiceAlgoResult->status == ASR_TIMEOUT)
        {
            _StandBy();
        }
    }
    return error;
}

static hal_output_status_t HAL_OutputDev_UiCoffeeMachine_InferComplete(const output_dev_t *dev,
                                                                       output_algo_source_t source,
                                                                       void *inferResult)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    if (inferResult == NULL)
    {
        return error;
    }

    LVGL_LOCK();
    screen_t currentScreenId = get_current_screen();

    if (currentScreenId == kScreen_Num)
    {
        return error;
    }

#if AQT_TEST
    if (source == kOutputAlgoSource_Voice)
    {
        _InferComplete_Voice(dev, inferResult, currentScreenId);
    }
#else
    if (source == kOutputAlgoSource_Vision)
    {
        _InferComplete_Vision(dev, inferResult, currentScreenId);
    }
    else if (source == kOutputAlgoSource_Voice)
    {
        _InferComplete_Voice(dev, inferResult, currentScreenId);
    }
#endif /* AQT_TEST */

    LVGL_UNLOCK();

    return error;
}

static hal_output_status_t HAL_OutputDev_UiCoffeeMachine_InputNotify(const output_dev_t *dev, void *data)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    event_base_t *pEventBase  = (event_base_t *)data;

    LVGL_LOCK();

    if (pEventBase->eventId == kEventID_SetPreviewMode)
    {
        event_smart_tlhmi_t *pEvent = (event_smart_tlhmi_t *)pEventBase;
        if (g_PreviewMode != pEvent->previewMode)
        {
            _DrawPreviewUI(pEvent->previewMode, 1, s_FaceRecIndicator, 1, s_FaceRecProgress);
            g_PreviewMode = pEvent->previewMode;
        }

        if (pEventBase->respond != NULL)
        {
            pEventBase->respond(pEventBase->eventId, &pEvent->previewMode, kEventStatus_Ok, true);
        }
    }
    else if (pEventBase->eventId == kEventID_GetPreviewMode)
    {
        if (pEventBase->respond != NULL)
        {
            pEventBase->respond(pEventBase->eventId, &g_PreviewMode, kEventStatus_Ok, true);
        }
    }
    else if (pEventBase->eventId == kEventID_WakeUp)
    {
        WakeUp(WAKE_UP_SOURCE_BUTTON);
    }
    else
    {
        /* Add 'inputNotify' event handler code here */
        APP_OutputDev_UiCoffeeMachine_InputNotifyDecode(pEventBase);
    }

    LVGL_UNLOCK();

    return error;
}

int HAL_OutputDev_UiCoffeeMachine_Register()
{
    int error = 0;
    LOGD("++HAL_OutputDev_UiCoffeeMachine_Register");

    error = FWK_OutputManager_DeviceRegister(&s_OutputDev_UiCoffeeMachine);

    LOGD("--HAL_OutputDev_UiCoffeeMachine_Register");
    return error;
}

#endif /* ENABLE_OUTPUT_DEV_UiCoffeeMachine */
