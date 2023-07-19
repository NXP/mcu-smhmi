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
#define MAX_THERMOSTAT_USER_NUM       20
#define MAX_THERMOSTAT_USER_NAME_SIZE 26 // including the terminating null character (1 byte)
#define TEMPERATURE_VALUE_SIZE        6
#define THERMOSTAT_MAX_TEMPERATURE    50
/**********************
 *      TYPEDEFS
 **********************/
typedef enum
{
    kScreen_Home = 0,
    kScreen_Thermostat,
    kScreen_Security,
    kScreen_AudioPlayer,
    kScreen_Security_Pincode,
    kScreen_Security_Arming,
    kScreen_Security_FaceReg,
    kScreen_Security_UserList,
    kScreen_Security_UserEdit,
    kScreen_Thermostat_FaceReg,
    kScreen_Thermostat_UserList,
    kScreen_Thermostat_UserEdit,
    kScreen_Help_Home,
    kScreen_Help_Thermostat,
    kScreen_Help_Security,
    kScreen_Help_AudioPlayer,
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
    kUserState_NoUser = 0,
    kUserState_Known,
    kUserState_Unknown,
    kUserState_Max
} user_state;

typedef enum
{
    kUserDB_Add,
    kUserDB_Remove,
    kUserDB_Update,
} db_op_t;

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
    const char *artist_name;
    const char *track_name;
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
    uint32_t temperature;
    user_state facerec_state;
} thermostat_obj_t;

typedef struct _thermostat_user_list_obj
{
    uint16_t face_id;
    char user_name[MAX_THERMOSTAT_USER_NAME_SIZE];
    char temperature[TEMPERATURE_VALUE_SIZE];
    thermostat_mode mode;
    thermostat_fan_speed fan_speed;
} thermostat_user_list_obj_t;

typedef enum
{
    kSecurity_Uninited,
    kSecurity_Off,
    kSecurity_On,
} security_state;

typedef enum
{
    kSecurity_Auth_Arming,
    kSecurity_Auth_DelUser,
    kSecurity_Auth_ResetPin,
    kSecurity_Auth_NewPin,
    kSecurity_Auth_ConfirmPin,
    kSecurity_Auth_FaceReg,
    kSecurity_Auth_FaceRec,
    kSecurity_Auth_NumMode,
} security_auth_mode;

typedef enum
{
    kSecurity_Front,
    kSecurity_Back,
    kSecurity_Garage,
    kSecurity_Last,
} security_door;

typedef struct _security_obj
{
    security_state state; /* kSecurity_Off: disarmed kSecurity_On: armed */
    security_state auth_state;
    user_state facerec_state;
    security_state front_door_state;
    security_state back_door_state;
    security_state garage_door_state;
} security_obj_t;

#define MAX_USER_NAME_LEN 32
typedef struct _security_user
{
    uint16_t face_id;
    char name[MAX_USER_NAME_LEN];
} security_user_t;

typedef enum
{
    PROMPT_LOAD_SETTING_CONFIRM,
    PROMPT_DELETE_USER_CONFIRM,
    PROMPT_START_REGISTRATION,
    PROMPT_REGISTRATION_VERIFY_PIN,
    PROMPT_INVALID_PIN,
    PROMPT_REGISTRATION_SUCCESS,
    PROMPT_DELETE_USER_VERIFY_PIN,
    PROMPT_USER_DELETED,
    PROMPT_REGISTERED_USER,
    PROMPT_COME_CLOSER,
    PROMPT_STAND_STILL,
    PROMPT_FACE_THE_CAMERA,
    PROMPT_REPOSITION_FACE,
    PROMPT_BEEP,
    PROMPT_MAX,
} prompt_id_t;

typedef enum
{
	USER_NAME_ERR_EMPTY,
	USER_NAME_ERR_ENTER,
	USER_NAME_ERR_REPEATED,
	USER_NAME_ERR_NO,
} user_name_err_num;

typedef enum
{
	BTN_LABEL_CONFIRM,
	BTN_LABEL_CANCEL,
	BTN_LABEL_INVALID
};

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

