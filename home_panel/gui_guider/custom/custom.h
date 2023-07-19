// SPDX-License-Identifier: MIT
// Copyright 2022 NXP

/*
 * custom.h
 *
 *  Created on: July 29, 2020
 *      Author: nxf53801
 */

#ifndef __CUSTOM_H_
#define __CUSTOM_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*********************
 *      INCLUDES
 *********************/
#include "gui_guider.h"

/*********************
 *      DEFINES
 *********************/
#define MAX_THERMOSTAT_USER_NUM 	   20
#define MAX_THERMOSTAT_USER_NAME_SIZE  26 //including the terminating null character (1 byte)
#define TEMPERATURE_VALUE_SIZE		   6
/**********************
 *      TYPEDEFS
 **********************/
typedef enum
{
    kScreen_Home = 0,
    kScreen_Thermostat,
    kScreen_Security,
    kScreen_AudioPlayer,
    kScreen_Security_FaceReg,
    kScreen_Thermostat_FaceReg,
	kScreen_Thermostat_UserList,
	kScreen_Thermostat_UserEdit,
    kScreen_Num
} home_panel_screen_id_t;

typedef enum
{
    kLanguage_EN = 0,
    kLanguage_CN,
    kLanguage_DE,
    kLanguage_FR,
    kLanguage_Ids
} language_t;

typedef enum
{
    kAudioState_Invalid = -1,
    kAudioState_Pause,
    kAudioState_Play,
    kAudioState_Max = 2
} audio_state_t;

typedef enum
{
    kAudioVolume_Invalid = -1,
    kAudioVolume_Default = 40,
    kAudioVolume_Max     = 101
} audio_volume_t;

typedef enum
{
    kAudioPlayer_Play,
    kAudioPlayer_Pause,
    kAudioPlayer_Next,
    kAudioPlayer_Previous,
    kAudioPlayer_Stop,
    kAudioPlayer_Load,
    kAudioPlayer_VolUp,
    kAudioPlayer_VolDown
} audio_event_t;

typedef struct
{
    const char *name;
    const void *album_img;
    uint32_t elapsed_time;
    uint32_t total_time;
} audio_info_t;

typedef struct
{
    audio_info_t audio_info;
    audio_state_t state;
    uint8_t speaker_volume;
} audio_player_obj_t;

typedef enum
{
    kThermostat_Off,
    kThermostat_On,
} thermostat_state;

typedef enum
{
    kThermostat_Auto,
    kThermostat_Cool,
    kThermostat_Heat,
	kThermostat_NumMode
} thermostat_mode;

typedef enum
{
    kThermostat_Fan_Low,
    kThermostat_Fan_Mid,
    kThermostat_Fan_High,
    kThermostat_Fan_Auto,
    kThermostat_NumFanSpeed
} thermostat_fan_speed;

typedef enum
{
    kThermostat_Fan_On,
    kThermostat_Fan_Off,
} thermostat_fan_state;

typedef struct _thermostat_obj
{
    thermostat_state state;
    thermostat_mode mode;
    thermostat_fan_state fan_state;
    thermostat_fan_speed fan_speed;
} thermostat_obj_t;

typedef struct _thermostat_user_list_obj
{
	char user_name[MAX_THERMOSTAT_USER_NAME_SIZE];
	char temperature[TEMPERATURE_VALUE_SIZE];
	thermostat_mode mode;
	thermostat_fan_speed fan_speed;
} thermostat_user_list_obj_t;

typedef enum
{
    kSecurity_Off,
    kSecurity_On,
} security_state;

typedef enum
{
    kSecurity_Front,
    kSecurity_Back,
    kSecurity_Garage,
} security_door;

typedef struct _security_obj
{
    security_state state; /* kSecurity_Off: disarmed kSecurity_On: armed */
    security_state front_door_state;
    security_state back_door_state;
    security_state garage_door_state;
} security_obj_t;

#ifndef LVGL_MULTITHREAD_LOCK
#define LVGL_MULTITHREAD_LOCK 1
#endif /* LVGL_MULTITHREAD_LOCK */
/**********************
 *
 **********************/
#define VOLUME_STEP_SIZE 20

