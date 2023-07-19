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

#define VOLUME_STEP_SIZE 15
typedef enum _face_rec_indicator
{
    FACE_REC_INDICATOR_INIT = 0,
    FACE_REC_INDICATOR_KNOWN,
    FACE_REC_INDICATOR_UNKNOWN,
    FACE_REC_INDICATOR_INVALID
} face_rec_indicator_type;

enum
{
    VOICE_CMD_THERM    = 0,
    VOICE_CMD_SECURITY = 1,
    VOICE_CMD_AUDIO    = 2,
    VOICE_CMD_HOME_INVALID
};

enum
{
    VOICE_CMD_MAIN       = 0,
    VOICE_CMD_BACK       = 1,
    VOICE_CMD_NEXT_MUSIC = 2,
    VOICE_CMD_PRE_MUSIC  = 3,
    VOICE_CMD_START_PLAY = 4,
    VOICE_CMD_PAUSE_PLAY = 5,
    VOICE_CMD_VOL_UP     = 6,
    VOICE_CMD_VOL_DOWN   = 7,
    VOICE_CMD_AUDIO_PLAYER_INVALID
};

enum
{
    ICON_PROGRESS_BAR = 0,
    ICON_VIRTUAL_FACE_BLUE,
    ICON_VIRTUAL_FACE_GREEN,
    ICON_VIRTUAL_FACE_RED,
    ICON_INVALID
};

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

static float s_FaceRecProgress = 0.0f;
TimerHandle_t pFaceRecProgressTimer;
#define FACE_REC_UPDATE_INTERVAL (60000 / 100)

static int s_Recognized                           = 0;
static int s_UserId                               = -1;
static face_rec_indicator_type s_FaceRecIndicator = FACE_REC_INDICATOR_INIT;
static bool s_EnterStandby                        = false;
preview_mode_t g_PreviewMode                      = PREVIEW_MODE_CAMERA;

static asr_language_t s_UserLanguage = ASR_ENGLISH;
static event_voice_t s_VoiceEvent;

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

static void _StopVoiceCmd(void);

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

/*******************************************************************************
 * Code
 ******************************************************************************/
#if 0
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
        enable_deregister_button(true);
    }
    else
    {
        enable_deregister_button(false);
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
        set_virtual_face(&s_VirtualFaceImage);
    }
}

static void _PlayPrompt(int id)
{
    static event_common_t s_PlayPromptEvent;
    LOGD("[UI] Play prompt \"%s\"", s_PromptName[id]);
    if (id >= PROMPT_CONFIRM_TONE && id < PROMPT_INVALID)
    {
        s_PlayPromptEvent.eventBase.eventId = kEventID_PlayPrompt;
        s_PlayPromptEvent.data              = (void *)id;

        uint8_t fromISR = __get_IPSR();
        s_OutputDev_UiHomePanel.cap.callback(s_OutputDev_UiHomePanel.id, kOutputEvent_OutputInputNotify,
                                                 &s_PlayPromptEvent, sizeof(s_PlayPromptEvent), fromISR);
    }
}

static void _CheckBrewingTimer_Stop()
{
    if (s_CheckBrewingTimer != NULL)
    {
        xTimerStop(s_CheckBrewingTimer, 0);
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
            (newCoffeeStrength != s_UserCoffeeStrengh))
        {
            // user's new selection is different with registered selection
            return 1;
        }
    }

    return 0;
}

static void _CheckBrewingTimer_Callback(TimerHandle_t xTimer)
{
    coffee_machine_screen_id_t currentScreenId = get_current_screen();
    LOGD("[UI] Screen:%s \"CheckBrewingTimer\" callback %d:%d", get_screen_name(currentScreenId), s_EnterStandby,
         s_IsWaitingRegisterSelection);
    if (currentScreenId == SCR_FINISHED)
    {
        if (_NeedToAskRegister())
        {
            // register selection?
            if (s_IsWaitingRegisterSelection == 0)
            {
                _PlayPrompt(PROMPT_REGISTER_SELECTION);
                s_IsWaitingRegisterSelection = 1;
            }
        }
        else
        {
            LOGD("[UI] Stop \"CheckBrewingTimer\" timer");
            xTimerStop(xTimer, 0);
            if (s_EnterStandby == false)
            {
                set_standby_screen();
            }
        }
    }
    else if (currentScreenId == SCR_HOME || currentScreenId == SCR_STANDBY)
    {
        // stop the timer
        LOGD("[UI] Stop \"CheckBrewingTimer\" timer");
        xTimerStop(xTimer, 0);
    }
}

