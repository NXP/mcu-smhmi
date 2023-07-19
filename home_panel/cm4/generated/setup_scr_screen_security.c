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


void setup_scr_screen_security(lv_ui *ui){

	//Write codes screen_security
	ui->screen_security = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->screen_security, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_main_main_default
	static lv_style_t style_screen_security_main_main_default;
	if (style_screen_security_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_main_main_default);
	else
		lv_style_init(&style_screen_security_main_main_default);
	lv_style_set_bg_color(&style_screen_security_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_screen_security_main_main_default, 0);
	lv_obj_add_style(ui->screen_security, &style_screen_security_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_security_cont_background
	ui->screen_security_cont_background = lv_obj_create(ui->screen_security);
	lv_obj_set_pos(ui->screen_security_cont_background, 0, 0);
	lv_obj_set_size(ui->screen_security_cont_background, 1280, 720);
	lv_obj_set_scrollbar_mode(ui->screen_security_cont_background, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_cont_background_main_main_default
	static lv_style_t style_screen_security_cont_background_main_main_default;
	if (style_screen_security_cont_background_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_cont_background_main_main_default);
	else
		lv_style_init(&style_screen_security_cont_background_main_main_default);
	lv_style_set_radius(&style_screen_security_cont_background_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_security_cont_background_main_main_default, lv_color_make(0xfc, 0xfc, 0xfc));
	lv_style_set_bg_grad_color(&style_screen_security_cont_background_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_security_cont_background_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_security_cont_background_main_main_default, 255);
	lv_style_set_shadow_width(&style_screen_security_cont_background_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_cont_background_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_cont_background_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_cont_background_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_cont_background_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_cont_background_main_main_default, 0);
	lv_style_set_border_color(&style_screen_security_cont_background_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_screen_security_cont_background_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_security_cont_background_main_main_default, 255);
	lv_style_set_pad_left(&style_screen_security_cont_background_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_security_cont_background_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_security_cont_background_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_security_cont_background_main_main_default, 0);
	lv_obj_add_style(ui->screen_security_cont_background, &style_screen_security_cont_background_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_security_img_homeBackGround
	ui->screen_security_img_homeBackGround = lv_img_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_img_homeBackGround, 0, 0);
	lv_obj_set_size(ui->screen_security_img_homeBackGround, 1280, 630);
	lv_obj_set_scrollbar_mode(ui->screen_security_img_homeBackGround, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_security_img_homeBackGround, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_security_img_homeBackGround,&_main_background_1280x630);
	lv_img_set_pivot(ui->screen_security_img_homeBackGround, 0,0);
	lv_img_set_angle(ui->screen_security_img_homeBackGround, 0);

	//Write codes screen_security_img_background_bottom
	ui->screen_security_img_background_bottom = lv_img_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_img_background_bottom, 0, 630);
	lv_obj_set_size(ui->screen_security_img_background_bottom, 1280, 90);
	lv_obj_set_scrollbar_mode(ui->screen_security_img_background_bottom, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_img_background_bottom_main_main_default
	static lv_style_t style_screen_security_img_background_bottom_main_main_default;
	if (style_screen_security_img_background_bottom_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_img_background_bottom_main_main_default);
	else
		lv_style_init(&style_screen_security_img_background_bottom_main_main_default);
	lv_style_set_img_recolor(&style_screen_security_img_background_bottom_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_security_img_background_bottom_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_security_img_background_bottom_main_main_default, 255);
	lv_obj_add_style(ui->screen_security_img_background_bottom, &style_screen_security_img_background_bottom_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_security_img_background_bottom, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_security_img_background_bottom,&_security_background_bottom_1280x90);
	lv_img_set_pivot(ui->screen_security_img_background_bottom, 50,50);
	lv_img_set_angle(ui->screen_security_img_background_bottom, 0);

	//Write codes screen_security_ddlist_language
	ui->screen_security_ddlist_language = lv_dropdown_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_ddlist_language, 1020, 30);
	lv_obj_set_size(ui->screen_security_ddlist_language, 193, 50);
	lv_obj_set_scrollbar_mode(ui->screen_security_ddlist_language, LV_SCROLLBAR_MODE_OFF);
	lv_dropdown_set_options(ui->screen_security_ddlist_language, "English\n中文");

	//Write style state: LV_STATE_DEFAULT for style_screen_security_ddlist_language_main_main_default
	static lv_style_t style_screen_security_ddlist_language_main_main_default;
	if (style_screen_security_ddlist_language_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_ddlist_language_main_main_default);
	else
		lv_style_init(&style_screen_security_ddlist_language_main_main_default);
	lv_style_set_radius(&style_screen_security_ddlist_language_main_main_default, 3);
	lv_style_set_bg_color(&style_screen_security_ddlist_language_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_security_ddlist_language_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_security_ddlist_language_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_security_ddlist_language_main_main_default, 255);
	lv_style_set_shadow_width(&style_screen_security_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_ddlist_language_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_ddlist_language_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_ddlist_language_main_main_default, 0);
	lv_style_set_border_color(&style_screen_security_ddlist_language_main_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_security_ddlist_language_main_main_default, 1);
	lv_style_set_border_opa(&style_screen_security_ddlist_language_main_main_default, 255);
	lv_style_set_text_color(&style_screen_security_ddlist_language_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_security_ddlist_language_main_main_default, &lv_font_simsun_32);
	lv_style_set_pad_left(&style_screen_security_ddlist_language_main_main_default, 6);
	lv_style_set_pad_right(&style_screen_security_ddlist_language_main_main_default, 6);
	lv_style_set_pad_top(&style_screen_security_ddlist_language_main_main_default, 8);
	lv_obj_add_style(ui->screen_security_ddlist_language, &style_screen_security_ddlist_language_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_CHECKED for style_screen_security_ddlist_language_extra_list_selected_checked
	static lv_style_t style_screen_security_ddlist_language_extra_list_selected_checked;
	if (style_screen_security_ddlist_language_extra_list_selected_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_security_ddlist_language_extra_list_selected_checked);
	else
		lv_style_init(&style_screen_security_ddlist_language_extra_list_selected_checked);
	lv_style_set_radius(&style_screen_security_ddlist_language_extra_list_selected_checked, 3);
	lv_style_set_bg_color(&style_screen_security_ddlist_language_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_color(&style_screen_security_ddlist_language_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_screen_security_ddlist_language_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_security_ddlist_language_extra_list_selected_checked, 255);
	lv_style_set_border_color(&style_screen_security_ddlist_language_extra_list_selected_checked, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_security_ddlist_language_extra_list_selected_checked, 1);
	lv_style_set_border_opa(&style_screen_security_ddlist_language_extra_list_selected_checked, 255);
	lv_style_set_text_color(&style_screen_security_ddlist_language_extra_list_selected_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_security_ddlist_language_extra_list_selected_checked, &lv_font_simsun_32);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_security_ddlist_language), &style_screen_security_ddlist_language_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_ddlist_language_extra_list_main_default
	static lv_style_t style_screen_security_ddlist_language_extra_list_main_default;
	if (style_screen_security_ddlist_language_extra_list_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_ddlist_language_extra_list_main_default);
	else
		lv_style_init(&style_screen_security_ddlist_language_extra_list_main_default);
	lv_style_set_radius(&style_screen_security_ddlist_language_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_screen_security_ddlist_language_extra_list_main_default, lv_color_make(0x8c, 0xa0, 0xa0));
	lv_style_set_bg_grad_color(&style_screen_security_ddlist_language_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_security_ddlist_language_extra_list_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_security_ddlist_language_extra_list_main_default, 255);
	lv_style_set_border_color(&style_screen_security_ddlist_language_extra_list_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_security_ddlist_language_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_screen_security_ddlist_language_extra_list_main_default, 255);
	lv_style_set_text_color(&style_screen_security_ddlist_language_extra_list_main_default, lv_color_make(0x0D, 0x30, 0x55));
	lv_style_set_text_font(&style_screen_security_ddlist_language_extra_list_main_default, &lv_font_simsun_32);
	lv_style_set_max_height(&style_screen_security_ddlist_language_extra_list_main_default, 150);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_security_ddlist_language), &style_screen_security_ddlist_language_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_ddlist_language_extra_list_scrollbar_default
	static lv_style_t style_screen_security_ddlist_language_extra_list_scrollbar_default;
	if (style_screen_security_ddlist_language_extra_list_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_ddlist_language_extra_list_scrollbar_default);
	else
		lv_style_init(&style_screen_security_ddlist_language_extra_list_scrollbar_default);
	lv_style_set_radius(&style_screen_security_ddlist_language_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_screen_security_ddlist_language_extra_list_scrollbar_default, lv_color_make(0xaf, 0xc3, 0xc3));
	lv_style_set_bg_opa(&style_screen_security_ddlist_language_extra_list_scrollbar_default, 255);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_security_ddlist_language), &style_screen_security_ddlist_language_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes screen_security_img_armed
	ui->screen_security_img_armed = lv_img_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_img_armed, 30, 114);
	lv_obj_set_size(ui->screen_security_img_armed, 324, 278);
	lv_obj_set_scrollbar_mode(ui->screen_security_img_armed, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_security_img_armed, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_security_img_armed,&_security_armed_324x278);
	lv_img_set_pivot(ui->screen_security_img_armed, 0,0);
	lv_img_set_angle(ui->screen_security_img_armed, 0);

	//Write codes screen_security_imgbtn_front_door
	ui->screen_security_imgbtn_front_door = lv_imgbtn_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_imgbtn_front_door, 188, 522);
	lv_obj_set_size(ui->screen_security_imgbtn_front_door, 212, 112);
	lv_obj_set_scrollbar_mode(ui->screen_security_imgbtn_front_door, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_security_imgbtn_front_door, LV_IMGBTN_STATE_RELEASED, NULL, &_security_front_door_locked_212x112, NULL);
	lv_imgbtn_set_src(ui->screen_security_imgbtn_front_door, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_security_front_door_unlocked_212x112, NULL);

	//Write codes screen_security_img_disarmed
	ui->screen_security_img_disarmed = lv_img_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_img_disarmed, 30, 114);
	lv_obj_set_size(ui->screen_security_img_disarmed, 324, 278);
	lv_obj_set_scrollbar_mode(ui->screen_security_img_disarmed, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_security_img_disarmed, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_security_img_disarmed,&_security_disarmed_324x278);
	lv_img_set_pivot(ui->screen_security_img_disarmed, 0,0);
	lv_img_set_angle(ui->screen_security_img_disarmed, 0);

	//Write codes screen_security_imgbtn_back_door
	ui->screen_security_imgbtn_back_door = lv_imgbtn_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_imgbtn_back_door, 522, 522);
	lv_obj_set_size(ui->screen_security_imgbtn_back_door, 212, 112);
	lv_obj_set_scrollbar_mode(ui->screen_security_imgbtn_back_door, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_security_imgbtn_back_door, LV_IMGBTN_STATE_RELEASED, NULL, &_security_back_door_locked_212x112, NULL);
	lv_imgbtn_set_src(ui->screen_security_imgbtn_back_door, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_security_back_door_unlocked_212x112, NULL);

	//Write codes screen_security_imgbtn_garage_door
	ui->screen_security_imgbtn_garage_door = lv_imgbtn_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_imgbtn_garage_door, 856, 522);
	lv_obj_set_size(ui->screen_security_imgbtn_garage_door, 212, 112);
	lv_obj_set_scrollbar_mode(ui->screen_security_imgbtn_garage_door, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_security_imgbtn_garage_door, LV_IMGBTN_STATE_RELEASED, NULL, &_security_garage_door_locked_212x112, NULL);
	lv_imgbtn_set_src(ui->screen_security_imgbtn_garage_door, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_security_garage_door_unlocked_212x112, NULL);

	//Write codes screen_security_imgbtn_user_setting
	ui->screen_security_imgbtn_user_setting = lv_imgbtn_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_imgbtn_user_setting, 1112, 430);
	lv_obj_set_size(ui->screen_security_imgbtn_user_setting, 128, 128);
	lv_obj_set_scrollbar_mode(ui->screen_security_imgbtn_user_setting, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_imgbtn_user_setting_main_main_default
	static lv_style_t style_screen_security_imgbtn_user_setting_main_main_default;
	if (style_screen_security_imgbtn_user_setting_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_imgbtn_user_setting_main_main_default);
	else
		lv_style_init(&style_screen_security_imgbtn_user_setting_main_main_default);
	lv_style_set_shadow_width(&style_screen_security_imgbtn_user_setting_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_imgbtn_user_setting_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_imgbtn_user_setting_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_imgbtn_user_setting_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_imgbtn_user_setting_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_imgbtn_user_setting_main_main_default, 0);
	lv_style_set_text_color(&style_screen_security_imgbtn_user_setting_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_align(&style_screen_security_imgbtn_user_setting_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_img_recolor(&style_screen_security_imgbtn_user_setting_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_security_imgbtn_user_setting_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_security_imgbtn_user_setting_main_main_default, 255);
	lv_obj_add_style(ui->screen_security_imgbtn_user_setting, &style_screen_security_imgbtn_user_setting_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_screen_security_imgbtn_user_setting_main_main_pressed
	static lv_style_t style_screen_security_imgbtn_user_setting_main_main_pressed;
	if (style_screen_security_imgbtn_user_setting_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_screen_security_imgbtn_user_setting_main_main_pressed);
	else
		lv_style_init(&style_screen_security_imgbtn_user_setting_main_main_pressed);
	lv_style_set_shadow_width(&style_screen_security_imgbtn_user_setting_main_main_pressed, 0);
	lv_style_set_shadow_color(&style_screen_security_imgbtn_user_setting_main_main_pressed, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_imgbtn_user_setting_main_main_pressed, 255);
	lv_style_set_shadow_spread(&style_screen_security_imgbtn_user_setting_main_main_pressed, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_imgbtn_user_setting_main_main_pressed, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_imgbtn_user_setting_main_main_pressed, 0);
	lv_style_set_text_color(&style_screen_security_imgbtn_user_setting_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_screen_security_imgbtn_user_setting_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_screen_security_imgbtn_user_setting_main_main_pressed, 74);
	lv_style_set_img_opa(&style_screen_security_imgbtn_user_setting_main_main_pressed, 130);
	lv_obj_add_style(ui->screen_security_imgbtn_user_setting, &style_screen_security_imgbtn_user_setting_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_screen_security_imgbtn_user_setting_main_main_checked
	static lv_style_t style_screen_security_imgbtn_user_setting_main_main_checked;
	if (style_screen_security_imgbtn_user_setting_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_security_imgbtn_user_setting_main_main_checked);
	else
		lv_style_init(&style_screen_security_imgbtn_user_setting_main_main_checked);
	lv_style_set_shadow_width(&style_screen_security_imgbtn_user_setting_main_main_checked, 0);
	lv_style_set_shadow_color(&style_screen_security_imgbtn_user_setting_main_main_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_imgbtn_user_setting_main_main_checked, 255);
	lv_style_set_shadow_spread(&style_screen_security_imgbtn_user_setting_main_main_checked, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_imgbtn_user_setting_main_main_checked, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_imgbtn_user_setting_main_main_checked, 0);
	lv_style_set_text_color(&style_screen_security_imgbtn_user_setting_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_screen_security_imgbtn_user_setting_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_screen_security_imgbtn_user_setting_main_main_checked, 0);
	lv_style_set_img_opa(&style_screen_security_imgbtn_user_setting_main_main_checked, 180);
	lv_obj_add_style(ui->screen_security_imgbtn_user_setting, &style_screen_security_imgbtn_user_setting_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->screen_security_imgbtn_user_setting, LV_IMGBTN_STATE_RELEASED, NULL, &_security_user_setting_128x128, NULL);

	//Write codes screen_security_imgbtn_help
	ui->screen_security_imgbtn_help = lv_imgbtn_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_imgbtn_help, 1158, 113);
	lv_obj_set_size(ui->screen_security_imgbtn_help, 100, 100);
	lv_obj_set_scrollbar_mode(ui->screen_security_imgbtn_help, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_security_imgbtn_help, LV_IMGBTN_STATE_RELEASED, NULL, &_help_100x100, NULL);

	//Write codes screen_security_label_title
	ui->screen_security_label_title = lv_label_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_label_title, 230, 30);
	lv_obj_set_size(ui->screen_security_label_title, 760, 60);
	lv_obj_set_scrollbar_mode(ui->screen_security_label_title, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_security_label_title, "SECURITY CONTROL");
	lv_label_set_long_mode(ui->screen_security_label_title, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_label_title_main_main_default
	static lv_style_t style_screen_security_label_title_main_main_default;
	if (style_screen_security_label_title_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_label_title_main_main_default);
	else
		lv_style_init(&style_screen_security_label_title_main_main_default);
	lv_style_set_radius(&style_screen_security_label_title_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_security_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_security_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_security_label_title_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_security_label_title_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_security_label_title_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_label_title_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_label_title_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_label_title_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_label_title_main_main_default, 0);
	lv_style_set_text_color(&style_screen_security_label_title_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_security_label_title_main_main_default, &lv_font_montserratMedium_50);
	lv_style_set_text_letter_space(&style_screen_security_label_title_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_security_label_title_main_main_default, 0);
	lv_style_set_text_align(&style_screen_security_label_title_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_security_label_title_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_security_label_title_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_security_label_title_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_security_label_title_main_main_default, 0);
	lv_obj_add_style(ui->screen_security_label_title, &style_screen_security_label_title_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_security_label_front_door
	ui->screen_security_label_front_door = lv_label_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_label_front_door, 123, 652);
	lv_obj_set_size(ui->screen_security_label_front_door, 344, 60);
	lv_obj_set_scrollbar_mode(ui->screen_security_label_front_door, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_security_label_front_door, "Front Door");
	lv_label_set_long_mode(ui->screen_security_label_front_door, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_label_front_door_main_main_default
	static lv_style_t style_screen_security_label_front_door_main_main_default;
	if (style_screen_security_label_front_door_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_label_front_door_main_main_default);
	else
		lv_style_init(&style_screen_security_label_front_door_main_main_default);
	lv_style_set_radius(&style_screen_security_label_front_door_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_security_label_front_door_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_security_label_front_door_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_security_label_front_door_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_security_label_front_door_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_security_label_front_door_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_label_front_door_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_label_front_door_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_label_front_door_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_label_front_door_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_label_front_door_main_main_default, 0);
	lv_style_set_text_color(&style_screen_security_label_front_door_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_security_label_front_door_main_main_default, &lv_font_montserratMedium_38);
	lv_style_set_text_letter_space(&style_screen_security_label_front_door_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_security_label_front_door_main_main_default, 0);
	lv_style_set_text_align(&style_screen_security_label_front_door_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_security_label_front_door_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_security_label_front_door_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_security_label_front_door_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_security_label_front_door_main_main_default, 0);
	lv_obj_add_style(ui->screen_security_label_front_door, &style_screen_security_label_front_door_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_security_label_back_door
	ui->screen_security_label_back_door = lv_label_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_label_back_door, 457, 652);
	lv_obj_set_size(ui->screen_security_label_back_door, 344, 60);
	lv_obj_set_scrollbar_mode(ui->screen_security_label_back_door, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_security_label_back_door, "Back Door");
	lv_label_set_long_mode(ui->screen_security_label_back_door, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_label_back_door_main_main_default
	static lv_style_t style_screen_security_label_back_door_main_main_default;
	if (style_screen_security_label_back_door_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_label_back_door_main_main_default);
	else
		lv_style_init(&style_screen_security_label_back_door_main_main_default);
	lv_style_set_radius(&style_screen_security_label_back_door_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_security_label_back_door_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_security_label_back_door_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_security_label_back_door_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_security_label_back_door_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_security_label_back_door_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_label_back_door_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_label_back_door_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_label_back_door_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_label_back_door_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_label_back_door_main_main_default, 0);
	lv_style_set_text_color(&style_screen_security_label_back_door_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_security_label_back_door_main_main_default, &lv_font_montserratMedium_38);
	lv_style_set_text_letter_space(&style_screen_security_label_back_door_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_security_label_back_door_main_main_default, 0);
	lv_style_set_text_align(&style_screen_security_label_back_door_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_security_label_back_door_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_security_label_back_door_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_security_label_back_door_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_security_label_back_door_main_main_default, 0);
	lv_obj_add_style(ui->screen_security_label_back_door, &style_screen_security_label_back_door_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_security_label_garage_door
	ui->screen_security_label_garage_door = lv_label_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_label_garage_door, 789, 652);
	lv_obj_set_size(ui->screen_security_label_garage_door, 344, 60);
	lv_obj_set_scrollbar_mode(ui->screen_security_label_garage_door, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_security_label_garage_door, "Garage Door");
	lv_label_set_long_mode(ui->screen_security_label_garage_door, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_label_garage_door_main_main_default
	static lv_style_t style_screen_security_label_garage_door_main_main_default;
	if (style_screen_security_label_garage_door_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_label_garage_door_main_main_default);
	else
		lv_style_init(&style_screen_security_label_garage_door_main_main_default);
	lv_style_set_radius(&style_screen_security_label_garage_door_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_security_label_garage_door_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_security_label_garage_door_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_security_label_garage_door_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_security_label_garage_door_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_security_label_garage_door_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_label_garage_door_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_label_garage_door_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_label_garage_door_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_label_garage_door_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_label_garage_door_main_main_default, 0);
	lv_style_set_text_color(&style_screen_security_label_garage_door_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_security_label_garage_door_main_main_default, &lv_font_montserratMedium_38);
	lv_style_set_text_letter_space(&style_screen_security_label_garage_door_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_security_label_garage_door_main_main_default, 0);
	lv_style_set_text_align(&style_screen_security_label_garage_door_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_security_label_garage_door_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_security_label_garage_door_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_security_label_garage_door_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_security_label_garage_door_main_main_default, 0);
	lv_obj_add_style(ui->screen_security_label_garage_door, &style_screen_security_label_garage_door_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_security_label_arm_state
	ui->screen_security_label_arm_state = lv_label_create(ui->screen_security_cont_background);
	lv_obj_set_pos(ui->screen_security_label_arm_state, 18, 410);
	lv_obj_set_size(ui->screen_security_label_arm_state, 406, 54);
	lv_obj_set_scrollbar_mode(ui->screen_security_label_arm_state, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_security_label_arm_state, "System Disarmed");
	lv_label_set_long_mode(ui->screen_security_label_arm_state, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_label_arm_state_main_main_default
	static lv_style_t style_screen_security_label_arm_state_main_main_default;
	if (style_screen_security_label_arm_state_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_label_arm_state_main_main_default);
	else
		lv_style_init(&style_screen_security_label_arm_state_main_main_default);
	lv_style_set_radius(&style_screen_security_label_arm_state_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_security_label_arm_state_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_security_label_arm_state_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_security_label_arm_state_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_security_label_arm_state_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_security_label_arm_state_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_label_arm_state_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_label_arm_state_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_label_arm_state_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_label_arm_state_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_label_arm_state_main_main_default, 0);
	lv_style_set_text_color(&style_screen_security_label_arm_state_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_security_label_arm_state_main_main_default, &lv_font_montserratMedium_38);
	lv_style_set_text_letter_space(&style_screen_security_label_arm_state_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_security_label_arm_state_main_main_default, 0);
	lv_style_set_text_align(&style_screen_security_label_arm_state_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_security_label_arm_state_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_security_label_arm_state_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_security_label_arm_state_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_security_label_arm_state_main_main_default, 0);
	lv_obj_add_style(ui->screen_security_label_arm_state, &style_screen_security_label_arm_state_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_security_imgbtn_home
	ui->screen_security_imgbtn_home = lv_imgbtn_create(ui->screen_security);
	lv_obj_set_pos(ui->screen_security_imgbtn_home, 24, 26);
	lv_obj_set_size(ui->screen_security_imgbtn_home, 85, 70);
	lv_obj_set_scrollbar_mode(ui->screen_security_imgbtn_home, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_security_imgbtn_home, LV_IMGBTN_STATE_RELEASED, NULL, &_home_85x70, NULL);

	//Write codes screen_security_imgbtn_arm
	ui->screen_security_imgbtn_arm = lv_imgbtn_create(ui->screen_security);
	lv_obj_set_pos(ui->screen_security_imgbtn_arm, 766, 186);
	lv_obj_set_size(ui->screen_security_imgbtn_arm, 270, 260);
	lv_obj_set_scrollbar_mode(ui->screen_security_imgbtn_arm, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_security_imgbtn_arm, LV_IMGBTN_STATE_RELEASED, NULL, &_security_arm_270x260, NULL);

	//Write codes screen_security_img_mic_on
	ui->screen_security_img_mic_on = lv_img_create(ui->screen_security);
	lv_obj_set_pos(ui->screen_security_img_mic_on, 1188, 595);
	lv_obj_set_size(ui->screen_security_img_mic_on, 70, 112);
	lv_obj_set_scrollbar_mode(ui->screen_security_img_mic_on, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_security_img_mic_on, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_security_img_mic_on,&_main_mic_on_70x112);
	lv_img_set_pivot(ui->screen_security_img_mic_on, 0,0);
	lv_img_set_angle(ui->screen_security_img_mic_on, 0);

	//Write codes screen_security_img_mic_off
	ui->screen_security_img_mic_off = lv_img_create(ui->screen_security);
	lv_obj_set_pos(ui->screen_security_img_mic_off, 1188, 595);
	lv_obj_set_size(ui->screen_security_img_mic_off, 70, 112);
	lv_obj_set_scrollbar_mode(ui->screen_security_img_mic_off, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_security_img_mic_off, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_security_img_mic_off,&_main_mic_off_70x112);
	lv_img_set_pivot(ui->screen_security_img_mic_off, 0,0);
	lv_img_set_angle(ui->screen_security_img_mic_off, 0);

	//Init events for screen
	events_init_screen_security(ui);
}