/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief smart tlhmi-specific event descriptor declarations.
 */

#ifndef _SMART_TLHMI_EVENT_DESCRIPTOR_H_
#define _SMART_TLHMI_EVENT_DESCRIPTOR_H_

#include "hal_event_descriptor_common.h"
#include "hal_event_descriptor_face_rec.h"
#include "hal_event_descriptor_voice.h"
#include "hal_sln_elevatordb.h"

typedef enum _event_smart_tlhmi_id
{
    kEventElevatorId_RegisterFloor = kEventType_App,
    kEventElevatorID_AddUserRemote,
    kEventElevatorID_GetPreviewMode,
    kEventElevatorID_SetPreviewMode,
    kEventIElevatorD_SmartTLHMIID_COUNT
} event_smart_tlhmi_id_t;

typedef enum _preview_mode
{
    kPreviewMode_Camera,
    kPreviewMode_VirtualFace,
} preview_mode_t;

typedef struct _elevator_info
{
    uint16_t id;
    uint32_t floorNum;
    asr_language_t language;
} elevator_info_t;

typedef struct _event_smart_tlhmi
{
    event_base_t eventBase;

    union
    {
        void *data;
        preview_mode_t previewMode;
        elevator_info_t elevatorInfo;
    };
} event_smart_tlhmi_t;

#endif /* _SMART_TLHMI_EVENT_DESCRIPTOR_H_ */
