/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"


void setup_scr_screen_thermostat(lv_ui *ui){

	//Write codes screen_thermostat
	ui->screen_thermostat = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_main_main_default
	static lv_style_t style_screen_thermostat_main_main_default;
	if (style_screen_thermostat_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_main_main_default);
	else
		lv_style_init(&style_screen_thermostat_main_main_default);
	lv_style_set_bg_color(&style_screen_thermostat_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_screen_thermostat_main_main_default, 0);
	lv_obj_add_style(ui->screen_thermostat, &style_screen_thermostat_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_thermostat_cont_thermBackground
	ui->screen_thermostat_cont_thermBackground = lv_obj_create(ui->screen_thermostat);
	lv_obj_set_pos(ui->screen_thermostat_cont_thermBackground, 0, 0);
	lv_obj_set_size(ui->screen_thermostat_cont_thermBackground, 1280, 720);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_cont_thermBackground, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_cont_thermbackground_main_main_default
	static lv_style_t style_screen_thermostat_cont_thermbackground_main_main_default;
	if (style_screen_thermostat_cont_thermbackground_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_cont_thermbackground_main_main_default);
	else
		lv_style_init(&style_screen_thermostat_cont_thermbackground_main_main_default);
	lv_style_set_radius(&style_screen_thermostat_cont_thermbackground_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_thermostat_cont_thermbackground_main_main_default, lv_color_make(0xfc, 0xfc, 0xfc));
	lv_style_set_bg_grad_color(&style_screen_thermostat_cont_thermbackground_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_thermostat_cont_thermbackground_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_thermostat_cont_thermbackground_main_main_default, 255);
	lv_style_set_shadow_width(&style_screen_thermostat_cont_thermbackground_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_thermostat_cont_thermbackground_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_thermostat_cont_thermbackground_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_thermostat_cont_thermbackground_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_thermostat_cont_thermbackground_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_thermostat_cont_thermbackground_main_main_default, 0);
	lv_style_set_border_color(&style_screen_thermostat_cont_thermbackground_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_screen_thermostat_cont_thermbackground_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_thermostat_cont_thermbackground_main_main_default, 255);
	lv_style_set_pad_left(&style_screen_thermostat_cont_thermbackground_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_thermostat_cont_thermbackground_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_thermostat_cont_thermbackground_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_thermostat_cont_thermbackground_main_main_default, 0);
	lv_obj_add_style(ui->screen_thermostat_cont_thermBackground, &style_screen_thermostat_cont_thermbackground_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_thermostat_img_background
	ui->screen_thermostat_img_background = lv_img_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_img_background, 0, 0);
	lv_obj_set_size(ui->screen_thermostat_img_background, 1280, 630);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_img_background, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_thermostat_img_background, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_thermostat_img_background,&_main_background_1280x630);
	lv_img_set_pivot(ui->screen_thermostat_img_background, 0,0);
	lv_img_set_angle(ui->screen_thermostat_img_background, 0);

	//Write codes screen_thermostat_img_background_bottom
	ui->screen_thermostat_img_background_bottom = lv_img_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_img_background_bottom, 0, 630);
	lv_obj_set_size(ui->screen_thermostat_img_background_bottom, 1280, 90);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_img_background_bottom, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_img_background_bottom_main_main_default
	static lv_style_t style_screen_thermostat_img_background_bottom_main_main_default;
	if (style_screen_thermostat_img_background_bottom_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_img_background_bottom_main_main_default);
	else
		lv_style_init(&style_screen_thermostat_img_background_bottom_main_main_default);
	lv_style_set_img_recolor(&style_screen_thermostat_img_background_bottom_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_thermostat_img_background_bottom_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_thermostat_img_background_bottom_main_main_default, 255);
	lv_obj_add_style(ui->screen_thermostat_img_background_bottom, &style_screen_thermostat_img_background_bottom_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_thermostat_img_background_bottom, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_thermostat_img_background_bottom,&_thermostat_background_bottom_1280x90);
	lv_img_set_pivot(ui->screen_thermostat_img_background_bottom, 50,50);
	lv_img_set_angle(ui->screen_thermostat_img_background_bottom, 0);

	//Write codes screen_thermostat_img_frame
	ui->screen_thermostat_img_frame = lv_img_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_img_frame, 484, 234);
	lv_obj_set_size(ui->screen_thermostat_img_frame, 312, 310);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_img_frame, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_img_frame_main_main_default
	static lv_style_t style_screen_thermostat_img_frame_main_main_default;
	if (style_screen_thermostat_img_frame_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_img_frame_main_main_default);
	else
		lv_style_init(&style_screen_thermostat_img_frame_main_main_default);
	lv_style_set_img_recolor(&style_screen_thermostat_img_frame_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_thermostat_img_frame_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_thermostat_img_frame_main_main_default, 255);
	lv_obj_add_style(ui->screen_thermostat_img_frame, &style_screen_thermostat_img_frame_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_thermostat_img_frame, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_thermostat_img_frame,&_thermostat_frame_312x310);
	lv_img_set_pivot(ui->screen_thermostat_img_frame, 50,50);
	lv_img_set_angle(ui->screen_thermostat_img_frame, 0);

	//Write codes screen_thermostat_label_title
	ui->screen_thermostat_label_title = lv_label_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_label_title, 355, 34);
	lv_obj_set_size(ui->screen_thermostat_label_title, 485, 50);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_label_title, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_thermostat_label_title, "THERMOSTAT");
	lv_label_set_long_mode(ui->screen_thermostat_label_title, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_label_title_main_main_default
	static lv_style_t style_screen_thermostat_label_title_main_main_default;
	if (style_screen_thermostat_label_title_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_label_title_main_main_default);
	else
		lv_style_init(&style_screen_thermostat_label_title_main_main_default);
	lv_style_set_radius(&style_screen_thermostat_label_title_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_thermostat_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_thermostat_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_thermostat_label_title_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_thermostat_label_title_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_thermostat_label_title_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_thermostat_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_thermostat_label_title_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_thermostat_label_title_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_thermostat_label_title_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_thermostat_label_title_main_main_default, 0);
	lv_style_set_text_color(&style_screen_thermostat_label_title_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_thermostat_label_title_main_main_default, &lv_font_montserratMedium_50);
	lv_style_set_text_letter_space(&style_screen_thermostat_label_title_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_thermostat_label_title_main_main_default, 0);
	lv_style_set_text_align(&style_screen_thermostat_label_title_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_thermostat_label_title_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_thermostat_label_title_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_thermostat_label_title_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_thermostat_label_title_main_main_default, 0);
	lv_obj_add_style(ui->screen_thermostat_label_title, &style_screen_thermostat_label_title_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_thermostat_ddlist_language
	ui->screen_thermostat_ddlist_language = lv_dropdown_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_ddlist_language, 1024, 31);
	lv_obj_set_size(ui->screen_thermostat_ddlist_language, 193, 50);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_ddlist_language, LV_SCROLLBAR_MODE_OFF);
	lv_dropdown_set_options(ui->screen_thermostat_ddlist_language, "English\n中文");

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_ddlist_language_main_main_default
	static lv_style_t style_screen_thermostat_ddlist_language_main_main_default;
	if (style_screen_thermostat_ddlist_language_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_ddlist_language_main_main_default);
	else
		lv_style_init(&style_screen_thermostat_ddlist_language_main_main_default);
	lv_style_set_radius(&style_screen_thermostat_ddlist_language_main_main_default, 3);
	lv_style_set_bg_color(&style_screen_thermostat_ddlist_language_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_thermostat_ddlist_language_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_thermostat_ddlist_language_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_thermostat_ddlist_language_main_main_default, 255);
	lv_style_set_shadow_width(&style_screen_thermostat_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_thermostat_ddlist_language_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_thermostat_ddlist_language_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_thermostat_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_thermostat_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_thermostat_ddlist_language_main_main_default, 0);
	lv_style_set_border_color(&style_screen_thermostat_ddlist_language_main_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_thermostat_ddlist_language_main_main_default, 1);
	lv_style_set_border_opa(&style_screen_thermostat_ddlist_language_main_main_default, 255);
	lv_style_set_text_color(&style_screen_thermostat_ddlist_language_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_thermostat_ddlist_language_main_main_default, &lv_font_simsun_32);
	lv_style_set_pad_left(&style_screen_thermostat_ddlist_language_main_main_default, 6);
	lv_style_set_pad_right(&style_screen_thermostat_ddlist_language_main_main_default, 6);
	lv_style_set_pad_top(&style_screen_thermostat_ddlist_language_main_main_default, 8);
	lv_obj_add_style(ui->screen_thermostat_ddlist_language, &style_screen_thermostat_ddlist_language_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_CHECKED for style_screen_thermostat_ddlist_language_extra_list_selected_checked
	static lv_style_t style_screen_thermostat_ddlist_language_extra_list_selected_checked;
	if (style_screen_thermostat_ddlist_language_extra_list_selected_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_ddlist_language_extra_list_selected_checked);
	else
		lv_style_init(&style_screen_thermostat_ddlist_language_extra_list_selected_checked);
	lv_style_set_radius(&style_screen_thermostat_ddlist_language_extra_list_selected_checked, 3);
	lv_style_set_bg_color(&style_screen_thermostat_ddlist_language_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_color(&style_screen_thermostat_ddlist_language_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_screen_thermostat_ddlist_language_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_thermostat_ddlist_language_extra_list_selected_checked, 255);
	lv_style_set_border_color(&style_screen_thermostat_ddlist_language_extra_list_selected_checked, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_thermostat_ddlist_language_extra_list_selected_checked, 1);
	lv_style_set_border_opa(&style_screen_thermostat_ddlist_language_extra_list_selected_checked, 255);
	lv_style_set_text_color(&style_screen_thermostat_ddlist_language_extra_list_selected_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_thermostat_ddlist_language_extra_list_selected_checked, &lv_font_simsun_32);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_thermostat_ddlist_language), &style_screen_thermostat_ddlist_language_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_ddlist_language_extra_list_main_default
	static lv_style_t style_screen_thermostat_ddlist_language_extra_list_main_default;
	if (style_screen_thermostat_ddlist_language_extra_list_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_ddlist_language_extra_list_main_default);
	else
		lv_style_init(&style_screen_thermostat_ddlist_language_extra_list_main_default);
	lv_style_set_radius(&style_screen_thermostat_ddlist_language_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_screen_thermostat_ddlist_language_extra_list_main_default, lv_color_make(0x8c, 0xa0, 0xa0));
	lv_style_set_bg_grad_color(&style_screen_thermostat_ddlist_language_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_thermostat_ddlist_language_extra_list_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_thermostat_ddlist_language_extra_list_main_default, 255);
	lv_style_set_border_color(&style_screen_thermostat_ddlist_language_extra_list_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_thermostat_ddlist_language_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_screen_thermostat_ddlist_language_extra_list_main_default, 255);
	lv_style_set_text_color(&style_screen_thermostat_ddlist_language_extra_list_main_default, lv_color_make(0x0D, 0x30, 0x55));
	lv_style_set_text_font(&style_screen_thermostat_ddlist_language_extra_list_main_default, &lv_font_simsun_32);
	lv_style_set_max_height(&style_screen_thermostat_ddlist_language_extra_list_main_default, 150);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_thermostat_ddlist_language), &style_screen_thermostat_ddlist_language_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_ddlist_language_extra_list_scrollbar_default
	static lv_style_t style_screen_thermostat_ddlist_language_extra_list_scrollbar_default;
	if (style_screen_thermostat_ddlist_language_extra_list_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_ddlist_language_extra_list_scrollbar_default);
	else
		lv_style_init(&style_screen_thermostat_ddlist_language_extra_list_scrollbar_default);
	lv_style_set_radius(&style_screen_thermostat_ddlist_language_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_screen_thermostat_ddlist_language_extra_list_scrollbar_default, lv_color_make(0xaf, 0xc3, 0xc3));
	lv_style_set_bg_opa(&style_screen_thermostat_ddlist_language_extra_list_scrollbar_default, 255);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_thermostat_ddlist_language), &style_screen_thermostat_ddlist_language_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes screen_thermostat_imgbtn_on_off
	ui->screen_thermostat_imgbtn_on_off = lv_imgbtn_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_imgbtn_on_off, 27, 114);
	lv_obj_set_size(ui->screen_thermostat_imgbtn_on_off, 128, 128);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_imgbtn_on_off, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_on_off, LV_IMGBTN_STATE_RELEASED, NULL, &_thermostat_on_128x128, NULL);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_on_off, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_thermostat_off_128x128, NULL);

	//Write codes screen_thermostat_imgbtn_mode_heat
	ui->screen_thermostat_imgbtn_mode_heat = lv_imgbtn_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_imgbtn_mode_heat, 38, 274);
	lv_obj_set_size(ui->screen_thermostat_imgbtn_mode_heat, 102, 92);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_imgbtn_mode_heat, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_mode_heat, LV_IMGBTN_STATE_RELEASED, NULL, &_thermostat_mode_heat_off_102x92, NULL);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_mode_heat, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_thermostat_mode_heat_on_102x92, NULL);

	//Write codes screen_thermostat_imgbtn_mode_auto
	ui->screen_thermostat_imgbtn_mode_auto = lv_imgbtn_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_imgbtn_mode_auto, 38, 365);
	lv_obj_set_size(ui->screen_thermostat_imgbtn_mode_auto, 102, 84);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_imgbtn_mode_auto, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_mode_auto, LV_IMGBTN_STATE_RELEASED, NULL, &_thermostat_mode_auto_off_102x84, NULL);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_mode_auto, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_thermostat_mode_auto_on_102x84, NULL);

	//Write codes screen_thermostat_imgbtn_mode_cool
	ui->screen_thermostat_imgbtn_mode_cool = lv_imgbtn_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_imgbtn_mode_cool, 38, 448);
	lv_obj_set_size(ui->screen_thermostat_imgbtn_mode_cool, 102, 92);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_imgbtn_mode_cool, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_mode_cool, LV_IMGBTN_STATE_RELEASED, NULL, &_thermostat_mode_cool_off_102x92, NULL);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_mode_cool, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_thermostat_mode_cool_on_102x92, NULL);

	//Write codes screen_thermostat_imgbtn_user_setting
	ui->screen_thermostat_imgbtn_user_setting = lv_imgbtn_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_imgbtn_user_setting, 26, 574);
	lv_obj_set_size(ui->screen_thermostat_imgbtn_user_setting, 128, 128);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_imgbtn_user_setting, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_user_setting, LV_IMGBTN_STATE_RELEASED, NULL, &_user_setting_128x128, NULL);

	//Write codes screen_thermostat_imgbtn_help
	ui->screen_thermostat_imgbtn_help = lv_imgbtn_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_imgbtn_help, 1140, 114);
	lv_obj_set_size(ui->screen_thermostat_imgbtn_help, 100, 100);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_imgbtn_help, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_help, LV_IMGBTN_STATE_RELEASED, NULL, &_help_100x100, NULL);

	//Write codes screen_thermostat_arc_temp_knob
	ui->screen_thermostat_arc_temp_knob = lv_arc_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_arc_temp_knob, 428, 176);
	lv_obj_set_size(ui->screen_thermostat_arc_temp_knob, 428, 428);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_arc_temp_knob, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_arc_temp_knob_main_main_default
	static lv_style_t style_screen_thermostat_arc_temp_knob_main_main_default;
	if (style_screen_thermostat_arc_temp_knob_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_arc_temp_knob_main_main_default);
	else
		lv_style_init(&style_screen_thermostat_arc_temp_knob_main_main_default);
	lv_style_set_radius(&style_screen_thermostat_arc_temp_knob_main_main_default, 6);
	lv_style_set_bg_color(&style_screen_thermostat_arc_temp_knob_main_main_default, lv_color_make(0xf6, 0xf6, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_thermostat_arc_temp_knob_main_main_default, lv_color_make(0xf6, 0xf6, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_thermostat_arc_temp_knob_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_thermostat_arc_temp_knob_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_thermostat_arc_temp_knob_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_thermostat_arc_temp_knob_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_thermostat_arc_temp_knob_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_thermostat_arc_temp_knob_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_thermostat_arc_temp_knob_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_thermostat_arc_temp_knob_main_main_default, 0);
	lv_style_set_border_width(&style_screen_thermostat_arc_temp_knob_main_main_default, 0);
	lv_style_set_pad_left(&style_screen_thermostat_arc_temp_knob_main_main_default, 20);
	lv_style_set_pad_right(&style_screen_thermostat_arc_temp_knob_main_main_default, 20);
	lv_style_set_pad_top(&style_screen_thermostat_arc_temp_knob_main_main_default, 20);
	lv_style_set_pad_bottom(&style_screen_thermostat_arc_temp_knob_main_main_default, 20);
	lv_style_set_arc_color(&style_screen_thermostat_arc_temp_knob_main_main_default, lv_color_make(0x87, 0x2c, 0x2c));
	lv_style_set_arc_width(&style_screen_thermostat_arc_temp_knob_main_main_default, 8);
	lv_obj_add_style(ui->screen_thermostat_arc_temp_knob, &style_screen_thermostat_arc_temp_knob_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_arc_temp_knob_main_indicator_default
	static lv_style_t style_screen_thermostat_arc_temp_knob_main_indicator_default;
	if (style_screen_thermostat_arc_temp_knob_main_indicator_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_arc_temp_knob_main_indicator_default);
	else
		lv_style_init(&style_screen_thermostat_arc_temp_knob_main_indicator_default);
	lv_style_set_arc_color(&style_screen_thermostat_arc_temp_knob_main_indicator_default, lv_color_make(0xe6, 0xfa, 0x43));
	lv_style_set_arc_width(&style_screen_thermostat_arc_temp_knob_main_indicator_default, 8);
	lv_obj_add_style(ui->screen_thermostat_arc_temp_knob, &style_screen_thermostat_arc_temp_knob_main_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_arc_temp_knob_main_knob_default
	static lv_style_t style_screen_thermostat_arc_temp_knob_main_knob_default;
	if (style_screen_thermostat_arc_temp_knob_main_knob_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_arc_temp_knob_main_knob_default);
	else
		lv_style_init(&style_screen_thermostat_arc_temp_knob_main_knob_default);
	lv_style_set_bg_color(&style_screen_thermostat_arc_temp_knob_main_knob_default, lv_color_make(0xE6, 0xFA, 0x43));
	lv_style_set_bg_grad_color(&style_screen_thermostat_arc_temp_knob_main_knob_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_thermostat_arc_temp_knob_main_knob_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_thermostat_arc_temp_knob_main_knob_default, 255);
	lv_style_set_pad_all(&style_screen_thermostat_arc_temp_knob_main_knob_default, 0);
	lv_obj_add_style(ui->screen_thermostat_arc_temp_knob, &style_screen_thermostat_arc_temp_knob_main_knob_default, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_arc_set_mode(ui->screen_thermostat_arc_temp_knob, LV_ARC_MODE_NORMAL);
	lv_arc_set_range(ui->screen_thermostat_arc_temp_knob, 0, 50);
	lv_arc_set_bg_angles(ui->screen_thermostat_arc_temp_knob, 0, 280);
	lv_arc_set_angles(ui->screen_thermostat_arc_temp_knob, 0, 120);
	lv_arc_set_rotation(ui->screen_thermostat_arc_temp_knob, 130);
	lv_obj_set_style_arc_rounded(ui->screen_thermostat_arc_temp_knob, 0,  LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_rounded(ui->screen_thermostat_arc_temp_knob, 0, LV_STATE_DEFAULT);

	//Write codes screen_thermostat_label_temperature
	ui->screen_thermostat_label_temperature = lv_label_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_label_temperature, 520, 356);
	lv_obj_set_size(ui->screen_thermostat_label_temperature, 240, 50);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_label_temperature, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_thermostat_label_temperature, "21 °C");
	lv_label_set_long_mode(ui->screen_thermostat_label_temperature, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_label_temperature_main_main_default
	static lv_style_t style_screen_thermostat_label_temperature_main_main_default;
	if (style_screen_thermostat_label_temperature_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_label_temperature_main_main_default);
	else
		lv_style_init(&style_screen_thermostat_label_temperature_main_main_default);
	lv_style_set_radius(&style_screen_thermostat_label_temperature_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_thermostat_label_temperature_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_thermostat_label_temperature_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_thermostat_label_temperature_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_thermostat_label_temperature_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_thermostat_label_temperature_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_thermostat_label_temperature_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_thermostat_label_temperature_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_thermostat_label_temperature_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_thermostat_label_temperature_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_thermostat_label_temperature_main_main_default, 0);
	lv_style_set_text_color(&style_screen_thermostat_label_temperature_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_thermostat_label_temperature_main_main_default, &lv_font_montserratMedium_50);
	lv_style_set_text_letter_space(&style_screen_thermostat_label_temperature_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_thermostat_label_temperature_main_main_default, 0);
	lv_style_set_text_align(&style_screen_thermostat_label_temperature_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_thermostat_label_temperature_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_thermostat_label_temperature_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_thermostat_label_temperature_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_thermostat_label_temperature_main_main_default, 0);
	lv_obj_add_style(ui->screen_thermostat_label_temperature, &style_screen_thermostat_label_temperature_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_thermostat_label_face_rec_state
	ui->screen_thermostat_label_face_rec_state = lv_label_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_label_face_rec_state, 497, 663);
	lv_obj_set_size(ui->screen_thermostat_label_face_rec_state, 266, 32);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_label_face_rec_state, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_thermostat_label_face_rec_state, "User Identified");
	lv_label_set_long_mode(ui->screen_thermostat_label_face_rec_state, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_label_face_rec_state_main_main_default
	static lv_style_t style_screen_thermostat_label_face_rec_state_main_main_default;
	if (style_screen_thermostat_label_face_rec_state_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_label_face_rec_state_main_main_default);
	else
		lv_style_init(&style_screen_thermostat_label_face_rec_state_main_main_default);
	lv_style_set_radius(&style_screen_thermostat_label_face_rec_state_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_thermostat_label_face_rec_state_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_thermostat_label_face_rec_state_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_thermostat_label_face_rec_state_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_thermostat_label_face_rec_state_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_thermostat_label_face_rec_state_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_thermostat_label_face_rec_state_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_thermostat_label_face_rec_state_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_thermostat_label_face_rec_state_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_thermostat_label_face_rec_state_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_thermostat_label_face_rec_state_main_main_default, 0);
	lv_style_set_text_color(&style_screen_thermostat_label_face_rec_state_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_thermostat_label_face_rec_state_main_main_default, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_screen_thermostat_label_face_rec_state_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_thermostat_label_face_rec_state_main_main_default, 0);
	lv_style_set_text_align(&style_screen_thermostat_label_face_rec_state_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_thermostat_label_face_rec_state_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_thermostat_label_face_rec_state_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_thermostat_label_face_rec_state_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_thermostat_label_face_rec_state_main_main_default, 0);
	lv_obj_add_style(ui->screen_thermostat_label_face_rec_state, &style_screen_thermostat_label_face_rec_state_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_thermostat_img_face_blue
	ui->screen_thermostat_img_face_blue = lv_img_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_img_face_blue, 578, 552);
	lv_obj_set_size(ui->screen_thermostat_img_face_blue, 96, 93);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_img_face_blue, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_img_face_blue_main_main_default
	static lv_style_t style_screen_thermostat_img_face_blue_main_main_default;
	if (style_screen_thermostat_img_face_blue_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_img_face_blue_main_main_default);
	else
		lv_style_init(&style_screen_thermostat_img_face_blue_main_main_default);
	lv_style_set_img_recolor(&style_screen_thermostat_img_face_blue_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_thermostat_img_face_blue_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_thermostat_img_face_blue_main_main_default, 255);
	lv_obj_add_style(ui->screen_thermostat_img_face_blue, &style_screen_thermostat_img_face_blue_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_thermostat_img_face_blue, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_thermostat_img_face_blue,&_face_blue_96x93);
	lv_img_set_pivot(ui->screen_thermostat_img_face_blue, 50,50);
	lv_img_set_angle(ui->screen_thermostat_img_face_blue, 0);

	//Write codes screen_thermostat_img_face_green
	ui->screen_thermostat_img_face_green = lv_img_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_img_face_green, 578, 552);
	lv_obj_set_size(ui->screen_thermostat_img_face_green, 96, 93);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_img_face_green, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_img_face_green_main_main_default
	static lv_style_t style_screen_thermostat_img_face_green_main_main_default;
	if (style_screen_thermostat_img_face_green_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_img_face_green_main_main_default);
	else
		lv_style_init(&style_screen_thermostat_img_face_green_main_main_default);
	lv_style_set_img_recolor(&style_screen_thermostat_img_face_green_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_thermostat_img_face_green_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_thermostat_img_face_green_main_main_default, 255);
	lv_obj_add_style(ui->screen_thermostat_img_face_green, &style_screen_thermostat_img_face_green_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_thermostat_img_face_green, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_thermostat_img_face_green,&_face_green_96x93);
	lv_img_set_pivot(ui->screen_thermostat_img_face_green, 50,50);
	lv_img_set_angle(ui->screen_thermostat_img_face_green, 0);

	//Write codes screen_thermostat_img_red
	ui->screen_thermostat_img_red = lv_img_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_img_red, 578, 552);
	lv_obj_set_size(ui->screen_thermostat_img_red, 96, 93);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_img_red, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_img_red_main_main_default
	static lv_style_t style_screen_thermostat_img_red_main_main_default;
	if (style_screen_thermostat_img_red_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_img_red_main_main_default);
	else
		lv_style_init(&style_screen_thermostat_img_red_main_main_default);
	lv_style_set_img_recolor(&style_screen_thermostat_img_red_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_thermostat_img_red_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_thermostat_img_red_main_main_default, 255);
	lv_obj_add_style(ui->screen_thermostat_img_red, &style_screen_thermostat_img_red_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_thermostat_img_red, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_thermostat_img_red,&_face_red_96x93);
	lv_img_set_pivot(ui->screen_thermostat_img_red, 50,50);
	lv_img_set_angle(ui->screen_thermostat_img_red, 0);

	//Write codes screen_thermostat_imgbtn_fan_onoff
	ui->screen_thermostat_imgbtn_fan_onoff = lv_imgbtn_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_imgbtn_fan_onoff, 1136, 274);
	lv_obj_set_size(ui->screen_thermostat_imgbtn_fan_onoff, 102, 92);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_imgbtn_fan_onoff, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_fan_onoff, LV_IMGBTN_STATE_RELEASED, NULL, &_thermostat_fan_off_102x92, NULL);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_fan_onoff, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_thermostat_fan_on_102x92, NULL);

	//Write codes screen_thermostat_imgbtn_fan_high
	ui->screen_thermostat_imgbtn_fan_high = lv_imgbtn_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_imgbtn_fan_high, 1136, 365);
	lv_obj_set_size(ui->screen_thermostat_imgbtn_fan_high, 102, 84);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_imgbtn_fan_high, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_fan_high, LV_IMGBTN_STATE_RELEASED, NULL, &_thermostat_fan_high_off_102x84, NULL);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_fan_high, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_thermostat_fan_high_on_102x84, NULL);

	//Write codes screen_thermostat_imgbtn_fan_mid
	ui->screen_thermostat_imgbtn_fan_mid = lv_imgbtn_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_imgbtn_fan_mid, 1136, 448);
	lv_obj_set_size(ui->screen_thermostat_imgbtn_fan_mid, 102, 84);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_imgbtn_fan_mid, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_fan_mid, LV_IMGBTN_STATE_RELEASED, NULL, &_thermostat_fan_mid_off_102x84, NULL);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_fan_mid, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_thermostat_fan_mid_on_102x84, NULL);

	//Write codes screen_thermostat_imgbtn_fan_low
	ui->screen_thermostat_imgbtn_fan_low = lv_imgbtn_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_imgbtn_fan_low, 1136, 532);
	lv_obj_set_size(ui->screen_thermostat_imgbtn_fan_low, 102, 84);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_imgbtn_fan_low, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_fan_low, LV_IMGBTN_STATE_RELEASED, NULL, &_thermostat_fan_low_off_102x84, NULL);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_fan_low, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_thermostat_fan_low_on_102x84, NULL);

	//Write codes screen_thermostat_imgbtn_fan_auto
	ui->screen_thermostat_imgbtn_fan_auto = lv_imgbtn_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_imgbtn_fan_auto, 1136, 616);
	lv_obj_set_size(ui->screen_thermostat_imgbtn_fan_auto, 102, 92);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_imgbtn_fan_auto, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_fan_auto, LV_IMGBTN_STATE_RELEASED, NULL, &_thermostat_fan_auto_off_102x92, NULL);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_fan_auto, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_thermostat_fan_auto_on_102x92, NULL);

	//Write codes screen_thermostat_imgbtn_confirm
	ui->screen_thermostat_imgbtn_confirm = lv_imgbtn_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_imgbtn_confirm, 251, 602);
	lv_obj_set_size(ui->screen_thermostat_imgbtn_confirm, 210, 110);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_imgbtn_confirm, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_imgbtn_confirm_main_main_default
	static lv_style_t style_screen_thermostat_imgbtn_confirm_main_main_default;
	if (style_screen_thermostat_imgbtn_confirm_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_imgbtn_confirm_main_main_default);
	else
		lv_style_init(&style_screen_thermostat_imgbtn_confirm_main_main_default);
	lv_style_set_shadow_width(&style_screen_thermostat_imgbtn_confirm_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_thermostat_imgbtn_confirm_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_thermostat_imgbtn_confirm_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_thermostat_imgbtn_confirm_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_thermostat_imgbtn_confirm_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_thermostat_imgbtn_confirm_main_main_default, 0);
	lv_style_set_text_color(&style_screen_thermostat_imgbtn_confirm_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_thermostat_imgbtn_confirm_main_main_default, &lv_font_simsun_40);
	lv_style_set_text_align(&style_screen_thermostat_imgbtn_confirm_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_img_recolor(&style_screen_thermostat_imgbtn_confirm_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_thermostat_imgbtn_confirm_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_thermostat_imgbtn_confirm_main_main_default, 255);
	lv_obj_add_style(ui->screen_thermostat_imgbtn_confirm, &style_screen_thermostat_imgbtn_confirm_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_confirm, LV_IMGBTN_STATE_RELEASED, NULL, &_button_green_alpha_210x110, NULL);
	lv_obj_add_flag(ui->screen_thermostat_imgbtn_confirm, LV_OBJ_FLAG_CHECKABLE);
	ui->screen_thermostat_imgbtn_confirm_label = lv_label_create(ui->screen_thermostat_imgbtn_confirm);
	lv_label_set_text(ui->screen_thermostat_imgbtn_confirm_label, "确定");
	lv_obj_set_style_pad_all(ui->screen_thermostat_imgbtn_confirm, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->screen_thermostat_imgbtn_confirm_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes screen_thermostat_imgbtn_cancel
	ui->screen_thermostat_imgbtn_cancel = lv_imgbtn_create(ui->screen_thermostat_cont_thermBackground);
	lv_obj_set_pos(ui->screen_thermostat_imgbtn_cancel, 797, 602);
	lv_obj_set_size(ui->screen_thermostat_imgbtn_cancel, 210, 110);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_imgbtn_cancel, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_thermostat_imgbtn_cancel_main_main_default
	static lv_style_t style_screen_thermostat_imgbtn_cancel_main_main_default;
	if (style_screen_thermostat_imgbtn_cancel_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_thermostat_imgbtn_cancel_main_main_default);
	else
		lv_style_init(&style_screen_thermostat_imgbtn_cancel_main_main_default);
	lv_style_set_shadow_width(&style_screen_thermostat_imgbtn_cancel_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_thermostat_imgbtn_cancel_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_thermostat_imgbtn_cancel_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_thermostat_imgbtn_cancel_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_thermostat_imgbtn_cancel_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_thermostat_imgbtn_cancel_main_main_default, 0);
	lv_style_set_text_color(&style_screen_thermostat_imgbtn_cancel_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_thermostat_imgbtn_cancel_main_main_default, &lv_font_simsun_40);
	lv_style_set_text_align(&style_screen_thermostat_imgbtn_cancel_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_img_recolor(&style_screen_thermostat_imgbtn_cancel_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_thermostat_imgbtn_cancel_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_thermostat_imgbtn_cancel_main_main_default, 255);
	lv_obj_add_style(ui->screen_thermostat_imgbtn_cancel, &style_screen_thermostat_imgbtn_cancel_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_cancel, LV_IMGBTN_STATE_RELEASED, NULL, &_button_red_alpha_210x110, NULL);
	ui->screen_thermostat_imgbtn_cancel_label = lv_label_create(ui->screen_thermostat_imgbtn_cancel);
	lv_label_set_text(ui->screen_thermostat_imgbtn_cancel_label, "取消");
	lv_obj_set_style_pad_all(ui->screen_thermostat_imgbtn_cancel, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->screen_thermostat_imgbtn_cancel_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes screen_thermostat_img_mic_on
	ui->screen_thermostat_img_mic_on = lv_img_create(ui->screen_thermostat);
	lv_obj_set_pos(ui->screen_thermostat_img_mic_on, 1046, 612);
	lv_obj_set_size(ui->screen_thermostat_img_mic_on, 50, 80);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_img_mic_on, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_thermostat_img_mic_on, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_thermostat_img_mic_on,&_thermostat_mic_on_50x80);
	lv_img_set_pivot(ui->screen_thermostat_img_mic_on, 0,0);
	lv_img_set_angle(ui->screen_thermostat_img_mic_on, 0);

	//Write codes screen_thermostat_img_mic_off
	ui->screen_thermostat_img_mic_off = lv_img_create(ui->screen_thermostat);
	lv_obj_set_pos(ui->screen_thermostat_img_mic_off, 1046, 612);
	lv_obj_set_size(ui->screen_thermostat_img_mic_off, 50, 80);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_img_mic_off, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_thermostat_img_mic_off, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_thermostat_img_mic_off,&_thermostat_mic_off_50x80);
	lv_img_set_pivot(ui->screen_thermostat_img_mic_off, 0,0);
	lv_img_set_angle(ui->screen_thermostat_img_mic_off, 0);

	//Write codes screen_thermostat_imgbtn_home
	ui->screen_thermostat_imgbtn_home = lv_imgbtn_create(ui->screen_thermostat);
	lv_obj_set_pos(ui->screen_thermostat_imgbtn_home, 24, 26);
	lv_obj_set_size(ui->screen_thermostat_imgbtn_home, 85, 70);
	lv_obj_set_scrollbar_mode(ui->screen_thermostat_imgbtn_home, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_thermostat_imgbtn_home, LV_IMGBTN_STATE_RELEASED, NULL, &_home_85x70, NULL);

	//Init events for screen
	events_init_screen_thermostat(ui);
}