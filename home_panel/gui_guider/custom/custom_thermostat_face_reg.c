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


void gui_thermostat_lvgl_add_user_info(void)
{
	thermostat_user_list_obj_t user_info;
	uint8_t last_item_no;
	char *user_name;

	user_name = lv_textarea_get_text(guider_ui.screen_thermostat_face_reg_ta_user_name);
	strcpy(&user_info.user_name, user_name);
	strcpy(&user_info.temperature, lv_label_get_text(guider_ui.screen_thermostat_face_reg_label_temperature));
	user_info.mode = gui_thermostat_get_mode();
	user_info.fan_speed = gui_thermostat_get_fan_speed();

	last_item_no = gui_thermostat_get_user_item_num();
	gui_thermostat_lvgl_update_user_item(&user_info, last_item_no);
	last_item_no++;
	gui_thermostat_set_user_item_num(last_item_no);
}



