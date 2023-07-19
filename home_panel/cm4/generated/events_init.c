/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#include "custom.h"

#ifdef RT_PLATFORM
#include "ui_strings_internal.h"
#endif

void events_init(lv_ui *ui)
{
}

void video_play(lv_ui *ui)
{

}

static void screen_home_ddlist_language_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
    case LV_EVENT_VALUE_CHANGED:
	{
			gui_home_language_changed_cb();
	}

		break;
	default:
		break;
	}
}

static void screen_home_imgbtn_thermostat_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Home, kScreen_Thermostat, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_home_imgbtn_security_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Home, kScreen_Security, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_home_imgbtn_audio_player_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Home, kScreen_AudioPlayer, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_home_imgbtn_help_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Home, kScreen_Help_Home, 0);
	}
		break;
	default:
		break;
	}
}

void events_init_screen_home(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_home_ddlist_language, screen_home_ddlist_language_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_home_imgbtn_thermostat, screen_home_imgbtn_thermostat_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_home_imgbtn_security, screen_home_imgbtn_security_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_home_imgbtn_audio_player, screen_home_imgbtn_audio_player_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_home_imgbtn_help, screen_home_imgbtn_help_event_handler, LV_EVENT_ALL, ui);
}

static void screen_help_home_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Help_Home, kScreen_Home, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_help_home_ddlist_language_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
	{
		gui_help_home_language_changed_cb();
	}
		break;
	default:
		break;
	}
}

void events_init_screen_help_home(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_help_home_imgbtn_home, screen_help_home_imgbtn_home_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_help_home_ddlist_language, screen_help_home_ddlist_language_event_handler, LV_EVENT_ALL, ui);
}

static void screen_audio_player_imgbtn_play_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		audio_state_t state = get_audio_player_state();
		state = !state;
		switch_audio_player_state(state);
	}
		break;
	default:
		break;
	}
}

static void screen_audio_player_imgbtn_volume_up_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_volume_up();
	}
		break;
	default:
		break;
	}
}

static void screen_audio_player_imgbtn_volume_down_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_volume_down();
	}
		break;
	default:
		break;
	}
}

static void screen_audio_player_ddlist_language_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
        {
            gui_audio_player_language_changed_cb();
        }
        break;
	default:
		break;
	}
}

static void screen_audio_player_imgbtn_pause_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		audio_state_t state = get_audio_player_state();
		state = !state;
		switch_audio_player_state(state);
	}
		break;
	default:
		break;
	}
}

static void screen_audio_player_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_AudioPlayer, kScreen_Home, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_audio_player_imgbtn_previous_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		audio_state_t state = get_audio_player_state();
		switch_audio_clip(kAudioPlayer_Previous,state);
	}
		break;
	default:
		break;
	}
}

static void screen_audio_player_imgbtn_next_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		audio_state_t state = get_audio_player_state();
		switch_audio_clip(kAudioPlayer_Next,state);
	}
		break;
	default:
		break;
	}
}

static void screen_audio_player_imgbtn_help_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_AudioPlayer, kScreen_Help_AudioPlayer, 0);
	}
		break;
	default:
		break;
	}
}

void events_init_screen_audio_player(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_audio_player_imgbtn_play, screen_audio_player_imgbtn_play_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_audio_player_imgbtn_volume_up, screen_audio_player_imgbtn_volume_up_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_audio_player_imgbtn_volume_down, screen_audio_player_imgbtn_volume_down_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_audio_player_ddlist_language, screen_audio_player_ddlist_language_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_audio_player_imgbtn_pause, screen_audio_player_imgbtn_pause_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_audio_player_imgbtn_home, screen_audio_player_imgbtn_home_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_audio_player_imgbtn_previous, screen_audio_player_imgbtn_previous_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_audio_player_imgbtn_next, screen_audio_player_imgbtn_next_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_audio_player_imgbtn_help, screen_audio_player_imgbtn_help_event_handler, LV_EVENT_ALL, ui);
}

