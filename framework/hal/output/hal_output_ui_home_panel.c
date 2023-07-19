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
#include <stdio.h>
#include "board_define.h"
#ifdef ENABLE_OUTPUT_DEV_UiHomePanel

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
#include "IndexCommands.h"

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

#define UI_BUFFER_HAND_WIDTH  180
#define UI_BUFFER_HAND_HEIGHT 200
#define UI_BUFFER_HAND_BPP    2
#define UI_BUFFER_HAND_PITCH  (UI_BUFFER_HAND_BPP * UI_BUFFER_HAND_HEIGHT)

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
#define RGB565_ORANGE   0xDC42
#define RGB565_UIBLUE   0x23BD

typedef enum _face_rec_indicator
{
    FACE_REC_INDICATOR_INIT = 0,
    FACE_REC_INDICATOR_KNOWN,
    FACE_REC_INDICATOR_UNKNOWN,
    FACE_REC_INDICATOR_INVALID
} face_rec_indicator_type;

enum
{
    ICON_PROGRESS_BAR = 0,
    ICON_INVALID
};

#define VOICE_SESSION_TIMER_IN_MS (8000)
static char *s_Icons[ICON_INVALID];
static int s_GuiderColor[FACE_REC_INDICATOR_INVALID] = {RGB565_BLUE, RGB565_GREEN, RGB565_RED};

static gfx_surface_t s_UiSurface;

SDK_ALIGN(static char s_AsBuffer[UI_BUFFER_WIDTH * UI_BUFFER_HEIGHT * UI_BUFFER_BPP], 32);

static float s_FaceRecProgress              = 0.0f;
static TimerHandle_t s_FaceRecProgressTimer = NULL;
#define FACE_REC_UPDATE_INTERVAL (60)

static uint16_t s_UserId                          = INVALID_FACE_ID;
static face_rec_indicator_type s_FaceRecIndicator = FACE_REC_INDICATOR_INIT;
preview_mode_t g_PreviewMode                      = PREVIEW_MODE_CAMERA;

static asr_language_t s_UserLanguage = ASR_ENGLISH;
static event_voice_t s_VoiceEvent;
static bool s_VoiceSessionStarted        = false;
static TimerHandle_t s_VoiceSessionTimer = NULL;
static bool s_PlayerInterrupted          = false;

static TimerHandle_t s_VoiceSessionDelayTimer = NULL;
static bool s_WaitPreferenceLoading           = false;
static bool s_PlayingAudioPrompt              = false;

AT_USR_SHMEM_SECTION_ALIGN(users_share_db_t g_UserList, 64);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

static hal_output_status_t HAL_OutputDev_UiHomePanel_Init(output_dev_t *dev, output_dev_callback_t callback);
static hal_output_status_t HAL_OutputDev_UiHomePanel_Deinit(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_UiHomePanel_Start(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_UiHomePanel_Stop(const output_dev_t *dev);

__attribute__((weak)) uint32_t APP_OutputDev_UiHomePanel_InferCompleteDecode(output_algo_source_t source,
                                                                             void *inferResult)
{
    return 0;
}
__attribute__((weak)) uint32_t APP_OutputDev_UiHomePanel_InputNotifyDecode(event_base_t *inputData)
{
    return 0;
}

static hal_output_status_t HAL_OutputDev_UiHomePanel_InferComplete(const output_dev_t *dev,
                                                                   output_algo_source_t source,
                                                                   void *inferResult);
static hal_output_status_t HAL_OutputDev_UiHomePanel_InputNotify(const output_dev_t *dev, void *data);

#if defined(__cplusplus)
}
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

const static output_dev_operator_t s_OutputDev_UiHomePanelOps = {
    .init   = HAL_OutputDev_UiHomePanel_Init,
    .deinit = HAL_OutputDev_UiHomePanel_Deinit,
    .start  = HAL_OutputDev_UiHomePanel_Start,
    .stop   = HAL_OutputDev_UiHomePanel_Stop,
};

static output_dev_t s_OutputDev_UiHomePanel = {
    .name          = "UiHomePanel",
    .attr.type     = kOutputDevType_UI,
    .attr.pSurface = &s_UiSurface,
    .ops           = &s_OutputDev_UiHomePanelOps,
};

const static output_dev_event_handler_t s_OutputDev_UiHomePanelHandler = {
    .inferenceComplete = HAL_OutputDev_UiHomePanel_InferComplete,
    .inputNotify       = HAL_OutputDev_UiHomePanel_InputNotify,
};

/*
 * @brief the fixed connection of the landmarks to each other
 */
const int landmarkConnect[2][HAND_LANDMARK_NUMBER] = {
    {0, 1, 2, 3, 0, 5, 6, 7, 5, 9, 10, 11, 9, 13, 14, 15, 13, 17, 18, 19, 17},
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 0},
};

void LoadIcons(void *base)
{
    s_Icons[ICON_PROGRESS_BAR] = (base + 0);
}

static void _setUiSurfaceArea(gfx_surface_t *pUiSurface, int width, int height, int bpp)
{
    pUiSurface->left   = 0;
    pUiSurface->top    = 0;
    pUiSurface->right  = width - 1;
    pUiSurface->bottom = height - 1;
    pUiSurface->height = height;
    pUiSurface->width  = width;
    pUiSurface->pitch  = width * bpp;
    memset(pUiSurface->buf, 0, pUiSurface->height * pUiSurface->pitch);
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

static void _DrawHandMarkers(const gfx_surface_t *pUiSurface, const uvita_gesture_out *pHand)
{
    const uvita_gesture_out hand  = *pHand;
    const gfx_surface_t uiSurface = *pUiSurface;

    for (int i = 0; i < uiSurface.height * uiSurface.pitch; i += sizeof(lv_color_t))
    {
        *(lv_color_t *)(uiSurface.buf + i) = LV_COLOR_CHROMA_KEY;
    }

    if (hand.has_hand == false)
    {
        GUI_AudioPlayer_RefreshHandHolder(uiSurface.buf);
        return;
    }

    int line_width = (int)(uiSurface.height / 120 + 2.0f);
    int radius     = (int)(uiSurface.height / 120 + 4.0f);
    int lmk_width  = hand.box.rect[RECT_RIGHT_X] - hand.box.rect[RECT_LEFT_X];
    int lmk_height = hand.box.rect[RECT_BOTTOM_Y] - hand.box.rect[RECT_TOP_Y];
    int max        = (lmk_width > lmk_height) ? lmk_width : lmk_width;
    float scale    = (float)uiSurface.width / max;
    int shift_x    = (uiSurface.width - lmk_width * scale) / 2.0f + 0.5f;

    for (int i = 0; i < HAND_LANDMARK_NUMBER; i++)
    {
        int left_x   = (int)((hand.handmark[landmarkConnect[0][i] * 3] - hand.box.rect[RECT_LEFT_X]) * scale) + shift_x;
        int top_y    = (int)((hand.handmark[landmarkConnect[0][i] * 3 + 1] - hand.box.rect[RECT_TOP_Y]) * scale);
        int right_x  = (int)((hand.handmark[landmarkConnect[1][i] * 3] - hand.box.rect[RECT_LEFT_X]) * scale) + shift_x;
        int bottom_y = (int)((hand.handmark[landmarkConnect[1][i] * 3 + 1] - hand.box.rect[RECT_TOP_Y]) * scale);
        gfx_drawLine(&uiSurface, left_x, top_y, right_x, bottom_y, line_width, RGB565_ORANGE);
    }

    for (int i = 0; i < HAND_LANDMARK_NUMBER; i++)
    {
        int points_x = (int)((hand.handmark[i * 3] - hand.box.rect[RECT_LEFT_X]) * scale) + shift_x;
        int points_y = (int)((hand.handmark[i * 3 + 1] - hand.box.rect[RECT_TOP_Y]) * scale);
        gfx_drawPoint(&uiSurface, points_x, points_y, radius, RGB565_UIBLUE);
    }

    GUI_AudioPlayer_RefreshHandHolder(s_UiSurface.buf);
}

static void _DrawProgressBar(preview_mode_t previewMode, float percent)
{
    /* process bar background */
    int x                   = (UI_BUFFER_WIDTH - PROCESS_BAR_BG_W / UI_SCALE_W) / 2;
    int y                   = (UI_BUFFER_HEIGHT + UI_GUIDE_RECT_H / UI_SCALE_H) / 2 + 36;
    gfx_surface_t *pSurface = &s_UiSurface;

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
    // LOGD("[UI] screen:%d FaceRec %d", get_current_screen(), (int)(s_FaceRecProgress * 100));
    if (s_FaceRecProgress < 1.0f)
    {
        s_FaceRecProgress += 0.01f;
    }
    else
    {
        s_FaceRecProgress = 0.0f;
    }
    _DrawPreviewUI(g_PreviewMode, 0, s_FaceRecIndicator, 1, s_FaceRecProgress);
}

static void _RegisterSecurityUser(void)
{
#if 0
    static event_smart_tlhmi_t s_TlhmiEvent;
    LOGD("[UI] Register user:%d", s_UserId);

    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_VisionAlgoInputNotify;
    output_event.data      = &s_TlhmiEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_TlhmiEvent);
    output_event.eventInfo = kEventInfo_Remote;

    s_TlhmiEvent.eventBase.eventId		= kEventFaceRecId_RegisterSecuirtyUser;
    s_TlhmiEvent.eventBase.eventInfo	= kEventInfo_Remote;
    s_TlhmiEvent.securityUser.id        = s_UserId;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_UiHomePanel.cap.callback(s_OutputDev_UiHomePanel.id, output_event, fromISR);
#endif
}

