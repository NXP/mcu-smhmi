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

static security_obj_t s_SecurityObj = {
    .state             = kSecurity_Off,
    .front_door_state  = kSecurity_Off,
    .back_door_state   = kSecurity_Off,
    .garage_door_state = kSecurity_Off,
};

thermostat_state gui_security_get_state()
{
    return s_SecurityObj.state;
}

void gui_security_set_state(security_state state)
{
    LVGL_LOG("security: state %d\r\n", state);
    s_SecurityObj.state = state;
    if (state == kSecurity_Off)
    {
        lv_obj_add_flag(guider_ui.screen_security_img_armed, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_security_img_disarmed, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_clear_flag(guider_ui.screen_security_img_armed, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_security_img_disarmed, LV_OBJ_FLAG_HIDDEN);
    }
}

void gui_security_switch_state(void)
{
    security_state new_state;
    if (s_SecurityObj.state == kSecurity_Off)
    {
        new_state = kSecurity_On;
    }
    else
    {
        new_state = kSecurity_Off;
    }
    LVGL_LOG("security: old state: %d new state %d\r\n", s_SecurityObj.state, new_state);
    gui_security_set_state(new_state);
}

security_state gui_security_get_door_state(security_door door)
{
    security_state state = kSecurity_Off;

    switch (door)
    {
        case kSecurity_Front:
        {
            state = s_SecurityObj.front_door_state;
        }
        break;

        case kSecurity_Back:
        {
            state = s_SecurityObj.back_door_state;
        }
        break;

        case kSecurity_Garage:
        {
            state = s_SecurityObj.garage_door_state;
        }
        break;

        default:
            break;
    }

    return state;
}

void gui_security_set_door_state(security_door door, security_state state)
{
    LVGL_LOG("security: door %d state:%d\r\n", door, state);
    security_state *pState = NULL;
    lv_obj_t *pDoorObj     = NULL;

    switch (door)
    {
        case kSecurity_Front:
        {
            pDoorObj = guider_ui.screen_security_imgbtn_front_door;
            pState   = &(s_SecurityObj.front_door_state);
        }
        break;

        case kSecurity_Back:
        {
            pDoorObj = guider_ui.screen_security_imgbtn_back_door;
            pState   = &(s_SecurityObj.back_door_state);
        }
        break;

        case kSecurity_Garage:
        {
            pDoorObj = guider_ui.screen_security_imgbtn_garage_door;
            pState   = &(s_SecurityObj.garage_door_state);
        }
        break;

        default:
            break;
    }

    if (pDoorObj != NULL && pState != NULL)
    {
        *pState = state;
        if (state == kSecurity_Off)
        {
            lv_imgbtn_set_state(pDoorObj, LV_IMGBTN_STATE_RELEASED);
        }
        else
        {
            lv_imgbtn_set_state(pDoorObj, LV_IMGBTN_STATE_CHECKED_RELEASED);
        }
    }
}

void gui_security_switch_door_state(security_door door)
{
    security_state new_state;
    security_state cur_state = gui_security_get_door_state(door);

    if (cur_state == kSecurity_Off)
    {
        new_state = kSecurity_On;
    }
    else
    {
        new_state = kSecurity_Off;
    }
    LVGL_LOG("security: door %d old state: %d new state %d\r\n", cur_state, new_state);
    gui_security_set_door_state(door, new_state);
}