static void screen_help_audioplayer_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Help_AudioPlayer, kScreen_Home, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_help_audioplayer_imgbtn_back_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Help_AudioPlayer, kScreen_AudioPlayer, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_help_audioplayer_ddlist_language_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
	{
		gui_help_audioplayer_language_changed_cb();
	}
		break;
	default:
		break;
	}
}

void events_init_screen_help_audioplayer(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_help_audioplayer_imgbtn_home, screen_help_audioplayer_imgbtn_home_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_help_audioplayer_imgbtn_back, screen_help_audioplayer_imgbtn_back_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_help_audioplayer_ddlist_language, screen_help_audioplayer_ddlist_language_event_handler, LV_EVENT_ALL, ui);
}

static void screen_security_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_PRESSED:
	{
	}
		break;
	default:
		break;
	}
}

static void screen_security_ddlist_language_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
        {
            gui_security_language_changed_cb();
        }
        break;
	default:
		break;
	}
}

static void screen_security_imgbtn_front_door_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_switch_door_state(kSecurity_Front);
	}
		break;
	default:
		break;
	}
}

static void screen_security_imgbtn_back_door_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_switch_door_state(kSecurity_Back);
	}
		break;
	default:
		break;
	}
}

static void screen_security_imgbtn_garage_door_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_switch_door_state(kSecurity_Garage);
	}
		break;
	default:
		break;
	}
}

static void screen_security_imgbtn_user_setting_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Security, kScreen_Security_UserList, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_security_imgbtn_help_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Security, kScreen_Help_Security, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_security_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Security, kScreen_Home, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_security_imgbtn_arm_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_set_authentication_mode(kSecurity_Auth_Arming);
		gui_load_screen(kScreen_Security, kScreen_Security_Pincode, 0);
	}
		break;
	default:
		break;
	}
}

void events_init_screen_security(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_security, screen_security_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_ddlist_language, screen_security_ddlist_language_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_imgbtn_front_door, screen_security_imgbtn_front_door_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_imgbtn_back_door, screen_security_imgbtn_back_door_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_imgbtn_garage_door, screen_security_imgbtn_garage_door_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_imgbtn_user_setting, screen_security_imgbtn_user_setting_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_imgbtn_help, screen_security_imgbtn_help_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_imgbtn_home, screen_security_imgbtn_home_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_imgbtn_arm, screen_security_imgbtn_arm_event_handler, LV_EVENT_ALL, ui);
}

static void screen_help_security_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Help_Security, kScreen_Home, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_help_security_ddlist_language_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
	{
		gui_help_security_language_changed_cb();
	}
		break;
	default:
		break;
	}
}

static void screen_help_security_imgbtn_back_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Help_Security, kScreen_Security, 0);
	}
		break;
	default:
		break;
	}
}

void events_init_screen_help_security(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_help_security_imgbtn_home, screen_help_security_imgbtn_home_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_help_security_ddlist_language, screen_help_security_ddlist_language_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_help_security_imgbtn_back, screen_help_security_imgbtn_back_event_handler, LV_EVENT_ALL, ui);
}

static void screen_security_pincode_imgbtn_key_1_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('1');
	}
		break;
	default:
		break;
	}
}

static void screen_security_pincode_imgbtn_key_2_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('2');
	}
		break;
	default:
		break;
	}
}

static void screen_security_pincode_imgbtn_key_3_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('3');
	}
		break;
	default:
		break;
	}
}

static void screen_security_pincode_imgbtn_key_4_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('4');
	}
		break;
	default:
		break;
	}
}

static void screen_security_pincode_imgbtn_key_5_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('5');
	}
		break;
	default:
		break;
	}
}

static void screen_security_pincode_imgbtn_key_6_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('6');
	}
		break;
	default:
		break;
	}
}

static void screen_security_pincode_imgbtn_key_7_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('7');
	}
		break;
	default:
		break;
	}
}

static void screen_security_pincode_imgbtn_key_8_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('8');
	}
		break;
	default:
		break;
	}
}

static void screen_security_pincode_imgbtn_key_9_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('9');
	}
		break;
	default:
		break;
	}
}

