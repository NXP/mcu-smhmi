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
	lv_style_set_bg_color(&style_home_ddlist_languagesel_extra_list_selected_default, lv_color_make(0xbe, 0x8c, 0x00));
	lv_style_set_bg_grad_color(&style_home_ddlist_languagesel_extra_list_selected_default, lv_color_make(0xbe, 0x8c, 0x00));
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
	lv_style_set_bg_color(&style_home_ddlist_languagesel_extra_list_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_home_ddlist_languagesel_extra_list_main_default, lv_color_make(0x00, 0x00, 0x00));
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
	lv_style_set_bg_color(&style_home_ddlist_languagesel_extra_list_scrollbar_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_home_ddlist_languagesel_extra_list_scrollbar_default, lv_color_make(0x00, 0x00, 0x00));
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

	//Write codes home_cont_background
	ui->home_cont_background = lv_obj_create(ui->home);
	lv_obj_set_pos(ui->home_cont_background, 0, 0);
	lv_obj_set_size(ui->home_cont_background, 1280, 720);

	//Write style state: LV_STATE_DEFAULT for style_home_cont_background_main_main_default
	static lv_style_t style_home_cont_background_main_main_default;
	if (style_home_cont_background_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_cont_background_main_main_default);
	else
		lv_style_init(&style_home_cont_background_main_main_default);
	lv_style_set_radius(&style_home_cont_background_main_main_default, 0);
	lv_style_set_bg_color(&style_home_cont_background_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_home_cont_background_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_home_cont_background_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_cont_background_main_main_default, 255);
	lv_style_set_border_color(&style_home_cont_background_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_home_cont_background_main_main_default, 0);
	lv_style_set_border_opa(&style_home_cont_background_main_main_default, 0);
	lv_style_set_pad_left(&style_home_cont_background_main_main_default, 0);
	lv_style_set_pad_right(&style_home_cont_background_main_main_default, 0);
	lv_style_set_pad_top(&style_home_cont_background_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_cont_background_main_main_default, 0);
	lv_obj_add_style(ui->home_cont_background, &style_home_cont_background_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_img_cameraPreview
	ui->home_img_cameraPreview = lv_img_create(ui->home_cont_background);
	lv_obj_set_pos(ui->home_img_cameraPreview, 10, 80);
	lv_obj_set_size(ui->home_img_cameraPreview, 640, 480);

	//Write style state: LV_STATE_DEFAULT for style_home_img_camerapreview_main_main_default
	static lv_style_t style_home_img_camerapreview_main_main_default;
	if (style_home_img_camerapreview_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_camerapreview_main_main_default);
	else
		lv_style_init(&style_home_img_camerapreview_main_main_default);
	lv_style_set_img_recolor(&style_home_img_camerapreview_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_camerapreview_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_camerapreview_main_main_default, 255);
	lv_obj_add_style(ui->home_img_cameraPreview, &style_home_img_camerapreview_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_img_nxpFaceRec
	ui->home_img_nxpFaceRec = lv_img_create(ui->home_cont_background);
	lv_obj_set_pos(ui->home_img_nxpFaceRec, 200, 0);
	lv_obj_set_size(ui->home_img_nxpFaceRec, 300, 80);

	//Write style state: LV_STATE_DEFAULT for style_home_img_nxpfacerec_main_main_default
	static lv_style_t style_home_img_nxpfacerec_main_main_default;
	if (style_home_img_nxpfacerec_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_nxpfacerec_main_main_default);
	else
		lv_style_init(&style_home_img_nxpfacerec_main_main_default);
	lv_style_set_img_recolor(&style_home_img_nxpfacerec_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_nxpfacerec_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_nxpfacerec_main_main_default, 255);
	lv_obj_add_style(ui->home_img_nxpFaceRec, &style_home_img_nxpfacerec_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->home_img_nxpFaceRec, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->home_img_nxpFaceRec,&_nxp_face_rec_300x80);
	lv_img_set_pivot(ui->home_img_nxpFaceRec, 0,0);
	lv_img_set_angle(ui->home_img_nxpFaceRec, 0);

	//Write codes home_img_nxpVoiceRec
	ui->home_img_nxpVoiceRec = lv_img_create(ui->home_cont_background);
	lv_obj_set_pos(ui->home_img_nxpVoiceRec, 765, 0);
	lv_obj_set_size(ui->home_img_nxpVoiceRec, 300, 80);

	//Write style state: LV_STATE_DEFAULT for style_home_img_nxpvoicerec_main_main_default
	static lv_style_t style_home_img_nxpvoicerec_main_main_default;
	if (style_home_img_nxpvoicerec_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_img_nxpvoicerec_main_main_default);
	else
		lv_style_init(&style_home_img_nxpvoicerec_main_main_default);
	lv_style_set_img_recolor(&style_home_img_nxpvoicerec_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_img_nxpvoicerec_main_main_default, 0);
	lv_style_set_img_opa(&style_home_img_nxpvoicerec_main_main_default, 255);
	lv_obj_add_style(ui->home_img_nxpVoiceRec, &style_home_img_nxpvoicerec_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->home_img_nxpVoiceRec, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->home_img_nxpVoiceRec,&_nxp_voice_rec_300x80);
	lv_img_set_pivot(ui->home_img_nxpVoiceRec, 0,0);
	lv_img_set_angle(ui->home_img_nxpVoiceRec, 0);

	//Write codes home_cont_size
	ui->home_cont_size = lv_obj_create(ui->home);
	lv_obj_set_pos(ui->home_cont_size, 660, 442);
	lv_obj_set_size(ui->home_cont_size, 290, 260);

	//Write style state: LV_STATE_DEFAULT for style_home_cont_size_main_main_default
	static lv_style_t style_home_cont_size_main_main_default;
	if (style_home_cont_size_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_cont_size_main_main_default);
	else
		lv_style_init(&style_home_cont_size_main_main_default);
	lv_style_set_radius(&style_home_cont_size_main_main_default, 10);
	lv_style_set_bg_color(&style_home_cont_size_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_cont_size_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_cont_size_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_cont_size_main_main_default, 0);
	lv_style_set_border_color(&style_home_cont_size_main_main_default, lv_color_make(0x96, 0x96, 0x96));
	lv_style_set_border_width(&style_home_cont_size_main_main_default, 1);
	lv_style_set_border_opa(&style_home_cont_size_main_main_default, 255);
	lv_style_set_pad_left(&style_home_cont_size_main_main_default, 0);
	lv_style_set_pad_right(&style_home_cont_size_main_main_default, 0);
	lv_style_set_pad_top(&style_home_cont_size_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_cont_size_main_main_default, 0);
	lv_obj_add_style(ui->home_cont_size, &style_home_cont_size_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_label_size_small
	ui->home_label_size_small = lv_label_create(ui->home_cont_size);
	lv_obj_set_pos(ui->home_label_size_small, 108, 35);
	lv_obj_set_size(ui->home_label_size_small, 160, 41);
	lv_label_set_text(ui->home_label_size_small, "Small");
	lv_label_set_long_mode(ui->home_label_size_small, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_size_small, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_size_small_main_main_default
	static lv_style_t style_home_label_size_small_main_main_default;
	if (style_home_label_size_small_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_size_small_main_main_default);
	else
		lv_style_init(&style_home_label_size_small_main_main_default);
	lv_style_set_radius(&style_home_label_size_small_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_size_small_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_home_label_size_small_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_home_label_size_small_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_size_small_main_main_default, 0);
	lv_style_set_text_color(&style_home_label_size_small_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_label_size_small_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_home_label_size_small_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_size_small_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_size_small_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_size_small_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_size_small_main_main_default, 0);
	lv_obj_add_style(ui->home_label_size_small, &style_home_label_size_small_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_label_size_medium
	ui->home_label_size_medium = lv_label_create(ui->home_cont_size);
	lv_obj_set_pos(ui->home_label_size_medium, 108, 122);
	lv_obj_set_size(ui->home_label_size_medium, 160, 41);
	lv_label_set_text(ui->home_label_size_medium, "Medium");
	lv_label_set_long_mode(ui->home_label_size_medium, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_size_medium, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_size_medium_main_main_default
	static lv_style_t style_home_label_size_medium_main_main_default;
	if (style_home_label_size_medium_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_size_medium_main_main_default);
	else
		lv_style_init(&style_home_label_size_medium_main_main_default);
	lv_style_set_radius(&style_home_label_size_medium_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_size_medium_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_label_size_medium_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_label_size_medium_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_size_medium_main_main_default, 0);
	lv_style_set_text_color(&style_home_label_size_medium_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_label_size_medium_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_home_label_size_medium_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_size_medium_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_size_medium_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_size_medium_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_size_medium_main_main_default, 0);
	lv_obj_add_style(ui->home_label_size_medium, &style_home_label_size_medium_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_label_size_large
	ui->home_label_size_large = lv_label_create(ui->home_cont_size);
	lv_obj_set_pos(ui->home_label_size_large, 108, 200);
	lv_obj_set_size(ui->home_label_size_large, 160, 41);
	lv_label_set_text(ui->home_label_size_large, "Large");
	lv_label_set_long_mode(ui->home_label_size_large, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_size_large, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_size_large_main_main_default
	static lv_style_t style_home_label_size_large_main_main_default;
	if (style_home_label_size_large_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_size_large_main_main_default);
	else
		lv_style_init(&style_home_label_size_large_main_main_default);
	lv_style_set_radius(&style_home_label_size_large_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_size_large_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_label_size_large_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_label_size_large_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_size_large_main_main_default, 0);
	lv_style_set_text_color(&style_home_label_size_large_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_label_size_large_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_home_label_size_large_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_size_large_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_size_large_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_size_large_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_size_large_main_main_default, 0);
	lv_obj_add_style(ui->home_label_size_large, &style_home_label_size_large_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_imgbtn_size_small
	ui->home_imgbtn_size_small = lv_imgbtn_create(ui->home_cont_size);
	lv_obj_set_pos(ui->home_imgbtn_size_small, 20, 20);
	lv_obj_set_size(ui->home_imgbtn_size_small, 80, 60);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_size_small_main_main_default
	static lv_style_t style_home_imgbtn_size_small_main_main_default;
	if (style_home_imgbtn_size_small_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_size_small_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_size_small_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_size_small_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_home_imgbtn_size_small_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_size_small_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_size_small_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_size_small, &style_home_imgbtn_size_small_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_size_small_main_main_pressed
	static lv_style_t style_home_imgbtn_size_small_main_main_pressed;
	if (style_home_imgbtn_size_small_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_size_small_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_size_small_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_size_small_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_size_small_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_size_small_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_size_small, &style_home_imgbtn_size_small_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_size_small_main_main_checked
	static lv_style_t style_home_imgbtn_size_small_main_main_checked;
	if (style_home_imgbtn_size_small_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_size_small_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_size_small_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_size_small_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_size_small_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_size_small_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_size_small, &style_home_imgbtn_size_small_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_size_small, LV_IMGBTN_STATE_RELEASED, NULL, &_Selection_80x60, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_size_small, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_Selection_Selected_80x60, NULL);
	lv_obj_add_flag(ui->home_imgbtn_size_small, LV_OBJ_FLAG_CHECKABLE);

	//Write codes home_imgbtn_size_medium
	ui->home_imgbtn_size_medium = lv_imgbtn_create(ui->home_cont_size);
	lv_obj_set_pos(ui->home_imgbtn_size_medium, 20, 105);
	lv_obj_set_size(ui->home_imgbtn_size_medium, 80, 60);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_size_medium_main_main_default
	static lv_style_t style_home_imgbtn_size_medium_main_main_default;
	if (style_home_imgbtn_size_medium_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_size_medium_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_size_medium_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_size_medium_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_home_imgbtn_size_medium_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_size_medium_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_size_medium_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_size_medium, &style_home_imgbtn_size_medium_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_size_medium_main_main_pressed
	static lv_style_t style_home_imgbtn_size_medium_main_main_pressed;
	if (style_home_imgbtn_size_medium_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_size_medium_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_size_medium_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_size_medium_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_size_medium_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_size_medium_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_size_medium, &style_home_imgbtn_size_medium_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_size_medium_main_main_checked
	static lv_style_t style_home_imgbtn_size_medium_main_main_checked;
	if (style_home_imgbtn_size_medium_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_size_medium_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_size_medium_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_size_medium_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_size_medium_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_size_medium_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_size_medium, &style_home_imgbtn_size_medium_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_size_medium, LV_IMGBTN_STATE_RELEASED, NULL, &_Selection_80x60, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_size_medium, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_Selection_Selected_80x60, NULL);
	lv_obj_add_flag(ui->home_imgbtn_size_medium, LV_OBJ_FLAG_CHECKABLE);

	//Write codes home_imgbtn_size_large
	ui->home_imgbtn_size_large = lv_imgbtn_create(ui->home_cont_size);
	lv_obj_set_pos(ui->home_imgbtn_size_large, 20, 185);
	lv_obj_set_size(ui->home_imgbtn_size_large, 80, 60);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_size_large_main_main_default
	static lv_style_t style_home_imgbtn_size_large_main_main_default;
	if (style_home_imgbtn_size_large_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_size_large_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_size_large_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_size_large_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_home_imgbtn_size_large_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_size_large_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_size_large_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_size_large, &style_home_imgbtn_size_large_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_size_large_main_main_pressed
	static lv_style_t style_home_imgbtn_size_large_main_main_pressed;
	if (style_home_imgbtn_size_large_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_size_large_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_size_large_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_size_large_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_size_large_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_size_large_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_size_large, &style_home_imgbtn_size_large_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_size_large_main_main_checked
	static lv_style_t style_home_imgbtn_size_large_main_main_checked;
	if (style_home_imgbtn_size_large_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_size_large_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_size_large_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_size_large_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_size_large_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_size_large_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_size_large, &style_home_imgbtn_size_large_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_size_large, LV_IMGBTN_STATE_RELEASED, NULL, &_Selection_80x60, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_size_large, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_Selection_Selected_80x60, NULL);
	lv_obj_add_flag(ui->home_imgbtn_size_large, LV_OBJ_FLAG_CHECKABLE);

	//Write codes home_cont_strength
	ui->home_cont_strength = lv_obj_create(ui->home);
	lv_obj_set_pos(ui->home_cont_strength, 980, 442);
	lv_obj_set_size(ui->home_cont_strength, 290, 260);

	//Write style state: LV_STATE_DEFAULT for style_home_cont_strength_main_main_default
	static lv_style_t style_home_cont_strength_main_main_default;
	if (style_home_cont_strength_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_cont_strength_main_main_default);
	else
		lv_style_init(&style_home_cont_strength_main_main_default);
	lv_style_set_radius(&style_home_cont_strength_main_main_default, 10);
	lv_style_set_bg_color(&style_home_cont_strength_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_cont_strength_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_cont_strength_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_cont_strength_main_main_default, 0);
	lv_style_set_border_color(&style_home_cont_strength_main_main_default, lv_color_make(0x96, 0x96, 0x96));
	lv_style_set_border_width(&style_home_cont_strength_main_main_default, 1);
	lv_style_set_border_opa(&style_home_cont_strength_main_main_default, 255);
	lv_style_set_pad_left(&style_home_cont_strength_main_main_default, 0);
	lv_style_set_pad_right(&style_home_cont_strength_main_main_default, 0);
	lv_style_set_pad_top(&style_home_cont_strength_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_cont_strength_main_main_default, 0);
	lv_obj_add_style(ui->home_cont_strength, &style_home_cont_strength_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_label_strength_soft
	ui->home_label_strength_soft = lv_label_create(ui->home_cont_strength);
	lv_obj_set_pos(ui->home_label_strength_soft, 108, 34);
	lv_obj_set_size(ui->home_label_strength_soft, 160, 41);
	lv_label_set_text(ui->home_label_strength_soft, "Soft");
	lv_label_set_long_mode(ui->home_label_strength_soft, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_strength_soft, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_strength_soft_main_main_default
	static lv_style_t style_home_label_strength_soft_main_main_default;
	if (style_home_label_strength_soft_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_strength_soft_main_main_default);
	else
		lv_style_init(&style_home_label_strength_soft_main_main_default);
	lv_style_set_radius(&style_home_label_strength_soft_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_strength_soft_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_label_strength_soft_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_label_strength_soft_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_strength_soft_main_main_default, 0);
	lv_style_set_text_color(&style_home_label_strength_soft_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_label_strength_soft_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_home_label_strength_soft_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_strength_soft_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_strength_soft_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_strength_soft_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_strength_soft_main_main_default, 0);
	lv_obj_add_style(ui->home_label_strength_soft, &style_home_label_strength_soft_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_label_strength_mild
	ui->home_label_strength_mild = lv_label_create(ui->home_cont_strength);
	lv_obj_set_pos(ui->home_label_strength_mild, 108, 122);
	lv_obj_set_size(ui->home_label_strength_mild, 160, 41);
	lv_label_set_text(ui->home_label_strength_mild, "Mild");
	lv_label_set_long_mode(ui->home_label_strength_mild, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_strength_mild, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_strength_mild_main_main_default
	static lv_style_t style_home_label_strength_mild_main_main_default;
	if (style_home_label_strength_mild_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_strength_mild_main_main_default);
	else
		lv_style_init(&style_home_label_strength_mild_main_main_default);
	lv_style_set_radius(&style_home_label_strength_mild_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_strength_mild_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_label_strength_mild_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_label_strength_mild_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_strength_mild_main_main_default, 0);
	lv_style_set_text_color(&style_home_label_strength_mild_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_label_strength_mild_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_home_label_strength_mild_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_strength_mild_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_strength_mild_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_strength_mild_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_strength_mild_main_main_default, 0);
	lv_obj_add_style(ui->home_label_strength_mild, &style_home_label_strength_mild_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_label_strength_strong
	ui->home_label_strength_strong = lv_label_create(ui->home_cont_strength);
	lv_obj_set_pos(ui->home_label_strength_strong, 108, 201);
	lv_obj_set_size(ui->home_label_strength_strong, 160, 41);
	lv_label_set_text(ui->home_label_strength_strong, "Strong");
	lv_label_set_long_mode(ui->home_label_strength_strong, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_strength_strong, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_strength_strong_main_main_default
	static lv_style_t style_home_label_strength_strong_main_main_default;
	if (style_home_label_strength_strong_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_strength_strong_main_main_default);
	else
		lv_style_init(&style_home_label_strength_strong_main_main_default);
	lv_style_set_radius(&style_home_label_strength_strong_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_strength_strong_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_label_strength_strong_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_label_strength_strong_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_strength_strong_main_main_default, 0);
	lv_style_set_text_color(&style_home_label_strength_strong_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_label_strength_strong_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_home_label_strength_strong_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_strength_strong_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_strength_strong_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_strength_strong_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_strength_strong_main_main_default, 0);
	lv_obj_add_style(ui->home_label_strength_strong, &style_home_label_strength_strong_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_imgbtn_strength_soft
	ui->home_imgbtn_strength_soft = lv_imgbtn_create(ui->home_cont_strength);
	lv_obj_set_pos(ui->home_imgbtn_strength_soft, 20, 20);
	lv_obj_set_size(ui->home_imgbtn_strength_soft, 80, 60);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_strength_soft_main_main_default
	static lv_style_t style_home_imgbtn_strength_soft_main_main_default;
	if (style_home_imgbtn_strength_soft_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_strength_soft_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_strength_soft_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_strength_soft_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_home_imgbtn_strength_soft_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_strength_soft_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_strength_soft_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_strength_soft, &style_home_imgbtn_strength_soft_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_strength_soft_main_main_pressed
	static lv_style_t style_home_imgbtn_strength_soft_main_main_pressed;
	if (style_home_imgbtn_strength_soft_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_strength_soft_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_strength_soft_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_strength_soft_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_strength_soft_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_strength_soft_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_strength_soft, &style_home_imgbtn_strength_soft_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_strength_soft_main_main_checked
	static lv_style_t style_home_imgbtn_strength_soft_main_main_checked;
	if (style_home_imgbtn_strength_soft_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_strength_soft_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_strength_soft_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_strength_soft_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_strength_soft_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_strength_soft_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_strength_soft, &style_home_imgbtn_strength_soft_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_strength_soft, LV_IMGBTN_STATE_RELEASED, NULL, &_Selection_80x60, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_strength_soft, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_Selection_Selected_80x60, NULL);
	lv_obj_add_flag(ui->home_imgbtn_strength_soft, LV_OBJ_FLAG_CHECKABLE);

	//Write codes home_imgbtn_strength_mild
	ui->home_imgbtn_strength_mild = lv_imgbtn_create(ui->home_cont_strength);
	lv_obj_set_pos(ui->home_imgbtn_strength_mild, 20, 105);
	lv_obj_set_size(ui->home_imgbtn_strength_mild, 80, 60);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_strength_mild_main_main_default
	static lv_style_t style_home_imgbtn_strength_mild_main_main_default;
	if (style_home_imgbtn_strength_mild_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_strength_mild_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_strength_mild_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_strength_mild_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_home_imgbtn_strength_mild_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_strength_mild_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_strength_mild_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_strength_mild, &style_home_imgbtn_strength_mild_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_strength_mild_main_main_pressed
	static lv_style_t style_home_imgbtn_strength_mild_main_main_pressed;
	if (style_home_imgbtn_strength_mild_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_strength_mild_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_strength_mild_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_strength_mild_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_strength_mild_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_strength_mild_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_strength_mild, &style_home_imgbtn_strength_mild_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_strength_mild_main_main_checked
	static lv_style_t style_home_imgbtn_strength_mild_main_main_checked;
	if (style_home_imgbtn_strength_mild_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_strength_mild_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_strength_mild_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_strength_mild_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_strength_mild_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_strength_mild_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_strength_mild, &style_home_imgbtn_strength_mild_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_strength_mild, LV_IMGBTN_STATE_RELEASED, NULL, &_Selection_80x60, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_strength_mild, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_Selection_Selected_80x60, NULL);
	lv_obj_add_flag(ui->home_imgbtn_strength_mild, LV_OBJ_FLAG_CHECKABLE);

	//Write codes home_imgbtn_strength_strong
	ui->home_imgbtn_strength_strong = lv_imgbtn_create(ui->home_cont_strength);
	lv_obj_set_pos(ui->home_imgbtn_strength_strong, 20, 185);
	lv_obj_set_size(ui->home_imgbtn_strength_strong, 80, 60);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_strength_strong_main_main_default
	static lv_style_t style_home_imgbtn_strength_strong_main_main_default;
	if (style_home_imgbtn_strength_strong_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_strength_strong_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_strength_strong_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_strength_strong_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_home_imgbtn_strength_strong_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_strength_strong_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_strength_strong_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_strength_strong, &style_home_imgbtn_strength_strong_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_strength_strong_main_main_pressed
	static lv_style_t style_home_imgbtn_strength_strong_main_main_pressed;
	if (style_home_imgbtn_strength_strong_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_strength_strong_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_strength_strong_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_strength_strong_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_strength_strong_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_strength_strong_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_strength_strong, &style_home_imgbtn_strength_strong_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_strength_strong_main_main_checked
	static lv_style_t style_home_imgbtn_strength_strong_main_main_checked;
	if (style_home_imgbtn_strength_strong_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_strength_strong_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_strength_strong_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_strength_strong_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_strength_strong_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_strength_strong_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_strength_strong, &style_home_imgbtn_strength_strong_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_strength_strong, LV_IMGBTN_STATE_RELEASED, NULL, &_Selection_80x60, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_strength_strong, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_Selection_Selected_80x60, NULL);
	lv_obj_add_flag(ui->home_imgbtn_strength_strong, LV_OBJ_FLAG_CHECKABLE);

	//Write codes home_imgbtn_start
	ui->home_imgbtn_start = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_start, 189, 577);
	lv_obj_set_size(ui->home_imgbtn_start, 270, 120);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_start_main_main_default
	static lv_style_t style_home_imgbtn_start_main_main_default;
	if (style_home_imgbtn_start_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_start_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_start_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_start_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_home_imgbtn_start_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_start_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_start_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_start, &style_home_imgbtn_start_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_start_main_main_pressed
	static lv_style_t style_home_imgbtn_start_main_main_pressed;
	if (style_home_imgbtn_start_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_start_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_start_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_start_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_start_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_start_main_main_pressed, 90);
	lv_obj_add_style(ui->home_imgbtn_start, &style_home_imgbtn_start_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_start_main_main_checked
	static lv_style_t style_home_imgbtn_start_main_main_checked;
	if (style_home_imgbtn_start_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_start_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_start_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_start_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_start_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_start_main_main_checked, 178);
	lv_obj_add_style(ui->home_imgbtn_start, &style_home_imgbtn_start_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_start, LV_IMGBTN_STATE_RELEASED, NULL, &_Start_Black_Reshaped_270x120, NULL);
	lv_obj_add_flag(ui->home_imgbtn_start, LV_OBJ_FLAG_CHECKABLE);

	//Write codes home_cont_type
	ui->home_cont_type = lv_obj_create(ui->home);
	lv_obj_set_pos(ui->home_cont_type, 660, 77);
	lv_obj_set_size(ui->home_cont_type, 610, 344);

	//Write style state: LV_STATE_DEFAULT for style_home_cont_type_main_main_default
	static lv_style_t style_home_cont_type_main_main_default;
	if (style_home_cont_type_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_cont_type_main_main_default);
	else
		lv_style_init(&style_home_cont_type_main_main_default);
	lv_style_set_radius(&style_home_cont_type_main_main_default, 10);
	lv_style_set_bg_color(&style_home_cont_type_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_home_cont_type_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_home_cont_type_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_cont_type_main_main_default, 0);
	lv_style_set_border_color(&style_home_cont_type_main_main_default, lv_color_make(0x96, 0x96, 0x96));
	lv_style_set_border_width(&style_home_cont_type_main_main_default, 1);
	lv_style_set_border_opa(&style_home_cont_type_main_main_default, 255);
	lv_style_set_pad_left(&style_home_cont_type_main_main_default, 0);
	lv_style_set_pad_right(&style_home_cont_type_main_main_default, 0);
	lv_style_set_pad_top(&style_home_cont_type_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_cont_type_main_main_default, 0);
	lv_obj_add_style(ui->home_cont_type, &style_home_cont_type_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_imgbtn_americano
	ui->home_imgbtn_americano = lv_imgbtn_create(ui->home_cont_type);
	lv_obj_set_pos(ui->home_imgbtn_americano, 108, 11);
	lv_obj_set_size(ui->home_imgbtn_americano, 120, 120);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_americano_main_main_default
	static lv_style_t style_home_imgbtn_americano_main_main_default;
	if (style_home_imgbtn_americano_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_americano_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_americano_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_americano_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_home_imgbtn_americano_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_americano_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_americano_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_americano, &style_home_imgbtn_americano_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_americano_main_main_pressed
	static lv_style_t style_home_imgbtn_americano_main_main_pressed;
	if (style_home_imgbtn_americano_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_americano_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_americano_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_americano_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_americano_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_americano_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_americano, &style_home_imgbtn_americano_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_americano_main_main_checked
	static lv_style_t style_home_imgbtn_americano_main_main_checked;
	if (style_home_imgbtn_americano_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_americano_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_americano_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_americano_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_americano_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_americano_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_americano, &style_home_imgbtn_americano_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_americano, LV_IMGBTN_STATE_RELEASED, NULL, &_Americano_Black_120x120, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_americano, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_Americano_Selected_Black_120x120, NULL);
	lv_obj_add_flag(ui->home_imgbtn_americano, LV_OBJ_FLAG_CHECKABLE);

	//Write codes home_label_cafelatte
	ui->home_label_cafelatte = lv_label_create(ui->home_cont_type);
	lv_obj_set_pos(ui->home_label_cafelatte, 325, 297);
	lv_obj_set_size(ui->home_label_cafelatte, 260, 36);
	lv_label_set_text(ui->home_label_cafelatte, "Cafe Latte");
	lv_label_set_long_mode(ui->home_label_cafelatte, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_cafelatte, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_cafelatte_main_main_default
	static lv_style_t style_home_label_cafelatte_main_main_default;
	if (style_home_label_cafelatte_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_cafelatte_main_main_default);
	else
		lv_style_init(&style_home_label_cafelatte_main_main_default);
	lv_style_set_radius(&style_home_label_cafelatte_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_cafelatte_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_label_cafelatte_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_label_cafelatte_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_cafelatte_main_main_default, 0);
	lv_style_set_text_color(&style_home_label_cafelatte_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_label_cafelatte_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_home_label_cafelatte_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_cafelatte_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_cafelatte_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_cafelatte_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_cafelatte_main_main_default, 0);
	lv_obj_add_style(ui->home_label_cafelatte, &style_home_label_cafelatte_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_label_espresso
	ui->home_label_espresso = lv_label_create(ui->home_cont_type);
	lv_obj_set_pos(ui->home_label_espresso, 47, 295);
	lv_obj_set_size(ui->home_label_espresso, 260, 36);
	lv_label_set_text(ui->home_label_espresso, "Espresso");
	lv_label_set_long_mode(ui->home_label_espresso, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_espresso, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_espresso_main_main_default
	static lv_style_t style_home_label_espresso_main_main_default;
	if (style_home_label_espresso_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_espresso_main_main_default);
	else
		lv_style_init(&style_home_label_espresso_main_main_default);
	lv_style_set_radius(&style_home_label_espresso_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_espresso_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_label_espresso_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_label_espresso_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_espresso_main_main_default, 0);
	lv_style_set_text_color(&style_home_label_espresso_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_label_espresso_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_home_label_espresso_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_espresso_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_espresso_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_espresso_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_espresso_main_main_default, 0);
	lv_obj_add_style(ui->home_label_espresso, &style_home_label_espresso_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_label_cappuccino
	ui->home_label_cappuccino = lv_label_create(ui->home_cont_type);
	lv_obj_set_pos(ui->home_label_cappuccino, 327, 133);
	lv_obj_set_size(ui->home_label_cappuccino, 260, 36);
	lv_label_set_text(ui->home_label_cappuccino, "Cappuccino");
	lv_label_set_long_mode(ui->home_label_cappuccino, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_cappuccino, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_cappuccino_main_main_default
	static lv_style_t style_home_label_cappuccino_main_main_default;
	if (style_home_label_cappuccino_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_cappuccino_main_main_default);
	else
		lv_style_init(&style_home_label_cappuccino_main_main_default);
	lv_style_set_radius(&style_home_label_cappuccino_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_cappuccino_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_label_cappuccino_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_label_cappuccino_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_cappuccino_main_main_default, 0);
	lv_style_set_text_color(&style_home_label_cappuccino_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_label_cappuccino_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_home_label_cappuccino_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_cappuccino_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_cappuccino_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_cappuccino_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_cappuccino_main_main_default, 0);
	lv_obj_add_style(ui->home_label_cappuccino, &style_home_label_cappuccino_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_label_americano
	ui->home_label_americano = lv_label_create(ui->home_cont_type);
	lv_obj_set_pos(ui->home_label_americano, 9, 133);
	lv_obj_set_size(ui->home_label_americano, 310, 36);
	lv_label_set_text(ui->home_label_americano, "Americano");
	lv_label_set_long_mode(ui->home_label_americano, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_americano, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_americano_main_main_default
	static lv_style_t style_home_label_americano_main_main_default;
	if (style_home_label_americano_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_americano_main_main_default);
	else
		lv_style_init(&style_home_label_americano_main_main_default);
	lv_style_set_radius(&style_home_label_americano_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_americano_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_label_americano_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_label_americano_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_americano_main_main_default, 0);
	lv_style_set_text_color(&style_home_label_americano_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_label_americano_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_home_label_americano_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_americano_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_americano_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_americano_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_americano_main_main_default, 0);
	lv_obj_add_style(ui->home_label_americano, &style_home_label_americano_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_imgbtn_caffelatte
	ui->home_imgbtn_caffelatte = lv_imgbtn_create(ui->home_cont_type);
	lv_obj_set_pos(ui->home_imgbtn_caffelatte, 392, 170);
	lv_obj_set_size(ui->home_imgbtn_caffelatte, 120, 120);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_caffelatte_main_main_default
	static lv_style_t style_home_imgbtn_caffelatte_main_main_default;
	if (style_home_imgbtn_caffelatte_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_caffelatte_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_caffelatte_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_caffelatte_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_home_imgbtn_caffelatte_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_caffelatte_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_caffelatte_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_caffelatte, &style_home_imgbtn_caffelatte_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_caffelatte_main_main_pressed
	static lv_style_t style_home_imgbtn_caffelatte_main_main_pressed;
	if (style_home_imgbtn_caffelatte_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_caffelatte_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_caffelatte_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_caffelatte_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_caffelatte_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_caffelatte_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_caffelatte, &style_home_imgbtn_caffelatte_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_caffelatte_main_main_checked
	static lv_style_t style_home_imgbtn_caffelatte_main_main_checked;
	if (style_home_imgbtn_caffelatte_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_caffelatte_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_caffelatte_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_caffelatte_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_caffelatte_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_caffelatte_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_caffelatte, &style_home_imgbtn_caffelatte_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_caffelatte, LV_IMGBTN_STATE_RELEASED, NULL, &_Cafe_Latte_Black_120x120, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_caffelatte, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_Cafe_Latte_Selected_Black_120x120, NULL);
	lv_obj_add_flag(ui->home_imgbtn_caffelatte, LV_OBJ_FLAG_CHECKABLE);

	//Write codes home_imgbtn_espresso
	ui->home_imgbtn_espresso = lv_imgbtn_create(ui->home_cont_type);
	lv_obj_set_pos(ui->home_imgbtn_espresso, 112, 170);
	lv_obj_set_size(ui->home_imgbtn_espresso, 120, 120);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_espresso_main_main_default
	static lv_style_t style_home_imgbtn_espresso_main_main_default;
	if (style_home_imgbtn_espresso_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_espresso_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_espresso_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_espresso_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_home_imgbtn_espresso_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_espresso_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_espresso_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_espresso, &style_home_imgbtn_espresso_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_espresso_main_main_pressed
	static lv_style_t style_home_imgbtn_espresso_main_main_pressed;
	if (style_home_imgbtn_espresso_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_espresso_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_espresso_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_espresso_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_espresso_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_espresso_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_espresso, &style_home_imgbtn_espresso_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_espresso_main_main_checked
	static lv_style_t style_home_imgbtn_espresso_main_main_checked;
	if (style_home_imgbtn_espresso_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_espresso_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_espresso_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_espresso_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_espresso_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_espresso_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_espresso, &style_home_imgbtn_espresso_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_espresso, LV_IMGBTN_STATE_RELEASED, NULL, &_Espresso_Black_120x120, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_espresso, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_Espresso_Selected_Black_120x120, NULL);
	lv_obj_add_flag(ui->home_imgbtn_espresso, LV_OBJ_FLAG_CHECKABLE);

	//Write codes home_imgbtn_cappuccino
	ui->home_imgbtn_cappuccino = lv_imgbtn_create(ui->home_cont_type);
	lv_obj_set_pos(ui->home_imgbtn_cappuccino, 392, 11);
	lv_obj_set_size(ui->home_imgbtn_cappuccino, 120, 120);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_cappuccino_main_main_default
	static lv_style_t style_home_imgbtn_cappuccino_main_main_default;
	if (style_home_imgbtn_cappuccino_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_cappuccino_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_cappuccino_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_cappuccino_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_home_imgbtn_cappuccino_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_cappuccino_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_cappuccino_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_cappuccino, &style_home_imgbtn_cappuccino_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_cappuccino_main_main_pressed
	static lv_style_t style_home_imgbtn_cappuccino_main_main_pressed;
	if (style_home_imgbtn_cappuccino_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_cappuccino_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_cappuccino_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_cappuccino_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_cappuccino_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_cappuccino_main_main_pressed, 0);
	lv_obj_add_style(ui->home_imgbtn_cappuccino, &style_home_imgbtn_cappuccino_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_cappuccino_main_main_checked
	static lv_style_t style_home_imgbtn_cappuccino_main_main_checked;
	if (style_home_imgbtn_cappuccino_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_cappuccino_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_cappuccino_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_cappuccino_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_cappuccino_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_cappuccino_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_cappuccino, &style_home_imgbtn_cappuccino_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_cappuccino, LV_IMGBTN_STATE_RELEASED, NULL, &_Cappuccinno_Black_120x120, NULL);
	lv_imgbtn_set_src(ui->home_imgbtn_cappuccino, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_Cappuccinno_selected_Black_120x120, NULL);
	lv_obj_add_flag(ui->home_imgbtn_cappuccino, LV_OBJ_FLAG_CHECKABLE);

	//Write codes home_imgbtn_unregister
	ui->home_imgbtn_unregister = lv_imgbtn_create(ui->home);
	lv_obj_set_pos(ui->home_imgbtn_unregister, 45, 0);
	lv_obj_set_size(ui->home_imgbtn_unregister, 75, 75);

	//Write style state: LV_STATE_DEFAULT for style_home_imgbtn_unregister_main_main_default
	static lv_style_t style_home_imgbtn_unregister_main_main_default;
	if (style_home_imgbtn_unregister_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_unregister_main_main_default);
	else
		lv_style_init(&style_home_imgbtn_unregister_main_main_default);
	lv_style_set_text_color(&style_home_imgbtn_unregister_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_home_imgbtn_unregister_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_unregister_main_main_default, 0);
	lv_style_set_img_opa(&style_home_imgbtn_unregister_main_main_default, 255);
	lv_obj_add_style(ui->home_imgbtn_unregister, &style_home_imgbtn_unregister_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_home_imgbtn_unregister_main_main_pressed
	static lv_style_t style_home_imgbtn_unregister_main_main_pressed;
	if (style_home_imgbtn_unregister_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_unregister_main_main_pressed);
	else
		lv_style_init(&style_home_imgbtn_unregister_main_main_pressed);
	lv_style_set_text_color(&style_home_imgbtn_unregister_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_unregister_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_unregister_main_main_pressed, 138);
	lv_obj_add_style(ui->home_imgbtn_unregister, &style_home_imgbtn_unregister_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_home_imgbtn_unregister_main_main_checked
	static lv_style_t style_home_imgbtn_unregister_main_main_checked;
	if (style_home_imgbtn_unregister_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_home_imgbtn_unregister_main_main_checked);
	else
		lv_style_init(&style_home_imgbtn_unregister_main_main_checked);
	lv_style_set_text_color(&style_home_imgbtn_unregister_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_home_imgbtn_unregister_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_home_imgbtn_unregister_main_main_checked, 0);
	lv_obj_add_style(ui->home_imgbtn_unregister, &style_home_imgbtn_unregister_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->home_imgbtn_unregister, LV_IMGBTN_STATE_RELEASED, NULL, &_unregister_75x75, NULL);
	lv_obj_add_flag(ui->home_imgbtn_unregister, LV_OBJ_FLAG_CHECKABLE);

	//Write codes home_ddlist_languageSel
	ui->home_ddlist_languageSel = lv_dropdown_create(ui->home);
	lv_obj_set_pos(ui->home_ddlist_languageSel, 1090, 10);
	lv_obj_set_width(ui->home_ddlist_languageSel, 185);
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
	lv_style_set_radius(&style_home_ddlist_languagesel_main_main_default, 2);
	lv_style_set_bg_color(&style_home_ddlist_languagesel_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_home_ddlist_languagesel_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_home_ddlist_languagesel_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_ddlist_languagesel_main_main_default, 255);
	lv_style_set_border_color(&style_home_ddlist_languagesel_main_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_home_ddlist_languagesel_main_main_default, 2);
	lv_style_set_text_color(&style_home_ddlist_languagesel_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_ddlist_languagesel_main_main_default, &lv_font_simsun_32);
	lv_style_set_text_line_space(&style_home_ddlist_languagesel_main_main_default, 20);
	lv_obj_add_style(ui->home_ddlist_languageSel, &style_home_ddlist_languagesel_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_label_algoinfo
	ui->home_label_algoinfo = lv_label_create(ui->home);
	lv_obj_set_pos(ui->home_label_algoinfo, 520, 20);
	lv_obj_set_size(ui->home_label_algoinfo, 400, 40);
	lv_label_set_text(ui->home_label_algoinfo, "invalid");
	lv_label_set_long_mode(ui->home_label_algoinfo, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_algoinfo, LV_TEXT_ALIGN_LEFT, 0);

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
	lv_style_set_text_font(&style_home_label_algoinfo_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_home_label_algoinfo_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_algoinfo_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_algoinfo_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_algoinfo_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_algoinfo_main_main_default, 0);
	lv_obj_add_style(ui->home_label_algoinfo, &style_home_label_algoinfo_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_label_size
	ui->home_label_size = lv_label_create(ui->home);
	lv_obj_set_pos(ui->home_label_size, 680, 429);
	lv_obj_set_size(ui->home_label_size, 65, 25);
	lv_label_set_text(ui->home_label_size, "Size:");
	lv_label_set_long_mode(ui->home_label_size, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_size, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_size_main_main_default
	static lv_style_t style_home_label_size_main_main_default;
	if (style_home_label_size_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_size_main_main_default);
	else
		lv_style_init(&style_home_label_size_main_main_default);
	lv_style_set_radius(&style_home_label_size_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_size_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_home_label_size_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_home_label_size_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_size_main_main_default, 255);
	lv_style_set_text_color(&style_home_label_size_main_main_default, lv_color_make(0xb4, 0xb4, 0xb4));
	lv_style_set_text_font(&style_home_label_size_main_main_default, &lv_font_montserratMedium_20);
	lv_style_set_text_letter_space(&style_home_label_size_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_size_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_size_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_size_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_size_main_main_default, 0);
	lv_obj_add_style(ui->home_label_size, &style_home_label_size_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_label_strength
	ui->home_label_strength = lv_label_create(ui->home);
	lv_obj_set_pos(ui->home_label_strength, 1000, 429);
	lv_obj_set_size(ui->home_label_strength, 122, 25);
	lv_label_set_text(ui->home_label_strength, "Strength:");
	lv_label_set_long_mode(ui->home_label_strength, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_strength, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_strength_main_main_default
	static lv_style_t style_home_label_strength_main_main_default;
	if (style_home_label_strength_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_strength_main_main_default);
	else
		lv_style_init(&style_home_label_strength_main_main_default);
	lv_style_set_radius(&style_home_label_strength_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_strength_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_home_label_strength_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_home_label_strength_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_strength_main_main_default, 255);
	lv_style_set_text_color(&style_home_label_strength_main_main_default, lv_color_make(0xb4, 0xb4, 0xb4));
	lv_style_set_text_font(&style_home_label_strength_main_main_default, &lv_font_montserratMedium_20);
	lv_style_set_text_letter_space(&style_home_label_strength_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_strength_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_strength_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_strength_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_strength_main_main_default, 0);
	lv_obj_add_style(ui->home_label_strength, &style_home_label_strength_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes home_label_start
	ui->home_label_start = lv_label_create(ui->home);
	lv_obj_set_pos(ui->home_label_start, 194, 621);
	lv_obj_set_size(ui->home_label_start, 257, 41);
	lv_label_set_text(ui->home_label_start, "START");
	lv_label_set_long_mode(ui->home_label_start, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->home_label_start, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_home_label_start_main_main_default
	static lv_style_t style_home_label_start_main_main_default;
	if (style_home_label_start_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_home_label_start_main_main_default);
	else
		lv_style_init(&style_home_label_start_main_main_default);
	lv_style_set_radius(&style_home_label_start_main_main_default, 0);
	lv_style_set_bg_color(&style_home_label_start_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_label_start_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_home_label_start_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_label_start_main_main_default, 0);
	lv_style_set_text_color(&style_home_label_start_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_home_label_start_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_home_label_start_main_main_default, 2);
	lv_style_set_pad_left(&style_home_label_start_main_main_default, 0);
	lv_style_set_pad_right(&style_home_label_start_main_main_default, 0);
	lv_style_set_pad_top(&style_home_label_start_main_main_default, 0);
	lv_style_set_pad_bottom(&style_home_label_start_main_main_default, 0);
	lv_obj_add_style(ui->home_label_start, &style_home_label_start_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Init events for screen
	events_init_home(ui);
}
