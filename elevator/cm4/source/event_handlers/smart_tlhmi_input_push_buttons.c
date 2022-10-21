/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "stdint.h"
#include "fsl_common.h"

#include "fwk_input_manager.h"
#include "fwk_log.h"
#include "hal_input_dev.h"
#include "hal_event_descriptor_common.h"

typedef enum _switch_press_type
{
    kSwitchPressType_Long  = 0,
    kSwitchPressType_Short = 1,
} switch_press_type_t;

typedef enum _switch_id
{
    kSwitchID_1 = 0,
    kSwitchID_2,
    kSwitchID_3,
    kSwitchID_Count,
} switch_id_t;

static event_common_t s_WakeUpEvent;

int APP_InputDev_PushButtons_SetEvent(
    switch_id_t button, switch_press_type_t pressType, void **event, uint32_t *receiverList, uint32_t *size)
{
    int ret = kStatus_Fail;

    if (event == NULL || receiverList == NULL || size == NULL)
    {
        return ret;
    }

    switch (button)
    {
        case kSwitchID_1:
        case kSwitchID_2:
        case kSwitchID_3:
        {
            *receiverList                     = 1 << kFWKTaskID_Output;
            s_WakeUpEvent.eventBase.eventId   = kEventID_WakeUp;
            s_WakeUpEvent.eventBase.eventInfo = kEventInfo_Local;
            *event                            = &s_WakeUpEvent;
            *size                             = sizeof(s_WakeUpEvent);
            ret                               = kStatus_Success;
        }
        break;
        default:
            break;
    }
        // LOGD("[InputButton]: button:%d pressType:%d", button, pressType);
#if 0
    *receiverList = 0;
    switch (button)
    {
        case kSwitchID_1:
            if (pressType == kSwitchPressType_Long)
            {
                LOGD("Long PRESS Detected.");
                unsigned int totalUsageCount;
                FWK_LpmManager_RequestStatus(&totalUsageCount);
                FWK_LpmManager_EnableSleepMode(kLPMManagerStatus_SleepEnable);
            }
            break;

        case kSwitchID_2:
            if ((pressType == kSwitchPressType_Short) || (pressType == kSwitchPressType_Long))
            {
                *receiverList                    = 1 << kFWKTaskID_VisionAlgo;
                s_FaceRecEvent.eventBase.eventId = kEventFaceRecID_DelUser;
                s_FaceRecEvent.delFace.hasName   = false;
                s_FaceRecEvent.delFace.hasID     = false;
                *event                           = &s_FaceRecEvent;
            }
            break;

        case kSwitchID_3:
            if ((pressType == kSwitchPressType_Short) || (pressType == kSwitchPressType_Long))
            {
                *receiverList                    = 1 << kFWKTaskID_VisionAlgo;
                s_FaceRecEvent.eventBase.eventId = kEventFaceRecID_AddUser;
                s_FaceRecEvent.addFace.hasName   = false;
                *event                           = &s_FaceRecEvent;
            }
            break;

        default:
            ret = kStatus_Fail;
            break;
    }
#endif

    return ret;
}