#ifdef RT_PLATFORM
#define LVGL_LOG(...)
#if LVGL_MULTITHREAD_LOCK
void _takeLVGLMutex();
void _giveLVGLMutex();
#define LVGL_LOCK()   _takeLVGLMutex()
#define LVGL_UNLOCK() _giveLVGLMutex()
#else
#define LVGL_LOCK()
#define LVGL_UNLOCK()
#endif /* LVGL_MULTITHREAD_LOCK */

#else
/* for the LVGL-Simulator */
#define LVGL_LOG printf
#define UI_AudioPlaying_Callback(x)
#define UI_VolumeChangeNotify(x)
#define UI_SetLanguage_Callback(x)
#define UI_ExitScreen_Callback(x)
#endif /* RT_PLATFORM */

/* Common */
void gui_load_screen(int src_scr_id, int dst_scr_id, int delay);
home_panel_screen_id_t get_current_screen(void);
home_panel_screen_id_t get_next_screen(void);

void gui_enable_mic(home_panel_screen_id_t screenId, bool enable);
void gui_volume_up(void);
void gui_volume_down(void);

void gui_lvgl_enable_buttons(lv_obj_t *btn_obj, bool en);

/* Callbacks */
void UI_AudioPlaying_Callback(audio_event_t event);
void UI_EnterScreen_Callback(home_panel_screen_id_t screenId);
void UI_ExitScreen_Callback(home_panel_screen_id_t screenId);
void UI_VolumeChangeNotify(uint8_t volume);
void UI_SetLanguage_Callback(language_t language);
language_t UI_GetLanguage(void);

void custom_init(lv_ui *ui);

/* Thermostat */
thermostat_state gui_thermostat_get_state();
void gui_thermostat_set_state(thermostat_state state);
void gui_thermostat_switch_state(void);
thermostat_mode gui_thermostat_get_mode();
void gui_thermostat_set_mode(thermostat_mode state);
thermostat_fan_speed gui_thermostat_get_fan_speed();
void gui_thermostat_set_fan_speed(thermostat_fan_speed speed);
void gui_thermostat_switch_fan_state(void);
void gui_thermostat_set_fan_state(thermostat_fan_state state);
void gui_thermostat_init_state(void);

void gui_thermostat_lvgl_hide_user_editor(bool hiding);
void gui_thermostat_lvgl_hide_user_del_ui(bool hiding);
void gui_thermostat_lvgl_focus_user_name(void);
void gui_thermostat_lvgl_focus_user_item(bool focusing);
void gui_thermostat_lvgl_update_kb_style(void);
void gui_thermostat_lvgl_update_cursor_color(lv_color_t color);
void gui_thermostat_events_init_user_misc_widgets(lv_ui *ui);
void gui_thermostat_lvgl_setup_user_items(void);
void gui_thermostat_lvgl_refresh_user_list(uint8_t item_no);
void gui_thermostat_lvgl_update_user_item(thermostat_user_list_obj_t *user_info, uint8_t item_no);
void gui_thermostat_lvgl_edit_user_item(void);
void gui_thermostat_lvgl_refresh_user_editing_item(void);

thermostat_user_list_obj_t gui_thermostat_get_user_info(uint8_t item_no);
void gui_thermostat_set_user_info(thermostat_user_list_obj_t *user_info, uint8_t item_no);
uint8_t gui_thermostat_get_user_item_num(void);
void gui_thermostat_set_user_item_num(uint8_t item_num);
uint8_t gui_thermostat_get_user_item_no(void);
void gui_thermostat_set_user_item_no(uint8_t item_no);

void gui_thermostat_lvgl_add_user_info();

/* Security */
thermostat_state gui_security_get_state();
void gui_security_set_state(security_state state);
void gui_security_switch_state(void);
security_state gui_security_get_door_state(security_door door);
void gui_security_set_door_state(security_door door, security_state state);
void gui_security_switch_door_state(security_door door);

void gui_thermostat_lvgl_add_user_info(void);

/* Audio Player */
void set_audio_position(uint32_t elapsed_time);
void refresh_audio_position();

void set_audio_info(audio_info_t audioInfo);
void refresh_audio_player_info();

void set_audio_player_state(audio_state_t state);
void refresh_audio_player_state();
audio_state_t get_audio_player_state(void);
audio_volume_t get_speaker_volume(void);
void set_speaker_volume(audio_volume_t volume);
void refresh_audio_player_volume(void);

void switch_audio_player_state(audio_state_t new_state);
void switch_audio_clip(audio_event_t switch_event, audio_state_t state);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* EVENT_CB_H_ */