static void _CheckBrewingTimer_Start()
{
    if (s_CheckBrewingTimer == NULL)
    {
        s_CheckBrewingTimer = xTimerCreate("BrewingTimer", (TickType_t)pdMS_TO_TICKS(3000), pdTRUE, NULL,
                                           (TimerCallbackFunction_t)_CheckBrewingTimer_Callback);
        if (s_CheckBrewingTimer == NULL)
        {
            LOGE("[UI] Failed to create \"CheckBrewingTimer\" timer.");
        }
    }

    if (s_CheckBrewingTimer != NULL)
    {
        if (xTimerStart(s_CheckBrewingTimer, 0) != pdPASS)
        {
            LOGE("Failed to start \"CheckBrewingTimer\" timer.");
        }
        LOGD("[UI] Start \"CheckBrewingTimer\" timer");
    }
}

static void _FaceRecProgressTimer_Start()
{
    if (pFaceRecProgressTimer != NULL)
    {
        if (xTimerStart(pFaceRecProgressTimer, 0) != pdPASS)
        {
            LOGE("[UI] Failed to start \"FaceRecProgress\" timer.");
        }
    }
}

static void _FaceRecProgressTimer_Stop()
{
    if (pFaceRecProgressTimer != NULL)
    {
        xTimerStop(pFaceRecProgressTimer, 0);
    }
}

static void _FaceRecProgressTimer_Callback(TimerHandle_t xTimer)
{
    int drawUpdate = 0;
    if (s_FaceRecProgress < 1.0f)
    {
        s_FaceRecProgress += 0.01f;
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
    static event_voice_t s_VoiceEvent;
    s_VoiceEvent.event_base.eventId = STOP_VOICE_CMD_SESSION;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_UiHomePanel.cap.callback(s_OutputDev_UiHomePanel.id, kOutputEvent_VoiceAlgoInputNotify,
                                             &s_VoiceEvent, sizeof(s_VoiceEvent), fromISR);
}

static void _StopFaceRec(int stop)
{
    static event_face_rec_t s_FaceRecEvent;
    LOGD("[UI] Stop face rec:%d", stop);
    // notify the face rec to start
    s_FaceRecEvent.eventBase.eventId = kEventFaceRecID_OasisSetState;
    if (stop)
    {
        s_FaceRecEvent.oasisState.state = kOasisState_Stopped;
    }
    else
    {
        s_FaceRecEvent.oasisState.state = kOasisState_Running;
    }
    uint8_t fromISR = __get_IPSR();
    s_OutputDev_UiHomePanel.cap.callback(s_OutputDev_UiHomePanel.id, kOutputEvent_VisionAlgoInputNotify,
                                             &s_FaceRecEvent, sizeof(s_FaceRecEvent), fromISR);
}

static void _RegisterCoffeeSelection(coffee_type_t type, coffee_size_t size, coffee_strength_t strength)
{
    static event_smart_tlhmi_t s_TlhmiEvent;
    LOGD("[UI] Register user:%d coffee selection %d:%d:%d", s_UserId, type, size, strength);
    s_TlhmiEvent.eventBase.eventId                 = kEventFaceRecId_RegisterCoffeeSelection;
    s_TlhmiEvent.regCoffeeSelection.id             = s_UserId;
    s_TlhmiEvent.regCoffeeSelection.coffeeType     = type;
    s_TlhmiEvent.regCoffeeSelection.coffeeSize     = size;
    s_TlhmiEvent.regCoffeeSelection.coffeeStrength = strength;
    uint8_t fromISR                                = __get_IPSR();
    s_OutputDev_UiHomePanel.cap.callback(s_OutputDev_UiHomePanel.id, kOutputEvent_VisionAlgoInputNotify,
                                             &s_TlhmiEvent, sizeof(s_TlhmiEvent), fromISR);
}

void DeregisterCoffeeSelection()
{
    static event_face_rec_t s_FaceRecEvent;
    LOGD("[UI] Deregister user:%d %d", s_UserId, s_Recognized);
    if (s_Recognized && s_UserId >= 0)
    {
        s_FaceRecEvent.eventBase.eventId = kEventFaceRecID_DelUser;
        s_FaceRecEvent.delFace.hasID     = 0;
        s_FaceRecEvent.delFace.hasName   = 0;
        uint8_t fromISR                  = __get_IPSR();
        s_OutputDev_UiHomePanel.cap.callback(s_OutputDev_UiHomePanel.id, kOutputEvent_VisionAlgoInputNotify,
                                                 &s_FaceRecEvent, sizeof(s_FaceRecEvent), fromISR);
        s_Recognized = 0;
        s_UserId     = -1;

        s_FaceRecIndicator = FACE_REC_INDICATOR_INIT;
        _DrawPreviewUI(g_PreviewMode, 1, s_FaceRecIndicator, 0, s_FaceRecProgress);
    }
}
#endif

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
    s_OutputDev_UiHomePanel.cap.callback(s_OutputDev_UiHomePanel.id, output_event, fromISR);
}

