/*
 * Copyright 2022 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl/lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"


void setup_scr_brewing(lv_ui *ui){

	//Write codes brewing
	ui->brewing = lv_obj_create(NULL);

	//Write style state: LV_STATE_DEFAULT for style_brewing_main_main_default
	static lv_style_t style_brewing_main_main_default;
	if (style_brewing_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_brewing_main_main_default);
	else
		lv_style_init(&style_brewing_main_main_default);
	lv_style_set_bg_color(&style_brewing_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_brewing_main_main_default, 0);
	lv_obj_add_style(ui->brewing, &style_brewing_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes brewing_cont_background
	ui->brewing_cont_background = lv_obj_create(ui->brewing);
	lv_obj_set_pos(ui->brewing_cont_background, 0, 0);
	lv_obj_set_size(ui->brewing_cont_background, 1280, 720);

	//Write style state: LV_STATE_DEFAULT for style_brewing_cont_background_main_main_default
	static lv_style_t style_brewing_cont_background_main_main_default;
	if (style_brewing_cont_background_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_brewing_cont_background_main_main_default);
	else
		lv_style_init(&style_brewing_cont_background_main_main_default);
	lv_style_set_radius(&style_brewing_cont_background_main_main_default, 0);
	lv_style_set_bg_color(&style_brewing_cont_background_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_brewing_cont_background_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_brewing_cont_background_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_brewing_cont_background_main_main_default, 255);
	lv_style_set_border_color(&style_brewing_cont_background_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_brewing_cont_background_main_main_default, 0);
	lv_style_set_border_opa(&style_brewing_cont_background_main_main_default, 255);
	lv_style_set_pad_left(&style_brewing_cont_background_main_main_default, 0);
	lv_style_set_pad_right(&style_brewing_cont_background_main_main_default, 0);
	lv_style_set_pad_top(&style_brewing_cont_background_main_main_default, 0);
	lv_style_set_pad_bottom(&style_brewing_cont_background_main_main_default, 0);
	lv_obj_add_style(ui->brewing_cont_background, &style_brewing_cont_background_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes brewing_label_coffee_type
	ui->brewing_label_coffee_type = lv_label_create(ui->brewing);
	lv_obj_set_pos(ui->brewing_label_coffee_type, 206, 252);
	lv_obj_set_size(ui->brewing_label_coffee_type, 310, 41);
	lv_label_set_text(ui->brewing_label_coffee_type, "Americano");
	lv_label_set_long_mode(ui->brewing_label_coffee_type, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->brewing_label_coffee_type, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_brewing_label_coffee_type_main_main_default
	static lv_style_t style_brewing_label_coffee_type_main_main_default;
	if (style_brewing_label_coffee_type_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_brewing_label_coffee_type_main_main_default);
	else
		lv_style_init(&style_brewing_label_coffee_type_main_main_default);
	lv_style_set_radius(&style_brewing_label_coffee_type_main_main_default, 0);
	lv_style_set_bg_color(&style_brewing_label_coffee_type_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_brewing_label_coffee_type_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_brewing_label_coffee_type_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_brewing_label_coffee_type_main_main_default, 0);
	lv_style_set_text_color(&style_brewing_label_coffee_type_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_brewing_label_coffee_type_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_brewing_label_coffee_type_main_main_default, 2);
	lv_style_set_pad_left(&style_brewing_label_coffee_type_main_main_default, 0);
	lv_style_set_pad_right(&style_brewing_label_coffee_type_main_main_default, 0);
	lv_style_set_pad_top(&style_brewing_label_coffee_type_main_main_default, 0);
	lv_style_set_pad_bottom(&style_brewing_label_coffee_type_main_main_default, 0);
	lv_obj_add_style(ui->brewing_label_coffee_type, &style_brewing_label_coffee_type_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes brewing_label_coffee_size
	ui->brewing_label_coffee_size = lv_label_create(ui->brewing);
	lv_obj_set_pos(ui->brewing_label_coffee_size, 231, 342);
	lv_obj_set_size(ui->brewing_label_coffee_size, 260, 41);
	lv_label_set_text(ui->brewing_label_coffee_size, "Small");
	lv_label_set_long_mode(ui->brewing_label_coffee_size, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->brewing_label_coffee_size, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_brewing_label_coffee_size_main_main_default
	static lv_style_t style_brewing_label_coffee_size_main_main_default;
	if (style_brewing_label_coffee_size_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_brewing_label_coffee_size_main_main_default);
	else
		lv_style_init(&style_brewing_label_coffee_size_main_main_default);
	lv_style_set_radius(&style_brewing_label_coffee_size_main_main_default, 0);
	lv_style_set_bg_color(&style_brewing_label_coffee_size_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_brewing_label_coffee_size_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_brewing_label_coffee_size_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_brewing_label_coffee_size_main_main_default, 0);
	lv_style_set_text_color(&style_brewing_label_coffee_size_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_brewing_label_coffee_size_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_brewing_label_coffee_size_main_main_default, 2);
	lv_style_set_pad_left(&style_brewing_label_coffee_size_main_main_default, 0);
	lv_style_set_pad_right(&style_brewing_label_coffee_size_main_main_default, 0);
	lv_style_set_pad_top(&style_brewing_label_coffee_size_main_main_default, 0);
	lv_style_set_pad_bottom(&style_brewing_label_coffee_size_main_main_default, 0);
	lv_obj_add_style(ui->brewing_label_coffee_size, &style_brewing_label_coffee_size_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes brewing_label_coffee_strength
	ui->brewing_label_coffee_strength = lv_label_create(ui->brewing);
	lv_obj_set_pos(ui->brewing_label_coffee_strength, 231, 432);
	lv_obj_set_size(ui->brewing_label_coffee_strength, 260, 41);
	lv_label_set_text(ui->brewing_label_coffee_strength, "Soft");
	lv_label_set_long_mode(ui->brewing_label_coffee_strength, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->brewing_label_coffee_strength, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_brewing_label_coffee_strength_main_main_default
	static lv_style_t style_brewing_label_coffee_strength_main_main_default;
	if (style_brewing_label_coffee_strength_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_brewing_label_coffee_strength_main_main_default);
	else
		lv_style_init(&style_brewing_label_coffee_strength_main_main_default);
	lv_style_set_radius(&style_brewing_label_coffee_strength_main_main_default, 0);
	lv_style_set_bg_color(&style_brewing_label_coffee_strength_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_brewing_label_coffee_strength_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_brewing_label_coffee_strength_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_brewing_label_coffee_strength_main_main_default, 0);
	lv_style_set_text_color(&style_brewing_label_coffee_strength_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_brewing_label_coffee_strength_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_brewing_label_coffee_strength_main_main_default, 2);
	lv_style_set_pad_left(&style_brewing_label_coffee_strength_main_main_default, 0);
	lv_style_set_pad_right(&style_brewing_label_coffee_strength_main_main_default, 0);
	lv_style_set_pad_top(&style_brewing_label_coffee_strength_main_main_default, 0);
	lv_style_set_pad_bottom(&style_brewing_label_coffee_strength_main_main_default, 0);
	lv_obj_add_style(ui->brewing_label_coffee_strength, &style_brewing_label_coffee_strength_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes brewing_cont_brewing
	ui->brewing_cont_brewing = lv_obj_create(ui->brewing);
	lv_obj_set_pos(ui->brewing_cont_brewing, 585, 160);
	lv_obj_set_size(ui->brewing_cont_brewing, 400, 335);

	//Write style state: LV_STATE_DEFAULT for style_brewing_cont_brewing_main_main_default
	static lv_style_t style_brewing_cont_brewing_main_main_default;
	if (style_brewing_cont_brewing_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_brewing_cont_brewing_main_main_default);
	else
		lv_style_init(&style_brewing_cont_brewing_main_main_default);
	lv_style_set_radius(&style_brewing_cont_brewing_main_main_default, 0);
	lv_style_set_bg_color(&style_brewing_cont_brewing_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_brewing_cont_brewing_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_brewing_cont_brewing_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_brewing_cont_brewing_main_main_default, 0);
	lv_style_set_border_color(&style_brewing_cont_brewing_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_brewing_cont_brewing_main_main_default, 0);
	lv_style_set_border_opa(&style_brewing_cont_brewing_main_main_default, 255);
	lv_style_set_pad_left(&style_brewing_cont_brewing_main_main_default, 0);
	lv_style_set_pad_right(&style_brewing_cont_brewing_main_main_default, 0);
	lv_style_set_pad_top(&style_brewing_cont_brewing_main_main_default, 0);
	lv_style_set_pad_bottom(&style_brewing_cont_brewing_main_main_default, 0);
	lv_obj_add_style(ui->brewing_cont_brewing, &style_brewing_cont_brewing_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes brewing_bar_fillStatus
	ui->brewing_bar_fillStatus = lv_bar_create(ui->brewing_cont_brewing);
	lv_obj_set_pos(ui->brewing_bar_fillStatus, 54, 310);
	lv_obj_set_size(ui->brewing_bar_fillStatus, 300, 20);

	//Write style state: LV_STATE_DEFAULT for style_brewing_bar_fillstatus_main_main_default
	static lv_style_t style_brewing_bar_fillstatus_main_main_default;
	if (style_brewing_bar_fillstatus_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_brewing_bar_fillstatus_main_main_default);
	else
		lv_style_init(&style_brewing_bar_fillstatus_main_main_default);
	lv_style_set_radius(&style_brewing_bar_fillstatus_main_main_default, 10);
	lv_style_set_bg_color(&style_brewing_bar_fillstatus_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_brewing_bar_fillstatus_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_brewing_bar_fillstatus_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_brewing_bar_fillstatus_main_main_default, 60);
	lv_style_set_pad_left(&style_brewing_bar_fillstatus_main_main_default, 0);
	lv_style_set_pad_right(&style_brewing_bar_fillstatus_main_main_default, 0);
	lv_style_set_pad_top(&style_brewing_bar_fillstatus_main_main_default, 0);
	lv_style_set_pad_bottom(&style_brewing_bar_fillstatus_main_main_default, 0);
	lv_obj_add_style(ui->brewing_bar_fillStatus, &style_brewing_bar_fillstatus_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_brewing_bar_fillstatus_main_indicator_default
	static lv_style_t style_brewing_bar_fillstatus_main_indicator_default;
	if (style_brewing_bar_fillstatus_main_indicator_default.prop_cnt > 1)
		lv_style_reset(&style_brewing_bar_fillstatus_main_indicator_default);
	else
		lv_style_init(&style_brewing_bar_fillstatus_main_indicator_default);
	lv_style_set_radius(&style_brewing_bar_fillstatus_main_indicator_default, 10);
	lv_style_set_bg_color(&style_brewing_bar_fillstatus_main_indicator_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_brewing_bar_fillstatus_main_indicator_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_brewing_bar_fillstatus_main_indicator_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_brewing_bar_fillstatus_main_indicator_default, 255);
	lv_obj_add_style(ui->brewing_bar_fillStatus, &style_brewing_bar_fillstatus_main_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_anim_time(ui->brewing_bar_fillStatus, 1000, 0);
	lv_bar_set_mode(ui->brewing_bar_fillStatus, LV_BAR_MODE_NORMAL);
	lv_bar_set_value(ui->brewing_bar_fillStatus, 50, LV_ANIM_OFF);

	//Write codes brewing_animimg_brewing
	ui->brewing_animimg_brewing = lv_animimg_create(ui->brewing_cont_brewing);
	lv_obj_set_pos(ui->brewing_animimg_brewing, 101, 50);
	lv_obj_set_size(ui->brewing_animimg_brewing, 200, 200);
}