static void _OutputManagerNotify(uint32_t eventId, void *event, size_t sz, event_info_t info)
{
    output_event_t output_event = {0};

    output_event.eventId   = eventId;
    output_event.data      = event;
    output_event.copy      = 1;
    output_event.size      = sz;
    output_event.eventInfo = info;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_UiHomePanel.cap.callback(s_OutputDev_UiHomePanel.id, output_event, fromISR);
}

static void _OutputManagerNotifyRemote(uint32_t eventId, void *event, size_t sz)
{
    _OutputManagerNotify(eventId, event, sz, kEventInfo_Remote);
}

static void _OutputManagerNotifyLocal(uint32_t eventId, void *event, size_t sz)
{
    _OutputManagerNotify(eventId, event, sz, kEventInfo_Local);
}

static void _EnableCameraPreview(bool enable)
{
    event_common_t event = {0};

    LOGD("[UI] %s display camera preview!", enable ? "enable" : "disable");

    event.eventBase.eventId                 = kEventID_SetDisplayCameraPreview;
    event.displayOutput.enableCameraPreview = enable;

    _OutputManagerNotifyLocal(kOutputEvent_DisplayInputNotify, &event, sizeof(event));
}

static void _SetFaceRec(home_panel_screen_id_t screenId, oasis_lite_state_t state)
{
    static event_face_rec_t s_FaceRecEvent;
    uint32_t eventId = kEventFaceRecID_OasisSetState;

    LOGD("[UI] Set face rec:%d, Screen ID %d", state, screenId);

    if (screenId == kScreen_Security_FaceReg)
    {
        eventId = kEventFaceRecID_OasisSetSecurityState;
    }

    // notify the face rec to start
    s_FaceRecEvent.eventBase.eventId = eventId;
    s_FaceRecEvent.oasisState.state  = state;

    _OutputManagerNotifyRemote(kOutputEvent_VisionAlgoInputNotify, &s_FaceRecEvent, sizeof(s_FaceRecEvent));
}

static void _SetVoiceModel(asr_inference_t modelId, asr_language_t lang, uint8_t ptt)
{
    LOGD("[UI] Set voice model:%d, language %d, ptt %d", modelId, lang, ptt);

    s_VoiceEvent.event_base.eventId   = SET_VOICE_MODEL;
    s_VoiceEvent.event_base.eventInfo = kEventInfo_Remote;
    s_VoiceEvent.set_asr_config.demo  = modelId;
    s_VoiceEvent.set_asr_config.lang  = lang;
    s_VoiceEvent.set_asr_config.ptt   = ptt;

    _OutputManagerNotifyRemote(kOutputEvent_VoiceAlgoInputNotify, &s_VoiceEvent, sizeof(s_VoiceEvent));
}

static void _SetPromptLanguage(asr_language_t language)
{
    event_voice_t event;

    LOGD("[UI] Set prompt language %d", language);

    event.event_base.eventId                = SET_MULTILINGUAL_CONFIG;
    event.set_multilingual_config.languages = language;
    _OutputManagerNotifyRemote(kOutputEvent_OutputInputNotify, &event, sizeof(event));
}

void PlayPrompt(int id, uint8_t asrEnabled)
{
    event_common_t event;

    LOGD("[UI] Play prompt %d", id);
    s_PlayingAudioPrompt = true;

    event.eventBase.eventId     = kEventID_PlayPrompt;
    event.promptInfo.id         = id;
    event.promptInfo.asrEnabled = asrEnabled;
    _OutputManagerNotifyRemote(kOutputEvent_OutputInputNotify, &event, sizeof(event));
}

static void _VoiceSessionTimer_Callback(TimerHandle_t xTimer)
{
    LVGL_LOCK();

    home_panel_screen_id_t currentScreenId = get_current_screen();
    LOGD("[UI] screen:%d Voice Session ended state:%d language:%d", currentScreenId, s_VoiceSessionStarted,
         s_UserLanguage);
    _SetVoiceModel(ASR_WW, s_UserLanguage, 0);
    s_VoiceSessionStarted = false;
    gui_enable_mic(currentScreenId, false);

    if ((currentScreenId == kScreen_AudioPlayer) && s_PlayerInterrupted)
    {
        GUI_AudioPlayer_BargeOut();
        s_PlayerInterrupted = false;
    }
    else if (currentScreenId == kScreen_Thermostat && s_WaitPreferenceLoading)
    {
        gui_thermostat_enable_confirm_cancel(false);
        s_WaitPreferenceLoading = false;
    }

    LVGL_UNLOCK();
}

static void _VoiceSessionTimer_Start(void)
{
    if (s_VoiceSessionTimer == NULL)
    {
        // create the timer
        s_VoiceSessionTimer = xTimerCreate("VoiceSessionTimer", (TickType_t)pdMS_TO_TICKS(VOICE_SESSION_TIMER_IN_MS),
                                           pdFALSE, NULL, (TimerCallbackFunction_t)_VoiceSessionTimer_Callback);
        if (s_VoiceSessionTimer == NULL)
        {
            LOGE("[UI] Failed to start \"VoiceSessionTimer\" timer.");
            return;
        }
    }

    if (xTimerStart(s_VoiceSessionTimer, 0) != pdPASS)
    {
        LOGE("[UI] Failed to start \"VoiceSessionTimer\" timer.");
    }
}

