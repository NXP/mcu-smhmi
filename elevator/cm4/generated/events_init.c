/*
 * Copyright 2022 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl/lvgl.h"





#include "custom.h"













void events_init(lv_ui *ui)
{
}

static void main_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_set_home();
	}
		break;
	default:
		break;
	}
}

void events_init_main(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->main_imgbtn_home, main_imgbtn_home_event_handler, LV_EVENT_ALL, NULL);
}

static void home_imgbtn_1_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		go_to_floor(1);
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_2_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		go_to_floor(2);
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_3_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		go_to_floor(3);
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_4_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		go_to_floor(4);
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_5_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		go_to_floor(5);
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_6_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		go_to_floor(6);
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_alarm_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_PRESSED:
	{
		start_alarm_sound();
	}
		break;
	case LV_EVENT_CLICKED:
	{
		stop_alarm_sound();
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_deregister_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		DeregisterUser();
	}
		break;
	default:
		break;
	}
}

static void home_ddlist_languageSel_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj        = lv_event_get_target(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		if (lv_dropdown_get_list(obj))
		{
			// Write style state: LV_STATE_DEFAULT for style_home_ddlist_languagesel_extra_list_selected_default
			static lv_style_t style_home_ddlist_languagesel_extra_list_selected_default;
			if (style_home_ddlist_languagesel_extra_list_selected_default.prop_cnt > 1)
				lv_style_reset(&style_home_ddlist_languagesel_extra_list_selected_default);
			else
				lv_style_init(&style_home_ddlist_languagesel_extra_list_selected_default);
			lv_style_set_bg_color(&style_home_ddlist_languagesel_extra_list_selected_default,
									lv_color_make(0xAF, 0xC3, 0xC3));
			lv_style_set_bg_grad_color(&style_home_ddlist_languagesel_extra_list_selected_default,
									lv_color_make(0xAF, 0xC3, 0xC3));
			lv_obj_add_style(lv_dropdown_get_list(obj), &style_home_ddlist_languagesel_extra_list_selected_default,
							LV_PART_SELECTED | LV_STATE_CHECKED);
		}
	}
		break;
	case LV_EVENT_VALUE_CHANGED:
	{
		gui_home_language_changed_cb();
	}
		break;
	default:
		break;
	}
}

static void home_btn_nxpFaceRec_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_home_debug_info_cb();
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_help_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_set_help();
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_confirm_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		UI_Confirm_Callback();
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_cancel_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		UI_Cancel_Callback();
	}
		break;
	default:
		break;
	}
}

static void home_btn_enable_disable_confirm_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		enable_disable_confirm_cancel_callback();
	}
		break;
	default:
		break;
	}
}

static void home_btn_turn_on_off_mic_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		turn_on_off_mic_callback();
	}
		break;
	default:
		break;
	}
}

void events_init_home(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->home_imgbtn_1, home_imgbtn_1_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_2, home_imgbtn_2_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_3, home_imgbtn_3_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_4, home_imgbtn_4_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_5, home_imgbtn_5_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_6, home_imgbtn_6_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_alarm, home_imgbtn_alarm_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_deregister, home_imgbtn_deregister_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_ddlist_languageSel, home_ddlist_languageSel_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_btn_nxpFaceRec, home_btn_nxpFaceRec_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_help, home_imgbtn_help_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_confirm, home_imgbtn_confirm_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_cancel, home_imgbtn_cancel_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_btn_enable_disable_confirm, home_btn_enable_disable_confirm_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_btn_turn_on_off_mic, home_btn_turn_on_off_mic_event_handler, LV_EVENT_ALL, NULL);
}

static void help_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_set_home();
	}
		break;
	default:
		break;
	}
}

static void help_imgbtn_back_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_set_home();
	}
		break;
	default:
		break;
	}
}

static void help_ddlist_languageSel_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj        = lv_event_get_target(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
            if (lv_dropdown_get_list(obj))
            {
                // Write style state: LV_STATE_DEFAULT for style_home_ddlist_languagesel_extra_list_selected_default
                static lv_style_t style_home_ddlist_languagesel_extra_list_selected_default;
                if (style_home_ddlist_languagesel_extra_list_selected_default.prop_cnt > 1)
                    lv_style_reset(&style_home_ddlist_languagesel_extra_list_selected_default);
                else
                    lv_style_init(&style_home_ddlist_languagesel_extra_list_selected_default);
                lv_style_set_bg_color(&style_home_ddlist_languagesel_extra_list_selected_default,
                                      lv_color_make(0xAF, 0xC3, 0xC3));
                lv_style_set_bg_grad_color(&style_home_ddlist_languagesel_extra_list_selected_default,
                                           lv_color_make(0xAF, 0xC3, 0xC3));
                lv_obj_add_style(lv_dropdown_get_list(obj), &style_home_ddlist_languagesel_extra_list_selected_default,
                                 LV_PART_SELECTED | LV_STATE_CHECKED);
            }
	}
		break;
	case LV_EVENT_VALUE_CHANGED:
	{
		gui_help_language_changed_cb();
	}
		break;
	default:
		break;
	}
}

void events_init_help(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->help_imgbtn_home, help_imgbtn_home_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->help_imgbtn_back, help_imgbtn_back_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->help_ddlist_languageSel, help_ddlist_languageSel_event_handler, LV_EVENT_ALL, NULL);
}
