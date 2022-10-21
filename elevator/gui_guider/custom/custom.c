// SPDX-License-Identifier: MIT
// Copyright 2022 NXP

/**
 * @file custom.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <stdlib.h>

#include "lvgl/lvgl.h"
#include "custom.h"
#include "ui_strings_internal.h"

#ifdef RT_PLATFORM
#include "FreeRTOS.h"
#include "semphr.h"
#include "lvgl_images_internal.h"
#define LVGL_LOG(...)
#endif /*RT_PLATFORM*/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
#ifdef RT_PLATFORM
/* LVGL should be single thread. Get this mutex for multithread access */
static SemaphoreHandle_t s_LVGLMutex;
#endif /*RT_PLATFORM*/

#define NO_DELAY                        0
#define BUTTON(Floor)                   guider_ui.home_imgbtn_##Floor
#define ELEVATOR_SPEED(secondsPerFloor) (secondsPerFloor * 1000)
#define ELEVATOR_IDLE_DURATION          15000

#define TABLE_DEBUGINFO_BRIGHTNESS_LOW  50
#define TABLE_DEBUGINFO_BRIGHTNESS_HIGH 180

static lv_timer_t *s_MotionTimer;

static uint32_t s_CurFloorNum                           = 1;
static uint32_t s_GoingToFloorNum                       = 1;
static bool s_Arrived                                   = false;
static bool s_AlarmActive                               = false;
static bool s_MicEnabled                                = false;
static bool s_ConfirmCancelEnabled                      = false;
static language_type_t s_Language                       = kLanguageType_English;
static face_rec_state_t s_FaceRecState                  = kFaceRec_NumStates;
screen_t s_ScreenId                                     = kScreen_Main;
static lv_img_dsc_t *s_DateImgs[kLanguageType_NumTypes] = {&_home_date_en_560x90, &_home_date_cn_560x90,
                                                           &_home_date_de_560x90, &_home_date_fr_560x90};

/**
 * @brief Set the language
 *
 * @return language_t of the current language
 */
static void _set_language(language_type_t lang)
{
    if ((lang >= kLanguageType_English) && (lang < kLanguageType_NumTypes))
    {
        s_Language = lang;
    }
}

