// SPDX-License-Identifier: MIT
// Copyright 2022 NXP

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
#define NO_DELAY              0
#define LVGL_MULTITHREAD_LOCK 1

/* VIT ASR does not support French currently.
 * When VIT is used we must set this definition to 0. */
#define FRENCH_LANG_SUPPORTED 1

#define NUM_FLOORS          6
#define ROLLER_INDEX(floor) ((NUM_FLOORS)-floor)

typedef enum _virtual_face_color
{
    kVirtualFace_Red = 0,
    kVirtualFace_Green,
    kVirtualFace_Blue,
    kVirtualFace_Invalid
} virtual_face_color_t;

typedef enum _face_rec_state
{
    kFaceRec_NoUser = 0,
    kFaceRec_NewUser,
    kFaceRec_KnownUser,
    kFaceRec_NumStates,
} face_rec_state_t;

enum
{
    PROMPT_CONFIRM_TONE = 0,
    PROMPT_TIMEOUT_TONE,
    PROMPT_ALARM_TONE,
    PROMPT_OPEN_DOOR_TONE,
    PROMPT_FLOORONE,
    PROMPT_FLOORTWO,
    PROMPT_FLOORTHREE,
    PROMPT_FLOORFOUR,
    PROMPT_FLOORFIVE,
    PROMPT_FLOORSIX,
    PROMPT_ONE_CONFIRM_CANCEL,
    PROMPT_TWO_CONFIRM_CANCEL,
    PROMPT_THREE_CONFIRM_CANCEL,
    PROMPT_FOUR_CONFIRM_CANCEL,
    PROMPT_FIVE_CONFIRM_CANCEL,
    PROMPT_SIX_CONFIRM_CANCEL,
    PROMPT_REGISTER_SELECTION,
    PROMPT_WHICH_FLOOR,
    PROMPT_INVALID
};

typedef enum
{
    kScreen_Main = 0,
    kScreen_Home,
    kScreen_Help,
    kScreen_Num
} screen_t;

typedef enum _elevator_state_t
{
    kElevatorState_Idle = 0,
    kElevatorState_Start,
    kElevatorState_StartSameFloor,
    kElevatorState_Arrival,
    kElevatorState_Waiting,
    kElevatorState_DoorOpen,
    kElevatorState_DoorClose,
    kElevatorState_Invalid
} elevator_state_t;

typedef enum
{
    kLanguageType_Invalid = -1,
    kLanguageType_English,
    kLanguageType_Chinese,
    kLanguageType_German,
    kLanguageType_French,
    kLanguageType_NumTypes
} language_type_t;

#ifdef RT_PLATFORM
/**
 * Running in the smart HMI kit
 */
#define enable_disable_confirm_cancel_callback()
#define turn_on_off_mic_callback()

#define start_alarm_sound() handle_alarm_action()
#define stop_alarm_sound()

void _takeLVGLMutex();
void _giveLVGLMutex();

void PlayPrompt(int id, uint8_t asrEnabled);
void StopPrompt(void);
void DeregisterUser(void);

/**
 * @brief enter screen callback
 * @return 1 if the enter screen operation was a success, 0 otherwise
 */
uint8_t UI_EnterScreen_Callback(screen_t screenId);
uint8_t UI_ElevatorStart_Callback(void);
void UI_ElevatorArrived_Callback(void);

/**
 * @brief Async call to the UI to advertise set language.
 */
void UI_SetLanguage_Callback(language_type_t language);

bool FaceRecDebugOptionTrigger(void);

void UI_Confirm_Callback(void);
void UI_Cancel_Callback(void);

#else
/**
 * Running in the lvgl emalator
 */
#define LVGL_LOG printf

#define PlayPrompt(x, y)
#define StopPrompt()
#define DeregisterUser()
#define start_alarm_sound()
#define stop_alarm_sound()
#define FaceRecDebugOptionTrigger() true

#define UI_SetLanguage_Callback(language)
#define UI_EnterScreen_Callback(scr) (1)
#define UI_ElevatorStart_Callback()  (1)
#define UI_ElevatorArrived_Callback()
#define UI_Confirm_Callback()
#define UI_Cancel_Callback()

void enable_disable_confirm_cancel_callback(void);
void turn_on_off_mic_callback(void);
#endif /*RT_PLATFORM*/

LV_IMG_DECLARE(_home_date_cn_560x90);
LV_IMG_DECLARE(_home_date_de_560x90);
LV_IMG_DECLARE(_home_date_fr_560x90);

void _idle_callback();

void setup_imgs(unsigned char *base);
void go_to_floor(uint32_t floorNum);
bool elevator_in_motion(void);

bool gui_get_alarm_status(void);
void gui_set_alarm_status(bool status);

/**
 * @brief starts playing the alarm sound effect
 *
 */
void handle_alarm_action(void);

uint32_t get_current_floor(void);
elevator_state_t get_elevator_state(void);
void set_elevator_state(elevator_state_t state);

const char *get_screen_name(screen_t scr);
face_rec_state_t get_current_face_rec_state(void);

void gui_set_virtual_face(virtual_face_color_t color);

void gui_load_screen(screen_t scr_id, int delay);
screen_t gui_get_screen_id(void);
void gui_set_home(void);
void gui_set_help(void);
void gui_main_anim_ready_cb(struct _lv_anim_t *anim);

/**
 * @brief get the language type selected on main screen
 *
 * @return language_type_t the current selected language type
 *
 */
language_type_t gui_get_language_type(void);

/**
 * @brief Set the language in the droplist
 *
 * @param Language.
 */
void gui_home_set_language(language_type_t language);
void gui_home_update_face_rec_state(face_rec_state_t state);
void gui_home_language_changed_cb(void);
void gui_help_language_changed_cb(void);
void gui_home_debug_info_cb(void);

void gui_enable_mic(bool enable);
void gui_enable_confirm_cancel(bool enable);
void gui_set_algo_debuginfo(uint8_t isDetFace,
                            uint8_t isSmallFace,
                            uint8_t isBlurry,
                            uint8_t isSideface,
                            uint8_t brightness,
                            int recognized,
                            int id);
void custom_init(lv_ui *ui);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* EVENT_CB_H_ */