static void _OutputManagerNotify(event_common_t *event)
{
    output_event_t output_event = {0};
    output_event.eventId        = kOutputEvent_OutputInputNotify;
    output_event.data           = event;
    output_event.copy           = 1;
    output_event.size           = sizeof(event_common_t);
    output_event.eventInfo      = kEventInfo_Remote;

    uint8_t fromISR = __get_IPSR();

    s_OutputDev_UiHomePanel.cap.callback(s_OutputDev_UiHomePanel.id, output_event, fromISR);
}

static void _PlayAudioStream(uint32_t event_id)
{
    event_common_t playAudioEvent;

    memset(&playAudioEvent, 0, sizeof(playAudioEvent));
    playAudioEvent.eventBase.eventId = event_id;
    playAudioEvent.data              = NULL;
    _OutputManagerNotify(&playAudioEvent);
}

void UI_VolumeChangeNotify(uint8_t volume)
{
    event_common_t setVolumeEvent;

    memset(&setVolumeEvent, 0, sizeof(setVolumeEvent));
    setVolumeEvent.eventBase.eventId    = kEventID_SetSpeakerVolume;
    setVolumeEvent.speakerVolume.volume = volume;
    _OutputManagerNotify(&setVolumeEvent);
}

static void _EnableGestureControl(uint8_t enable)
{
    static event_face_rec_t s_EnableGestureControl;
    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_VisionAlgoInputNotify;
    output_event.data      = &s_EnableGestureControl;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_EnableGestureControl);
    output_event.eventInfo = kEventInfo_Remote;

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

    uint8_t fromISR = __get_IPSR();

    s_OutputDev_UiHomePanel.cap.callback(s_OutputDev_UiHomePanel.id, output_event, fromISR);
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
        case kAudioPlayer_Pre:
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
    switch (screenId)
    {
        case kScreen_Home:
        {
            _SetVoiceModel(ASR_CMD_HP_MAIN_MENU, s_UserLanguage, 0);
        }
        break;
        case kScreen_Therm:
        {
        }
        break;
        case kScreen_Security:
        {
        }
        break;
        case kScreen_AudioPlayer:
        {
            /* Set voice model for audio player */
            _SetVoiceModel(ASR_CMD_HP_AUDIO_PLAYER, s_UserLanguage, 0);
            /* Start gesture */
            _EnableGestureControl(true);
        }
        break;
        default:
            break;
    }
}

void UI_ExitScreen_Callback(home_panel_screen_id_t screenId)
{
    switch (screenId)
    {
        case kScreen_Home:
        {
        }
        break;
        case kScreen_Therm:
        {
        }
        break;
        case kScreen_Security:
        {
        }
        break;
        case kScreen_AudioPlayer:
        {
            _EnableGestureControl(false);
        }
        break;
        default:
            break;
    }
}

static hal_output_status_t HAL_OutputDev_UiHomePanel_Init(output_dev_t *dev, output_dev_callback_t callback)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_UiHomePanel_Init");

    dev->cap.callback = callback;

    //    LoadIcons((unsigned char *)APP_ICONS_BASE);

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

    //    pFaceRecProgressTimer =
    //        xTimerCreate("FaceRecProgress", (TickType_t)pdMS_TO_TICKS(FACE_REC_UPDATE_INTERVAL), pdTRUE,
    //                     (void *)&s_FaceRecProgress, (TimerCallbackFunction_t)_FaceRecProgressTimer_Callback);
    //    if (pFaceRecProgressTimer == NULL)
    //    {
    //        LOGE("[UI] Failed to start \"FaceRecProgress\" timer.");
    //    }
    //
    //    s_VirtualFaceSurface.left   = 0;
    //    s_VirtualFaceSurface.top    = 0;
    //    s_VirtualFaceSurface.right  = UI_BUFFER_WIDTH - 1;
    //    s_VirtualFaceSurface.bottom = UI_BUFFER_HEIGHT - 1;
    //    s_VirtualFaceSurface.height = UI_BUFFER_HEIGHT;
    //    s_VirtualFaceSurface.width  = UI_BUFFER_WIDTH;
    //    s_VirtualFaceSurface.pitch  = UI_BUFFER_WIDTH * 2;
    //    s_VirtualFaceSurface.format = kPixelFormat_RGB565;
    //    s_VirtualFaceSurface.buf    = s_VirtualFaceBuffer;
    //    s_VirtualFaceSurface.lock   = NULL;
    //
    //    _DrawVirtualFaceIcon(&s_VirtualFaceSurface, s_Icons[ICON_VIRTUAL_FACE_BLUE]);

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