static void screen_security_pincode_imgbtn_key_del_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_del();
	}
		break;
	default:
		break;
	}
}

static void screen_security_pincode_imgbtn_key_0_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('0');
	}
		break;
	default:
		break;
	}
}

static void screen_security_pincode_imgbtn_key_ok_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_ok();
	}
		break;
	default:
		break;
	}
}

static void screen_security_pincode_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Security_Pincode, kScreen_Home, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_security_pincode_imgbtn_back_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Security_Pincode, kScreen_Security, 0);
	}
		break;
	default:
		break;
	}
}

void events_init_screen_security_pincode(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_security_pincode_imgbtn_key_1, screen_security_pincode_imgbtn_key_1_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_pincode_imgbtn_key_2, screen_security_pincode_imgbtn_key_2_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_pincode_imgbtn_key_3, screen_security_pincode_imgbtn_key_3_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_pincode_imgbtn_key_4, screen_security_pincode_imgbtn_key_4_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_pincode_imgbtn_key_5, screen_security_pincode_imgbtn_key_5_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_pincode_imgbtn_key_6, screen_security_pincode_imgbtn_key_6_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_pincode_imgbtn_key_7, screen_security_pincode_imgbtn_key_7_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_pincode_imgbtn_key_8, screen_security_pincode_imgbtn_key_8_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_pincode_imgbtn_key_9, screen_security_pincode_imgbtn_key_9_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_pincode_imgbtn_key_del, screen_security_pincode_imgbtn_key_del_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_pincode_imgbtn_key_0, screen_security_pincode_imgbtn_key_0_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_pincode_imgbtn_key_ok, screen_security_pincode_imgbtn_key_ok_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_pincode_imgbtn_home, screen_security_pincode_imgbtn_home_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_pincode_imgbtn_back, screen_security_pincode_imgbtn_back_event_handler, LV_EVENT_ALL, ui);
}

void events_init_screen_security_arming(lv_ui *ui)
{
}

static void screen_security_face_reg_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Security_FaceReg, kScreen_Home, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_back_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Security_FaceReg, kScreen_Security, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_key_1_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('1');
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_key_2_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('2');
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_key_3_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('3');
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_key_4_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('4');
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_key_5_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('5');
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_key_6_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('6');
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_key_7_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('7');
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_key_8_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('8');
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_key_9_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('9');
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_key_del_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_del();
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_key_0_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_append('0');
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_key_ok_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_pincode_ok();
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_save_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_userlist_add_newuser();
		gui_load_screen(kScreen_Security_FaceReg, kScreen_Security_UserList, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_security_face_reg_imgbtn_cancel_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Security_FaceReg, kScreen_Security, 0);
	}
		break;
	default:
		break;
	}
}

void events_init_screen_security_face_reg(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_home, screen_security_face_reg_imgbtn_home_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_back, screen_security_face_reg_imgbtn_back_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_key_1, screen_security_face_reg_imgbtn_key_1_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_key_2, screen_security_face_reg_imgbtn_key_2_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_key_3, screen_security_face_reg_imgbtn_key_3_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_key_4, screen_security_face_reg_imgbtn_key_4_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_key_5, screen_security_face_reg_imgbtn_key_5_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_key_6, screen_security_face_reg_imgbtn_key_6_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_key_7, screen_security_face_reg_imgbtn_key_7_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_key_8, screen_security_face_reg_imgbtn_key_8_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_key_9, screen_security_face_reg_imgbtn_key_9_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_key_del, screen_security_face_reg_imgbtn_key_del_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_key_0, screen_security_face_reg_imgbtn_key_0_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_key_ok, screen_security_face_reg_imgbtn_key_ok_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_save, screen_security_face_reg_imgbtn_save_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_face_reg_imgbtn_cancel, screen_security_face_reg_imgbtn_cancel_event_handler, LV_EVENT_ALL, ui);
}