void gui_home_set_language_text(language_t language);
void gui_home_init_state(language_t language);

/* Common */
void gui_load_screen(int src_scr_id, int dst_scr_id, int delay);
home_panel_screen_id_t get_current_screen(void);
home_panel_screen_id_t get_next_screen(void);

void gui_enable_mic(home_panel_screen_id_t screenId, bool enable);
void audio_volume_up(void);
void audio_volume_max(void);
void audio_volume_mute(void);
void audio_volume_down(void);

/* touch volume up/down event handler */
void gui_volume_up(void);
void gui_volume_down(void);

void gui_lvgl_enable_buttons(lv_obj_t *btn_obj, bool en);
void gui_lvgl_popup_msgbox_user_name_err(const char *title, const char *content);
user_name_err_num gui_lvgl_check_user_name(lv_obj_t *name_obj);

void GUI_AudioPlayer_RefreshHandHolder(const void *handBuffer);
void GUI_AudioPlayer_VolumeUp(void);
void GUI_AudioPlayer_VolumeDown(void);
void GUI_AudioPlayer_VolumeMute(void);
void GUI_AudioPlayer_VolumeMax(void);
void GUI_AudioPlayer_Previous(void);
void GUI_AudioPlayer_Next(void);
void GUI_AudioPlayer_BargeOut(void);
void GUI_AudioPlayer_BargeIn(void);

/* Callbacks */
void UI_EnableKeyboardPreview(bool enable);
void UI_AudioPlaying_Callback(audio_event_t event);
void UI_EnterScreen_Callback(home_panel_screen_id_t screenId);
void UI_ExitScreen_Callback(home_panel_screen_id_t screenId);
void UI_VolumeChangeNotify(uint8_t volume);
void UI_SetLanguage_Callback(language_t language);
void UI_Thermostat_UpdateDB_Callback(const thermostat_user_list_obj_t *user_info, db_op_t op);
language_t UI_GetLanguage(void);
uint16_t UI_GetFaceId(void);
int UI_InitThermostatUserList(thermostat_user_list_obj_t *users, int size);
void UI_Security_UpdateDB_Callback(const security_user_t *user, db_op_t op);
int UI_InitSecurityUserList(security_user_t *user, int size);

void PlayPrompt(int id, uint8_t asrEnabled);

uint16_t UI_GetFaceId(void);
void custom_init(lv_ui *ui);

/* Thermostat */
thermostat_state gui_thermostat_get_state();
void gui_thermostat_switch_state(void);
thermostat_mode gui_thermostat_get_mode();
void gui_thermostat_set_mode(thermostat_mode state);
thermostat_fan_speed gui_thermostat_get_fan_speed();
void gui_thermostat_set_fan_speed(thermostat_fan_speed speed);
void gui_thermostat_switch_fan_state(void);
void gui_thermostat_set_fan_state(thermostat_fan_state state);
void gui_thermostat_init_state(language_t language);
uint32_t gui_thermostat_get_temperature(void);
void gui_thermostat_change_temperature(int delta);
void gui_thermostat_set_temperature(uint32_t temp);
void gui_thermostat_tempknob_event_init(void);
void gui_thermostat_set_facerec_state(user_state state);
void gui_thermostat_set_facereg_state(user_state state);
user_state gui_thermostat_get_facerec_state(void);
void gui_thermostat_load_setting(bool confirm);
void gui_thermostat_enable_confirm_cancel(bool enable);
const thermostat_user_list_obj_t *gui_thermostat_get_user_info_from_faceid(uint16_t face_id);

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
void gui_thermostat_lvgl_add_user_item(thermostat_user_list_obj_t *user_info);
void gui_thermostat_lvgl_edit_user_item(void);
void gui_thermostat_lvgl_refresh_user_editing_item(void);
void gui_thermostat_lvgl_del_selected_user_item(void);

thermostat_user_list_obj_t *gui_thermostat_get_selected_user_info(void);
void gui_thermostat_set_user_info(thermostat_user_list_obj_t *user_info, uint8_t item_no);
uint8_t gui_thermostat_get_user_item_num(void);
void gui_thermostat_set_user_item_num(uint8_t item_num);
uint8_t gui_thermostat_get_user_item_no(void);
void gui_thermostat_set_user_item_no(uint8_t item_no);
void gui_thermostat_init_userlist_fromdb();

