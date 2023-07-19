/*
 * Copyright 2023 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */
#ifndef __SMART_THLMI_MQS_H__
#define __SMART_THLMI_MQS_H__

typedef enum
{
    Have_Your_Favorite_Setttings_Confirm_Cancel,
    Delete_Selected_User_Confirm_Cancel,
    Start_Registration,
    Identified_New_User_Verify_Pin,
    Invalid_Pin,
    Registration_Successful,
    Verify_Pin_To_Delete_User,
    User_Deleted,
    Registered_User,
    Come_Closer,
    Stand_Still,
    Face_The_Camera,
    Reposition_Face,
    Beep_Tone,
    PROMPT_INVALID
} prompt_type_t;

typedef struct _audio_prompt_info
{
    void *data;
    uint32_t len;
} audio_prompt_info_t;

#endif /* __SMART_THLMI_MQS_H__ */
