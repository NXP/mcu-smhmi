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

static const lv_img_dsc_t* main_animimg_timeout_imgs[1] = {
	&main_animimg_timeoutmain_rb_10_10
};

void setup_scr_main(lv_ui *ui){

	//Write codes main
	ui->main = lv_obj_create(NULL);

	//Write style state: LV_STATE_DEFAULT for style_main_main_main_default
	static lv_style_t style_main_main_main_default;
	if (style_main_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_main_main_main_default);
	else
		lv_style_init(&style_main_main_main_default);
	lv_style_set_bg_color(&style_main_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_main_main_main_default, 0);
	lv_obj_add_style(ui->main, &style_main_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes main_imgbtn_home
	ui->main_imgbtn_home = lv_imgbtn_create(ui->main);
	lv_obj_set_pos(ui->main_imgbtn_home, 0, 0);
	lv_obj_set_size(ui->main_imgbtn_home, 1280, 720);

	//Write style state: LV_STATE_DEFAULT for style_main_imgbtn_home_main_main_default
	static lv_style_t style_main_imgbtn_home_main_main_default;
	if (style_main_imgbtn_home_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_main_imgbtn_home_main_main_default);
	else
		lv_style_init(&style_main_imgbtn_home_main_main_default);
	lv_style_set_text_color(&style_main_imgbtn_home_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor(&style_main_imgbtn_home_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_main_imgbtn_home_main_main_default, 0);
	lv_style_set_img_opa(&style_main_imgbtn_home_main_main_default, 255);
	lv_obj_add_style(ui->main_imgbtn_home, &style_main_imgbtn_home_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_PRESSED for style_main_imgbtn_home_main_main_pressed
	static lv_style_t style_main_imgbtn_home_main_main_pressed;
	if (style_main_imgbtn_home_main_main_pressed.prop_cnt > 1)
		lv_style_reset(&style_main_imgbtn_home_main_main_pressed);
	else
		lv_style_init(&style_main_imgbtn_home_main_main_pressed);
	lv_style_set_text_color(&style_main_imgbtn_home_main_main_pressed, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_main_imgbtn_home_main_main_pressed, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_main_imgbtn_home_main_main_pressed, 0);
	lv_obj_add_style(ui->main_imgbtn_home, &style_main_imgbtn_home_main_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style state: LV_STATE_CHECKED for style_main_imgbtn_home_main_main_checked
	static lv_style_t style_main_imgbtn_home_main_main_checked;
	if (style_main_imgbtn_home_main_main_checked.prop_cnt > 1)
		lv_style_reset(&style_main_imgbtn_home_main_main_checked);
	else
		lv_style_init(&style_main_imgbtn_home_main_main_checked);
	lv_style_set_text_color(&style_main_imgbtn_home_main_main_checked, lv_color_make(0xFF, 0x33, 0xFF));
	lv_style_set_img_recolor(&style_main_imgbtn_home_main_main_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_img_recolor_opa(&style_main_imgbtn_home_main_main_checked, 0);
	lv_obj_add_style(ui->main_imgbtn_home, &style_main_imgbtn_home_main_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->main_imgbtn_home, LV_IMGBTN_STATE_RELEASED, NULL, &_main_coffee_machine_1280x720, NULL);
	lv_obj_add_flag(ui->main_imgbtn_home, LV_OBJ_FLAG_CHECKABLE);

	//Write codes main_animimg_timeout
	ui->main_animimg_timeout = lv_animimg_create(ui->main);
	lv_obj_set_pos(ui->main_animimg_timeout, 1270, 710);
	lv_obj_set_size(ui->main_animimg_timeout, 10, 10);
	lv_animimg_set_src(ui->main_animimg_timeout, (lv_img_dsc_t**) main_animimg_timeout_imgs, 1);
	lv_animimg_set_duration(ui->main_animimg_timeout, 1000);
	lv_animimg_t* main_animimg_timeout_ext = (lv_animimg_t *)ui->main_animimg_timeout;
	lv_anim_set_ready_cb(&main_animimg_timeout_ext->anim, gui_main_anim_ready_cb);
	lv_animimg_set_repeat_count(ui->main_animimg_timeout, 10);
	lv_animimg_start(ui->main_animimg_timeout);

	//Init events for screen
	events_init_main(ui);
}