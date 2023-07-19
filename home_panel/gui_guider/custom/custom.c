// SPDX-License-Identifier: MIT
// Copyright 2022 NXP

/**
 * @file custom.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "custom.h"
#include "lvgl/lvgl.h"
#include <stdio.h>
#if defined(RT_PLATFORM) & LVGL_MULTITHREAD_LOCK
#include "FreeRTOS.h"
#include "semphr.h"
#endif

/*********************
 *      DEFINES
 *********************/
#define AUDIO_IMG_ROTATION_CYCLE     10000
#define AUDIO_TIME_REFRESH_PERIOD_1S 1000
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
#if defined(RT_PLATFORM) & LVGL_MULTITHREAD_LOCK
/* LVGL should be single thread. Get this mutex for multithread access */
static SemaphoreHandle_t s_LVGLMutex;
#endif /*RT_PLATFORM*/

#ifdef RT_PLATFORM
#include "lvgl_images_internal.h"

void setup_imgs(unsigned char *base)
{
    _audio_player_background_1280x720.data      = (base + 0);
    _audio_player_mic_off_70x112.data           = (base + 1843200);
    _audio_player_next_140x214.data             = (base + 1858880);
    _audio_player_pause_170x190.data            = (base + 1918848);
    _audio_player_play_170x190.data             = (base + 1983488);
    _audio_player_previous_140x214.data         = (base + 2048128);
    _audio_player_volume_down_144x130.data      = (base + 2108096);
    _audio_player_volume_up_144x130.data        = (base + 2145536);
    _auido_player_mic_on_70x112.data            = (base + 2182976);
    _cancel_160x81.data                         = (base + 2198656);
    _confirm_160x81.data                        = (base + 2224576);
    _face_blue_96x93.data                       = (base + 2250496);
    _face_green_96x93.data                      = (base + 2268352);
    _face_red_96x93.data                        = (base + 2286208);
    _face_reg_128x128.data                      = (base + 2304064);
    _help_100x100.data                          = (base + 2336832);
    _home_85x70.data                            = (base + 2356864);
    _main_audio_player_160x160.data             = (base + 2368768);
    _main_background_1280x630.data              = (base + 2419968);
    _main_background_bottom_1280x90.data        = (base + 4032768);
    _main_mic_off_70x112.data                   = (base + 4263168);
    _main_mic_on_70x112.data                    = (base + 4278848);
    _main_security_160x160.data                 = (base + 4294528);
    _main_temp_humidity_34x128.data             = (base + 4345728);
    _main_thermostat_160x160.data               = (base + 4354432);
    _other_background_bottom_1280x90.data       = (base + 4405632);
    _security_arm_270x260.data                  = (base + 4636032);
    _security_armed_278x222.data                = (base + 4776448);
    _security_back_door_locked_212x112.data     = (base + 4961600);
    _security_back_door_unlocked_212x112.data   = (base + 5009088);
    _security_background_bottom_1280x90.data    = (base + 5056576);
    _security_disarmed_278x222.data             = (base + 5286976);
    _security_front_door_locked_212x112.data    = (base + 5410432);
    _security_front_door_unlocked_212x112.data  = (base + 5457920);
    _security_garage_door_locked_212x112.data   = (base + 5505408);
    _security_garage_door_unlocked_212x112.data = (base + 5552896);
    _security_mic_off_70x112.data               = (base + 5600384);
    _security_mic_on_70x112.data                = (base + 5616064);
    _thermostat_add_90x90.data                  = (base + 5631744);
    _thermostat_background_bottom_1280x90.data  = (base + 5648000);
    _thermostat_cancel_160x80.data              = (base + 5878400);
    _thermostat_delete_90x90.data               = (base + 5904000);
    _thermostat_edit_90x90.data                 = (base + 5920256);
    _thermostat_fan_auto_off_102x92.data        = (base + 5936512);
    _thermostat_fan_auto_on_102x92.data         = (base + 5955328);
    _thermostat_fan_high_off_102x84.data        = (base + 5974144);
    _thermostat_fan_high_on_102x84.data         = (base + 5991296);
    _thermostat_fan_low_off_102x84.data         = (base + 6008448);
    _thermostat_fan_low_on_102x84.data          = (base + 6025600);
    _thermostat_fan_mid_off_102x84.data         = (base + 6042752);
    _thermostat_fan_mid_on_102x84.data          = (base + 6059904);
    _thermostat_fan_off_102x92.data             = (base + 6077056);
    _thermostat_fan_on_102x92.data              = (base + 6095872);
    _thermostat_frame_312x310.data              = (base + 6114688);
    _thermostat_mic_off_50x80.data              = (base + 6308160);
    _thermostat_mic_on_50x80.data               = (base + 6316160);
    _thermostat_mode_auto_off_102x84.data       = (base + 6324160);
    _thermostat_mode_auto_on_102x84.data        = (base + 6341312);
    _thermostat_mode_cool_off_102x92.data       = (base + 6358464);
    _thermostat_mode_cool_on_102x92.data        = (base + 6377280);
    _thermostat_mode_heat_off_102x92.data       = (base + 6396096);
    _thermostat_mode_heat_on_102x92.data        = (base + 6414912);
    _thermostat_off_128x128.data                = (base + 6433728);
    _thermostat_ok_160x80.data                  = (base + 6466496);
    _thermostat_on_128x128.data                 = (base + 6492096);
    _thermostat_save_160x80.data                = (base + 6524864);
    _user_setting_128x128.data                  = (base + 6550464);
}