void gui_thermostat_useredit_set_language_text(language_t language);
void gui_thermostat_useredit_init(language_t language);

void gui_thermostat_userlist_set_language_text(language_t language);
void gui_thermostat_userlist_init_state(language_t language);

void gui_thermostat_set_language_text(language_t language);
void gui_thermostat_init_state(language_t language);

void gui_thermostat_lvgl_add_user_info();
void gui_thermostat_face_reg_init_state(language_t language);

const char *thermostat_mode_string(language_t lang, thermostat_mode mode);
const char *thermostat_fanspeed_string(language_t lang, thermostat_fan_speed speed);

/* Security */
security_state gui_security_get_state();
void gui_security_set_state(security_state state);
security_state gui_security_get_door_state(security_door door);
void gui_security_set_door_state(security_door door, security_state state);
void gui_security_switch_door_state(security_door door);
void security_switch_state(void);

void gui_security_pincode_append(char c);
void gui_security_pincode_del(void);
void gui_security_pincode_ok(void);
void gui_security_pincode_update_stars(void);
void gui_security_pincode_init_state(language_t language);

security_auth_mode gui_security_get_authentication_mode();
void gui_security_set_authentication_mode(security_auth_mode mode);
void gui_security_arming_init_state(language_t language);

void gui_security_userlist_ui_init(void);
void gui_security_userlist_set_language_text(language_t language);
void gui_security_userlist_init(language_t language);
void gui_security_init_userlist_fromdb(void);
void gui_security_userlist_add_newuser(void);
void gui_security_del_selected_user(void);
void gui_security_update_selected_user(void);
void gui_security_useredit_init(language_t language);
const char *gui_security_get_user_name(int user_no);
int gui_security_get_selected_user_no(void);
int gui_security_get_max_user_num(void);

void gui_security_set_language_text(language_t language);
void gui_security_init_state(language_t language);

void gui_security_face_reg_init_state(language_t language);
void gui_security_set_facereg_state(user_state state);

void gui_thermostat_lvgl_add_user_info(void);

/* Audio Player */
void set_audio_position(uint32_t elapsed_time);
void refresh_audio_position();

void set_audio_info(const audio_info_t *audioInfo);
void refresh_audio_player_info();

void set_audio_player_mute_state(bool state);
bool get_audio_player_mute_state(void);

void set_audio_player_state(audio_state_t state);
void refresh_audio_player_state();
audio_state_t get_audio_player_state(void);
audio_volume_t get_speaker_volume(void);
void set_speaker_volume(audio_volume_t volume);
void set_speaker_volume_alone(audio_volume_t volume);
void refresh_audio_player_volume(void);

void switch_audio_player_state(audio_state_t new_state);
void switch_audio_clip(audio_event_t switch_event, audio_state_t state);

void gui_audio_player_init_state(language_t language);
void gui_audio_player_set_language(language_t language);

/* help screens */
void gui_help_home_init_state(void);
void gui_help_home_language_changed_cb(void);
void gui_help_thermostat_init_state(void);
void gui_help_thermostat_language_changed_cb(void);
void gui_help_security_init_state(void);
void gui_help_security_language_changed_cb(void);
void gui_help_audioplayer_init_state(void);
void gui_help_audioplayer_language_changed_cb(void);

/* Callbacks from ui */

void gui_home_language_changed_cb(void);
void gui_audio_player_language_changed_cb(void);
void gui_security_language_changed_cb(void);
void gui_security_userlist_language_changed_cb(void);
void gui_thermostat_language_changed_cb(void);
void gui_thermostat_user_list_language_changed_cb(void);
void gui_thermostat_user_edit_language_changed_cb(void);
void gui_thermostat_db_add_user(void);
void gui_thermostat_db_remove_user(void);
void gui_thermostat_db_update_user(void);

void setup_imgs(unsigned char *base);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* EVENT_CB_H_ */
