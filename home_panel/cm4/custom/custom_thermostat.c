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

static thermostat_obj_t s_ThermostatObj = {
    .state     = kThermostat_On,
    .mode      = kThermostat_Auto,
    .fan_state = kThermostat_Fan_On,
    .fan_speed = kThermostat_Fan_Mid,
    .temperature = 21,
};

thermostat_state gui_thermostat_get_state()
{
    return s_ThermostatObj.state;
}

static void gui_thermostat_set_state(thermostat_state state)
{
    LVGL_LOG("thermostat: state %d\r\n", state);
    // if (state != s_ThermostatObj.state)
    {
        s_ThermostatObj.state = state;
        if (state == kThermostat_Off)
        {
            lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_on_off, LV_IMGBTN_STATE_CHECKED_RELEASED);
        }
        else
        {
            lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_on_off, LV_IMGBTN_STATE_RELEASED);
        }
    }
}

void gui_thermostat_switch_state(void)
{
    thermostat_state new_state;
    if (s_ThermostatObj.state == kThermostat_Off)
    {
        new_state = kThermostat_On;
    }
    else
    {
        new_state = kThermostat_Off;
    }
    LVGL_LOG("thermostat: old state: %d new state %d\r\n", s_ThermostatObj.state, new_state);
    gui_thermostat_set_state(new_state);

    if (new_state == kThermostat_On)
    {
        UI_EnterScreen_Callback(kScreen_Thermostat);
    }
    else
    {
        gui_thermostat_set_facerec_state(kUserState_NoUser);
        gui_thermostat_enable_confirm_cancel(false);
        UI_ExitScreen_Callback(kScreen_Thermostat);
    }
}

thermostat_mode gui_thermostat_get_mode()
{
    return s_ThermostatObj.mode;
}

void gui_thermostat_set_mode(thermostat_mode mode)
{
    LVGL_LOG("thermostat: mode %d\r\n", mode);
    // if (mode != s_ThermostatObj.state)
    {
        s_ThermostatObj.mode = mode;
        switch (mode)
        {
            case kThermostat_Heat:
            {
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_heat, LV_IMGBTN_STATE_CHECKED_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_auto, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_cool, LV_IMGBTN_STATE_RELEASED);
            }
            break;
            case kThermostat_Cool:
            {
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_heat, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_auto, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_cool, LV_IMGBTN_STATE_CHECKED_RELEASED);
            }
            break;
            case kThermostat_Auto:
            {
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_heat, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_auto, LV_IMGBTN_STATE_CHECKED_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_cool, LV_IMGBTN_STATE_RELEASED);
            }
            break;
            default:
                break;
        }
    }
}

thermostat_fan_speed gui_thermostat_get_fan_speed()
{
    return s_ThermostatObj.fan_speed;
}

void gui_thermostat_set_fan_speed(thermostat_fan_speed speed)
{
    LVGL_LOG("thermostat: fan_speed %d\r\n", speed);
    // if (speed != s_ThermostatObj.fan_speed)
    {
        s_ThermostatObj.fan_speed = speed;
        switch (speed)
        {
            case kThermostat_Fan_High:
            {
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_high, LV_IMGBTN_STATE_CHECKED_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_mid, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_low, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_auto, LV_IMGBTN_STATE_RELEASED);
            }
            break;
            case kThermostat_Fan_Mid:
            {
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_high, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_mid, LV_IMGBTN_STATE_CHECKED_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_low, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_auto, LV_IMGBTN_STATE_RELEASED);
            }
            break;
            case kThermostat_Fan_Low:
            {
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_high, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_mid, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_low, LV_IMGBTN_STATE_CHECKED_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_auto, LV_IMGBTN_STATE_RELEASED);
            }
            break;
            case kThermostat_Fan_Auto:
            {
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_high, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_mid, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_low, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_auto, LV_IMGBTN_STATE_CHECKED_RELEASED);
            }
            break;
            default:
                break;
        }
    }
}

void gui_thermostat_set_fan_state(thermostat_fan_state state)
{
    LVGL_LOG("thermostat: fan state %d\r\n", state);
    // if (state != s_ThermostatObj.fan_state)
    {
        s_ThermostatObj.fan_state = state;
        if (state == kThermostat_Fan_On)
        {
            lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_onoff, LV_IMGBTN_STATE_CHECKED_RELEASED);
        }
        else
        {
            lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_onoff, LV_IMGBTN_STATE_RELEASED);
        }
    }

}

void gui_thermostat_switch_fan_state()
{
    thermostat_fan_state newstate;

    if (s_ThermostatObj.fan_state == kThermostat_Fan_On)
    {
        newstate = kThermostat_Fan_Off;
    }
    else
    {
        newstate = kThermostat_Fan_On;
    }

    gui_thermostat_set_fan_state(newstate);
}

uint32_t gui_thermostat_get_temperature()
{
    return s_ThermostatObj.temperature;
}

void gui_thermostat_set_temperature(uint32_t temp)
{
    if (temp > THERMOSTAT_MAX_TEMPERATURE)
    {
        LVGL_LOG("thermostat: invalid temperature %d\r\n", temp);
        return;
    }

    lv_arc_set_value(guider_ui.screen_thermostat_arc_temp_knob, temp);
    lv_label_set_text_fmt(guider_ui.screen_thermostat_label_temperature, "%lu°C", temp);

    s_ThermostatObj.temperature = temp;
}

