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

static lv_obj_t * g_kb;
static void kb_event_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *kb = lv_event_get_target(e);
	if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL){
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}
static void ta_event_cb(lv_event_t *e)
{

	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *ta = lv_event_get_target(e);
	lv_obj_t *kb = lv_event_get_user_data(e);
	if (code == LV_EVENT_FOCUSED)
	{
		lv_keyboard_set_textarea(kb, ta);
		lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
	if (code == LV_EVENT_DEFOCUSED)
	{
		lv_keyboard_set_textarea(kb, NULL);
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}

void setup_scr_screen_font_holder(lv_ui *ui){

	//Write codes screen_font_holder
	ui->screen_font_holder = lv_obj_create(NULL);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_main_main_default
	static lv_style_t style_screen_font_holder_main_main_default;
	if (style_screen_font_holder_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_main_main_default);
	else
		lv_style_init(&style_screen_font_holder_main_main_default);
	lv_style_set_bg_color(&style_screen_font_holder_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_opa(&style_screen_font_holder_main_main_default, 255);
	lv_obj_add_style(ui->screen_font_holder, &style_screen_font_holder_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_font_holder_ta_simsun_32
	ui->screen_font_holder_ta_simsun_32 = lv_textarea_create(ui->screen_font_holder);
	lv_obj_set_pos(ui->screen_font_holder_ta_simsun_32, 10, 44);
	lv_obj_set_size(ui->screen_font_holder_ta_simsun_32, 1070, 55);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_simsun_32_main_main_default
	static lv_style_t style_screen_font_holder_ta_simsun_32_main_main_default;
	if (style_screen_font_holder_ta_simsun_32_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_simsun_32_main_main_default);
	else
		lv_style_init(&style_screen_font_holder_ta_simsun_32_main_main_default);
	lv_style_set_radius(&style_screen_font_holder_ta_simsun_32_main_main_default, 4);
	lv_style_set_bg_color(&style_screen_font_holder_ta_simsun_32_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_simsun_32_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_simsun_32_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_simsun_32_main_main_default, 255);
	lv_style_set_border_color(&style_screen_font_holder_ta_simsun_32_main_main_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_screen_font_holder_ta_simsun_32_main_main_default, 2);
	lv_style_set_text_color(&style_screen_font_holder_ta_simsun_32_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_font_holder_ta_simsun_32_main_main_default, &lv_font_simsun_32);
	lv_style_set_text_letter_space(&style_screen_font_holder_ta_simsun_32_main_main_default, 2);
	lv_style_set_text_align(&style_screen_font_holder_ta_simsun_32_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_font_holder_ta_simsun_32_main_main_default, 2);
	lv_style_set_pad_right(&style_screen_font_holder_ta_simsun_32_main_main_default, 2);
	lv_style_set_pad_top(&style_screen_font_holder_ta_simsun_32_main_main_default, 2);
	lv_style_set_pad_bottom(&style_screen_font_holder_ta_simsun_32_main_main_default, 2);
	lv_obj_add_style(ui->screen_font_holder_ta_simsun_32, &style_screen_font_holder_ta_simsun_32_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_simsun_32_main_scrollbar_default
	static lv_style_t style_screen_font_holder_ta_simsun_32_main_scrollbar_default;
	if (style_screen_font_holder_ta_simsun_32_main_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_simsun_32_main_scrollbar_default);
	else
		lv_style_init(&style_screen_font_holder_ta_simsun_32_main_scrollbar_default);
	lv_style_set_radius(&style_screen_font_holder_ta_simsun_32_main_scrollbar_default, 0);
	lv_style_set_bg_color(&style_screen_font_holder_ta_simsun_32_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_simsun_32_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_simsun_32_main_scrollbar_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_simsun_32_main_scrollbar_default, 255);
	lv_obj_add_style(ui->screen_font_holder_ta_simsun_32, &style_screen_font_holder_ta_simsun_32_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->screen_font_holder_ta_simsun_32,"确定取消帮助");
	if (g_kb == NULL)
	{
		g_kb = lv_keyboard_create(ui->screen_font_holder);
		lv_obj_add_event_cb(g_kb, kb_event_cb, LV_EVENT_ALL, NULL);
		lv_obj_add_flag(g_kb, LV_OBJ_FLAG_HIDDEN);
	}
	lv_keyboard_set_textarea(g_kb, ui->screen_font_holder_ta_simsun_32);
	lv_obj_add_event_cb(ui->screen_font_holder_ta_simsun_32, ta_event_cb, LV_EVENT_ALL, g_kb);

	//Write codes screen_font_holder_ta_montserratmedium_32
	ui->screen_font_holder_ta_montserratmedium_32 = lv_textarea_create(ui->screen_font_holder);
	lv_obj_set_pos(ui->screen_font_holder_ta_montserratmedium_32, 12, 112);
	lv_obj_set_size(ui->screen_font_holder_ta_montserratmedium_32, 1070, 56);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_montserratmedium_32_main_main_default
	static lv_style_t style_screen_font_holder_ta_montserratmedium_32_main_main_default;
	if (style_screen_font_holder_ta_montserratmedium_32_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_montserratmedium_32_main_main_default);
	else
		lv_style_init(&style_screen_font_holder_ta_montserratmedium_32_main_main_default);
	lv_style_set_radius(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, 4);
	lv_style_set_bg_color(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, 255);
	lv_style_set_border_color(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, 2);
	lv_style_set_text_color(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, &lv_font_montserratMedium_32);
	lv_style_set_text_letter_space(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, 2);
	lv_style_set_text_align(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, 2);
	lv_style_set_pad_right(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, 2);
	lv_style_set_pad_top(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, 2);
	lv_style_set_pad_bottom(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, 2);
	lv_obj_add_style(ui->screen_font_holder_ta_montserratmedium_32, &style_screen_font_holder_ta_montserratmedium_32_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_montserratmedium_32_main_scrollbar_default
	static lv_style_t style_screen_font_holder_ta_montserratmedium_32_main_scrollbar_default;
	if (style_screen_font_holder_ta_montserratmedium_32_main_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_montserratmedium_32_main_scrollbar_default);
	else
		lv_style_init(&style_screen_font_holder_ta_montserratmedium_32_main_scrollbar_default);
	lv_style_set_radius(&style_screen_font_holder_ta_montserratmedium_32_main_scrollbar_default, 0);
	lv_style_set_bg_color(&style_screen_font_holder_ta_montserratmedium_32_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_montserratmedium_32_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_montserratmedium_32_main_scrollbar_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_montserratmedium_32_main_scrollbar_default, 255);
	lv_obj_add_style(ui->screen_font_holder_ta_montserratmedium_32, &style_screen_font_holder_ta_montserratmedium_32_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->screen_font_holder_ta_montserratmedium_32,"Bestätigen");
	if (g_kb == NULL)
	{
		g_kb = lv_keyboard_create(ui->screen_font_holder);
		lv_obj_add_event_cb(g_kb, kb_event_cb, LV_EVENT_ALL, NULL);
		lv_obj_add_flag(g_kb, LV_OBJ_FLAG_HIDDEN);
	}
	lv_keyboard_set_textarea(g_kb, ui->screen_font_holder_ta_montserratmedium_32);
	lv_obj_add_event_cb(ui->screen_font_holder_ta_montserratmedium_32, ta_event_cb, LV_EVENT_ALL, g_kb);

	//Write codes screen_font_holder_ta_simsun_20
	ui->screen_font_holder_ta_simsun_20 = lv_textarea_create(ui->screen_font_holder);
	lv_obj_set_pos(ui->screen_font_holder_ta_simsun_20, 12, 310);
	lv_obj_set_size(ui->screen_font_holder_ta_simsun_20, 346, 44);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_simsun_20_main_main_default
	static lv_style_t style_screen_font_holder_ta_simsun_20_main_main_default;
	if (style_screen_font_holder_ta_simsun_20_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_simsun_20_main_main_default);
	else
		lv_style_init(&style_screen_font_holder_ta_simsun_20_main_main_default);
	lv_style_set_radius(&style_screen_font_holder_ta_simsun_20_main_main_default, 4);
	lv_style_set_bg_color(&style_screen_font_holder_ta_simsun_20_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_simsun_20_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_simsun_20_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_simsun_20_main_main_default, 255);
	lv_style_set_border_color(&style_screen_font_holder_ta_simsun_20_main_main_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_screen_font_holder_ta_simsun_20_main_main_default, 2);
	lv_style_set_text_color(&style_screen_font_holder_ta_simsun_20_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_font_holder_ta_simsun_20_main_main_default, &lv_font_simsun_20);
	lv_style_set_text_letter_space(&style_screen_font_holder_ta_simsun_20_main_main_default, 2);
	lv_style_set_text_align(&style_screen_font_holder_ta_simsun_20_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_font_holder_ta_simsun_20_main_main_default, 2);
	lv_style_set_pad_right(&style_screen_font_holder_ta_simsun_20_main_main_default, 2);
	lv_style_set_pad_top(&style_screen_font_holder_ta_simsun_20_main_main_default, 2);
	lv_style_set_pad_bottom(&style_screen_font_holder_ta_simsun_20_main_main_default, 2);
	lv_obj_add_style(ui->screen_font_holder_ta_simsun_20, &style_screen_font_holder_ta_simsun_20_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_simsun_20_main_scrollbar_default
	static lv_style_t style_screen_font_holder_ta_simsun_20_main_scrollbar_default;
	if (style_screen_font_holder_ta_simsun_20_main_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_simsun_20_main_scrollbar_default);
	else
		lv_style_init(&style_screen_font_holder_ta_simsun_20_main_scrollbar_default);
	lv_style_set_radius(&style_screen_font_holder_ta_simsun_20_main_scrollbar_default, 0);
	lv_style_set_bg_color(&style_screen_font_holder_ta_simsun_20_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_simsun_20_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_simsun_20_main_scrollbar_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_simsun_20_main_scrollbar_default, 255);
	lv_obj_add_style(ui->screen_font_holder_ta_simsun_20, &style_screen_font_holder_ta_simsun_20_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->screen_font_holder_ta_simsun_20,"没有检测到用户新的已注册");
	if (g_kb == NULL)
	{
		g_kb = lv_keyboard_create(ui->screen_font_holder);
		lv_obj_add_event_cb(g_kb, kb_event_cb, LV_EVENT_ALL, NULL);
		lv_obj_add_flag(g_kb, LV_OBJ_FLAG_HIDDEN);
	}
	lv_keyboard_set_textarea(g_kb, ui->screen_font_holder_ta_simsun_20);
	lv_obj_add_event_cb(ui->screen_font_holder_ta_simsun_20, ta_event_cb, LV_EVENT_ALL, g_kb);

	//Write codes screen_font_holder_ta_montserratmedium_20
	ui->screen_font_holder_ta_montserratmedium_20 = lv_textarea_create(ui->screen_font_holder);
	lv_obj_set_pos(ui->screen_font_holder_ta_montserratmedium_20, 12, 183);
	lv_obj_set_size(ui->screen_font_holder_ta_montserratmedium_20, 1070, 50);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_montserratmedium_20_main_main_default
	static lv_style_t style_screen_font_holder_ta_montserratmedium_20_main_main_default;
	if (style_screen_font_holder_ta_montserratmedium_20_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_montserratmedium_20_main_main_default);
	else
		lv_style_init(&style_screen_font_holder_ta_montserratmedium_20_main_main_default);
	lv_style_set_radius(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, 4);
	lv_style_set_bg_color(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, 255);
	lv_style_set_border_color(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, 2);
	lv_style_set_text_color(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, &lv_font_montserratMedium_20);
	lv_style_set_text_letter_space(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, 2);
	lv_style_set_text_align(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, 2);
	lv_style_set_pad_right(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, 2);
	lv_style_set_pad_top(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, 2);
	lv_style_set_pad_bottom(&style_screen_font_holder_ta_montserratmedium_20_main_main_default, 2);
	lv_obj_add_style(ui->screen_font_holder_ta_montserratmedium_20, &style_screen_font_holder_ta_montserratmedium_20_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_montserratmedium_20_main_scrollbar_default
	static lv_style_t style_screen_font_holder_ta_montserratmedium_20_main_scrollbar_default;
	if (style_screen_font_holder_ta_montserratmedium_20_main_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_montserratmedium_20_main_scrollbar_default);
	else
		lv_style_init(&style_screen_font_holder_ta_montserratmedium_20_main_scrollbar_default);
	lv_style_set_radius(&style_screen_font_holder_ta_montserratmedium_20_main_scrollbar_default, 0);
	lv_style_set_bg_color(&style_screen_font_holder_ta_montserratmedium_20_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_montserratmedium_20_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_montserratmedium_20_main_scrollbar_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_montserratmedium_20_main_scrollbar_default, 255);
	lv_obj_add_style(ui->screen_font_holder_ta_montserratmedium_20, &style_screen_font_holder_ta_montserratmedium_20_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->screen_font_holder_ta_montserratmedium_20,"détecté");
	if (g_kb == NULL)
	{
		g_kb = lv_keyboard_create(ui->screen_font_holder);
		lv_obj_add_event_cb(g_kb, kb_event_cb, LV_EVENT_ALL, NULL);
		lv_obj_add_flag(g_kb, LV_OBJ_FLAG_HIDDEN);
	}
	lv_keyboard_set_textarea(g_kb, ui->screen_font_holder_ta_montserratmedium_20);
	lv_obj_add_event_cb(ui->screen_font_holder_ta_montserratmedium_20, ta_event_cb, LV_EVENT_ALL, g_kb);

	//Write codes screen_font_holder_ta_simsun_22
	ui->screen_font_holder_ta_simsun_22 = lv_textarea_create(ui->screen_font_holder);
	lv_obj_set_pos(ui->screen_font_holder_ta_simsun_22, 11, 248);
	lv_obj_set_size(ui->screen_font_holder_ta_simsun_22, 214, 51);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_simsun_22_main_main_default
	static lv_style_t style_screen_font_holder_ta_simsun_22_main_main_default;
	if (style_screen_font_holder_ta_simsun_22_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_simsun_22_main_main_default);
	else
		lv_style_init(&style_screen_font_holder_ta_simsun_22_main_main_default);
	lv_style_set_radius(&style_screen_font_holder_ta_simsun_22_main_main_default, 4);
	lv_style_set_bg_color(&style_screen_font_holder_ta_simsun_22_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_simsun_22_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_simsun_22_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_simsun_22_main_main_default, 255);
	lv_style_set_border_color(&style_screen_font_holder_ta_simsun_22_main_main_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_screen_font_holder_ta_simsun_22_main_main_default, 2);
	lv_style_set_text_color(&style_screen_font_holder_ta_simsun_22_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_font_holder_ta_simsun_22_main_main_default, &lv_font_simsun_22);
	lv_style_set_text_letter_space(&style_screen_font_holder_ta_simsun_22_main_main_default, 2);
	lv_style_set_text_align(&style_screen_font_holder_ta_simsun_22_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_font_holder_ta_simsun_22_main_main_default, 2);
	lv_style_set_pad_right(&style_screen_font_holder_ta_simsun_22_main_main_default, 2);
	lv_style_set_pad_top(&style_screen_font_holder_ta_simsun_22_main_main_default, 2);
	lv_style_set_pad_bottom(&style_screen_font_holder_ta_simsun_22_main_main_default, 2);
	lv_obj_add_style(ui->screen_font_holder_ta_simsun_22, &style_screen_font_holder_ta_simsun_22_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_simsun_22_main_scrollbar_default
	static lv_style_t style_screen_font_holder_ta_simsun_22_main_scrollbar_default;
	if (style_screen_font_holder_ta_simsun_22_main_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_simsun_22_main_scrollbar_default);
	else
		lv_style_init(&style_screen_font_holder_ta_simsun_22_main_scrollbar_default);
	lv_style_set_radius(&style_screen_font_holder_ta_simsun_22_main_scrollbar_default, 0);
	lv_style_set_bg_color(&style_screen_font_holder_ta_simsun_22_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_simsun_22_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_simsun_22_main_scrollbar_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_simsun_22_main_scrollbar_default, 255);
	lv_obj_add_style(ui->screen_font_holder_ta_simsun_22, &style_screen_font_holder_ta_simsun_22_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->screen_font_holder_ta_simsun_22,"你已经到达");
	if (g_kb == NULL)
	{
		g_kb = lv_keyboard_create(ui->screen_font_holder);
		lv_obj_add_event_cb(g_kb, kb_event_cb, LV_EVENT_ALL, NULL);
		lv_obj_add_flag(g_kb, LV_OBJ_FLAG_HIDDEN);
	}
	lv_keyboard_set_textarea(g_kb, ui->screen_font_holder_ta_simsun_22);
	lv_obj_add_event_cb(ui->screen_font_holder_ta_simsun_22, ta_event_cb, LV_EVENT_ALL, g_kb);

	//Write codes screen_font_holder_ta_montserratmedium_22
	ui->screen_font_holder_ta_montserratmedium_22 = lv_textarea_create(ui->screen_font_holder);
	lv_obj_set_pos(ui->screen_font_holder_ta_montserratmedium_22, 243, 253);
	lv_obj_set_size(ui->screen_font_holder_ta_montserratmedium_22, 827, 47);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_montserratmedium_22_main_main_default
	static lv_style_t style_screen_font_holder_ta_montserratmedium_22_main_main_default;
	if (style_screen_font_holder_ta_montserratmedium_22_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_montserratmedium_22_main_main_default);
	else
		lv_style_init(&style_screen_font_holder_ta_montserratmedium_22_main_main_default);
	lv_style_set_radius(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, 4);
	lv_style_set_bg_color(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, 255);
	lv_style_set_border_color(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, 2);
	lv_style_set_text_color(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, &lv_font_montserratMedium_22);
	lv_style_set_text_letter_space(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, 2);
	lv_style_set_text_align(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, 2);
	lv_style_set_pad_right(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, 2);
	lv_style_set_pad_top(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, 2);
	lv_style_set_pad_bottom(&style_screen_font_holder_ta_montserratmedium_22_main_main_default, 2);
	lv_obj_add_style(ui->screen_font_holder_ta_montserratmedium_22, &style_screen_font_holder_ta_montserratmedium_22_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_montserratmedium_22_main_scrollbar_default
	static lv_style_t style_screen_font_holder_ta_montserratmedium_22_main_scrollbar_default;
	if (style_screen_font_holder_ta_montserratmedium_22_main_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_montserratmedium_22_main_scrollbar_default);
	else
		lv_style_init(&style_screen_font_holder_ta_montserratmedium_22_main_scrollbar_default);
	lv_style_set_radius(&style_screen_font_holder_ta_montserratmedium_22_main_scrollbar_default, 0);
	lv_style_set_bg_color(&style_screen_font_holder_ta_montserratmedium_22_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_montserratmedium_22_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_montserratmedium_22_main_scrollbar_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_montserratmedium_22_main_scrollbar_default, 255);
	lv_obj_add_style(ui->screen_font_holder_ta_montserratmedium_22, &style_screen_font_holder_ta_montserratmedium_22_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->screen_font_holder_ta_montserratmedium_22,"VOUS ÊTES ARRIVÊS");
	if (g_kb == NULL)
	{
		g_kb = lv_keyboard_create(ui->screen_font_holder);
		lv_obj_add_event_cb(g_kb, kb_event_cb, LV_EVENT_ALL, NULL);
		lv_obj_add_flag(g_kb, LV_OBJ_FLAG_HIDDEN);
	}
	lv_keyboard_set_textarea(g_kb, ui->screen_font_holder_ta_montserratmedium_22);
	lv_obj_add_event_cb(ui->screen_font_holder_ta_montserratmedium_22, ta_event_cb, LV_EVENT_ALL, g_kb);

	//Write codes screen_font_holder_ta_simsun_24
	ui->screen_font_holder_ta_simsun_24 = lv_textarea_create(ui->screen_font_holder);
	lv_obj_set_pos(ui->screen_font_holder_ta_simsun_24, 403, 366);
	lv_obj_set_size(ui->screen_font_holder_ta_simsun_24, 656, 47);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_simsun_24_main_main_default
	static lv_style_t style_screen_font_holder_ta_simsun_24_main_main_default;
	if (style_screen_font_holder_ta_simsun_24_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_simsun_24_main_main_default);
	else
		lv_style_init(&style_screen_font_holder_ta_simsun_24_main_main_default);
	lv_style_set_radius(&style_screen_font_holder_ta_simsun_24_main_main_default, 4);
	lv_style_set_bg_color(&style_screen_font_holder_ta_simsun_24_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_simsun_24_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_simsun_24_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_simsun_24_main_main_default, 255);
	lv_style_set_border_color(&style_screen_font_holder_ta_simsun_24_main_main_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_screen_font_holder_ta_simsun_24_main_main_default, 2);
	lv_style_set_text_color(&style_screen_font_holder_ta_simsun_24_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_font_holder_ta_simsun_24_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_font_holder_ta_simsun_24_main_main_default, 2);
	lv_style_set_text_align(&style_screen_font_holder_ta_simsun_24_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_font_holder_ta_simsun_24_main_main_default, 2);
	lv_style_set_pad_right(&style_screen_font_holder_ta_simsun_24_main_main_default, 2);
	lv_style_set_pad_top(&style_screen_font_holder_ta_simsun_24_main_main_default, 2);
	lv_style_set_pad_bottom(&style_screen_font_holder_ta_simsun_24_main_main_default, 2);
	lv_obj_add_style(ui->screen_font_holder_ta_simsun_24, &style_screen_font_holder_ta_simsun_24_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_simsun_24_main_scrollbar_default
	static lv_style_t style_screen_font_holder_ta_simsun_24_main_scrollbar_default;
	if (style_screen_font_holder_ta_simsun_24_main_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_simsun_24_main_scrollbar_default);
	else
		lv_style_init(&style_screen_font_holder_ta_simsun_24_main_scrollbar_default);
	lv_style_set_radius(&style_screen_font_holder_ta_simsun_24_main_scrollbar_default, 0);
	lv_style_set_bg_color(&style_screen_font_holder_ta_simsun_24_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_simsun_24_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_simsun_24_main_scrollbar_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_simsun_24_main_scrollbar_default, 255);
	lv_obj_add_style(ui->screen_font_holder_ta_simsun_24, &style_screen_font_holder_ta_simsun_24_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->screen_font_holder_ta_simsun_24,"语音命令一二三四五六楼确认取消删除用户");
	if (g_kb == NULL)
	{
		g_kb = lv_keyboard_create(ui->screen_font_holder);
		lv_obj_add_event_cb(g_kb, kb_event_cb, LV_EVENT_ALL, NULL);
		lv_obj_add_flag(g_kb, LV_OBJ_FLAG_HIDDEN);
	}
	lv_keyboard_set_textarea(g_kb, ui->screen_font_holder_ta_simsun_24);
	lv_obj_add_event_cb(ui->screen_font_holder_ta_simsun_24, ta_event_cb, LV_EVENT_ALL, g_kb);

	//Write codes screen_font_holder_ta_montserratmedium_24
	ui->screen_font_holder_ta_montserratmedium_24 = lv_textarea_create(ui->screen_font_holder);
	lv_obj_set_pos(ui->screen_font_holder_ta_montserratmedium_24, 16, 428);
	lv_obj_set_size(ui->screen_font_holder_ta_montserratmedium_24, 1070, 120);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_montserratmedium_24_main_main_default
	static lv_style_t style_screen_font_holder_ta_montserratmedium_24_main_main_default;
	if (style_screen_font_holder_ta_montserratmedium_24_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_montserratmedium_24_main_main_default);
	else
		lv_style_init(&style_screen_font_holder_ta_montserratmedium_24_main_main_default);
	lv_style_set_radius(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, 4);
	lv_style_set_bg_color(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, 255);
	lv_style_set_border_color(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, 2);
	lv_style_set_text_color(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, 2);
	lv_style_set_text_align(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, 2);
	lv_style_set_pad_right(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, 2);
	lv_style_set_pad_top(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, 2);
	lv_style_set_pad_bottom(&style_screen_font_holder_ta_montserratmedium_24_main_main_default, 2);
	lv_obj_add_style(ui->screen_font_holder_ta_montserratmedium_24, &style_screen_font_holder_ta_montserratmedium_24_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_montserratmedium_24_main_scrollbar_default
	static lv_style_t style_screen_font_holder_ta_montserratmedium_24_main_scrollbar_default;
	if (style_screen_font_holder_ta_montserratmedium_24_main_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_montserratmedium_24_main_scrollbar_default);
	else
		lv_style_init(&style_screen_font_holder_ta_montserratmedium_24_main_scrollbar_default);
	lv_style_set_radius(&style_screen_font_holder_ta_montserratmedium_24_main_scrollbar_default, 0);
	lv_style_set_bg_color(&style_screen_font_holder_ta_montserratmedium_24_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_montserratmedium_24_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_montserratmedium_24_main_scrollbar_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_montserratmedium_24_main_scrollbar_default, 255);
	lv_obj_add_style(ui->screen_font_holder_ta_montserratmedium_24, &style_screen_font_holder_ta_montserratmedium_24_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->screen_font_holder_ta_montserratmedium_24,"Voice commands Bestätigen Deuxième Troisième Quatrième Cinquième préférence Sixième Confirmer  Annuler Supprimer  Fünfte");
	if (g_kb == NULL)
	{
		g_kb = lv_keyboard_create(ui->screen_font_holder);
		lv_obj_add_event_cb(g_kb, kb_event_cb, LV_EVENT_ALL, NULL);
		lv_obj_add_flag(g_kb, LV_OBJ_FLAG_HIDDEN);
	}
	lv_keyboard_set_textarea(g_kb, ui->screen_font_holder_ta_montserratmedium_24);
	lv_obj_add_event_cb(ui->screen_font_holder_ta_montserratmedium_24, ta_event_cb, LV_EVENT_ALL, g_kb);

	//Write codes screen_font_holder_ta_simsun_48
	ui->screen_font_holder_ta_simsun_48 = lv_textarea_create(ui->screen_font_holder);
	lv_obj_set_pos(ui->screen_font_holder_ta_simsun_48, 16, 572);
	lv_obj_set_size(ui->screen_font_holder_ta_simsun_48, 1070, 60);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_simsun_48_main_main_default
	static lv_style_t style_screen_font_holder_ta_simsun_48_main_main_default;
	if (style_screen_font_holder_ta_simsun_48_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_simsun_48_main_main_default);
	else
		lv_style_init(&style_screen_font_holder_ta_simsun_48_main_main_default);
	lv_style_set_radius(&style_screen_font_holder_ta_simsun_48_main_main_default, 4);
	lv_style_set_bg_color(&style_screen_font_holder_ta_simsun_48_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_simsun_48_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_simsun_48_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_simsun_48_main_main_default, 255);
	lv_style_set_border_color(&style_screen_font_holder_ta_simsun_48_main_main_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_screen_font_holder_ta_simsun_48_main_main_default, 2);
	lv_style_set_text_color(&style_screen_font_holder_ta_simsun_48_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_font_holder_ta_simsun_48_main_main_default, &lv_font_simsun_48);
	lv_style_set_text_letter_space(&style_screen_font_holder_ta_simsun_48_main_main_default, 2);
	lv_style_set_text_align(&style_screen_font_holder_ta_simsun_48_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_font_holder_ta_simsun_48_main_main_default, 2);
	lv_style_set_pad_right(&style_screen_font_holder_ta_simsun_48_main_main_default, 2);
	lv_style_set_pad_top(&style_screen_font_holder_ta_simsun_48_main_main_default, 2);
	lv_style_set_pad_bottom(&style_screen_font_holder_ta_simsun_48_main_main_default, 2);
	lv_obj_add_style(ui->screen_font_holder_ta_simsun_48, &style_screen_font_holder_ta_simsun_48_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_simsun_48_main_scrollbar_default
	static lv_style_t style_screen_font_holder_ta_simsun_48_main_scrollbar_default;
	if (style_screen_font_holder_ta_simsun_48_main_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_simsun_48_main_scrollbar_default);
	else
		lv_style_init(&style_screen_font_holder_ta_simsun_48_main_scrollbar_default);
	lv_style_set_radius(&style_screen_font_holder_ta_simsun_48_main_scrollbar_default, 0);
	lv_style_set_bg_color(&style_screen_font_holder_ta_simsun_48_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_simsun_48_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_simsun_48_main_scrollbar_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_simsun_48_main_scrollbar_default, 255);
	lv_obj_add_style(ui->screen_font_holder_ta_simsun_48, &style_screen_font_holder_ta_simsun_48_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->screen_font_holder_ta_simsun_48,"请说：“你好，恩智浦”来唤醒语音控制返回");
	if (g_kb == NULL)
	{
		g_kb = lv_keyboard_create(ui->screen_font_holder);
		lv_obj_add_event_cb(g_kb, kb_event_cb, LV_EVENT_ALL, NULL);
		lv_obj_add_flag(g_kb, LV_OBJ_FLAG_HIDDEN);
	}
	lv_keyboard_set_textarea(g_kb, ui->screen_font_holder_ta_simsun_48);
	lv_obj_add_event_cb(ui->screen_font_holder_ta_simsun_48, ta_event_cb, LV_EVENT_ALL, g_kb);

	//Write codes screen_font_holder_ta_montserratmedium_48
	ui->screen_font_holder_ta_montserratmedium_48 = lv_textarea_create(ui->screen_font_holder);
	lv_obj_set_pos(ui->screen_font_holder_ta_montserratmedium_48, 15, 642);
	lv_obj_set_size(ui->screen_font_holder_ta_montserratmedium_48, 1070, 60);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_montserratmedium_48_main_main_default
	static lv_style_t style_screen_font_holder_ta_montserratmedium_48_main_main_default;
	if (style_screen_font_holder_ta_montserratmedium_48_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_montserratmedium_48_main_main_default);
	else
		lv_style_init(&style_screen_font_holder_ta_montserratmedium_48_main_main_default);
	lv_style_set_radius(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, 4);
	lv_style_set_bg_color(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, 255);
	lv_style_set_border_color(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, 2);
	lv_style_set_text_color(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, &lv_font_montserratMedium_48);
	lv_style_set_text_letter_space(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, 2);
	lv_style_set_text_align(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, 2);
	lv_style_set_pad_right(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, 2);
	lv_style_set_pad_top(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, 2);
	lv_style_set_pad_bottom(&style_screen_font_holder_ta_montserratmedium_48_main_main_default, 2);
	lv_obj_add_style(ui->screen_font_holder_ta_montserratmedium_48, &style_screen_font_holder_ta_montserratmedium_48_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_ta_montserratmedium_48_main_scrollbar_default
	static lv_style_t style_screen_font_holder_ta_montserratmedium_48_main_scrollbar_default;
	if (style_screen_font_holder_ta_montserratmedium_48_main_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_ta_montserratmedium_48_main_scrollbar_default);
	else
		lv_style_init(&style_screen_font_holder_ta_montserratmedium_48_main_scrollbar_default);
	lv_style_set_radius(&style_screen_font_holder_ta_montserratmedium_48_main_scrollbar_default, 0);
	lv_style_set_bg_color(&style_screen_font_holder_ta_montserratmedium_48_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_font_holder_ta_montserratmedium_48_main_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_ta_montserratmedium_48_main_scrollbar_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_ta_montserratmedium_48_main_scrollbar_default, 255);
	lv_obj_add_style(ui->screen_font_holder_ta_montserratmedium_48, &style_screen_font_holder_ta_montserratmedium_48_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->screen_font_holder_ta_montserratmedium_48,"Zurück à  réveiller Salut, NXP Rücken");
	if (g_kb == NULL)
	{
		g_kb = lv_keyboard_create(ui->screen_font_holder);
		lv_obj_add_event_cb(g_kb, kb_event_cb, LV_EVENT_ALL, NULL);
		lv_obj_add_flag(g_kb, LV_OBJ_FLAG_HIDDEN);
	}
	lv_keyboard_set_textarea(g_kb, ui->screen_font_holder_ta_montserratmedium_48);
	lv_obj_add_event_cb(ui->screen_font_holder_ta_montserratmedium_48, ta_event_cb, LV_EVENT_ALL, g_kb);
}