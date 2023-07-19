/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board_define.h"
#ifdef ENABLE_OUTPUT_DEV_RgbLed
#include "hal_output_dev.h"
#include "hal_voice_algo_asr_local.h"
#include "hal_event_descriptor_common.h"
#include "hal_vision_algo.h"

rgbLedColor_t APP_OutputDev_RgbLed_InferCompleteDecode(output_algo_source_t source,
                                                       void *inferResult,
                                                       uint32_t *timerOn)
{
    rgbLedColor_t color = kRGBLedColor_Off;
    switch (source)
    {
        case kOutputAlgoSource_Voice:
        {
            asr_inference_result_t *voiceAlgoResult = (asr_inference_result_t *)inferResult;

            if (voiceAlgoResult != NULL)
            {
                switch (voiceAlgoResult->status)
                {
                    case ASR_WW_DETECT:
                        color = kRGBLedColor_Blue;
                        *timerOn = 1000;
                        break;
                    case ASR_CMD_DETECT:
                        color    = kRGBLedColor_Green;
                        *timerOn = 1000;
                        break;
                    case ASR_TIMEOUT:
                        color    = kRGBLedColor_Red;
                        *timerOn = 1000;
                        break;
                    default:
                        break;
                }
            }
            break;
        }

        case kOutputAlgoSource_Vision:
        {
            vision_algo_result_t *vres = (vision_algo_result_t *)inferResult;
            if (vres->id == kVisionAlgoID_OasisLite)
            {
                oasis_lite_result_t *oasis = &vres->oasisLite;
                if (oasis->state == kOASISLiteState_Recognition)
                {
                    if ((!oasis->face_recognized) && (oasis->face_count > 0))
                    {
                        // face detected, but not recognized
                        color = kRGBLedColor_Blue;
                        *timerOn = 200;
                    }
                }
            }
            break;
        }

        default:
            break;
    }

    return color;
}

rgbLedColor_t APP_OutputDev_RgbLed_InputNotifyDecode(void *inputData)
{
    rgbLedColor_t color    = kRGBLedColor_None;
    event_base_t eventBase = *(event_base_t *)inputData;

    if (eventBase.eventId == kEventID_OTAStart)
    {
        color = kRGBLedColor_Purple;
    }
    else if (eventBase.eventId == kEventID_OTAStop)
    {
        color = kRGBLedColor_Green;
    }
    else if (eventBase.eventId == kEventID_OTAStatus)
    {
        event_common_t eventCommon = *(event_common_t *)inputData;
        if (eventCommon.otaStatus.percentage % 2)
        {
            color = kRGBLedColor_Purple;
        }
        else
        {
            color = kRGBLedColor_Off;
        }
    }

    return color;
}

#endif /* ENABLE_OUTPUT_DEV_RgbLed */