#else
/* LVGL-Simulator */
void UI_EnterScreen_Callback(home_panel_screen_id_t screenId)
{
    switch (screenId)
    {
        case kScreen_Home:
        {
            gui_enable_mic(kScreen_Home, false);
        }
        break;
        case kScreen_Thermostat:
        {
        }
        break;
        case kScreen_Security:
        {
        }
        break;
        case kScreen_AudioPlayer:
        {
        }
        break;
        default:
            break;
    }
}

language_t UI_GetLanguage(void)
{
    return kLanguage_EN;
}
#endif /* RT_PLATFORM */

/**
 * @brief get next screen
 *
 */
home_panel_screen_id_t get_next_screen(void)
{
    lv_disp_t *pDefaultScreen           = lv_disp_get_default();
    home_panel_screen_id_t nextScreenId = kScreen_Num;

    if (pDefaultScreen->scr_to_load == NULL)
    {
        nextScreenId = kScreen_Num;
    }
    else if (pDefaultScreen->scr_to_load == guider_ui.screen_audio_player)
    {
        nextScreenId = kScreen_AudioPlayer;
    }

    return nextScreenId;
}

/**
 * @brief get current screen
 *
 */
home_panel_screen_id_t get_current_screen(void)
{
    lv_obj_t *pCurrentScreen               = lv_scr_act();
    home_panel_screen_id_t currentScreenId = kScreen_Num;

    if (pCurrentScreen == NULL)
    {
        currentScreenId = kScreen_Num;
    }
    else if (pCurrentScreen == guider_ui.screen_home)
    {
        currentScreenId = kScreen_Home;
    }
    else if (pCurrentScreen == guider_ui.screen_audio_player)
    {
        currentScreenId = kScreen_AudioPlayer;
    }
    else if (pCurrentScreen == guider_ui.screen_security)
    {
        currentScreenId = kScreen_Security;
    }
    else if (pCurrentScreen == guider_ui.screen_thermostat)
    {
        currentScreenId = kScreen_Thermostat;
    }
    else if (pCurrentScreen == guider_ui.screen_security_face_reg)
    {
        currentScreenId = kScreen_Security_FaceReg;
    }
    else if (pCurrentScreen == guider_ui.screen_thermostat_face_reg)
    {
        currentScreenId = kScreen_Thermostat_FaceReg;
    }

    return currentScreenId;
}