static void screen_security_user_list_imgbtn_back_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Security_UserList, kScreen_Security, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_security_user_list_ddlist_language_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
	{
		gui_security_userlist_language_changed_cb();
	}
		break;
	default:
		break;
	}
}

static void screen_security_user_list_imgbtn_delete_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_set_authentication_mode(kSecurity_Auth_DelUser);
		gui_load_screen(kScreen_Security_UserList, kScreen_Security_Pincode, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_security_user_list_imgbtn_edit_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Security_UserList, kScreen_Security_UserEdit, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_security_user_list_imgbtn_add_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_set_authentication_mode(kSecurity_Auth_FaceReg);
		gui_load_screen(kScreen_Security_UserList, kScreen_Security_FaceReg, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_security_user_list_imgbtn_pin_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_security_set_authentication_mode(kSecurity_Auth_ResetPin);
		gui_load_screen(kScreen_Security_UserList, kScreen_Security_Pincode, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_security_user_list_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Security_UserList, kScreen_Home, 0);
	}
		break;
	default:
		break;
	}
}

void events_init_screen_security_user_list(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_security_user_list_imgbtn_back, screen_security_user_list_imgbtn_back_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_user_list_ddlist_language, screen_security_user_list_ddlist_language_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_user_list_imgbtn_delete, screen_security_user_list_imgbtn_delete_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_user_list_imgbtn_edit, screen_security_user_list_imgbtn_edit_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_user_list_imgbtn_add, screen_security_user_list_imgbtn_add_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_user_list_imgbtn_pin, screen_security_user_list_imgbtn_pin_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_user_list_imgbtn_home, screen_security_user_list_imgbtn_home_event_handler, LV_EVENT_ALL, ui);
}

static void screen_security_user_edit_imgbtn_save_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		user_name_err_num err_num = gui_lvgl_check_user_name(guider_ui.screen_security_user_edit_ta_user_name);
		if (err_num != USER_NAME_ERR_NO)
		{
			language_t language =  UI_GetLanguage();
			gui_lvgl_popup_msgbox_user_name_err(s_UserNameErrTitleStr[language], s_UserNameErrContentStr[language][err_num]);
		}
		else
		{
			gui_security_update_selected_user();
			gui_load_screen(kScreen_Security_UserEdit, kScreen_Security_UserList, 0);
		}
	}
		break;
	default:
		break;
	}
}

static void screen_security_user_edit_imgbtn_edit_cancel_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Security_UserEdit, kScreen_Security_UserList, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_security_user_edit_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Security_UserEdit, kScreen_Home, 0);
	}
		break;
	default:
		break;
	}
}

void events_init_screen_security_user_edit(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_security_user_edit_imgbtn_save, screen_security_user_edit_imgbtn_save_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_user_edit_imgbtn_edit_cancel, screen_security_user_edit_imgbtn_edit_cancel_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_security_user_edit_imgbtn_home, screen_security_user_edit_imgbtn_home_event_handler, LV_EVENT_ALL, ui);
}

static void screen_thermostat_ddlist_language_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
        {
            gui_thermostat_language_changed_cb();
        }
        break;
	default:
		break;
	}
}

static void screen_thermostat_imgbtn_on_off_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_thermostat_switch_state();
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_imgbtn_mode_heat_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_thermostat_set_mode(kThermostat_Heat);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_imgbtn_mode_auto_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_thermostat_set_mode(kThermostat_Auto);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_imgbtn_mode_cool_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_thermostat_set_mode(kThermostat_Cool);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_imgbtn_user_setting_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Thermostat, kScreen_Thermostat_UserList, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_imgbtn_help_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Thermostat, kScreen_Help_Thermostat, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_imgbtn_fan_onoff_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_thermostat_switch_fan_state();
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_imgbtn_fan_high_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_thermostat_set_fan_speed(kThermostat_Fan_High);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_imgbtn_fan_mid_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_thermostat_set_fan_speed(kThermostat_Fan_Mid);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_imgbtn_fan_low_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_thermostat_set_fan_speed(kThermostat_Fan_Low);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_imgbtn_fan_auto_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_thermostat_set_fan_speed(kThermostat_Fan_Auto);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_imgbtn_confirm_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_thermostat_load_setting(true);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_imgbtn_cancel_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_thermostat_load_setting(false);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Thermostat, kScreen_Home, 0);
	}
		break;
	default:
		break;
	}
}

