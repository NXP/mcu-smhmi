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

static thermostat_obj_t s_ThermostatObj = {
    .state     = kThermostat_On,
    .mode      = kThermostat_Auto,
    .fan_state = kThermostat_Fan_On,
    .fan_speed = kThermostat_Fan_Mid,
};

thermostat_state gui_thermostat_get_state()
{
    return s_ThermostatObj.state;
}

void gui_thermostat_set_state(thermostat_state state)
{
    LVGL_LOG("thermostat: state %d\r\n", state);
    // if (state != s_ThermostatObj.state)
    {
        s_ThermostatObj.state = state;
        if (state == kThermostat_Off)
        {
            lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_on_off, LV_IMGBTN_STATE_CHECKED_RELEASED);
        }
        else
        {
            lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_on_off, LV_IMGBTN_STATE_RELEASED);
        }
    }
}

void gui_thermostat_switch_state(void)
{
    thermostat_state new_state;
    if (s_ThermostatObj.state == kThermostat_Off)
    {
        new_state = kThermostat_On;
    }
    else
    {
        new_state = kThermostat_Off;
    }
    LVGL_LOG("thermostat: old state: %d new state %d\r\n", s_ThermostatObj.state, new_state);
    gui_thermostat_set_state(new_state);
}

thermostat_mode gui_thermostat_get_mode()
{
    return s_ThermostatObj.mode;
}

void gui_thermostat_set_mode(thermostat_mode mode)
{
    LVGL_LOG("thermostat: mode %d\r\n", mode);
    // if (mode != s_ThermostatObj.state)
    {
        s_ThermostatObj.mode = mode;
        switch (mode)
        {
            case kThermostat_Heat:
            {
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_heat, LV_IMGBTN_STATE_CHECKED_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_auto, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_cool, LV_IMGBTN_STATE_RELEASED);
            }
            break;
            case kThermostat_Cool:
            {
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_heat, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_auto, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_cool, LV_IMGBTN_STATE_CHECKED_RELEASED);
            }
            break;
            case kThermostat_Auto:
            {
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_heat, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_auto, LV_IMGBTN_STATE_CHECKED_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_mode_cool, LV_IMGBTN_STATE_RELEASED);
            }
            break;
        }
    }
}

thermostat_fan_speed gui_thermostat_get_fan_speed()
{
    return s_ThermostatObj.fan_speed;
}

void gui_thermostat_set_fan_speed(thermostat_fan_speed speed)
{
    LVGL_LOG("thermostat: fan_speed %d\r\n", speed);
    // if (speed != s_ThermostatObj.fan_speed)
    {
        s_ThermostatObj.fan_speed = speed;
        switch (speed)
        {
            case kThermostat_Fan_High:
            {
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_high, LV_IMGBTN_STATE_CHECKED_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_mid, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_low, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_auto, LV_IMGBTN_STATE_RELEASED);
            }
            break;
            case kThermostat_Fan_Mid:
            {
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_high, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_mid, LV_IMGBTN_STATE_CHECKED_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_low, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_auto, LV_IMGBTN_STATE_RELEASED);
            }
            break;
            case kThermostat_Fan_Low:
            {
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_high, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_mid, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_low, LV_IMGBTN_STATE_CHECKED_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_auto, LV_IMGBTN_STATE_RELEASED);
            }
            break;
            case kThermostat_Fan_Auto:
            {
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_high, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_mid, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_low, LV_IMGBTN_STATE_RELEASED);
                lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_auto, LV_IMGBTN_STATE_CHECKED_RELEASED);
            }
            break;
        }
    }
}

void gui_thermostat_set_fan_state(thermostat_fan_state state)
{
    LVGL_LOG("thermostat: fan state %d\r\n", state);
    // if (state != s_ThermostatObj.fan_state)
    {
        s_ThermostatObj.fan_state = state;
        if (state == kThermostat_Fan_On)
        {
            lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_onoff, LV_IMGBTN_STATE_CHECKED_RELEASED);
        }
        else
        {
            lv_imgbtn_set_state(guider_ui.screen_thermostat_imgbtn_fan_onoff, LV_IMGBTN_STATE_RELEASED);
        }
    }

}

void gui_thermostat_switch_fan_state()
{
    thermostat_fan_state newstate;

    if (s_ThermostatObj.fan_state == kThermostat_Fan_On)
    {
        newstate = kThermostat_Fan_Off;
    }
    else
    {
        newstate = kThermostat_Fan_On;
    }

    gui_thermostat_set_fan_state(newstate);
}

void gui_thermostat_init_state()
{
    gui_thermostat_set_state(s_ThermostatObj.state);
    gui_thermostat_set_mode(s_ThermostatObj.mode);
    gui_thermostat_set_fan_state(s_ThermostatObj.fan_state);
    gui_thermostat_set_fan_speed(s_ThermostatObj.fan_speed);
}
