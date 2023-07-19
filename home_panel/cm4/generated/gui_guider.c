/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"


void init_scr_del_flag(lv_ui *ui){
	ui->screen_home_del = true;
	ui->screen_help_home_del = true;
	ui->screen_audio_player_del = true;
	ui->screen_help_audioplayer_del = true;
	ui->screen_security_del = true;
	ui->screen_help_security_del = true;
	ui->screen_security_pincode_del = true;
	ui->screen_security_arming_del = true;
	ui->screen_security_face_reg_del = true;
	ui->screen_security_user_list_del = true;
	ui->screen_security_user_edit_del = true;
	ui->screen_thermostat_del = true;
	ui->screen_help_thermostat_del = true;
	ui->screen_thermostat_face_reg_del = true;
	ui->screen_thermostat_user_list_del = true;
	ui->screen_thermostat_user_edit_del = true;
}

void setup_ui(lv_ui *ui){
	init_scr_del_flag(ui);
	setup_scr_screen_home(ui);
	lv_scr_load(ui->screen_home);
}