void events_init_screen_thermostat(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_thermostat_ddlist_language, screen_thermostat_ddlist_language_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_imgbtn_on_off, screen_thermostat_imgbtn_on_off_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_imgbtn_mode_heat, screen_thermostat_imgbtn_mode_heat_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_imgbtn_mode_auto, screen_thermostat_imgbtn_mode_auto_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_imgbtn_mode_cool, screen_thermostat_imgbtn_mode_cool_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_imgbtn_user_setting, screen_thermostat_imgbtn_user_setting_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_imgbtn_help, screen_thermostat_imgbtn_help_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_imgbtn_fan_onoff, screen_thermostat_imgbtn_fan_onoff_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_imgbtn_fan_high, screen_thermostat_imgbtn_fan_high_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_imgbtn_fan_mid, screen_thermostat_imgbtn_fan_mid_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_imgbtn_fan_low, screen_thermostat_imgbtn_fan_low_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_imgbtn_fan_auto, screen_thermostat_imgbtn_fan_auto_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_imgbtn_confirm, screen_thermostat_imgbtn_confirm_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_imgbtn_cancel, screen_thermostat_imgbtn_cancel_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_imgbtn_home, screen_thermostat_imgbtn_home_event_handler, LV_EVENT_ALL, ui);
}

static void screen_help_thermostat_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Help_Thermostat, kScreen_Home, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_help_thermostat_imgbtn_back_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Help_Thermostat, kScreen_Thermostat, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_help_thermostat_ddlist_language_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
	{
		gui_help_thermostat_language_changed_cb();
	}
		break;
	default:
		break;
	}
}

void events_init_screen_help_thermostat(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_help_thermostat_imgbtn_home, screen_help_thermostat_imgbtn_home_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_help_thermostat_imgbtn_back, screen_help_thermostat_imgbtn_back_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_help_thermostat_ddlist_language, screen_help_thermostat_ddlist_language_event_handler, LV_EVENT_ALL, ui);
}

static void screen_thermostat_face_reg_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Thermostat_FaceReg, kScreen_Home, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_face_reg_imgbtn_save_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		user_name_err_num err_num = gui_lvgl_check_user_name(guider_ui.screen_thermostat_face_reg_ta_user_name);
		if (err_num != USER_NAME_ERR_NO)
		{
			language_t language = UI_GetLanguage();
			gui_lvgl_popup_msgbox_user_name_err(s_UserNameErrTitleStr[language], s_UserNameErrContentStr[language][err_num]);
		}
		else
		{
			// Save user info. to DB
			gui_thermostat_db_add_user();
			gui_thermostat_lvgl_add_user_info();
			// go to user list and add a new item
			gui_load_screen(kScreen_Thermostat_FaceReg, kScreen_Thermostat_UserList, 0);
		}
		
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_face_reg_imgbtn_cancel_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Thermostat_FaceReg, kScreen_Thermostat_UserList, 0);
	}
		break;
	default:
		break;
	}
}

void events_init_screen_thermostat_face_reg(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_thermostat_face_reg_imgbtn_home, screen_thermostat_face_reg_imgbtn_home_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_face_reg_imgbtn_save, screen_thermostat_face_reg_imgbtn_save_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_face_reg_imgbtn_cancel, screen_thermostat_face_reg_imgbtn_cancel_event_handler, LV_EVENT_ALL, ui);
}

static void screen_thermostat_user_list_ddlist_language_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_delete, false);
		gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_edit, false);
	}
		break;
	case LV_EVENT_VALUE_CHANGED:
	{
		gui_thermostat_user_list_language_changed_cb();
	}
	break;
	default:
		break;
	}
}