static hal_output_status_t _InferComplete_Vision(const output_dev_t *dev,
                                                 void *inferResult,
                                                 home_panel_screen_id_t currentScreenId)
{
    hal_output_status_t error              = kStatus_HAL_OutputSuccess;
    vision_algo_result_t *visionAlgoResult = (vision_algo_result_t *)inferResult;
    oasis_lite_result_t *pResult           = NULL;

    /* Call to avoid warning for not used */
    pResult;

    if (visionAlgoResult == NULL)
    {
        LOGD("Receive NULL results");
        return kStatus_HAL_OutputSuccess;
    }

    if (currentScreenId == kScreen_AudioPlayer)
    {
        gesture_result_t *pHandResult = NULL;
        if (visionAlgoResult->id == kVisionAlgoID_UvitaGesture)
        {
            pHandResult = (gesture_result_t *)&(visionAlgoResult->uvitaHandGesture);
        }

        if (pHandResult != NULL)
        {
            static gesture_type oldGestureSign = GESTURE_NONE;
            static hand_lr oldHand             = NONE_HAND;
            LOGI("Hand gesture %d Old Hand gesture %d, Hand %d", pHandResult->hand.gtype, oldGestureSign,
                 pHandResult->hand.left_right)
            if ((oldGestureSign != pHandResult->hand.gtype) && (pHandResult->hand.gtype == GESTURE_OK))
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
                    audioPlayerState = kAudioPlayer_Play;
                }
                set_audio_player_state(audioPlayerState);
                refresh_audio_player_state();
            }
            else if (pHandResult->hand.gtype == GESTURE_THUMB_UP)
            {
                if ((pHandResult->hand.left_right == oldHand) && (oldGestureSign == pHandResult->hand.gtype))
                {
                    /* Nothing to do, same sign */
                }
                else if (LEFT_HAND == pHandResult->hand.left_right)
                {
                    UI_AudioPlaying_Callback(kAudioPlayer_Pre);
                }
                else if (RIGHT_HAND == pHandResult->hand.left_right)
                {
                    UI_AudioPlaying_Callback(kAudioPlayer_Next);
                }
            }
            else if ((oldGestureSign != pHandResult->hand.gtype) && (pHandResult->hand.gtype == GESTURE_PALM))
            {
                int8_t volume = get_speaker_volume();
                volume        = volume + VOLUME_STEP_SIZE;
                if (volume >= kAudioVolume_Max)
                {
                    volume = kAudioVolume_Max - 1;
                }
                set_speaker_volume(volume);
                refresh_audio_player_volume();
                UI_VolumeChangeNotify(volume);
            }
            else if ((oldGestureSign != pHandResult->hand.gtype) && (pHandResult->hand.gtype == GESTURE_FIST))
            {
                int8_t volume = get_speaker_volume();
                volume        = volume - VOLUME_STEP_SIZE;
                if (volume <= kAudioState_Invalid)
                {
                    volume = 0;
                }
                set_speaker_volume(volume);
                refresh_audio_player_volume();
                UI_VolumeChangeNotify(volume);
            }

            if (pHandResult->hand.gtype != GESTURE_NONE)
            {
                oldGestureSign = pHandResult->hand.gtype;
                oldHand        = pHandResult->hand.left_right;
            }
        }
    }

    return error;
}

