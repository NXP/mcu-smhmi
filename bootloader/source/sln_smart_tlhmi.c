/*
 * Copyright 2019-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <stdbool.h>
#include "board.h"
#include "sln_app_specific.h"
#include "fica_definition.h"

/* include buttons api */
#include "sln_push_buttons_driver.h"

void SLN_CheckForAppBoot(int32_t *imgType)
{
    *imgType = FICA_IMG_TYPE_NONE;

    if (PUSH_BUTTONS_SW1Pressed())
    {
        *imgType = FICA_IMG_TYPE_APP_A;
    }
    if (PUSH_BUTTONS_SW2Pressed())
    {
        *imgType = FICA_IMG_TYPE_APP_B;
    }
    else if (PUSH_BUTTONS_SW3Pressed())
    {
        *imgType = FICA_IMG_TYPE_APP_C;
    }
}
