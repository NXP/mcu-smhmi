// SPDX-License-Identifier: MIT
// Copyright 2022 NXP

/**
 * @file custom.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "custom.h"
#include "lvgl/lvgl.h"
#include <stdio.h>
#include "ui_strings_internal.h"

static security_obj_t s_SecurityObj = {
    .state             = kSecurity_Uninited,
    .auth_state        = kSecurity_Off,
    .facerec_state     = kUserState_NoUser,
    .front_door_state  = kSecurity_Off,
    .back_door_state   = kSecurity_Off,
    .garage_door_state = kSecurity_Off,
};

#define PASSWORD_LEN    4
static char s_Password[PASSWORD_LEN + 1] = { "1234" };  // admin password
static char s_NewPassword[PASSWORD_LEN + 1];            // new password
static char s_InputPassword[PASSWORD_LEN + 1];          // current entered password
static char s_Stars[PASSWORD_LEN + 1];                  // "* * _ _"
static int  s_PasswordLen;                              // current entered password length
static security_auth_mode s_AuthMode;

static security_door  s_PendingDoor;
static security_state s_PendingDoorState;

security_state gui_security_get_state()
{
    return s_SecurityObj.state;
}

void gui_security_set_state(security_state state)
{
    LVGL_LOG("security: state %d\r\n", state);
    s_SecurityObj.state = state;
    if (state == kSecurity_On)
    {
        lv_obj_clear_flag(guider_ui.screen_security_img_armed, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_security_img_disarmed, LV_OBJ_FLAG_HIDDEN);
        gui_lvgl_enable_buttons(guider_ui.screen_security_imgbtn_user_setting, true);
    }
    else
    {
        lv_obj_add_flag(guider_ui.screen_security_img_armed, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_security_img_disarmed, LV_OBJ_FLAG_HIDDEN);
        gui_lvgl_enable_buttons(guider_ui.screen_security_imgbtn_user_setting, false);
    }
}

void security_switch_state(void)
{
    security_state new_state;

    if (s_SecurityObj.state != kSecurity_On)
    {
        new_state = kSecurity_On;
    }
    else
    {
        new_state = kSecurity_Uninited;
    }

    s_SecurityObj.state = new_state;
}

security_state gui_security_get_door_state(security_door door)
{
    security_state state = kSecurity_Off;

    switch (door)
    {
        case kSecurity_Front:
        {
            state = s_SecurityObj.front_door_state;
        }
        break;

        case kSecurity_Back:
        {
            state = s_SecurityObj.back_door_state;
        }
        break;

        case kSecurity_Garage:
        {
            state = s_SecurityObj.garage_door_state;
        }
        break;

        default:
            break;
    }

    return state;
}

void _gui_security_set_door_state(security_door door, security_state state)
{
    LVGL_LOG("security: door %d state:%d\r\n", door, state);
    security_state *pState = NULL;
    lv_obj_t *pDoorObj     = NULL;

    switch (door)
    {
        case kSecurity_Front:
        {
            pDoorObj = guider_ui.screen_security_imgbtn_front_door;
            pState   = &(s_SecurityObj.front_door_state);
        }
        break;

        case kSecurity_Back:
        {
            pDoorObj = guider_ui.screen_security_imgbtn_back_door;
            pState   = &(s_SecurityObj.back_door_state);
        }
        break;

        case kSecurity_Garage:
        {
            pDoorObj = guider_ui.screen_security_imgbtn_garage_door;
            pState   = &(s_SecurityObj.garage_door_state);
        }
        break;

        default:
            break;
    }

    if (pDoorObj != NULL && pState != NULL)
    {
        *pState = state;
        if (state == kSecurity_Off)
        {
            lv_imgbtn_set_state(pDoorObj, LV_IMGBTN_STATE_RELEASED);
        }
        else
        {
            lv_imgbtn_set_state(pDoorObj, LV_IMGBTN_STATE_CHECKED_RELEASED);
        }
    }
}

void gui_security_switch_door_state(security_door door)
{
    security_state new_state;
    security_state cur_state = gui_security_get_door_state(door);

    if (cur_state == kSecurity_Off)
    {
        new_state = kSecurity_On;
    }
    else
    {
        new_state = kSecurity_Off;
    }

    gui_security_set_door_state(door, new_state);
}

void gui_security_set_door_state(security_door door, security_state state)
{
    if ((s_SecurityObj.state != kSecurity_On) || (s_SecurityObj.auth_state == kSecurity_On))
    {
        security_state cur_state = gui_security_get_door_state(door);

        if (cur_state != state)
        {
            LVGL_LOG("security: door %d old state: %d new state %d\r\n", cur_state, new_state);
            _gui_security_set_door_state(door, state);
        }
    }
    else
    {
        gui_security_set_authentication_mode(kSecurity_Auth_FaceRec);
        s_PendingDoor      = door;
        s_PendingDoorState = state;
        gui_load_screen(kScreen_Security, kScreen_Security_FaceReg, 0);
    }
}

void gui_security_set_language_font(language_t language)
{
    lv_style_value_t fontLabel;
    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_40 : &lv_font_montserratMedium_38;

    lv_obj_set_local_style_prop(guider_ui.screen_security_label_front_door, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_local_style_prop(guider_ui.screen_security_label_back_door, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_local_style_prop(guider_ui.screen_security_label_garage_door, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_local_style_prop(guider_ui.screen_security_label_arm_state, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_50 : &lv_font_montserratMedium_50;
    lv_obj_set_local_style_prop(guider_ui.screen_security_label_title, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
}

void gui_security_set_language_text(language_t language)
{
    gui_security_set_language_font(language);

    lv_dropdown_set_selected(guider_ui.screen_security_ddlist_language, language);

    lv_label_set_text(guider_ui.screen_security_label_front_door, s_DoorStr[language][kSecurity_Front]);
    lv_label_set_text(guider_ui.screen_security_label_back_door, s_DoorStr[language][kSecurity_Back]);
    lv_label_set_text(guider_ui.screen_security_label_garage_door, s_DoorStr[language][kSecurity_Garage]);
    lv_label_set_text(guider_ui.screen_security_label_arm_state, s_SecurityStateStr[language][s_SecurityObj.state]);
    lv_label_set_text(guider_ui.screen_security_label_title, s_SecurityControlStr[language][0]);
}

void gui_security_init_state(language_t language)
{
    _gui_security_set_door_state(kSecurity_Front, s_SecurityObj.front_door_state);
    _gui_security_set_door_state(kSecurity_Back, s_SecurityObj.back_door_state);
    _gui_security_set_door_state(kSecurity_Garage, s_SecurityObj.garage_door_state);
    gui_security_set_state(s_SecurityObj.state);
    gui_security_init_userlist_fromdb();
    gui_security_set_language_text(language);
}

void gui_security_pincode_update_stars()
{
    for (int i = 0; i  < s_PasswordLen; i++)
    {
        s_Stars[i] = '*';
    }

    for (int i = s_PasswordLen; i < PASSWORD_LEN; i++)
    {
        s_Stars[i] = '_';
    }

    if (s_AuthMode >= kSecurity_Auth_Arming && s_AuthMode <= kSecurity_Auth_ConfirmPin)
    {
        lv_label_set_text(guider_ui.screen_security_pincode_label_pin_stars, s_Stars);
    }
    else if ((s_AuthMode == kSecurity_Auth_FaceReg) || (s_AuthMode == kSecurity_Auth_FaceRec))
    {
        lv_label_set_text(guider_ui.screen_security_face_reg_label_stars, s_Stars);
    }
}

void gui_security_pincode_append(char c)
{
    PlayPrompt(PROMPT_BEEP, 1);

    if (s_PasswordLen < PASSWORD_LEN)
    {
        s_InputPassword[s_PasswordLen] = c;
        s_InputPassword[s_PasswordLen + 1] = '\0';
        s_PasswordLen++;
    }
    gui_security_pincode_update_stars();
}

void gui_security_pincode_del()
{
    PlayPrompt(PROMPT_BEEP, 1);

    if (s_PasswordLen > 0)
    {
        s_PasswordLen--;
        s_InputPassword[s_PasswordLen] = '\0';
    }
    gui_security_pincode_update_stars();
}

static const char *get_pincode_hint_string(security_auth_mode mode)
{
    int index           = mode;
    language_t language = UI_GetLanguage();

    if (mode == kSecurity_Auth_Arming)
    {
        // 3 states Uninit/Off/On for Arming mode
        index += s_SecurityObj.state;
    }
    else
    {
        index += kSecurity_On;
    }

    return s_PincodeHintStr[language][index];
}

static bool _auth_correct_password()
{
    if (s_PasswordLen == PASSWORD_LEN && !memcmp(s_Password, s_InputPassword, s_PasswordLen))
    {
        return true;
    }
    return false;
}

static void _auth_password_failed()
{
    PlayPrompt(PROMPT_INVALID_PIN, 1);
    s_PasswordLen = 0;
    gui_security_pincode_update_stars();
}

static security_auth_mode _auth_del_user()
{
    if (_auth_correct_password())
    {
        gui_load_screen(kScreen_Security_Pincode, kScreen_Security_UserList, 0);
        gui_security_del_selected_user();
        return kSecurity_Auth_NumMode;
    }

    _auth_password_failed();
    return kSecurity_Auth_DelUser;
}

static security_auth_mode _auth_reset_pin()
{
    if (_auth_correct_password())
    {
        lv_label_set_text(guider_ui.screen_security_pincode_label_enter_pin,
                          get_pincode_hint_string(kSecurity_Auth_NewPin));
        s_PasswordLen = 0;
        gui_security_pincode_update_stars();

        memset(s_NewPassword, 0, sizeof(s_NewPassword));
        return kSecurity_Auth_NewPin;
    }

    _auth_password_failed();
    return kSecurity_Auth_ResetPin;
}

static security_auth_mode _auth_new_pin()
{
    if (s_PasswordLen == PASSWORD_LEN)
    {
        memcpy(s_NewPassword, s_InputPassword, sizeof(s_InputPassword));

        lv_label_set_text(guider_ui.screen_security_pincode_label_enter_pin,
                          get_pincode_hint_string(kSecurity_Auth_ConfirmPin));
        s_PasswordLen = 0;
        gui_security_pincode_update_stars();

        return kSecurity_Auth_ConfirmPin;
    }
    return kSecurity_Auth_NewPin;
}

static security_auth_mode _auth_confirm_pin()
{
    if (s_PasswordLen == PASSWORD_LEN && !memcmp(s_NewPassword, s_InputPassword, s_PasswordLen))
    {
        memcpy(s_Password, s_InputPassword, sizeof(s_InputPassword));
        memset(s_NewPassword, 0, sizeof(s_NewPassword));

        if (s_SecurityObj.state == kSecurity_Uninited)
        {
            s_SecurityObj.state = kSecurity_Off;
            gui_load_screen(kScreen_Security_Pincode, kScreen_Security_Arming, 0);
        }
        else
        {
            gui_load_screen(kScreen_Security_Pincode, kScreen_Security_UserList, 0);
        }
        return kSecurity_Auth_NumMode;
    }

    _auth_password_failed();
    return kSecurity_Auth_ConfirmPin;
}

static security_auth_mode _auth_face_registration()
{
    if (_auth_correct_password())
    {
        if (s_SecurityObj.facerec_state == kUserState_Unknown)
        {
            lv_obj_clear_flag(guider_ui.screen_security_face_reg_imgbtn_save, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.screen_security_face_reg_imgbtn_cancel, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(guider_ui.screen_security_face_reg_label_stars, LV_OBJ_FLAG_HIDDEN);
            return kSecurity_Auth_NumMode;
        }
    }
    else if (s_SecurityObj.facerec_state == kUserState_Known)
    {
        gui_load_screen(kScreen_Security_FaceReg, kScreen_Security_UserList, 0);
        return kSecurity_Auth_NumMode;
    }
    else
    {
        _auth_password_failed();
    }

    return kSecurity_Auth_FaceReg;
}

static security_auth_mode _auth_arming()
{
    if (s_SecurityObj.state == kSecurity_Uninited)
    {
        return _auth_new_pin();
    }

    if (_auth_correct_password())
    {
        gui_load_screen(kScreen_Security_Pincode, kScreen_Security_Arming, 0);
        return kSecurity_Auth_NumMode;
    }

    _auth_password_failed();
    return kSecurity_Auth_Arming;
}

static void auth_session_timeout_cb(lv_timer_t *timer)
{
    s_SecurityObj.auth_state = kSecurity_Off;
    lv_timer_pause(timer);
}

static void security_start_auth_session()
{
    static lv_timer_t *auth_session_timer = NULL;
    if (auth_session_timer == NULL)
    {
        auth_session_timer = lv_timer_create(auth_session_timeout_cb, 15*1000, NULL);
    }

    lv_timer_reset(auth_session_timer);
    lv_timer_resume(auth_session_timer);

    s_SecurityObj.auth_state = kSecurity_On;
}

static security_auth_mode _auth_face_recognition()
{
    if (_auth_correct_password() || (s_SecurityObj.facerec_state == kUserState_Known))
    {
        security_start_auth_session();
        gui_load_screen(kScreen_Security_FaceReg, kScreen_Security, 0);
        _gui_security_set_door_state(s_PendingDoor, s_PendingDoorState);
        return kSecurity_Auth_NumMode;
    }

    if (!_auth_correct_password())
    {
        _auth_password_failed();
    }
    return kSecurity_Auth_FaceRec;
}

typedef security_auth_mode (*auth_handler)();
static auth_handler s_auth_handlers[kSecurity_Auth_NumMode] = {
    [kSecurity_Auth_Arming] = _auth_arming,          [kSecurity_Auth_DelUser] = _auth_del_user,
    [kSecurity_Auth_ResetPin] = _auth_reset_pin,     [kSecurity_Auth_NewPin] = _auth_new_pin,
    [kSecurity_Auth_ConfirmPin] = _auth_confirm_pin, [kSecurity_Auth_FaceReg] = _auth_face_registration,
    [kSecurity_Auth_FaceRec] = _auth_face_recognition,
};

void gui_security_pincode_ok()
{
    if (s_AuthMode >= kSecurity_Auth_Arming && s_AuthMode < kSecurity_Auth_NumMode)
    {
        s_AuthMode = s_auth_handlers[s_AuthMode]();
    }
}

security_auth_mode gui_security_get_authentication_mode()
{
    return s_AuthMode;
}

void gui_security_set_authentication_mode(security_auth_mode mode)
{
    s_AuthMode    = mode;
    s_PasswordLen = 0;
}

static void gui_security_pincode_set_language_font(language_t language)
{
    lv_style_value_t fontLabel;

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_50 : &lv_font_montserratMedium_50;

    lv_obj_set_local_style_prop(guider_ui.screen_security_pincode_label_title, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_40 : &lv_font_montserratMedium_38;
    lv_obj_set_local_style_prop(guider_ui.screen_security_pincode_label_enter_pin, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
}

void gui_security_pincode_set_language_text(language_t language)
{
    gui_security_pincode_set_language_font(language);

    lv_label_set_text(guider_ui.screen_security_pincode_label_title, s_SecurityControlStr[language][0]);
    lv_label_set_text(guider_ui.screen_security_pincode_label_enter_pin, get_pincode_hint_string(s_AuthMode));
}

void gui_security_pincode_init_state(language_t language)
{
    s_PasswordLen = 0;
    gui_security_pincode_update_stars();

    gui_security_pincode_set_language_text(language);

    if (s_AuthMode == kSecurity_Auth_DelUser)
    {
        PlayPrompt(PROMPT_DELETE_USER_VERIFY_PIN, 1);
    }
}

void gui_security_set_facereg_state(user_state state)
{
    if (state >= kUserState_NoUser && state < kUserState_Max)
    {
        s_SecurityObj.facerec_state = state;

        language_t language = UI_GetLanguage();
        lv_label_set_text(guider_ui.screen_security_face_reg_label_facereg_state, s_UserInteractionStr[language][s_SecurityObj.facerec_state]);

        if (state == kUserState_Known)
        {
            PlayPrompt(PROMPT_REGISTERED_USER, 1);
            vTaskDelay(1000);
            gui_security_pincode_ok();
        }
        else if (state == kUserState_Unknown)
        {
            if (s_AuthMode == kSecurity_Auth_FaceReg)
            {
                PlayPrompt(PROMPT_REGISTRATION_VERIFY_PIN, 1);
            }
        }
    }
}

static void gui_security_facereg_set_language_font(language_t language)
{
    lv_style_value_t fontLabel;

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_32 : &lv_font_montserratMedium_32;
    lv_obj_set_local_style_prop(guider_ui.screen_security_face_reg_label_facereg_state, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_50 : &lv_font_montserratMedium_50;
    lv_obj_set_local_style_prop(guider_ui.screen_security_face_reg_label_title, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
}

void gui_security_face_reg_init_state(language_t language)
{
    s_SecurityObj.facerec_state = kUserState_NoUser;

    gui_security_facereg_set_language_font(language);
    lv_label_set_text(guider_ui.screen_security_face_reg_label_title, s_SecurityControlStr[language][0]);
    lv_label_set_text(guider_ui.screen_security_face_reg_label_facereg_state, s_UserInteractionStr[language][s_SecurityObj.facerec_state]);

    // hide save/cancel button and show pincode stars
    lv_obj_add_flag(guider_ui.screen_security_face_reg_imgbtn_save, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_security_face_reg_imgbtn_cancel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(guider_ui.screen_security_face_reg_label_stars, LV_OBJ_FLAG_HIDDEN);
    gui_security_pincode_update_stars();

    if (s_AuthMode == kSecurity_Auth_FaceReg)
    {
        PlayPrompt(PROMPT_START_REGISTRATION, 1);
    }
}
