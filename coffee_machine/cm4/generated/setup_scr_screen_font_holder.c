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

	//Write codes screen_font_holder_label_simsun_20
	ui->screen_font_holder_label_simsun_20 = lv_label_create(ui->screen_font_holder);
	lv_obj_set_pos(ui->screen_font_holder_label_simsun_20, 10, 10);
	lv_obj_set_size(ui->screen_font_holder_label_simsun_20, 1063, 24);
	lv_label_set_text(ui->screen_font_holder_label_simsun_20, "：杯型浓度");
	lv_label_set_long_mode(ui->screen_font_holder_label_simsun_20, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_font_holder_label_simsun_20, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_font_holder_label_simsun_20_main_main_default
	static lv_style_t style_screen_font_holder_label_simsun_20_main_main_default;
	if (style_screen_font_holder_label_simsun_20_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_font_holder_label_simsun_20_main_main_default);
	else
		lv_style_init(&style_screen_font_holder_label_simsun_20_main_main_default);
	lv_style_set_radius(&style_screen_font_holder_label_simsun_20_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_font_holder_label_simsun_20_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_font_holder_label_simsun_20_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_font_holder_label_simsun_20_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_font_holder_label_simsun_20_main_main_default, 0);
	lv_style_set_text_color(&style_screen_font_holder_label_simsun_20_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_font_holder_label_simsun_20_main_main_default, &lv_font_simsun_20);
	lv_style_set_text_letter_space(&style_screen_font_holder_label_simsun_20_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_font_holder_label_simsun_20_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_font_holder_label_simsun_20_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_font_holder_label_simsun_20_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_font_holder_label_simsun_20_main_main_default, 0);
	lv_obj_add_style(ui->screen_font_holder_label_simsun_20, &style_screen_font_holder_label_simsun_20_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_font_holder_ta_simsun_32
	ui->screen_font_holder_ta_simsun_32 = lv_textarea_create(ui->screen_font_holder);
	lv_obj_set_pos(ui->screen_font_holder_ta_simsun_32, 10, 58);
	lv_obj_set_size(ui->screen_font_holder_ta_simsun_32, 1070, 92);

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
	lv_style_set_border_width(&style_screen_font_holder_ta_simsun_32_main_main_default, 1);
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
	lv_textarea_set_text(ui->screen_font_holder_ta_simsun_32,"美意式咖啡拿铁卡布奇诺开始请说：“你好，恩智浦！”或者触摸此屏幕来开启您的选择大小中杯柔润温和浓烈");
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
	lv_obj_set_pos(ui->screen_font_holder_ta_montserratmedium_32, 10, 176);
	lv_obj_set_size(ui->screen_font_holder_ta_montserratmedium_32, 1070, 191);

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
	lv_style_set_border_width(&style_screen_font_holder_ta_montserratmedium_32_main_main_default, 1);
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
	lv_textarea_set_text(ui->screen_font_holder_ta_montserratmedium_32,"Café Latte Klein Mittel Groß Leicht oder Berühre den Bildschirm Wähle deinen Kaffee  Größe Stärke Starten Café Americano Café Latté Court Moyen Long Léger Normal Serré Taille Intensité « Salut NXP » ou Appuyez sur l’écran Pour sélectionner votre café Commencer");
	if (g_kb == NULL)
	{
		g_kb = lv_keyboard_create(ui->screen_font_holder);
		lv_obj_add_event_cb(g_kb, kb_event_cb, LV_EVENT_ALL, NULL);
		lv_obj_add_flag(g_kb, LV_OBJ_FLAG_HIDDEN);
	}
	lv_keyboard_set_textarea(g_kb, ui->screen_font_holder_ta_montserratmedium_32);
	lv_obj_add_event_cb(ui->screen_font_holder_ta_montserratmedium_32, ta_event_cb, LV_EVENT_ALL, g_kb);

	//Write codes screen_font_holder_ta_montserratmedium_20
	ui->screen_font_holder_ta_montserratmedium_20 = lv_textarea_create(ui->screen_font_holder);
	lv_obj_set_pos(ui->screen_font_holder_ta_montserratmedium_20, 15, 416);
	lv_obj_set_size(ui->screen_font_holder_ta_montserratmedium_20, 1070, 76);

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
	lv_textarea_set_text(ui->screen_font_holder_ta_montserratmedium_20,"Größe Stärke Taille Intensité");
	if (g_kb == NULL)
	{
		g_kb = lv_keyboard_create(ui->screen_font_holder);
		lv_obj_add_event_cb(g_kb, kb_event_cb, LV_EVENT_ALL, NULL);
		lv_obj_add_flag(g_kb, LV_OBJ_FLAG_HIDDEN);
	}
	lv_keyboard_set_textarea(g_kb, ui->screen_font_holder_ta_montserratmedium_20);
	lv_obj_add_event_cb(ui->screen_font_holder_ta_montserratmedium_20, ta_event_cb, LV_EVENT_ALL, g_kb);
}