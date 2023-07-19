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
    _back_85x70.data                            = (base + 2198656);
    _button_green_alpha_210x110.data            = (base + 2210560);
    _button_red_alpha_210x110.data              = (base + 2279872);
    _face_blue_96x93.data                       = (base + 2349184);
    _face_green_96x93.data                      = (base + 2367040);
    _face_red_96x93.data                        = (base + 2384896);
    _help_100x100.data                          = (base + 2402752);
    _home_85x70.data                            = (base + 2422784);
    _main_audio_player_160x160.data             = (base + 2434688);
    _main_background_1280x630.data              = (base + 2485888);
    _main_background_bottom_1280x90.data        = (base + 4098688);
    _main_mic_off_70x112.data                   = (base + 4329088);
    _main_mic_on_70x112.data                    = (base + 4344768);
    _main_security_160x160.data                 = (base + 4360448);
    _main_temp_humidity_34x128.data             = (base + 4411648);
    _main_thermostat_160x160.data               = (base + 4420352);
    _other_background_bottom_1280x90.data       = (base + 4471552);
    _security_arm_270x260.data                  = (base + 4701952);
    _security_armed_324x278.data                = (base + 4842368);
    _security_back_door_locked_212x112.data     = (base + 5022528);
    _security_back_door_unlocked_212x112.data   = (base + 5070016);
    _security_background_bottom_1280x90.data    = (base + 5117504);
    _security_disarmed_324x278.data             = (base + 5347904);
    _security_facereg_keypad_frame_575x490.data = (base + 5528064);
    _security_front_door_locked_212x112.data    = (base + 6091584);
    _security_front_door_unlocked_212x112.data  = (base + 6139072);
    _security_garage_door_locked_212x112.data   = (base + 6186560);
    _security_garage_door_unlocked_212x112.data = (base + 6234048);
    _security_keypad_button_alpha_118x75.data   = (base + 6281536);
    _security_keypad_frame_601x536.data         = (base + 6308096);
    _security_pin_121x121.data                  = (base + 6952384);
    _security_timer_green_301x302.data          = (base + 6981696);
    _security_timer_red_301x302.data            = (base + 7163520);
    _security_user_setting_128x128.data         = (base + 7345344);
    _thermostat_add_121x121.data                = (base + 7378112);
    _thermostat_background_bottom_1280x90.data  = (base + 7407424);
    _thermostat_delete_121x121.data             = (base + 7637824);
    _thermostat_edit_121x121.data               = (base + 7667136);
    _thermostat_fan_auto_off_102x92.data        = (base + 7696448);
    _thermostat_fan_auto_on_102x92.data         = (base + 7715264);
    _thermostat_fan_high_off_102x84.data        = (base + 7734080);
    _thermostat_fan_high_on_102x84.data         = (base + 7751232);
    _thermostat_fan_low_off_102x84.data         = (base + 7768384);
    _thermostat_fan_low_on_102x84.data          = (base + 7785536);
    _thermostat_fan_mid_off_102x84.data         = (base + 7802688);
    _thermostat_fan_mid_on_102x84.data          = (base + 7819840);
    _thermostat_fan_off_102x92.data             = (base + 7836992);
    _thermostat_fan_on_102x92.data              = (base + 7855808);
    _thermostat_frame_312x310.data              = (base + 7874624);
    _thermostat_mic_off_50x80.data              = (base + 8068096);
    _thermostat_mic_on_50x80.data               = (base + 8076096);
    _thermostat_mode_auto_off_102x84.data       = (base + 8084096);
    _thermostat_mode_auto_on_102x84.data        = (base + 8101248);
    _thermostat_mode_cool_off_102x92.data       = (base + 8118400);
    _thermostat_mode_cool_on_102x92.data        = (base + 8137216);
    _thermostat_mode_heat_off_102x92.data       = (base + 8156032);
    _thermostat_mode_heat_on_102x92.data        = (base + 8174848);
    _thermostat_off_128x128.data                = (base + 8193664);
    _thermostat_on_128x128.data                 = (base + 8226432);
    _user_setting_128x128.data                  = (base + 8259200);
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

        case kScreen_Security_Pincode:
        {
            scr_del = &guider_ui.screen_security_pincode_del;
        }
        break;

        case kScreen_Security_Arming:
        {
            scr_del = &guider_ui.screen_security_arming_del;
        }
        break;

        case kScreen_Security_UserList:
        {
            scr_del = &guider_ui.screen_security_user_list_del;
        }
        break;

        case kScreen_Security_UserEdit:
        {
            scr_del = &guider_ui.screen_security_user_edit_del;
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

        case kScreen_Thermostat_UserEdit:
        {
            scr_del = &guider_ui.screen_thermostat_user_edit_del;
        }
        break;

        case kScreen_Help_Home:
        {
            scr_del = &guider_ui.screen_help_home_del;
        }
        break;

        case kScreen_Help_Thermostat:
        {
            scr_del = &guider_ui.screen_help_thermostat_del;
        }
        break;

        case kScreen_Help_Security:
        {
            scr_del = &guider_ui.screen_help_security_del;
        }
        break;

        case kScreen_Help_AudioPlayer:
        {
            scr_del = &guider_ui.screen_help_audioplayer_del;
        }
        break;
    }

    return scr_del;
}

