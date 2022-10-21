/*
 * Copyright 2022 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"
#include "guider_fonts.h"

typedef struct
{
	lv_obj_t *main;
	lv_obj_t *main_imgbtn_home;
	lv_obj_t *main_imgbtn_home_label;
	lv_obj_t *main_animimg_timeout;
	lv_obj_t *home;
	lv_obj_t *home_cont_background;
	lv_obj_t *home_img_cameraPreview;
	lv_obj_t *home_img_nxpFaceRec;
	lv_obj_t *home_img_nxpVoiceRec;
	lv_obj_t *home_cont_size;
	lv_obj_t *home_label_size_small;
	lv_obj_t *home_label_size_medium;
	lv_obj_t *home_label_size_large;
	lv_obj_t *home_imgbtn_size_small;
	lv_obj_t *home_imgbtn_size_small_label;
	lv_obj_t *home_imgbtn_size_medium;
	lv_obj_t *home_imgbtn_size_medium_label;
	lv_obj_t *home_imgbtn_size_large;
	lv_obj_t *home_imgbtn_size_large_label;
	lv_obj_t *home_cont_strength;
	lv_obj_t *home_label_strength_soft;
	lv_obj_t *home_label_strength_mild;
	lv_obj_t *home_label_strength_strong;
	lv_obj_t *home_imgbtn_strength_soft;
	lv_obj_t *home_imgbtn_strength_soft_label;
	lv_obj_t *home_imgbtn_strength_mild;
	lv_obj_t *home_imgbtn_strength_mild_label;
	lv_obj_t *home_imgbtn_strength_strong;
	lv_obj_t *home_imgbtn_strength_strong_label;
	lv_obj_t *home_imgbtn_start;
	lv_obj_t *home_imgbtn_start_label;
	lv_obj_t *home_cont_type;
	lv_obj_t *home_imgbtn_americano;
	lv_obj_t *home_imgbtn_americano_label;
	lv_obj_t *home_label_cafelatte;
	lv_obj_t *home_label_espresso;
	lv_obj_t *home_label_cappuccino;
	lv_obj_t *home_label_americano;
	lv_obj_t *home_imgbtn_caffelatte;
	lv_obj_t *home_imgbtn_caffelatte_label;
	lv_obj_t *home_imgbtn_espresso;
	lv_obj_t *home_imgbtn_espresso_label;
	lv_obj_t *home_imgbtn_cappuccino;
	lv_obj_t *home_imgbtn_cappuccino_label;
	lv_obj_t *home_imgbtn_unregister;
	lv_obj_t *home_imgbtn_unregister_label;
	lv_obj_t *home_ddlist_languageSel;
	lv_obj_t *home_label_algoinfo;
	lv_obj_t *home_label_size;
	lv_obj_t *home_label_strength;
	lv_obj_t *home_label_start;
	lv_obj_t *brewing;
	lv_obj_t *brewing_cont_background;
	lv_obj_t *brewing_label_coffee_type;
	lv_obj_t *brewing_label_coffee_size;
	lv_obj_t *brewing_label_coffee_strength;
	lv_obj_t *brewing_cont_brewing;
	lv_obj_t *brewing_bar_fillStatus;
	lv_obj_t *brewing_animimg_brewing;
	lv_obj_t *finished;
	lv_obj_t *finished_cont_background;
	lv_obj_t *finished_animimg_coffee_type;
	lv_obj_t *finished_label_coffee_type;
	lv_obj_t *finished_label_coffee_size;
	lv_obj_t *finished_label_coffee_strength;
	lv_obj_t *standby;
	lv_obj_t *standby_cont_header;
	lv_obj_t *standby_btn_header_home;
	lv_obj_t *standby_btn_header_home_label;
	lv_obj_t *standby_ddlist_languageSel;
	lv_obj_t *standby_img_nxpVoiceRec;
	lv_obj_t *standby_img_nxpFaceRec;
	lv_obj_t *standby_cont_body;
	lv_obj_t *standby_imgbtn_home;
	lv_obj_t *standby_imgbtn_home_label;
	lv_obj_t *standby_animimg_screensaver;
	lv_obj_t *standby_cont_help;
	lv_obj_t *standby_label_select_coffee;
	lv_obj_t *standby_label_touch_ww;
	lv_obj_t *standby_label_or;
	lv_obj_t *standby_label_voice_ww;
	lv_obj_t *screen_font_holder;
	lv_obj_t *screen_font_holder_label_simsun_20;
	lv_obj_t *screen_font_holder_ta_simsun_32;
	lv_obj_t *screen_font_holder_ta_montserratmedium_32;
	lv_obj_t *screen_font_holder_ta_montserratmedium_20;
}lv_ui;

void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_main(lv_ui *ui);

#include "lvgl/src/extra/widgets/animimg/lv_animimg.h"
LV_IMG_DECLARE(main_animimg_timeoutmain_rb_10_10)
void setup_scr_home(lv_ui *ui);
void setup_scr_brewing(lv_ui *ui);

#include "lvgl/src/extra/widgets/animimg/lv_animimg.h"
void setup_scr_finished(lv_ui *ui);

#include "lvgl/src/extra/widgets/animimg/lv_animimg.h"
void setup_scr_standby(lv_ui *ui);

#include "lvgl/src/extra/widgets/animimg/lv_animimg.h"
void setup_scr_screen_font_holder(lv_ui *ui);
LV_IMG_DECLARE(_main_coffee_machine_1280x720);
LV_IMG_DECLARE(_unregister_75x75);
LV_IMG_DECLARE(_Americano_Black_120x120);
LV_IMG_DECLARE(_Selection_Selected_80x60);
LV_IMG_DECLARE(_Americano_Selected_Black_120x120);
LV_IMG_DECLARE(_nxp_voice_rec_300x80);
LV_IMG_DECLARE(_Selection_80x60);
LV_IMG_DECLARE(_Espresso_Selected_Black_120x120);
LV_IMG_DECLARE(_Cafe_Latte_Black_120x120);
LV_IMG_DECLARE(_Cafe_Latte_Selected_Black_120x120);
LV_IMG_DECLARE(_Start_Black_Reshaped_270x120);
LV_IMG_DECLARE(_nxp_face_rec_300x80);
LV_IMG_DECLARE(_Espresso_Black_120x120);
LV_IMG_DECLARE(_Cappuccinno_selected_Black_120x120);
LV_IMG_DECLARE(_Cappuccinno_Black_120x120);

#ifdef __cplusplus
}
#endif
#endif
