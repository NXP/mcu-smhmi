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

static void _help_ddlist_languageSel_event_cb(lv_event_t * e)
{
	lv_obj_t * obj = lv_event_get_target(e);

	//Write style state: LV_STATE_DEFAULT for style_help_ddlist_languagesel_extra_list_selected_default
	static lv_style_t style_help_ddlist_languagesel_extra_list_selected_default;
	if (style_help_ddlist_languagesel_extra_list_selected_default.prop_cnt > 1)
		lv_style_reset(&style_help_ddlist_languagesel_extra_list_selected_default);
	else
		lv_style_init(&style_help_ddlist_languagesel_extra_list_selected_default);
	lv_style_set_radius(&style_help_ddlist_languagesel_extra_list_selected_default, 3);
	lv_style_set_bg_color(&style_help_ddlist_languagesel_extra_list_selected_default, lv_color_make(0x46, 0x5a, 0x5a));
	lv_style_set_bg_grad_color(&style_help_ddlist_languagesel_extra_list_selected_default, lv_color_make(0x46, 0x5a, 0x5a));
	lv_style_set_bg_grad_dir(&style_help_ddlist_languagesel_extra_list_selected_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_ddlist_languagesel_extra_list_selected_default, 255);
	lv_style_set_border_color(&style_help_ddlist_languagesel_extra_list_selected_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_help_ddlist_languagesel_extra_list_selected_default, 1);
	lv_style_set_text_color(&style_help_ddlist_languagesel_extra_list_selected_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_ddlist_languagesel_extra_list_selected_default, &lv_font_simsun_32);
	lv_obj_add_style(lv_dropdown_get_list(obj), &style_help_ddlist_languagesel_extra_list_selected_default, LV_PART_SELECTED|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_help_ddlist_languagesel_extra_list_main_default
	static lv_style_t style_help_ddlist_languagesel_extra_list_main_default;
	if (style_help_ddlist_languagesel_extra_list_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_ddlist_languagesel_extra_list_main_default);
	else
		lv_style_init(&style_help_ddlist_languagesel_extra_list_main_default);
	lv_style_set_radius(&style_help_ddlist_languagesel_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_help_ddlist_languagesel_extra_list_main_default, lv_color_make(0x8c, 0xa0, 0xa0));
	lv_style_set_bg_grad_color(&style_help_ddlist_languagesel_extra_list_main_default, lv_color_make(0x69, 0x7d, 0x7d));
	lv_style_set_bg_grad_dir(&style_help_ddlist_languagesel_extra_list_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_ddlist_languagesel_extra_list_main_default, 255);
	lv_style_set_border_color(&style_help_ddlist_languagesel_extra_list_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_help_ddlist_languagesel_extra_list_main_default, 1);
	lv_style_set_text_color(&style_help_ddlist_languagesel_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_ddlist_languagesel_extra_list_main_default, &lv_font_simsun_32);
	/* Must modify the dropbox height if removing the French language option */
#if FRENCH_LANG_SUPPORTED
	lv_style_set_max_height(&style_help_ddlist_languagesel_extra_list_main_default, 220);
#else
	lv_style_set_max_height(&style_help_ddlist_languagesel_extra_list_main_default, 165);
#endif /* FRENCH_LANG_SUPPORTED */
	lv_obj_add_style(lv_dropdown_get_list(obj), &style_help_ddlist_languagesel_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_help_ddlist_languagesel_extra_list_scrollbar_default
	static lv_style_t style_help_ddlist_languagesel_extra_list_scrollbar_default;
	if (style_help_ddlist_languagesel_extra_list_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_help_ddlist_languagesel_extra_list_scrollbar_default);
	else
		lv_style_init(&style_help_ddlist_languagesel_extra_list_scrollbar_default);
	lv_style_set_radius(&style_help_ddlist_languagesel_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_help_ddlist_languagesel_extra_list_scrollbar_default, lv_color_make(0xaf, 0xc3, 0xc3));
	lv_style_set_bg_grad_color(&style_help_ddlist_languagesel_extra_list_scrollbar_default, lv_color_make(0xaf, 0xc3, 0xc3));
	lv_style_set_bg_grad_dir(&style_help_ddlist_languagesel_extra_list_scrollbar_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_ddlist_languagesel_extra_list_scrollbar_default, 0);
	lv_obj_add_style(lv_dropdown_get_list(obj), &style_help_ddlist_languagesel_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
}

void setup_scr_help(lv_ui *ui){

	//Write codes help
	ui->help = lv_obj_create(NULL);

	//Write style state: LV_STATE_DEFAULT for style_help_main_main_default
	static lv_style_t style_help_main_main_default;
	if (style_help_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_main_main_default);
	else
		lv_style_init(&style_help_main_main_default);
	lv_style_set_bg_color(&style_help_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_help_main_main_default, 0);
	lv_obj_add_style(ui->help, &style_help_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_imgbtn_home
	ui->help_imgbtn_home = lv_imgbtn_create(ui->help);
	lv_obj_set_pos(ui->help_imgbtn_home, 0, 0);
	lv_obj_set_size(ui->help_imgbtn_home, 1280, 720);

	//Write style state: LV_STATE_DEFAULT for style_help_imgbtn_home_main_main_default
	static lv_style_t style_help_imgbtn_home_main_main_default;
	if (style_help_imgbtn_home_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_imgbtn_home_main_main_default);
	else
		lv_style_init(&style_help_imgbtn_home_main_main_default);
	lv_style_set_text_color(&style_help_imgbtn_home_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_help_imgbtn_home_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_help_imgbtn_home_main_main_default, 0);
	lv_style_set_img_opa(&style_help_imgbtn_home_main_main_default, 255);
	lv_obj_add_style(ui->help_imgbtn_home, &style_help_imgbtn_home_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_help_imgbtn_home_main_main_pressed
	static lv_style_t style_help_imgbtn_home_main_main_pressed;
	if (style_help_imgbtn_home_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_help_imgbtn_home_main_main_pressed);
	else
		lv_style_init(&style_help_imgbtn_home_main_main_pressed);
	lv_style_set_text_color(&style_help_imgbtn_home_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_help_imgbtn_home_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_help_imgbtn_home_main_main_pressed, 0);
	lv_obj_add_style(ui->help_imgbtn_home, &style_help_imgbtn_home_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_help_imgbtn_home_main_main_checked
	static lv_style_t style_help_imgbtn_home_main_main_checked;
	if (style_help_imgbtn_home_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_help_imgbtn_home_main_main_checked);
	else
		lv_style_init(&style_help_imgbtn_home_main_main_checked);
	lv_style_set_text_color(&style_help_imgbtn_home_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_help_imgbtn_home_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_help_imgbtn_home_main_main_checked, 0);
	lv_obj_add_style(ui->help_imgbtn_home, &style_help_imgbtn_home_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->help_imgbtn_home, LV_IMGBTN_STATE_RELEASED, NULL, &_home_background_1280x720, NULL);
	lv_obj_add_flag(ui->help_imgbtn_home, LV_OBJ_FLAG_CHECKABLE);

	//Write codes help_cont_commands
	ui->help_cont_commands = lv_obj_create(ui->help);
	lv_obj_set_pos(ui->help_cont_commands, 843, 185);
	lv_obj_set_size(ui->help_cont_commands, 372, 471);

	//Write style state: LV_STATE_DEFAULT for style_help_cont_commands_main_main_default
	static lv_style_t style_help_cont_commands_main_main_default;
	if (style_help_cont_commands_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_cont_commands_main_main_default);
	else
		lv_style_init(&style_help_cont_commands_main_main_default);
	lv_style_set_radius(&style_help_cont_commands_main_main_default, 10);
	lv_style_set_bg_color(&style_help_cont_commands_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_help_cont_commands_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_help_cont_commands_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_cont_commands_main_main_default, 78);
	lv_style_set_border_color(&style_help_cont_commands_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_help_cont_commands_main_main_default, 1);
	lv_style_set_border_opa(&style_help_cont_commands_main_main_default, 255);
	lv_style_set_pad_left(&style_help_cont_commands_main_main_default, 0);
	lv_style_set_pad_right(&style_help_cont_commands_main_main_default, 0);
	lv_style_set_pad_top(&style_help_cont_commands_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_cont_commands_main_main_default, 0);
	lv_obj_add_style(ui->help_cont_commands, &style_help_cont_commands_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_label_delete_user
	ui->help_label_delete_user = lv_label_create(ui->help_cont_commands);
	lv_obj_set_pos(ui->help_label_delete_user, 25, 425);
	lv_obj_set_size(ui->help_label_delete_user, 331, 36);
	lv_label_set_text(ui->help_label_delete_user, "Delete user");
	lv_label_set_long_mode(ui->help_label_delete_user, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->help_label_delete_user, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_help_label_delete_user_main_main_default
	static lv_style_t style_help_label_delete_user_main_main_default;
	if (style_help_label_delete_user_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_label_delete_user_main_main_default);
	else
		lv_style_init(&style_help_label_delete_user_main_main_default);
	lv_style_set_radius(&style_help_label_delete_user_main_main_default, 0);
	lv_style_set_bg_color(&style_help_label_delete_user_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_help_label_delete_user_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_help_label_delete_user_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_label_delete_user_main_main_default, 0);
	lv_style_set_text_color(&style_help_label_delete_user_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_label_delete_user_main_main_default, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_help_label_delete_user_main_main_default, 2);
	lv_style_set_pad_left(&style_help_label_delete_user_main_main_default, 0);
	lv_style_set_pad_right(&style_help_label_delete_user_main_main_default, 0);
	lv_style_set_pad_top(&style_help_label_delete_user_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_label_delete_user_main_main_default, 0);
	lv_obj_add_style(ui->help_label_delete_user, &style_help_label_delete_user_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_label_cancel
	ui->help_label_cancel = lv_label_create(ui->help_cont_commands);
	lv_obj_set_pos(ui->help_label_cancel, 25, 345);
	lv_obj_set_size(ui->help_label_cancel, 277, 32);
	lv_label_set_text(ui->help_label_cancel, "Cancel");
	lv_label_set_long_mode(ui->help_label_cancel, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->help_label_cancel, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_help_label_cancel_main_main_default
	static lv_style_t style_help_label_cancel_main_main_default;
	if (style_help_label_cancel_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_label_cancel_main_main_default);
	else
		lv_style_init(&style_help_label_cancel_main_main_default);
	lv_style_set_radius(&style_help_label_cancel_main_main_default, 0);
	lv_style_set_bg_color(&style_help_label_cancel_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_help_label_cancel_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_help_label_cancel_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_label_cancel_main_main_default, 0);
	lv_style_set_text_color(&style_help_label_cancel_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_label_cancel_main_main_default, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_help_label_cancel_main_main_default, 2);
	lv_style_set_pad_left(&style_help_label_cancel_main_main_default, 0);
	lv_style_set_pad_right(&style_help_label_cancel_main_main_default, 0);
	lv_style_set_pad_top(&style_help_label_cancel_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_label_cancel_main_main_default, 0);
	lv_obj_add_style(ui->help_label_cancel, &style_help_label_cancel_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_label_confirm
	ui->help_label_confirm = lv_label_create(ui->help_cont_commands);
	lv_obj_set_pos(ui->help_label_confirm, 25, 305);
	lv_obj_set_size(ui->help_label_confirm, 277, 32);
	lv_label_set_text(ui->help_label_confirm, "Confirm");
	lv_label_set_long_mode(ui->help_label_confirm, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->help_label_confirm, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_help_label_confirm_main_main_default
	static lv_style_t style_help_label_confirm_main_main_default;
	if (style_help_label_confirm_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_label_confirm_main_main_default);
	else
		lv_style_init(&style_help_label_confirm_main_main_default);
	lv_style_set_radius(&style_help_label_confirm_main_main_default, 0);
	lv_style_set_bg_color(&style_help_label_confirm_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_help_label_confirm_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_help_label_confirm_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_label_confirm_main_main_default, 0);
	lv_style_set_text_color(&style_help_label_confirm_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_label_confirm_main_main_default, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_help_label_confirm_main_main_default, 2);
	lv_style_set_pad_left(&style_help_label_confirm_main_main_default, 0);
	lv_style_set_pad_right(&style_help_label_confirm_main_main_default, 0);
	lv_style_set_pad_top(&style_help_label_confirm_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_label_confirm_main_main_default, 0);
	lv_obj_add_style(ui->help_label_confirm, &style_help_label_confirm_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_label_floor_six
	ui->help_label_floor_six = lv_label_create(ui->help_cont_commands);
	lv_obj_set_pos(ui->help_label_floor_six, 25, 225);
	lv_obj_set_size(ui->help_label_floor_six, 277, 32);
	lv_label_set_text(ui->help_label_floor_six, "Sixth floor");
	lv_label_set_long_mode(ui->help_label_floor_six, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->help_label_floor_six, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_help_label_floor_six_main_main_default
	static lv_style_t style_help_label_floor_six_main_main_default;
	if (style_help_label_floor_six_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_label_floor_six_main_main_default);
	else
		lv_style_init(&style_help_label_floor_six_main_main_default);
	lv_style_set_radius(&style_help_label_floor_six_main_main_default, 0);
	lv_style_set_bg_color(&style_help_label_floor_six_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_help_label_floor_six_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_help_label_floor_six_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_label_floor_six_main_main_default, 0);
	lv_style_set_text_color(&style_help_label_floor_six_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_label_floor_six_main_main_default, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_help_label_floor_six_main_main_default, 2);
	lv_style_set_pad_left(&style_help_label_floor_six_main_main_default, 0);
	lv_style_set_pad_right(&style_help_label_floor_six_main_main_default, 0);
	lv_style_set_pad_top(&style_help_label_floor_six_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_label_floor_six_main_main_default, 0);
	lv_obj_add_style(ui->help_label_floor_six, &style_help_label_floor_six_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_label_floor_five
	ui->help_label_floor_five = lv_label_create(ui->help_cont_commands);
	lv_obj_set_pos(ui->help_label_floor_five, 25, 185);
	lv_obj_set_size(ui->help_label_floor_five, 277, 32);
	lv_label_set_text(ui->help_label_floor_five, "Fifth floor");
	lv_label_set_long_mode(ui->help_label_floor_five, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->help_label_floor_five, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_help_label_floor_five_main_main_default
	static lv_style_t style_help_label_floor_five_main_main_default;
	if (style_help_label_floor_five_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_label_floor_five_main_main_default);
	else
		lv_style_init(&style_help_label_floor_five_main_main_default);
	lv_style_set_radius(&style_help_label_floor_five_main_main_default, 0);
	lv_style_set_bg_color(&style_help_label_floor_five_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_help_label_floor_five_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_help_label_floor_five_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_label_floor_five_main_main_default, 0);
	lv_style_set_text_color(&style_help_label_floor_five_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_label_floor_five_main_main_default, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_help_label_floor_five_main_main_default, 2);
	lv_style_set_pad_left(&style_help_label_floor_five_main_main_default, 0);
	lv_style_set_pad_right(&style_help_label_floor_five_main_main_default, 0);
	lv_style_set_pad_top(&style_help_label_floor_five_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_label_floor_five_main_main_default, 0);
	lv_obj_add_style(ui->help_label_floor_five, &style_help_label_floor_five_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_label_floor_four
	ui->help_label_floor_four = lv_label_create(ui->help_cont_commands);
	lv_obj_set_pos(ui->help_label_floor_four, 25, 145);
	lv_obj_set_size(ui->help_label_floor_four, 277, 32);
	lv_label_set_text(ui->help_label_floor_four, "Fourth floor");
	lv_label_set_long_mode(ui->help_label_floor_four, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->help_label_floor_four, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_help_label_floor_four_main_main_default
	static lv_style_t style_help_label_floor_four_main_main_default;
	if (style_help_label_floor_four_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_label_floor_four_main_main_default);
	else
		lv_style_init(&style_help_label_floor_four_main_main_default);
	lv_style_set_radius(&style_help_label_floor_four_main_main_default, 0);
	lv_style_set_bg_color(&style_help_label_floor_four_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_help_label_floor_four_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_help_label_floor_four_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_label_floor_four_main_main_default, 0);
	lv_style_set_text_color(&style_help_label_floor_four_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_label_floor_four_main_main_default, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_help_label_floor_four_main_main_default, 2);
	lv_style_set_pad_left(&style_help_label_floor_four_main_main_default, 0);
	lv_style_set_pad_right(&style_help_label_floor_four_main_main_default, 0);
	lv_style_set_pad_top(&style_help_label_floor_four_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_label_floor_four_main_main_default, 0);
	lv_obj_add_style(ui->help_label_floor_four, &style_help_label_floor_four_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_label_floor_three
	ui->help_label_floor_three = lv_label_create(ui->help_cont_commands);
	lv_obj_set_pos(ui->help_label_floor_three, 25, 105);
	lv_obj_set_size(ui->help_label_floor_three, 277, 32);
	lv_label_set_text(ui->help_label_floor_three, "Third floor");
	lv_label_set_long_mode(ui->help_label_floor_three, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->help_label_floor_three, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_help_label_floor_three_main_main_default
	static lv_style_t style_help_label_floor_three_main_main_default;
	if (style_help_label_floor_three_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_label_floor_three_main_main_default);
	else
		lv_style_init(&style_help_label_floor_three_main_main_default);
	lv_style_set_radius(&style_help_label_floor_three_main_main_default, 0);
	lv_style_set_bg_color(&style_help_label_floor_three_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_help_label_floor_three_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_help_label_floor_three_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_label_floor_three_main_main_default, 0);
	lv_style_set_text_color(&style_help_label_floor_three_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_label_floor_three_main_main_default, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_help_label_floor_three_main_main_default, 2);
	lv_style_set_pad_left(&style_help_label_floor_three_main_main_default, 0);
	lv_style_set_pad_right(&style_help_label_floor_three_main_main_default, 0);
	lv_style_set_pad_top(&style_help_label_floor_three_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_label_floor_three_main_main_default, 0);
	lv_obj_add_style(ui->help_label_floor_three, &style_help_label_floor_three_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_label_floor_one
	ui->help_label_floor_one = lv_label_create(ui->help_cont_commands);
	lv_obj_set_pos(ui->help_label_floor_one, 25, 25);
	lv_obj_set_size(ui->help_label_floor_one, 277, 32);
	lv_label_set_text(ui->help_label_floor_one, "First floor");
	lv_label_set_long_mode(ui->help_label_floor_one, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->help_label_floor_one, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_help_label_floor_one_main_main_default
	static lv_style_t style_help_label_floor_one_main_main_default;
	if (style_help_label_floor_one_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_label_floor_one_main_main_default);
	else
		lv_style_init(&style_help_label_floor_one_main_main_default);
	lv_style_set_radius(&style_help_label_floor_one_main_main_default, 0);
	lv_style_set_bg_color(&style_help_label_floor_one_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_help_label_floor_one_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_help_label_floor_one_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_label_floor_one_main_main_default, 0);
	lv_style_set_text_color(&style_help_label_floor_one_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_label_floor_one_main_main_default, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_help_label_floor_one_main_main_default, 2);
	lv_style_set_pad_left(&style_help_label_floor_one_main_main_default, 0);
	lv_style_set_pad_right(&style_help_label_floor_one_main_main_default, 0);
	lv_style_set_pad_top(&style_help_label_floor_one_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_label_floor_one_main_main_default, 0);
	lv_obj_add_style(ui->help_label_floor_one, &style_help_label_floor_one_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_label_floor_two
	ui->help_label_floor_two = lv_label_create(ui->help_cont_commands);
	lv_obj_set_pos(ui->help_label_floor_two, 25, 65);
	lv_obj_set_size(ui->help_label_floor_two, 277, 32);
	lv_label_set_text(ui->help_label_floor_two, "Second floor");
	lv_label_set_long_mode(ui->help_label_floor_two, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->help_label_floor_two, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_help_label_floor_two_main_main_default
	static lv_style_t style_help_label_floor_two_main_main_default;
	if (style_help_label_floor_two_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_label_floor_two_main_main_default);
	else
		lv_style_init(&style_help_label_floor_two_main_main_default);
	lv_style_set_radius(&style_help_label_floor_two_main_main_default, 0);
	lv_style_set_bg_color(&style_help_label_floor_two_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_help_label_floor_two_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_help_label_floor_two_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_label_floor_two_main_main_default, 0);
	lv_style_set_text_color(&style_help_label_floor_two_main_main_default, lv_color_make(0xfa, 0xfa, 0xfa));
	lv_style_set_text_font(&style_help_label_floor_two_main_main_default, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_help_label_floor_two_main_main_default, 2);
	lv_style_set_pad_left(&style_help_label_floor_two_main_main_default, 0);
	lv_style_set_pad_right(&style_help_label_floor_two_main_main_default, 0);
	lv_style_set_pad_top(&style_help_label_floor_two_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_label_floor_two_main_main_default, 0);
	lv_obj_add_style(ui->help_label_floor_two, &style_help_label_floor_two_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_label_commands
	ui->help_label_commands = lv_label_create(ui->help);
	lv_obj_set_pos(ui->help_label_commands, 918, 75);
	lv_obj_set_size(ui->help_label_commands, 213, 68);
	lv_label_set_text(ui->help_label_commands, "Voice commands");
	lv_label_set_long_mode(ui->help_label_commands, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->help_label_commands, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_help_label_commands_main_main_default
	static lv_style_t style_help_label_commands_main_main_default;
	if (style_help_label_commands_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_label_commands_main_main_default);
	else
		lv_style_init(&style_help_label_commands_main_main_default);
	lv_style_set_radius(&style_help_label_commands_main_main_default, 0);
	lv_style_set_bg_color(&style_help_label_commands_main_main_default, lv_color_make(0xaa, 0xe4, 0xe0));
	lv_style_set_bg_grad_color(&style_help_label_commands_main_main_default, lv_color_make(0xa9, 0xe5, 0xe1));
	lv_style_set_bg_grad_dir(&style_help_label_commands_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_label_commands_main_main_default, 0);
	lv_style_set_text_color(&style_help_label_commands_main_main_default, lv_color_make(0x00, 0xff, 0x00));
	lv_style_set_text_font(&style_help_label_commands_main_main_default, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_help_label_commands_main_main_default, 2);
	lv_style_set_pad_left(&style_help_label_commands_main_main_default, 0);
	lv_style_set_pad_right(&style_help_label_commands_main_main_default, 0);
	lv_style_set_pad_top(&style_help_label_commands_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_label_commands_main_main_default, 0);
	lv_obj_add_style(ui->help_label_commands, &style_help_label_commands_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_cont_tips
	ui->help_cont_tips = lv_obj_create(ui->help);
	lv_obj_set_pos(ui->help_cont_tips, 64, 52);
	lv_obj_set_size(ui->help_cont_tips, 649, 377);

	//Write style state: LV_STATE_DEFAULT for style_help_cont_tips_main_main_default
	static lv_style_t style_help_cont_tips_main_main_default;
	if (style_help_cont_tips_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_cont_tips_main_main_default);
	else
		lv_style_init(&style_help_cont_tips_main_main_default);
	lv_style_set_radius(&style_help_cont_tips_main_main_default, 5);
	lv_style_set_bg_color(&style_help_cont_tips_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_help_cont_tips_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_help_cont_tips_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_cont_tips_main_main_default, 0);
	lv_style_set_border_color(&style_help_cont_tips_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_help_cont_tips_main_main_default, 1);
	lv_style_set_border_opa(&style_help_cont_tips_main_main_default, 0);
	lv_style_set_pad_left(&style_help_cont_tips_main_main_default, 0);
	lv_style_set_pad_right(&style_help_cont_tips_main_main_default, 0);
	lv_style_set_pad_top(&style_help_cont_tips_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_cont_tips_main_main_default, 0);
	lv_obj_add_style(ui->help_cont_tips, &style_help_cont_tips_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_label_say_ww
	ui->help_label_say_ww = lv_label_create(ui->help_cont_tips);
	lv_obj_set_pos(ui->help_label_say_ww, 28, 52);
	lv_obj_set_size(ui->help_label_say_ww, 561, 72);
	lv_label_set_text(ui->help_label_say_ww, "Say \"Hey, NXP\"");
	lv_label_set_long_mode(ui->help_label_say_ww, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->help_label_say_ww, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_help_label_say_ww_main_main_default
	static lv_style_t style_help_label_say_ww_main_main_default;
	if (style_help_label_say_ww_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_label_say_ww_main_main_default);
	else
		lv_style_init(&style_help_label_say_ww_main_main_default);
	lv_style_set_radius(&style_help_label_say_ww_main_main_default, 0);
	lv_style_set_bg_color(&style_help_label_say_ww_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_help_label_say_ww_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_help_label_say_ww_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_label_say_ww_main_main_default, 0);
	lv_style_set_text_color(&style_help_label_say_ww_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_label_say_ww_main_main_default, &lv_font_montserratMedium_48);
	lv_style_set_text_letter_space(&style_help_label_say_ww_main_main_default, 2);
	lv_style_set_pad_left(&style_help_label_say_ww_main_main_default, 0);
	lv_style_set_pad_right(&style_help_label_say_ww_main_main_default, 0);
	lv_style_set_pad_top(&style_help_label_say_ww_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_label_say_ww_main_main_default, 0);
	lv_obj_add_style(ui->help_label_say_ww, &style_help_label_say_ww_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_label_enable_voice
	ui->help_label_enable_voice = lv_label_create(ui->help);
	lv_obj_set_pos(ui->help_label_enable_voice, 92, 210);
	lv_obj_set_size(ui->help_label_enable_voice, 591, 105);
	lv_label_set_text(ui->help_label_enable_voice, "to wake up the");
	lv_label_set_long_mode(ui->help_label_enable_voice, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->help_label_enable_voice, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_help_label_enable_voice_main_main_default
	static lv_style_t style_help_label_enable_voice_main_main_default;
	if (style_help_label_enable_voice_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_label_enable_voice_main_main_default);
	else
		lv_style_init(&style_help_label_enable_voice_main_main_default);
	lv_style_set_radius(&style_help_label_enable_voice_main_main_default, 0);
	lv_style_set_bg_color(&style_help_label_enable_voice_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_help_label_enable_voice_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_help_label_enable_voice_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_label_enable_voice_main_main_default, 0);
	lv_style_set_text_color(&style_help_label_enable_voice_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_label_enable_voice_main_main_default, &lv_font_montserratMedium_48);
	lv_style_set_text_letter_space(&style_help_label_enable_voice_main_main_default, 2);
	lv_style_set_pad_left(&style_help_label_enable_voice_main_main_default, 0);
	lv_style_set_pad_right(&style_help_label_enable_voice_main_main_default, 0);
	lv_style_set_pad_top(&style_help_label_enable_voice_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_label_enable_voice_main_main_default, 0);
	lv_obj_add_style(ui->help_label_enable_voice, &style_help_label_enable_voice_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_imgbtn_back
	ui->help_imgbtn_back = lv_imgbtn_create(ui->help);
	lv_obj_set_pos(ui->help_imgbtn_back, 46, 608);
	lv_obj_set_size(ui->help_imgbtn_back, 81, 82);

	//Write style state: LV_STATE_DEFAULT for style_help_imgbtn_back_main_main_default
	static lv_style_t style_help_imgbtn_back_main_main_default;
	if (style_help_imgbtn_back_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_imgbtn_back_main_main_default);
	else
		lv_style_init(&style_help_imgbtn_back_main_main_default);
	lv_style_set_text_color(&style_help_imgbtn_back_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_help_imgbtn_back_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_help_imgbtn_back_main_main_default, 0);
	lv_style_set_img_opa(&style_help_imgbtn_back_main_main_default, 255);
	lv_obj_add_style(ui->help_imgbtn_back, &style_help_imgbtn_back_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_help_imgbtn_back_main_main_pressed
	static lv_style_t style_help_imgbtn_back_main_main_pressed;
	if (style_help_imgbtn_back_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_help_imgbtn_back_main_main_pressed);
	else
		lv_style_init(&style_help_imgbtn_back_main_main_pressed);
	lv_style_set_text_color(&style_help_imgbtn_back_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_help_imgbtn_back_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_help_imgbtn_back_main_main_pressed, 0);
	lv_obj_add_style(ui->help_imgbtn_back, &style_help_imgbtn_back_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_help_imgbtn_back_main_main_checked
	static lv_style_t style_help_imgbtn_back_main_main_checked;
	if (style_help_imgbtn_back_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_help_imgbtn_back_main_main_checked);
	else
		lv_style_init(&style_help_imgbtn_back_main_main_checked);
	lv_style_set_text_color(&style_help_imgbtn_back_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_help_imgbtn_back_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_help_imgbtn_back_main_main_checked, 0);
	lv_obj_add_style(ui->help_imgbtn_back, &style_help_imgbtn_back_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->help_imgbtn_back, LV_IMGBTN_STATE_RELEASED, NULL, &_back_alpha_81x82, NULL);
	lv_obj_add_flag(ui->help_imgbtn_back, LV_OBJ_FLAG_CHECKABLE);

	//Write codes help_img_nxp_logo
	ui->help_img_nxp_logo = lv_img_create(ui->help);
	lv_obj_set_pos(ui->help_img_nxp_logo, 606, 400);
	lv_obj_set_size(ui->help_img_nxp_logo, 113, 38);

	//Write style state: LV_STATE_DEFAULT for style_help_img_nxp_logo_main_main_default
	static lv_style_t style_help_img_nxp_logo_main_main_default;
	if (style_help_img_nxp_logo_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_img_nxp_logo_main_main_default);
	else
		lv_style_init(&style_help_img_nxp_logo_main_main_default);
	lv_style_set_img_recolor(&style_help_img_nxp_logo_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_help_img_nxp_logo_main_main_default, 0);
	lv_style_set_img_opa(&style_help_img_nxp_logo_main_main_default, 255);
	lv_obj_add_style(ui->help_img_nxp_logo, &style_help_img_nxp_logo_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->help_img_nxp_logo, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->help_img_nxp_logo,&_nxp_logo_113x38);
	lv_img_set_pivot(ui->help_img_nxp_logo, 0,0);
	lv_img_set_angle(ui->help_img_nxp_logo, 0);

	//Write codes help_label_back
	ui->help_label_back = lv_label_create(ui->help);
	lv_obj_set_pos(ui->help_label_back, 146, 625);
	lv_obj_set_size(ui->help_label_back, 530, 60);
	lv_label_set_text(ui->help_label_back, "Back to Main");
	lv_label_set_long_mode(ui->help_label_back, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->help_label_back, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_help_label_back_main_main_default
	static lv_style_t style_help_label_back_main_main_default;
	if (style_help_label_back_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_label_back_main_main_default);
	else
		lv_style_init(&style_help_label_back_main_main_default);
	lv_style_set_radius(&style_help_label_back_main_main_default, 0);
	lv_style_set_bg_color(&style_help_label_back_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_help_label_back_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_help_label_back_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_label_back_main_main_default, 0);
	lv_style_set_text_color(&style_help_label_back_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_label_back_main_main_default, &lv_font_montserratMedium_48);
	lv_style_set_text_letter_space(&style_help_label_back_main_main_default, 2);
	lv_style_set_pad_left(&style_help_label_back_main_main_default, 0);
	lv_style_set_pad_right(&style_help_label_back_main_main_default, 0);
	lv_style_set_pad_top(&style_help_label_back_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_label_back_main_main_default, 0);
	lv_obj_add_style(ui->help_label_back, &style_help_label_back_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_label_voice_command
	ui->help_label_voice_command = lv_label_create(ui->help);
	lv_obj_set_pos(ui->help_label_voice_command, 92, 331);
	lv_obj_set_size(ui->help_label_voice_command, 561, 72);
	lv_label_set_text(ui->help_label_voice_command, "voice command");
	lv_label_set_long_mode(ui->help_label_voice_command, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->help_label_voice_command, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_help_label_voice_command_main_main_default
	static lv_style_t style_help_label_voice_command_main_main_default;
	if (style_help_label_voice_command_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_label_voice_command_main_main_default);
	else
		lv_style_init(&style_help_label_voice_command_main_main_default);
	lv_style_set_radius(&style_help_label_voice_command_main_main_default, 0);
	lv_style_set_bg_color(&style_help_label_voice_command_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_help_label_voice_command_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_help_label_voice_command_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_label_voice_command_main_main_default, 0);
	lv_style_set_text_color(&style_help_label_voice_command_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_label_voice_command_main_main_default, &lv_font_montserratMedium_48);
	lv_style_set_text_letter_space(&style_help_label_voice_command_main_main_default, 2);
	lv_style_set_pad_left(&style_help_label_voice_command_main_main_default, 0);
	lv_style_set_pad_right(&style_help_label_voice_command_main_main_default, 0);
	lv_style_set_pad_top(&style_help_label_voice_command_main_main_default, 0);
	lv_style_set_pad_bottom(&style_help_label_voice_command_main_main_default, 0);
	lv_obj_add_style(ui->help_label_voice_command, &style_help_label_voice_command_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes help_ddlist_languageSel
	ui->help_ddlist_languageSel = lv_dropdown_create(ui->help);
	lv_obj_set_pos(ui->help_ddlist_languageSel, 48, 490);
	lv_obj_set_width(ui->help_ddlist_languageSel, 193);
	/* Must remove the French language option from the list if not supported */
#if FRENCH_LANG_SUPPORTED
	lv_dropdown_set_options(ui->help_ddlist_languageSel, "English\n中文\nDeutsch\nFrançais");
#else
	lv_dropdown_set_options(ui->help_ddlist_languageSel, "English\n中文\nDeutsch");
#endif /* FRENCH_LANG_SUPPORTED */
	lv_obj_add_event_cb(ui->help_ddlist_languageSel, _help_ddlist_languageSel_event_cb, LV_EVENT_READY, NULL);

	//Write style state: LV_STATE_DEFAULT for style_help_ddlist_languagesel_main_main_default
	static lv_style_t style_help_ddlist_languagesel_main_main_default;
	if (style_help_ddlist_languagesel_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_help_ddlist_languagesel_main_main_default);
	else
		lv_style_init(&style_help_ddlist_languagesel_main_main_default);
	lv_style_set_radius(&style_help_ddlist_languagesel_main_main_default, 3);
	lv_style_set_bg_color(&style_help_ddlist_languagesel_main_main_default, lv_color_make(0x8c, 0xa0, 0xa0));
	lv_style_set_bg_grad_color(&style_help_ddlist_languagesel_main_main_default, lv_color_make(0x69, 0x7d, 0x7d));
	lv_style_set_bg_grad_dir(&style_help_ddlist_languagesel_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_help_ddlist_languagesel_main_main_default, 255);
	lv_style_set_border_color(&style_help_ddlist_languagesel_main_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_help_ddlist_languagesel_main_main_default, 1);
	lv_style_set_text_color(&style_help_ddlist_languagesel_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_help_ddlist_languagesel_main_main_default, &lv_font_simsun_32);
	lv_style_set_text_line_space(&style_help_ddlist_languagesel_main_main_default, 20);
	lv_obj_add_style(ui->help_ddlist_languageSel, &style_help_ddlist_languagesel_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Init events for screen
	events_init_help(ui);
}