void gui_enable_mic(home_panel_screen_id_t screenId, bool enable)
{
    lv_obj_t *img_mic_on  = NULL;
    lv_obj_t *img_mic_off = NULL;

    switch (screenId)
    {
        case (kScreen_Home):
        {
            img_mic_on  = guider_ui.screen_home_img_mic_on;
            img_mic_off = guider_ui.screen_home_img_mic_off;
        }
        break;
        case (kScreen_Thermostat):
        {
            img_mic_on  = guider_ui.screen_thermostat_img_mic_on;
            img_mic_off = guider_ui.screen_thermostat_img_mic_off;
        }
        break;
        case (kScreen_Security):
        {
            img_mic_on  = guider_ui.screen_security_img_mic_on;
            img_mic_off = guider_ui.screen_security_img_mic_off;
        }
        break;
        case (kScreen_AudioPlayer):
        {
            img_mic_on  = guider_ui.screen_audio_player_img_mic_on;
            img_mic_off = guider_ui.screen_audio_player_img_mic_off;
        }
        break;
        default:
            break;
    }

    if ((img_mic_on == NULL) || (img_mic_off == NULL))
    {
        return;
    }

    if (enable)
    {
        lv_obj_add_flag(img_mic_off, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(img_mic_on, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(img_mic_on, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(img_mic_off, LV_OBJ_FLAG_HIDDEN);
    }
}

bool *get_screen_del(int scr_id)
{
    bool *scr_del = NULL;
    switch (scr_id)
    {
        case kScreen_Home:
        {
            scr_del = &guider_ui.screen_home_del;
        }
        break;

        case kScreen_Thermostat:
        {
            scr_del = &guider_ui.screen_thermostat_del;
        }
        break;

        case kScreen_Security:
        {
            scr_del = &guider_ui.screen_security_del;
        }
        break;

        case kScreen_AudioPlayer:
        {
            scr_del = &guider_ui.screen_audio_player_del;
        }
        break;

        case kScreen_Security_FaceReg:
        {
            scr_del = &guider_ui.screen_security_face_reg_del;
        }
        break;

        case kScreen_Thermostat_FaceReg:
        {
            scr_del = &guider_ui.screen_thermostat_face_reg_del;
            // scr_del = &guider_ui.screen_thermostat_del;
        }
        break;
        case kScreen_Thermostat_UserList:
		{
			scr_del = &guider_ui.screen_thermostat_user_list_del;
		}
		break;
    }

    return scr_del;
}


void gui_lvgl_enable_buttons(lv_obj_t *btn_obj, bool en)
{
	if(en == true)
	{
		lv_obj_clear_state(btn_obj, LV_STATE_CHECKED);
		lv_obj_add_flag(btn_obj, LV_OBJ_FLAG_CLICKABLE);
	}
	else
	{
		lv_obj_add_state(btn_obj, LV_STATE_CHECKED);
		lv_obj_clear_flag(btn_obj, LV_OBJ_FLAG_CLICKABLE);
	}
}

/**
 * @brief Load the screen specified by `dst_scr_id` after `delay` milliseconds from `src_scr_id` screen
 *
 * @param src_scr_id the id of the source screen
 * @param dst_scr_id the id of the destination screen to load
 * @param delay milliseconds to delay
 */
void gui_load_screen(int src_scr_id, int dst_scr_id, int delay)
{
    lv_obj_t **scr_to_load = NULL;
    lv_obj_t *act_scr      = lv_scr_act();
    lv_disp_t *d           = lv_obj_get_disp(act_scr);

    if (src_scr_id != dst_scr_id)
    {
        UI_ExitScreen_Callback(src_scr_id);
    }

    /* Prepare and load the new screen */
    UI_EnterScreen_Callback(dst_scr_id);

    bool *scr_del = get_screen_del(src_scr_id);

    LVGL_LOG("LOAD_SCREEN: %d ---> %d\r\n", src_scr_id, dst_scr_id);

    switch (dst_scr_id)
    {
        case kScreen_Home:
        {
            if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
            {
                scr_to_load = &guider_ui.screen_home;
                if (guider_ui.screen_home_del == true)
                {
                    setup_scr_screen_home(&guider_ui);
                    gui_enable_mic(kScreen_Thermostat, false);
                }
                language_t uiLanguate = UI_GetLanguage();
                if ((uiLanguate >= kLanguage_EN) && (uiLanguate < kLanguage_Ids))
                {
                    lv_dropdown_set_selected(guider_ui.screen_home_ddlist_language, uiLanguate);
                }
            }
        }
        break;

        case kScreen_Thermostat:
        {
            if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
            {
                scr_to_load = &guider_ui.screen_thermostat;
                if (guider_ui.screen_thermostat_del == true)
                {
                    setup_scr_screen_thermostat(&guider_ui);
                    gui_enable_mic(kScreen_Thermostat, false);
                }
                language_t uiLanguate = UI_GetLanguage();
                if ((uiLanguate >= kLanguage_EN) && (uiLanguate < kLanguage_Ids))
                {
                    lv_dropdown_set_selected(guider_ui.screen_thermostat_ddlist_language, uiLanguate);
                }

                gui_thermostat_init_state();
            }
        }
        break;

        case kScreen_Thermostat_FaceReg:
        {
            if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
            {
                scr_to_load = &guider_ui.screen_thermostat_face_reg;
                if (guider_ui.screen_thermostat_face_reg_del == true)
                {
                    setup_scr_screen_thermostat_face_reg(&guider_ui);
                }
            }
        }
        break;

        case kScreen_Thermostat_UserList:
		{
			if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
			{
				scr_to_load = &guider_ui.screen_thermostat_user_list;
				if (guider_ui.screen_thermostat_user_list_del == true)
				{
					setup_scr_screen_thermostat_user_list(&guider_ui);
					gui_thermostat_lvgl_setup_user_items();
					gui_thermostat_events_init_user_misc_widgets(&guider_ui);

					uint8_t item_num = 0;
					//TODO: get user management info. from DB and set the item_num with user number

					gui_thermostat_set_user_item_num(item_num);
				}
				gui_thermostat_lvgl_hide_user_del_ui(true);

				if (src_scr_id == kScreen_Thermostat_UserEdit)
				{
					gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_delete, true);
					gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_edit, true);
					gui_thermostat_lvgl_focus_user_item(true);
				}
				else
				{
					gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_delete, false);
					gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_edit, false);
					uint8_t sel_item_no = gui_thermostat_get_user_item_no();
					if (sel_item_no < gui_thermostat_get_user_item_num())
					{
						gui_thermostat_lvgl_focus_user_item(false);
					}
				}
			}
		}
		break;

        case kScreen_Thermostat_UserEdit:
		{
			if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
			{
				scr_to_load = &guider_ui.screen_thermostat_user_edit;
				if (guider_ui.screen_thermostat_user_edit_del == true)
				{
					setup_scr_screen_thermostat_user_edit(&guider_ui);
				}
				gui_thermostat_lvgl_refresh_user_editing_item();
				gui_thermostat_lvgl_update_kb_style();
				gui_thermostat_lvgl_focus_user_name();
				gui_thermostat_lvgl_update_cursor_color(lv_color_make(0xff, 0xff, 0xff));
			}
		}
		break;

        case kScreen_Security:
        {
            if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
            {
                scr_to_load = &guider_ui.screen_security;
                if (guider_ui.screen_security_del == true)
                {
                    setup_scr_screen_security(&guider_ui);
                    gui_enable_mic(kScreen_Security, false);
                }
                language_t uiLanguate = UI_GetLanguage();
                if ((uiLanguate >= kLanguage_EN) && (uiLanguate < kLanguage_Ids))
                {
                    lv_dropdown_set_selected(guider_ui.screen_security_ddlist_language, uiLanguate);
                }
                security_state door_sate = gui_security_get_door_state(kSecurity_Front);
                gui_security_set_door_state(kSecurity_Front, door_sate);
                door_sate = gui_security_get_door_state(kSecurity_Back);
                gui_security_set_door_state(kSecurity_Back, door_sate);
                door_sate = gui_security_get_door_state(kSecurity_Garage);
                gui_security_set_door_state(kSecurity_Garage, door_sate);
            }
        }
        break;

        case kScreen_Security_FaceReg:
        {
            if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
            {
                scr_to_load = &guider_ui.screen_security_face_reg;
                if (guider_ui.screen_security_face_reg_del == true)
                {
                    setup_scr_screen_security_face_reg(&guider_ui);
                }
            }
        }
        break;

        case kScreen_AudioPlayer:
        {
            if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
            {
                scr_to_load = &guider_ui.screen_audio_player;
                if (guider_ui.screen_audio_player_del == true)
                {
                    setup_scr_screen_audio_player(&guider_ui);
                    gui_enable_mic(kScreen_AudioPlayer, false);
                    refresh_audio_player_state();
                    refresh_audio_player_info();
                    refresh_audio_player_volume();
                }
                language_t uiLanguate = UI_GetLanguage();
                if ((uiLanguate >= kLanguage_EN) && (uiLanguate < kLanguage_Ids))
                {
                    lv_dropdown_set_selected(guider_ui.screen_audio_player_ddlist_language, uiLanguate);
                }
                if (guider_ui.screen_audio_player_del == false)
                {
                    set_audio_player_state(kAudioState_Pause);
                    refresh_audio_player_info();
                    refresh_audio_player_state();
                    refresh_audio_player_volume();
                }
                UI_AudioPlaying_Callback(kAudioPlayer_Load);
            }
        }
        break;
    }

    if (scr_to_load)
    {
        lv_scr_load_anim(*scr_to_load, LV_SCR_LOAD_ANIM_NONE, 0, delay, false);
    }

    if (scr_del)
    {
        *scr_del = false;
    }
}