static void _VoiceSessionTimer_Stop(void)
{
    if (s_VoiceSessionTimer != NULL)
    {
        xTimerStop(s_VoiceSessionTimer, 0);
    }
}

static void _StartVoiceSession(home_panel_screen_id_t screenId,
                               asr_inference_t modelId,
                               asr_language_t lang,
                               uint8_t ptt)
{
    LOGD("[UI] StartVoiceSesion");
    _SetVoiceModel(modelId, lang, ptt);
    _VoiceSessionTimer_Start();
    gui_enable_mic(screenId, true);
    s_VoiceSessionStarted = true;
}

static asr_inference_t _GetVoiceModel(home_panel_screen_id_t currentScreenId);
static void _VoiceSessionDelayTimer_Callback(TimerHandle_t xTimer)
{
    LVGL_LOCK();

    home_panel_screen_id_t currentScreenId = get_current_screen();
    if (currentScreenId == kScreen_Thermostat && s_WaitPreferenceLoading)
    {
        gui_thermostat_enable_confirm_cancel(true);
        asr_inference_t modelId = _GetVoiceModel(currentScreenId);
        _StartVoiceSession(currentScreenId, modelId, s_UserLanguage, 0);
    }

    LVGL_UNLOCK();
}

static void _StartVoiceSessionDelayed(int delay_ms)
{
    if (s_VoiceSessionDelayTimer == NULL)
    {
        s_VoiceSessionDelayTimer = xTimerCreate("VoiceSessionDelayTimer", (TickType_t)pdMS_TO_TICKS(delay_ms), pdFALSE,
                                                NULL, _VoiceSessionDelayTimer_Callback);
        if (s_VoiceSessionDelayTimer == NULL)
        {
            LOGE("[UI] Failed to start \"VoiceSessionDelayTimer\" timer.");
            return;
        }
    }

    if (xTimerStart(s_VoiceSessionDelayTimer, 0) != pdPASS)
    {
        LOGE("[UI] Failed to start \"VoiceSessionDelayTimer\" timer.");
    }
}

static void _StopVoiceSession(home_panel_screen_id_t screenId)
{
    LOGD("[UI] StopVoiceSesion");
    s_VoiceSessionStarted = false;
    s_PlayerInterrupted   = false;
    s_WaitPreferenceLoading = false;
    _VoiceSessionTimer_Stop();
    gui_enable_mic(screenId, false);
    _SetVoiceModel(ASR_WW, s_UserLanguage, 0);
}

static void _PlayAudioStream(uint32_t event_id)
{
    event_common_t playAudioEvent;

    memset(&playAudioEvent, 0, sizeof(playAudioEvent));
    playAudioEvent.eventBase.eventId = event_id;
    playAudioEvent.data              = NULL;

    _OutputManagerNotifyRemote(kOutputEvent_OutputInputNotify, &playAudioEvent, sizeof(playAudioEvent));
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
        default:
            asrLanguage = UNDEFINED_LANGUAGE;
            break;
    }

    return asrLanguage;
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
        default:
            uiLanguage = kLanguage_Ids;
            break;
    }

    return uiLanguage;
}

void UI_VolumeChangeNotify(uint8_t volume)
{
    event_common_t setVolumeEvent;
    LOGD("[UI] Set Volume: %d\r\n", volume);

    memset(&setVolumeEvent, 0, sizeof(setVolumeEvent));
    setVolumeEvent.eventBase.eventId    = kEventID_SetSpeakerVolume;
    setVolumeEvent.speakerVolume.volume = volume;

    _OutputManagerNotifyRemote(kOutputEvent_OutputInputNotify, &setVolumeEvent, sizeof(setVolumeEvent));
}

static void _EnableGestureControl(uint8_t enable)
{
    static event_face_rec_t s_EnableGestureControl;

    memset(&s_EnableGestureControl, 0, sizeof(s_EnableGestureControl));

    s_EnableGestureControl.eventBase.eventId = kEventFaceRecID_UvitaGestureSetState;
    if (enable == true)
    {
        s_EnableGestureControl.uvitaState.state = kUvitaState_Running;
    }
    else
    {
        s_EnableGestureControl.uvitaState.state = kUvitaState_Stopped;
    }

    _OutputManagerNotifyRemote(kOutputEvent_VisionAlgoInputNotify, &s_EnableGestureControl,
                               sizeof(s_EnableGestureControl));
}

void UI_EnableKeyboardPreview(bool enable)
{
    event_smart_tlhmi_t event = {0};
    LOGD("[UI] %s keyboard!", enable ? "Enable" : "Disable");

    event.eventBase.eventId = kEventID_SetKeyboardOverlay;
    event.data              = (void *)enable;

    _OutputManagerNotifyLocal(kOutputEvent_DisplayInputNotify, &event, sizeof(event));
}

void UI_AudioPlaying_Callback(audio_event_t audio_event)
{
    switch (audio_event)
    {
        case kAudioPlayer_Play:
            _PlayAudioStream(kEventID_StreamerPlay);
            break;
        case kAudioPlayer_Pause:
            _PlayAudioStream(kEventID_StreamerPause);
            break;
        case kAudioPlayer_Next:
            _PlayAudioStream(kEventID_StreamerNext);
            break;
        case kAudioPlayer_Previous:
            _PlayAudioStream(kEventID_StreamerPrevious);
            break;
        case kAudioPlayer_Stop:
            _PlayAudioStream(kEventID_StreamerStop);
            break;
        case kAudioPlayer_Load:
            _PlayAudioStream(kEventID_StreamerLoad);
            break;
        default:
            break;
    }
}

void UI_EnterScreen_Callback(home_panel_screen_id_t screenId)
{
    LOGD("[UI] EnterScreen %d", screenId);
    switch (screenId)
    {
        case kScreen_Home:
        {
            _StopVoiceSession(screenId);
        }
        break;
        case kScreen_Thermostat:
        {
            _StopVoiceSession(screenId);

            if (gui_thermostat_get_state() == kThermostat_On)
            {
                _SetFaceRec(screenId, kOASISLiteState_Recognition);
            }
        }
        break;
        case kScreen_Security:
        {
            _StopVoiceSession(screenId);
        }
        break;
        case kScreen_AudioPlayer:
        {
            _setUiSurfaceArea(&s_UiSurface, UI_BUFFER_HAND_WIDTH, UI_BUFFER_HAND_HEIGHT, UI_BUFFER_HAND_BPP);
            _StopVoiceSession(screenId);
            /* Start gesture */
            _EnableGestureControl(true);
        }
        break;
        case kScreen_Thermostat_FaceReg:
        case kScreen_Security_FaceReg:
        {
            oasis_lite_state_t oasisNewState = kOASISLiteState_Registration;

            s_FaceRecProgress  = 0.0f;
            s_FaceRecIndicator = FACE_REC_INDICATOR_INIT;
            _DrawPreviewUI(g_PreviewMode, 1, s_FaceRecIndicator, 1, s_FaceRecProgress);
            _FaceRecProgressTimer_Start();
            _EnableCameraPreview(true);
            if (screenId == kScreen_Security_FaceReg)
            {
                if (gui_security_get_authentication_mode() == kSecurity_Auth_FaceRec)
                {
                    oasisNewState = kOASISLiteState_Recognition;
                }
            }

            _SetFaceRec(screenId, oasisNewState);
        }
        break;
        default:
            break;
    }
}

