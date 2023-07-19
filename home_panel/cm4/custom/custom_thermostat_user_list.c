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
#include "ui_strings_internal.h"

#define MAX_USER_ITEM_STR_SIZE 55//69
#define TEMPERATURE_STR_LOC	   27//29
#define MODE_STR_LOC		   38//46
#define FAN_SPEED_STR_LOC	   48//63

#define INVALID_USER_ITEM_NO 0xff

static thermostat_user_list_obj_t s_UserInfo[MAX_THERMOSTAT_USER_NUM];
static lv_obj_t *s_UserItemObj[MAX_THERMOSTAT_USER_NUM] = {NULL};
static uint8_t s_UserItemSelectedNo;
static uint8_t s_UserItemNum;

const char *thermostat_mode_string(language_t lang, thermostat_mode mode)
{
    return s_ModeStr[lang][mode];
}

const char *thermostat_fanspeed_string(language_t lang, thermostat_fan_speed speed)
{
    return s_FanSpeedStr[lang][speed];
}

void gui_thermostat_lvgl_hide_user_editor(bool hiding)
{
    if (hiding == true)
    {
    	lv_obj_add_flag(guider_ui.screen_thermostat_user_list_imgbtn_delete, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_thermostat_user_list_imgbtn_edit, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_thermostat_user_list_imgbtn_add, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
    	lv_obj_clear_flag(guider_ui.screen_thermostat_user_list_imgbtn_delete, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(guider_ui.screen_thermostat_user_list_imgbtn_edit, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(guider_ui.screen_thermostat_user_list_imgbtn_add, LV_OBJ_FLAG_HIDDEN);
    }
}

void gui_thermostat_lvgl_hide_user_del_ui(bool hiding)
{
    if (hiding == true)
    {
    	lv_obj_add_flag(guider_ui.screen_thermostat_user_list_label_delete, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_thermostat_user_list_imgbtn_del_ok, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_thermostat_user_list_imgbtn_del_cancel, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
    	lv_obj_clear_flag(guider_ui.screen_thermostat_user_list_label_delete, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(guider_ui.screen_thermostat_user_list_imgbtn_del_ok, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(guider_ui.screen_thermostat_user_list_imgbtn_del_cancel, LV_OBJ_FLAG_HIDDEN);
    }
}

void gui_thermostat_lvgl_focus_user_name(void)
{
	lv_event_send(guider_ui.screen_thermostat_user_edit_ta_user_name, LV_EVENT_FOCUSED, NULL);
}

void gui_thermostat_lvgl_focus_user_item(bool focusing)
{
	if (focusing == true)
	{
		lv_obj_add_state(s_UserItemObj[s_UserItemSelectedNo], LV_STATE_FOCUSED);
	}
	else
	{
		lv_obj_clear_state(s_UserItemObj[s_UserItemSelectedNo], LV_STATE_FOCUSED);
	}
}

void gui_thermostat_lvgl_update_kb_style(void)
{
	lv_obj_t *kb = lv_obj_get_child(guider_ui.screen_thermostat_user_edit, 0);
	lv_obj_set_style_text_font(kb, &lv_font_montserratMedium_25, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(kb, LV_OPA_COVER, LV_PART_MAIN|LV_STATE_DEFAULT);
}

void gui_thermostat_lvgl_update_cursor_color(lv_color_t color)
{
	lv_obj_set_style_border_color(guider_ui.screen_thermostat_user_edit_ta_user_name, color, LV_PART_CURSOR | LV_STATE_FOCUSED);
}

static void _user_misc_widgets_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_RELEASED:
	{
		gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_delete, false);
		gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_edit, false);

		if(s_UserItemSelectedNo != INVALID_USER_ITEM_NO)
		{
			gui_thermostat_lvgl_focus_user_item(false);
		}
		s_UserItemSelectedNo = INVALID_USER_ITEM_NO;
	}
		break;
	default:
		break;
	}
}

void gui_thermostat_events_init_user_misc_widgets(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_thermostat_user_list_img_background, _user_misc_widgets_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_list_label_title, _user_misc_widgets_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_list_label_user_list, _user_misc_widgets_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_thermostat_user_list_list_user_info, _user_misc_widgets_event_handler, LV_EVENT_ALL, ui);
}

static uint8_t _find_user_item_no(lv_obj_t *obj)
{
	uint8_t item_no = INVALID_USER_ITEM_NO;

	for (uint8_t i = 0; i < s_UserItemNum; i++)
	{
		if (obj == s_UserItemObj[i])
		{
			item_no = i;
			break;
		}
	}
	return item_no;
}

static void _user_items_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *item_obj = lv_event_get_target(e);
	switch (code)
	{
		case LV_EVENT_RELEASED:
		{
			gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_delete, true);
			gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_edit, true);
			uint8_t item_no = _find_user_item_no(item_obj);
			if (item_no == INVALID_USER_ITEM_NO)
			{
				break;
			}
			if (s_UserItemSelectedNo != item_no && s_UserItemSelectedNo != INVALID_USER_ITEM_NO)
			{
				gui_thermostat_lvgl_focus_user_item(false);
			}
			s_UserItemSelectedNo = item_no;
		}
			break;
		default:
			break;
	}
}

