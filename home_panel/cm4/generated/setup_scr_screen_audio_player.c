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


void setup_scr_screen_audio_player(lv_ui *ui){

	//Write codes screen_audio_player
	ui->screen_audio_player = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_main_main_default
	static lv_style_t style_screen_audio_player_main_main_default;
	if (style_screen_audio_player_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_main_main_default);
	lv_style_set_bg_color(&style_screen_audio_player_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_screen_audio_player_main_main_default, 0);
	lv_obj_add_style(ui->screen_audio_player, &style_screen_audio_player_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_audio_player_cont_background
	ui->screen_audio_player_cont_background = lv_obj_create(ui->screen_audio_player);
	lv_obj_set_pos(ui->screen_audio_player_cont_background, 0, 0);
	lv_obj_set_size(ui->screen_audio_player_cont_background, 1280, 720);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_cont_background, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_cont_background_main_main_default
	static lv_style_t style_screen_audio_player_cont_background_main_main_default;
	if (style_screen_audio_player_cont_background_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_cont_background_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_cont_background_main_main_default);
	lv_style_set_radius(&style_screen_audio_player_cont_background_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_audio_player_cont_background_main_main_default, lv_color_make(0xfc, 0xfc, 0xfc));
	lv_style_set_bg_grad_color(&style_screen_audio_player_cont_background_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_audio_player_cont_background_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_audio_player_cont_background_main_main_default, 255);
	lv_style_set_shadow_width(&style_screen_audio_player_cont_background_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_cont_background_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_cont_background_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_cont_background_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_cont_background_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_cont_background_main_main_default, 0);
	lv_style_set_border_color(&style_screen_audio_player_cont_background_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_screen_audio_player_cont_background_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_audio_player_cont_background_main_main_default, 255);
	lv_style_set_pad_left(&style_screen_audio_player_cont_background_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_audio_player_cont_background_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_audio_player_cont_background_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_audio_player_cont_background_main_main_default, 0);
	lv_obj_add_style(ui->screen_audio_player_cont_background, &style_screen_audio_player_cont_background_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_audio_player_img_background
	ui->screen_audio_player_img_background = lv_img_create(ui->screen_audio_player_cont_background);
	lv_obj_set_pos(ui->screen_audio_player_img_background, 0, 0);
	lv_obj_set_size(ui->screen_audio_player_img_background, 1280, 720);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_img_background, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_img_background_main_main_default
	static lv_style_t style_screen_audio_player_img_background_main_main_default;
	if (style_screen_audio_player_img_background_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_img_background_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_img_background_main_main_default);
	lv_style_set_img_recolor(&style_screen_audio_player_img_background_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_img_background_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_audio_player_img_background_main_main_default, 255);
	lv_obj_add_style(ui->screen_audio_player_img_background, &style_screen_audio_player_img_background_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_audio_player_img_background, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_audio_player_img_background,&_audio_player_background_1280x720);
	lv_img_set_pivot(ui->screen_audio_player_img_background, 0,0);
	lv_img_set_angle(ui->screen_audio_player_img_background, 0);

	//Write codes screen_audio_player_arc_progress
	ui->screen_audio_player_arc_progress = lv_arc_create(ui->screen_audio_player_cont_background);
	lv_obj_set_pos(ui->screen_audio_player_arc_progress, 418, 183);
	lv_obj_set_size(ui->screen_audio_player_arc_progress, 452, 448);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_arc_progress, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_arc_progress_main_main_default
	static lv_style_t style_screen_audio_player_arc_progress_main_main_default;
	if (style_screen_audio_player_arc_progress_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_arc_progress_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_arc_progress_main_main_default);
	lv_style_set_radius(&style_screen_audio_player_arc_progress_main_main_default, 6);
	lv_style_set_bg_color(&style_screen_audio_player_arc_progress_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_audio_player_arc_progress_main_main_default, lv_color_make(0xf6, 0xf6, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_audio_player_arc_progress_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_audio_player_arc_progress_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_audio_player_arc_progress_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_arc_progress_main_main_default, lv_color_make(0x05, 0x12, 0x2d));
	lv_style_set_shadow_opa(&style_screen_audio_player_arc_progress_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_arc_progress_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_arc_progress_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_arc_progress_main_main_default, 0);
	lv_style_set_border_width(&style_screen_audio_player_arc_progress_main_main_default, 0);
	lv_style_set_pad_left(&style_screen_audio_player_arc_progress_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_audio_player_arc_progress_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_audio_player_arc_progress_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_audio_player_arc_progress_main_main_default, 0);
	lv_style_set_arc_color(&style_screen_audio_player_arc_progress_main_main_default, lv_color_make(0x99, 0x9d, 0xbc));
	lv_style_set_arc_width(&style_screen_audio_player_arc_progress_main_main_default, 12);
	lv_obj_add_style(ui->screen_audio_player_arc_progress, &style_screen_audio_player_arc_progress_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_FOCUSED for style_screen_audio_player_arc_progress_main_main_focused
	static lv_style_t style_screen_audio_player_arc_progress_main_main_focused;
	if (style_screen_audio_player_arc_progress_main_main_focused.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_arc_progress_main_main_focused);
	else
		lv_style_init(&style_screen_audio_player_arc_progress_main_main_focused);
	lv_style_set_radius(&style_screen_audio_player_arc_progress_main_main_focused, 6);
	lv_style_set_bg_color(&style_screen_audio_player_arc_progress_main_main_focused, lv_color_make(0xf6, 0xf6, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_audio_player_arc_progress_main_main_focused, lv_color_make(0xf6, 0xf6, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_audio_player_arc_progress_main_main_focused, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_audio_player_arc_progress_main_main_focused, 0);
	lv_style_set_shadow_width(&style_screen_audio_player_arc_progress_main_main_focused, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_arc_progress_main_main_focused, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_arc_progress_main_main_focused, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_arc_progress_main_main_focused, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_arc_progress_main_main_focused, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_arc_progress_main_main_focused, 0);
	lv_style_set_border_width(&style_screen_audio_player_arc_progress_main_main_focused, 0);
	lv_style_set_pad_left(&style_screen_audio_player_arc_progress_main_main_focused, 0);
	lv_style_set_pad_right(&style_screen_audio_player_arc_progress_main_main_focused, 0);
	lv_style_set_pad_top(&style_screen_audio_player_arc_progress_main_main_focused, 0);
	lv_style_set_pad_bottom(&style_screen_audio_player_arc_progress_main_main_focused, 0);
	lv_style_set_arc_color(&style_screen_audio_player_arc_progress_main_main_focused, lv_color_make(0x99, 0x9D, 0xBc));
	lv_obj_add_style(ui->screen_audio_player_arc_progress, &style_screen_audio_player_arc_progress_main_main_focused, LV_PART_MAIN|LV_STATE_FOCUSED);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_arc_progress_main_indicator_default
	static lv_style_t style_screen_audio_player_arc_progress_main_indicator_default;
	if (style_screen_audio_player_arc_progress_main_indicator_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_arc_progress_main_indicator_default);
	else
		lv_style_init(&style_screen_audio_player_arc_progress_main_indicator_default);
	lv_style_set_arc_color(&style_screen_audio_player_arc_progress_main_indicator_default, lv_color_make(0xe0, 0x96, 0x53));
	lv_style_set_arc_width(&style_screen_audio_player_arc_progress_main_indicator_default, 12);
	lv_obj_add_style(ui->screen_audio_player_arc_progress, &style_screen_audio_player_arc_progress_main_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_FOCUSED for style_screen_audio_player_arc_progress_main_indicator_focused
	static lv_style_t style_screen_audio_player_arc_progress_main_indicator_focused;
	if (style_screen_audio_player_arc_progress_main_indicator_focused.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_arc_progress_main_indicator_focused);
	else
		lv_style_init(&style_screen_audio_player_arc_progress_main_indicator_focused);
	lv_style_set_arc_color(&style_screen_audio_player_arc_progress_main_indicator_focused, lv_color_make(0xE0, 0x96, 0x53));
	lv_obj_add_style(ui->screen_audio_player_arc_progress, &style_screen_audio_player_arc_progress_main_indicator_focused, LV_PART_INDICATOR|LV_STATE_FOCUSED);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_arc_progress_main_knob_default
	static lv_style_t style_screen_audio_player_arc_progress_main_knob_default;
	if (style_screen_audio_player_arc_progress_main_knob_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_arc_progress_main_knob_default);
	else
		lv_style_init(&style_screen_audio_player_arc_progress_main_knob_default);
	lv_style_set_bg_color(&style_screen_audio_player_arc_progress_main_knob_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_audio_player_arc_progress_main_knob_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_audio_player_arc_progress_main_knob_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_audio_player_arc_progress_main_knob_default, 0);
	lv_style_set_pad_all(&style_screen_audio_player_arc_progress_main_knob_default, 0);
	lv_obj_add_style(ui->screen_audio_player_arc_progress, &style_screen_audio_player_arc_progress_main_knob_default, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_arc_set_mode(ui->screen_audio_player_arc_progress, LV_ARC_MODE_NORMAL);
	lv_arc_set_range(ui->screen_audio_player_arc_progress, 0, 100);
	lv_arc_set_bg_angles(ui->screen_audio_player_arc_progress, 0, 280);
	lv_arc_set_angles(ui->screen_audio_player_arc_progress, 0, 0);
	lv_arc_set_rotation(ui->screen_audio_player_arc_progress, 130);

	//Write codes screen_audio_player_imgbtn_play
	ui->screen_audio_player_imgbtn_play = lv_imgbtn_create(ui->screen_audio_player_cont_background);
	lv_obj_set_pos(ui->screen_audio_player_imgbtn_play, 572, 310);
	lv_obj_set_size(ui->screen_audio_player_imgbtn_play, 170, 190);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_imgbtn_play, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_audio_player_imgbtn_play, LV_IMGBTN_STATE_RELEASED, NULL, &_audio_player_play_170x190, NULL);

	//Write codes screen_audio_player_imgbtn_volume_up
	ui->screen_audio_player_imgbtn_volume_up = lv_imgbtn_create(ui->screen_audio_player_cont_background);
	lv_obj_set_pos(ui->screen_audio_player_imgbtn_volume_up, 30, 246);
	lv_obj_set_size(ui->screen_audio_player_imgbtn_volume_up, 144, 130);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_imgbtn_volume_up, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_imgbtn_volume_up_main_main_default
	static lv_style_t style_screen_audio_player_imgbtn_volume_up_main_main_default;
	if (style_screen_audio_player_imgbtn_volume_up_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_volume_up_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_volume_up_main_main_default);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_volume_up_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_volume_up_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_volume_up_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_volume_up_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_volume_up_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_volume_up_main_main_default, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_volume_up_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_align(&style_screen_audio_player_imgbtn_volume_up_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_volume_up_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_volume_up_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_volume_up_main_main_default, 255);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_volume_up, &style_screen_audio_player_imgbtn_volume_up_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_screen_audio_player_imgbtn_volume_up_main_main_pressed
	static lv_style_t style_screen_audio_player_imgbtn_volume_up_main_main_pressed;
	if (style_screen_audio_player_imgbtn_volume_up_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_volume_up_main_main_pressed);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_volume_up_main_main_pressed);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_volume_up_main_main_pressed, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_volume_up_main_main_pressed, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_volume_up_main_main_pressed, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_volume_up_main_main_pressed, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_volume_up_main_main_pressed, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_volume_up_main_main_pressed, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_volume_up_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_volume_up_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_volume_up_main_main_pressed, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_volume_up_main_main_pressed, 130);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_volume_up, &style_screen_audio_player_imgbtn_volume_up_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_screen_audio_player_imgbtn_volume_up_main_main_checked
	static lv_style_t style_screen_audio_player_imgbtn_volume_up_main_main_checked;
	if (style_screen_audio_player_imgbtn_volume_up_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_volume_up_main_main_checked);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_volume_up_main_main_checked);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_volume_up_main_main_checked, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_volume_up_main_main_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_volume_up_main_main_checked, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_volume_up_main_main_checked, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_volume_up_main_main_checked, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_volume_up_main_main_checked, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_volume_up_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_volume_up_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_volume_up_main_main_checked, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_volume_up_main_main_checked, 255);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_volume_up, &style_screen_audio_player_imgbtn_volume_up_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->screen_audio_player_imgbtn_volume_up, LV_IMGBTN_STATE_RELEASED, NULL, &_audio_player_volume_up_144x130, NULL);
	lv_obj_add_flag(ui->screen_audio_player_imgbtn_volume_up, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_audio_player_imgbtn_volume_down
	ui->screen_audio_player_imgbtn_volume_down = lv_imgbtn_create(ui->screen_audio_player_cont_background);
	lv_obj_set_pos(ui->screen_audio_player_imgbtn_volume_down, 30, 450);
	lv_obj_set_size(ui->screen_audio_player_imgbtn_volume_down, 144, 130);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_imgbtn_volume_down, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_imgbtn_volume_down_main_main_default
	static lv_style_t style_screen_audio_player_imgbtn_volume_down_main_main_default;
	if (style_screen_audio_player_imgbtn_volume_down_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_volume_down_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_volume_down_main_main_default);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_volume_down_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_volume_down_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_volume_down_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_volume_down_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_volume_down_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_volume_down_main_main_default, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_volume_down_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_align(&style_screen_audio_player_imgbtn_volume_down_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_volume_down_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_volume_down_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_volume_down_main_main_default, 255);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_volume_down, &style_screen_audio_player_imgbtn_volume_down_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_screen_audio_player_imgbtn_volume_down_main_main_pressed
	static lv_style_t style_screen_audio_player_imgbtn_volume_down_main_main_pressed;
	if (style_screen_audio_player_imgbtn_volume_down_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_volume_down_main_main_pressed);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_volume_down_main_main_pressed);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_volume_down_main_main_pressed, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_volume_down_main_main_pressed, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_volume_down_main_main_pressed, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_volume_down_main_main_pressed, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_volume_down_main_main_pressed, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_volume_down_main_main_pressed, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_volume_down_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_volume_down_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_volume_down_main_main_pressed, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_volume_down_main_main_pressed, 130);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_volume_down, &style_screen_audio_player_imgbtn_volume_down_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_screen_audio_player_imgbtn_volume_down_main_main_checked
	static lv_style_t style_screen_audio_player_imgbtn_volume_down_main_main_checked;
	if (style_screen_audio_player_imgbtn_volume_down_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_volume_down_main_main_checked);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_volume_down_main_main_checked);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_volume_down_main_main_checked, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_volume_down_main_main_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_volume_down_main_main_checked, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_volume_down_main_main_checked, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_volume_down_main_main_checked, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_volume_down_main_main_checked, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_volume_down_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_volume_down_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_volume_down_main_main_checked, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_volume_down_main_main_checked, 255);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_volume_down, &style_screen_audio_player_imgbtn_volume_down_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->screen_audio_player_imgbtn_volume_down, LV_IMGBTN_STATE_RELEASED, NULL, &_audio_player_volume_down_144x130, NULL);
	lv_obj_add_flag(ui->screen_audio_player_imgbtn_volume_down, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_audio_player_label_volume
	ui->screen_audio_player_label_volume = lv_label_create(ui->screen_audio_player_cont_background);
	lv_obj_set_pos(ui->screen_audio_player_label_volume, 25, 389);
	lv_obj_set_size(ui->screen_audio_player_label_volume, 148, 45);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_label_volume, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_audio_player_label_volume, "40/100");
	lv_label_set_long_mode(ui->screen_audio_player_label_volume, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_label_volume_main_main_default
	static lv_style_t style_screen_audio_player_label_volume_main_main_default;
	if (style_screen_audio_player_label_volume_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_label_volume_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_label_volume_main_main_default);
	lv_style_set_radius(&style_screen_audio_player_label_volume_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_audio_player_label_volume_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_audio_player_label_volume_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_audio_player_label_volume_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_audio_player_label_volume_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_audio_player_label_volume_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_label_volume_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_label_volume_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_label_volume_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_label_volume_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_label_volume_main_main_default, 0);
	lv_style_set_text_color(&style_screen_audio_player_label_volume_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_audio_player_label_volume_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_screen_audio_player_label_volume_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_audio_player_label_volume_main_main_default, 0);
	lv_style_set_text_align(&style_screen_audio_player_label_volume_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_audio_player_label_volume_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_audio_player_label_volume_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_audio_player_label_volume_main_main_default, 8);
	lv_style_set_pad_bottom(&style_screen_audio_player_label_volume_main_main_default, 0);
	lv_obj_add_style(ui->screen_audio_player_label_volume, &style_screen_audio_player_label_volume_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_audio_player_img_mic_off
	ui->screen_audio_player_img_mic_off = lv_img_create(ui->screen_audio_player_cont_background);
	lv_obj_set_pos(ui->screen_audio_player_img_mic_off, 1170, 594);
	lv_obj_set_size(ui->screen_audio_player_img_mic_off, 70, 112);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_img_mic_off, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_img_mic_off_main_main_default
	static lv_style_t style_screen_audio_player_img_mic_off_main_main_default;
	if (style_screen_audio_player_img_mic_off_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_img_mic_off_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_img_mic_off_main_main_default);
	lv_style_set_img_recolor(&style_screen_audio_player_img_mic_off_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_img_mic_off_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_audio_player_img_mic_off_main_main_default, 255);
	lv_obj_add_style(ui->screen_audio_player_img_mic_off, &style_screen_audio_player_img_mic_off_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_audio_player_img_mic_off, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_audio_player_img_mic_off,&_audio_player_mic_off_70x112);
	lv_img_set_pivot(ui->screen_audio_player_img_mic_off, 0,0);
	lv_img_set_angle(ui->screen_audio_player_img_mic_off, 0);

	//Write codes screen_audio_player_img_mic_on
	ui->screen_audio_player_img_mic_on = lv_img_create(ui->screen_audio_player_cont_background);
	lv_obj_set_pos(ui->screen_audio_player_img_mic_on, 1170, 594);
	lv_obj_set_size(ui->screen_audio_player_img_mic_on, 70, 112);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_img_mic_on, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_img_mic_on_main_main_default
	static lv_style_t style_screen_audio_player_img_mic_on_main_main_default;
	if (style_screen_audio_player_img_mic_on_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_img_mic_on_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_img_mic_on_main_main_default);
	lv_style_set_img_recolor(&style_screen_audio_player_img_mic_on_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_img_mic_on_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_audio_player_img_mic_on_main_main_default, 255);
	lv_obj_add_style(ui->screen_audio_player_img_mic_on, &style_screen_audio_player_img_mic_on_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_audio_player_img_mic_on, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_audio_player_img_mic_on,&_auido_player_mic_on_70x112);
	lv_img_set_pivot(ui->screen_audio_player_img_mic_on, 0,0);
	lv_img_set_angle(ui->screen_audio_player_img_mic_on, 0);

	//Write codes screen_audio_player_ddlist_language
	ui->screen_audio_player_ddlist_language = lv_dropdown_create(ui->screen_audio_player_cont_background);
	lv_obj_set_pos(ui->screen_audio_player_ddlist_language, 1053, 30);
	lv_obj_set_size(ui->screen_audio_player_ddlist_language, 193, 50);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_ddlist_language, LV_SCROLLBAR_MODE_OFF);
	lv_dropdown_set_options(ui->screen_audio_player_ddlist_language, "English\n中文");

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_ddlist_language_main_main_default
	static lv_style_t style_screen_audio_player_ddlist_language_main_main_default;
	if (style_screen_audio_player_ddlist_language_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_ddlist_language_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_ddlist_language_main_main_default);
	lv_style_set_radius(&style_screen_audio_player_ddlist_language_main_main_default, 3);
	lv_style_set_bg_color(&style_screen_audio_player_ddlist_language_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_audio_player_ddlist_language_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_audio_player_ddlist_language_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_audio_player_ddlist_language_main_main_default, 255);
	lv_style_set_shadow_width(&style_screen_audio_player_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_ddlist_language_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_ddlist_language_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_ddlist_language_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_ddlist_language_main_main_default, 0);
	lv_style_set_border_color(&style_screen_audio_player_ddlist_language_main_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_audio_player_ddlist_language_main_main_default, 1);
	lv_style_set_border_opa(&style_screen_audio_player_ddlist_language_main_main_default, 255);
	lv_style_set_text_color(&style_screen_audio_player_ddlist_language_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_audio_player_ddlist_language_main_main_default, &lv_font_simsun_32);
	lv_style_set_pad_left(&style_screen_audio_player_ddlist_language_main_main_default, 6);
	lv_style_set_pad_right(&style_screen_audio_player_ddlist_language_main_main_default, 6);
	lv_style_set_pad_top(&style_screen_audio_player_ddlist_language_main_main_default, 8);
	lv_obj_add_style(ui->screen_audio_player_ddlist_language, &style_screen_audio_player_ddlist_language_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_CHECKED for style_screen_audio_player_ddlist_language_extra_list_selected_checked
	static lv_style_t style_screen_audio_player_ddlist_language_extra_list_selected_checked;
	if (style_screen_audio_player_ddlist_language_extra_list_selected_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_ddlist_language_extra_list_selected_checked);
	else
		lv_style_init(&style_screen_audio_player_ddlist_language_extra_list_selected_checked);
	lv_style_set_radius(&style_screen_audio_player_ddlist_language_extra_list_selected_checked, 3);
	lv_style_set_bg_color(&style_screen_audio_player_ddlist_language_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_color(&style_screen_audio_player_ddlist_language_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_screen_audio_player_ddlist_language_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_audio_player_ddlist_language_extra_list_selected_checked, 255);
	lv_style_set_border_color(&style_screen_audio_player_ddlist_language_extra_list_selected_checked, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_audio_player_ddlist_language_extra_list_selected_checked, 1);
	lv_style_set_border_opa(&style_screen_audio_player_ddlist_language_extra_list_selected_checked, 255);
	lv_style_set_text_color(&style_screen_audio_player_ddlist_language_extra_list_selected_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_audio_player_ddlist_language_extra_list_selected_checked, &lv_font_simsun_32);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_audio_player_ddlist_language), &style_screen_audio_player_ddlist_language_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_ddlist_language_extra_list_main_default
	static lv_style_t style_screen_audio_player_ddlist_language_extra_list_main_default;
	if (style_screen_audio_player_ddlist_language_extra_list_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_ddlist_language_extra_list_main_default);
	else
		lv_style_init(&style_screen_audio_player_ddlist_language_extra_list_main_default);
	lv_style_set_radius(&style_screen_audio_player_ddlist_language_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_screen_audio_player_ddlist_language_extra_list_main_default, lv_color_make(0x8c, 0xa0, 0xa0));
	lv_style_set_bg_grad_color(&style_screen_audio_player_ddlist_language_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_audio_player_ddlist_language_extra_list_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_audio_player_ddlist_language_extra_list_main_default, 255);
	lv_style_set_border_color(&style_screen_audio_player_ddlist_language_extra_list_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_audio_player_ddlist_language_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_screen_audio_player_ddlist_language_extra_list_main_default, 255);
	lv_style_set_text_color(&style_screen_audio_player_ddlist_language_extra_list_main_default, lv_color_make(0x0D, 0x30, 0x55));
	lv_style_set_text_font(&style_screen_audio_player_ddlist_language_extra_list_main_default, &lv_font_simsun_32);
	lv_style_set_max_height(&style_screen_audio_player_ddlist_language_extra_list_main_default, 150);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_audio_player_ddlist_language), &style_screen_audio_player_ddlist_language_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_ddlist_language_extra_list_scrollbar_default
	static lv_style_t style_screen_audio_player_ddlist_language_extra_list_scrollbar_default;
	if (style_screen_audio_player_ddlist_language_extra_list_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_ddlist_language_extra_list_scrollbar_default);
	else
		lv_style_init(&style_screen_audio_player_ddlist_language_extra_list_scrollbar_default);
	lv_style_set_radius(&style_screen_audio_player_ddlist_language_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_screen_audio_player_ddlist_language_extra_list_scrollbar_default, lv_color_make(0xaf, 0xc3, 0xc3));
	lv_style_set_bg_opa(&style_screen_audio_player_ddlist_language_extra_list_scrollbar_default, 255);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_audio_player_ddlist_language), &style_screen_audio_player_ddlist_language_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes screen_audio_player_imgbtn_pause
	ui->screen_audio_player_imgbtn_pause = lv_imgbtn_create(ui->screen_audio_player_cont_background);
	lv_obj_set_pos(ui->screen_audio_player_imgbtn_pause, 572, 310);
	lv_obj_set_size(ui->screen_audio_player_imgbtn_pause, 170, 190);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_imgbtn_pause, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_imgbtn_pause_main_main_default
	static lv_style_t style_screen_audio_player_imgbtn_pause_main_main_default;
	if (style_screen_audio_player_imgbtn_pause_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_pause_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_pause_main_main_default);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_pause_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_pause_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_pause_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_pause_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_pause_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_pause_main_main_default, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_pause_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_align(&style_screen_audio_player_imgbtn_pause_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_pause_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_pause_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_pause_main_main_default, 255);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_pause, &style_screen_audio_player_imgbtn_pause_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_screen_audio_player_imgbtn_pause_main_main_pressed
	static lv_style_t style_screen_audio_player_imgbtn_pause_main_main_pressed;
	if (style_screen_audio_player_imgbtn_pause_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_pause_main_main_pressed);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_pause_main_main_pressed);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_pause_main_main_pressed, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_pause_main_main_pressed, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_pause_main_main_pressed, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_pause_main_main_pressed, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_pause_main_main_pressed, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_pause_main_main_pressed, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_pause_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_pause_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_pause_main_main_pressed, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_pause_main_main_pressed, 255);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_pause, &style_screen_audio_player_imgbtn_pause_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_screen_audio_player_imgbtn_pause_main_main_checked
	static lv_style_t style_screen_audio_player_imgbtn_pause_main_main_checked;
	if (style_screen_audio_player_imgbtn_pause_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_pause_main_main_checked);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_pause_main_main_checked);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_pause_main_main_checked, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_pause_main_main_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_pause_main_main_checked, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_pause_main_main_checked, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_pause_main_main_checked, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_pause_main_main_checked, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_pause_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_pause_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_pause_main_main_checked, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_pause_main_main_checked, 255);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_pause, &style_screen_audio_player_imgbtn_pause_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->screen_audio_player_imgbtn_pause, LV_IMGBTN_STATE_RELEASED, NULL, &_audio_player_pause_170x190, NULL);
	lv_obj_add_flag(ui->screen_audio_player_imgbtn_pause, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_audio_player_label_audio_name
	ui->screen_audio_player_label_audio_name = lv_label_create(ui->screen_audio_player_cont_background);
	lv_obj_set_pos(ui->screen_audio_player_label_audio_name, 40, 120);
	lv_obj_set_size(ui->screen_audio_player_label_audio_name, 1200, 40);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_label_audio_name, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_audio_player_label_audio_name, "Forever young (dance mix)");
	lv_label_set_long_mode(ui->screen_audio_player_label_audio_name, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_label_audio_name_main_main_default
	static lv_style_t style_screen_audio_player_label_audio_name_main_main_default;
	if (style_screen_audio_player_label_audio_name_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_label_audio_name_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_label_audio_name_main_main_default);
	lv_style_set_radius(&style_screen_audio_player_label_audio_name_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_audio_player_label_audio_name_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_audio_player_label_audio_name_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_audio_player_label_audio_name_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_audio_player_label_audio_name_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_audio_player_label_audio_name_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_label_audio_name_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_label_audio_name_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_label_audio_name_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_label_audio_name_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_label_audio_name_main_main_default, 0);
	lv_style_set_text_color(&style_screen_audio_player_label_audio_name_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_audio_player_label_audio_name_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_screen_audio_player_label_audio_name_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_audio_player_label_audio_name_main_main_default, 0);
	lv_style_set_text_align(&style_screen_audio_player_label_audio_name_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_audio_player_label_audio_name_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_audio_player_label_audio_name_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_audio_player_label_audio_name_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_audio_player_label_audio_name_main_main_default, 0);
	lv_obj_add_style(ui->screen_audio_player_label_audio_name, &style_screen_audio_player_label_audio_name_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_audio_player_hand_holder
    ui->screen_audio_player_hand_holder = lv_img_create(ui->screen_audio_player_cont_background);
    lv_obj_set_pos(ui->screen_audio_player_hand_holder, 1091, 309);
    lv_obj_set_size(ui->screen_audio_player_hand_holder, 180, 200);
    lv_obj_set_scrollbar_mode(ui->screen_audio_player_hand_holder, LV_SCROLLBAR_MODE_OFF);

    //Write style state: LV_STATE_DEFAULT for style_screen_audio_player_hand_holder_main_main_default
    static lv_style_t style_screen_audio_player_hand_holder_main_main_default;
    if (style_screen_audio_player_hand_holder_main_main_default.prop_cnt > 1)
        lv_style_reset(&style_screen_audio_player_hand_holder_main_main_default);
    else
        lv_style_init(&style_screen_audio_player_hand_holder_main_main_default);
    lv_style_set_img_recolor(&style_screen_audio_player_hand_holder_main_main_default, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_img_recolor_opa(&style_screen_audio_player_hand_holder_main_main_default, 0);
    lv_style_set_img_opa(&style_screen_audio_player_hand_holder_main_main_default, 255);
    lv_obj_add_style(ui->screen_audio_player_hand_holder, &style_screen_audio_player_hand_holder_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_flag(ui->screen_audio_player_hand_holder, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_pivot(ui->screen_audio_player_hand_holder, 50,50);
    lv_img_set_angle(ui->screen_audio_player_hand_holder, 0);

	//Write codes screen_audio_player_imgbtn_home
	ui->screen_audio_player_imgbtn_home = lv_imgbtn_create(ui->screen_audio_player);
	lv_obj_set_pos(ui->screen_audio_player_imgbtn_home, 24, 26);
	lv_obj_set_size(ui->screen_audio_player_imgbtn_home, 85, 70);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_imgbtn_home, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_audio_player_imgbtn_home, LV_IMGBTN_STATE_RELEASED, NULL, &_home_85x70, NULL);
	lv_obj_add_flag(ui->screen_audio_player_imgbtn_home, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_audio_player_label_title
	ui->screen_audio_player_label_title = lv_label_create(ui->screen_audio_player);
	lv_obj_set_pos(ui->screen_audio_player_label_title, 230, 30);
	lv_obj_set_size(ui->screen_audio_player_label_title, 760, 60);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_label_title, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_audio_player_label_title, "ELESIUM - BEST MOMENTS");
	lv_label_set_long_mode(ui->screen_audio_player_label_title, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_label_title_main_main_default
	static lv_style_t style_screen_audio_player_label_title_main_main_default;
	if (style_screen_audio_player_label_title_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_label_title_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_label_title_main_main_default);
	lv_style_set_radius(&style_screen_audio_player_label_title_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_audio_player_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_audio_player_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_audio_player_label_title_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_audio_player_label_title_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_audio_player_label_title_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_label_title_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_label_title_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_label_title_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_label_title_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_label_title_main_main_default, 0);
	lv_style_set_text_color(&style_screen_audio_player_label_title_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_audio_player_label_title_main_main_default, &lv_font_montserratMedium_50);
	lv_style_set_text_letter_space(&style_screen_audio_player_label_title_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_audio_player_label_title_main_main_default, 0);
	lv_style_set_text_align(&style_screen_audio_player_label_title_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_audio_player_label_title_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_audio_player_label_title_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_audio_player_label_title_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_audio_player_label_title_main_main_default, 0);
	lv_obj_add_style(ui->screen_audio_player_label_title, &style_screen_audio_player_label_title_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_audio_player_imgbtn_previous
	ui->screen_audio_player_imgbtn_previous = lv_imgbtn_create(ui->screen_audio_player);
	lv_obj_set_pos(ui->screen_audio_player_imgbtn_previous, 246, 304);
	lv_obj_set_size(ui->screen_audio_player_imgbtn_previous, 140, 214);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_imgbtn_previous, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_imgbtn_previous_main_main_default
	static lv_style_t style_screen_audio_player_imgbtn_previous_main_main_default;
	if (style_screen_audio_player_imgbtn_previous_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_previous_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_previous_main_main_default);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_previous_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_previous_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_previous_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_previous_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_previous_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_previous_main_main_default, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_previous_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_align(&style_screen_audio_player_imgbtn_previous_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_previous_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_previous_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_previous_main_main_default, 255);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_previous, &style_screen_audio_player_imgbtn_previous_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_screen_audio_player_imgbtn_previous_main_main_pressed
	static lv_style_t style_screen_audio_player_imgbtn_previous_main_main_pressed;
	if (style_screen_audio_player_imgbtn_previous_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_previous_main_main_pressed);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_previous_main_main_pressed);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_previous_main_main_pressed, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_previous_main_main_pressed, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_previous_main_main_pressed, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_previous_main_main_pressed, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_previous_main_main_pressed, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_previous_main_main_pressed, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_previous_main_main_pressed, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_previous_main_main_pressed, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_previous_main_main_pressed, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_previous_main_main_pressed, 130);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_previous, &style_screen_audio_player_imgbtn_previous_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_screen_audio_player_imgbtn_previous_main_main_checked
	static lv_style_t style_screen_audio_player_imgbtn_previous_main_main_checked;
	if (style_screen_audio_player_imgbtn_previous_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_previous_main_main_checked);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_previous_main_main_checked);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_previous_main_main_checked, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_previous_main_main_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_previous_main_main_checked, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_previous_main_main_checked, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_previous_main_main_checked, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_previous_main_main_checked, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_previous_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_previous_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_previous_main_main_checked, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_previous_main_main_checked, 255);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_previous, &style_screen_audio_player_imgbtn_previous_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->screen_audio_player_imgbtn_previous, LV_IMGBTN_STATE_RELEASED, NULL, &_audio_player_previous_140x214, NULL);
	lv_obj_add_flag(ui->screen_audio_player_imgbtn_previous, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_audio_player_imgbtn_next
	ui->screen_audio_player_imgbtn_next = lv_imgbtn_create(ui->screen_audio_player);
	lv_obj_set_pos(ui->screen_audio_player_imgbtn_next, 906, 304);
	lv_obj_set_size(ui->screen_audio_player_imgbtn_next, 140, 214);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_imgbtn_next, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_imgbtn_next_main_main_default
	static lv_style_t style_screen_audio_player_imgbtn_next_main_main_default;
	if (style_screen_audio_player_imgbtn_next_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_next_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_next_main_main_default);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_next_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_next_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_next_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_next_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_next_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_next_main_main_default, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_next_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_align(&style_screen_audio_player_imgbtn_next_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_next_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_next_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_next_main_main_default, 255);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_next, &style_screen_audio_player_imgbtn_next_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_screen_audio_player_imgbtn_next_main_main_pressed
	static lv_style_t style_screen_audio_player_imgbtn_next_main_main_pressed;
	if (style_screen_audio_player_imgbtn_next_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_next_main_main_pressed);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_next_main_main_pressed);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_next_main_main_pressed, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_next_main_main_pressed, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_next_main_main_pressed, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_next_main_main_pressed, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_next_main_main_pressed, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_next_main_main_pressed, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_next_main_main_pressed, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_next_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_next_main_main_pressed, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_next_main_main_pressed, 130);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_next, &style_screen_audio_player_imgbtn_next_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_screen_audio_player_imgbtn_next_main_main_checked
	static lv_style_t style_screen_audio_player_imgbtn_next_main_main_checked;
	if (style_screen_audio_player_imgbtn_next_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_imgbtn_next_main_main_checked);
	else
		lv_style_init(&style_screen_audio_player_imgbtn_next_main_main_checked);
	lv_style_set_shadow_width(&style_screen_audio_player_imgbtn_next_main_main_checked, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_imgbtn_next_main_main_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_imgbtn_next_main_main_checked, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_imgbtn_next_main_main_checked, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_imgbtn_next_main_main_checked, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_imgbtn_next_main_main_checked, 0);
	lv_style_set_text_color(&style_screen_audio_player_imgbtn_next_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_screen_audio_player_imgbtn_next_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_screen_audio_player_imgbtn_next_main_main_checked, 0);
	lv_style_set_img_opa(&style_screen_audio_player_imgbtn_next_main_main_checked, 255);
	lv_obj_add_style(ui->screen_audio_player_imgbtn_next, &style_screen_audio_player_imgbtn_next_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->screen_audio_player_imgbtn_next, LV_IMGBTN_STATE_RELEASED, NULL, &_audio_player_next_140x214, NULL);
	lv_obj_add_flag(ui->screen_audio_player_imgbtn_next, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_audio_player_label_track_time
	ui->screen_audio_player_label_track_time = lv_label_create(ui->screen_audio_player);
	lv_obj_set_pos(ui->screen_audio_player_label_track_time, 525, 650);
	lv_obj_set_size(ui->screen_audio_player_label_track_time, 110, 30);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_label_track_time, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_audio_player_label_track_time, "01:35");
	lv_label_set_long_mode(ui->screen_audio_player_label_track_time, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_label_track_time_main_main_default
	static lv_style_t style_screen_audio_player_label_track_time_main_main_default;
	if (style_screen_audio_player_label_track_time_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_label_track_time_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_label_track_time_main_main_default);
	lv_style_set_radius(&style_screen_audio_player_label_track_time_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_audio_player_label_track_time_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_audio_player_label_track_time_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_audio_player_label_track_time_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_audio_player_label_track_time_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_audio_player_label_track_time_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_label_track_time_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_label_track_time_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_label_track_time_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_label_track_time_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_label_track_time_main_main_default, 0);
	lv_style_set_text_color(&style_screen_audio_player_label_track_time_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_audio_player_label_track_time_main_main_default, &lv_font_montserratMedium_30);
	lv_style_set_text_letter_space(&style_screen_audio_player_label_track_time_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_audio_player_label_track_time_main_main_default, 0);
	lv_style_set_text_align(&style_screen_audio_player_label_track_time_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_audio_player_label_track_time_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_audio_player_label_track_time_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_audio_player_label_track_time_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_audio_player_label_track_time_main_main_default, 0);
	lv_obj_add_style(ui->screen_audio_player_label_track_time, &style_screen_audio_player_label_track_time_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_audio_player_label_album_time
	ui->screen_audio_player_label_album_time = lv_label_create(ui->screen_audio_player);
	lv_obj_set_pos(ui->screen_audio_player_label_album_time, 645, 650);
	lv_obj_set_size(ui->screen_audio_player_label_album_time, 110, 30);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_label_album_time, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_audio_player_label_album_time, "04:12");
	lv_label_set_long_mode(ui->screen_audio_player_label_album_time, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_label_album_time_main_main_default
	static lv_style_t style_screen_audio_player_label_album_time_main_main_default;
	if (style_screen_audio_player_label_album_time_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_label_album_time_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_label_album_time_main_main_default);
	lv_style_set_radius(&style_screen_audio_player_label_album_time_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_audio_player_label_album_time_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_audio_player_label_album_time_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_audio_player_label_album_time_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_audio_player_label_album_time_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_audio_player_label_album_time_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_label_album_time_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_label_album_time_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_label_album_time_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_label_album_time_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_label_album_time_main_main_default, 0);
	lv_style_set_text_color(&style_screen_audio_player_label_album_time_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_audio_player_label_album_time_main_main_default, &lv_font_montserratMedium_30);
	lv_style_set_text_letter_space(&style_screen_audio_player_label_album_time_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_audio_player_label_album_time_main_main_default, 0);
	lv_style_set_text_align(&style_screen_audio_player_label_album_time_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_audio_player_label_album_time_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_audio_player_label_album_time_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_audio_player_label_album_time_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_audio_player_label_album_time_main_main_default, 0);
	lv_obj_add_style(ui->screen_audio_player_label_album_time, &style_screen_audio_player_label_album_time_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_audio_player_label_to
	ui->screen_audio_player_label_to = lv_label_create(ui->screen_audio_player);
	lv_obj_set_pos(ui->screen_audio_player_label_to, 625, 653);
	lv_obj_set_size(ui->screen_audio_player_label_to, 30, 25);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_label_to, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_audio_player_label_to, "-");
	lv_label_set_long_mode(ui->screen_audio_player_label_to, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_audio_player_label_to_main_main_default
	static lv_style_t style_screen_audio_player_label_to_main_main_default;
	if (style_screen_audio_player_label_to_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_audio_player_label_to_main_main_default);
	else
		lv_style_init(&style_screen_audio_player_label_to_main_main_default);
	lv_style_set_radius(&style_screen_audio_player_label_to_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_audio_player_label_to_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_audio_player_label_to_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_audio_player_label_to_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_audio_player_label_to_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_audio_player_label_to_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_audio_player_label_to_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_audio_player_label_to_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_audio_player_label_to_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_audio_player_label_to_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_audio_player_label_to_main_main_default, 0);
	lv_style_set_text_color(&style_screen_audio_player_label_to_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_audio_player_label_to_main_main_default, &lv_font_montserratMedium_25);
	lv_style_set_text_letter_space(&style_screen_audio_player_label_to_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_audio_player_label_to_main_main_default, 0);
	lv_style_set_text_align(&style_screen_audio_player_label_to_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_audio_player_label_to_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_audio_player_label_to_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_audio_player_label_to_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_audio_player_label_to_main_main_default, 0);
	lv_obj_add_style(ui->screen_audio_player_label_to, &style_screen_audio_player_label_to_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_audio_player_imgbtn_help
	ui->screen_audio_player_imgbtn_help = lv_imgbtn_create(ui->screen_audio_player);
	lv_obj_set_pos(ui->screen_audio_player_imgbtn_help, 1158, 113);
	lv_obj_set_size(ui->screen_audio_player_imgbtn_help, 100, 100);
	lv_obj_set_scrollbar_mode(ui->screen_audio_player_imgbtn_help, LV_SCROLLBAR_MODE_OFF);
	lv_imgbtn_set_src(ui->screen_audio_player_imgbtn_help, LV_IMGBTN_STATE_RELEASED, NULL, &_help_100x100, NULL);
	lv_obj_add_flag(ui->screen_audio_player_imgbtn_help, LV_OBJ_FLAG_CHECKABLE);

	//Init events for screen
	events_init_screen_audio_player(ui);
}