void UI_ExitScreen_Callback(home_panel_screen_id_t screenId)
{
    LOGD("[UI] ExitScreen %d", screenId);
    switch (screenId)
    {
        case kScreen_Home:
        {
        }
        break;
        case kScreen_Thermostat:
        {
            _SetFaceRec(screenId, kOASISLiteState_Stopped);
        }
        break;
        case kScreen_Security:
        {
        }
        break;
        case kScreen_AudioPlayer:
        {
            _setUiSurfaceArea(&s_UiSurface, UI_BUFFER_WIDTH, UI_BUFFER_HEIGHT, UI_BUFFER_BPP);
            switch_audio_player_state(kAudioState_Pause);
            _EnableGestureControl(false);
            /* force reset the volume to streamer default volume for prompt */
            set_speaker_volume(STREAMER_AUDIO_DEFAULT_VOLUME);
        }
        break;
        case kScreen_Thermostat_FaceReg:
        case kScreen_Security_FaceReg:
        {
            _SetFaceRec(screenId, kOASISLiteState_Stopped);
            _FaceRecProgressTimer_Stop();
            _VoiceSessionTimer_Stop();
            _EnableCameraPreview(false);
            UI_EnableKeyboardPreview(false);
        }
        break;
        default:
            break;
    }
}

language_t UI_GetLanguage(void)
{
    language_t uiLanguage = _ConvertASRLanguageToUILanguage(s_UserLanguage);
    return uiLanguage;
}

uint16_t UI_GetFaceId(void)
{
    return s_UserId;
}

int UI_InitThermostatUserList(thermostat_user_list_obj_t *users, int size)
{
    BOARD_ShareDb_Lock();

    thermostat_users_share_db_t *db = &g_UserList.thermostat_list;

    if (db->count < size)
    {
        size = db->count;
    }

    for (int i = 0; i < size; i++)
    {
        thermostat_user_list_obj *src   = &db->objs[i];
        thermostat_user_list_obj_t *dst = &users[i];

        dst->face_id = src->face_id;
        snprintf(dst->user_name, sizeof(dst->user_name), "%s", src->user_name);
        snprintf(dst->temperature, sizeof(dst->temperature), "%s", src->temperature);
        dst->mode = src->mode;
        dst->fan_speed = src->fan_speed;

        LOGD("user %d name %s temperature %s", dst->face_id, dst->user_name, dst->temperature);
    }

    BOARD_ShareDb_Unlock();

    return size;
}

int UI_InitSecurityUserList(security_user_t *users, int size)
{
    BOARD_ShareDb_Lock();

    security_users_share_db_t *db = &g_UserList.security_list;

    if (db->count < size)
    {
        size = db->count;
    }

    for (int i = 0; i < size; i++)
    {
        security_user_list_obj_t *src = &db->objs[i];
        security_user_t *dst          = &users[i];

        dst->face_id = src->face_id;
        snprintf(dst->name, sizeof(dst->name), "%s", src->user_name);

        LOGD("security user %d name %s", dst->face_id, dst->name);
    }

    BOARD_ShareDb_Unlock();

    return size;
}

void UI_SetLanguage_Callback(language_t language)
{
    asr_language_t uiUserLanguage = _ConvertUILanguageToASRLanguage(language);

    LOGD("[UI] Set old_language:%d language:%d", s_UserLanguage, uiUserLanguage);

    if (s_UserLanguage != uiUserLanguage)
    {
        s_UserLanguage = uiUserLanguage;

        _SetPromptLanguage(s_UserLanguage);
        _SetVoiceModel(UNDEFINED_INFERENCE, s_UserLanguage, 0);
    }
}

void UI_Thermostat_UpdateDB_Callback(const thermostat_user_list_obj_t *user_info, db_op_t op)
{
    if (op == kUserDB_Add)
    {
        LOGD("[UI] add user id %u", user_info->face_id);

        event_smart_tlhmi_t event = {0};
        event.eventBase.eventId = kEventFaceRecId_RespondAddUser;

        event.thermostatUserInfo.id = user_info->face_id;
        memcpy(event.thermostatUserInfo.name, user_info->user_name, sizeof(event.thermostatUserInfo.name));
        event.thermostatUserInfo.mode = user_info->mode;
        event.thermostatUserInfo.fan_speed = user_info->fan_speed;
        memcpy(event.thermostatUserInfo.temperature, user_info->temperature, sizeof(event.thermostatUserInfo.temperature));
        _OutputManagerNotifyRemote(kOutputEvent_VisionAlgoInputNotify, &event, sizeof(event));
    }
    else if (op == kUserDB_Update)
    {
        LOGD("[UI] update user id %u", user_info->face_id);
        event_smart_tlhmi_t event = {0};
        event.eventBase.eventId = kEventFaceRecId_RespondUpdateUser;

        event.thermostatUserInfo.id = user_info->face_id;
        memcpy(event.thermostatUserInfo.name, user_info->user_name, sizeof(event.thermostatUserInfo.name));
        event.thermostatUserInfo.mode = user_info->mode;
        event.thermostatUserInfo.fan_speed = user_info->fan_speed;
        memcpy(event.thermostatUserInfo.temperature, user_info->temperature, sizeof(event.thermostatUserInfo.temperature));
        _OutputManagerNotifyRemote(kOutputEvent_VisionAlgoInputNotify, &event, sizeof(event));
    }
    else if (op == kUserDB_Remove)
    {
        LOGD("[UI] delete user id %u", user_info->face_id);

        event_face_rec_t event = {0};
        event.eventBase.eventId = kEventFaceRecID_DelUser;

        event.delFace.hasID = true;
        event.delFace.id = user_info->face_id;
        _OutputManagerNotifyRemote(kOutputEvent_VisionAlgoInputNotify, &event, sizeof(event));
    }
}

void UI_Security_UpdateDB_Callback(const security_user_t *user, db_op_t op)
{
    if (op == kUserDB_Add)
    {
        LOGD("[UI] add security user id %u", user->face_id);

        event_smart_tlhmi_t event = {0};
        event.eventBase.eventId   = kEventFaceRecId_RespondAddSecurityUser;

        // reuse thermostatUserInfo to pass security user info
        event.thermostatUserInfo.id = user->face_id;
        memcpy(event.thermostatUserInfo.name, user->name, sizeof(event.thermostatUserInfo.name));

        _OutputManagerNotifyRemote(kOutputEvent_VisionAlgoInputNotify, &event, sizeof(event));
    }
    else if (op == kUserDB_Update)
    {
        LOGD("[UI] update security user id %u", user->face_id);
        event_smart_tlhmi_t event = {0};
        event.eventBase.eventId   = kEventFaceRecId_RespondUpdateSecurityUser;

        event.thermostatUserInfo.id = user->face_id;
        memcpy(event.thermostatUserInfo.name, user->name, sizeof(event.thermostatUserInfo.name));
        _OutputManagerNotifyRemote(kOutputEvent_VisionAlgoInputNotify, &event, sizeof(event));
    }
    else if (op == kUserDB_Remove)
    {
        LOGD("[UI] delete security user id %u", user->face_id);

        event_face_rec_t event  = {0};
        event.eventBase.eventId = kEventFaceRecId_DelSecurityUser;

        event.delFace.hasID = true;
        event.delFace.id    = user->face_id;
        _OutputManagerNotifyRemote(kOutputEvent_VisionAlgoInputNotify, &event, sizeof(event));
    }
}

