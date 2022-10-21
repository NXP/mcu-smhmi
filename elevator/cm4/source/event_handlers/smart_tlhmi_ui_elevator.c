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
    }
    return 0;
}

#endif /* ENABLE_OUTPUT_DEV_UiLvgl */
