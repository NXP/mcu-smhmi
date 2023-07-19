// SPDX-License-Identifier: MIT
// Copyright 2022 NXP

/**
 * @file custom_security_arming.c
 *
 * security arming animation
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "custom.h"
#include "lvgl/lvgl.h"
#include <stdio.h>
#include "ui_strings_internal.h"

#define COUNT_DOWN_PERIOD  4000 // 4s counting down
#define IDLE_PERIOD        2000 // 2s idle time
#define UPDATE_INTERVAL    100  // 100 ms interval to update the animation
#define TIMER_REPEAT_COUNT ((COUNT_DOWN_PERIOD + IDLE_PERIOD) / (UPDATE_INTERVAL) + 2)

static lv_timer_t *s_arming_timer;
static uint32_t timer_count;

static char *_get_countdown_string(int remaining_seconds)
{
    static char str[32];
    security_state state = gui_security_get_state();
    language_t language = UI_GetLanguage();

    snprintf(str, sizeof(str), s_CountDownStr[language][state], remaining_seconds);
    return str;
}

static void gui_security_arming_update_screen(security_state state)
{
    if (state == kSecurity_On)
    {
        lv_obj_clear_flag(guider_ui.screen_security_arming_img_timer_greed, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_security_arming_img_timer_red, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_security_arming_img_armed, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_security_arming_img_disarmed, LV_OBJ_FLAG_HIDDEN);
    }
    else if (state == kSecurity_Off)
    {
        lv_obj_add_flag(guider_ui.screen_security_arming_img_timer_greed, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_security_arming_img_timer_red, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_security_arming_img_armed, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_security_arming_img_disarmed, LV_OBJ_FLAG_HIDDEN);
    }

    lv_obj_add_state(guider_ui.screen_security_arming_arc_arming_progress, LV_STATE_DISABLED);
}

static void arming_timer(lv_timer_t *timer)
{
    uint32_t elapsed_ms;
    security_state state = gui_security_get_state();
    language_t language = UI_GetLanguage();

    timer_count++;
    elapsed_ms = timer_count * UPDATE_INTERVAL;

    if (elapsed_ms <= COUNT_DOWN_PERIOD)
    {
        lv_arc_set_value(guider_ui.screen_security_arming_arc_arming_progress, elapsed_ms * 100 / COUNT_DOWN_PERIOD);

        int remaining = (COUNT_DOWN_PERIOD - elapsed_ms + 1000) / 1000;
        lv_label_set_text(guider_ui.screen_security_arming_label_countdown, _get_countdown_string(remaining));
    }
    else if (elapsed_ms <= COUNT_DOWN_PERIOD + IDLE_PERIOD)
    {
        if (state == kSecurity_Off)
        {
            // switch to security On state
            gui_security_arming_update_screen(kSecurity_On);
        }
        else if (state == kSecurity_On)
        {
            // switch to security On state
            gui_security_arming_update_screen(kSecurity_Off);
        }

        lv_label_set_text(guider_ui.screen_security_arming_label_countdown, s_ArmedStateStr[language][state]);
    }
    else
    {
        // automatically exit the screen
        security_switch_state();
        gui_load_screen(kScreen_Security_Arming, kScreen_Security, 0);
        lv_timer_del(s_arming_timer);
        s_arming_timer = NULL;
    }
}

static void gui_security_arming_set_language_font(language_t language)
{
    lv_style_value_t fontLabel;

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_50 : &lv_font_montserratMedium_50;

    lv_obj_set_local_style_prop(guider_ui.screen_security_arming_label_title, LV_STYLE_TEXT_FONT, fontLabel,
            LV_PART_MAIN | LV_STATE_DEFAULT);

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_40 : &lv_font_montserratMedium_38;
    lv_obj_set_local_style_prop(guider_ui.screen_security_arming_label_countdown, LV_STYLE_TEXT_FONT, fontLabel,
            LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void gui_security_arming_set_language_text(language_t language)
{
    gui_security_arming_set_language_font(language);

    lv_label_set_text(guider_ui.screen_security_arming_label_title, s_SecurityControlStr[language][0]);
    lv_label_set_text(guider_ui.screen_security_arming_label_countdown, _get_countdown_string(COUNT_DOWN_PERIOD / 1000));
}

static void gui_security_start_arming()
{
    security_state state = gui_security_get_state();
    gui_security_arming_update_screen(state);

    timer_count    = 0;
    s_arming_timer = lv_timer_create(arming_timer, UPDATE_INTERVAL, NULL);
    lv_timer_set_repeat_count(s_arming_timer, TIMER_REPEAT_COUNT);
}

void gui_security_arming_init_state(language_t language)
{
    gui_security_arming_set_language_text(language);
    gui_security_start_arming();
}