static hal_output_status_t HAL_OutputDev_UiHomePanel_Init(output_dev_t *dev, output_dev_callback_t callback)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_UiHomePanel_Init");

    dev->cap.callback = callback;

    LoadIcons((unsigned char *)APP_ICONS_BASE);

    /* Add initialization code here */

    s_UiSurface.format = kPixelFormat_RGB565;
    s_UiSurface.buf    = s_AsBuffer;
    s_UiSurface.lock   = xSemaphoreCreateMutex();
    _setUiSurfaceArea(&s_UiSurface, UI_BUFFER_WIDTH, UI_BUFFER_HEIGHT, UI_BUFFER_BPP);

    s_FaceRecProgressTimer =
        xTimerCreate("FaceRecProgress", (TickType_t)pdMS_TO_TICKS(FACE_REC_UPDATE_INTERVAL), pdTRUE,
                     (void *)&s_FaceRecProgress, (TimerCallbackFunction_t)_FaceRecProgressTimer_Callback);
    if (s_FaceRecProgressTimer == NULL)
    {
        LOGE("[UI] Failed to start \"FaceRecProgress\" timer.");
    }

    BOARD_ShareDb_Lock();
    uint32_t thermostat_list_count = g_UserList.thermostat_list.count;
    uint32_t security_list_count = g_UserList.security_list.count;
    BOARD_ShareDb_Unlock();

    LOGD("[UI] thermostat user count[%d], security user count[%d]", thermostat_list_count, security_list_count);

    LOGD("--HAL_OutputDev_UiHomePanel_Init");
    return error;
}

static hal_output_status_t HAL_OutputDev_UiHomePanel_Deinit(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_UiLvgl_Deinit");

    /* Add de-initialization code here */

    LOGD("--HAL_OutputDev_UiLvgl_Deinit");
    return error;
}

static hal_output_status_t HAL_OutputDev_UiHomePanel_Start(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_UiHomePanel_Start");

    /* Add start code here */
    if (FWK_OutputManager_RegisterEventHandler(dev, &s_OutputDev_UiHomePanelHandler) != 0)
    {
        error = kStatus_HAL_OutputError;
    }

    LOGD("--HAL_OutputDev_UiHomePanel_Start");
    return error;
}

static hal_output_status_t HAL_OutputDev_UiHomePanel_Stop(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_UiHomePanel_Stop");

    /* Add stop code here */

    LOGD("--HAL_OutputDev_UiHomePanel_Stop");
    return error;
}

static void _InferComplete_VisionFaceRec(oasis_lite_result_t *pResult, home_panel_screen_id_t currentScreenId)
{
    user_state user_state = kUserState_Max;
    if (pResult->state != kOASISLiteState_Recognition)
    {
        return;
    }

    LOGD("FaceRec state: %d, result %d", pResult->state, pResult->rec_result);

    if (pResult->rec_result == kOASISLiteRecognitionResult_Success)
    {
        // recognize known user
        LOGD("FaceRec known user: %u", pResult->face_id);
        s_UserId = pResult->face_id;

        if (currentScreenId == kScreen_Thermostat)
        {
            gui_thermostat_set_facerec_state(kUserState_Known);
            PlayPrompt(PROMPT_LOAD_SETTING_CONFIRM, 1);
            s_WaitPreferenceLoading = true;
            _StartVoiceSessionDelayed(4000);
        }
        else if (currentScreenId == kScreen_Security_FaceReg)
        {
            _FaceRecProgressTimer_Stop();
            s_FaceRecProgress  = 1.0f;
            s_FaceRecIndicator = FACE_REC_INDICATOR_KNOWN;
            _DrawPreviewUI(g_PreviewMode, 1, s_FaceRecIndicator, 1, s_FaceRecProgress);
            gui_security_set_facereg_state(kUserState_Known);
        }
    }
    else if (pResult->rec_result == kOASISLiteRecognitionResult_Unknown)
    {
        if (currentScreenId == kScreen_Thermostat)
        {
            LOGD("FaceRec new user sim %d", pResult->debug_info.sim);
            gui_thermostat_set_facerec_state(kUserState_Unknown);
        }
    }
}

static void _InferComplete_VisionFaceReg(oasis_lite_result_t *pResult, home_panel_screen_id_t currentScreenId)
{
    user_state user_state = kUserState_Max;
    if (pResult->state != kOASISLiteState_Registration)
    {
        return;
    }

    LOGD("FaceReg state: %d, result %d", pResult->state, pResult->reg_result);

    if (pResult->reg_result == kOASISLiteRegistrationResult_Success)
    {
        // registration success
        s_UserId = pResult->face_id;
        _FaceRecProgressTimer_Stop();
        s_FaceRecProgress  = 1.0f;
        s_FaceRecIndicator = FACE_REC_INDICATOR_UNKNOWN;
        _DrawPreviewUI(g_PreviewMode, 1, s_FaceRecIndicator, 1, s_FaceRecProgress);
        user_state = kUserState_Unknown;
    }
    else if (pResult->reg_result == kOASISLiteRegistrationResult_Duplicated)
    {
        // already registrated
        s_UserId = pResult->face_id;

        _FaceRecProgressTimer_Stop();
        s_FaceRecProgress  = 1.0f;
        s_FaceRecIndicator = FACE_REC_INDICATOR_KNOWN;
        _DrawPreviewUI(g_PreviewMode, 1, s_FaceRecIndicator, 1, s_FaceRecProgress);
        user_state = kUserState_Known;
    }
    else if (pResult->face_count > 0)
    {
        static uint32_t time_prev = 0;
        static uint32_t time_curr;
        prompt_id_t id = PROMPT_MAX;

        // detected a face, but failed to pass the quality check
        if (pResult->qualityCheck == kOasisLiteQualityCheck_SmallFace)
        {
            LOGD("[UI] quality check: small face");
            id = PROMPT_COME_CLOSER;
        }
        else if (pResult->qualityCheck == kOasisLiteQualityCheck_Blurry)
        {
            LOGD("[UI] quality check: blurry face");
            id = PROMPT_STAND_STILL;
        }
        else if (pResult->qualityCheck == kOasisLiteQualityCheck_SideFace)
        {
            LOGD("[UI] quality check: sided face");
            id = PROMPT_FACE_THE_CAMERA;
        }
        else if (pResult->qualityCheck == kOasisLiteQualityCheck_PartialBrightness)
        {
            LOGD("[UI] quality check: partial brightness");
            id = PROMPT_REPOSITION_FACE;
        }

        time_curr = FWK_CurrentTimeUs();
        if (id != PROMPT_MAX)
        {
            if (!s_PlayingAudioPrompt && (time_curr - time_prev >= 3000000))
            {
                PlayPrompt(id, 1);
                time_prev = time_curr;
            }
        }
    }

    if (user_state != kUserState_Max)
    {
        if (currentScreenId == kScreen_Thermostat_FaceReg)
        {
            gui_thermostat_set_facereg_state(user_state);
        }
        else if (currentScreenId == kScreen_Security_FaceReg)
        {
            gui_security_set_facereg_state(user_state);
        }
    }
}

