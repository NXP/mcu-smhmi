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
		gui_set_standby();
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

static void home_imgbtn_size_small_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_set_home_coffee_size(kCoffeeSize_Small);
		
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_size_medium_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_set_home_coffee_size(kCoffeeSize_Medium);
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_size_large_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_set_home_coffee_size(kCoffeeSize_Large);
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_strength_soft_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_set_home_coffee_strength(kCoffeeStrength_Soft);
		
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_strength_mild_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_set_home_coffee_strength(kCoffeeStrength_Medium);
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_strength_strong_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_set_home_coffee_strength(kCoffeeStrength_Strong);
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_start_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_set_brewing();
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_americano_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_set_home_coffee_type(kCoffeeType_Americano);
		
		
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_caffelatte_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_set_home_coffee_type(kCoffeeType_Latte);
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_espresso_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_set_home_coffee_type(kCoffeeType_Espresso);
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_cappuccino_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_set_home_coffee_type(kCoffeeType_Cappuccino);
	}
		break;
	default:
		break;
	}
}

static void home_imgbtn_unregister_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		DeregisterCoffeeSelection();
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
			// Write style state: LV_STATE_DEFAULT for style_ddlist_languagesel_extra_list_selected_default
			static lv_style_t style_ddlist_languagesel_extra_list_selected_default;
			if (style_ddlist_languagesel_extra_list_selected_default.prop_cnt > 1)
				lv_style_reset(&style_ddlist_languagesel_extra_list_selected_default);
			else
				lv_style_init(&style_ddlist_languagesel_extra_list_selected_default);

			lv_style_set_bg_color(&style_ddlist_languagesel_extra_list_selected_default, lv_color_make(0x50, 0x32, 0x00));
			lv_style_set_bg_grad_color(&style_ddlist_languagesel_extra_list_selected_default, lv_color_make(0x50, 0x32, 0x00));
			lv_obj_add_style(lv_dropdown_get_list(obj), &style_ddlist_languagesel_extra_list_selected_default, LV_PART_SELECTED | LV_STATE_CHECKED);
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

void events_init_home(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->home_imgbtn_size_small, home_imgbtn_size_small_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_size_medium, home_imgbtn_size_medium_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_size_large, home_imgbtn_size_large_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_strength_soft, home_imgbtn_strength_soft_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_strength_mild, home_imgbtn_strength_mild_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_strength_strong, home_imgbtn_strength_strong_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_start, home_imgbtn_start_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_americano, home_imgbtn_americano_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_caffelatte, home_imgbtn_caffelatte_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_espresso, home_imgbtn_espresso_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_cappuccino, home_imgbtn_cappuccino_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_imgbtn_unregister, home_imgbtn_unregister_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_ddlist_languageSel, home_ddlist_languageSel_event_handler, LV_EVENT_ALL, NULL);

	lv_obj_add_event_cb(ui->home_label_americano, home_imgbtn_americano_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_label_cafelatte, home_imgbtn_caffelatte_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_label_espresso, home_imgbtn_espresso_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_label_cappuccino, home_imgbtn_cappuccino_event_handler, LV_EVENT_ALL, NULL);

	lv_obj_add_event_cb(ui->home_label_size_small, home_imgbtn_size_small_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_label_size_medium, home_imgbtn_size_medium_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_label_size_large, home_imgbtn_size_large_event_handler, LV_EVENT_ALL, NULL);

	lv_obj_add_event_cb(ui->home_label_strength_soft, home_imgbtn_strength_soft_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_label_strength_mild, home_imgbtn_strength_mild_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->home_label_strength_strong, home_imgbtn_strength_strong_event_handler, LV_EVENT_ALL, NULL);
}

static void standby_btn_header_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		WakeUp(2);
	}
		break;
	default:
		break;
	}
}

static void standby_ddlist_languageSel_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj        = lv_event_get_target(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		if (lv_dropdown_get_list(obj))
		{
		// Write style state: LV_STATE_DEFAULT for style_ddlist_languagesel_extra_list_selected_default
		static lv_style_t style_ddlist_languagesel_extra_list_selected_default;
		if (style_ddlist_languagesel_extra_list_selected_default.prop_cnt > 1)
			lv_style_reset(&style_ddlist_languagesel_extra_list_selected_default);
		else
			lv_style_init(&style_ddlist_languagesel_extra_list_selected_default);

		lv_style_set_bg_color(&style_ddlist_languagesel_extra_list_selected_default, lv_color_make(0x50, 0x32, 0x00));
		lv_style_set_bg_grad_color(&style_ddlist_languagesel_extra_list_selected_default, lv_color_make(0x50, 0x32, 0x00));
		lv_obj_add_style(lv_dropdown_get_list(obj), &style_ddlist_languagesel_extra_list_selected_default, LV_PART_SELECTED | LV_STATE_CHECKED);
		}
	}
		break;
	case LV_EVENT_VALUE_CHANGED:
        {
                gui_standby_language_changed_cb();
        }
                break;
	default:
		break;
	}
}

static void standby_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		WakeUp(2);
	}
		break;
	default:
		break;
	}
}

void events_init_standby(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->standby_btn_header_home, standby_btn_header_home_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->standby_ddlist_languageSel, standby_ddlist_languageSel_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->standby_imgbtn_home, standby_imgbtn_home_event_handler, LV_EVENT_ALL, NULL);
}
