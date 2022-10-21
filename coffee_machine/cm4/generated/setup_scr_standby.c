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

static void _standby_ddlist_languageSel_event_cb(lv_event_t * e)
{
	lv_obj_t * obj = lv_event_get_target(e);

	//Write style state: LV_STATE_DEFAULT for style_standby_ddlist_languagesel_extra_list_selected_default
	static lv_style_t style_standby_ddlist_languagesel_extra_list_selected_default;
	if (style_standby_ddlist_languagesel_extra_list_selected_default.prop_cnt > 1)
		lv_style_reset(&style_standby_ddlist_languagesel_extra_list_selected_default);
	else
		lv_style_init(&style_standby_ddlist_languagesel_extra_list_selected_default);
	lv_style_set_radius(&style_standby_ddlist_languagesel_extra_list_selected_default, 3);
	lv_style_set_bg_color(&style_standby_ddlist_languagesel_extra_list_selected_default, lv_color_make(0xbe, 0x8c, 0x00));
	lv_style_set_bg_grad_color(&style_standby_ddlist_languagesel_extra_list_selected_default, lv_color_make(0xbe, 0x8c, 0x00));
	lv_style_set_bg_grad_dir(&style_standby_ddlist_languagesel_extra_list_selected_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_standby_ddlist_languagesel_extra_list_selected_default, 255);
	lv_style_set_border_color(&style_standby_ddlist_languagesel_extra_list_selected_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_standby_ddlist_languagesel_extra_list_selected_default, 1);
	lv_style_set_text_color(&style_standby_ddlist_languagesel_extra_list_selected_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_standby_ddlist_languagesel_extra_list_selected_default, &lv_font_simsun_32);
	lv_obj_add_style(lv_dropdown_get_list(obj), &style_standby_ddlist_languagesel_extra_list_selected_default, LV_PART_SELECTED|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_standby_ddlist_languagesel_extra_list_main_default
	static lv_style_t style_standby_ddlist_languagesel_extra_list_main_default;
	if (style_standby_ddlist_languagesel_extra_list_main_default.prop_cnt > 1)
		lv_style_reset(&style_standby_ddlist_languagesel_extra_list_main_default);
	else
		lv_style_init(&style_standby_ddlist_languagesel_extra_list_main_default);
	lv_style_set_radius(&style_standby_ddlist_languagesel_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_standby_ddlist_languagesel_extra_list_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_standby_ddlist_languagesel_extra_list_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_standby_ddlist_languagesel_extra_list_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_standby_ddlist_languagesel_extra_list_main_default, 255);
	lv_style_set_border_color(&style_standby_ddlist_languagesel_extra_list_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_standby_ddlist_languagesel_extra_list_main_default, 1);
	lv_style_set_text_color(&style_standby_ddlist_languagesel_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_standby_ddlist_languagesel_extra_list_main_default, &lv_font_simsun_32);
	/* Must modify the dropbox height if removing the French language option */
#if FRENCH_LANG_SUPPORTED
	lv_style_set_max_height(&style_standby_ddlist_languagesel_extra_list_main_default, 220);
#else
	lv_style_set_max_height(&style_standby_ddlist_languagesel_extra_list_main_default, 165);
#endif /* FRENCH_LANG_SUPPORTED */
	lv_obj_add_style(lv_dropdown_get_list(obj), &style_standby_ddlist_languagesel_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_standby_ddlist_languagesel_extra_list_scrollbar_default
	static lv_style_t style_standby_ddlist_languagesel_extra_list_scrollbar_default;
	if (style_standby_ddlist_languagesel_extra_list_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_standby_ddlist_languagesel_extra_list_scrollbar_default);
	else
		lv_style_init(&style_standby_ddlist_languagesel_extra_list_scrollbar_default);
	lv_style_set_radius(&style_standby_ddlist_languagesel_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_standby_ddlist_languagesel_extra_list_scrollbar_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_standby_ddlist_languagesel_extra_list_scrollbar_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_standby_ddlist_languagesel_extra_list_scrollbar_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_standby_ddlist_languagesel_extra_list_scrollbar_default, 0);
	lv_obj_add_style(lv_dropdown_get_list(obj), &style_standby_ddlist_languagesel_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
}

void setup_scr_standby(lv_ui *ui){

	//Write codes standby
	ui->standby = lv_obj_create(NULL);

	//Write style state: LV_STATE_DEFAULT for style_standby_main_main_default
	static lv_style_t style_standby_main_main_default;
	if (style_standby_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_standby_main_main_default);
	else
		lv_style_init(&style_standby_main_main_default);
	lv_style_set_bg_color(&style_standby_main_main_default, lv_color_make(0x3e, 0x3e, 0x3e));
	lv_style_set_bg_opa(&style_standby_main_main_default, 0);
	lv_obj_add_style(ui->standby, &style_standby_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes standby_cont_header
	ui->standby_cont_header = lv_obj_create(ui->standby);
	lv_obj_set_pos(ui->standby_cont_header, 0, 0);
	lv_obj_set_size(ui->standby_cont_header, 1280, 100);

	//Write style state: LV_STATE_DEFAULT for style_standby_cont_header_main_main_default
	static lv_style_t style_standby_cont_header_main_main_default;
	if (style_standby_cont_header_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_standby_cont_header_main_main_default);
	else
		lv_style_init(&style_standby_cont_header_main_main_default);
	lv_style_set_radius(&style_standby_cont_header_main_main_default, 0);
	lv_style_set_bg_color(&style_standby_cont_header_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_standby_cont_header_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_standby_cont_header_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_standby_cont_header_main_main_default, 255);
	lv_style_set_border_color(&style_standby_cont_header_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_border_width(&style_standby_cont_header_main_main_default, 0);
	lv_style_set_border_opa(&style_standby_cont_header_main_main_default, 0);
	lv_style_set_pad_left(&style_standby_cont_header_main_main_default, 0);
	lv_style_set_pad_right(&style_standby_cont_header_main_main_default, 0);
	lv_style_set_pad_top(&style_standby_cont_header_main_main_default, 0);
	lv_style_set_pad_bottom(&style_standby_cont_header_main_main_default, 0);
	lv_obj_add_style(ui->standby_cont_header, &style_standby_cont_header_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes standby_btn_header_home
	ui->standby_btn_header_home = lv_btn_create(ui->standby_cont_header);
	lv_obj_set_pos(ui->standby_btn_header_home, 0, 0);
	lv_obj_set_size(ui->standby_btn_header_home, 1080, 100);

	//Write style state: LV_STATE_DEFAULT for style_standby_btn_header_home_main_main_default
	static lv_style_t style_standby_btn_header_home_main_main_default;
	if (style_standby_btn_header_home_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_standby_btn_header_home_main_main_default);
	else
		lv_style_init(&style_standby_btn_header_home_main_main_default);
	lv_style_set_radius(&style_standby_btn_header_home_main_main_default, 0);
	lv_style_set_bg_color(&style_standby_btn_header_home_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_standby_btn_header_home_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_standby_btn_header_home_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_standby_btn_header_home_main_main_default, 255);
	lv_style_set_shadow_color(&style_standby_btn_header_home_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_standby_btn_header_home_main_main_default, 255);
	lv_style_set_border_color(&style_standby_btn_header_home_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_border_width(&style_standby_btn_header_home_main_main_default, 0);
	lv_style_set_border_opa(&style_standby_btn_header_home_main_main_default, 255);
	lv_obj_add_style(ui->standby_btn_header_home, &style_standby_btn_header_home_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->standby_btn_header_home_label = lv_label_create(ui->standby_btn_header_home);
	lv_label_set_text(ui->standby_btn_header_home_label, "default");
	lv_obj_set_style_text_color(ui->standby_btn_header_home_label, lv_color_make(0x00, 0x00, 0x00), LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->standby_btn_header_home_label, &lv_font_montserratMedium_32, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_all(ui->standby_btn_header_home, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->standby_btn_header_home_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes standby_ddlist_languageSel
	ui->standby_ddlist_languageSel = lv_dropdown_create(ui->standby_cont_header);
	lv_obj_set_pos(ui->standby_ddlist_languageSel, 1090, 10);
	lv_obj_set_width(ui->standby_ddlist_languageSel, 185);
	/* Must remove the French language option from the list if not supported */
#if FRENCH_LANG_SUPPORTED
	lv_dropdown_set_options(ui->standby_ddlist_languageSel, "English\n中文\nDeutsch\nFrançais");
#else
	lv_dropdown_set_options(ui->standby_ddlist_languageSel, "English\n中文\nDeutsch");
#endif /* FRENCH_LANG_SUPPORTED */
	lv_obj_add_event_cb(ui->standby_ddlist_languageSel, _standby_ddlist_languageSel_event_cb, LV_EVENT_READY, NULL);

	//Write style state: LV_STATE_DEFAULT for style_standby_ddlist_languagesel_main_main_default
	static lv_style_t style_standby_ddlist_languagesel_main_main_default;
	if (style_standby_ddlist_languagesel_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_standby_ddlist_languagesel_main_main_default);
	else
		lv_style_init(&style_standby_ddlist_languagesel_main_main_default);
	lv_style_set_radius(&style_standby_ddlist_languagesel_main_main_default, 3);
	lv_style_set_bg_color(&style_standby_ddlist_languagesel_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_standby_ddlist_languagesel_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_standby_ddlist_languagesel_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_standby_ddlist_languagesel_main_main_default, 255);
	lv_style_set_border_color(&style_standby_ddlist_languagesel_main_main_default, lv_color_make(0x96, 0x96, 0x96));
	lv_style_set_border_width(&style_standby_ddlist_languagesel_main_main_default, 1);
	lv_style_set_text_color(&style_standby_ddlist_languagesel_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_standby_ddlist_languagesel_main_main_default, &lv_font_simsun_32);
	lv_style_set_text_line_space(&style_standby_ddlist_languagesel_main_main_default, 20);
	lv_obj_add_style(ui->standby_ddlist_languageSel, &style_standby_ddlist_languagesel_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes standby_img_nxpVoiceRec
	ui->standby_img_nxpVoiceRec = lv_img_create(ui->standby_cont_header);
	lv_obj_set_pos(ui->standby_img_nxpVoiceRec, 77, 0);
	lv_obj_set_size(ui->standby_img_nxpVoiceRec, 300, 80);

	//Write style state: LV_STATE_DEFAULT for style_standby_img_nxpvoicerec_main_main_default
	static lv_style_t style_standby_img_nxpvoicerec_main_main_default;
	if (style_standby_img_nxpvoicerec_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_standby_img_nxpvoicerec_main_main_default);
	else
		lv_style_init(&style_standby_img_nxpvoicerec_main_main_default);
	lv_style_set_img_recolor(&style_standby_img_nxpvoicerec_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_standby_img_nxpvoicerec_main_main_default, 0);
	lv_style_set_img_opa(&style_standby_img_nxpvoicerec_main_main_default, 255);
	lv_obj_add_style(ui->standby_img_nxpVoiceRec, &style_standby_img_nxpvoicerec_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->standby_img_nxpVoiceRec, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->standby_img_nxpVoiceRec,&_nxp_face_rec_300x80);
	lv_img_set_pivot(ui->standby_img_nxpVoiceRec, 0,0);
	lv_img_set_angle(ui->standby_img_nxpVoiceRec, 0);

	//Write codes standby_img_nxpFaceRec
	ui->standby_img_nxpFaceRec = lv_img_create(ui->standby_cont_header);
	lv_obj_set_pos(ui->standby_img_nxpFaceRec, 775, 0);
	lv_obj_set_size(ui->standby_img_nxpFaceRec, 300, 80);

	//Write style state: LV_STATE_DEFAULT for style_standby_img_nxpfacerec_main_main_default
	static lv_style_t style_standby_img_nxpfacerec_main_main_default;
	if (style_standby_img_nxpfacerec_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_standby_img_nxpfacerec_main_main_default);
	else
		lv_style_init(&style_standby_img_nxpfacerec_main_main_default);
	lv_style_set_img_recolor(&style_standby_img_nxpfacerec_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_standby_img_nxpfacerec_main_main_default, 0);
	lv_style_set_img_opa(&style_standby_img_nxpfacerec_main_main_default, 255);
	lv_obj_add_style(ui->standby_img_nxpFaceRec, &style_standby_img_nxpfacerec_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->standby_img_nxpFaceRec, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->standby_img_nxpFaceRec,&_nxp_voice_rec_300x80);
	lv_img_set_pivot(ui->standby_img_nxpFaceRec, 0,0);
	lv_img_set_angle(ui->standby_img_nxpFaceRec, 0);

	//Write codes standby_cont_body
	ui->standby_cont_body = lv_obj_create(ui->standby);
	lv_obj_set_pos(ui->standby_cont_body, 0, 100);
	lv_obj_set_size(ui->standby_cont_body, 1280, 620);

	//Write style state: LV_STATE_DEFAULT for style_standby_cont_body_main_main_default
	static lv_style_t style_standby_cont_body_main_main_default;
	if (style_standby_cont_body_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_standby_cont_body_main_main_default);
	else
		lv_style_init(&style_standby_cont_body_main_main_default);
	lv_style_set_radius(&style_standby_cont_body_main_main_default, 0);
	lv_style_set_bg_color(&style_standby_cont_body_main_main_default, lv_color_make(0x3e, 0x3e, 0x3e));
	lv_style_set_bg_grad_color(&style_standby_cont_body_main_main_default, lv_color_make(0x3e, 0x3e, 0x3e));
	lv_style_set_bg_grad_dir(&style_standby_cont_body_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_standby_cont_body_main_main_default, 255);
	lv_style_set_border_color(&style_standby_cont_body_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_standby_cont_body_main_main_default, 0);
	lv_style_set_border_opa(&style_standby_cont_body_main_main_default, 255);
	lv_style_set_pad_left(&style_standby_cont_body_main_main_default, 0);
	lv_style_set_pad_right(&style_standby_cont_body_main_main_default, 0);
	lv_style_set_pad_top(&style_standby_cont_body_main_main_default, 0);
	lv_style_set_pad_bottom(&style_standby_cont_body_main_main_default, 0);
	lv_obj_add_style(ui->standby_cont_body, &style_standby_cont_body_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes standby_imgbtn_home
	ui->standby_imgbtn_home = lv_imgbtn_create(ui->standby_cont_body);
	lv_obj_set_pos(ui->standby_imgbtn_home, 0, 0);
	lv_obj_set_size(ui->standby_imgbtn_home, 1280, 620);

	//Write style state: LV_STATE_DEFAULT for style_standby_imgbtn_home_main_main_default
	static lv_style_t style_standby_imgbtn_home_main_main_default;
	if (style_standby_imgbtn_home_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_standby_imgbtn_home_main_main_default);
	else
		lv_style_init(&style_standby_imgbtn_home_main_main_default);
	lv_style_set_text_color(&style_standby_imgbtn_home_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_standby_imgbtn_home_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_standby_imgbtn_home_main_main_default, 0);
	lv_style_set_img_opa(&style_standby_imgbtn_home_main_main_default, 255);
	lv_obj_add_style(ui->standby_imgbtn_home, &style_standby_imgbtn_home_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_standby_imgbtn_home_main_main_pressed
	static lv_style_t style_standby_imgbtn_home_main_main_pressed;
	if (style_standby_imgbtn_home_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_standby_imgbtn_home_main_main_pressed);
	else
		lv_style_init(&style_standby_imgbtn_home_main_main_pressed);
	lv_style_set_text_color(&style_standby_imgbtn_home_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_standby_imgbtn_home_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_standby_imgbtn_home_main_main_pressed, 0);
	lv_obj_add_style(ui->standby_imgbtn_home, &style_standby_imgbtn_home_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_standby_imgbtn_home_main_main_checked
	static lv_style_t style_standby_imgbtn_home_main_main_checked;
	if (style_standby_imgbtn_home_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_standby_imgbtn_home_main_main_checked);
	else
		lv_style_init(&style_standby_imgbtn_home_main_main_checked);
	lv_style_set_text_color(&style_standby_imgbtn_home_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_standby_imgbtn_home_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_standby_imgbtn_home_main_main_checked, 0);
	lv_obj_add_style(ui->standby_imgbtn_home, &style_standby_imgbtn_home_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_add_flag(ui->standby_imgbtn_home, LV_OBJ_FLAG_CHECKABLE);

	//Write codes standby_animimg_screensaver
	ui->standby_animimg_screensaver = lv_animimg_create(ui->standby_cont_body);
	lv_obj_set_pos(ui->standby_animimg_screensaver, 835, 177);
	lv_obj_set_size(ui->standby_animimg_screensaver, 200, 200);

	//Write codes standby_cont_help
	ui->standby_cont_help = lv_obj_create(ui->standby);
	lv_obj_set_pos(ui->standby_cont_help, 100, 213);
	lv_obj_set_size(ui->standby_cont_help, 605, 350);

	//Write style state: LV_STATE_DEFAULT for style_standby_cont_help_main_main_default
	static lv_style_t style_standby_cont_help_main_main_default;
	if (style_standby_cont_help_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_standby_cont_help_main_main_default);
	else
		lv_style_init(&style_standby_cont_help_main_main_default);
	lv_style_set_radius(&style_standby_cont_help_main_main_default, 10);
	lv_style_set_bg_color(&style_standby_cont_help_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_standby_cont_help_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_standby_cont_help_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_standby_cont_help_main_main_default, 0);
	lv_style_set_border_color(&style_standby_cont_help_main_main_default, lv_color_make(0xa0, 0xa0, 0xa0));
	lv_style_set_border_width(&style_standby_cont_help_main_main_default, 1);
	lv_style_set_border_opa(&style_standby_cont_help_main_main_default, 255);
	lv_style_set_pad_left(&style_standby_cont_help_main_main_default, 0);
	lv_style_set_pad_right(&style_standby_cont_help_main_main_default, 0);
	lv_style_set_pad_top(&style_standby_cont_help_main_main_default, 0);
	lv_style_set_pad_bottom(&style_standby_cont_help_main_main_default, 0);
	lv_obj_add_style(ui->standby_cont_help, &style_standby_cont_help_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes standby_label_select_coffee
	ui->standby_label_select_coffee = lv_label_create(ui->standby_cont_help);
	lv_obj_set_pos(ui->standby_label_select_coffee, 10, 248);
	lv_obj_set_size(ui->standby_label_select_coffee, 570, 41);
	lv_label_set_text(ui->standby_label_select_coffee, "to select your coffee");
	lv_label_set_long_mode(ui->standby_label_select_coffee, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->standby_label_select_coffee, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_standby_label_select_coffee_main_main_default
	static lv_style_t style_standby_label_select_coffee_main_main_default;
	if (style_standby_label_select_coffee_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_standby_label_select_coffee_main_main_default);
	else
		lv_style_init(&style_standby_label_select_coffee_main_main_default);
	lv_style_set_radius(&style_standby_label_select_coffee_main_main_default, 0);
	lv_style_set_bg_color(&style_standby_label_select_coffee_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_standby_label_select_coffee_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_standby_label_select_coffee_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_standby_label_select_coffee_main_main_default, 0);
	lv_style_set_text_color(&style_standby_label_select_coffee_main_main_default, lv_color_make(0xa0, 0xa0, 0xa0));
	lv_style_set_text_font(&style_standby_label_select_coffee_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_standby_label_select_coffee_main_main_default, 2);
	lv_style_set_pad_left(&style_standby_label_select_coffee_main_main_default, 0);
	lv_style_set_pad_right(&style_standby_label_select_coffee_main_main_default, 0);
	lv_style_set_pad_top(&style_standby_label_select_coffee_main_main_default, 0);
	lv_style_set_pad_bottom(&style_standby_label_select_coffee_main_main_default, 0);
	lv_obj_add_style(ui->standby_label_select_coffee, &style_standby_label_select_coffee_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes standby_label_touch_ww
	ui->standby_label_touch_ww = lv_label_create(ui->standby_cont_help);
	lv_obj_set_pos(ui->standby_label_touch_ww, 10, 167);
	lv_obj_set_size(ui->standby_label_touch_ww, 570, 41);
	lv_label_set_text(ui->standby_label_touch_ww, "Touch the screen");
	lv_label_set_long_mode(ui->standby_label_touch_ww, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->standby_label_touch_ww, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_standby_label_touch_ww_main_main_default
	static lv_style_t style_standby_label_touch_ww_main_main_default;
	if (style_standby_label_touch_ww_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_standby_label_touch_ww_main_main_default);
	else
		lv_style_init(&style_standby_label_touch_ww_main_main_default);
	lv_style_set_radius(&style_standby_label_touch_ww_main_main_default, 0);
	lv_style_set_bg_color(&style_standby_label_touch_ww_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_standby_label_touch_ww_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_standby_label_touch_ww_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_standby_label_touch_ww_main_main_default, 0);
	lv_style_set_text_color(&style_standby_label_touch_ww_main_main_default, lv_color_make(0xe1, 0xe1, 0xe1));
	lv_style_set_text_font(&style_standby_label_touch_ww_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_standby_label_touch_ww_main_main_default, 2);
	lv_style_set_pad_left(&style_standby_label_touch_ww_main_main_default, 0);
	lv_style_set_pad_right(&style_standby_label_touch_ww_main_main_default, 0);
	lv_style_set_pad_top(&style_standby_label_touch_ww_main_main_default, 0);
	lv_style_set_pad_bottom(&style_standby_label_touch_ww_main_main_default, 0);
	lv_obj_add_style(ui->standby_label_touch_ww, &style_standby_label_touch_ww_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes standby_label_or
	ui->standby_label_or = lv_label_create(ui->standby_cont_help);
	lv_obj_set_pos(ui->standby_label_or, 10, 100);
	lv_obj_set_size(ui->standby_label_or, 570, 41);
	lv_label_set_text(ui->standby_label_or, "or");
	lv_label_set_long_mode(ui->standby_label_or, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->standby_label_or, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_standby_label_or_main_main_default
	static lv_style_t style_standby_label_or_main_main_default;
	if (style_standby_label_or_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_standby_label_or_main_main_default);
	else
		lv_style_init(&style_standby_label_or_main_main_default);
	lv_style_set_radius(&style_standby_label_or_main_main_default, 0);
	lv_style_set_bg_color(&style_standby_label_or_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_standby_label_or_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_standby_label_or_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_standby_label_or_main_main_default, 0);
	lv_style_set_text_color(&style_standby_label_or_main_main_default, lv_color_make(0xa0, 0xa0, 0xa0));
	lv_style_set_text_font(&style_standby_label_or_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_standby_label_or_main_main_default, 2);
	lv_style_set_pad_left(&style_standby_label_or_main_main_default, 0);
	lv_style_set_pad_right(&style_standby_label_or_main_main_default, 0);
	lv_style_set_pad_top(&style_standby_label_or_main_main_default, 0);
	lv_style_set_pad_bottom(&style_standby_label_or_main_main_default, 0);
	lv_obj_add_style(ui->standby_label_or, &style_standby_label_or_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes standby_label_voice_ww
	ui->standby_label_voice_ww = lv_label_create(ui->standby_cont_help);
	lv_obj_set_pos(ui->standby_label_voice_ww, 10, 29);
	lv_obj_set_size(ui->standby_label_voice_ww, 570, 41);
	lv_label_set_text(ui->standby_label_voice_ww, "Say, “Hey, NXP”");
	lv_label_set_long_mode(ui->standby_label_voice_ww, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->standby_label_voice_ww, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_standby_label_voice_ww_main_main_default
	static lv_style_t style_standby_label_voice_ww_main_main_default;
	if (style_standby_label_voice_ww_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_standby_label_voice_ww_main_main_default);
	else
		lv_style_init(&style_standby_label_voice_ww_main_main_default);
	lv_style_set_radius(&style_standby_label_voice_ww_main_main_default, 0);
	lv_style_set_bg_color(&style_standby_label_voice_ww_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_standby_label_voice_ww_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_standby_label_voice_ww_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_standby_label_voice_ww_main_main_default, 0);
	lv_style_set_text_color(&style_standby_label_voice_ww_main_main_default, lv_color_make(0xe1, 0xe1, 0xe1));
	lv_style_set_text_font(&style_standby_label_voice_ww_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_standby_label_voice_ww_main_main_default, 2);
	lv_style_set_pad_left(&style_standby_label_voice_ww_main_main_default, 0);
	lv_style_set_pad_right(&style_standby_label_voice_ww_main_main_default, 0);
	lv_style_set_pad_top(&style_standby_label_voice_ww_main_main_default, 0);
	lv_style_set_pad_bottom(&style_standby_label_voice_ww_main_main_default, 0);
	lv_obj_add_style(ui->standby_label_voice_ww, &style_standby_label_voice_ww_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Init events for screen
	events_init_standby(ui);
}