static void screen_thermostat_user_list_imgbtn_delete_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_thermostat_lvgl_hide_user_del_ui(false);
		gui_thermostat_lvgl_hide_user_editor(true);
		gui_thermostat_lvgl_focus_user_item(true);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_user_list_imgbtn_edit_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Thermostat_UserList, kScreen_Thermostat_UserEdit, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_user_list_imgbtn_add_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Thermostat_UserList, kScreen_Thermostat_FaceReg, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_user_list_imgbtn_del_ok_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_thermostat_lvgl_hide_user_del_ui(true);
		gui_thermostat_lvgl_hide_user_editor(false);
		gui_thermostat_db_remove_user();
		gui_thermostat_lvgl_del_selected_user_item();
		gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_delete, false);
		gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_edit, false);
		
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_user_list_imgbtn_del_cancel_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_thermostat_lvgl_hide_user_del_ui(true);
		gui_thermostat_lvgl_hide_user_editor(false);
		
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_user_list_imgbtn_back_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Thermostat_UserList, kScreen_Thermostat, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_user_list_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Thermostat_UserList, kScreen_Home, 0);
	}
		break;
	default:
		break;
	}
}

void events_init_screen_thermostat_user_list(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_thermostat_user_list_ddlist_language, screen_thermostat_user_list_ddlist_language_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_list_imgbtn_delete, screen_thermostat_user_list_imgbtn_delete_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_list_imgbtn_edit, screen_thermostat_user_list_imgbtn_edit_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_list_imgbtn_add, screen_thermostat_user_list_imgbtn_add_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_list_imgbtn_del_ok, screen_thermostat_user_list_imgbtn_del_ok_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_list_imgbtn_del_cancel, screen_thermostat_user_list_imgbtn_del_cancel_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_list_imgbtn_back, screen_thermostat_user_list_imgbtn_back_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_list_imgbtn_home, screen_thermostat_user_list_imgbtn_home_event_handler, LV_EVENT_ALL, ui);
}

static void screen_thermostat_user_edit_ddlist_language_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
        {
            gui_thermostat_user_edit_language_changed_cb();
        }
        break;
	default:
		break;
	}
}

static void screen_thermostat_user_edit_imgbtn_save_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		user_name_err_num err_num = gui_lvgl_check_user_name(guider_ui.screen_thermostat_user_edit_ta_user_name);
		if (err_num != USER_NAME_ERR_NO)
		{
			language_t language = UI_GetLanguage();
			gui_lvgl_popup_msgbox_user_name_err(s_UserNameErrTitleStr[language], s_UserNameErrContentStr[language][err_num]);
		}
		else
		{
			gui_thermostat_lvgl_edit_user_item();
			gui_thermostat_db_update_user();
			gui_load_screen(kScreen_Thermostat_UserEdit, kScreen_Thermostat_UserList, 0);
		}
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_user_edit_imgbtn_edit_cancel_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Thermostat_UserEdit, kScreen_Thermostat_UserList, 0);
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_user_edit_ddlist_temperatrue_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_user_edit_ddlist_mode_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_user_edit_ddlist_fan_speed_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
	}
		break;
	default:
		break;
	}
}

static void screen_thermostat_user_edit_imgbtn_home_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_load_screen(kScreen_Thermostat_UserEdit, kScreen_Home, 0);
	}
		break;
	default:
		break;
	}
}

void events_init_screen_thermostat_user_edit(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_thermostat_user_edit_ddlist_language, screen_thermostat_user_edit_ddlist_language_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_edit_imgbtn_save, screen_thermostat_user_edit_imgbtn_save_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_edit_imgbtn_edit_cancel, screen_thermostat_user_edit_imgbtn_edit_cancel_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_edit_ddlist_temperatrue, screen_thermostat_user_edit_ddlist_temperatrue_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_edit_ddlist_mode, screen_thermostat_user_edit_ddlist_mode_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_edit_ddlist_fan_speed, screen_thermostat_user_edit_ddlist_fan_speed_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_edit_imgbtn_home, screen_thermostat_user_edit_imgbtn_home_event_handler, LV_EVENT_ALL, ui);
}