static hal_output_status_t _InferComplete_Voice(const output_dev_t *dev,
                                                void *inferResult,
                                                home_panel_screen_id_t currentScreenId)
{
    hal_output_status_t error               = kStatus_HAL_OutputSuccess;
    asr_inference_result_t *voiceAlgoResult = (asr_inference_result_t *)inferResult;
    LOGD("[UI] Screen:%d voice command status:%d  cmd:%d", currentScreenId, voiceAlgoResult->status,
         voiceAlgoResult->keywordID);

    if (currentScreenId == kScreen_Home)
    {
        if (voiceAlgoResult->status == ASR_CMD_DETECT && voiceAlgoResult->keywordID > -1)
        {
            // voice command detected
            switch (voiceAlgoResult->keywordID)
            {
                case (VOICE_CMD_THERM):
                {
                    gui_load_screen(kScreen_Therm, 0);
                }
                break;
                case (VOICE_CMD_SECURITY):
                {
                    gui_load_screen(kScreen_Security, 0);
                }
                break;
                case (VOICE_CMD_AUDIO):
                {
                    gui_load_screen(kScreen_AudioPlayer, 0);
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
                case (VOICE_CMD_MAIN):
                case (VOICE_CMD_BACK):
                {
                    gui_load_screen(kScreen_Home, 0);
                }
                break;
                case (VOICE_CMD_NEXT_MUSIC):
                {
                    audio_state_t state = get_audio_player_state();
                    audio_index_t index = get_audio_info_index();
                    switch_audio_clip(index + 1, state);
                }
                break;
                case (VOICE_CMD_PRE_MUSIC):
                {
                    audio_state_t state = get_audio_player_state();
                    audio_index_t index = get_audio_info_index();
                    switch_audio_clip(index - 1, state);
                }
                break;
                case (VOICE_CMD_START_PLAY):
                {
                    switch_audio_player_state(kAudioState_Play);
                }
                break;
                case (VOICE_CMD_PAUSE_PLAY):
                {
                    switch_audio_player_state(kAudioState_Pause);
                }
                case (VOICE_CMD_VOL_UP):
                {
                    // TODO: set speaker volume via voice
                }
                break;
                case (VOICE_CMD_VOL_DOWN):
                {
                    // TODO: set speaker volume via voice
                }
                break;
                default:
                    break;
            }
        }
    }

    return error;
}

static hal_output_status_t HAL_OutputDev_UiHomePanel_InferComplete(const output_dev_t *dev,
                                                                   output_algo_source_t source,
                                                                   void *inferResult)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    if (inferResult == NULL)
    {
        return error;
    }

    LVGL_LOCK();
    home_panel_screen_id_t currentScreenId = get_current_screen();

    if (currentScreenId == kScreen_Num)
    {
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

    if (pEventBase->eventId == kEventID_SetPreviewMode)
    {
        //        event_smart_tlhmi_t *pEvent = (event_smart_tlhmi_t *)pEventBase;
        //        if (g_PreviewMode != pEvent->previewMode)
        //        {
        //            _DrawPreviewUI(pEvent->previewMode, 1, s_FaceRecIndicator, 1, s_FaceRecProgress);
        //            g_PreviewMode = pEvent->previewMode;
        //        }

        //        if (pEventBase->respond != NULL)
        //        {
        //            pEventBase->respond(pEventBase->eventId, &pEvent->previewMode, kEventStatus_Ok, true);
        //        }
    }
    else if (pEventBase->eventId == kEventID_GetPreviewMode)
    {
        if (pEventBase->respond != NULL)
        {
            pEventBase->respond(pEventBase->eventId, &g_PreviewMode, kEventStatus_Ok, true);
        }
    }

    LVGL_LOCK();

    home_panel_screen_id_t currScreen = get_current_screen();
    home_panel_screen_id_t nextScreen = get_next_screen();

    LOGI("CurrScreen %d, NextScreen %d", currScreen, nextScreen);
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

            if (pEvent->mediaPlayerInfo.volume != get_speaker_volume())
            {
                set_speaker_volume(pEvent->mediaPlayerInfo.volume);
                refresh_audio_player_volume();
            }
        }
        else if (pEventBase->eventId == kEventID_MediaPlayer_TrackInfo)
        {
            static uint8_t songArtist[32];
            static uint8_t songName[32];
            event_smart_tlhmi_t *pEvent = (event_smart_tlhmi_t *)pEventBase;

            strcpy(songArtist, (char *)pEvent->trackInfo.songArtist);
            strcpy(songName, (char *)pEvent->trackInfo.songName);

            audio_info_t audioInfo = {
                .name = (char *)songName, .elapsed_time = 0, .total_time = pEvent->trackInfo.songDurationS};
            set_audio_info(audioInfo);
            refresh_audio_player_info();
        }
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
