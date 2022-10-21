/*
 * Copyright 2019-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief output led device implementation.
 */

#include "board_define.h"
#ifdef ENABLE_OUTPUT_DEV_RgbLed
#include "FreeRTOS.h"
#include "board.h"

#include "fwk_log.h"
#include "fwk_output_manager.h"
#include "hal_output_dev.h"

#if defined(__cplusplus)
extern "C" {
#endif

__attribute__((weak)) rgbLedColor_t APP_OutputDev_RgbLed_InferCompleteDecode(output_algo_source_t source,
                                                                             void *inferResult,
                                                                             uint32_t *timerOn)
{
    return kRGBLedColor_Off;
}

__attribute__((weak)) rgbLedColor_t APP_OutputDev_RgbLed_InputNotifyDecode(void *inputData)
{
    return kRGBLedColor_Off;
}

static hal_output_status_t HAL_OutputDev_RgbLed_InferComplete(const output_dev_t *dev,
                                                              output_algo_source_t source,
                                                              void *inferResult);
static hal_output_status_t HAL_OutputDev_RgbLed_InputNotify(const output_dev_t *dev, void *data);

static hal_output_status_t HAL_OutputDev_RgbLed_Init(output_dev_t *dev, output_dev_callback_t callback);
static hal_output_status_t HAL_OutputDev_RgbLed_Start(const output_dev_t *dev);

#if defined(__cplusplus)
}
#endif

static void _SetLedColor(rgbLedColor_t color)
{
    switch (color)
    {
        case kRGBLedColor_Red:
            GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 1);
            GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 0);
            GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 0);
            break;
        case kRGBLedColor_Orange:
            GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 1);
            GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 1);
            GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 0);
            break;
        case kRGBLedColor_Yellow:
            GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 1);
            GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 1);
            GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 0);
            break;
        case kRGBLedColor_Green:
            GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 0);
            GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 1);
            GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 0);
            break;
        case kRGBLedColor_Blue:
            GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 0);
            GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 0);
            GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);
            break;
        case kRGBLedColor_Purple:
            GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 1);
            GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 0);
            GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);
            break;
        case kRGBLedColor_Cyan:
            GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 0);
            GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 1);
            GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);
            break;
        case kRGBLedColor_White:
            GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 1);
            GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 1);
            GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);
            break;
        case kRGBLedColor_Off:
            GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 0);
            GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 0);
            GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 0);
            break;

        default:
            /* Better to do nothing */
            break;
    }
}
const static output_dev_event_handler_t s_OutputDev_RgbLedHandler = {
    .inferenceComplete = HAL_OutputDev_RgbLed_InferComplete,
    .inputNotify       = HAL_OutputDev_RgbLed_InputNotify,
};

static TimerHandle_t OutputRgbTimer = NULL;

static void LedTimerCallback(TimerHandle_t xTimer)
{
    _SetLedColor(kRGBLedColor_Off);
}

static hal_output_status_t HAL_OutputDev_RgbLed_InferComplete(const output_dev_t *dev,
                                                              output_algo_source_t source,
                                                              void *inferResult)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    uint32_t timerOn          = 0;
    _SetLedColor(APP_OutputDev_RgbLed_InferCompleteDecode(source, inferResult, &timerOn));

    if (timerOn != 0)
    {
        xTimerChangePeriod(OutputRgbTimer, pdMS_TO_TICKS(timerOn), 0);
    }
    return error;
}

static hal_output_status_t HAL_OutputDev_RgbLed_InputNotify(const output_dev_t *dev, void *data)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    _SetLedColor(APP_OutputDev_RgbLed_InputNotifyDecode(data));

    return error;
}

const static output_dev_operator_t s_OutputDev_RgbLedOps = {
    .init   = HAL_OutputDev_RgbLed_Init,
    .deinit = NULL,
    .start  = HAL_OutputDev_RgbLed_Start,
    .stop   = NULL,
};

static output_dev_t s_OutputDev_RgbLed = {
    .name         = "rgb_led",
    .attr.type    = kOutputDevType_Other,
    .attr.reserve = NULL,
    .ops          = &s_OutputDev_RgbLedOps,
    .cap          = {.callback = NULL},
};

hal_output_status_t HAL_OutputDev_RgbLed_Init(output_dev_t *dev, output_dev_callback_t callback)
{
    int error         = kStatus_HAL_OutputSuccess;
    uint32_t timer_id = 0;
    LOGD("++HAL_OutputDev_RgbLed_Init");

    dev->cap.callback        = callback;
    gpio_pin_config_t config = {
        .direction     = kGPIO_DigitalOutput, /*!< Specifies the pin direction. */
        .outputLogic   = 0,                   /*!< Set a default output logic, which has no use in input */
        .interruptMode = kGPIO_NoIntmode,     /*!< Specifies the pin interrupt mode, a value of
                                                 @ref gpio_interrupt_mode_t. */
    };

    GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, &config);
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, &config);
    GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, &config);

    OutputRgbTimer = xTimerCreate("OutputRgbTimer", (TickType_t)pdMS_TO_TICKS(2000), pdFALSE, &timer_id,
                                  (TimerCallbackFunction_t)LedTimerCallback);

    LOGD("--HAL_OutputDev_RgbLed_Init");
    return error;
}

static hal_output_status_t HAL_OutputDev_RgbLed_Start(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_RgbLed_Start");

    /* TODO: the "const" of both args is discarded by being passed here */
    if (FWK_OutputManager_RegisterEventHandler(dev, &s_OutputDev_RgbLedHandler) != 0)
    {
        error = kStatus_HAL_OutputError;
    }

    LOGD("--HAL_OutputDev_RgbLed_Start");
    return error;
}

int HAL_OutputDev_RgbLed_Register()
{
    int error = 0;
    LOGD("++HAL_OutputDev_RgbLed_Register");

    error = FWK_OutputManager_DeviceRegister(&s_OutputDev_RgbLed);

    LOGD("--HAL_OutputDev_RgbLed_Register");
    return error;
}

#endif /* ENABLE_OUTPUT_DEV_RgbLed */
