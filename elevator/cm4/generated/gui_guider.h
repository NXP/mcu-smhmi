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
	lv_obj_t *home_img_background;
	lv_obj_t *home_imgbtn_1;
	lv_obj_t *home_imgbtn_1_label;
	lv_obj_t *home_imgbtn_2;
	lv_obj_t *home_imgbtn_2_label;
	lv_obj_t *home_imgbtn_3;
	lv_obj_t *home_imgbtn_3_label;
	lv_obj_t *home_imgbtn_4;
	lv_obj_t *home_imgbtn_4_label;
	lv_obj_t *home_imgbtn_5;
	lv_obj_t *home_imgbtn_5_label;
	lv_obj_t *home_imgbtn_6;
	lv_obj_t *home_imgbtn_6_label;
	lv_obj_t *home_imgbtn_doorOpen;
	lv_obj_t *home_imgbtn_doorOpen_label;
	lv_obj_t *home_imgbtn_alarm;
	lv_obj_t *home_imgbtn_alarm_label;
	lv_obj_t *home_imgbtn_doorClose;
	lv_obj_t *home_imgbtn_doorClose_label;
	lv_obj_t *home_img_arrow;
	lv_obj_t *home_roller_floorNum;
	lv_obj_t *home_label_arrived;
	lv_obj_t *home_img_faceNotDetected;
	lv_obj_t *home_img_faceDetected;
	lv_obj_t *home_img_faceRecognized;
	lv_obj_t *home_imgbtn_deregister;
	lv_obj_t *home_imgbtn_deregister_label;
	lv_obj_t *home_ddlist_languageSel;
	lv_obj_t *home_label_algoinfo;
	lv_obj_t *home_btn_nxpFaceRec;
	lv_obj_t *home_btn_nxpFaceRec_label;
	lv_obj_t *home_img_date;
	lv_obj_t *home_img_temp;
	lv_obj_t *home_img_sun;
	lv_obj_t *home_imgbtn_help;
	lv_obj_t *home_imgbtn_help_label;
	lv_obj_t *home_label_help;
	lv_obj_t *home_img_mic_on;
	lv_obj_t *home_img_mic_off;
	lv_obj_t *home_imgbtn_confirm;
	lv_obj_t *home_imgbtn_confirm_label;
	lv_obj_t *home_imgbtn_cancel;
	lv_obj_t *home_imgbtn_cancel_label;
	lv_obj_t *home_label_face_rec_state;
	lv_obj_t *home_img_nxp_logo;
	lv_obj_t *home_img_time;
	lv_obj_t *home_btn_enable_disable_confirm;
	lv_obj_t *home_btn_enable_disable_confirm_label;
	lv_obj_t *home_btn_turn_on_off_mic;
	lv_obj_t *home_btn_turn_on_off_mic_label;
	lv_obj_t *help;
	lv_obj_t *help_imgbtn_home;
	lv_obj_t *help_imgbtn_home_label;
	lv_obj_t *help_cont_commands;
	lv_obj_t *help_label_delete_user;
	lv_obj_t *help_label_cancel;
	lv_obj_t *help_label_confirm;
	lv_obj_t *help_label_floor_six;
	lv_obj_t *help_label_floor_five;
	lv_obj_t *help_label_floor_four;
	lv_obj_t *help_label_floor_three;
	lv_obj_t *help_label_floor_one;
	lv_obj_t *help_label_floor_two;
	lv_obj_t *help_label_commands;
	lv_obj_t *help_cont_tips;
	lv_obj_t *help_label_say_ww;
	lv_obj_t *help_label_enable_voice;
	lv_obj_t *help_imgbtn_back;
	lv_obj_t *help_imgbtn_back_label;
	lv_obj_t *help_img_nxp_logo;
	lv_obj_t *help_label_back;
	lv_obj_t *help_label_voice_command;
	lv_obj_t *help_ddlist_languageSel;
	lv_obj_t *screen_font_holder;
	lv_obj_t *screen_font_holder_ta_simsun_32;
	lv_obj_t *screen_font_holder_ta_montserratmedium_32;
	lv_obj_t *screen_font_holder_ta_simsun_20;
	lv_obj_t *screen_font_holder_ta_montserratmedium_20;
	lv_obj_t *screen_font_holder_ta_simsun_22;
	lv_obj_t *screen_font_holder_ta_montserratmedium_22;
	lv_obj_t *screen_font_holder_ta_simsun_24;
	lv_obj_t *screen_font_holder_ta_montserratmedium_24;
	lv_obj_t *screen_font_holder_ta_simsun_48;
	lv_obj_t *screen_font_holder_ta_montserratmedium_48;
}lv_ui;

void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_main(lv_ui *ui);

#include "lvgl/src/extra/widgets/animimg/lv_animimg.h"
LV_IMG_DECLARE(main_animimg_timeoutmain_rb_10_10)
void setup_scr_home(lv_ui *ui);
void setup_scr_help(lv_ui *ui);
void setup_scr_screen_font_holder(lv_ui *ui);
LV_IMG_DECLARE(_button_open_pressed_alpha_90x90);
LV_IMG_DECLARE(_button_close_pressed_alpha_90x90);
LV_IMG_DECLARE(_button_delete_alpha_62x62);
LV_IMG_DECLARE(_virtual_face_green_180x185);
LV_IMG_DECLARE(_home_help_91x91);
LV_IMG_DECLARE(_button_gold_alpha_90x90);
LV_IMG_DECLARE(_nxp_logo_113x38);
LV_IMG_DECLARE(_virtual_face_red_180x185);
LV_IMG_DECLARE(_home_mic_off_61x111);
LV_IMG_DECLARE(_button_close_alpha_90x90);
LV_IMG_DECLARE(_button_delete_pressed_alpha_62x62);
LV_IMG_DECLARE(_button_call_alpha_90x90);
LV_IMG_DECLARE(_home_temp_262x95);
LV_IMG_DECLARE(_up_64x56);
LV_IMG_DECLARE(_main_elevator_1280x720);
LV_IMG_DECLARE(_home_time_225x55);
LV_IMG_DECLARE(_button_call_pressed_alpha_90x90);
LV_IMG_DECLARE(_virtual_face_blue_180x185);
LV_IMG_DECLARE(_home_confirm_264x139);
LV_IMG_DECLARE(_button_gold_pressed_alpha_90x90);
LV_IMG_DECLARE(_home_date_en_560x90);
LV_IMG_DECLARE(_home_sun_128x135);
LV_IMG_DECLARE(_back_alpha_81x82);
LV_IMG_DECLARE(_home_mic_on_118x197);
LV_IMG_DECLARE(_home_background_1280x720);
LV_IMG_DECLARE(_button_open_alpha_90x90);
LV_IMG_DECLARE(_home_cancel_264x139);
LV_IMG_DECLARE(_button_silver_alpha_90x90);
LV_IMG_DECLARE(_button_silver_pressed_alpha_90x90);

#ifdef __cplusplus
}
#endif
#endif