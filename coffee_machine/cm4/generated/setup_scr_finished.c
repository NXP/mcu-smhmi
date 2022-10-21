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


void setup_scr_finished(lv_ui *ui){

	//Write codes finished
	ui->finished = lv_obj_create(NULL);

	//Write style state: LV_STATE_DEFAULT for style_finished_main_main_default
	static lv_style_t style_finished_main_main_default;
	if (style_finished_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_finished_main_main_default);
	else
		lv_style_init(&style_finished_main_main_default);
	lv_style_set_bg_color(&style_finished_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_finished_main_main_default, 0);
	lv_obj_add_style(ui->finished, &style_finished_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes finished_cont_background
	ui->finished_cont_background = lv_obj_create(ui->finished);
	lv_obj_set_pos(ui->finished_cont_background, 0, 0);
	lv_obj_set_size(ui->finished_cont_background, 1280, 720);

	//Write style state: LV_STATE_DEFAULT for style_finished_cont_background_main_main_default
	static lv_style_t style_finished_cont_background_main_main_default;
	if (style_finished_cont_background_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_finished_cont_background_main_main_default);
	else
		lv_style_init(&style_finished_cont_background_main_main_default);
	lv_style_set_radius(&style_finished_cont_background_main_main_default, 0);
	lv_style_set_bg_color(&style_finished_cont_background_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_finished_cont_background_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_finished_cont_background_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_finished_cont_background_main_main_default, 255);
	lv_style_set_border_color(&style_finished_cont_background_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_finished_cont_background_main_main_default, 0);
	lv_style_set_border_opa(&style_finished_cont_background_main_main_default, 0);
	lv_style_set_pad_left(&style_finished_cont_background_main_main_default, 0);
	lv_style_set_pad_right(&style_finished_cont_background_main_main_default, 0);
	lv_style_set_pad_top(&style_finished_cont_background_main_main_default, 0);
	lv_style_set_pad_bottom(&style_finished_cont_background_main_main_default, 0);
	lv_obj_add_style(ui->finished_cont_background, &style_finished_cont_background_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes finished_animimg_coffee_type
	ui->finished_animimg_coffee_type = lv_animimg_create(ui->finished_cont_background);
	lv_obj_set_pos(ui->finished_animimg_coffee_type, 611, 225);
	lv_obj_set_size(ui->finished_animimg_coffee_type, 250, 250);

	//Write codes finished_label_coffee_type
	ui->finished_label_coffee_type = lv_label_create(ui->finished_cont_background);
	lv_obj_set_pos(ui->finished_label_coffee_type, 206, 252);
	lv_obj_set_size(ui->finished_label_coffee_type, 310, 41);
	lv_label_set_text(ui->finished_label_coffee_type, "Americano");
	lv_label_set_long_mode(ui->finished_label_coffee_type, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->finished_label_coffee_type, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_finished_label_coffee_type_main_main_default
	static lv_style_t style_finished_label_coffee_type_main_main_default;
	if (style_finished_label_coffee_type_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_finished_label_coffee_type_main_main_default);
	else
		lv_style_init(&style_finished_label_coffee_type_main_main_default);
	lv_style_set_radius(&style_finished_label_coffee_type_main_main_default, 0);
	lv_style_set_bg_color(&style_finished_label_coffee_type_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_finished_label_coffee_type_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_finished_label_coffee_type_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_finished_label_coffee_type_main_main_default, 0);
	lv_style_set_text_color(&style_finished_label_coffee_type_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_finished_label_coffee_type_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_finished_label_coffee_type_main_main_default, 2);
	lv_style_set_pad_left(&style_finished_label_coffee_type_main_main_default, 0);
	lv_style_set_pad_right(&style_finished_label_coffee_type_main_main_default, 0);
	lv_style_set_pad_top(&style_finished_label_coffee_type_main_main_default, 0);
	lv_style_set_pad_bottom(&style_finished_label_coffee_type_main_main_default, 0);
	lv_obj_add_style(ui->finished_label_coffee_type, &style_finished_label_coffee_type_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes finished_label_coffee_size
	ui->finished_label_coffee_size = lv_label_create(ui->finished_cont_background);
	lv_obj_set_pos(ui->finished_label_coffee_size, 231, 342);
	lv_obj_set_size(ui->finished_label_coffee_size, 260, 41);
	lv_label_set_text(ui->finished_label_coffee_size, "Small");
	lv_label_set_long_mode(ui->finished_label_coffee_size, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->finished_label_coffee_size, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_finished_label_coffee_size_main_main_default
	static lv_style_t style_finished_label_coffee_size_main_main_default;
	if (style_finished_label_coffee_size_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_finished_label_coffee_size_main_main_default);
	else
		lv_style_init(&style_finished_label_coffee_size_main_main_default);
	lv_style_set_radius(&style_finished_label_coffee_size_main_main_default, 0);
	lv_style_set_bg_color(&style_finished_label_coffee_size_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_finished_label_coffee_size_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_finished_label_coffee_size_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_finished_label_coffee_size_main_main_default, 0);
	lv_style_set_text_color(&style_finished_label_coffee_size_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_finished_label_coffee_size_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_finished_label_coffee_size_main_main_default, 2);
	lv_style_set_pad_left(&style_finished_label_coffee_size_main_main_default, 0);
	lv_style_set_pad_right(&style_finished_label_coffee_size_main_main_default, 0);
	lv_style_set_pad_top(&style_finished_label_coffee_size_main_main_default, 0);
	lv_style_set_pad_bottom(&style_finished_label_coffee_size_main_main_default, 0);
	lv_obj_add_style(ui->finished_label_coffee_size, &style_finished_label_coffee_size_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes finished_label_coffee_strength
	ui->finished_label_coffee_strength = lv_label_create(ui->finished_cont_background);
	lv_obj_set_pos(ui->finished_label_coffee_strength, 231, 432);
	lv_obj_set_size(ui->finished_label_coffee_strength, 260, 41);
	lv_label_set_text(ui->finished_label_coffee_strength, "Soft");
	lv_label_set_long_mode(ui->finished_label_coffee_strength, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->finished_label_coffee_strength, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_finished_label_coffee_strength_main_main_default
	static lv_style_t style_finished_label_coffee_strength_main_main_default;
	if (style_finished_label_coffee_strength_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_finished_label_coffee_strength_main_main_default);
	else
		lv_style_init(&style_finished_label_coffee_strength_main_main_default);
	lv_style_set_radius(&style_finished_label_coffee_strength_main_main_default, 0);
	lv_style_set_bg_color(&style_finished_label_coffee_strength_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_finished_label_coffee_strength_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_finished_label_coffee_strength_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_finished_label_coffee_strength_main_main_default, 0);
	lv_style_set_text_color(&style_finished_label_coffee_strength_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_finished_label_coffee_strength_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_finished_label_coffee_strength_main_main_default, 2);
	lv_style_set_pad_left(&style_finished_label_coffee_strength_main_main_default, 0);
	lv_style_set_pad_right(&style_finished_label_coffee_strength_main_main_default, 0);
	lv_style_set_pad_top(&style_finished_label_coffee_strength_main_main_default, 0);
	lv_style_set_pad_bottom(&style_finished_label_coffee_strength_main_main_default, 0);
	lv_obj_add_style(ui->finished_label_coffee_strength, &style_finished_label_coffee_strength_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
}