static void _set_label_font(lv_obj_t *label, language_type_t lang)
{
    if ((lang == kLanguageType_Chinese))
    {
        lv_style_value_t v = {.ptr = &lv_font_simsun_32};
        lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, v, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_style_value_t v = {.ptr = &lv_font_montserratMedium_32};
        lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, v, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

static void _set_label_font_20(lv_obj_t *label, language_type_t lang)
{
    if (lang == kLanguageType_Chinese)
    {
        lv_style_value_t v = {.ptr = &lv_font_simsun_20};
        lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, v, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_style_value_t v = {.ptr = &lv_font_montserratMedium_20};
        lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, v, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

static void _set_label_font_22(lv_obj_t *label, language_type_t lang)
{
    if (lang == kLanguageType_Chinese)
    {
        lv_style_value_t v = {.ptr = &lv_font_simsun_22};
        lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, v, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_style_value_t v = {.ptr = &lv_font_montserratMedium_22};
        lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, v, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

static void _set_label_font_24(lv_obj_t *label, language_type_t lang)
{
    if (lang == kLanguageType_Chinese)
    {
        lv_style_value_t v = {.ptr = &lv_font_simsun_24};
        lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, v, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_style_value_t v = {.ptr = &lv_font_montserratMedium_24};
        lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, v, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

static void _set_label_font_48(lv_obj_t *label, language_type_t lang)
{
    if (lang == kLanguageType_Chinese)
    {
        lv_style_value_t v = {.ptr = &lv_font_simsun_48};
        lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, v, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_style_value_t v = {.ptr = &lv_font_montserratMedium_48};
        lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, v, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

static void _home_update_language(void)
{
    // Language widget
    lv_dropdown_set_selected(guider_ui.home_ddlist_languageSel, s_Language);
    // Date image
    lv_img_set_src(guider_ui.home_img_date, s_DateImgs[s_Language]);
    // Confirm
    _set_label_font(guider_ui.home_imgbtn_confirm_label, s_Language);
    lv_label_set_text(guider_ui.home_imgbtn_confirm_label, s_ConfirmStr[s_Language][0]);
    // Cancel
    _set_label_font(guider_ui.home_imgbtn_cancel_label, s_Language);
    lv_label_set_text(guider_ui.home_imgbtn_cancel_label, s_CancelStr[s_Language][0]);
    // Help screen
    if (s_Language == kLanguageType_English)
    {
        lv_obj_set_pos(guider_ui.home_label_help, 138, 616);
    }
    else
    {
        lv_obj_set_pos(guider_ui.home_label_help, 118, 636);
    }
    _set_label_font(guider_ui.home_label_help, s_Language);
    lv_label_set_text(guider_ui.home_label_help, s_HelpScreenStr[s_Language][0]);
    // Face rec state
    _set_label_font_20(guider_ui.home_label_face_rec_state, s_Language);
    lv_label_set_text(guider_ui.home_label_face_rec_state, s_FaceRecStateStr[s_Language][s_FaceRecState]);
    // Arrived
    _set_label_font_22(guider_ui.home_label_arrived, s_Language);
    lv_label_set_text(guider_ui.home_label_arrived, s_ArrivedStr[s_Language][0]);
}

static void _help_update_language(void)
{
    // Language widget
    lv_dropdown_set_selected(guider_ui.help_ddlist_languageSel, s_Language);
    // Tips
    _set_label_font_48(guider_ui.help_label_say_ww, s_Language);
    lv_label_set_text(guider_ui.help_label_say_ww, s_TipsStr[s_Language][0]);
    _set_label_font_48(guider_ui.help_label_enable_voice, s_Language);
    lv_label_set_text(guider_ui.help_label_enable_voice, s_TipsStr[s_Language][1]);
    _set_label_font_48(guider_ui.help_label_voice_command, s_Language);
    lv_label_set_text(guider_ui.help_label_voice_command, s_TipsStr[s_Language][2]);
    // Back to home
    _set_label_font_48(guider_ui.help_label_back, s_Language);
    lv_label_set_text(guider_ui.help_label_back, s_BackToHomeStr[s_Language][0]);
    // Voice commands
    _set_label_font_24(guider_ui.help_label_commands, s_Language);
    lv_label_set_text(guider_ui.help_label_commands, s_VoiceCommandsStr[s_Language][0]);
    // Voice command list
    _set_label_font_24(guider_ui.help_label_floor_one, s_Language);
    lv_label_set_text(guider_ui.help_label_floor_one, s_VoiceCommandsListStr[s_Language][0]);
    _set_label_font_24(guider_ui.help_label_floor_two, s_Language);
    lv_label_set_text(guider_ui.help_label_floor_two, s_VoiceCommandsListStr[s_Language][1]);
    _set_label_font_24(guider_ui.help_label_floor_three, s_Language);
    lv_label_set_text(guider_ui.help_label_floor_three, s_VoiceCommandsListStr[s_Language][2]);
    _set_label_font_24(guider_ui.help_label_floor_four, s_Language);
    lv_label_set_text(guider_ui.help_label_floor_four, s_VoiceCommandsListStr[s_Language][3]);
    _set_label_font_24(guider_ui.help_label_floor_five, s_Language);
    lv_label_set_text(guider_ui.help_label_floor_five, s_VoiceCommandsListStr[s_Language][4]);
    _set_label_font_24(guider_ui.help_label_floor_six, s_Language);
    lv_label_set_text(guider_ui.help_label_floor_six, s_VoiceCommandsListStr[s_Language][5]);
    _set_label_font_24(guider_ui.help_label_confirm, s_Language);
    lv_label_set_text(guider_ui.help_label_confirm, s_VoiceCommandsListStr[s_Language][6]);
    _set_label_font_24(guider_ui.help_label_cancel, s_Language);
    lv_label_set_text(guider_ui.help_label_cancel, s_VoiceCommandsListStr[s_Language][7]);
    _set_label_font_24(guider_ui.help_label_delete_user, s_Language);
    lv_label_set_text(guider_ui.help_label_delete_user, s_VoiceCommandsListStr[s_Language][8]);
}

static void _enable_floor_buttons(void)
{
    lv_obj_add_flag(BUTTON(1), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(BUTTON(2), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(BUTTON(3), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(BUTTON(4), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(BUTTON(5), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(BUTTON(6), LV_OBJ_FLAG_CLICKABLE);
}

static void _disable_floor_buttons(void)
{
    lv_obj_clear_flag(BUTTON(1), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(BUTTON(2), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(BUTTON(3), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(BUTTON(4), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(BUTTON(5), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(BUTTON(6), LV_OBJ_FLAG_CLICKABLE);
}

static void _turn_on_mic(void)
{
    lv_obj_add_flag(guider_ui.home_img_mic_off, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(guider_ui.home_img_mic_on, LV_OBJ_FLAG_HIDDEN);
}

static void _turn_off_mic(void)
{
    lv_obj_add_flag(guider_ui.home_img_mic_on, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(guider_ui.home_img_mic_off, LV_OBJ_FLAG_HIDDEN);
}

static void _disable_confirm_cancel(void)
{
    lv_obj_add_flag(guider_ui.home_imgbtn_confirm, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.home_imgbtn_cancel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.home_imgbtn_confirm_label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.home_imgbtn_cancel_label, LV_OBJ_FLAG_HIDDEN);

    lv_obj_clear_flag(guider_ui.home_img_temp, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(guider_ui.home_img_sun, LV_OBJ_FLAG_HIDDEN);
}

static void _enable_confirm_cancel(void)
{
    lv_obj_clear_flag(guider_ui.home_imgbtn_confirm, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(guider_ui.home_imgbtn_cancel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(guider_ui.home_imgbtn_confirm_label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(guider_ui.home_imgbtn_cancel_label, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(guider_ui.home_img_temp, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.home_img_sun, LV_OBJ_FLAG_HIDDEN);
}

static void _mark_floor_button(uint32_t floorNum)
{
    switch (floorNum)
    {
        case 1:
            lv_obj_add_state(BUTTON(1), LV_STATE_CHECKED | LV_STATE_FOCUSED);
            break;
        case 2:
            lv_obj_add_state(BUTTON(2), LV_STATE_CHECKED | LV_STATE_FOCUSED);
            break;
        case 3:
            lv_obj_add_state(BUTTON(3), LV_STATE_CHECKED | LV_STATE_FOCUSED);
            break;
        case 4:
            lv_obj_add_state(BUTTON(4), LV_STATE_CHECKED | LV_STATE_FOCUSED);
            break;
        case 5:
            lv_obj_add_state(BUTTON(5), LV_STATE_CHECKED | LV_STATE_FOCUSED);
            break;
        case 6:
            lv_obj_add_state(BUTTON(6), LV_STATE_CHECKED | LV_STATE_FOCUSED);
            break;
    }
}

static void _unmark_floor_button(uint32_t floorNum)
{
    switch (floorNum)
    {
        case 1:
            lv_obj_clear_state(BUTTON(1), LV_STATE_CHECKED);
            PlayPrompt(PROMPT_FLOORONE, 0);
            break;
        case 2:
            lv_obj_clear_state(BUTTON(2), LV_STATE_CHECKED);
            PlayPrompt(PROMPT_FLOORTWO, 0);
            break;
        case 3:
            lv_obj_clear_state(BUTTON(3), LV_STATE_CHECKED);
            PlayPrompt(PROMPT_FLOORTHREE, 0);
            break;
        case 4:
            lv_obj_clear_state(BUTTON(4), LV_STATE_CHECKED);
            PlayPrompt(PROMPT_FLOORFOUR, 0);
            break;
        case 5:
            lv_obj_clear_state(BUTTON(5), LV_STATE_CHECKED);
            PlayPrompt(PROMPT_FLOORFIVE, 0);
            break;
        case 6:
            lv_obj_clear_state(BUTTON(6), LV_STATE_CHECKED);
            PlayPrompt(PROMPT_FLOORSIX, 0);
            break;
        default:
            break;
    }
}

static void _init_home_screen(void)
{
    // Update to default language
    _home_update_language();

    // set the virtual face
    if (s_FaceRecState == kFaceRec_NumStates)
    {
        gui_home_update_face_rec_state(kFaceRec_NoUser);
    }
    else
    {
        gui_home_update_face_rec_state(s_FaceRecState);
    }

    // start at first floor (last index of roller)
    lv_roller_set_selected(guider_ui.home_roller_floorNum, ROLLER_INDEX(get_current_floor()), LV_ANIM_OFF);

    // disable moving the floor number by hand
    lv_obj_clear_flag(guider_ui.home_roller_floorNum, LV_OBJ_FLAG_CLICKABLE);

    // hide arrow while not moving
    lv_obj_add_flag(guider_ui.home_img_arrow, LV_OBJ_FLAG_HIDDEN);

    // hide arrived label
    lv_obj_add_flag(guider_ui.home_label_arrived, LV_OBJ_FLAG_HIDDEN);

    // init the confirm and cancel buttons
    gui_enable_confirm_cancel(s_ConfirmCancelEnabled);

    // init the mic state
    gui_enable_mic(s_MicEnabled);
}

void _idle_callback(lv_timer_t *timer)
{
    if (!elevator_in_motion())
    {
        s_CurFloorNum     = 1;
        s_GoingToFloorNum = 1;
        lv_roller_set_selected(guider_ui.home_roller_floorNum, ROLLER_INDEX(s_CurFloorNum), LV_ANIM_ON);
    }
}

static void _arrive_floor_callback(lv_timer_t *timer)
{
    // add or subtract from floor count based on whether going up or down
    uint32_t *floorNum = timer->user_data;

    // going down
    if (*floorNum < s_CurFloorNum)
    {
        PlayPrompt(PROMPT_CONFIRM_TONE, 0);
        s_CurFloorNum--;
        lv_roller_set_selected(guider_ui.home_roller_floorNum, ROLLER_INDEX(s_CurFloorNum), LV_ANIM_ON);
    }
    // going up
    else if (*floorNum > s_CurFloorNum)
    {
        PlayPrompt(PROMPT_CONFIRM_TONE, 0);
        s_CurFloorNum++;
        lv_roller_set_selected(guider_ui.home_roller_floorNum, ROLLER_INDEX(s_CurFloorNum), LV_ANIM_ON);
    }
    // arrived
    else
    {
        if (s_Arrived)
        {
            PlayPrompt(PROMPT_OPEN_DOOR_TONE, 0);
            // hide "you have arrived" message and enable the buttons for next travel
            if (lv_scr_act() == guider_ui.home && lv_obj_is_valid(guider_ui.home_label_arrived))
            {
                lv_obj_add_flag(guider_ui.home_label_arrived, LV_OBJ_FLAG_HIDDEN);
            }

            _enable_floor_buttons();

            UI_ElevatorArrived_Callback();
        }
        else
        {
            // arrive the destination floor.
            _unmark_floor_button(*floorNum);

            // hide the arrow
            lv_obj_add_flag(guider_ui.home_img_arrow, LV_OBJ_FLAG_HIDDEN);

            // show "you have arrived" message
            lv_obj_clear_flag(guider_ui.home_label_arrived, LV_OBJ_FLAG_HIDDEN);
            s_Arrived = true;
        }
    }
}

void gui_set_virtual_face(virtual_face_color_t color)
{
    if ((guider_ui.main == NULL) || (color < kVirtualFace_Red || kVirtualFace_Invalid < color))
    {
        return;
    }

    switch (color)
    {
        case kVirtualFace_Blue:
        {
            lv_obj_clear_flag(guider_ui.home_img_faceNotDetected, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(guider_ui.home_img_faceDetected, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(guider_ui.home_img_faceRecognized, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(guider_ui.home_imgbtn_deregister, LV_OBJ_FLAG_HIDDEN);
        }
        break;
        case kVirtualFace_Red:
        {
            lv_obj_clear_flag(guider_ui.home_img_faceDetected, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(guider_ui.home_img_faceNotDetected, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(guider_ui.home_img_faceRecognized, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(guider_ui.home_imgbtn_deregister, LV_OBJ_FLAG_HIDDEN);
        }
        break;
        case kVirtualFace_Green:
        {
            lv_obj_clear_flag(guider_ui.home_img_faceRecognized, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(guider_ui.home_img_faceNotDetected, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(guider_ui.home_img_faceDetected, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.home_imgbtn_deregister, LV_OBJ_FLAG_HIDDEN);
        }
        break;
        default:
        {
        }
        break;
    }
    lv_label_set_text(guider_ui.home_label_face_rec_state, s_FaceRecStateStr[s_Language][s_FaceRecState]);
}

bool elevator_in_motion(void)
{
    return s_CurFloorNum != s_GoingToFloorNum;
}

uint32_t get_current_floor(void)
{
    return s_CurFloorNum;
}

face_rec_state_t get_current_face_rec_state(void)
{
    return s_FaceRecState;
}

/**
 * @brief get screen name
 *
 */
const char *get_screen_name(screen_t scr)
{
    return s_ScreenName[scr];
}

void go_to_floor(uint32_t floorNum)
{
    if (floorNum < 1 || floorNum > NUM_FLOORS)
    {
        return;
    }
    // Elevator demo cannot travel to multiple floors in one 'trip'
    else if (elevator_in_motion())
    {
        return;
    }

    UI_ElevatorStart_Callback();
    _mark_floor_button(floorNum);
    s_GoingToFloorNum = floorNum;
    s_Arrived         = false;
    _disable_floor_buttons();

    lv_obj_add_flag(guider_ui.home_label_arrived, LV_OBJ_FLAG_HIDDEN);
    if (s_GoingToFloorNum < s_CurFloorNum)
    {
        // flip arrow direction down
        lv_obj_clear_flag(guider_ui.home_img_arrow, LV_OBJ_FLAG_HIDDEN);
        lv_img_set_angle(guider_ui.home_img_arrow, 1800);
    }
    else if (s_GoingToFloorNum > s_CurFloorNum)
    {
        // flip arrow direction up
        lv_obj_clear_flag(guider_ui.home_img_arrow, LV_OBJ_FLAG_HIDDEN);
        lv_img_set_angle(guider_ui.home_img_arrow, 0);
    }

    s_MotionTimer = lv_timer_create(_arrive_floor_callback, ELEVATOR_SPEED(1), &s_GoingToFloorNum);
    // repeat count: total floors + show "You have arrived"
    // the timer will be automatically delete after reached the repeat count
    lv_timer_set_repeat_count(s_MotionTimer, abs(s_CurFloorNum - s_GoingToFloorNum) + 2);
}

/**
 * @brief setup the screen specified by `id` after `delay` milliseconds
 *
 * @param scr_id the id of the screen to load
 * @param delay milliseconds to delay
 */
static void gui_setup_screen(screen_t id, int delay)
{
    lv_obj_t **scr_to_load = NULL;
    lv_disp_t *d           = lv_obj_get_disp(lv_scr_act());
    lv_anim_del_all();

    switch (id)
    {
        case kScreen_Home:
        {
            if (d->prev_scr == NULL && d->scr_to_load == NULL)
            {
                scr_to_load = &guider_ui.home;
                if (!lv_obj_is_valid(*scr_to_load))
                {
                    setup_scr_home(&guider_ui);
                    lv_obj_add_flag(guider_ui.home_label_algoinfo, LV_OBJ_FLAG_HIDDEN);
                }
                _init_home_screen();
            }
        }
        break;

        case kScreen_Help:
        {
            if (d->prev_scr == NULL && d->scr_to_load == NULL)
            {
                scr_to_load = &guider_ui.help;
                if (!lv_obj_is_valid(*scr_to_load))
                {
                    setup_scr_help(&guider_ui);
                }
                _help_update_language();
            }
        }
        break;
        default:
            break;
    }

    if (*scr_to_load)
    {
        s_ScreenId = id;
        lv_scr_load_anim(*scr_to_load, LV_SCR_LOAD_ANIM_NONE, 0, delay, false);
    }
}

screen_t gui_get_screen_id(void)
{
    return s_ScreenId;
}

void gui_set_home(void)
{
    if (UI_EnterScreen_Callback(kScreen_Home))
    {
        gui_setup_screen(kScreen_Home, NO_DELAY);
    }
}

void gui_set_help(void)
{
    if (UI_EnterScreen_Callback(kScreen_Help))
    {
        gui_setup_screen(kScreen_Help, NO_DELAY);
    }
}

void gui_main_anim_ready_cb(struct _lv_anim_t *anim)
{
    gui_set_home();
}

void gui_home_update_face_rec_state(face_rec_state_t state)
{
    if ((state >= kFaceRec_NoUser) && (state < kFaceRec_NumStates) && (state != s_FaceRecState))
    {
        s_FaceRecState = state;

        _set_label_font_20(guider_ui.home_label_face_rec_state, s_Language);
        lv_label_set_text(guider_ui.home_label_face_rec_state, s_FaceRecStateStr[s_Language][s_FaceRecState]);
        if (state == kFaceRec_NoUser)
        {
            gui_set_virtual_face(kVirtualFace_Blue);
        }
        else if (state == kFaceRec_NewUser)
        {
            gui_set_virtual_face(kVirtualFace_Red);
        }
        else if (state == kFaceRec_KnownUser)
        {
            gui_set_virtual_face(kVirtualFace_Green);
        }
    }
}

/**
 * @brief get the language type selected on main screen
 *
 * @return language_type_t the current selected language type
 *
 */
language_type_t gui_get_language_type(void)
{
    return s_Language;
}

void gui_home_set_language(language_type_t language)
{
    if ((language > kLanguageType_Invalid) && (language < kLanguageType_NumTypes) && (language != s_Language))
    {
        _set_language(language);
        lv_dropdown_set_selected(guider_ui.home_ddlist_languageSel, language);
        _home_update_language();
        UI_SetLanguage_Callback(language);
    }
}

void gui_home_language_changed_cb(void)
{
    uint16_t opt_id              = lv_dropdown_get_selected(guider_ui.home_ddlist_languageSel);
    language_type_t new_language = (language_type_t)opt_id;
    LVGL_LOG("+-gui_home_language_changed_cb:[%d:%d]\r\n", s_Language, new_language);
    if ((new_language >= kLanguageType_English) && (new_language < kLanguageType_NumTypes) &&
        (new_language != s_Language))
    {
        _set_language(new_language);
        _home_update_language();
        UI_SetLanguage_Callback(new_language);
    }
}

void gui_help_language_changed_cb(void)
{
    uint16_t opt_id              = lv_dropdown_get_selected(guider_ui.help_ddlist_languageSel);
    language_type_t new_language = (language_type_t)opt_id;
    LVGL_LOG("+-gui_help_language_changed_cb:[%d:%d]\r\n", s_Language, new_language);
    if ((new_language >= kLanguageType_English) && (new_language < kLanguageType_NumTypes) &&
        (new_language != s_Language))
    {
        _set_language(new_language);
        _help_update_language();
        UI_SetLanguage_Callback(new_language);
    }
}

void gui_enable_mic(bool enable)
{
    if (enable)
    {
        _turn_on_mic();
    }
    else
    {
        _turn_off_mic();
    }
    s_MicEnabled = enable;
}

void gui_enable_confirm_cancel(bool enable)
{
    if (enable)
    {
        _enable_confirm_cancel();
    }
    else
    {
        _disable_confirm_cancel();
    }
    s_ConfirmCancelEnabled = enable;
}

void gui_set_algo_debuginfo(uint8_t isDetFace,
                            uint8_t isSmallFace,
                            uint8_t isBlurry,
                            uint8_t isSideface,
                            uint8_t brightness,
                            int recognized,
                            int id)
{
    char labelInfo[20];
    char *curText = lv_label_get_text(guider_ui.home_label_algoinfo);

    if (!isDetFace)
    {
        sprintf(labelInfo, "No Face");
    }
    else if (isSmallFace)
    {
        sprintf(labelInfo, "Small");
    }
    else if (isBlurry)
    {
        sprintf(labelInfo, "Blurry");
    }
    else if (isSideface)
    {
        sprintf(labelInfo, "Side");
    }
    else if (brightness)
    {
        if (brightness > TABLE_DEBUGINFO_BRIGHTNESS_HIGH)
        {
            sprintf(labelInfo, "Light [%d]", brightness);
        }
        else if (brightness < TABLE_DEBUGINFO_BRIGHTNESS_LOW)
        {
            sprintf(labelInfo, "Dark [%d]", brightness);
        }
    }
    else
    {
        if (recognized && (id >= 0))
        {
            sprintf(labelInfo, "User_%02d", id);
        }
        else if (recognized && (id < 0))
        {
            sprintf(labelInfo, "New user");
        }
        else
        {
            sprintf(labelInfo, "Invalid");
        }
    }

    if ((curText != NULL) && strcmp(curText, labelInfo))
    {
        lv_label_set_text(guider_ui.home_label_algoinfo, labelInfo);
    }
}

bool gui_get_alarm_status(void)
{
    return s_AlarmActive;
}

void gui_set_alarm_status(bool status)
{
    s_AlarmActive = status;
}

void handle_alarm_action(void)
{
    if (s_AlarmActive == false)
    {
        gui_set_alarm_status(true);
        PlayPrompt(PROMPT_ALARM_TONE, 0);
    }
    else if (s_AlarmActive == true)
    {
        gui_set_alarm_status(false);
        StopPrompt();
    }
}

void gui_home_debug_info_cb(void)
{
    bool isEnabled = FaceRecDebugOptionTrigger();
    if (isEnabled)
    {
        lv_obj_clear_flag(guider_ui.home_label_algoinfo, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(guider_ui.home_label_algoinfo, LV_OBJ_FLAG_HIDDEN);
    }
}

/**
 * Initializes demo application
 */
void custom_init(lv_ui *ui)
{ /* Add your code here */
#ifdef RT_PLATFORM
    s_LVGLMutex = xSemaphoreCreateMutex();

    if (s_LVGLMutex == NULL)
    {
        while (1)
        {
        }
    }
#endif
}

#ifdef RT_PLATFORM
void _takeLVGLMutex()
{
    if (s_LVGLMutex)
    {
        xSemaphoreTake(s_LVGLMutex, portMAX_DELAY);
    }
}

void _giveLVGLMutex()
{
    if (s_LVGLMutex)
    {
        xSemaphoreGive(s_LVGLMutex);
    }
}

void setup_imgs(unsigned char *base)
{
    _back_alpha_81x82.data                  = (base + 0);
    _button_call_alpha_90x90.data           = (base + 19968);
    _button_call_pressed_alpha_90x90.data   = (base + 44288);
    _button_close_alpha_90x90.data          = (base + 68608);
    _button_close_pressed_alpha_90x90.data  = (base + 92928);
    _button_delete_alpha_62x62.data         = (base + 117248);
    _button_delete_pressed_alpha_62x62.data = (base + 128832);
    _button_gold_alpha_90x90.data           = (base + 140416);
    _button_gold_pressed_alpha_90x90.data   = (base + 164736);
    _button_open_alpha_90x90.data           = (base + 189056);
    _button_open_pressed_alpha_90x90.data   = (base + 213376);
    _button_silver_alpha_90x90.data         = (base + 237696);
    _button_silver_pressed_alpha_90x90.data = (base + 262016);
    _home_background_1280x720.data          = (base + 286336);
    _home_cancel_264x139.data               = (base + 2129536);
    _home_confirm_264x139.data              = (base + 2202944);
    _home_date_cn_560x90.data               = (base + 2276352);
    _home_date_de_560x90.data               = (base + 2377152);
    _home_date_en_560x90.data               = (base + 2477952);
    _home_date_fr_560x90.data               = (base + 2578752);
    _home_help_91x91.data                   = (base + 2679552);
    _home_mic_off_61x111.data               = (base + 2696128);
    _home_mic_on_118x197.data               = (base + 2709696);
    _home_sun_128x135.data                  = (base + 2756224);
    _home_temp_262x95.data                  = (base + 2790784);
    _home_time_225x55.data                  = (base + 2840576);
    main_animimg_timeoutmain_rb_10_10.data  = (base + 2865344);
    _main_elevator_1280x720.data            = (base + 2865664);
    _nxp_logo_113x38.data                   = (base + 4708864);
    _up_64x56.data                          = (base + 4721792);
    _virtual_face_blue_180x185.data         = (base + 4732544);
    _virtual_face_green_180x185.data        = (base + 4799168);
    _virtual_face_red_180x185.data          = (base + 4865792);

    // Images Total: 0x4b4340, 4932416
}
#else
// For LVGL Emulator to swith the UI
void enable_disable_confirm_cancel_callback(void)
{
    static int on_off = 0;

    if (on_off)
    {
        _enable_confirm_cancel();
    }
    else
    {
        _disable_confirm_cancel();
    }

    on_off = ~on_off;
}

void turn_on_off_mic_callback(void)
{
    static int on_off = 0;

    if (on_off)
    {
        _turn_on_mic();
    }
    else
    {
        _turn_off_mic();
    }

    on_off = ~on_off;
}
#endif /*RT_PLATFORM*/
