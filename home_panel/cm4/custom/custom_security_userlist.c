// SPDX-License-Identifier: MIT
// Copyright 2022 NXP

/**
 * @file custom_security_userlist.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "custom.h"
#include "lvgl/lvgl.h"
#include <stdio.h>
#include "ui_strings_internal.h"

#define MAX_SECURITY_USERS 5
#define INVALID_USER       (-1)

static lv_obj_t *s_UserItems[MAX_SECURITY_USERS];
static int s_CurrentUser      = INVALID_USER;
static int s_LastSelectedUser = INVALID_USER;
static int s_RegisteredUsers;

static security_user_t users[MAX_SECURITY_USERS];

#define UPDATE_UI_USER_ITEM(INDEX)                                                                      \
    {                                                                                                   \
        s_UserItems[INDEX] = guider_ui.screen_security_user_list_list_user_info_item##INDEX;            \
        lv_obj_del(lv_obj_get_child(s_UserItems[INDEX], 0));                                            \
        lv_obj_add_event_cb(s_UserItems[INDEX], user_item_event_cb, LV_EVENT_ALL, &s_UserItems[INDEX]); \
        lv_obj_add_flag(s_UserItems[INDEX], LV_OBJ_FLAG_HIDDEN);                                        \
    }

static int obj_to_index(void *userdata)
{
    lv_obj_t **pItem = userdata;
    unsigned index   = pItem - s_UserItems;

    if (index > MAX_SECURITY_USERS)
    {
        index = INVALID_USER;
    }

    return index;
}

static void user_item_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    switch (code)
    {
        case LV_EVENT_RELEASED:
        {
            gui_lvgl_enable_buttons(guider_ui.screen_security_user_list_imgbtn_delete, true);
            gui_lvgl_enable_buttons(guider_ui.screen_security_user_list_imgbtn_edit, true);
            s_CurrentUser = obj_to_index(lv_event_get_user_data(e));
        }
        break;

        case LV_EVENT_DEFOCUSED:
        {
            gui_lvgl_enable_buttons(guider_ui.screen_security_user_list_imgbtn_delete, false);
            gui_lvgl_enable_buttons(guider_ui.screen_security_user_list_imgbtn_edit, false);
            s_LastSelectedUser = s_CurrentUser;
            s_CurrentUser      = INVALID_USER;
        }
        break;

        default:
            break;
    }
}

void gui_security_userlist_ui_init()
{
    // delete the image symbol and hide all the UI items first
    UPDATE_UI_USER_ITEM(0);
    UPDATE_UI_USER_ITEM(1);
    UPDATE_UI_USER_ITEM(2);
    UPDATE_UI_USER_ITEM(3);
    UPDATE_UI_USER_ITEM(4);

    gui_lvgl_enable_buttons(guider_ui.screen_security_user_list_imgbtn_delete, false);
    gui_lvgl_enable_buttons(guider_ui.screen_security_user_list_imgbtn_edit, false);
}

static void gui_security_userlist_show_user(int index)
{
    char info[32];

    if (index < 0 || index > s_RegisteredUsers)
    {
        return;
    }

    memset(info, ' ', sizeof(info));
    language_t language = UI_GetLanguage();
    size_t len = strlen(users[index].name);
    if (len > 16)
        len = 16;
    memcpy(info, users[index].name, len);
    snprintf(&info[16], 16, "%s", s_AccessGrantedStr[language][0]);

    lv_obj_t *label = lv_obj_get_child(s_UserItems[index], 0);
    lv_label_set_text(label, info);
    lv_obj_clear_flag(s_UserItems[index], LV_OBJ_FLAG_HIDDEN);
}

static void gui_security_userlist_set_language_font(language_t language)
{
    lv_style_value_t fontLabel;

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_50 : &lv_font_montserratMedium_50;

    lv_obj_set_local_style_prop(guider_ui.screen_security_user_list_label_title, LV_STYLE_TEXT_FONT, fontLabel,
            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_local_style_prop(guider_ui.screen_security_user_list_label_user_list, LV_STYLE_TEXT_FONT, fontLabel,
            LV_PART_MAIN | LV_STATE_DEFAULT);
}

void gui_security_userlist_set_language_text(language_t language)
{
    gui_security_userlist_set_language_font(language);

    lv_dropdown_set_selected(guider_ui.screen_security_user_list_ddlist_language, language);
    lv_label_set_text(guider_ui.screen_security_user_list_label_title, s_SecurityControlStr[language][0]);
    lv_label_set_text(guider_ui.screen_security_user_list_label_user_list, s_UserListStr[language][0]);

    for (int i = 0; i < s_RegisteredUsers; i++)
    {
        gui_security_userlist_show_user(i);
    }

    for (int i = s_RegisteredUsers; i < MAX_SECURITY_USERS; i++)
    {
        lv_obj_add_flag(s_UserItems[i], LV_OBJ_FLAG_HIDDEN);
    }
}

void gui_security_init_userlist_fromdb()
{
    s_RegisteredUsers = UI_InitSecurityUserList(users, MAX_SECURITY_USERS);
}

void gui_security_userlist_init(language_t language)
{
    gui_security_userlist_set_language_text(language);
}

void gui_security_userlist_add_newuser()
{
    if (s_RegisteredUsers < MAX_SECURITY_USERS)
    {
        uint16_t faceid = UI_GetFaceId();

        if (faceid != (uint16_t)-1)
        {
            int index = s_RegisteredUsers++;

            users[index].face_id = faceid;
            snprintf(users[index].name, sizeof(users[index].name), "User_%02d", index);
            gui_security_userlist_show_user(index);

            PlayPrompt(PROMPT_REGISTRATION_SUCCESS, 1);
            UI_Security_UpdateDB_Callback(&users[index], kUserDB_Add);
        }
    }
}

void gui_security_del_selected_user()
{
    if (s_LastSelectedUser == INVALID_USER)
    {
        return;
    }

    PlayPrompt(PROMPT_USER_DELETED, 1);
    UI_Security_UpdateDB_Callback(&users[s_LastSelectedUser], kUserDB_Remove);

    for (int i = s_LastSelectedUser; i < s_RegisteredUsers - 1; i++)
    {
        // move the later users upper one slot
        users[i] = users[i + 1];
        gui_security_userlist_show_user(i);
    }
    --s_RegisteredUsers;

    for (int i = s_RegisteredUsers; i < MAX_SECURITY_USERS; i++)
    {
        // hide remaining unregistered slots
        lv_obj_add_flag(s_UserItems[i], LV_OBJ_FLAG_HIDDEN);
    }

    s_LastSelectedUser = INVALID_USER;
}

void gui_security_update_selected_user()
{
    if (s_LastSelectedUser < 0 || s_LastSelectedUser >= MAX_SECURITY_USERS)
    {
        return;
    }

    const char *name      = lv_textarea_get_text(guider_ui.screen_security_user_edit_ta_user_name);
    security_user_t *user = &users[s_LastSelectedUser];
    snprintf(user->name, sizeof(user->name), "%s", name);

    UI_Security_UpdateDB_Callback(user, kUserDB_Update);

    s_LastSelectedUser = INVALID_USER;
}

static void gui_security_useredit_set_language_font(language_t language)
{
    lv_style_value_t fontLabel;

    fontLabel.ptr = (language == kLanguage_CN) ? &lv_font_simsun_50 : &lv_font_montserratMedium_50;

    lv_obj_set_local_style_prop(guider_ui.screen_security_user_edit_label_title, LV_STYLE_TEXT_FONT, fontLabel,
            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_local_style_prop(guider_ui.screen_security_user_edit_label_user_list, LV_STYLE_TEXT_FONT, fontLabel,
            LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void gui_security_useredit_set_language_text(language_t language)
{
    gui_security_useredit_set_language_font(language);

    lv_label_set_text(guider_ui.screen_security_user_edit_label_title, s_SecurityControlStr[language][0]);
    lv_label_set_text(guider_ui.screen_security_user_edit_label_user_list, s_UserListStr[language][0]);
}

void gui_security_useredit_init(language_t language)
{
    gui_security_useredit_set_language_text(language);

    // update keyboard style: default keyboard text too small
    lv_obj_t *kb = lv_obj_get_child(guider_ui.screen_security_user_edit, 0);
    lv_obj_set_style_text_font(kb, &lv_font_montserratMedium_25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(kb, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // update textarea with selected username, and popup the keyboard by default
    lv_textarea_set_text(guider_ui.screen_security_user_edit_ta_user_name,
                         (const char *)users[s_LastSelectedUser].name);
    lv_event_send(guider_ui.screen_security_user_edit_ta_user_name, LV_EVENT_FOCUSED, NULL);
    lv_obj_set_style_border_color(guider_ui.screen_security_user_edit_ta_user_name, lv_color_make(0xff, 0xff, 0xff),
                                  LV_PART_CURSOR | LV_STATE_FOCUSED);
}

const char *gui_security_get_user_name(int user_no)
{
	return users[user_no].name;
}

int gui_security_get_selected_user_no(void)
{
	return s_LastSelectedUser;
}

int gui_security_get_max_user_num(void)
{
	return MAX_SECURITY_USERS;
}