static void _events_init_user_items(uint8_t item_no)
{
	lv_obj_add_event_cb(s_UserItemObj[item_no], _user_items_event_handler, LV_EVENT_ALL, s_UserItemObj[item_no]);
}

void gui_thermostat_lvgl_setup_user_items(void)
{
	static lv_style_t style_user_item_main_default;
	if (style_user_item_main_default.prop_cnt > 1)
		lv_style_reset(&style_user_item_main_default);
	else
		lv_style_init(&style_user_item_main_default);
	lv_style_set_radius(&style_user_item_main_default, 3);
	lv_style_set_bg_color(&style_user_item_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_user_item_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_user_item_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_user_item_main_default, 0);
	lv_style_set_text_color(&style_user_item_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_user_item_main_default, &lv_font_simsun_40);

	static lv_style_t style_user_item_main_pressed;
	if (style_user_item_main_default.prop_cnt > 1)
		lv_style_reset(&style_user_item_main_pressed);
	else
		lv_style_init(&style_user_item_main_pressed);
	lv_style_set_radius(&style_user_item_main_pressed, 3);
	lv_style_set_bg_color(&style_user_item_main_pressed, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_user_item_main_pressed, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_user_item_main_pressed, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_user_item_main_pressed, 0);
	lv_style_set_text_color(&style_user_item_main_pressed, lv_color_make(0x2F, 0xDA, 0x64));
	lv_style_set_text_font(&style_user_item_main_pressed, &lv_font_simsun_40);

	static lv_style_t style_user_item_main_focused;
	if (style_user_item_main_default.prop_cnt > 1)
		lv_style_reset(&style_user_item_main_focused);
	else
		lv_style_init(&style_user_item_main_focused);
	lv_style_set_radius(&style_user_item_main_focused, 3);
	lv_style_set_bg_color(&style_user_item_main_focused, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_user_item_main_focused, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_user_item_main_focused, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_user_item_main_focused, 0);
	lv_style_set_text_color(&style_user_item_main_focused, lv_color_make(0x00, 0xff, 0x00));
	lv_style_set_text_font(&style_user_item_main_focused, &lv_font_simsun_40);

	s_UserItemObj[0] = guider_ui.screen_thermostat_user_list_list_user_info_item0;
	lv_obj_t *img = lv_obj_get_child(s_UserItemObj[0], 0);
	lv_obj_del(img);
	_events_init_user_items(0);
	lv_obj_add_flag(s_UserItemObj[0], LV_OBJ_FLAG_HIDDEN);

	if(s_UserItemObj[1]==NULL)
	{
		for (int i = 1; i < MAX_THERMOSTAT_USER_NUM; i++)
		{
			s_UserItemObj[i] = lv_list_add_btn(guider_ui.screen_thermostat_user_list_list_user_info, NULL, " ");
			lv_obj_add_style(s_UserItemObj[i], &style_user_item_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
			lv_obj_add_style(s_UserItemObj[i], &style_user_item_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);
			lv_obj_add_style(s_UserItemObj[i], &style_user_item_main_focused, LV_PART_MAIN|LV_STATE_FOCUSED);
			lv_obj_add_flag(s_UserItemObj[i], LV_OBJ_FLAG_HIDDEN);

			_events_init_user_items(i);
		}
		s_UserItemSelectedNo = INVALID_USER_ITEM_NO;
	}
}

uint8_t gui_thermostat_get_user_item_num(void)
{
	return s_UserItemNum;
}

void gui_thermostat_set_user_item_num(uint8_t item_num)
{
	if (s_UserItemNum < MAX_THERMOSTAT_USER_NUM)
	{
		s_UserItemNum = item_num;
	}
}

uint8_t gui_thermostat_get_user_item_no(void)
{
	return s_UserItemSelectedNo;
}

void gui_thermostat_set_user_item_no(uint8_t item_no)
{
	if (item_no < s_UserItemNum)
	{
		s_UserItemSelectedNo = item_no;
	}
}

const thermostat_user_list_obj_t *gui_thermostat_get_user_info(uint8_t item_no)
{
    if (item_no < s_UserItemNum)
    {
        return &s_UserInfo[item_no];
    }
    return NULL;
}

const thermostat_user_list_obj_t *gui_thermostat_get_user_info_from_faceid(uint16_t face_id)
{
    for (int i = 0; i < s_UserItemNum; i++)
    {
        if (s_UserInfo[i].face_id == face_id)
            return &s_UserInfo[i];
    }

    return NULL;
}

thermostat_user_list_obj_t *gui_thermostat_get_selected_user_info()
{
    return &s_UserInfo[s_UserItemSelectedNo];
}

void gui_thermostat_set_user_info(thermostat_user_list_obj_t *user_info, uint8_t item_no)
{
	if (item_no <= s_UserItemNum)
	{
     user_info->user_name[MAX_THERMOSTAT_USER_NAME_SIZE - 1] = '\0';
     s_UserInfo[item_no].face_id                             = user_info->face_id;
     memcpy(&s_UserInfo[item_no].user_name, user_info->user_name, sizeof(user_info->user_name));
     memcpy(&s_UserInfo[item_no].temperature, user_info->temperature, sizeof(user_info->temperature));
     s_UserInfo[item_no].mode      = user_info->mode;
     s_UserInfo[item_no].fan_speed = user_info->fan_speed;
 }
}

void gui_thermostat_lvgl_refresh_user_list(uint8_t item_no)
{
    if (item_no >= MAX_THERMOSTAT_USER_NUM)
    {
        return;
    }
    char s_UserItemStr[MAX_USER_ITEM_STR_SIZE];

    language_t language = UI_GetLanguage();

    memset(s_UserItemStr, ' ', MAX_USER_ITEM_STR_SIZE);

    memcpy(s_UserItemStr, &s_UserInfo[item_no].user_name, strlen((char *)&s_UserInfo[item_no].user_name));
    memcpy(&s_UserItemStr[TEMPERATURE_STR_LOC], &s_UserInfo[item_no].temperature,
           strlen((char *)&s_UserInfo[item_no].temperature));
    memcpy(&s_UserItemStr[MODE_STR_LOC], s_ModeStr[language][s_UserInfo[item_no].mode],
           strlen(s_ModeStr[language][s_UserInfo[item_no].mode]));
    memcpy(&s_UserItemStr[FAN_SPEED_STR_LOC], s_FanSpeedStr[language][s_UserInfo[item_no].fan_speed],
           strlen(s_FanSpeedStr[language][s_UserInfo[item_no].fan_speed]));
    s_UserItemStr[MAX_USER_ITEM_STR_SIZE - 1] = '\0';

    lv_obj_t *label_child = lv_obj_get_child(s_UserItemObj[item_no], 0);
    lv_label_set_text(label_child, s_UserItemStr);
    lv_obj_clear_flag(s_UserItemObj[item_no], LV_OBJ_FLAG_HIDDEN);
}

void gui_thermostat_lvgl_update_user_item(thermostat_user_list_obj_t *user_info, uint8_t item_no)
{
	gui_thermostat_set_user_info(user_info, item_no);
	gui_thermostat_lvgl_refresh_user_list(item_no);
}

void gui_thermostat_lvgl_add_user_item(thermostat_user_list_obj_t *user_info)
{
    gui_thermostat_lvgl_update_user_item(user_info, s_UserItemNum);
    ++s_UserItemNum;
}

void gui_thermostat_lvgl_del_user_item(uint8_t item_no)
{
	if(item_no == INVALID_USER_ITEM_NO || s_UserItemNum == 0)
	{
		return;
	}

	lv_obj_clear_state(s_UserItemObj[item_no], LV_STATE_FOCUSED);

	for (int i = item_no; i < s_UserItemNum; i++)
	{
		uint8_t next_item_no = i + 1;
		if (next_item_no < s_UserItemNum)
		{
			gui_thermostat_lvgl_update_user_item(&s_UserInfo[next_item_no], i);
		}
		else
		{
			memset(&s_UserInfo[i], 0, sizeof(thermostat_user_list_obj_t));
			lv_obj_add_flag(s_UserItemObj[i], LV_OBJ_FLAG_HIDDEN);
			break;
		}
	}
	s_UserItemNum--;
}

void gui_thermostat_lvgl_del_selected_user_item(void)
{
    gui_thermostat_lvgl_del_user_item(s_UserItemSelectedNo);
    s_UserItemSelectedNo = INVALID_USER_ITEM_NO;
}

void gui_thermostat_lvgl_edit_user_item(void)
{
	thermostat_user_list_obj_t user_info;

 const char *user_name = lv_textarea_get_text(guider_ui.screen_thermostat_user_edit_ta_user_name);
 strcpy((char *)&user_info.user_name, user_name);
 lv_dropdown_get_selected_str(guider_ui.screen_thermostat_user_edit_ddlist_temperatrue, (char *)&user_info.temperature,
                              TEMPERATURE_VALUE_SIZE);
 user_info.mode      = lv_dropdown_get_selected(guider_ui.screen_thermostat_user_edit_ddlist_mode);
 user_info.fan_speed = lv_dropdown_get_selected(guider_ui.screen_thermostat_user_edit_ddlist_fan_speed);
 user_info.face_id = s_UserInfo[s_UserItemSelectedNo].face_id;

	gui_thermostat_lvgl_update_user_item(&user_info, s_UserItemSelectedNo);
}

void gui_thermostat_lvgl_refresh_user_editing_item(void)
{
    const thermostat_user_list_obj_t *user_info = gui_thermostat_get_user_info(s_UserItemSelectedNo);

    if (user_info)
    {
        lv_textarea_set_text(guider_ui.screen_thermostat_user_edit_ta_user_name, (char *)&user_info->user_name);
        int opt = lv_dropdown_get_option_index(guider_ui.screen_thermostat_user_edit_ddlist_temperatrue,
                                               (char *)&user_info->temperature);
        lv_dropdown_set_selected(guider_ui.screen_thermostat_user_edit_ddlist_temperatrue, opt);
        lv_dropdown_set_selected(guider_ui.screen_thermostat_user_edit_ddlist_mode, user_info->mode);
        lv_dropdown_set_selected(guider_ui.screen_thermostat_user_edit_ddlist_fan_speed, user_info->fan_speed);
    }
}

void gui_thermostat_init_userlist_fromdb()
{
    s_UserItemNum = UI_InitThermostatUserList(s_UserInfo, MAX_THERMOSTAT_USER_NUM);
}

void gui_thermostat_useredit_set_language_font(language_t language)
{
    lv_style_value_t fontLabel;

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_50 : &lv_font_montserratMedium_50;

    lv_obj_set_local_style_prop(guider_ui.screen_thermostat_user_edit_label_user_list, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
}

void gui_thermostat_useredit_set_language_text(language_t language)
{
    gui_thermostat_useredit_set_language_font(language);
    lv_dropdown_set_selected(guider_ui.screen_thermostat_user_edit_ddlist_language, language);
    lv_label_set_text(guider_ui.screen_thermostat_user_edit_label_user_list, s_UserListStr[language][0]);
}

void gui_thermostat_useredit_init(language_t language)
{
    gui_thermostat_useredit_set_language_text(language);
    gui_thermostat_lvgl_refresh_user_editing_item();
    gui_thermostat_lvgl_update_kb_style();
    gui_thermostat_lvgl_focus_user_name();
    gui_thermostat_lvgl_update_cursor_color(lv_color_make(0xff, 0xff, 0xff));
}

void gui_thermostat_userlist_set_language_font(language_t language)
{
    lv_style_value_t fontLabel;

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_40 : &lv_font_montserratMedium_38;
    lv_obj_set_local_style_prop(guider_ui.screen_thermostat_user_list_label_delete, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_50 : &lv_font_montserratMedium_50;

    lv_obj_set_local_style_prop(guider_ui.screen_thermostat_user_list_label_user_list, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_local_style_prop(guider_ui.screen_thermostat_user_list_label_title, LV_STYLE_TEXT_FONT, fontLabel,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
}

void gui_thermostat_userlist_set_language_text(language_t language)
{
    gui_thermostat_userlist_set_language_font(language);

    lv_dropdown_set_selected(guider_ui.screen_thermostat_user_list_ddlist_language, language);
    lv_label_set_text(guider_ui.screen_thermostat_user_list_label_delete, s_DeleteInteractionStr[language][0]);
    lv_label_set_text(guider_ui.screen_thermostat_user_list_label_user_list, s_UserListStr[language][0]);
    lv_label_set_text(guider_ui.screen_thermostat_user_list_label_title, s_ThermostatTitleStr[language][0]);

    for (int i = 0; i < s_UserItemNum; i++)
    {
        gui_thermostat_lvgl_refresh_user_list(i);
    }

    for (int i = s_UserItemNum; i < MAX_THERMOSTAT_USER_NUM; i++)
    {
        lv_obj_add_flag(s_UserItemObj[i], LV_OBJ_FLAG_HIDDEN);
    }
}

void gui_thermostat_userlist_init_state(language_t language)
{
    gui_thermostat_lvgl_hide_user_del_ui(true);
    gui_thermostat_lvgl_hide_user_editor(false);
    gui_thermostat_events_init_user_misc_widgets(&guider_ui);

    gui_thermostat_userlist_set_language_text(language);
    lv_label_set_text(guider_ui.screen_thermostat_user_list_label_user_list, s_UserListStr[language][0]);
    lv_dropdown_set_selected(guider_ui.screen_thermostat_user_list_ddlist_language, language);
    lv_label_set_text(guider_ui.screen_thermostat_user_list_label_title, s_ThermostatTitleStr[language][0]);

    if (gui_thermostat_get_user_item_num())
    {
        gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_delete, true);
        gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_edit, true);
        if (s_UserItemSelectedNo == INVALID_USER_ITEM_NO)
        {
            s_UserItemSelectedNo = 0;
        }
        gui_thermostat_lvgl_focus_user_item(true);
    }
    else
    {
        gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_delete, false);
        gui_lvgl_enable_buttons(guider_ui.screen_thermostat_user_list_imgbtn_edit, false);
    }
}