static void _InferComplete_VisionAudioPlayer(gesture_result_t *pHandResult)
{
    static gesture_type oldGestureSign = GESTURE_NONE;

    LOGI("Hand gesture %d Old Hand gesture %d, Hand %d", pHandResult->hand.gtype, oldGestureSign,
         pHandResult->hand.left_right)

    _DrawHandMarkers(&s_UiSurface, &pHandResult->hand);
    if (pHandResult->hand.has_hand == false)
    {
        oldGestureSign = GESTURE_NONE;
    }
    else if (oldGestureSign == pHandResult->hand.gtype)
    {
        /* Drop this inference result as it is the same, accept the next one */
        oldGestureSign = GESTURE_NONE;
    }
    else if (oldGestureSign != pHandResult->hand.gtype)
    {
        if (pHandResult->hand.gtype == GESTURE_OK)
        {
            audio_state_t audioPlayerState = get_audio_player_state();
            if (kAudioState_Play == audioPlayerState)
            {
                UI_AudioPlaying_Callback(kAudioPlayer_Pause);
                audioPlayerState = kAudioState_Pause;
            }
            else if (kAudioState_Pause == audioPlayerState)
            {
                UI_AudioPlaying_Callback(kAudioPlayer_Play);
                audioPlayerState = kAudioState_Play;
            }
            set_audio_player_state(audioPlayerState);
            refresh_audio_player_state();
        }
        else if (pHandResult->hand.gtype == GESTURE_PREVIOUS)
        {
            GUI_AudioPlayer_Previous();
            UI_AudioPlaying_Callback(kAudioPlayer_Previous);
        }
        else if (pHandResult->hand.gtype == GESTURE_NEXT)
        {
            GUI_AudioPlayer_Next();
            UI_AudioPlaying_Callback(kAudioPlayer_Next);
        }
        else if (pHandResult->hand.gtype == GESTURE_PALM)
        {
            GUI_AudioPlayer_VolumeUp();
        }
        else if (pHandResult->hand.gtype == GESTURE_FIST)
        {
            GUI_AudioPlayer_VolumeDown();
        }

        oldGestureSign = pHandResult->hand.gtype;
    }
}

static hal_output_status_t _InferComplete_Vision(const output_dev_t *dev,
                                                 void *inferResult,
                                                 home_panel_screen_id_t currentScreenId)
{
    hal_output_status_t error              = kStatus_HAL_OutputSuccess;
    vision_algo_result_t *visionAlgoResult = (vision_algo_result_t *)inferResult;

    if (visionAlgoResult == NULL)
    {
        LOGD("Receive NULL results");
        return kStatus_HAL_OutputSuccess;
    }

    if ((currentScreenId == kScreen_AudioPlayer) && (visionAlgoResult->id == kVisionAlgoID_UvitaGesture))
    {
        gesture_result_t *pHandResult = (gesture_result_t *)&(visionAlgoResult->uvitaHandGesture);
        _InferComplete_VisionAudioPlayer(pHandResult);
    }
    else if ((currentScreenId == kScreen_Thermostat_FaceReg) && (visionAlgoResult->id == kVisionAlgoID_OasisLite))
    {
        oasis_lite_result_t *pResult = (oasis_lite_result_t *)&(visionAlgoResult->oasisLite);
        _InferComplete_VisionFaceReg(pResult, currentScreenId);
    }
    else if ((currentScreenId == kScreen_Security_FaceReg) && (visionAlgoResult->id == kVisionAlgoID_OasisLite))
    {
        oasis_lite_result_t *pResult = (oasis_lite_result_t *)&(visionAlgoResult->oasisLite);
        security_auth_mode authMode  = gui_security_get_authentication_mode();
        if (authMode == kSecurity_Auth_FaceRec)
        {
            _InferComplete_VisionFaceRec(pResult, currentScreenId);
        }
        else if (authMode == kSecurity_Auth_FaceReg)
        {
            _InferComplete_VisionFaceReg(pResult, currentScreenId);
        }
    }
    else if ((currentScreenId == kScreen_Thermostat) && (visionAlgoResult->id == kVisionAlgoID_OasisLite))
    {
        oasis_lite_result_t *pResult = (oasis_lite_result_t *)&(visionAlgoResult->oasisLite);
        _InferComplete_VisionFaceRec(pResult, currentScreenId);
    }

    return error;
}

static asr_inference_t _GetVoiceModel(home_panel_screen_id_t currentScreenId)
{
    asr_inference_t model = ASR_WW;
    switch (currentScreenId)
    {
        case (kScreen_Home):
        {
            model = ASR_CMD_HP_MAIN_MENU;
        }
        break;

        case (kScreen_Thermostat):
        {
            model = ASR_CMD_HP_THERMOSTAT;
        }
        break;

        case (kScreen_Security):
        {
            model = ASR_CMD_HP_SECURITY;
        }
        break;

        case (kScreen_AudioPlayer):
        {
            model = ASR_CMD_HP_AUDIO_PLAYER;
        }
        break;

        default:
            break;
    }

    return model;
}

