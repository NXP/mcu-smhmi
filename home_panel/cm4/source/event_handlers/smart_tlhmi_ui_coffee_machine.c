/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board_define.h"
#ifdef ENABLE_OUTPUT_DEV_UiLvgl
#include "hal_output_dev.h"
#include "smart_tlhmi_event_descriptor.h"
#include "hal_voice_algo_asr_local.h"
#include "fwk_config.h"
#include "fwk_log.h"

static inline void _HAL_OutputDev_UiLvgl_Response(event_base_t eventBase,
                                                  void *response,
                                                  event_status_t status,
                                                  unsigned char isFinished)
{
    if (eventBase.respond != NULL)
    {
        eventBase.respond(eventBase.eventId, response, status, isFinished);
    }
}

uint32_t APP_OutputDev_UiLvgl_InputNotifyDecode(event_base_t *inputData)
{
    event_status_t eventResponse = kEventStatus_Ok;

    switch (inputData->eventId)
    {
        case kEventID_SetCoffeeType:
        {
            event_smart_tlhmi_t event = *(event_smart_tlhmi_t *)inputData;
            set_coffee_type(event.coffeeType.type);

            // TODO: Add coffee type as part of flash config
            if (kSLNConfigStatus_Success /*!= HAL_OutputDev_SmartLockConfig_SetSpeakerVolume(event.speakerVolume.volume)*/)
            {
                LOGE("Failed to save coffee type to flash");
            }

            if (inputData->respond != NULL)
            {
                _HAL_OutputDev_UiLvgl_Response(*inputData, &event.coffeeType.type, eventResponse, true);
            }
        }
        break;

        case kEventID_GetCoffeeType:
        {
            coffee_type_event_t coffeeType;
            coffeeType.type = get_coffee_type();
            LOGD("Coffee type is currently set to: %d", coffeeType.type);
            if (inputData->respond != NULL)
            {
                _HAL_OutputDev_UiLvgl_Response(*inputData, &coffeeType, eventResponse, true);
            }
        }
    }
    return 0;
}

#endif /* ENABLE_OUTPUT_DEV_UiLvgl */