void gui_thermostat_set_language_font(language_t language)
{
    lv_style_value_t fontLabel;

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_25 : &lv_font_montserratMedium_24;
    lv_obj_set_local_style_prop(guider_ui.screen_thermostat_label_face_rec_state, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_50 : &lv_font_montserratMedium_50;
    lv_obj_set_local_style_prop(guider_ui.screen_thermostat_label_title, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
}

void gui_thermostat_set_language_text(language_t language)
{
    gui_thermostat_set_language_font(language);

    lv_dropdown_set_selected(guider_ui.screen_thermostat_ddlist_language, language);
    lv_label_set_text(guider_ui.screen_thermostat_label_face_rec_state,
                      s_UserInteractionStr[language][s_ThermostatObj.facerec_state]);
    lv_label_set_text(guider_ui.screen_thermostat_label_title, s_ThermostatTitleStr[language][0]);
}

void gui_thermostat_change_temperature(int delta)
{
    uint32_t newtemp = (int)s_ThermostatObj.temperature + delta;
    gui_thermostat_set_temperature(newtemp);
}

static void gui_thermostat_arc_tempknob_eventcb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    switch (code)
    {
        case LV_EVENT_VALUE_CHANGED:
        {
            uint32_t value = lv_arc_get_value(guider_ui.screen_thermostat_arc_temp_knob);
            gui_thermostat_set_temperature(value);
        }
        break;

        default:
        break;
    }
}

void gui_thermostat_tempknob_event_init()
{
    lv_obj_add_event_cb(guider_ui.screen_thermostat_arc_temp_knob, gui_thermostat_arc_tempknob_eventcb, LV_EVENT_ALL, &guider_ui);
}

static void _gui_thermostat_set_face_icon(user_state state)
{
    if (state == kUserState_Known)
    {
        lv_obj_add_flag(guider_ui.screen_thermostat_img_red, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_thermostat_img_face_green, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_thermostat_img_face_blue, LV_OBJ_FLAG_HIDDEN);
    }
    else if (state == kUserState_NoUser)
    {
        lv_obj_add_flag(guider_ui.screen_thermostat_img_red, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_thermostat_img_face_green, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_thermostat_img_face_blue, LV_OBJ_FLAG_HIDDEN);
    }
    else if (state == kUserState_Unknown)
    {
        lv_obj_clear_flag(guider_ui.screen_thermostat_img_red, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_thermostat_img_face_green, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_thermostat_img_face_blue, LV_OBJ_FLAG_HIDDEN);
    }

    language_t language = UI_GetLanguage();
    lv_label_set_text(guider_ui.screen_thermostat_label_face_rec_state, s_UserInteractionStr[language][state]);
}

void gui_thermostat_set_facerec_state(user_state state)
{
    if (state >= kUserState_NoUser && state < kUserState_Max)
    {
        s_ThermostatObj.facerec_state = state;
        _gui_thermostat_set_face_icon(state);
    }
}

static void gui_thermostat_set_user_preferences(uint16_t face_id)
{
    const thermostat_user_list_obj_t *user_info = gui_thermostat_get_user_info_from_faceid(face_id);

    if (user_info)
    {
        uint32_t temperature = atoi(user_info->temperature);
        gui_thermostat_set_temperature(temperature);
        gui_thermostat_set_mode(user_info->mode);
        gui_thermostat_set_fan_speed(user_info->fan_speed);
    }
}

void gui_thermostat_enable_confirm_cancel(bool enable)
{
    if (enable)
    {
    	language_t language = UI_GetLanguage();
    	lv_label_set_text(guider_ui.screen_thermostat_imgbtn_confirm_label, s_BtnLabelStr[language][BTN_LABEL_CONFIRM]);
    	lv_label_set_text(guider_ui.screen_thermostat_imgbtn_cancel_label, s_BtnLabelStr[language][BTN_LABEL_CANCEL]);
        lv_obj_clear_flag(guider_ui.screen_thermostat_imgbtn_confirm, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_thermostat_imgbtn_cancel, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(guider_ui.screen_thermostat_imgbtn_confirm, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_thermostat_imgbtn_cancel, LV_OBJ_FLAG_HIDDEN);
    }
}

void gui_thermostat_load_setting(bool confirm)
{
    uint16_t face_id = UI_GetFaceId();

    if (confirm)
    {
        gui_thermostat_set_user_preferences(face_id);
    }

    gui_thermostat_enable_confirm_cancel(false);
}

user_state gui_thermostat_get_facerec_state()
{
    return s_ThermostatObj.facerec_state;
}

void gui_thermostat_init_state(language_t language)
{
    gui_thermostat_set_state(s_ThermostatObj.state);
    gui_thermostat_set_mode(s_ThermostatObj.mode);
    gui_thermostat_set_fan_state(s_ThermostatObj.fan_state);
    gui_thermostat_set_fan_speed(s_ThermostatObj.fan_speed);
    gui_thermostat_set_temperature(s_ThermostatObj.temperature);
    gui_thermostat_set_language_text(language);
    gui_thermostat_set_facerec_state(kUserState_NoUser);
    gui_thermostat_tempknob_event_init();

    gui_thermostat_init_userlist_fromdb();

    gui_thermostat_enable_confirm_cancel(false);
}