static hal_output_status_t _InferComplete_Voice(const output_dev_t *dev,
                                                void *inferResult,
                                                home_panel_screen_id_t currentScreenId)
{
    hal_output_status_t error               = kStatus_HAL_OutputSuccess;
    asr_inference_result_t *voiceAlgoResult = (asr_inference_result_t *)inferResult;
    LOGD("[UI] Screen:%d voice command status:%d  cmd:%d", currentScreenId, voiceAlgoResult->status,
         voiceAlgoResult->keywordID);

    if ((s_VoiceSessionStarted == false) && (voiceAlgoResult->status == ASR_WW_DETECT))
    {
        // wake word detected, Pause the player if it is playing
        if (currentScreenId == kScreen_AudioPlayer)
        {
            audio_state_t player_state = get_audio_player_state();
            if (player_state == kAudioState_Play)
            {
                GUI_AudioPlayer_BargeIn();
                s_PlayerInterrupted = true;
            }
        }
        asr_inference_t modelId = _GetVoiceModel(currentScreenId);
        _StartVoiceSession(currentScreenId, modelId, s_UserLanguage, 0);
        return error;
    }

    if (currentScreenId == kScreen_Home)
    {
        if (voiceAlgoResult->status == ASR_CMD_DETECT && voiceAlgoResult->keywordID > -1)
        {
            // voice command detected
            switch (voiceAlgoResult->keywordID)
            {
                case (kHpHomeMenuActionThermostat):
                {
                    gui_load_screen(kScreen_Home, kScreen_Thermostat, 0);
                }
                break;
                case (kHpHomeMenuActionSecurity):
                {
                    gui_load_screen(kScreen_Home, kScreen_Security, 0);
                }
                break;
                case (kHpHomeMenuActionAudioPlayer):
                {
                    gui_load_screen(kScreen_Home, kScreen_AudioPlayer, 0);
                }
                break;
                default:
                    break;
            }
        }
    }
    else if (currentScreenId == kScreen_AudioPlayer)
    {
        if (voiceAlgoResult->status == ASR_CMD_DETECT && voiceAlgoResult->keywordID > -1)
        {
            switch (voiceAlgoResult->keywordID)
            {
                case (kHpAudioPlayerActionMainMenu):
                case (kHpAudioPlayerActionGoBack):
                {
                    gui_load_screen(kScreen_AudioPlayer, kScreen_Home, 0);
                }
                break;
                case (kHpAudioPlayerActionNextSong):
                {
                    audio_state_t state = get_audio_player_state();
                    switch_audio_clip(kAudioPlayer_Next, state);
                    GUI_AudioPlayer_Next();
                }
                break;
                case (kHpAudioPlayerActionPreviousSong):
                {
                    audio_state_t state = get_audio_player_state();
                    switch_audio_clip(kAudioPlayer_Previous, state);
                    GUI_AudioPlayer_Previous();
                }
                break;
                case (kHpAudioPlayerActionPlay):
                {
                    switch_audio_player_state(kAudioState_Play);
                }
                break;
                case (kHpAudioPlayerActionPause):
                {
                    switch_audio_player_state(kAudioState_Pause);
                }
                break;
                case (kHpAudioPlayerActionVolumeUp):
                {
                    GUI_AudioPlayer_VolumeUp();
                }
                break;
                case (kHpAudioPlayerActionVolumeDown):
                {
                    GUI_AudioPlayer_VolumeDown();
                }
                break;
                case (kHpAudioPlayerActionMaxVolume):
                {
                    GUI_AudioPlayer_VolumeMax();
                }
                break;
                case (kHpAudioPlayerActionMute):
                {
                    GUI_AudioPlayer_VolumeMute();
                }
                break;
                default:
                    break;
            }
        }
    }
    else if (currentScreenId == kScreen_Thermostat)
    {
        if (voiceAlgoResult->status == ASR_CMD_DETECT && voiceAlgoResult->keywordID > -1)
        {
            switch (voiceAlgoResult->keywordID)
            {
                case (kHpThermostatActionMainMenu):
                case (kHpThermostatActionGoBack):
                {
                    gui_load_screen(kScreen_Thermostat, kScreen_Home, 0);
                }
                break;
                case (kHpThermostatActionSetCoolMode):
                {
                    gui_thermostat_set_mode(kThermostat_Cool);
                }
                break;
                case (kHpThermostatActionSetAutoMode):
                {
                    gui_thermostat_set_mode(kThermostat_Auto);
                }
                break;
                case (kHpThermostatActionSetHeatMode):
                {
                    gui_thermostat_set_mode(kThermostat_Heat);
                }
                break;
                case (kHpThermostatActionThermostatOn):
                {
                    if (gui_thermostat_get_state() == kThermostat_Off)
                    {
                        gui_thermostat_switch_state();
                    }
                }
                break;
                case (kHpThermostatActionThermostatOff):
                {
                    if (gui_thermostat_get_state() == kThermostat_On)
                    {
                        gui_thermostat_switch_state();
                    }
                }
                break;
                case (kHpThermostatActionSlow):
                {
                    gui_thermostat_set_fan_speed(kThermostat_Fan_Low);
                }
                break;
                case (kHpThermostatActionMedium):
                {
                    gui_thermostat_set_fan_speed(kThermostat_Fan_Mid);
                }
                break;
                case (kHpThermostatActionFast):
                {
                    gui_thermostat_set_fan_speed(kThermostat_Fan_High);
                }
                break;
                case (kHpThermostatActionAuto):
                {
                    gui_thermostat_set_fan_speed(kThermostat_Fan_Auto);
                }
                break;
                case (kHpThermostatActionFanOn):
                {
                    gui_thermostat_set_fan_state(kThermostat_Fan_On);
                }
                break;
                case (kHpThermostatActionFanOff):
                {
                    gui_thermostat_set_fan_state(kThermostat_Fan_Off);
                }
                break;
                case (kHpThermostatActionEighteen):
                {
                    gui_thermostat_set_temperature(18);
                }
                break;
                case (kHpThermostatActionNineteen):
                {
                    gui_thermostat_set_temperature(19);
                }
                break;
                case (kHpThermostatActionTwenty):
                {
                    gui_thermostat_set_temperature(20);
                }
                break;
                case (kHpThermostatActioTwentyOne):
                {
                    gui_thermostat_set_temperature(21);
                }
                break;
                case (kHpThermostatActionTemperatureUp):
                {
                    gui_thermostat_change_temperature(1);
                }
                break;
                case (kHpThermostatActionTemperatureDown):
                {
                    gui_thermostat_change_temperature(-1);
                }
                break;
                case (kHpThermostatActionConfirm):
                {
                    if (s_WaitPreferenceLoading)
                    {
                        LOGD("[UI] confirm to load user preferences");

                        s_WaitPreferenceLoading = false;
                        gui_thermostat_load_setting(true);
                    }
                }
                break;
                case (kHpThermostatActionCancel):
                {
                    if (s_WaitPreferenceLoading)
                    {
                        LOGD("[UI] cancel to load user preferences");
                        s_WaitPreferenceLoading = false;
                        gui_thermostat_load_setting(false);
                    }
                }
                break;
                default:
                    break;
            }
        }
    }
    else if (currentScreenId == kScreen_Security)
    {
        if (voiceAlgoResult->status == ASR_CMD_DETECT && voiceAlgoResult->keywordID > -1)
        {
            switch (voiceAlgoResult->keywordID)
            {
                case (kHpSecurityActionMainMenu):
                case (kHpSecurityActionGoBack):
                {
                    gui_load_screen(kScreen_Security, kScreen_Home, 0);
                }
                break;
                case (kHpSecurityActionActivate):
                {
                    if (gui_security_get_state() != kSecurity_On)
                    {
                        gui_security_set_authentication_mode(kSecurity_Auth_Arming);
                        gui_load_screen(kScreen_Security, kScreen_Security_Pincode, 0);
                    }
                }
                break;
                case (kHpSecurityActionDisable):
                {
                    if (gui_security_get_state() == kSecurity_On)
                    {
                        gui_security_set_authentication_mode(kSecurity_Auth_Arming);
                        gui_load_screen(kScreen_Security, kScreen_Security_Pincode, 0);
                    }
                }
                break;
                case (kHpSecurityActionOpenFrontDoor):
                {
                    gui_security_set_door_state(kSecurity_Front, kSecurity_On);
                }
                break;
                case (kHpSecurityActionCloseFrontDoor):
                {
                    gui_security_set_door_state(kSecurity_Front, kSecurity_Off);
                }
                break;
                case (kHpSecurityActionOpenBackDoor):
                {
                    gui_security_set_door_state(kSecurity_Back, kSecurity_On);
                }
                break;
                case (kHpSecurityActionCloseBackDoor):
                {
                    gui_security_set_door_state(kSecurity_Back, kSecurity_Off);
                }
                break;
                case (kHpSecurityActionOpenGarageDoor):
                {
                    gui_security_set_door_state(kSecurity_Garage, kSecurity_On);
                }
                break;
                case (kHpSecurityActionCloseGarageDoor):
                {
                    gui_security_set_door_state(kSecurity_Garage, kSecurity_Off);
                }
                break;
                default:
                    break;
            }
        }
    }

    if (voiceAlgoResult->status == ASR_CMD_DETECT && voiceAlgoResult->keywordID > -1)
    {
        // stop the voice session as one command has been detected
        if ((currentScreenId == kScreen_AudioPlayer) && s_PlayerInterrupted)
        {
            GUI_AudioPlayer_BargeOut();
        }
        _StopVoiceSession(currentScreenId);
    }

    return error;
}

