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

static void _home_ddlist_languageSel_event_cb(lv_event_t * e)
{
	lv_obj_t * obj = lv_event_get_target(e);

	//Write style state: LV_STATE_DEFAULT for style_home_ddlist_languagesel_extra_list_selected_default
	static lv_style_t style_home_ddlist_languagesel_extra_list_selected_default;
	if (style_home_ddlist_languagesel_extra_list_selected_default.prop_cnt > 1)
		lv_style_reset(&style_home_ddlist_languagesel_extra_list_selected_default);
	else
		lv_style_init(&style_home_ddlist_languagesel_extra_list_selected_default);
	lv_style_set_radius(&style_home_ddlist_languagesel_extra_list_selected_default, 3);
	lv_style_set_bg_color(&style_home_ddlist_languagesel_extra_list_selected_default, lv_color_make(0x46, 0x5a, 0x5a));
	lv_style_set_bg_grad_color(&style_home_ddlist_languagesel_extra_list_selected_default, lv_color_make(0x46, 0x5a, 0x5a));
	lv_style_set_bg_grad_dir(&style_home_ddlist_languagesel_extra_list_selected_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_ddlist_languagesel_extra_list_selected_default, 255);
	lv_style_set_border_color(&style_home_ddlist_languagesel_extra_list_selected_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_home_ddlist_languagesel_extra_list_selected_default, 1);
	lv_style_set_text_color(&style_home_ddlist_languagesel_extra_list_selected_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_ddlist_languagesel_extra_list_selected_default, &lv_font_simsun_32);
	lv_obj_add_style(lv_dropdown_get_list(obj), &style_home_ddlist_languagesel_extra_list_selected_default, LV_PART_SELECTED|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_home_ddlist_languagesel_extra_list_main_default
	static lv_style_t style_home_ddlist_languagesel_extra_list_main_default;
	if (style_home_ddlist_languagesel_extra_list_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_ddlist_languagesel_extra_list_main_default);
	else
		lv_style_init(&style_home_ddlist_languagesel_extra_list_main_default);
	lv_style_set_radius(&style_home_ddlist_languagesel_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_home_ddlist_languagesel_extra_list_main_default, lv_color_make(0x8c, 0xa0, 0xa0));
	lv_style_set_bg_grad_color(&style_home_ddlist_languagesel_extra_list_main_default, lv_color_make(0x69, 0x7d, 0x7d));
	lv_style_set_bg_grad_dir(&style_home_ddlist_languagesel_extra_list_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_ddlist_languagesel_extra_list_main_default, 255);
	lv_style_set_border_color(&style_home_ddlist_languagesel_extra_list_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_home_ddlist_languagesel_extra_list_main_default, 1);
	lv_style_set_text_color(&style_home_ddlist_languagesel_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_ddlist_languagesel_extra_list_main_default, &lv_font_simsun_32);
	/* Must modify the dropbox height if removing the French language option */
#if FRENCH_LANG_SUPPORTED
	lv_style_set_max_height(&style_home_ddlist_languagesel_extra_list_main_default, 220);
#else
	lv_style_set_max_height(&style_home_ddlist_languagesel_extra_list_main_default, 165);
#endif /* FRENCH_LANG_SUPPORTED */
	lv_obj_add_style(lv_dropdown_get_list(obj), &style_home_ddlist_languagesel_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_home_ddlist_languagesel_extra_list_scrollbar_default
	static lv_style_t style_home_ddlist_languagesel_extra_list_scrollbar_default;
	if (style_home_ddlist_languagesel_extra_list_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_home_ddlist_languagesel_extra_list_scrollbar_default);
	else
		lv_style_init(&style_home_ddlist_languagesel_extra_list_scrollbar_default);
	lv_style_set_radius(&style_home_ddlist_languagesel_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_home_ddlist_languagesel_extra_list_scrollbar_default, lv_color_make(0xaf, 0xc3, 0xc3));
	lv_style_set_bg_grad_color(&style_home_ddlist_languagesel_extra_list_scrollbar_default, lv_color_make(0xaf, 0xc3, 0xc3));
	lv_style_set_bg_grad_dir(&style_home_ddlist_languagesel_extra_list_scrollbar_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_ddlist_languagesel_extra_list_scrollbar_default, 0);
	lv_obj_add_style(lv_dropdown_get_list(obj), &style_home_ddlist_languagesel_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
}

void setup_scr_home(lv_ui *ui){

	//Write codes home
	ui->home = lv_obj_create(NULL);

	//Write style state: LV_STATE_DEFAULT for style_home_main_main_default
	static lv_style_t style_home_main_main_default;
	if (style_home_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_main_main_default);
	else
		lv_style_init(&style_home_main_main_default);
	lv_style_set_bg_color(&style_home_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_home_main_main_default, 0);
	lv_obj_add_style(ui->home, &style_home_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_img_background
	ui->home_img_background = lv_img_create(ui->home);
	lv_obj_set_pos(ui->home_img_background, 0, 0);
	lv_obj_set_size(ui->home_img_background, 1280, 720);

	//Write style state: LV_STATE_DEFAULT for style_home_img_background_main_main_default
	static lv_style_t style_home_img_background_main_main_default;
	if (style_home_img_background_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_background_main_main_default);
	else
		lv_style_init(&style_home_img_background_main_main_default);
	lv_style_set_img_recolor(&style_home_img_background_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_background_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_background_main_main_default, 255);
	lv_obj_add_style(ui->home_img_background, &style_home_img_background_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->home_img_background, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->home_img_background,&_home_background_1280x720);
	lv_img_set_pivot(ui->home_img_background, 0,0);
	lv_img_set_angle(ui->home_img_background, 0);

	//Write codes home_imgbtn_1
	ui->home_imgbtn_1 = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_1, 913, 452);
	lv_obj_set_size(ui->home_imgbtn_1, 90, 90);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_1_main_main_default
	static lv_style_t style_home_imgbtn_1_main_main_default;
	if (style_home_imgbtn_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_1_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_1_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_1_main_main_default, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_1_main_main_default, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_1_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_1_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_1, &style_home_imgbtn_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_1_main_main_pressed
	static lv_style_t style_home_imgbtn_1_main_main_pressed;
	if (style_home_imgbtn_1_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_1_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_1_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_1_main_main_pressed, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_1_main_main_pressed, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_1_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_1_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_1, &style_home_imgbtn_1_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_1_main_main_checked
	static lv_style_t style_home_imgbtn_1_main_main_checked;
	if (style_home_imgbtn_1_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_1_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_1_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_1_main_main_checked, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_1_main_main_checked, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_1_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_1_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_1, &style_home_imgbtn_1_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_1, LV_IMGBTN_STATE_RELEASED, NULL, &_button_silver_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_1, LV_IMGBTN_STATE_PRESSED, NULL, &_button_silver_pressed_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_1, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_button_gold_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_1, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_button_gold_pressed_alpha_90x90, NULL);
	lv_obj_add_flag(ui->home_imgbtn_1, LV_OBJ_FLAG_CHECKABLE);
	ui->home_imgbtn_1_label = lv_label_create(ui->home_imgbtn_1);
	lv_label_set_text(ui->home_imgbtn_1_label, "1");
	lv_obj_set_style_pad_all(ui->home_imgbtn_1, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->home_imgbtn_1_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes home_imgbtn_2
	ui->home_imgbtn_2 = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_2, 1048, 452);
	lv_obj_set_size(ui->home_imgbtn_2, 90, 90);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_2_main_main_default
	static lv_style_t style_home_imgbtn_2_main_main_default;
	if (style_home_imgbtn_2_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_2_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_2_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_2_main_main_default, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_2_main_main_default, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_2_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_2_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_2_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_2, &style_home_imgbtn_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_2_main_main_pressed
	static lv_style_t style_home_imgbtn_2_main_main_pressed;
	if (style_home_imgbtn_2_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_2_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_2_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_2_main_main_pressed, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_2_main_main_pressed, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_2_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_2_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_2, &style_home_imgbtn_2_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_2_main_main_checked
	static lv_style_t style_home_imgbtn_2_main_main_checked;
	if (style_home_imgbtn_2_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_2_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_2_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_2_main_main_checked, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_2_main_main_checked, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_2_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_2_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_2, &style_home_imgbtn_2_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_2, LV_IMGBTN_STATE_RELEASED, NULL, &_button_silver_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_2, LV_IMGBTN_STATE_PRESSED, NULL, &_button_silver_pressed_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_2, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_button_gold_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_2, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_button_gold_pressed_alpha_90x90, NULL);
	lv_obj_add_flag(ui->home_imgbtn_2, LV_OBJ_FLAG_CHECKABLE);
	ui->home_imgbtn_2_label = lv_label_create(ui->home_imgbtn_2);
	lv_label_set_text(ui->home_imgbtn_2_label, "2");
	lv_obj_set_style_pad_all(ui->home_imgbtn_2, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->home_imgbtn_2_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes home_imgbtn_3
	ui->home_imgbtn_3 = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_3, 913, 318);
	lv_obj_set_size(ui->home_imgbtn_3, 90, 90);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_3_main_main_default
	static lv_style_t style_home_imgbtn_3_main_main_default;
	if (style_home_imgbtn_3_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_3_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_3_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_3_main_main_default, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_3_main_main_default, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_3_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_3_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_3_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_3, &style_home_imgbtn_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_3_main_main_pressed
	static lv_style_t style_home_imgbtn_3_main_main_pressed;
	if (style_home_imgbtn_3_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_3_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_3_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_3_main_main_pressed, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_3_main_main_pressed, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_3_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_3_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_3, &style_home_imgbtn_3_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_3_main_main_checked
	static lv_style_t style_home_imgbtn_3_main_main_checked;
	if (style_home_imgbtn_3_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_3_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_3_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_3_main_main_checked, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_3_main_main_checked, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_3_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_3_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_3, &style_home_imgbtn_3_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_3, LV_IMGBTN_STATE_RELEASED, NULL, &_button_silver_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_3, LV_IMGBTN_STATE_PRESSED, NULL, &_button_silver_pressed_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_3, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_button_gold_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_3, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_button_gold_pressed_alpha_90x90, NULL);
	lv_obj_add_flag(ui->home_imgbtn_3, LV_OBJ_FLAG_CHECKABLE);
	ui->home_imgbtn_3_label = lv_label_create(ui->home_imgbtn_3);
	lv_label_set_text(ui->home_imgbtn_3_label, "3");
	lv_obj_set_style_pad_all(ui->home_imgbtn_3, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->home_imgbtn_3_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes home_imgbtn_4
	ui->home_imgbtn_4 = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_4, 1048, 318);
	lv_obj_set_size(ui->home_imgbtn_4, 90, 90);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_4_main_main_default
	static lv_style_t style_home_imgbtn_4_main_main_default;
	if (style_home_imgbtn_4_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_4_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_4_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_4_main_main_default, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_4_main_main_default, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_4_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_4_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_4_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_4, &style_home_imgbtn_4_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_4_main_main_pressed
	static lv_style_t style_home_imgbtn_4_main_main_pressed;
	if (style_home_imgbtn_4_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_4_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_4_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_4_main_main_pressed, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_4_main_main_pressed, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_4_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_4_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_4, &style_home_imgbtn_4_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_4_main_main_checked
	static lv_style_t style_home_imgbtn_4_main_main_checked;
	if (style_home_imgbtn_4_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_4_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_4_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_4_main_main_checked, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_4_main_main_checked, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_4_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_4_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_4, &style_home_imgbtn_4_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_4, LV_IMGBTN_STATE_RELEASED, NULL, &_button_silver_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_4, LV_IMGBTN_STATE_PRESSED, NULL, &_button_silver_pressed_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_4, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_button_gold_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_4, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_button_gold_pressed_alpha_90x90, NULL);
	lv_obj_add_flag(ui->home_imgbtn_4, LV_OBJ_FLAG_CHECKABLE);
	ui->home_imgbtn_4_label = lv_label_create(ui->home_imgbtn_4);
	lv_label_set_text(ui->home_imgbtn_4_label, "4");
	lv_obj_set_style_pad_all(ui->home_imgbtn_4, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->home_imgbtn_4_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes home_imgbtn_5
	ui->home_imgbtn_5 = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_5, 913, 183);
	lv_obj_set_size(ui->home_imgbtn_5, 90, 90);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_5_main_main_default
	static lv_style_t style_home_imgbtn_5_main_main_default;
	if (style_home_imgbtn_5_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_5_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_5_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_5_main_main_default, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_5_main_main_default, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_5_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_5_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_5_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_5, &style_home_imgbtn_5_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_5_main_main_pressed
	static lv_style_t style_home_imgbtn_5_main_main_pressed;
	if (style_home_imgbtn_5_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_5_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_5_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_5_main_main_pressed, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_5_main_main_pressed, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_5_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_5_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_5, &style_home_imgbtn_5_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_5_main_main_checked
	static lv_style_t style_home_imgbtn_5_main_main_checked;
	if (style_home_imgbtn_5_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_5_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_5_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_5_main_main_checked, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_5_main_main_checked, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_5_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_5_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_5, &style_home_imgbtn_5_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_5, LV_IMGBTN_STATE_RELEASED, NULL, &_button_silver_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_5, LV_IMGBTN_STATE_PRESSED, NULL, &_button_silver_pressed_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_5, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_button_gold_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_5, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_button_gold_pressed_alpha_90x90, NULL);
	lv_obj_add_flag(ui->home_imgbtn_5, LV_OBJ_FLAG_CHECKABLE);
	ui->home_imgbtn_5_label = lv_label_create(ui->home_imgbtn_5);
	lv_label_set_text(ui->home_imgbtn_5_label, "5");
	lv_obj_set_style_pad_all(ui->home_imgbtn_5, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->home_imgbtn_5_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes home_imgbtn_6
	ui->home_imgbtn_6 = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_6, 1048, 183);
	lv_obj_set_size(ui->home_imgbtn_6, 90, 90);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_6_main_main_default
	static lv_style_t style_home_imgbtn_6_main_main_default;
	if (style_home_imgbtn_6_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_6_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_6_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_6_main_main_default, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_6_main_main_default, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_6_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_6_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_6_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_6, &style_home_imgbtn_6_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_6_main_main_pressed
	static lv_style_t style_home_imgbtn_6_main_main_pressed;
	if (style_home_imgbtn_6_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_6_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_6_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_6_main_main_pressed, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_6_main_main_pressed, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_6_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_6_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_6, &style_home_imgbtn_6_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_6_main_main_checked
	static lv_style_t style_home_imgbtn_6_main_main_checked;
	if (style_home_imgbtn_6_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_6_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_6_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_6_main_main_checked, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_text_font(&style_home_imgbtn_6_main_main_checked, &lv_font_arial_60);
	lv_style_set_img_recolor(&style_home_imgbtn_6_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_6_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_6, &style_home_imgbtn_6_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_6, LV_IMGBTN_STATE_RELEASED, NULL, &_button_silver_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_6, LV_IMGBTN_STATE_PRESSED, NULL, &_button_silver_pressed_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_6, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_button_gold_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_6, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_button_gold_pressed_alpha_90x90, NULL);
	lv_obj_add_flag(ui->home_imgbtn_6, LV_OBJ_FLAG_CHECKABLE);
	ui->home_imgbtn_6_label = lv_label_create(ui->home_imgbtn_6);
	lv_label_set_text(ui->home_imgbtn_6_label, "6");
	lv_obj_set_style_pad_all(ui->home_imgbtn_6, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->home_imgbtn_6_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes home_imgbtn_doorOpen
	ui->home_imgbtn_doorOpen = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_doorOpen, 839, 576);
	lv_obj_set_size(ui->home_imgbtn_doorOpen, 90, 90);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_dooropen_main_main_default
	static lv_style_t style_home_imgbtn_dooropen_main_main_default;
	if (style_home_imgbtn_dooropen_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_dooropen_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_dooropen_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_dooropen_main_main_default, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_img_recolor(&style_home_imgbtn_dooropen_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_dooropen_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_dooropen_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_doorOpen, &style_home_imgbtn_dooropen_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_dooropen_main_main_pressed
	static lv_style_t style_home_imgbtn_dooropen_main_main_pressed;
	if (style_home_imgbtn_dooropen_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_dooropen_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_dooropen_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_dooropen_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_dooropen_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_dooropen_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_doorOpen, &style_home_imgbtn_dooropen_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_dooropen_main_main_checked
	static lv_style_t style_home_imgbtn_dooropen_main_main_checked;
	if (style_home_imgbtn_dooropen_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_dooropen_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_dooropen_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_dooropen_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_dooropen_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_dooropen_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_doorOpen, &style_home_imgbtn_dooropen_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_doorOpen, LV_IMGBTN_STATE_RELEASED, NULL, &_button_open_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_doorOpen, LV_IMGBTN_STATE_PRESSED, NULL, &_button_open_pressed_alpha_90x90, NULL);

	//Write codes home_imgbtn_alarm
	ui->home_imgbtn_alarm = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_alarm, 982, 576);
	lv_obj_set_size(ui->home_imgbtn_alarm, 90, 90);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_alarm_main_main_default
	static lv_style_t style_home_imgbtn_alarm_main_main_default;
	if (style_home_imgbtn_alarm_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_alarm_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_alarm_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_alarm_main_main_default, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_img_recolor(&style_home_imgbtn_alarm_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_alarm_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_alarm_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_alarm, &style_home_imgbtn_alarm_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_alarm_main_main_pressed
	static lv_style_t style_home_imgbtn_alarm_main_main_pressed;
	if (style_home_imgbtn_alarm_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_alarm_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_alarm_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_alarm_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_alarm_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_alarm_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_alarm, &style_home_imgbtn_alarm_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_alarm_main_main_checked
	static lv_style_t style_home_imgbtn_alarm_main_main_checked;
	if (style_home_imgbtn_alarm_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_alarm_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_alarm_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_alarm_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_alarm_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_alarm_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_alarm, &style_home_imgbtn_alarm_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_alarm, LV_IMGBTN_STATE_RELEASED, NULL, &_button_call_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_alarm, LV_IMGBTN_STATE_PRESSED, NULL, &_button_call_pressed_alpha_90x90, NULL);

	//Write codes home_imgbtn_doorClose
	ui->home_imgbtn_doorClose = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_doorClose, 1125, 576);
	lv_obj_set_size(ui->home_imgbtn_doorClose, 90, 90);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_doorclose_main_main_default
	static lv_style_t style_home_imgbtn_doorclose_main_main_default;
	if (style_home_imgbtn_doorclose_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_doorclose_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_doorclose_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_doorclose_main_main_default, lv_color_make(0x31, 0x2f, 0x2f));
	lv_style_set_img_recolor(&style_home_imgbtn_doorclose_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_doorclose_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_doorclose_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_doorClose, &style_home_imgbtn_doorclose_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_doorclose_main_main_pressed
	static lv_style_t style_home_imgbtn_doorclose_main_main_pressed;
	if (style_home_imgbtn_doorclose_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_doorclose_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_doorclose_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_doorclose_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_doorclose_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_doorclose_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_doorClose, &style_home_imgbtn_doorclose_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_doorclose_main_main_checked
	static lv_style_t style_home_imgbtn_doorclose_main_main_checked;
	if (style_home_imgbtn_doorclose_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_doorclose_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_doorclose_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_doorclose_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_doorclose_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_doorclose_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_doorClose, &style_home_imgbtn_doorclose_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_doorClose, LV_IMGBTN_STATE_RELEASED, NULL, &_button_close_alpha_90x90, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_doorClose, LV_IMGBTN_STATE_PRESSED, NULL, &_button_close_pressed_alpha_90x90, NULL);

	//Write codes home_img_arrow
	ui->home_img_arrow = lv_img_create(ui->home);
	lv_obj_set_pos(ui->home_img_arrow, 935, 82);
	lv_obj_set_size(ui->home_img_arrow, 64, 56);

	//Write style state: LV_STATE_DEFAULT for style_home_img_arrow_main_main_default
	static lv_style_t style_home_img_arrow_main_main_default;
	if (style_home_img_arrow_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_arrow_main_main_default);
	else
		lv_style_init(&style_home_img_arrow_main_main_default);
	lv_style_set_img_recolor(&style_home_img_arrow_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_arrow_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_arrow_main_main_default, 255);
	lv_obj_add_style(ui->home_img_arrow, &style_home_img_arrow_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->home_img_arrow, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->home_img_arrow,&_up_64x56);
	lv_img_set_pivot(ui->home_img_arrow, 32,28);
	lv_img_set_angle(ui->home_img_arrow, 0);

	//Write codes home_roller_floorNum
	ui->home_roller_floorNum = lv_roller_create(ui->home);
	lv_obj_set_pos(ui->home_roller_floorNum, 982, 63);

	//Write style state: LV_STATE_DEFAULT for style_home_roller_floornum_main_main_default
	static lv_style_t style_home_roller_floornum_main_main_default;
	if (style_home_roller_floornum_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_roller_floornum_main_main_default);
	else
		lv_style_init(&style_home_roller_floornum_main_main_default);
	lv_style_set_radius(&style_home_roller_floornum_main_main_default, 0);
	lv_style_set_bg_color(&style_home_roller_floornum_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_home_roller_floornum_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_home_roller_floornum_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_roller_floornum_main_main_default, 1);
	lv_style_set_border_color(&style_home_roller_floornum_main_main_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_home_roller_floornum_main_main_default, 0);
	lv_style_set_text_color(&style_home_roller_floornum_main_main_default, lv_color_make(0x33, 0x33, 0x33));
	lv_obj_add_style(ui->home_roller_floorNum, &style_home_roller_floornum_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_home_roller_floornum_main_selected_default
	static lv_style_t style_home_roller_floornum_main_selected_default;
	if (style_home_roller_floornum_main_selected_default.prop_cnt > 1)
		lv_style_reset(&style_home_roller_floornum_main_selected_default);
	else
		lv_style_init(&style_home_roller_floornum_main_selected_default);
	lv_style_set_bg_color(&style_home_roller_floornum_main_selected_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_roller_floornum_main_selected_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_roller_floornum_main_selected_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_roller_floornum_main_selected_default, 0);
	lv_style_set_text_color(&style_home_roller_floornum_main_selected_default, lv_color_make(0xff, 0x00, 0x00));
	lv_obj_add_style(ui->home_roller_floorNum, &style_home_roller_floornum_main_selected_default, LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_roller_set_options(ui->home_roller_floorNum,"6\n5\n4\n3\n2\n1",LV_ROLLER_MODE_NORMAL);
	lv_obj_set_style_text_font(ui->home_roller_floorNum, &lv_font_arial_70, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->home_roller_floorNum, &lv_font_arial_70, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_text_font(ui->home_roller_floorNum, &lv_font_arial_70, LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_roller_set_visible_row_count(ui->home_roller_floorNum,1);

	//Write codes home_label_arrived
	ui->home_label_arrived = lv_label_create(ui->home);
	lv_obj_set_pos(ui->home_label_arrived, 921, 83);
	lv_obj_set_size(ui->home_label_arrived, 207, 62);
	lv_label_set_text(ui->home_label_arrived, "YOU HAVE ARRIVED");
	lv_label_set_long_mode(ui->home_label_arrived, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_arrived, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_arrived_main_main_default
	static lv_style_t style_home_label_arrived_main_main_default;
	if (style_home_label_arrived_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_arrived_main_main_default);
	else
		lv_style_init(&style_home_label_arrived_main_main_default);
	lv_style_set_radius(&style_home_label_arrived_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_arrived_main_main_default, lv_color_make(0x0e, 0x12, 0x13));
	lv_style_set_bg_grad_color(&style_home_label_arrived_main_main_default, lv_color_make(0x0e, 0x12, 0x13));
	lv_style_set_bg_grad_dir(&style_home_label_arrived_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_arrived_main_main_default, 255);
	lv_style_set_text_color(&style_home_label_arrived_main_main_default, lv_color_make(0x00, 0xff, 0x2a));
	lv_style_set_text_font(&style_home_label_arrived_main_main_default, &lv_font_montserratMedium_22);
	lv_style_set_text_letter_space(&style_home_label_arrived_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_arrived_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_arrived_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_arrived_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_arrived_main_main_default, 0);
	lv_obj_add_style(ui->home_label_arrived, &style_home_label_arrived_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DISABLED for style_home_label_arrived_main_main_disabled
	static lv_style_t style_home_label_arrived_main_main_disabled;
	if (style_home_label_arrived_main_main_disabled.prop_cnt > 1)
		lv_style_reset(&style_home_label_arrived_main_main_disabled);
	else
		lv_style_init(&style_home_label_arrived_main_main_disabled);
	lv_style_set_radius(&style_home_label_arrived_main_main_disabled, 0);
	lv_style_set_bg_color(&style_home_label_arrived_main_main_disabled, lv_color_make(0x0e, 0x12, 0x13));
	lv_style_set_bg_grad_color(&style_home_label_arrived_main_main_disabled, lv_color_make(0x0e, 0x12, 0x13));
	lv_style_set_bg_grad_dir(&style_home_label_arrived_main_main_disabled, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_arrived_main_main_disabled, 0);
	lv_style_set_text_color(&style_home_label_arrived_main_main_disabled, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_home_label_arrived_main_main_disabled, &lv_font_arial_30);
	lv_style_set_text_letter_space(&style_home_label_arrived_main_main_disabled, 2);
	lv_style_set_pad_left(&style_home_label_arrived_main_main_disabled, 0);
	lv_style_set_pad_right(&style_home_label_arrived_main_main_disabled, 0);
	lv_style_set_pad_top(&style_home_label_arrived_main_main_disabled, 0);
	lv_style_set_pad_bottom(&style_home_label_arrived_main_main_disabled, 0);
	lv_obj_add_style(ui->home_label_arrived, &style_home_label_arrived_main_main_disabled, LV_PART_MAIN|LV_STATE_DISABLED);

	//Write codes home_img_faceNotDetected
	ui->home_img_faceNotDetected = lv_img_create(ui->home);
	lv_obj_set_pos(ui->home_img_faceNotDetected, 510, 480);
	lv_obj_set_size(ui->home_img_faceNotDetected, 180, 185);

	//Write style state: LV_STATE_DEFAULT for style_home_img_facenotdetected_main_main_default
	static lv_style_t style_home_img_facenotdetected_main_main_default;
	if (style_home_img_facenotdetected_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_facenotdetected_main_main_default);
	else
		lv_style_init(&style_home_img_facenotdetected_main_main_default);
	lv_style_set_img_recolor(&style_home_img_facenotdetected_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_facenotdetected_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_facenotdetected_main_main_default, 255);
	lv_obj_add_style(ui->home_img_faceNotDetected, &style_home_img_facenotdetected_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->home_img_faceNotDetected, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->home_img_faceNotDetected,&_virtual_face_blue_180x185);
	lv_img_set_pivot(ui->home_img_faceNotDetected, 0,0);
	lv_img_set_angle(ui->home_img_faceNotDetected, 0);

	//Write codes home_img_faceDetected
	ui->home_img_faceDetected = lv_img_create(ui->home);
	lv_obj_set_pos(ui->home_img_faceDetected, 510, 480);
	lv_obj_set_size(ui->home_img_faceDetected, 180, 185);

	//Write style state: LV_STATE_DEFAULT for style_home_img_facedetected_main_main_default
	static lv_style_t style_home_img_facedetected_main_main_default;
	if (style_home_img_facedetected_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_facedetected_main_main_default);
	else
		lv_style_init(&style_home_img_facedetected_main_main_default);
	lv_style_set_img_recolor(&style_home_img_facedetected_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_facedetected_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_facedetected_main_main_default, 255);
	lv_obj_add_style(ui->home_img_faceDetected, &style_home_img_facedetected_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->home_img_faceDetected, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->home_img_faceDetected,&_virtual_face_red_180x185);
	lv_img_set_pivot(ui->home_img_faceDetected, 0,0);
	lv_img_set_angle(ui->home_img_faceDetected, 0);

	//Write codes home_img_faceRecognized
	ui->home_img_faceRecognized = lv_img_create(ui->home);
	lv_obj_set_pos(ui->home_img_faceRecognized, 510, 480);
	lv_obj_set_size(ui->home_img_faceRecognized, 180, 185);

	//Write style state: LV_STATE_DEFAULT for style_home_img_facerecognized_main_main_default
	static lv_style_t style_home_img_facerecognized_main_main_default;
	if (style_home_img_facerecognized_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_facerecognized_main_main_default);
	else
		lv_style_init(&style_home_img_facerecognized_main_main_default);
	lv_style_set_img_recolor(&style_home_img_facerecognized_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_facerecognized_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_facerecognized_main_main_default, 255);
	lv_obj_add_style(ui->home_img_faceRecognized, &style_home_img_facerecognized_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->home_img_faceRecognized, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->home_img_faceRecognized,&_virtual_face_green_180x185);
	lv_img_set_pivot(ui->home_img_faceRecognized, 0,0);
	lv_img_set_angle(ui->home_img_faceRecognized, 0);

	//Write codes home_imgbtn_deregister
	ui->home_imgbtn_deregister = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_deregister, 671, 466);
	lv_obj_set_size(ui->home_imgbtn_deregister, 62, 62);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_deregister_main_main_default
	static lv_style_t style_home_imgbtn_deregister_main_main_default;
	if (style_home_imgbtn_deregister_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_deregister_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_deregister_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_deregister_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_home_imgbtn_deregister_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_deregister_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_deregister_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_deregister, &style_home_imgbtn_deregister_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_deregister_main_main_pressed
	static lv_style_t style_home_imgbtn_deregister_main_main_pressed;
	if (style_home_imgbtn_deregister_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_deregister_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_deregister_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_deregister_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_deregister_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_deregister_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_deregister, &style_home_imgbtn_deregister_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_deregister_main_main_checked
	static lv_style_t style_home_imgbtn_deregister_main_main_checked;
	if (style_home_imgbtn_deregister_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_deregister_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_deregister_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_deregister_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_deregister_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_deregister_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_deregister, &style_home_imgbtn_deregister_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_deregister, LV_IMGBTN_STATE_RELEASED, NULL, &_button_delete_alpha_62x62, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_deregister, LV_IMGBTN_STATE_PRESSED, NULL, &_button_delete_pressed_alpha_62x62, NULL);
	lv_obj_add_flag(ui->home_imgbtn_deregister, LV_OBJ_FLAG_CHECKABLE);

	//Write codes home_ddlist_languageSel
	ui->home_ddlist_languageSel = lv_dropdown_create(ui->home);
	lv_obj_set_pos(ui->home_ddlist_languageSel, 43, 490);
	lv_obj_set_width(ui->home_ddlist_languageSel, 193);
	/* Must remove the French language option from the list if not supported */
#if FRENCH_LANG_SUPPORTED
	lv_dropdown_set_options(ui->home_ddlist_languageSel, "English\n中文\nDeutsch\nFrançais");
#else
	lv_dropdown_set_options(ui->home_ddlist_languageSel, "English\n中文\nDeutsch");
#endif /* FRENCH_LANG_SUPPORTED */
	lv_obj_add_event_cb(ui->home_ddlist_languageSel, _home_ddlist_languageSel_event_cb, LV_EVENT_READY, NULL);

	//Write style state: LV_STATE_DEFAULT for style_home_ddlist_languagesel_main_main_default
	static lv_style_t style_home_ddlist_languagesel_main_main_default;
	if (style_home_ddlist_languagesel_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_ddlist_languagesel_main_main_default);
	else
		lv_style_init(&style_home_ddlist_languagesel_main_main_default);
	lv_style_set_radius(&style_home_ddlist_languagesel_main_main_default, 3);
	lv_style_set_bg_color(&style_home_ddlist_languagesel_main_main_default, lv_color_make(0x8c, 0xa0, 0xa0));
	lv_style_set_bg_grad_color(&style_home_ddlist_languagesel_main_main_default, lv_color_make(0x69, 0x7d, 0x7d));
	lv_style_set_bg_grad_dir(&style_home_ddlist_languagesel_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_ddlist_languagesel_main_main_default, 255);
	lv_style_set_border_color(&style_home_ddlist_languagesel_main_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_home_ddlist_languagesel_main_main_default, 1);
	lv_style_set_text_color(&style_home_ddlist_languagesel_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_ddlist_languagesel_main_main_default, &lv_font_simsun_32);
	lv_style_set_text_line_space(&style_home_ddlist_languagesel_main_main_default, 20);
	lv_obj_add_style(ui->home_ddlist_languageSel, &style_home_ddlist_languagesel_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_label_algoinfo
	ui->home_label_algoinfo = lv_label_create(ui->home);
	lv_obj_set_pos(ui->home_label_algoinfo, 343, 405);
	lv_obj_set_size(ui->home_label_algoinfo, 259, 31);
	lv_label_set_text(ui->home_label_algoinfo, "OK");
	lv_label_set_long_mode(ui->home_label_algoinfo, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_algoinfo, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_algoinfo_main_main_default
	static lv_style_t style_home_label_algoinfo_main_main_default;
	if (style_home_label_algoinfo_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_algoinfo_main_main_default);
	else
		lv_style_init(&style_home_label_algoinfo_main_main_default);
	lv_style_set_radius(&style_home_label_algoinfo_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_algoinfo_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_label_algoinfo_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_label_algoinfo_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_algoinfo_main_main_default, 0);
	lv_style_set_text_color(&style_home_label_algoinfo_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_label_algoinfo_main_main_default, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_home_label_algoinfo_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_algoinfo_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_algoinfo_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_algoinfo_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_algoinfo_main_main_default, 0);
	lv_obj_add_style(ui->home_label_algoinfo, &style_home_label_algoinfo_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_btn_nxpFaceRec
	ui->home_btn_nxpFaceRec = lv_btn_create(ui->home);
	lv_obj_set_pos(ui->home_btn_nxpFaceRec, 544, 506);
	lv_obj_set_size(ui->home_btn_nxpFaceRec, 109, 104);

	//Write style state: LV_STATE_DEFAULT for style_home_btn_nxpfacerec_main_main_default
	static lv_style_t style_home_btn_nxpfacerec_main_main_default;
	if (style_home_btn_nxpfacerec_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_btn_nxpfacerec_main_main_default);
	else
		lv_style_init(&style_home_btn_nxpfacerec_main_main_default);
	lv_style_set_radius(&style_home_btn_nxpfacerec_main_main_default, 5);
	lv_style_set_bg_color(&style_home_btn_nxpfacerec_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_home_btn_nxpfacerec_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_home_btn_nxpfacerec_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_btn_nxpfacerec_main_main_default, 0);
	lv_style_set_shadow_color(&style_home_btn_nxpfacerec_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_shadow_opa(&style_home_btn_nxpfacerec_main_main_default, 0);
	lv_style_set_border_color(&style_home_btn_nxpfacerec_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_home_btn_nxpfacerec_main_main_default, 0);
	lv_style_set_border_opa(&style_home_btn_nxpfacerec_main_main_default, 0);
	lv_obj_add_style(ui->home_btn_nxpFaceRec, &style_home_btn_nxpfacerec_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->home_btn_nxpFaceRec_label = lv_label_create(ui->home_btn_nxpFaceRec);
	lv_label_set_text(ui->home_btn_nxpFaceRec_label, "");
	lv_obj_set_style_text_color(ui->home_btn_nxpFaceRec_label, lv_color_make(0x00, 0x00, 0x00), LV_STATE_DEFAULT);
	lv_obj_set_style_pad_all(ui->home_btn_nxpFaceRec, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->home_btn_nxpFaceRec_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes home_img_date
	ui->home_img_date = lv_img_create(ui->home);
	lv_obj_set_pos(ui->home_img_date, 100, 60);
	lv_obj_set_size(ui->home_img_date, 560, 90);

	//Write style state: LV_STATE_DEFAULT for style_home_img_date_main_main_default
	static lv_style_t style_home_img_date_main_main_default;
	if (style_home_img_date_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_date_main_main_default);
	else
		lv_style_init(&style_home_img_date_main_main_default);
	lv_style_set_img_recolor(&style_home_img_date_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_date_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_date_main_main_default, 255);
	lv_obj_add_style(ui->home_img_date, &style_home_img_date_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->home_img_date, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->home_img_date,&_home_date_en_560x90);
	lv_img_set_pivot(ui->home_img_date, 0,0);
	lv_img_set_angle(ui->home_img_date, 0);

	//Write codes home_img_temp
	ui->home_img_temp = lv_img_create(ui->home);
	lv_obj_set_pos(ui->home_img_temp, 171, 282);
	lv_obj_set_size(ui->home_img_temp, 262, 95);

	//Write style state: LV_STATE_DEFAULT for style_home_img_temp_main_main_default
	static lv_style_t style_home_img_temp_main_main_default;
	if (style_home_img_temp_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_temp_main_main_default);
	else
		lv_style_init(&style_home_img_temp_main_main_default);
	lv_style_set_img_recolor(&style_home_img_temp_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_temp_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_temp_main_main_default, 255);
	lv_obj_add_style(ui->home_img_temp, &style_home_img_temp_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->home_img_temp, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->home_img_temp,&_home_temp_262x95);
	lv_img_set_pivot(ui->home_img_temp, 0,0);
	lv_img_set_angle(ui->home_img_temp, 0);

	//Write codes home_img_sun
	ui->home_img_sun = lv_img_create(ui->home);
	lv_obj_set_pos(ui->home_img_sun, 504, 261);
	lv_obj_set_size(ui->home_img_sun, 128, 135);

	//Write style state: LV_STATE_DEFAULT for style_home_img_sun_main_main_default
	static lv_style_t style_home_img_sun_main_main_default;
	if (style_home_img_sun_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_sun_main_main_default);
	else
		lv_style_init(&style_home_img_sun_main_main_default);
	lv_style_set_img_recolor(&style_home_img_sun_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_sun_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_sun_main_main_default, 255);
	lv_obj_add_style(ui->home_img_sun, &style_home_img_sun_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->home_img_sun, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->home_img_sun,&_home_sun_128x135);
	lv_img_set_pivot(ui->home_img_sun, 0,0);
	lv_img_set_angle(ui->home_img_sun, 0);

	//Write codes home_imgbtn_help
	ui->home_imgbtn_help = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_help, 37, 606);
	lv_obj_set_size(ui->home_imgbtn_help, 91, 91);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_help_main_main_default
	static lv_style_t style_home_imgbtn_help_main_main_default;
	if (style_home_imgbtn_help_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_help_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_help_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_help_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_home_imgbtn_help_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_help_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_help_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_help, &style_home_imgbtn_help_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_help_main_main_pressed
	static lv_style_t style_home_imgbtn_help_main_main_pressed;
	if (style_home_imgbtn_help_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_help_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_help_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_help_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_help_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_help_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_help, &style_home_imgbtn_help_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_help_main_main_checked
	static lv_style_t style_home_imgbtn_help_main_main_checked;
	if (style_home_imgbtn_help_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_help_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_help_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_help_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_help_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_help_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_help, &style_home_imgbtn_help_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_help, LV_IMGBTN_STATE_RELEASED, NULL, &_home_help_91x91, NULL);
	lv_obj_add_flag(ui->home_imgbtn_help, LV_OBJ_FLAG_CHECKABLE);

	//Write codes home_label_help
	ui->home_label_help = lv_label_create(ui->home);
	lv_obj_set_pos(ui->home_label_help, 138, 616);
	lv_obj_set_size(ui->home_label_help, 132, 69);
	lv_label_set_text(ui->home_label_help, "Help Screen");
	lv_label_set_long_mode(ui->home_label_help, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_help, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_help_main_main_default
	static lv_style_t style_home_label_help_main_main_default;
	if (style_home_label_help_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_help_main_main_default);
	else
		lv_style_init(&style_home_label_help_main_main_default);
	lv_style_set_radius(&style_home_label_help_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_help_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_label_help_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_label_help_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_help_main_main_default, 0);
	lv_style_set_text_color(&style_home_label_help_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_label_help_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_home_label_help_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_help_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_help_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_help_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_help_main_main_default, 0);
	lv_obj_add_style(ui->home_label_help, &style_home_label_help_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_img_mic_on
	ui->home_img_mic_on = lv_img_create(ui->home);
	lv_obj_set_pos(ui->home_img_mic_on, 316, 475);
	lv_obj_set_size(ui->home_img_mic_on, 118, 197);

	//Write style state: LV_STATE_DEFAULT for style_home_img_mic_on_main_main_default
	static lv_style_t style_home_img_mic_on_main_main_default;
	if (style_home_img_mic_on_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_mic_on_main_main_default);
	else
		lv_style_init(&style_home_img_mic_on_main_main_default);
	lv_style_set_img_recolor(&style_home_img_mic_on_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_mic_on_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_mic_on_main_main_default, 255);
	lv_obj_add_style(ui->home_img_mic_on, &style_home_img_mic_on_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->home_img_mic_on, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->home_img_mic_on,&_home_mic_on_118x197);
	lv_img_set_pivot(ui->home_img_mic_on, 0,0);
	lv_img_set_angle(ui->home_img_mic_on, 0);

	//Write codes home_img_mic_off
	ui->home_img_mic_off = lv_img_create(ui->home);
	lv_obj_set_pos(ui->home_img_mic_off, 345, 518);
	lv_obj_set_size(ui->home_img_mic_off, 61, 111);

	//Write style state: LV_STATE_DEFAULT for style_home_img_mic_off_main_main_default
	static lv_style_t style_home_img_mic_off_main_main_default;
	if (style_home_img_mic_off_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_mic_off_main_main_default);
	else
		lv_style_init(&style_home_img_mic_off_main_main_default);
	lv_style_set_img_recolor(&style_home_img_mic_off_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_mic_off_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_mic_off_main_main_default, 255);
	lv_obj_add_style(ui->home_img_mic_off, &style_home_img_mic_off_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->home_img_mic_off, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->home_img_mic_off,&_home_mic_off_61x111);
	lv_img_set_pivot(ui->home_img_mic_off, 0,0);
	lv_img_set_angle(ui->home_img_mic_off, 0);

	//Write codes home_imgbtn_confirm
	ui->home_imgbtn_confirm = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_confirm, 76, 248);
	lv_obj_set_size(ui->home_imgbtn_confirm, 264, 139);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_confirm_main_main_default
	static lv_style_t style_home_imgbtn_confirm_main_main_default;
	if (style_home_imgbtn_confirm_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_confirm_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_confirm_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_confirm_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_imgbtn_confirm_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_img_recolor(&style_home_imgbtn_confirm_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_confirm_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_confirm_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_confirm, &style_home_imgbtn_confirm_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_confirm_main_main_pressed
	static lv_style_t style_home_imgbtn_confirm_main_main_pressed;
	if (style_home_imgbtn_confirm_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_confirm_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_confirm_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_confirm_main_main_pressed, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_imgbtn_confirm_main_main_pressed, &lv_font_montserratMedium_32);
	lv_style_set_img_recolor(&style_home_imgbtn_confirm_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_confirm_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_confirm, &style_home_imgbtn_confirm_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_confirm_main_main_checked
	static lv_style_t style_home_imgbtn_confirm_main_main_checked;
	if (style_home_imgbtn_confirm_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_confirm_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_confirm_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_confirm_main_main_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_imgbtn_confirm_main_main_checked, &lv_font_montserratMedium_32);
	lv_style_set_img_recolor(&style_home_imgbtn_confirm_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_confirm_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_confirm, &style_home_imgbtn_confirm_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_confirm, LV_IMGBTN_STATE_RELEASED, NULL, &_home_confirm_264x139, NULL);
	lv_obj_add_flag(ui->home_imgbtn_confirm, LV_OBJ_FLAG_CHECKABLE);
	ui->home_imgbtn_confirm_label = lv_label_create(ui->home_imgbtn_confirm);
	lv_label_set_text(ui->home_imgbtn_confirm_label, "Confirm");
	lv_obj_set_style_pad_all(ui->home_imgbtn_confirm, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->home_imgbtn_confirm_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes home_imgbtn_cancel
	ui->home_imgbtn_cancel = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_cancel, 435, 248);
	lv_obj_set_size(ui->home_imgbtn_cancel, 264, 139);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_cancel_main_main_default
	static lv_style_t style_home_imgbtn_cancel_main_main_default;
	if (style_home_imgbtn_cancel_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_cancel_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_cancel_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_cancel_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_imgbtn_cancel_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_img_recolor(&style_home_imgbtn_cancel_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_cancel_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_cancel_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_cancel, &style_home_imgbtn_cancel_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_cancel_main_main_pressed
	static lv_style_t style_home_imgbtn_cancel_main_main_pressed;
	if (style_home_imgbtn_cancel_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_cancel_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_cancel_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_cancel_main_main_pressed, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_imgbtn_cancel_main_main_pressed, &lv_font_montserratMedium_32);
	lv_style_set_img_recolor(&style_home_imgbtn_cancel_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_cancel_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_cancel, &style_home_imgbtn_cancel_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_cancel_main_main_checked
	static lv_style_t style_home_imgbtn_cancel_main_main_checked;
	if (style_home_imgbtn_cancel_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_cancel_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_cancel_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_cancel_main_main_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_imgbtn_cancel_main_main_checked, &lv_font_montserratMedium_32);
	lv_style_set_img_recolor(&style_home_imgbtn_cancel_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_cancel_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_cancel, &style_home_imgbtn_cancel_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_cancel, LV_IMGBTN_STATE_RELEASED, NULL, &_home_cancel_264x139, NULL);
	lv_obj_add_flag(ui->home_imgbtn_cancel, LV_OBJ_FLAG_CHECKABLE);
	ui->home_imgbtn_cancel_label = lv_label_create(ui->home_imgbtn_cancel);
	lv_label_set_text(ui->home_imgbtn_cancel_label, "Cancel");
	lv_obj_set_style_pad_all(ui->home_imgbtn_cancel, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->home_imgbtn_cancel_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes home_label_face_rec_state
	ui->home_label_face_rec_state = lv_label_create(ui->home);
	lv_obj_set_pos(ui->home_label_face_rec_state, 482, 671);
	lv_obj_set_size(ui->home_label_face_rec_state, 239, 24);
	lv_label_set_text(ui->home_label_face_rec_state, "No user detected");
	lv_label_set_long_mode(ui->home_label_face_rec_state, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_face_rec_state, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_face_rec_state_main_main_default
	static lv_style_t style_home_label_face_rec_state_main_main_default;
	if (style_home_label_face_rec_state_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_face_rec_state_main_main_default);
	else
		lv_style_init(&style_home_label_face_rec_state_main_main_default);
	lv_style_set_radius(&style_home_label_face_rec_state_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_face_rec_state_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_label_face_rec_state_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_label_face_rec_state_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_face_rec_state_main_main_default, 0);
	lv_style_set_text_color(&style_home_label_face_rec_state_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_label_face_rec_state_main_main_default, &lv_font_montserratMedium_20);
	lv_style_set_text_letter_space(&style_home_label_face_rec_state_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_face_rec_state_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_face_rec_state_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_face_rec_state_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_face_rec_state_main_main_default, 0);
	lv_obj_add_style(ui->home_label_face_rec_state, &style_home_label_face_rec_state_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_img_nxp_logo
	ui->home_img_nxp_logo = lv_img_create(ui->home);
	lv_obj_set_pos(ui->home_img_nxp_logo, 606, 400);
	lv_obj_set_size(ui->home_img_nxp_logo, 113, 38);

	//Write style state: LV_STATE_DEFAULT for style_home_img_nxp_logo_main_main_default
	static lv_style_t style_home_img_nxp_logo_main_main_default;
	if (style_home_img_nxp_logo_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_nxp_logo_main_main_default);
	else
		lv_style_init(&style_home_img_nxp_logo_main_main_default);
	lv_style_set_img_recolor(&style_home_img_nxp_logo_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_nxp_logo_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_nxp_logo_main_main_default, 255);
	lv_obj_add_style(ui->home_img_nxp_logo, &style_home_img_nxp_logo_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->home_img_nxp_logo, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->home_img_nxp_logo,&_nxp_logo_113x38);
	lv_img_set_pivot(ui->home_img_nxp_logo, 0,0);
	lv_img_set_angle(ui->home_img_nxp_logo, 0);

	//Write codes home_img_time
	ui->home_img_time = lv_img_create(ui->home);
	lv_obj_set_pos(ui->home_img_time, 438, 157);
	lv_obj_set_size(ui->home_img_time, 225, 55);

	//Write style state: LV_STATE_DEFAULT for style_home_img_time_main_main_default
	static lv_style_t style_home_img_time_main_main_default;
	if (style_home_img_time_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_time_main_main_default);
	else
		lv_style_init(&style_home_img_time_main_main_default);
	lv_style_set_img_recolor(&style_home_img_time_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_time_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_time_main_main_default, 255);
	lv_obj_add_style(ui->home_img_time, &style_home_img_time_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->home_img_time, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->home_img_time,&_home_time_225x55);
	lv_img_set_pivot(ui->home_img_time, 0,0);
	lv_img_set_angle(ui->home_img_time, 0);

	//Write codes home_btn_enable_disable_confirm
	ui->home_btn_enable_disable_confirm = lv_btn_create(ui->home);
	lv_obj_set_pos(ui->home_btn_enable_disable_confirm, 612, 403);
	lv_obj_set_size(ui->home_btn_enable_disable_confirm, 103, 38);

	//Write style state: LV_STATE_DEFAULT for style_home_btn_enable_disable_confirm_main_main_default
	static lv_style_t style_home_btn_enable_disable_confirm_main_main_default;
	if (style_home_btn_enable_disable_confirm_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_btn_enable_disable_confirm_main_main_default);
	else
		lv_style_init(&style_home_btn_enable_disable_confirm_main_main_default);
	lv_style_set_radius(&style_home_btn_enable_disable_confirm_main_main_default, 5);
	lv_style_set_bg_color(&style_home_btn_enable_disable_confirm_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_btn_enable_disable_confirm_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_btn_enable_disable_confirm_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_btn_enable_disable_confirm_main_main_default, 0);
	lv_style_set_shadow_color(&style_home_btn_enable_disable_confirm_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_home_btn_enable_disable_confirm_main_main_default, 0);
	lv_style_set_border_color(&style_home_btn_enable_disable_confirm_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_home_btn_enable_disable_confirm_main_main_default, 0);
	lv_style_set_border_opa(&style_home_btn_enable_disable_confirm_main_main_default, 0);
	lv_obj_add_style(ui->home_btn_enable_disable_confirm, &style_home_btn_enable_disable_confirm_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->home_btn_enable_disable_confirm_label = lv_label_create(ui->home_btn_enable_disable_confirm);
	lv_label_set_text(ui->home_btn_enable_disable_confirm_label, "");
	lv_obj_set_style_text_color(ui->home_btn_enable_disable_confirm_label, lv_color_make(0x00, 0x00, 0x00), LV_STATE_DEFAULT);
	lv_obj_set_style_pad_all(ui->home_btn_enable_disable_confirm, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->home_btn_enable_disable_confirm_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes home_btn_turn_on_off_mic
	ui->home_btn_turn_on_off_mic = lv_btn_create(ui->home);
	lv_obj_set_pos(ui->home_btn_turn_on_off_mic, 332, 684);
	lv_obj_set_size(ui->home_btn_turn_on_off_mic, 90, 23);

	//Write style state: LV_STATE_DEFAULT for style_home_btn_turn_on_off_mic_main_main_default
	static lv_style_t style_home_btn_turn_on_off_mic_main_main_default;
	if (style_home_btn_turn_on_off_mic_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_btn_turn_on_off_mic_main_main_default);
	else
		lv_style_init(&style_home_btn_turn_on_off_mic_main_main_default);
	lv_style_set_radius(&style_home_btn_turn_on_off_mic_main_main_default, 5);
	lv_style_set_bg_color(&style_home_btn_turn_on_off_mic_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_btn_turn_on_off_mic_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_btn_turn_on_off_mic_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_btn_turn_on_off_mic_main_main_default, 0);
	lv_style_set_shadow_color(&style_home_btn_turn_on_off_mic_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_home_btn_turn_on_off_mic_main_main_default, 0);
	lv_style_set_border_color(&style_home_btn_turn_on_off_mic_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_home_btn_turn_on_off_mic_main_main_default, 0);
	lv_style_set_border_opa(&style_home_btn_turn_on_off_mic_main_main_default, 0);
	lv_obj_add_style(ui->home_btn_turn_on_off_mic, &style_home_btn_turn_on_off_mic_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->home_btn_turn_on_off_mic_label = lv_label_create(ui->home_btn_turn_on_off_mic);
	lv_label_set_text(ui->home_btn_turn_on_off_mic_label, "");
	lv_obj_set_style_text_color(ui->home_btn_turn_on_off_mic_label, lv_color_make(0x00, 0x00, 0x00), LV_STATE_DEFAULT);
	lv_obj_set_style_pad_all(ui->home_btn_turn_on_off_mic, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->home_btn_turn_on_off_mic_label, LV_ALIGN_CENTER, 0, 0);

	//Init events for screen
	events_init_home(ui);
}