void gui_home_language_changed_cb(void)
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.screen_home_ddlist_language);
    language_t new_language = (language_t)opt_id;
    if ((new_language >= kLanguage_EN) && (new_language < kLanguage_Ids))
    {
        UI_SetLanguage_Callback(new_language);
    }
}

void gui_thermostat_language_changed_cb(void)
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.screen_thermostat_ddlist_language);
    language_t new_language = (language_t)opt_id;
    if ((new_language >= kLanguage_EN) && (new_language < kLanguage_Ids))
    {
        UI_SetLanguage_Callback(new_language);
    }
}

void gui_security_language_changed_cb(void)
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.screen_security_ddlist_language);
    language_t new_language = (language_t)opt_id;
    if ((new_language >= kLanguage_EN) && (new_language < kLanguage_Ids))
    {
        UI_SetLanguage_Callback(new_language);
    }
}

#if defined(RT_PLATFORM) & LVGL_MULTITHREAD_LOCK
void _takeLVGLMutex()
{
    if (s_LVGLMutex)
    {
        xSemaphoreTake(s_LVGLMutex, portMAX_DELAY);
    }
}

void _giveLVGLMutex()
{
    if (s_LVGLMutex)
    {
        xSemaphoreGive(s_LVGLMutex);
    }
}
#endif /* LVGL_MULTITHREAD_LOCK */

/**
 * Initializes demo application
 */

void custom_init(lv_ui *ui)
{
#if defined(RT_PLATFORM) & LVGL_MULTITHREAD_LOCK
    s_LVGLMutex = xSemaphoreCreateMutex();

    if (s_LVGLMutex == NULL)
    {
        while (1)
        {
        }
    }
#endif
    /* Add your code here */
    UI_EnterScreen_Callback(kScreen_Home);
}
