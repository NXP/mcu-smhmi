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


void setup_scr_screen_security_arming(lv_ui *ui){

	//Write codes screen_security_arming
	ui->screen_security_arming = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->screen_security_arming, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_arming_main_main_default
	static lv_style_t style_screen_security_arming_main_main_default;
	if (style_screen_security_arming_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_arming_main_main_default);
	else
		lv_style_init(&style_screen_security_arming_main_main_default);
	lv_style_set_bg_color(&style_screen_security_arming_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_screen_security_arming_main_main_default, 0);
	lv_obj_add_style(ui->screen_security_arming, &style_screen_security_arming_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_security_arming_cont_background
	ui->screen_security_arming_cont_background = lv_obj_create(ui->screen_security_arming);
	lv_obj_set_pos(ui->screen_security_arming_cont_background, 0, 0);
	lv_obj_set_size(ui->screen_security_arming_cont_background, 1280, 720);
	lv_obj_set_scrollbar_mode(ui->screen_security_arming_cont_background, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_arming_cont_background_main_main_default
	static lv_style_t style_screen_security_arming_cont_background_main_main_default;
	if (style_screen_security_arming_cont_background_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_arming_cont_background_main_main_default);
	else
		lv_style_init(&style_screen_security_arming_cont_background_main_main_default);
	lv_style_set_radius(&style_screen_security_arming_cont_background_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_security_arming_cont_background_main_main_default, lv_color_make(0xfc, 0xfc, 0xfc));
	lv_style_set_bg_grad_color(&style_screen_security_arming_cont_background_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_security_arming_cont_background_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_security_arming_cont_background_main_main_default, 255);
	lv_style_set_shadow_width(&style_screen_security_arming_cont_background_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_arming_cont_background_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_arming_cont_background_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_arming_cont_background_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_arming_cont_background_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_arming_cont_background_main_main_default, 0);
	lv_style_set_border_color(&style_screen_security_arming_cont_background_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_screen_security_arming_cont_background_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_security_arming_cont_background_main_main_default, 255);
	lv_style_set_pad_left(&style_screen_security_arming_cont_background_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_security_arming_cont_background_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_security_arming_cont_background_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_security_arming_cont_background_main_main_default, 0);
	lv_obj_add_style(ui->screen_security_arming_cont_background, &style_screen_security_arming_cont_background_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_security_arming_img_background
	ui->screen_security_arming_img_background = lv_img_create(ui->screen_security_arming_cont_background);
	lv_obj_set_pos(ui->screen_security_arming_img_background, 0, 0);
	lv_obj_set_size(ui->screen_security_arming_img_background, 1280, 630);
	lv_obj_set_scrollbar_mode(ui->screen_security_arming_img_background, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_arming_img_background_main_main_default
	static lv_style_t style_screen_security_arming_img_background_main_main_default;
	if (style_screen_security_arming_img_background_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_arming_img_background_main_main_default);
	else
		lv_style_init(&style_screen_security_arming_img_background_main_main_default);
	lv_style_set_img_recolor(&style_screen_security_arming_img_background_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_security_arming_img_background_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_security_arming_img_background_main_main_default, 255);
	lv_obj_add_style(ui->screen_security_arming_img_background, &style_screen_security_arming_img_background_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_security_arming_img_background, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_security_arming_img_background,&_main_background_1280x630);
	lv_img_set_pivot(ui->screen_security_arming_img_background, 0,0);
	lv_img_set_angle(ui->screen_security_arming_img_background, 0);

	//Write codes screen_security_arming_img_background_bottom
	ui->screen_security_arming_img_background_bottom = lv_img_create(ui->screen_security_arming_cont_background);
	lv_obj_set_pos(ui->screen_security_arming_img_background_bottom, 0, 630);
	lv_obj_set_size(ui->screen_security_arming_img_background_bottom, 1280, 90);
	lv_obj_set_scrollbar_mode(ui->screen_security_arming_img_background_bottom, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_arming_img_background_bottom_main_main_default
	static lv_style_t style_screen_security_arming_img_background_bottom_main_main_default;
	if (style_screen_security_arming_img_background_bottom_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_arming_img_background_bottom_main_main_default);
	else
		lv_style_init(&style_screen_security_arming_img_background_bottom_main_main_default);
	lv_style_set_img_recolor(&style_screen_security_arming_img_background_bottom_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_security_arming_img_background_bottom_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_security_arming_img_background_bottom_main_main_default, 255);
	lv_obj_add_style(ui->screen_security_arming_img_background_bottom, &style_screen_security_arming_img_background_bottom_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_security_arming_img_background_bottom, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_security_arming_img_background_bottom,&_other_background_bottom_1280x90);
	lv_img_set_pivot(ui->screen_security_arming_img_background_bottom, 50,50);
	lv_img_set_angle(ui->screen_security_arming_img_background_bottom, 0);

	//Write codes screen_security_arming_img_armed
	ui->screen_security_arming_img_armed = lv_img_create(ui->screen_security_arming_cont_background);
	lv_obj_set_pos(ui->screen_security_arming_img_armed, 30, 114);
	lv_obj_set_size(ui->screen_security_arming_img_armed, 324, 278);
	lv_obj_set_scrollbar_mode(ui->screen_security_arming_img_armed, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_security_arming_img_armed, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_security_arming_img_armed,&_security_armed_324x278);
	lv_img_set_pivot(ui->screen_security_arming_img_armed, 0,0);
	lv_img_set_angle(ui->screen_security_arming_img_armed, 0);

	//Write codes screen_security_arming_img_disarmed
	ui->screen_security_arming_img_disarmed = lv_img_create(ui->screen_security_arming_cont_background);
	lv_obj_set_pos(ui->screen_security_arming_img_disarmed, 30, 114);
	lv_obj_set_size(ui->screen_security_arming_img_disarmed, 324, 278);
	lv_obj_set_scrollbar_mode(ui->screen_security_arming_img_disarmed, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_security_arming_img_disarmed, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_security_arming_img_disarmed,&_security_disarmed_324x278);
	lv_img_set_pivot(ui->screen_security_arming_img_disarmed, 0,0);
	lv_img_set_angle(ui->screen_security_arming_img_disarmed, 0);

 //Write codes screen_security_arming_img_timer_greed
	ui->screen_security_arming_img_timer_greed = lv_img_create(ui->screen_security_arming_cont_background);
	lv_obj_set_pos(ui->screen_security_arming_img_timer_greed, 487, 244);
	lv_obj_set_size(ui->screen_security_arming_img_timer_greed, 301, 302);
	lv_obj_set_scrollbar_mode(ui->screen_security_arming_img_timer_greed, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_arming_img_timer_greed_main_main_default
	static lv_style_t style_screen_security_arming_img_timer_greed_main_main_default;
	if (style_screen_security_arming_img_timer_greed_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_arming_img_timer_greed_main_main_default);
	else
		lv_style_init(&style_screen_security_arming_img_timer_greed_main_main_default);
	lv_style_set_img_recolor(&style_screen_security_arming_img_timer_greed_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_security_arming_img_timer_greed_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_security_arming_img_timer_greed_main_main_default, 255);
	lv_obj_add_style(ui->screen_security_arming_img_timer_greed, &style_screen_security_arming_img_timer_greed_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_security_arming_img_timer_greed, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_security_arming_img_timer_greed,&_security_timer_green_301x302);
	lv_img_set_pivot(ui->screen_security_arming_img_timer_greed, 50,50);
	lv_img_set_angle(ui->screen_security_arming_img_timer_greed, 0);

	//Write codes screen_security_arming_img_timer_red
	ui->screen_security_arming_img_timer_red = lv_img_create(ui->screen_security_arming_cont_background);
	lv_obj_set_pos(ui->screen_security_arming_img_timer_red, 487, 244);
	lv_obj_set_size(ui->screen_security_arming_img_timer_red, 301, 302);
	lv_obj_set_scrollbar_mode(ui->screen_security_arming_img_timer_red, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_arming_img_timer_red_main_main_default
	static lv_style_t style_screen_security_arming_img_timer_red_main_main_default;
	if (style_screen_security_arming_img_timer_red_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_arming_img_timer_red_main_main_default);
	else
		lv_style_init(&style_screen_security_arming_img_timer_red_main_main_default);
	lv_style_set_img_recolor(&style_screen_security_arming_img_timer_red_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_security_arming_img_timer_red_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_security_arming_img_timer_red_main_main_default, 255);
	lv_obj_add_style(ui->screen_security_arming_img_timer_red, &style_screen_security_arming_img_timer_red_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_security_arming_img_timer_red, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_security_arming_img_timer_red,&_security_timer_red_301x302);
	lv_img_set_pivot(ui->screen_security_arming_img_timer_red, 50,50);
	lv_img_set_angle(ui->screen_security_arming_img_timer_red, 0);

	//Write codes screen_security_arming_arc_arming_progress
	ui->screen_security_arming_arc_arming_progress = lv_arc_create(ui->screen_security_arming_cont_background);
	lv_obj_set_pos(ui->screen_security_arming_arc_arming_progress, 410, 175);
	lv_obj_set_size(ui->screen_security_arming_arc_arming_progress, 469, 458);
	lv_obj_set_scrollbar_mode(ui->screen_security_arming_arc_arming_progress, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_arming_arc_arming_progress_main_main_default
	static lv_style_t style_screen_security_arming_arc_arming_progress_main_main_default;
	if (style_screen_security_arming_arc_arming_progress_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_arming_arc_arming_progress_main_main_default);
	else
		lv_style_init(&style_screen_security_arming_arc_arming_progress_main_main_default);
	lv_style_set_radius(&style_screen_security_arming_arc_arming_progress_main_main_default, 6);
	lv_style_set_bg_color(&style_screen_security_arming_arc_arming_progress_main_main_default, lv_color_make(0xf6, 0xf6, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_security_arming_arc_arming_progress_main_main_default, lv_color_make(0xf6, 0xf6, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_security_arming_arc_arming_progress_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_security_arming_arc_arming_progress_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_security_arming_arc_arming_progress_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_arming_arc_arming_progress_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_arming_arc_arming_progress_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_arming_arc_arming_progress_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_arming_arc_arming_progress_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_arming_arc_arming_progress_main_main_default, 0);
	lv_style_set_border_width(&style_screen_security_arming_arc_arming_progress_main_main_default, 0);
	lv_style_set_pad_left(&style_screen_security_arming_arc_arming_progress_main_main_default, 20);
	lv_style_set_pad_right(&style_screen_security_arming_arc_arming_progress_main_main_default, 20);
	lv_style_set_pad_top(&style_screen_security_arming_arc_arming_progress_main_main_default, 20);
	lv_style_set_pad_bottom(&style_screen_security_arming_arc_arming_progress_main_main_default, 20);
	lv_style_set_arc_color(&style_screen_security_arming_arc_arming_progress_main_main_default, lv_color_make(0x87, 0x2c, 0x2c));
	lv_style_set_arc_width(&style_screen_security_arming_arc_arming_progress_main_main_default, 8);
	lv_obj_add_style(ui->screen_security_arming_arc_arming_progress, &style_screen_security_arming_arc_arming_progress_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_arming_arc_arming_progress_main_indicator_default
	static lv_style_t style_screen_security_arming_arc_arming_progress_main_indicator_default;
	if (style_screen_security_arming_arc_arming_progress_main_indicator_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_arming_arc_arming_progress_main_indicator_default);
	else
		lv_style_init(&style_screen_security_arming_arc_arming_progress_main_indicator_default);
	lv_style_set_arc_color(&style_screen_security_arming_arc_arming_progress_main_indicator_default, lv_color_make(0xe6, 0xfa, 0x43));
	lv_style_set_arc_width(&style_screen_security_arming_arc_arming_progress_main_indicator_default, 8);
	lv_obj_add_style(ui->screen_security_arming_arc_arming_progress, &style_screen_security_arming_arc_arming_progress_main_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_arming_arc_arming_progress_main_knob_default
	static lv_style_t style_screen_security_arming_arc_arming_progress_main_knob_default;
	if (style_screen_security_arming_arc_arming_progress_main_knob_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_arming_arc_arming_progress_main_knob_default);
	else
		lv_style_init(&style_screen_security_arming_arc_arming_progress_main_knob_default);
	lv_style_set_bg_color(&style_screen_security_arming_arc_arming_progress_main_knob_default, lv_color_make(0xE6, 0xFA, 0x43));
	lv_style_set_bg_grad_color(&style_screen_security_arming_arc_arming_progress_main_knob_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_security_arming_arc_arming_progress_main_knob_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_security_arming_arc_arming_progress_main_knob_default, 255);
	lv_style_set_pad_all(&style_screen_security_arming_arc_arming_progress_main_knob_default, 0);
	lv_obj_add_style(ui->screen_security_arming_arc_arming_progress, &style_screen_security_arming_arc_arming_progress_main_knob_default, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_arc_set_mode(ui->screen_security_arming_arc_arming_progress, LV_ARC_MODE_NORMAL);
	lv_arc_set_range(ui->screen_security_arming_arc_arming_progress, 0, 100);
	lv_arc_set_bg_angles(ui->screen_security_arming_arc_arming_progress, 0, 280);
	lv_arc_set_angles(ui->screen_security_arming_arc_arming_progress, 0, 0);
	lv_arc_set_rotation(ui->screen_security_arming_arc_arming_progress, 130);
	lv_obj_set_style_arc_rounded(ui->screen_security_arming_arc_arming_progress, 0,  LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_rounded(ui->screen_security_arming_arc_arming_progress, 0, LV_STATE_DEFAULT);

	//Write codes screen_security_arming_label_countdown
	ui->screen_security_arming_label_countdown = lv_label_create(ui->screen_security_arming_cont_background);
	lv_obj_set_pos(ui->screen_security_arming_label_countdown, 235, 596);
	lv_obj_set_size(ui->screen_security_arming_label_countdown, 848, 48);
	lv_obj_set_scrollbar_mode(ui->screen_security_arming_label_countdown, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_security_arming_label_countdown, "Your Home is disarmed now");
	lv_label_set_long_mode(ui->screen_security_arming_label_countdown, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_arming_label_countdown_main_main_default
	static lv_style_t style_screen_security_arming_label_countdown_main_main_default;
	if (style_screen_security_arming_label_countdown_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_arming_label_countdown_main_main_default);
	else
		lv_style_init(&style_screen_security_arming_label_countdown_main_main_default);
	lv_style_set_radius(&style_screen_security_arming_label_countdown_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_security_arming_label_countdown_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_security_arming_label_countdown_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_security_arming_label_countdown_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_security_arming_label_countdown_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_security_arming_label_countdown_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_arming_label_countdown_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_arming_label_countdown_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_arming_label_countdown_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_arming_label_countdown_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_arming_label_countdown_main_main_default, 0);
	lv_style_set_text_color(&style_screen_security_arming_label_countdown_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_security_arming_label_countdown_main_main_default, &lv_font_montserratMedium_38);
	lv_style_set_text_letter_space(&style_screen_security_arming_label_countdown_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_security_arming_label_countdown_main_main_default, 0);
	lv_style_set_text_align(&style_screen_security_arming_label_countdown_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_security_arming_label_countdown_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_security_arming_label_countdown_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_security_arming_label_countdown_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_security_arming_label_countdown_main_main_default, 0);
	lv_obj_add_style(ui->screen_security_arming_label_countdown, &style_screen_security_arming_label_countdown_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_security_arming_label_title
	ui->screen_security_arming_label_title = lv_label_create(ui->screen_security_arming);
	lv_obj_set_pos(ui->screen_security_arming_label_title, 230, 30);
	lv_obj_set_size(ui->screen_security_arming_label_title, 760, 60);
	lv_obj_set_scrollbar_mode(ui->screen_security_arming_label_title, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_security_arming_label_title, "SECURITY CONTROL");
	lv_label_set_long_mode(ui->screen_security_arming_label_title, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_security_arming_label_title_main_main_default
	static lv_style_t style_screen_security_arming_label_title_main_main_default;
	if (style_screen_security_arming_label_title_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_security_arming_label_title_main_main_default);
	else
		lv_style_init(&style_screen_security_arming_label_title_main_main_default);
	lv_style_set_radius(&style_screen_security_arming_label_title_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_security_arming_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_security_arming_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_security_arming_label_title_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_security_arming_label_title_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_security_arming_label_title_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_security_arming_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_security_arming_label_title_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_security_arming_label_title_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_security_arming_label_title_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_security_arming_label_title_main_main_default, 0);
	lv_style_set_text_color(&style_screen_security_arming_label_title_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_security_arming_label_title_main_main_default, &lv_font_montserratMedium_50);
	lv_style_set_text_letter_space(&style_screen_security_arming_label_title_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_security_arming_label_title_main_main_default, 0);
	lv_style_set_text_align(&style_screen_security_arming_label_title_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_security_arming_label_title_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_security_arming_label_title_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_security_arming_label_title_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_security_arming_label_title_main_main_default, 0);
	lv_obj_add_style(ui->screen_security_arming_label_title, &style_screen_security_arming_label_title_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Init events for screen
	events_init_screen_security_arming(ui);
}
