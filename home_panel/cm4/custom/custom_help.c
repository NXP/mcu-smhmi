// SPDX-License-Identifier: MIT
// Copyright 2023 NXP

/**
 * @file custom_help.c
 *
 * help screens
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "custom.h"
#include "lvgl/lvgl.h"
#include <stdio.h>
#include "ui_strings_internal.h"

static void gui_help_set_title_language(lv_obj_t *label, language_t language, const char *str)
{
    lv_style_value_t fontLabel;
    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_50 : &lv_font_montserratMedium_50;
    lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, fontLabel, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(label, str);
}

static void gui_help_show_objs(lv_obj_t **objs)
{
    for (lv_obj_t **obj = objs; *obj; obj++)
    {
        lv_obj_clear_flag(*obj, LV_OBJ_FLAG_HIDDEN);
    }
}

static void gui_help_hide_objs(lv_obj_t **objs)
{
    for (lv_obj_t **obj = objs; *obj; obj++)
    {
        lv_obj_add_flag(*obj, LV_OBJ_FLAG_HIDDEN);
    }
}

static void gui_help_update_objs(lv_obj_t **en, lv_obj_t **cn, language_t language)
{
    if (language == kLanguage_EN)
    {
        gui_help_show_objs(en);
        gui_help_hide_objs(cn);
    }
    else if (language == kLanguage_CN)
    {
        gui_help_show_objs(cn);
        gui_help_hide_objs(en);
    }
}

static void gui_help_home_set_language_text(language_t language)
{
    lv_dropdown_set_selected(guider_ui.screen_help_home_ddlist_language, language);

    gui_help_set_title_language(guider_ui.screen_help_home_label_title, language, s_CalendarString[language][0]);

    lv_obj_t *en_list[] =
    {
        guider_ui.screen_help_home_label_wakeword_en,
        guider_ui.screen_help_home_label_voicecmd_en,
        guider_ui.screen_help_home_label_cmdlist_en,
        NULL,
    };
    lv_obj_t *cn_list[] =
    {
        guider_ui.screen_help_home_label_wakeword_cn,
        guider_ui.screen_help_home_label_voicecmd_cn,
        guider_ui.screen_help_home_label_cmdlist_cn,
        NULL,
    };
    gui_help_update_objs(en_list, cn_list, language);
}

void gui_help_home_init_state()
{
    language_t language = UI_GetLanguage();
    gui_help_home_set_language_text(language);
}

void gui_help_home_language_changed_cb(void)
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.screen_help_home_ddlist_language);
    language_t new_language = (language_t)opt_id;

    if (gui_language_change_cb(new_language) == 0)
    {
        gui_help_home_set_language_text(new_language);
    }
}

static void gui_help_thermostat_set_language_text(language_t language)
{
    lv_dropdown_set_selected(guider_ui.screen_help_thermostat_ddlist_language, language);

    gui_help_set_title_language(guider_ui.screen_help_thermostat_label_title, language, s_ThermostatTitleStr[language][0]);

    lv_obj_t *en_list[] =
    {
        guider_ui.screen_help_thermostat_label_wakeword_en,
        guider_ui.screen_help_thermostat_label_voicecmd_en,
        guider_ui.screen_help_thermostat_label_cmdlist_en,
        guider_ui.screen_help_thermostat_label_cmdlist2_en,
        NULL,
    };
    lv_obj_t *cn_list[] =
    {
        guider_ui.screen_help_thermostat_label_wakeword_cn,
        guider_ui.screen_help_thermostat_label_voicecmd_cn,
        guider_ui.screen_help_thermostat_label_cmdlist_cn,
        guider_ui.screen_help_thermostat_label_cmdlist2_cn,
        NULL,
    };

    gui_help_update_objs(en_list, cn_list, language);
}

void gui_help_thermostat_init_state()
{
    language_t language = UI_GetLanguage();
    gui_help_thermostat_set_language_text(language);
}

void gui_help_thermostat_language_changed_cb()
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.screen_help_thermostat_ddlist_language);
    language_t new_language = (language_t)opt_id;

    if (gui_language_change_cb(new_language) == 0)
    {
        gui_help_thermostat_set_language_text(new_language);
    }
}

static void gui_help_security_set_language_text(language_t language)
{
    lv_dropdown_set_selected(guider_ui.screen_help_security_ddlist_language, language);

    gui_help_set_title_language(guider_ui.screen_help_security_label_title, language, s_SecurityControlStr[language][0]);

    lv_obj_t *en_list[] =
    {
        guider_ui.screen_help_security_label_wakeword_en,
        guider_ui.screen_help_security_label_voicecmd_en,
        guider_ui.screen_help_security_label_cmdlist_en,
        NULL,
    };
    lv_obj_t *cn_list[] =
    {
        guider_ui.screen_help_security_label_wakeword_cn,
        guider_ui.screen_help_security_label_voicecmd_cn,
        guider_ui.screen_help_security_label_cmdlist_cn,
        NULL,
    };

    gui_help_update_objs(en_list, cn_list, language);
}

void gui_help_security_init_state()
{
    language_t language = UI_GetLanguage();
    gui_help_security_set_language_text(language);
}

void gui_help_security_language_changed_cb()
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.screen_help_security_ddlist_language);
    language_t new_language = (language_t)opt_id;

    if (gui_language_change_cb(new_language) == 0)
    {
        gui_help_security_set_language_text(new_language);
    }
}

static void gui_help_audioplayer_set_language_text(language_t language)
{
    lv_dropdown_set_selected(guider_ui.screen_help_audioplayer_ddlist_language, language);

    gui_help_set_title_language(guider_ui.screen_help_audioplayer_label_title, language, s_AudioPlayerTitleStr[language][0]);

    lv_obj_t *en_list[] =
    {
        guider_ui.screen_help_audioplayer_label_wakeword_en,
        guider_ui.screen_help_audioplayer_label_voicecmd_en,
        guider_ui.screen_help_audioplayer_label_cmdlist_en,
        NULL,
    };
    lv_obj_t *cn_list[] =
    {
        guider_ui.screen_help_audioplayer_label_wakeword_cn,
        guider_ui.screen_help_audioplayer_label_voicecmd_cn,
        guider_ui.screen_help_audioplayer_label_cmdlist_cn,
        NULL,
    };

    gui_help_update_objs(en_list, cn_list, language);
}

void gui_help_audioplayer_init_state()
{
    language_t language = UI_GetLanguage();
    gui_help_audioplayer_set_language_text(language);
}

void gui_help_audioplayer_language_changed_cb()
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.screen_help_audioplayer_ddlist_language);
    language_t new_language = (language_t)opt_id;

    if (gui_language_change_cb(new_language) == 0)
    {
        gui_help_audioplayer_set_language_text(new_language);
    }
}