void gui_lvgl_enable_buttons(lv_obj_t *btn_obj, bool en)
{
    if (en == true)
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

void gui_lvgl_popup_msgbox_user_name_err(const char *title, const char *content)
{
	lv_obj_t *obj = lv_msgbox_create(lv_scr_act(), title, content, NULL, true);
	lv_obj_set_pos(obj, 750, 255);
	lv_obj_set_size(obj, 500, 100);
	lv_obj_set_ext_click_area(obj, 1300);

	lv_msgbox_t * mbox = (lv_msgbox_t *)obj;
	lv_obj_set_size(mbox->close_btn, 38, 38);

	static lv_style_t style_title_default;
	if (style_title_default.prop_cnt > 1)
		lv_style_reset(&style_title_default);
	else
		lv_style_init(&style_title_default);
	lv_style_set_text_font(&style_title_default, &lv_font_simsun_30);
	lv_obj_add_style(lv_msgbox_get_title(obj), &style_title_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	static lv_style_t style_content_default;
	if (style_content_default.prop_cnt > 1)
		lv_style_reset(&style_content_default);
	else
		lv_style_init(&style_content_default);
	lv_style_set_text_font(&style_content_default, &lv_font_simsun_25);
	lv_obj_add_style(lv_msgbox_get_text(obj), &style_content_default, LV_PART_MAIN|LV_STATE_DEFAULT);
}


user_name_err_num gui_lvgl_check_user_name(lv_obj_t *name_obj)
{
	const char *user_name = lv_textarea_get_text(name_obj);

	if (user_name[0] == 0)
	{
		return USER_NAME_ERR_EMPTY;
	}
	else if (strchr(user_name, '\n'))
	{
		return USER_NAME_ERR_ENTER;
	}
	else
	{
		lv_obj_t *current_ui = lv_scr_act();
		if (current_ui == guider_ui.screen_thermostat_user_edit || current_ui == guider_ui.screen_thermostat_face_reg)
		{
			uint8_t cur_user_no = gui_thermostat_get_user_item_no();
			uint8_t user_num = gui_thermostat_get_user_item_num();
			for (uint8_t i = 0; i < user_num; i++)
			{
				if(i != cur_user_no)
				{
					const thermostat_user_list_obj_t *user_info = gui_thermostat_get_user_info(i);
					if (strcmp(user_name, user_info->user_name) == 0)
					{
						return USER_NAME_ERR_REPEATED;
					}
				}
			}
		}
		else if(current_ui == guider_ui.screen_security_user_edit)
		{
			uint8_t cur_user_no = gui_security_get_selected_user_no();
			uint8_t user_num = gui_security_get_max_user_num();
			for (int i = 0; i < user_num; i++)
			{
				if(i != cur_user_no)
				{
					const char *existed_user_name = gui_security_get_user_name(i);
					if (strcmp(user_name, existed_user_name) == 0)
					{
						return USER_NAME_ERR_REPEATED;
					}
				}
			}
		}
	}
    return USER_NAME_ERR_NO;
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
                language_t uiLanguage = UI_GetLanguage();
                gui_home_init_state(uiLanguage);
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
                language_t uiLanguage = UI_GetLanguage();

                gui_thermostat_init_state(uiLanguage);
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

                language_t uiLanguage = UI_GetLanguage();
                gui_thermostat_face_reg_init_state(uiLanguage);
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
                }

                language_t uiLanguage = UI_GetLanguage();
                gui_thermostat_userlist_init_state(uiLanguage);
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
                language_t uiLanguage = UI_GetLanguage();
                gui_thermostat_useredit_init(uiLanguage);
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
                language_t uiLanguage = UI_GetLanguage();

                gui_security_init_state(uiLanguage);
            }
        }
        break;

        case kScreen_Security_Pincode:
        {
            if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
            {
                scr_to_load = &guider_ui.screen_security_pincode;
                if (guider_ui.screen_security_pincode_del == true)
                {
                    setup_scr_screen_security_pincode(&guider_ui);
                }
            }
            language_t uiLanguage = UI_GetLanguage();
            gui_security_pincode_init_state(uiLanguage);
        }
        break;

        case kScreen_Security_Arming:
        {
            if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
            {
                scr_to_load = &guider_ui.screen_security_arming;
                if (guider_ui.screen_security_arming_del == true)
                {
                    setup_scr_screen_security_arming(&guider_ui);
                }
            }
            language_t uiLanguage = UI_GetLanguage();
            gui_security_arming_init_state(uiLanguage);
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
                language_t uiLanguage = UI_GetLanguage();
                gui_security_face_reg_init_state(uiLanguage);
            }
        }
        break;

        case kScreen_Security_UserList:
        {
            if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
            {
                scr_to_load = &guider_ui.screen_security_user_list;
                if (guider_ui.screen_security_user_list_del == true)
                {
                    setup_scr_screen_security_user_list(&guider_ui);
                    gui_security_userlist_ui_init();
                }
                language_t uiLanguage = UI_GetLanguage();
                gui_security_userlist_init(uiLanguage);
            }
        }
        break;

        case kScreen_Security_UserEdit:
        {
            if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
            {
                scr_to_load = &guider_ui.screen_security_user_edit;
                if (guider_ui.screen_security_user_edit_del == true)
                {
                    setup_scr_screen_security_user_edit(&guider_ui);
                }
                language_t uiLanguage = UI_GetLanguage();
                gui_security_useredit_init(uiLanguage);
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
                    lv_obj_clear_flag(guider_ui.screen_audio_player_arc_progress, LV_OBJ_FLAG_CLICKABLE);
                }

                language_t uiLanguage = UI_GetLanguage();

                gui_audio_player_init_state(uiLanguage);
                UI_AudioPlaying_Callback(kAudioPlayer_Load);
            }
        }
        break;

        case kScreen_Help_Home:
        {
            if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
            {
                scr_to_load = &guider_ui.screen_help_home;
                if (guider_ui.screen_help_home_del == true)
                {
                    setup_scr_screen_help_home(&guider_ui);
                }
                gui_help_home_init_state();
            }
        }
        break;

        case kScreen_Help_Thermostat:
        {
            if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
            {
                scr_to_load = &guider_ui.screen_help_thermostat;
                if (guider_ui.screen_help_thermostat_del == true)
                {
                    setup_scr_screen_help_thermostat(&guider_ui);
                }
                gui_help_thermostat_init_state();
            }
        }
        break;

        case kScreen_Help_Security:
        {
            if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
            {
                scr_to_load = &guider_ui.screen_help_security;
                if (guider_ui.screen_help_security_del == true)
                {
                    setup_scr_screen_help_security(&guider_ui);
                }
                gui_help_security_init_state();
            }
        }
        break;

        case kScreen_Help_AudioPlayer:
        {
            if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
            {
                scr_to_load = &guider_ui.screen_help_audioplayer;
                if (guider_ui.screen_help_audioplayer_del == true)
                {
                    setup_scr_screen_help_audioplayer(&guider_ui);
                }
                gui_help_audioplayer_init_state();
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

uint8_t gui_language_change_cb(language_t new_language)
{
    if ((new_language >= kLanguage_EN) && (new_language < kLanguage_Ids))
    {
        UI_SetLanguage_Callback(new_language);
        return 0;
    }
    else
    {
        return 1;
    }
}

void gui_home_language_changed_cb(void)
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.screen_home_ddlist_language);
    language_t new_language = (language_t)opt_id;
    if (gui_language_change_cb(new_language) == 0)
    {
        gui_home_set_language_text(new_language);
    }
}

