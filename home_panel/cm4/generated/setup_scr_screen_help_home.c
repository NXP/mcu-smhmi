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


void setup_scr_screen_help_home(lv_ui *ui){

	//Write codes screen_help_home
	ui->screen_help_home = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->screen_help_home, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_help_home_main_main_default
	static lv_style_t style_screen_help_home_main_main_default;
	if (style_screen_help_home_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_main_main_default);
	else
		lv_style_init(&style_screen_help_home_main_main_default);
	lv_style_set_bg_color(&style_screen_help_home_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_screen_help_home_main_main_default, 0);
	lv_obj_add_style(ui->screen_help_home, &style_screen_help_home_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_help_home_cont_background
	ui->screen_help_home_cont_background = lv_obj_create(ui->screen_help_home);
	lv_obj_set_pos(ui->screen_help_home_cont_background, 0, 0);
	lv_obj_set_size(ui->screen_help_home_cont_background, 1280, 720);
	lv_obj_set_scrollbar_mode(ui->screen_help_home_cont_background, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_help_home_cont_background_main_main_default
	static lv_style_t style_screen_help_home_cont_background_main_main_default;
	if (style_screen_help_home_cont_background_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_cont_background_main_main_default);
	else
		lv_style_init(&style_screen_help_home_cont_background_main_main_default);
	lv_style_set_radius(&style_screen_help_home_cont_background_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_help_home_cont_background_main_main_default, lv_color_make(0xfc, 0xfc, 0xfc));
	lv_style_set_bg_grad_color(&style_screen_help_home_cont_background_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_help_home_cont_background_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_help_home_cont_background_main_main_default, 255);
	lv_style_set_shadow_width(&style_screen_help_home_cont_background_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_help_home_cont_background_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_help_home_cont_background_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_help_home_cont_background_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_help_home_cont_background_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_help_home_cont_background_main_main_default, 0);
	lv_style_set_border_color(&style_screen_help_home_cont_background_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_screen_help_home_cont_background_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_help_home_cont_background_main_main_default, 255);
	lv_style_set_pad_left(&style_screen_help_home_cont_background_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_help_home_cont_background_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_help_home_cont_background_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_help_home_cont_background_main_main_default, 0);
	lv_obj_add_style(ui->screen_help_home_cont_background, &style_screen_help_home_cont_background_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_help_home_img_background
	ui->screen_help_home_img_background = lv_img_create(ui->screen_help_home_cont_background);
	lv_obj_set_pos(ui->screen_help_home_img_background, -1, 0);
	lv_obj_set_size(ui->screen_help_home_img_background, 1280, 630);
	lv_obj_set_scrollbar_mode(ui->screen_help_home_img_background, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_help_home_img_background_main_main_default
	static lv_style_t style_screen_help_home_img_background_main_main_default;
	if (style_screen_help_home_img_background_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_img_background_main_main_default);
	else
		lv_style_init(&style_screen_help_home_img_background_main_main_default);
	lv_style_set_img_recolor(&style_screen_help_home_img_background_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_help_home_img_background_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_help_home_img_background_main_main_default, 255);
	lv_obj_add_style(ui->screen_help_home_img_background, &style_screen_help_home_img_background_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_help_home_img_background, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_help_home_img_background,&_main_background_1280x630);
	lv_img_set_pivot(ui->screen_help_home_img_background, 0,0);
	lv_img_set_angle(ui->screen_help_home_img_background, 0);

	//Write codes screen_help_home_img_background_bottom
	ui->screen_help_home_img_background_bottom = lv_img_create(ui->screen_help_home_cont_background);
	lv_obj_set_pos(ui->screen_help_home_img_background_bottom, 0, 630);
	lv_obj_set_size(ui->screen_help_home_img_background_bottom, 1280, 90);
	lv_obj_set_scrollbar_mode(ui->screen_help_home_img_background_bottom, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_help_home_img_background_bottom_main_main_default
	static lv_style_t style_screen_help_home_img_background_bottom_main_main_default;
	if (style_screen_help_home_img_background_bottom_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_img_background_bottom_main_main_default);
	else
		lv_style_init(&style_screen_help_home_img_background_bottom_main_main_default);
	lv_style_set_img_recolor(&style_screen_help_home_img_background_bottom_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_help_home_img_background_bottom_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_help_home_img_background_bottom_main_main_default, 255);
	lv_obj_add_style(ui->screen_help_home_img_background_bottom, &style_screen_help_home_img_background_bottom_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_help_home_img_background_bottom, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_help_home_img_background_bottom,&_other_background_bottom_1280x90);
	lv_img_set_pivot(ui->screen_help_home_img_background_bottom, 50,50);
	lv_img_set_angle(ui->screen_help_home_img_background_bottom, 0);

	//Write codes screen_help_home_label_title
	ui->screen_help_home_label_title = lv_label_create(ui->screen_help_home_cont_background);
	lv_obj_set_pos(ui->screen_help_home_label_title, 390, 29);
	lv_obj_set_size(ui->screen_help_home_label_title, 500, 60);
	lv_obj_set_scrollbar_mode(ui->screen_help_home_label_title, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_help_home_label_title, "Monday, May 18");
	lv_label_set_long_mode(ui->screen_help_home_label_title, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_help_home_label_title_main_main_default
	static lv_style_t style_screen_help_home_label_title_main_main_default;
	if (style_screen_help_home_label_title_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_label_title_main_main_default);
	else
		lv_style_init(&style_screen_help_home_label_title_main_main_default);
	lv_style_set_radius(&style_screen_help_home_label_title_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_help_home_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_help_home_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_help_home_label_title_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_help_home_label_title_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_help_home_label_title_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_help_home_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_help_home_label_title_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_help_home_label_title_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_help_home_label_title_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_help_home_label_title_main_main_default, 0);
	lv_style_set_text_color(&style_screen_help_home_label_title_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_help_home_label_title_main_main_default, &lv_font_montserratMedium_50);
	lv_style_set_text_letter_space(&style_screen_help_home_label_title_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_help_home_label_title_main_main_default, 0);
	lv_style_set_text_align(&style_screen_help_home_label_title_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_help_home_label_title_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_help_home_label_title_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_help_home_label_title_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_help_home_label_title_main_main_default, 0);
	lv_obj_add_style(ui->screen_help_home_label_title, &style_screen_help_home_label_title_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_help_home_imgbtn_home
	ui->screen_help_home_imgbtn_home = lv_imgbtn_create(ui->screen_help_home_cont_background);
	lv_obj_set_pos(ui->screen_help_home_imgbtn_home, 24, 26);
	lv_obj_set_size(ui->screen_help_home_imgbtn_home, 85, 70);
	lv_obj_set_scrollbar_mode(ui->screen_help_home_imgbtn_home, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_help_home_imgbtn_home, LV_IMGBTN_STATE_RELEASED, NULL, &_home_85x70, NULL);

	//Write codes screen_help_home_ddlist_language
	ui->screen_help_home_ddlist_language = lv_dropdown_create(ui->screen_help_home_cont_background);
	lv_obj_set_pos(ui->screen_help_home_ddlist_language, 1020, 30);
	lv_obj_set_size(ui->screen_help_home_ddlist_language, 193, 50);
	lv_obj_set_scrollbar_mode(ui->screen_help_home_ddlist_language, LV_SCROLLBAR_MODE_OFF);
	lv_dropdown_set_options(ui->screen_help_home_ddlist_language, "English\n中文");

	//Write style state: LV_STATE_DEFAULT for style_screen_help_home_ddlist_language_main_main_default
	static lv_style_t style_screen_help_home_ddlist_language_main_main_default;
	if (style_screen_help_home_ddlist_language_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_ddlist_language_main_main_default);
	else
		lv_style_init(&style_screen_help_home_ddlist_language_main_main_default);
	lv_style_set_radius(&style_screen_help_home_ddlist_language_main_main_default, 3);
	lv_style_set_bg_color(&style_screen_help_home_ddlist_language_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_help_home_ddlist_language_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_help_home_ddlist_language_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_help_home_ddlist_language_main_main_default, 255);
	lv_style_set_shadow_width(&style_screen_help_home_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_help_home_ddlist_language_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_help_home_ddlist_language_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_help_home_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_help_home_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_help_home_ddlist_language_main_main_default, 0);
	lv_style_set_border_color(&style_screen_help_home_ddlist_language_main_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_help_home_ddlist_language_main_main_default, 1);
	lv_style_set_border_opa(&style_screen_help_home_ddlist_language_main_main_default, 255);
	lv_style_set_text_color(&style_screen_help_home_ddlist_language_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_help_home_ddlist_language_main_main_default, &lv_font_simsun_32);
	lv_style_set_pad_left(&style_screen_help_home_ddlist_language_main_main_default, 6);
	lv_style_set_pad_right(&style_screen_help_home_ddlist_language_main_main_default, 6);
	lv_style_set_pad_top(&style_screen_help_home_ddlist_language_main_main_default, 8);
	lv_obj_add_style(ui->screen_help_home_ddlist_language, &style_screen_help_home_ddlist_language_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_CHECKED for style_screen_help_home_ddlist_language_extra_list_selected_checked
	static lv_style_t style_screen_help_home_ddlist_language_extra_list_selected_checked;
	if (style_screen_help_home_ddlist_language_extra_list_selected_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_ddlist_language_extra_list_selected_checked);
	else
		lv_style_init(&style_screen_help_home_ddlist_language_extra_list_selected_checked);
	lv_style_set_radius(&style_screen_help_home_ddlist_language_extra_list_selected_checked, 3);
	lv_style_set_bg_color(&style_screen_help_home_ddlist_language_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_color(&style_screen_help_home_ddlist_language_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_screen_help_home_ddlist_language_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_help_home_ddlist_language_extra_list_selected_checked, 255);
	lv_style_set_border_color(&style_screen_help_home_ddlist_language_extra_list_selected_checked, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_help_home_ddlist_language_extra_list_selected_checked, 1);
	lv_style_set_border_opa(&style_screen_help_home_ddlist_language_extra_list_selected_checked, 255);
	lv_style_set_text_color(&style_screen_help_home_ddlist_language_extra_list_selected_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_help_home_ddlist_language_extra_list_selected_checked, &lv_font_simsun_32);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_help_home_ddlist_language), &style_screen_help_home_ddlist_language_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Write style state: LV_STATE_DEFAULT for style_screen_help_home_ddlist_language_extra_list_main_default
	static lv_style_t style_screen_help_home_ddlist_language_extra_list_main_default;
	if (style_screen_help_home_ddlist_language_extra_list_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_ddlist_language_extra_list_main_default);
	else
		lv_style_init(&style_screen_help_home_ddlist_language_extra_list_main_default);
	lv_style_set_radius(&style_screen_help_home_ddlist_language_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_screen_help_home_ddlist_language_extra_list_main_default, lv_color_make(0x8c, 0xa0, 0xa0));
	lv_style_set_bg_grad_color(&style_screen_help_home_ddlist_language_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_help_home_ddlist_language_extra_list_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_help_home_ddlist_language_extra_list_main_default, 255);
	lv_style_set_border_color(&style_screen_help_home_ddlist_language_extra_list_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_help_home_ddlist_language_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_screen_help_home_ddlist_language_extra_list_main_default, 255);
	lv_style_set_text_color(&style_screen_help_home_ddlist_language_extra_list_main_default, lv_color_make(0x0D, 0x30, 0x55));
	lv_style_set_text_font(&style_screen_help_home_ddlist_language_extra_list_main_default, &lv_font_simsun_32);
	lv_style_set_max_height(&style_screen_help_home_ddlist_language_extra_list_main_default, 150);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_help_home_ddlist_language), &style_screen_help_home_ddlist_language_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_help_home_ddlist_language_extra_list_scrollbar_default
	static lv_style_t style_screen_help_home_ddlist_language_extra_list_scrollbar_default;
	if (style_screen_help_home_ddlist_language_extra_list_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_ddlist_language_extra_list_scrollbar_default);
	else
		lv_style_init(&style_screen_help_home_ddlist_language_extra_list_scrollbar_default);
	lv_style_set_radius(&style_screen_help_home_ddlist_language_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_screen_help_home_ddlist_language_extra_list_scrollbar_default, lv_color_make(0xaf, 0xc3, 0xc3));
	lv_style_set_bg_opa(&style_screen_help_home_ddlist_language_extra_list_scrollbar_default, 255);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_help_home_ddlist_language), &style_screen_help_home_ddlist_language_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes screen_help_home_label_wakeword_en
	ui->screen_help_home_label_wakeword_en = lv_label_create(ui->screen_help_home_cont_background);
	lv_obj_set_pos(ui->screen_help_home_label_wakeword_en, 88, 166);
	lv_obj_set_size(ui->screen_help_home_label_wakeword_en, 414, 184);
	lv_obj_set_scrollbar_mode(ui->screen_help_home_label_wakeword_en, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_help_home_label_wakeword_en, "Say \"Hey NXP\" \nto wake up the\nvoice command");
	lv_label_set_long_mode(ui->screen_help_home_label_wakeword_en, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_help_home_label_wakeword_en_main_main_default
	static lv_style_t style_screen_help_home_label_wakeword_en_main_main_default;
	if (style_screen_help_home_label_wakeword_en_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_label_wakeword_en_main_main_default);
	else
		lv_style_init(&style_screen_help_home_label_wakeword_en_main_main_default);
	lv_style_set_radius(&style_screen_help_home_label_wakeword_en_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_help_home_label_wakeword_en_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_help_home_label_wakeword_en_main_main_default, lv_color_make(0xBE, 0xAF, 0x14));
	lv_style_set_bg_grad_dir(&style_screen_help_home_label_wakeword_en_main_main_default, LV_GRAD_DIR_HOR);
	lv_style_set_bg_opa(&style_screen_help_home_label_wakeword_en_main_main_default, 45);
	lv_style_set_shadow_width(&style_screen_help_home_label_wakeword_en_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_help_home_label_wakeword_en_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_help_home_label_wakeword_en_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_help_home_label_wakeword_en_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_help_home_label_wakeword_en_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_help_home_label_wakeword_en_main_main_default, 0);
	lv_style_set_text_color(&style_screen_help_home_label_wakeword_en_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_help_home_label_wakeword_en_main_main_default, &lv_font_montserratMedium_38);
	lv_style_set_text_letter_space(&style_screen_help_home_label_wakeword_en_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_help_home_label_wakeword_en_main_main_default, 17);
	lv_style_set_text_align(&style_screen_help_home_label_wakeword_en_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_help_home_label_wakeword_en_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_help_home_label_wakeword_en_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_help_home_label_wakeword_en_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_help_home_label_wakeword_en_main_main_default, 0);
	lv_obj_add_style(ui->screen_help_home_label_wakeword_en, &style_screen_help_home_label_wakeword_en_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_help_home_label_voicecmd_en
	ui->screen_help_home_label_voicecmd_en = lv_label_create(ui->screen_help_home_cont_background);
	lv_obj_set_pos(ui->screen_help_home_label_voicecmd_en, 650, 116);
	lv_obj_set_size(ui->screen_help_home_label_voicecmd_en, 366, 58);
	lv_obj_set_scrollbar_mode(ui->screen_help_home_label_voicecmd_en, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_help_home_label_voicecmd_en, "Voice commands");
	lv_label_set_long_mode(ui->screen_help_home_label_voicecmd_en, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_help_home_label_voicecmd_en_main_main_default
	static lv_style_t style_screen_help_home_label_voicecmd_en_main_main_default;
	if (style_screen_help_home_label_voicecmd_en_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_label_voicecmd_en_main_main_default);
	else
		lv_style_init(&style_screen_help_home_label_voicecmd_en_main_main_default);
	lv_style_set_radius(&style_screen_help_home_label_voicecmd_en_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_help_home_label_voicecmd_en_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_help_home_label_voicecmd_en_main_main_default, lv_color_make(0xBE, 0xAF, 0x14));
	lv_style_set_bg_grad_dir(&style_screen_help_home_label_voicecmd_en_main_main_default, LV_GRAD_DIR_HOR);
	lv_style_set_bg_opa(&style_screen_help_home_label_voicecmd_en_main_main_default, 45);
	lv_style_set_shadow_width(&style_screen_help_home_label_voicecmd_en_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_help_home_label_voicecmd_en_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_help_home_label_voicecmd_en_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_help_home_label_voicecmd_en_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_help_home_label_voicecmd_en_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_help_home_label_voicecmd_en_main_main_default, 0);
	lv_style_set_text_color(&style_screen_help_home_label_voicecmd_en_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_help_home_label_voicecmd_en_main_main_default, &lv_font_montserratMedium_38);
	lv_style_set_text_letter_space(&style_screen_help_home_label_voicecmd_en_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_help_home_label_voicecmd_en_main_main_default, 17);
	lv_style_set_text_align(&style_screen_help_home_label_voicecmd_en_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_help_home_label_voicecmd_en_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_help_home_label_voicecmd_en_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_help_home_label_voicecmd_en_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_help_home_label_voicecmd_en_main_main_default, 0);
	lv_obj_add_style(ui->screen_help_home_label_voicecmd_en, &style_screen_help_home_label_voicecmd_en_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_help_home_label_cmdlist_en
	ui->screen_help_home_label_cmdlist_en = lv_label_create(ui->screen_help_home_cont_background);
	lv_obj_set_pos(ui->screen_help_home_label_cmdlist_en, 650, 210);
	lv_obj_set_size(ui->screen_help_home_label_cmdlist_en, 394, 220);
	lv_obj_set_scrollbar_mode(ui->screen_help_home_label_cmdlist_en, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_help_home_label_cmdlist_en, "Thermostat\nSecurity\nAudio Player");
	lv_label_set_long_mode(ui->screen_help_home_label_cmdlist_en, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_help_home_label_cmdlist_en_main_main_default
	static lv_style_t style_screen_help_home_label_cmdlist_en_main_main_default;
	if (style_screen_help_home_label_cmdlist_en_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_label_cmdlist_en_main_main_default);
	else
		lv_style_init(&style_screen_help_home_label_cmdlist_en_main_main_default);
	lv_style_set_radius(&style_screen_help_home_label_cmdlist_en_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_help_home_label_cmdlist_en_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_help_home_label_cmdlist_en_main_main_default, lv_color_make(0xBE, 0xAF, 0x14));
	lv_style_set_bg_grad_dir(&style_screen_help_home_label_cmdlist_en_main_main_default, LV_GRAD_DIR_HOR);
	lv_style_set_bg_opa(&style_screen_help_home_label_cmdlist_en_main_main_default, 45);
	lv_style_set_shadow_width(&style_screen_help_home_label_cmdlist_en_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_help_home_label_cmdlist_en_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_help_home_label_cmdlist_en_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_help_home_label_cmdlist_en_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_help_home_label_cmdlist_en_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_help_home_label_cmdlist_en_main_main_default, 0);
	lv_style_set_text_color(&style_screen_help_home_label_cmdlist_en_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_help_home_label_cmdlist_en_main_main_default, &lv_font_montserratMedium_38);
	lv_style_set_text_letter_space(&style_screen_help_home_label_cmdlist_en_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_help_home_label_cmdlist_en_main_main_default, 17);
	lv_style_set_text_align(&style_screen_help_home_label_cmdlist_en_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_help_home_label_cmdlist_en_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_help_home_label_cmdlist_en_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_help_home_label_cmdlist_en_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_help_home_label_cmdlist_en_main_main_default, 0);
	lv_obj_add_style(ui->screen_help_home_label_cmdlist_en, &style_screen_help_home_label_cmdlist_en_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_help_home_label_wakeword_cn
	ui->screen_help_home_label_wakeword_cn = lv_label_create(ui->screen_help_home_cont_background);
	lv_obj_set_pos(ui->screen_help_home_label_wakeword_cn, 88, 166);
	lv_obj_set_size(ui->screen_help_home_label_wakeword_cn, 468, 153);
	lv_obj_set_scrollbar_mode(ui->screen_help_home_label_wakeword_cn, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_help_home_label_wakeword_cn, "请说：“你好，恩智浦”\n来唤醒语音控制");
	lv_label_set_long_mode(ui->screen_help_home_label_wakeword_cn, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_help_home_label_wakeword_cn_main_main_default
	static lv_style_t style_screen_help_home_label_wakeword_cn_main_main_default;
	if (style_screen_help_home_label_wakeword_cn_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_label_wakeword_cn_main_main_default);
	else
		lv_style_init(&style_screen_help_home_label_wakeword_cn_main_main_default);
	lv_style_set_radius(&style_screen_help_home_label_wakeword_cn_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_help_home_label_wakeword_cn_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_help_home_label_wakeword_cn_main_main_default, lv_color_make(0xBE, 0xAF, 0x14));
	lv_style_set_bg_grad_dir(&style_screen_help_home_label_wakeword_cn_main_main_default, LV_GRAD_DIR_HOR);
	lv_style_set_bg_opa(&style_screen_help_home_label_wakeword_cn_main_main_default, 45);
	lv_style_set_shadow_width(&style_screen_help_home_label_wakeword_cn_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_help_home_label_wakeword_cn_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_help_home_label_wakeword_cn_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_help_home_label_wakeword_cn_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_help_home_label_wakeword_cn_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_help_home_label_wakeword_cn_main_main_default, 0);
	lv_style_set_text_color(&style_screen_help_home_label_wakeword_cn_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_help_home_label_wakeword_cn_main_main_default, &lv_font_simsun_40);
	lv_style_set_text_letter_space(&style_screen_help_home_label_wakeword_cn_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_help_home_label_wakeword_cn_main_main_default, 17);
	lv_style_set_text_align(&style_screen_help_home_label_wakeword_cn_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_help_home_label_wakeword_cn_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_help_home_label_wakeword_cn_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_help_home_label_wakeword_cn_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_help_home_label_wakeword_cn_main_main_default, 0);
	lv_obj_add_style(ui->screen_help_home_label_wakeword_cn, &style_screen_help_home_label_wakeword_cn_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_help_home_label_voicecmd_cn
	ui->screen_help_home_label_voicecmd_cn = lv_label_create(ui->screen_help_home_cont_background);
	lv_obj_set_pos(ui->screen_help_home_label_voicecmd_cn, 650, 116);
	lv_obj_set_size(ui->screen_help_home_label_voicecmd_cn, 366, 58);
	lv_obj_set_scrollbar_mode(ui->screen_help_home_label_voicecmd_cn, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_help_home_label_voicecmd_cn, "语音命令");
	lv_label_set_long_mode(ui->screen_help_home_label_voicecmd_cn, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_help_home_label_voicecmd_cn_main_main_default
	static lv_style_t style_screen_help_home_label_voicecmd_cn_main_main_default;
	if (style_screen_help_home_label_voicecmd_cn_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_label_voicecmd_cn_main_main_default);
	else
		lv_style_init(&style_screen_help_home_label_voicecmd_cn_main_main_default);
	lv_style_set_radius(&style_screen_help_home_label_voicecmd_cn_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_help_home_label_voicecmd_cn_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_help_home_label_voicecmd_cn_main_main_default, lv_color_make(0xBE, 0xAF, 0x14));
	lv_style_set_bg_grad_dir(&style_screen_help_home_label_voicecmd_cn_main_main_default, LV_GRAD_DIR_HOR);
	lv_style_set_bg_opa(&style_screen_help_home_label_voicecmd_cn_main_main_default, 45);
	lv_style_set_shadow_width(&style_screen_help_home_label_voicecmd_cn_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_help_home_label_voicecmd_cn_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_help_home_label_voicecmd_cn_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_help_home_label_voicecmd_cn_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_help_home_label_voicecmd_cn_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_help_home_label_voicecmd_cn_main_main_default, 0);
	lv_style_set_text_color(&style_screen_help_home_label_voicecmd_cn_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_help_home_label_voicecmd_cn_main_main_default, &lv_font_simsun_40);
	lv_style_set_text_letter_space(&style_screen_help_home_label_voicecmd_cn_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_help_home_label_voicecmd_cn_main_main_default, 17);
	lv_style_set_text_align(&style_screen_help_home_label_voicecmd_cn_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_help_home_label_voicecmd_cn_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_help_home_label_voicecmd_cn_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_help_home_label_voicecmd_cn_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_help_home_label_voicecmd_cn_main_main_default, 0);
	lv_obj_add_style(ui->screen_help_home_label_voicecmd_cn, &style_screen_help_home_label_voicecmd_cn_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_help_home_label_cmdlist_cn
	ui->screen_help_home_label_cmdlist_cn = lv_label_create(ui->screen_help_home_cont_background);
	lv_obj_set_pos(ui->screen_help_home_label_cmdlist_cn, 650, 210);
	lv_obj_set_size(ui->screen_help_home_label_cmdlist_cn, 394, 220);
	lv_obj_set_scrollbar_mode(ui->screen_help_home_label_cmdlist_cn, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_help_home_label_cmdlist_cn, "温控系统\n安防系统\n娱乐系统\n");
	lv_label_set_long_mode(ui->screen_help_home_label_cmdlist_cn, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_help_home_label_cmdlist_cn_main_main_default
	static lv_style_t style_screen_help_home_label_cmdlist_cn_main_main_default;
	if (style_screen_help_home_label_cmdlist_cn_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_help_home_label_cmdlist_cn_main_main_default);
	else
		lv_style_init(&style_screen_help_home_label_cmdlist_cn_main_main_default);
	lv_style_set_radius(&style_screen_help_home_label_cmdlist_cn_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_help_home_label_cmdlist_cn_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_help_home_label_cmdlist_cn_main_main_default, lv_color_make(0xBE, 0xAF, 0x14));
	lv_style_set_bg_grad_dir(&style_screen_help_home_label_cmdlist_cn_main_main_default, LV_GRAD_DIR_HOR);
	lv_style_set_bg_opa(&style_screen_help_home_label_cmdlist_cn_main_main_default, 45);
	lv_style_set_shadow_width(&style_screen_help_home_label_cmdlist_cn_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_help_home_label_cmdlist_cn_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_help_home_label_cmdlist_cn_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_help_home_label_cmdlist_cn_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_help_home_label_cmdlist_cn_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_help_home_label_cmdlist_cn_main_main_default, 0);
	lv_style_set_text_color(&style_screen_help_home_label_cmdlist_cn_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_help_home_label_cmdlist_cn_main_main_default, &lv_font_simsun_40);
	lv_style_set_text_letter_space(&style_screen_help_home_label_cmdlist_cn_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_help_home_label_cmdlist_cn_main_main_default, 17);
	lv_style_set_text_align(&style_screen_help_home_label_cmdlist_cn_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_help_home_label_cmdlist_cn_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_help_home_label_cmdlist_cn_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_help_home_label_cmdlist_cn_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_help_home_label_cmdlist_cn_main_main_default, 0);
	lv_obj_add_style(ui->screen_help_home_label_cmdlist_cn, &style_screen_help_home_label_cmdlist_cn_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Init events for screen
	events_init_screen_help_home(ui);
}