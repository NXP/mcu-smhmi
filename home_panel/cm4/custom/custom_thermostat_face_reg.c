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

static void _get_register_user_info(thermostat_user_list_obj_t *user_info)
{
    user_info->face_id = UI_GetFaceId();
    if (user_info->face_id == (uint16_t)-1)
    {
        user_info->face_id = gui_thermostat_get_user_item_num();
    }
    snprintf(user_info->user_name, sizeof(user_info->user_name), "%s",
             lv_textarea_get_text(guider_ui.screen_thermostat_face_reg_ta_user_name));

    user_info->mode      = gui_thermostat_get_mode();
    user_info->fan_speed = gui_thermostat_get_fan_speed();

    uint32_t temp = gui_thermostat_get_temperature();
    snprintf(user_info->temperature, sizeof(user_info->temperature), "%lu℃", temp);
}

static void _gui_thermostat_face_reg_set_language_font(language_t language)
{
    lv_style_value_t fontLabel;

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_32 : &lv_font_montserratMedium_32;
    lv_obj_set_local_style_prop(guider_ui.screen_thermostat_face_reg_label_face_reg_state, LV_STYLE_TEXT_FONT,
                                fontLabel, LV_PART_MAIN | LV_STATE_DEFAULT);

    fontLabel.ptr = &lv_font_simsun_50;
    lv_obj_set_local_style_prop(guider_ui.screen_thermostat_face_reg_label_temperature, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_local_style_prop(guider_ui.screen_thermostat_face_reg_label_mode, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_local_style_prop(guider_ui.screen_thermostat_face_reg_label_fan_speed, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
}

void gui_thermostat_lvgl_add_user_info(void)
{
    thermostat_user_list_obj_t user_info;
    _get_register_user_info(&user_info);

    PlayPrompt(PROMPT_REGISTRATION_SUCCESS, 1);
    gui_thermostat_lvgl_add_user_item(&user_info);
}

void gui_thermostat_facereg_enable_save(bool enable)
{
    if (enable)
    {
        lv_imgbtn_set_state(guider_ui.screen_thermostat_face_reg_imgbtn_save, LV_IMGBTN_STATE_RELEASED);
    }
    else
    {
        lv_imgbtn_set_state(guider_ui.screen_thermostat_face_reg_imgbtn_save, LV_IMGBTN_STATE_DISABLED);
    }
}

void gui_thermostat_set_facereg_state(user_state state)
{
    if (state >= kUserState_NoUser && state < kUserState_Max)
    {
        gui_thermostat_set_facerec_state(state);

        if (state == kUserState_Unknown)
        {
            gui_thermostat_facereg_enable_save(true);
        }
        else
        {
            if (state == kUserState_Known)
            {
                PlayPrompt(PROMPT_REGISTERED_USER, 1);
            }
            gui_thermostat_facereg_enable_save(false);
        }

        language_t lang  = UI_GetLanguage();
        lv_label_set_text(guider_ui.screen_thermostat_face_reg_label_face_reg_state,
                          s_UserInteractionStr[lang][state]);
    }
}

void gui_thermostat_face_reg_init_state(language_t language)
{
    char user_name[MAX_THERMOSTAT_USER_NAME_SIZE] = {0};
    char temperature[TEMPERATURE_VALUE_SIZE] = {0};

    uint8_t last_item_no = gui_thermostat_get_user_item_num();
    sprintf(user_name, "User_%02d", last_item_no);
    lv_textarea_set_text(guider_ui.screen_thermostat_face_reg_ta_user_name, user_name);

    _gui_thermostat_face_reg_set_language_font(language);

    snprintf(temperature, sizeof(temperature), "%lu℃", gui_thermostat_get_temperature());
    lv_label_set_text(guider_ui.screen_thermostat_face_reg_label_temperature, temperature);

    thermostat_mode mode = gui_thermostat_get_mode();
    lv_label_set_text(guider_ui.screen_thermostat_face_reg_label_mode, thermostat_mode_string(language, mode));

    thermostat_fan_speed fanspeed = gui_thermostat_get_fan_speed();
    lv_label_set_text(guider_ui.screen_thermostat_face_reg_label_fan_speed,
                      thermostat_fanspeed_string(language, fanspeed));

    PlayPrompt(PROMPT_START_REGISTRATION, 1);
    gui_thermostat_set_facereg_state(kUserState_NoUser);

    static lv_obj_t *kb = NULL;
    if (kb == NULL)
    {
        /* setup keyboard styles Only at first run we know that is child 0 */
        kb = lv_obj_get_child(guider_ui.screen_thermostat_face_reg, 0);
    }
    lv_obj_set_style_text_font(kb, &lv_font_montserratMedium_25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(kb, LV_OPA_COVER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_border_color(guider_ui.screen_thermostat_face_reg_ta_user_name, lv_color_make(0xff, 0xff, 0xff), LV_PART_CURSOR | LV_STATE_FOCUSED);
}

void gui_thermostat_db_add_user(void)
{
    thermostat_user_list_obj_t user_info;
    _get_register_user_info(&user_info);

    UI_Thermostat_UpdateDB_Callback(&user_info, kUserDB_Add);
}

void gui_thermostat_db_remove_user(void)
{
    thermostat_user_list_obj_t *user_info = gui_thermostat_get_selected_user_info();

    UI_Thermostat_UpdateDB_Callback(user_info, kUserDB_Remove);
}

void gui_thermostat_db_update_user()
{
    thermostat_user_list_obj_t *user_info = gui_thermostat_get_selected_user_info();

    UI_Thermostat_UpdateDB_Callback(user_info, kUserDB_Update);
}
