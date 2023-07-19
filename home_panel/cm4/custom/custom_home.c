// SPDX-License-Identifier: MIT
// Copyright 2023 NXP

/**
 * @file custom_home.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "custom.h"
#include "lvgl/lvgl.h"
#include <stdio.h>
#include "ui_strings_internal.h"

void gui_home_set_language_font(language_t language)
{
    lv_style_value_t fontLabel;

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_40 : &lv_font_montserratMedium_38;
    lv_obj_set_local_style_prop(guider_ui.screen_home_label_audio_player, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_local_style_prop(guider_ui.screen_home_label_security, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_local_style_prop(guider_ui.screen_home_label_thermostat, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_50 : &lv_font_montserratMedium_50;
    lv_obj_set_local_style_prop(guider_ui.screen_home_label_homeTitle, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
}

void gui_home_set_language_text(language_t language)
{
    gui_home_set_language_font(language);

    lv_dropdown_set_selected(guider_ui.screen_home_ddlist_language, language);
    lv_label_set_text(guider_ui.screen_home_label_audio_player, s_ScreenName[language][kScreen_AudioPlayer]);
    lv_label_set_text(guider_ui.screen_home_label_security, s_ScreenName[language][kScreen_Security]);
    lv_label_set_text(guider_ui.screen_home_label_thermostat, s_ScreenName[language][kScreen_Thermostat]);
    lv_label_set_text(guider_ui.screen_home_label_homeTitle, s_CalendarString[language][0]);
}

void gui_home_init_state(language_t language)
{
    gui_home_set_language_text(language);
}