void gui_thermostat_language_changed_cb(void)
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.screen_thermostat_ddlist_language);
    language_t new_language = (language_t)opt_id;
    if (gui_language_change_cb(new_language) == 0)
    {
        gui_thermostat_set_language_text(new_language);
    }
}

void gui_security_language_changed_cb(void)
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.screen_security_ddlist_language);
    language_t new_language = (language_t)opt_id;
    if (gui_language_change_cb(new_language) == 0)
    {
        gui_security_set_language_text(new_language);
    }
}

void gui_security_userlist_language_changed_cb(void)
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.screen_security_user_list_ddlist_language);
    language_t new_language = (language_t)opt_id;
    if (gui_language_change_cb(new_language) == 0)
    {
        gui_security_userlist_set_language_text(new_language);
    }
}

void gui_thermostat_user_list_language_changed_cb(void)
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.screen_thermostat_user_list_ddlist_language);
    language_t new_language = (language_t)opt_id;
    if (gui_language_change_cb(new_language) == 0)
    {
        gui_thermostat_userlist_set_language_text(new_language);
    }
}

void gui_thermostat_user_edit_language_changed_cb(void)
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.screen_thermostat_user_edit_ddlist_language);
    language_t new_language = (language_t)opt_id;
    if (gui_language_change_cb(new_language) == 0)
    {
        gui_thermostat_useredit_set_language_text(new_language);
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