#if AQT_TEST
static void _control_audio_player(asr_inference_result_t *voiceAlgoResult)
{
    if (voiceAlgoResult->status == ASR_CMD_DETECT    &&
        voiceAlgoResult->keywordID > -1              &&
        voiceAlgoResult->demo == ASR_CMD_HP_AUDIO_PLAYER)
    {
         switch (voiceAlgoResult->keywordID)
         {
            case (kHpAudioPlayerActionNextSong):
            {
                UI_AudioPlaying_Callback(kAudioPlayer_Next);
            }
            break;
            case (kHpAudioPlayerActionPreviousSong):
            {
                UI_AudioPlaying_Callback(kAudioPlayer_Previous);
            }
            break;
            case (kHpAudioPlayerActionPlay):
            {
                UI_AudioPlaying_Callback(kAudioPlayer_Play);
            }
            break;
            case (kHpAudioPlayerActionPause):
            {
                UI_AudioPlaying_Callback(kAudioPlayer_Pause);
            }
            break;
            case (kHpAudioPlayerActionVolumeUp):
            {
                audio_volume_up();
            }
            break;
            case (kHpAudioPlayerActionVolumeDown):
            {
                audio_volume_down();
            }
            break;
            case (kHpAudioPlayerActionMute):
            {
                UI_VolumeChangeNotify(0);
            }
            break;
            case (kHpAudioPlayerActionMaxVolume):
            {
                audio_volume_max();
            }
            break;
            default:
                break;
         }
    }
}
#endif /* AQT_TEST */

static hal_output_status_t HAL_OutputDev_UiHomePanel_InferComplete(const output_dev_t *dev,
                                                                   output_algo_source_t source,
                                                                   void *inferResult)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    if (inferResult == NULL)
    {
        return error;
    }

#if AQT_TEST
    if(source == kOutputAlgoSource_Voice)
    {
        _control_audio_player(inferResult);
    }
#endif /* AQT_TEST */

    LVGL_LOCK();
    home_panel_screen_id_t currentScreenId = get_current_screen();

    if (currentScreenId == kScreen_Num)
    {
        LVGL_UNLOCK();
        return error;
    }

    if (source == kOutputAlgoSource_Vision)
    {
        _InferComplete_Vision(dev, inferResult, currentScreenId);
    }

    else if (source == kOutputAlgoSource_Voice)
    {
        _InferComplete_Voice(dev, inferResult, currentScreenId);
    }
    LVGL_UNLOCK();
    return error;
}

static hal_output_status_t HAL_OutputDev_UiHomePanel_InputNotify(const output_dev_t *dev, void *data)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    event_base_t *pEventBase  = (event_base_t *)data;
    /* Get current screen to help the state machine */

    if (pEventBase->eventId == SET_MULTILINGUAL_CONFIG)
    {
        event_voice_t event = *(event_voice_t *)data;
        if (s_UserLanguage != event.set_multilingual_config.languages)
        {
            s_UserLanguage = event.set_multilingual_config.languages;
            LOGD("[UI] Set language:%d", s_UserLanguage);
        }
    }

    LVGL_LOCK();

    home_panel_screen_id_t currScreen = get_current_screen();
    home_panel_screen_id_t nextScreen = get_next_screen();

    LOGI("CurrScreen %d, NextScreen %d eventId %d", currScreen, nextScreen, pEventBase->eventId);
    if (currScreen == kScreen_AudioPlayer || nextScreen == kScreen_AudioPlayer)
    {
        if (pEventBase->eventId == kEventID_MediaPlayer_TrackPosition)
        {
            event_smart_tlhmi_t *pEvent = (event_smart_tlhmi_t *)pEventBase;
            set_audio_position(pEvent->trackInfo.offset);
            refresh_audio_position();
        }
        else if (pEventBase->eventId == kEventID_MediaPlayerInfo)
        {
            event_smart_tlhmi_t *pEvent = (event_smart_tlhmi_t *)pEventBase;
            /* TODO */
            audio_state_t currState = get_audio_player_state();

            LOGI("pEvent->mediaPlayerInfo.isPlaying %d", pEvent->mediaPlayerInfo.isPlaying);
            if (pEvent->mediaPlayerInfo.isPlaying)
            {
                set_audio_player_state(kAudioState_Play);
            }
            else
            {
                set_audio_player_state(kAudioState_Pause);
            }

            if (currState != get_audio_player_state())
            {
                refresh_audio_player_state();
            }
        }
        else if (pEventBase->eventId == kEventID_MediaPlayer_TrackInfo)
        {
            static char songArtist[32];
            static char songName[32];
            event_smart_tlhmi_t *pEvent = (event_smart_tlhmi_t *)pEventBase;

            strcpy(songArtist, (char *)pEvent->trackInfo.songArtist);
            strcpy(songName, (char *)pEvent->trackInfo.songName);

            audio_info_t audioInfo = {.artist_name  = (char *)songArtist,
                                      .track_name   = (char *)songName,
                                      .elapsed_time = 0,
                                      .total_time   = pEvent->trackInfo.songDurationS};
            set_audio_info(&audioInfo);
            refresh_audio_player_info();
        }
        else if (pEventBase->eventId == kEventID_SetSpeakerVolume) /* Only shell sends this command for volume adjustment */
        {
            event_common_t *pEvent               = (event_common_t *)data;
            if (pEvent->speakerVolume.volume != get_speaker_volume())
            {
                LOGD("update volume: %d", pEvent->speakerVolume.volume);
                set_speaker_volume_alone(pEvent->speakerVolume.volume);
                refresh_audio_player_volume();
            }
        }
    }

    if (pEventBase->eventId == kEventID_PlayPromptDone)
    {
        const event_common_t *eventCommon = (event_common_t*)pEventBase;
        LOGD("Prompt finished playing. Prompt id %d", (uint32_t)eventCommon->data);

        s_PlayingAudioPrompt = false;
    }
    else if (pEventBase->eventId == kEventFaceRecID_GetUserList)
    {
        BOARD_ShareDb_Lock();

        LOGD("[UI] Thermostat user list:");
        for (int i = 0; i < g_UserList.thermostat_list.count; i++)
        {
            thermostat_user_list_obj *user = &g_UserList.thermostat_list.objs[i];

            LOGD("[%u][%s][%d][%d][%s]", user->face_id, user->user_name, user->mode, user->fan_speed, user->temperature);
        }

        LOGD("[UI] Security user list:");
        for (int i = 0; i < g_UserList.security_list.count; i++)
        {
            LOGD("[%s]", g_UserList.security_list.objs[i].user_name);
        }

        BOARD_ShareDb_Unlock();
    }

    LVGL_UNLOCK();
    return error;
}

int HAL_OutputDev_UiHomePanel_Register()
{
    int error = 0;
    LOGD("++HAL_OutputDev_UiHomePanel_Register");

    error = FWK_OutputManager_DeviceRegister(&s_OutputDev_UiHomePanel);

    LOGD("--HAL_OutputDev_UiHomePanel_Register");
    return error;
}

#endif /* ENABLE_OUTPUT_DEV_UiHomePanel */
