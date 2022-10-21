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
#include "custom.h"

typedef enum _event_smart_tlhmi_id
{
    kEventID_GetCoffeeType = kEventType_App,
    kEventFaceRecId_RegisterCoffeeSelection,
    kEventID_SetCoffeeType,
    kEventID_GetPreviewMode,
    kEventID_SetPreviewMode,
    kEventID_SmartTLHMIID_COUNT
} event_smart_tlhmi_id_t;

typedef struct _coffee_type_event
{
    coffee_type_t type;
} coffee_type_event_t;

typedef enum _preview_mode
{
    PREVIEW_MODE_CAMERA,
    PREVIEW_MODE_VIRTUAL_FACE,
} preview_mode_t;

typedef struct _coffee_result
{
    uint8_t coffeeType;
    uint8_t coffeeSize;
    uint8_t coffeeStrength;
    asr_language_t language;
} coffee_result_t;

typedef struct _register_coffee_selection_event
{
    uint16_t id;
    coffee_result_t coffeeInfo;
} register_coffee_selection_event_t;

typedef struct _event_smart_tlhmi
{
    event_base_t eventBase;

    union
    {
        void *data;
        register_coffee_selection_event_t regCoffeeSelection;
        coffee_type_event_t coffeeType;
        preview_mode_t previewMode;
    };
} event_smart_tlhmi_t;

#endif /* _SMART_TLHMI_EVENT_DESCRIPTOR_H_ */
