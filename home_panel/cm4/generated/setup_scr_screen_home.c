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


void setup_scr_screen_home(lv_ui *ui){

	//Write codes screen_home
	ui->screen_home = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->screen_home, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_main_main_default
	static lv_style_t style_screen_home_main_main_default;
	if (style_screen_home_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_main_main_default);
	else
		lv_style_init(&style_screen_home_main_main_default);
	lv_style_set_bg_color(&style_screen_home_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_screen_home_main_main_default, 0);
	lv_obj_add_style(ui->screen_home, &style_screen_home_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_home_cont_background
	ui->screen_home_cont_background = lv_obj_create(ui->screen_home);
	lv_obj_set_pos(ui->screen_home_cont_background, 0, 0);
	lv_obj_set_size(ui->screen_home_cont_background, 1280, 720);
	lv_obj_set_scrollbar_mode(ui->screen_home_cont_background, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_cont_background_main_main_default
	static lv_style_t style_screen_home_cont_background_main_main_default;
	if (style_screen_home_cont_background_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_cont_background_main_main_default);
	else
		lv_style_init(&style_screen_home_cont_background_main_main_default);
	lv_style_set_radius(&style_screen_home_cont_background_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_home_cont_background_main_main_default, lv_color_make(0xfc, 0xfc, 0xfc));
	lv_style_set_bg_grad_color(&style_screen_home_cont_background_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_home_cont_background_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_home_cont_background_main_main_default, 255);
	lv_style_set_shadow_width(&style_screen_home_cont_background_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_home_cont_background_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_home_cont_background_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_home_cont_background_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_home_cont_background_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_home_cont_background_main_main_default, 0);
	lv_style_set_border_color(&style_screen_home_cont_background_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_screen_home_cont_background_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_home_cont_background_main_main_default, 255);
	lv_style_set_pad_left(&style_screen_home_cont_background_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_home_cont_background_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_home_cont_background_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_home_cont_background_main_main_default, 0);
	lv_obj_add_style(ui->screen_home_cont_background, &style_screen_home_cont_background_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_home_img_background
	ui->screen_home_img_background = lv_img_create(ui->screen_home_cont_background);
	lv_obj_set_pos(ui->screen_home_img_background, 0, 0);
	lv_obj_set_size(ui->screen_home_img_background, 1280, 630);
	lv_obj_set_scrollbar_mode(ui->screen_home_img_background, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_img_background_main_main_default
	static lv_style_t style_screen_home_img_background_main_main_default;
	if (style_screen_home_img_background_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_img_background_main_main_default);
	else
		lv_style_init(&style_screen_home_img_background_main_main_default);
	lv_style_set_img_recolor(&style_screen_home_img_background_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_home_img_background_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_home_img_background_main_main_default, 255);
	lv_obj_add_style(ui->screen_home_img_background, &style_screen_home_img_background_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_home_img_background, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_home_img_background,&_main_background_1280x630);
	lv_img_set_pivot(ui->screen_home_img_background, 0,0);
	lv_img_set_angle(ui->screen_home_img_background, 0);

	//Write codes screen_home_img_background_bottom
	ui->screen_home_img_background_bottom = lv_img_create(ui->screen_home_cont_background);
	lv_obj_set_pos(ui->screen_home_img_background_bottom, 0, 630);
	lv_obj_set_size(ui->screen_home_img_background_bottom, 1280, 90);
	lv_obj_set_scrollbar_mode(ui->screen_home_img_background_bottom, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_img_background_bottom_main_main_default
	static lv_style_t style_screen_home_img_background_bottom_main_main_default;
	if (style_screen_home_img_background_bottom_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_img_background_bottom_main_main_default);
	else
		lv_style_init(&style_screen_home_img_background_bottom_main_main_default);
	lv_style_set_img_recolor(&style_screen_home_img_background_bottom_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_home_img_background_bottom_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_home_img_background_bottom_main_main_default, 255);
	lv_obj_add_style(ui->screen_home_img_background_bottom, &style_screen_home_img_background_bottom_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_home_img_background_bottom, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_home_img_background_bottom,&_main_background_bottom_1280x90);
	lv_img_set_pivot(ui->screen_home_img_background_bottom, 50,50);
	lv_img_set_angle(ui->screen_home_img_background_bottom, 0);

	//Write codes screen_home_ddlist_language
	ui->screen_home_ddlist_language = lv_dropdown_create(ui->screen_home_cont_background);
	lv_obj_set_pos(ui->screen_home_ddlist_language, 1030, 35);
	lv_obj_set_size(ui->screen_home_ddlist_language, 193, 50);
	lv_obj_set_scrollbar_mode(ui->screen_home_ddlist_language, LV_SCROLLBAR_MODE_OFF);
	lv_dropdown_set_options(ui->screen_home_ddlist_language, "English\n中文");

	//Write style state: LV_STATE_DEFAULT for style_screen_home_ddlist_language_main_main_default
	static lv_style_t style_screen_home_ddlist_language_main_main_default;
	if (style_screen_home_ddlist_language_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_ddlist_language_main_main_default);
	else
		lv_style_init(&style_screen_home_ddlist_language_main_main_default);
	lv_style_set_radius(&style_screen_home_ddlist_language_main_main_default, 3);
	lv_style_set_bg_color(&style_screen_home_ddlist_language_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_home_ddlist_language_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_home_ddlist_language_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_home_ddlist_language_main_main_default, 255);
	lv_style_set_shadow_width(&style_screen_home_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_home_ddlist_language_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_home_ddlist_language_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_home_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_home_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_home_ddlist_language_main_main_default, 0);
	lv_style_set_border_color(&style_screen_home_ddlist_language_main_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_home_ddlist_language_main_main_default, 1);
	lv_style_set_border_opa(&style_screen_home_ddlist_language_main_main_default, 255);
	lv_style_set_text_color(&style_screen_home_ddlist_language_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_home_ddlist_language_main_main_default, &lv_font_simsun_32);
	lv_style_set_pad_left(&style_screen_home_ddlist_language_main_main_default, 6);
	lv_style_set_pad_right(&style_screen_home_ddlist_language_main_main_default, 6);
	lv_style_set_pad_top(&style_screen_home_ddlist_language_main_main_default, 8);
	lv_obj_add_style(ui->screen_home_ddlist_language, &style_screen_home_ddlist_language_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_CHECKED for style_screen_home_ddlist_language_extra_list_selected_checked
	static lv_style_t style_screen_home_ddlist_language_extra_list_selected_checked;
	if (style_screen_home_ddlist_language_extra_list_selected_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_home_ddlist_language_extra_list_selected_checked);
	else
		lv_style_init(&style_screen_home_ddlist_language_extra_list_selected_checked);
	lv_style_set_radius(&style_screen_home_ddlist_language_extra_list_selected_checked, 3);
	lv_style_set_bg_color(&style_screen_home_ddlist_language_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_color(&style_screen_home_ddlist_language_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_screen_home_ddlist_language_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_home_ddlist_language_extra_list_selected_checked, 255);
	lv_style_set_border_color(&style_screen_home_ddlist_language_extra_list_selected_checked, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_home_ddlist_language_extra_list_selected_checked, 1);
	lv_style_set_border_opa(&style_screen_home_ddlist_language_extra_list_selected_checked, 255);
	lv_style_set_text_color(&style_screen_home_ddlist_language_extra_list_selected_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_home_ddlist_language_extra_list_selected_checked, &lv_font_simsun_32);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_home_ddlist_language), &style_screen_home_ddlist_language_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_ddlist_language_extra_list_main_default
	static lv_style_t style_screen_home_ddlist_language_extra_list_main_default;
	if (style_screen_home_ddlist_language_extra_list_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_ddlist_language_extra_list_main_default);
	else
		lv_style_init(&style_screen_home_ddlist_language_extra_list_main_default);
	lv_style_set_radius(&style_screen_home_ddlist_language_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_screen_home_ddlist_language_extra_list_main_default, lv_color_make(0x8c, 0xa0, 0xa0));
	lv_style_set_bg_grad_color(&style_screen_home_ddlist_language_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_home_ddlist_language_extra_list_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_home_ddlist_language_extra_list_main_default, 255);
	lv_style_set_border_color(&style_screen_home_ddlist_language_extra_list_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_home_ddlist_language_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_screen_home_ddlist_language_extra_list_main_default, 255);
	lv_style_set_text_color(&style_screen_home_ddlist_language_extra_list_main_default, lv_color_make(0x0D, 0x30, 0x55));
	lv_style_set_text_font(&style_screen_home_ddlist_language_extra_list_main_default, &lv_font_simsun_32);
	lv_style_set_max_height(&style_screen_home_ddlist_language_extra_list_main_default, 150);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_home_ddlist_language), &style_screen_home_ddlist_language_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_ddlist_language_extra_list_scrollbar_default
	static lv_style_t style_screen_home_ddlist_language_extra_list_scrollbar_default;
	if (style_screen_home_ddlist_language_extra_list_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_ddlist_language_extra_list_scrollbar_default);
	else
		lv_style_init(&style_screen_home_ddlist_language_extra_list_scrollbar_default);
	lv_style_set_radius(&style_screen_home_ddlist_language_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_screen_home_ddlist_language_extra_list_scrollbar_default, lv_color_make(0xaf, 0xc3, 0xc3));
	lv_style_set_bg_opa(&style_screen_home_ddlist_language_extra_list_scrollbar_default, 255);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_home_ddlist_language), &style_screen_home_ddlist_language_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes screen_home_imgbtn_thermostat
	ui->screen_home_imgbtn_thermostat = lv_imgbtn_create(ui->screen_home_cont_background);
	lv_obj_set_pos(ui->screen_home_imgbtn_thermostat, 211, 445);
	lv_obj_set_size(ui->screen_home_imgbtn_thermostat, 160, 160);
	lv_obj_set_scrollbar_mode(ui->screen_home_imgbtn_thermostat, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_home_imgbtn_thermostat, LV_IMGBTN_STATE_RELEASED, NULL, &_main_thermostat_160x160, NULL);
	lv_obj_add_flag(ui->screen_home_imgbtn_thermostat, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_home_imgbtn_security
	ui->screen_home_imgbtn_security = lv_imgbtn_create(ui->screen_home_cont_background);
	lv_obj_set_pos(ui->screen_home_imgbtn_security, 553, 445);
	lv_obj_set_size(ui->screen_home_imgbtn_security, 160, 160);
	lv_obj_set_scrollbar_mode(ui->screen_home_imgbtn_security, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_home_imgbtn_security, LV_IMGBTN_STATE_RELEASED, NULL, &_main_security_160x160, NULL);
	lv_obj_add_flag(ui->screen_home_imgbtn_security, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_home_imgbtn_audio_player
	ui->screen_home_imgbtn_audio_player = lv_imgbtn_create(ui->screen_home_cont_background);
	lv_obj_set_pos(ui->screen_home_imgbtn_audio_player, 928, 445);
	lv_obj_set_size(ui->screen_home_imgbtn_audio_player, 160, 160);
	lv_obj_set_scrollbar_mode(ui->screen_home_imgbtn_audio_player, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_home_imgbtn_audio_player, LV_IMGBTN_STATE_RELEASED, NULL, &_main_audio_player_160x160, NULL);
	lv_obj_add_flag(ui->screen_home_imgbtn_audio_player, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_home_label_homeTitle
	ui->screen_home_label_homeTitle = lv_label_create(ui->screen_home_cont_background);
	lv_obj_set_pos(ui->screen_home_label_homeTitle, 390, 29);
	lv_obj_set_size(ui->screen_home_label_homeTitle, 500, 60);
	lv_obj_set_scrollbar_mode(ui->screen_home_label_homeTitle, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_home_label_homeTitle, "Monday, May 18");
	lv_label_set_long_mode(ui->screen_home_label_homeTitle, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_label_hometitle_main_main_default
	static lv_style_t style_screen_home_label_hometitle_main_main_default;
	if (style_screen_home_label_hometitle_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_label_hometitle_main_main_default);
	else
		lv_style_init(&style_screen_home_label_hometitle_main_main_default);
	lv_style_set_radius(&style_screen_home_label_hometitle_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_home_label_hometitle_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_home_label_hometitle_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_home_label_hometitle_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_home_label_hometitle_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_home_label_hometitle_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_home_label_hometitle_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_home_label_hometitle_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_home_label_hometitle_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_home_label_hometitle_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_home_label_hometitle_main_main_default, 0);
	lv_style_set_text_color(&style_screen_home_label_hometitle_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_home_label_hometitle_main_main_default, &lv_font_montserratMedium_50);
	lv_style_set_text_letter_space(&style_screen_home_label_hometitle_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_home_label_hometitle_main_main_default, 0);
	lv_style_set_text_align(&style_screen_home_label_hometitle_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_home_label_hometitle_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_home_label_hometitle_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_home_label_hometitle_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_home_label_hometitle_main_main_default, 0);
	lv_obj_add_style(ui->screen_home_label_homeTitle, &style_screen_home_label_hometitle_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_home_imgbtn_help
	ui->screen_home_imgbtn_help = lv_imgbtn_create(ui->screen_home_cont_background);
	lv_obj_set_pos(ui->screen_home_imgbtn_help, 1158, 113);
	lv_obj_set_size(ui->screen_home_imgbtn_help, 100, 100);
	lv_obj_set_scrollbar_mode(ui->screen_home_imgbtn_help, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_home_imgbtn_help, LV_IMGBTN_STATE_RELEASED, NULL, &_help_100x100, NULL);
	lv_obj_add_flag(ui->screen_home_imgbtn_help, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_home_img_temp_humidity
	ui->screen_home_img_temp_humidity = lv_img_create(ui->screen_home_cont_background);
	lv_obj_set_pos(ui->screen_home_img_temp_humidity, 20, 142);
	lv_obj_set_size(ui->screen_home_img_temp_humidity, 34, 128);
	lv_obj_set_scrollbar_mode(ui->screen_home_img_temp_humidity, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_img_temp_humidity_main_main_default
	static lv_style_t style_screen_home_img_temp_humidity_main_main_default;
	if (style_screen_home_img_temp_humidity_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_img_temp_humidity_main_main_default);
	else
		lv_style_init(&style_screen_home_img_temp_humidity_main_main_default);
	lv_style_set_img_recolor(&style_screen_home_img_temp_humidity_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_home_img_temp_humidity_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_home_img_temp_humidity_main_main_default, 255);
	lv_obj_add_style(ui->screen_home_img_temp_humidity, &style_screen_home_img_temp_humidity_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_home_img_temp_humidity, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_home_img_temp_humidity,&_main_temp_humidity_34x128);
	lv_img_set_pivot(ui->screen_home_img_temp_humidity, 50,50);
	lv_img_set_angle(ui->screen_home_img_temp_humidity, 0);

	//Write codes screen_home_label_temperature
	ui->screen_home_label_temperature = lv_label_create(ui->screen_home_cont_background);
	lv_obj_set_pos(ui->screen_home_label_temperature, 59, 154);
	lv_obj_set_size(ui->screen_home_label_temperature, 101, 41);
	lv_obj_set_scrollbar_mode(ui->screen_home_label_temperature, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_home_label_temperature, "21 °c");
	lv_label_set_long_mode(ui->screen_home_label_temperature, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_label_temperature_main_main_default
	static lv_style_t style_screen_home_label_temperature_main_main_default;
	if (style_screen_home_label_temperature_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_label_temperature_main_main_default);
	else
		lv_style_init(&style_screen_home_label_temperature_main_main_default);
	lv_style_set_radius(&style_screen_home_label_temperature_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_home_label_temperature_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_home_label_temperature_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_home_label_temperature_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_home_label_temperature_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_home_label_temperature_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_home_label_temperature_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_home_label_temperature_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_home_label_temperature_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_home_label_temperature_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_home_label_temperature_main_main_default, 0);
	lv_style_set_text_color(&style_screen_home_label_temperature_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_home_label_temperature_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_screen_home_label_temperature_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_home_label_temperature_main_main_default, 0);
	lv_style_set_text_align(&style_screen_home_label_temperature_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_home_label_temperature_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_home_label_temperature_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_home_label_temperature_main_main_default, 8);
	lv_style_set_pad_bottom(&style_screen_home_label_temperature_main_main_default, 0);
	lv_obj_add_style(ui->screen_home_label_temperature, &style_screen_home_label_temperature_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_home_label_humidity
	ui->screen_home_label_humidity = lv_label_create(ui->screen_home_cont_background);
	lv_obj_set_pos(ui->screen_home_label_humidity, 56, 224);
	lv_obj_set_size(ui->screen_home_label_humidity, 100, 40);
	lv_obj_set_scrollbar_mode(ui->screen_home_label_humidity, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_home_label_humidity, "68%");
	lv_label_set_long_mode(ui->screen_home_label_humidity, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_label_humidity_main_main_default
	static lv_style_t style_screen_home_label_humidity_main_main_default;
	if (style_screen_home_label_humidity_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_label_humidity_main_main_default);
	else
		lv_style_init(&style_screen_home_label_humidity_main_main_default);
	lv_style_set_radius(&style_screen_home_label_humidity_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_home_label_humidity_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_home_label_humidity_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_home_label_humidity_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_home_label_humidity_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_home_label_humidity_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_home_label_humidity_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_home_label_humidity_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_home_label_humidity_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_home_label_humidity_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_home_label_humidity_main_main_default, 0);
	lv_style_set_text_color(&style_screen_home_label_humidity_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_home_label_humidity_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_screen_home_label_humidity_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_home_label_humidity_main_main_default, 0);
	lv_style_set_text_align(&style_screen_home_label_humidity_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_home_label_humidity_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_home_label_humidity_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_home_label_humidity_main_main_default, 8);
	lv_style_set_pad_bottom(&style_screen_home_label_humidity_main_main_default, 0);
	lv_obj_add_style(ui->screen_home_label_humidity, &style_screen_home_label_humidity_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_home_label_thermostat
	ui->screen_home_label_thermostat = lv_label_create(ui->screen_home);
	lv_obj_set_pos(ui->screen_home_label_thermostat, 114, 655);
	lv_obj_set_size(ui->screen_home_label_thermostat, 338, 52);
	lv_obj_set_scrollbar_mode(ui->screen_home_label_thermostat, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_home_label_thermostat, "Thermostat");
	lv_label_set_long_mode(ui->screen_home_label_thermostat, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_label_thermostat_main_main_default
	static lv_style_t style_screen_home_label_thermostat_main_main_default;
	if (style_screen_home_label_thermostat_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_label_thermostat_main_main_default);
	else
		lv_style_init(&style_screen_home_label_thermostat_main_main_default);
	lv_style_set_radius(&style_screen_home_label_thermostat_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_home_label_thermostat_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_home_label_thermostat_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_home_label_thermostat_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_home_label_thermostat_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_home_label_thermostat_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_home_label_thermostat_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_home_label_thermostat_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_home_label_thermostat_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_home_label_thermostat_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_home_label_thermostat_main_main_default, 0);
	lv_style_set_text_color(&style_screen_home_label_thermostat_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_home_label_thermostat_main_main_default, &lv_font_montserratMedium_38);
	lv_style_set_text_letter_space(&style_screen_home_label_thermostat_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_home_label_thermostat_main_main_default, 0);
	lv_style_set_text_align(&style_screen_home_label_thermostat_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_home_label_thermostat_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_home_label_thermostat_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_home_label_thermostat_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_home_label_thermostat_main_main_default, 0);
	lv_obj_add_style(ui->screen_home_label_thermostat, &style_screen_home_label_thermostat_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_home_label_security
	ui->screen_home_label_security = lv_label_create(ui->screen_home);
	lv_obj_set_pos(ui->screen_home_label_security, 518, 655);
	lv_obj_set_size(ui->screen_home_label_security, 221, 49);
	lv_obj_set_scrollbar_mode(ui->screen_home_label_security, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_home_label_security, "Security");
	lv_label_set_long_mode(ui->screen_home_label_security, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_label_security_main_main_default
	static lv_style_t style_screen_home_label_security_main_main_default;
	if (style_screen_home_label_security_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_label_security_main_main_default);
	else
		lv_style_init(&style_screen_home_label_security_main_main_default);
	lv_style_set_radius(&style_screen_home_label_security_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_home_label_security_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_home_label_security_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_home_label_security_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_home_label_security_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_home_label_security_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_home_label_security_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_home_label_security_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_home_label_security_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_home_label_security_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_home_label_security_main_main_default, 0);
	lv_style_set_text_color(&style_screen_home_label_security_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_home_label_security_main_main_default, &lv_font_montserratMedium_38);
	lv_style_set_text_letter_space(&style_screen_home_label_security_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_home_label_security_main_main_default, 0);
	lv_style_set_text_align(&style_screen_home_label_security_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_home_label_security_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_home_label_security_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_home_label_security_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_home_label_security_main_main_default, 0);
	lv_obj_add_style(ui->screen_home_label_security, &style_screen_home_label_security_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_home_label_audio_player
	ui->screen_home_label_audio_player = lv_label_create(ui->screen_home);
	lv_obj_set_pos(ui->screen_home_label_audio_player, 832, 655);
	lv_obj_set_size(ui->screen_home_label_audio_player, 352, 49);
	lv_obj_set_scrollbar_mode(ui->screen_home_label_audio_player, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_home_label_audio_player, "Audio Player");
	lv_label_set_long_mode(ui->screen_home_label_audio_player, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_label_audio_player_main_main_default
	static lv_style_t style_screen_home_label_audio_player_main_main_default;
	if (style_screen_home_label_audio_player_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_label_audio_player_main_main_default);
	else
		lv_style_init(&style_screen_home_label_audio_player_main_main_default);
	lv_style_set_radius(&style_screen_home_label_audio_player_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_home_label_audio_player_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_home_label_audio_player_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_home_label_audio_player_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_home_label_audio_player_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_home_label_audio_player_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_home_label_audio_player_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_home_label_audio_player_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_home_label_audio_player_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_home_label_audio_player_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_home_label_audio_player_main_main_default, 0);
	lv_style_set_text_color(&style_screen_home_label_audio_player_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_home_label_audio_player_main_main_default, &lv_font_montserratMedium_38);
	lv_style_set_text_letter_space(&style_screen_home_label_audio_player_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_home_label_audio_player_main_main_default, 0);
	lv_style_set_text_align(&style_screen_home_label_audio_player_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_home_label_audio_player_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_home_label_audio_player_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_home_label_audio_player_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_home_label_audio_player_main_main_default, 0);
	lv_obj_add_style(ui->screen_home_label_audio_player, &style_screen_home_label_audio_player_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_home_img_mic_on
	ui->screen_home_img_mic_on = lv_img_create(ui->screen_home);
	lv_obj_set_pos(ui->screen_home_img_mic_on, 1188, 595);
	lv_obj_set_size(ui->screen_home_img_mic_on, 70, 112);
	lv_obj_set_scrollbar_mode(ui->screen_home_img_mic_on, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_img_mic_on_main_main_default
	static lv_style_t style_screen_home_img_mic_on_main_main_default;
	if (style_screen_home_img_mic_on_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_img_mic_on_main_main_default);
	else
		lv_style_init(&style_screen_home_img_mic_on_main_main_default);
	lv_style_set_img_recolor(&style_screen_home_img_mic_on_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_home_img_mic_on_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_home_img_mic_on_main_main_default, 255);
	lv_obj_add_style(ui->screen_home_img_mic_on, &style_screen_home_img_mic_on_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_home_img_mic_on, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_home_img_mic_on,&_main_mic_on_70x112);
	lv_img_set_pivot(ui->screen_home_img_mic_on, 0,0);
	lv_img_set_angle(ui->screen_home_img_mic_on, 0);

	//Write codes screen_home_img_mic_off
	ui->screen_home_img_mic_off = lv_img_create(ui->screen_home);
	lv_obj_set_pos(ui->screen_home_img_mic_off, 1189, 595);
	lv_obj_set_size(ui->screen_home_img_mic_off, 70, 112);
	lv_obj_set_scrollbar_mode(ui->screen_home_img_mic_off, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_home_img_mic_off_main_main_default
	static lv_style_t style_screen_home_img_mic_off_main_main_default;
	if (style_screen_home_img_mic_off_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_home_img_mic_off_main_main_default);
	else
		lv_style_init(&style_screen_home_img_mic_off_main_main_default);
	lv_style_set_img_recolor(&style_screen_home_img_mic_off_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_home_img_mic_off_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_home_img_mic_off_main_main_default, 255);
	lv_obj_add_style(ui->screen_home_img_mic_off, &style_screen_home_img_mic_off_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_home_img_mic_off, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_home_img_mic_off,&_main_mic_off_70x112);
	lv_img_set_pivot(ui->screen_home_img_mic_off, 0,0);
	lv_img_set_angle(ui->screen_home_img_mic_off, 0);

	//Init events for screen
	events_init_screen_home(ui);
}