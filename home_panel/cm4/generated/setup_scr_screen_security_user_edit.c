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

static lv_obj_t * g_kb_screen_security_user_edit;
static void kb_screen_security_user_edit_event_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *kb = lv_event_get_target(e);
	if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL){
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}
static void ta_screen_security_user_edit_event_cb(lv_event_t *e)
{

	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *ta = lv_event_get_target(e);
	lv_obj_t *kb = lv_event_get_user_data(e);
	if (code == LV_EVENT_FOCUSED)
	{
		lv_keyboard_set_textarea(kb, ta);
		lv_obj_move_foreground(kb);
		lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
	if (code == LV_EVENT_CANCEL)
	{
		lv_keyboard_set_textarea(kb, NULL);
		lv_obj_move_background(kb);
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}

void setup_scr_screen_security_user_edit(lv_ui *ui){

	//Write codes screen_security_user_edit
	ui->screen_security_user_edit = lv_obj_create(NULL);

	//Create keyboard on screen_security_user_edit
	g_kb_screen_security_user_edit = lv_keyboard_create(ui->screen_security_user_edit);
	lv_obj_add_event_cb(g_kb_screen_security_user_edit, kb_screen_security_user_edit_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(g_kb_screen_security_user_edit, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_scrollbar_mode(ui->screen_security_user_edit, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_user_edit_main_main_default
	static lv_style_t style_screen_security_user_edit_main_main_default;
	if (style_screen_security_user_edit_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_user_edit_main_main_default);
	else
		lv_style_init(&style_screen_security_user_edit_main_main_default);
	lv_style_set_bg_color(&style_screen_security_user_edit_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_screen_security_user_edit_main_main_default, 0);
	lv_obj_add_style(ui->screen_security_user_edit, &style_screen_security_user_edit_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_security_user_edit_cont_thermBackground
	ui->screen_security_user_edit_cont_thermBackground = lv_obj_create(ui->screen_security_user_edit);
	lv_obj_set_pos(ui->screen_security_user_edit_cont_thermBackground, 0, 0);
	lv_obj_set_size(ui->screen_security_user_edit_cont_thermBackground, 1280, 720);
	lv_obj_set_scrollbar_mode(ui->screen_security_user_edit_cont_thermBackground, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_user_edit_cont_thermbackground_main_main_default
	static lv_style_t style_screen_security_user_edit_cont_thermbackground_main_main_default;
	if (style_screen_security_user_edit_cont_thermbackground_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_user_edit_cont_thermbackground_main_main_default);
	else
		lv_style_init(&style_screen_security_user_edit_cont_thermbackground_main_main_default);
	lv_style_set_radius(&style_screen_security_user_edit_cont_thermbackground_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_security_user_edit_cont_thermbackground_main_main_default, lv_color_make(0xfc, 0xfc, 0xfc));
	lv_style_set_bg_grad_color(&style_screen_security_user_edit_cont_thermbackground_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_security_user_edit_cont_thermbackground_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_security_user_edit_cont_thermbackground_main_main_default, 255);
	lv_style_set_shadow_width(&style_screen_security_user_edit_cont_thermbackground_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_user_edit_cont_thermbackground_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_user_edit_cont_thermbackground_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_user_edit_cont_thermbackground_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_user_edit_cont_thermbackground_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_user_edit_cont_thermbackground_main_main_default, 0);
	lv_style_set_border_color(&style_screen_security_user_edit_cont_thermbackground_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_screen_security_user_edit_cont_thermbackground_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_security_user_edit_cont_thermbackground_main_main_default, 255);
	lv_style_set_pad_left(&style_screen_security_user_edit_cont_thermbackground_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_security_user_edit_cont_thermbackground_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_security_user_edit_cont_thermbackground_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_security_user_edit_cont_thermbackground_main_main_default, 0);
	lv_obj_add_style(ui->screen_security_user_edit_cont_thermBackground, &style_screen_security_user_edit_cont_thermbackground_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_security_user_edit_img_background
	ui->screen_security_user_edit_img_background = lv_img_create(ui->screen_security_user_edit_cont_thermBackground);
	lv_obj_set_pos(ui->screen_security_user_edit_img_background, 0, 0);
	lv_obj_set_size(ui->screen_security_user_edit_img_background, 1280, 630);
	lv_obj_set_scrollbar_mode(ui->screen_security_user_edit_img_background, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_security_user_edit_img_background, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_security_user_edit_img_background,&_main_background_1280x630);
	lv_img_set_pivot(ui->screen_security_user_edit_img_background, 0,0);
	lv_img_set_angle(ui->screen_security_user_edit_img_background, 0);

	//Write codes screen_security_user_edit_img_background_bottom
	ui->screen_security_user_edit_img_background_bottom = lv_img_create(ui->screen_security_user_edit_cont_thermBackground);
	lv_obj_set_pos(ui->screen_security_user_edit_img_background_bottom, 0, 630);
	lv_obj_set_size(ui->screen_security_user_edit_img_background_bottom, 1280, 90);
	lv_obj_set_scrollbar_mode(ui->screen_security_user_edit_img_background_bottom, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_user_edit_img_background_bottom_main_main_default
	static lv_style_t style_screen_security_user_edit_img_background_bottom_main_main_default;
	if (style_screen_security_user_edit_img_background_bottom_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_user_edit_img_background_bottom_main_main_default);
	else
		lv_style_init(&style_screen_security_user_edit_img_background_bottom_main_main_default);
	lv_style_set_img_recolor(&style_screen_security_user_edit_img_background_bottom_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_security_user_edit_img_background_bottom_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_security_user_edit_img_background_bottom_main_main_default, 255);
	lv_obj_add_style(ui->screen_security_user_edit_img_background_bottom, &style_screen_security_user_edit_img_background_bottom_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_security_user_edit_img_background_bottom, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_security_user_edit_img_background_bottom,&_other_background_bottom_1280x90);
	lv_img_set_pivot(ui->screen_security_user_edit_img_background_bottom, 50,50);
	lv_img_set_angle(ui->screen_security_user_edit_img_background_bottom, 0);

	//Write codes screen_security_user_edit_label_title
	ui->screen_security_user_edit_label_title = lv_label_create(ui->screen_security_user_edit_cont_thermBackground);
	lv_obj_set_pos(ui->screen_security_user_edit_label_title, 230, 30);
	lv_obj_set_size(ui->screen_security_user_edit_label_title, 760, 60);
	lv_obj_set_scrollbar_mode(ui->screen_security_user_edit_label_title, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_security_user_edit_label_title, "SECURITY CONTROL");
	lv_label_set_long_mode(ui->screen_security_user_edit_label_title, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_user_edit_label_title_main_main_default
	static lv_style_t style_screen_security_user_edit_label_title_main_main_default;
	if (style_screen_security_user_edit_label_title_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_user_edit_label_title_main_main_default);
	else
		lv_style_init(&style_screen_security_user_edit_label_title_main_main_default);
	lv_style_set_radius(&style_screen_security_user_edit_label_title_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_security_user_edit_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_security_user_edit_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_security_user_edit_label_title_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_security_user_edit_label_title_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_security_user_edit_label_title_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_user_edit_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_user_edit_label_title_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_user_edit_label_title_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_user_edit_label_title_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_user_edit_label_title_main_main_default, 0);
	lv_style_set_text_color(&style_screen_security_user_edit_label_title_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_security_user_edit_label_title_main_main_default, &lv_font_montserratMedium_50);
	lv_style_set_text_letter_space(&style_screen_security_user_edit_label_title_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_security_user_edit_label_title_main_main_default, 0);
	lv_style_set_text_align(&style_screen_security_user_edit_label_title_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_security_user_edit_label_title_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_security_user_edit_label_title_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_security_user_edit_label_title_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_security_user_edit_label_title_main_main_default, 0);
	lv_obj_add_style(ui->screen_security_user_edit_label_title, &style_screen_security_user_edit_label_title_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_security_user_edit_label_user_list
	ui->screen_security_user_edit_label_user_list = lv_label_create(ui->screen_security_user_edit_cont_thermBackground);
	lv_obj_set_pos(ui->screen_security_user_edit_label_user_list, 15, 120);
	lv_obj_set_size(ui->screen_security_user_edit_label_user_list, 265, 50);
	lv_obj_set_scrollbar_mode(ui->screen_security_user_edit_label_user_list, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_security_user_edit_label_user_list, "User List");
	lv_label_set_long_mode(ui->screen_security_user_edit_label_user_list, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_user_edit_label_user_list_main_main_default
	static lv_style_t style_screen_security_user_edit_label_user_list_main_main_default;
	if (style_screen_security_user_edit_label_user_list_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_user_edit_label_user_list_main_main_default);
	else
		lv_style_init(&style_screen_security_user_edit_label_user_list_main_main_default);
	lv_style_set_radius(&style_screen_security_user_edit_label_user_list_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_security_user_edit_label_user_list_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_security_user_edit_label_user_list_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_security_user_edit_label_user_list_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_security_user_edit_label_user_list_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_security_user_edit_label_user_list_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_user_edit_label_user_list_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_user_edit_label_user_list_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_user_edit_label_user_list_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_user_edit_label_user_list_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_user_edit_label_user_list_main_main_default, 0);
	lv_style_set_text_color(&style_screen_security_user_edit_label_user_list_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_security_user_edit_label_user_list_main_main_default, &lv_font_montserratMedium_50);
	lv_style_set_text_letter_space(&style_screen_security_user_edit_label_user_list_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_security_user_edit_label_user_list_main_main_default, 0);
	lv_style_set_text_align(&style_screen_security_user_edit_label_user_list_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_security_user_edit_label_user_list_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_security_user_edit_label_user_list_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_security_user_edit_label_user_list_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_security_user_edit_label_user_list_main_main_default, 0);
	lv_obj_add_style(ui->screen_security_user_edit_label_user_list, &style_screen_security_user_edit_label_user_list_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_security_user_edit_imgbtn_save
	ui->screen_security_user_edit_imgbtn_save = lv_imgbtn_create(ui->screen_security_user_edit_cont_thermBackground);
	lv_obj_set_pos(ui->screen_security_user_edit_imgbtn_save, 731, 595);
	lv_obj_set_size(ui->screen_security_user_edit_imgbtn_save, 210, 110);
	lv_obj_set_scrollbar_mode(ui->screen_security_user_edit_imgbtn_save, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_user_edit_imgbtn_save_main_main_default
	static lv_style_t style_screen_security_user_edit_imgbtn_save_main_main_default;
	if (style_screen_security_user_edit_imgbtn_save_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_user_edit_imgbtn_save_main_main_default);
	else
		lv_style_init(&style_screen_security_user_edit_imgbtn_save_main_main_default);
	lv_style_set_shadow_width(&style_screen_security_user_edit_imgbtn_save_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_user_edit_imgbtn_save_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_user_edit_imgbtn_save_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_user_edit_imgbtn_save_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_user_edit_imgbtn_save_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_user_edit_imgbtn_save_main_main_default, 0);
	lv_style_set_text_color(&style_screen_security_user_edit_imgbtn_save_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_security_user_edit_imgbtn_save_main_main_default, &lv_font_montserratMedium_50);
	lv_style_set_text_align(&style_screen_security_user_edit_imgbtn_save_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_img_recolor(&style_screen_security_user_edit_imgbtn_save_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_security_user_edit_imgbtn_save_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_security_user_edit_imgbtn_save_main_main_default, 255);
	lv_obj_add_style(ui->screen_security_user_edit_imgbtn_save, &style_screen_security_user_edit_imgbtn_save_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_imgbtn_set_src(ui->screen_security_user_edit_imgbtn_save, LV_IMGBTN_STATE_RELEASED, NULL, &_button_green_alpha_210x110, NULL);
	ui->screen_security_user_edit_imgbtn_save_label = lv_label_create(ui->screen_security_user_edit_imgbtn_save);
	lv_label_set_text(ui->screen_security_user_edit_imgbtn_save_label, "Save");
	lv_obj_set_style_pad_all(ui->screen_security_user_edit_imgbtn_save, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->screen_security_user_edit_imgbtn_save_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes screen_security_user_edit_imgbtn_edit_cancel
	ui->screen_security_user_edit_imgbtn_edit_cancel = lv_imgbtn_create(ui->screen_security_user_edit_cont_thermBackground);
	lv_obj_set_pos(ui->screen_security_user_edit_imgbtn_edit_cancel, 1005, 595);
	lv_obj_set_size(ui->screen_security_user_edit_imgbtn_edit_cancel, 210, 110);
	lv_obj_set_scrollbar_mode(ui->screen_security_user_edit_imgbtn_edit_cancel, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default
	static lv_style_t style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default;
	if (style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default);
	else
		lv_style_init(&style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default);
	lv_style_set_shadow_width(&style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default, 0);
	lv_style_set_text_color(&style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default, &lv_font_montserratMedium_50);
	lv_style_set_text_align(&style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_img_recolor(&style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default, 255);
	lv_obj_add_style(ui->screen_security_user_edit_imgbtn_edit_cancel, &style_screen_security_user_edit_imgbtn_edit_cancel_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_imgbtn_set_src(ui->screen_security_user_edit_imgbtn_edit_cancel, LV_IMGBTN_STATE_RELEASED, NULL, &_button_red_alpha_210x110, NULL);
	ui->screen_security_user_edit_imgbtn_edit_cancel_label = lv_label_create(ui->screen_security_user_edit_imgbtn_edit_cancel);
	lv_label_set_text(ui->screen_security_user_edit_imgbtn_edit_cancel_label, "Cancel");
	lv_obj_set_style_pad_all(ui->screen_security_user_edit_imgbtn_edit_cancel, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->screen_security_user_edit_imgbtn_edit_cancel_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes screen_security_user_edit_ta_user_name
	ui->screen_security_user_edit_ta_user_name = lv_textarea_create(ui->screen_security_user_edit_cont_thermBackground);
	lv_obj_set_pos(ui->screen_security_user_edit_ta_user_name, 225, 221);
	lv_obj_set_size(ui->screen_security_user_edit_ta_user_name, 330, 55);
	lv_obj_set_scrollbar_mode(ui->screen_security_user_edit_ta_user_name, LV_SCROLLBAR_MODE_AUTO);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_user_edit_ta_user_name_main_main_default
	static lv_style_t style_screen_security_user_edit_ta_user_name_main_main_default;
	if (style_screen_security_user_edit_ta_user_name_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_user_edit_ta_user_name_main_main_default);
	else
		lv_style_init(&style_screen_security_user_edit_ta_user_name_main_main_default);
	lv_style_set_radius(&style_screen_security_user_edit_ta_user_name_main_main_default, 4);
	lv_style_set_bg_color(&style_screen_security_user_edit_ta_user_name_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_security_user_edit_ta_user_name_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_security_user_edit_ta_user_name_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_security_user_edit_ta_user_name_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_security_user_edit_ta_user_name_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_user_edit_ta_user_name_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_user_edit_ta_user_name_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_user_edit_ta_user_name_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_user_edit_ta_user_name_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_user_edit_ta_user_name_main_main_default, 0);
	lv_style_set_border_color(&style_screen_security_user_edit_ta_user_name_main_main_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_screen_security_user_edit_ta_user_name_main_main_default, 2);
	lv_style_set_border_opa(&style_screen_security_user_edit_ta_user_name_main_main_default, 0);
	lv_style_set_text_color(&style_screen_security_user_edit_ta_user_name_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_security_user_edit_ta_user_name_main_main_default, &lv_font_simsun_40);
	lv_style_set_text_letter_space(&style_screen_security_user_edit_ta_user_name_main_main_default, 2);
	lv_style_set_text_align(&style_screen_security_user_edit_ta_user_name_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_security_user_edit_ta_user_name_main_main_default, 4);
	lv_style_set_pad_right(&style_screen_security_user_edit_ta_user_name_main_main_default, 4);
	lv_style_set_pad_top(&style_screen_security_user_edit_ta_user_name_main_main_default, 4);
	lv_style_set_pad_bottom(&style_screen_security_user_edit_ta_user_name_main_main_default, 4);
	lv_obj_add_style(ui->screen_security_user_edit_ta_user_name, &style_screen_security_user_edit_ta_user_name_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_FOCUSED for style_screen_security_user_edit_ta_user_name_main_main_focused
	static lv_style_t style_screen_security_user_edit_ta_user_name_main_main_focused;
	if (style_screen_security_user_edit_ta_user_name_main_main_focused.prop_cnt > 1)
		lv_style_reset(&style_screen_security_user_edit_ta_user_name_main_main_focused);
	else
		lv_style_init(&style_screen_security_user_edit_ta_user_name_main_main_focused);
	lv_style_set_radius(&style_screen_security_user_edit_ta_user_name_main_main_focused, 4);
	lv_style_set_bg_color(&style_screen_security_user_edit_ta_user_name_main_main_focused, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_security_user_edit_ta_user_name_main_main_focused, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_security_user_edit_ta_user_name_main_main_focused, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_security_user_edit_ta_user_name_main_main_focused, 0);
	lv_style_set_shadow_width(&style_screen_security_user_edit_ta_user_name_main_main_focused, 0);
	lv_style_set_shadow_color(&style_screen_security_user_edit_ta_user_name_main_main_focused, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_user_edit_ta_user_name_main_main_focused, 255);
	lv_style_set_shadow_spread(&style_screen_security_user_edit_ta_user_name_main_main_focused, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_user_edit_ta_user_name_main_main_focused, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_user_edit_ta_user_name_main_main_focused, 0);
	lv_style_set_border_color(&style_screen_security_user_edit_ta_user_name_main_main_focused, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_screen_security_user_edit_ta_user_name_main_main_focused, 2);
	lv_style_set_border_opa(&style_screen_security_user_edit_ta_user_name_main_main_focused, 0);
	lv_style_set_text_color(&style_screen_security_user_edit_ta_user_name_main_main_focused, lv_color_make(0x00, 0xff, 0x00));
	lv_style_set_text_font(&style_screen_security_user_edit_ta_user_name_main_main_focused, &lv_font_montserratMedium_38);
	lv_style_set_pad_left(&style_screen_security_user_edit_ta_user_name_main_main_focused, 4);
	lv_style_set_pad_right(&style_screen_security_user_edit_ta_user_name_main_main_focused, 4);
	lv_style_set_pad_top(&style_screen_security_user_edit_ta_user_name_main_main_focused, 4);
	lv_style_set_pad_bottom(&style_screen_security_user_edit_ta_user_name_main_main_focused, 4);
	lv_obj_add_style(ui->screen_security_user_edit_ta_user_name, &style_screen_security_user_edit_ta_user_name_main_main_focused, LV_PART_MAIN|LV_STATE_FOCUSED);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_user_edit_ta_user_name_main_scrollbar_default
	static lv_style_t style_screen_security_user_edit_ta_user_name_main_scrollbar_default;
	if (style_screen_security_user_edit_ta_user_name_main_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_user_edit_ta_user_name_main_scrollbar_default);
	else
		lv_style_init(&style_screen_security_user_edit_ta_user_name_main_scrollbar_default);
	lv_style_set_radius(&style_screen_security_user_edit_ta_user_name_main_scrollbar_default, 0);
	lv_style_set_bg_color(&style_screen_security_user_edit_ta_user_name_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_opa(&style_screen_security_user_edit_ta_user_name_main_scrollbar_default, 255);
	lv_obj_add_style(ui->screen_security_user_edit_ta_user_name, &style_screen_security_user_edit_ta_user_name_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->screen_security_user_edit_ta_user_name,"User_00");

	//use keyboard on screen_security_user_edit_ta_user_name
	lv_obj_add_event_cb(ui->screen_security_user_edit_ta_user_name, ta_screen_security_user_edit_event_cb, LV_EVENT_ALL, g_kb_screen_security_user_edit);

	//Write codes screen_security_user_edit_imgbtn_home
	ui->screen_security_user_edit_imgbtn_home = lv_imgbtn_create(ui->screen_security_user_edit);
	lv_obj_set_pos(ui->screen_security_user_edit_imgbtn_home, 24, 26);
	lv_obj_set_size(ui->screen_security_user_edit_imgbtn_home, 85, 70);
	lv_obj_set_scrollbar_mode(ui->screen_security_user_edit_imgbtn_home, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_security_user_edit_imgbtn_home, LV_IMGBTN_STATE_RELEASED, NULL, &_home_85x70, NULL);

	//Init events for screen
	events_init_screen_security_user_edit(ui);
}