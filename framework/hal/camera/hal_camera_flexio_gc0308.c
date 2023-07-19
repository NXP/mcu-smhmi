/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief camera dev flexio gc0308 HAL driver implementation.
 */

#include "board_define.h"
#ifdef ENABLE_CAMERA_DEV_FlexioGc0308
#include <FreeRTOS.h>
#include <task.h>
#include <stdlib.h>
#include <time.h>

#include "fsl_gpio.h"
#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "fsl_camera_device.h"
#include "fsl_flexio_camera_adapter.h"

#include "fsl_gc0308.h"

#include "board.h"
#include "fwk_log.h"
#include "fwk_camera_manager.h"

#include "hal_event_descriptor_common.h"

#define CAMERA_NAME              "FLEXIO_GC0308"
#define CAMERA_RGB_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)

#ifdef ENABLE_CAMERA_DEV_FlexioGc0308_FRAME_BUFFER_SDRAM
AT_NONCACHEABLE_SECTION_ALIGN_SDRAM(
    static uint8_t s_FrameBuffers[CAMERA_DEV_FlexioGc0308_BUFFER_COUNT][CAMERA_DEV_FlexioGc0308_HEIGHT]
                                 [CAMERA_DEV_FlexioGc0308_WIDTH * CAMERA_DEV_FlexioGc0308_BPP],
    64);
#else
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_FrameBuffers[CAMERA_DEV_FlexioGc0308_BUFFER_COUNT][CAMERA_DEV_FlexioGc0308_HEIGHT]
                                 [CAMERA_DEV_FlexioGc0308_WIDTH * CAMERA_DEV_FlexioGc0308_BPP],
    64);
#endif
static uint8_t *s_pCurrentFrameBuffer = NULL;
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FLEXIO_CAMERA_INST                 BOARD_FLEXIO_CAMERA_INST
#define FLEXIO_CAMERA_DATA_PIN_START_INDEX BOARD_FLEXIO_CAMERA_DATA_PIN_START_INDEX
#define FLEXIO_CAMERA_DATA_PIN_END_INDEX   BOARD_FLEXIO_CAMERA_DATA_PIN_END_INDEX
#define FLEXIO_CAMERA_XCLK_PIN_INDEX       BOARD_FLEXIO_CAMERA_XCLK_PIN_INDEX
#define FLEXIO_CAMERA_PCLK_PIN_INDEX       BOARD_FLEXIO_CAMERA_PCLK_PIN_INDEX
#define FLEXIO_CAMERA_HREF_PIN_INDEX       BOARD_FLEXIO_CAMERA_HREF_PIN_INDEX
#define FLEXIO_CAMERA_VSYNC_GPIO_BASE      BOARD_FLEXIO_CAMERA_VSYNC_GPIO_BASE
#define FLEXIO_CAMERA_VSYNC_PIN_INDEX      BOARD_FLEXIO_CAMERA_VSYNC_PIN_INDEX
#define FLEXIO_CAMERA_VSYNC_IRQn           BOARD_FLEXIO_CAMERA_VSYNC_IRQn
#define FLEXIO_CAMERA_VSYNC_IRQHandler     BOARD_FLEXIO_CAMERA_VSYNC_IRQHandler

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
void BOARD_FlexioCameraPullResetPin(bool pullUp);
void BOARD_FlexioCameraPullPowerDownPin(bool pullUp);
void BOARD_FlexioCameraI2CInit(void);
status_t BOARD_FlexioCameraI2CSend(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_FlexioCameraI2CReceive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#if defined(__cplusplus)
}
#endif /* __cplusplus */

static FLEXIO_CAMERA_Type s_FlexioCameraConfig = {
    .flexioBase      = FLEXIO_CAMERA_INST,
    .datPinStartIdx  = FLEXIO_CAMERA_DATA_PIN_START_INDEX,
    .pclkPinIdx      = FLEXIO_CAMERA_PCLK_PIN_INDEX,
    .hrefPinIdx      = FLEXIO_CAMERA_HREF_PIN_INDEX,
    .shifterStartIdx = 0U,
    .shifterCount    = 8U,
    .timerIdx        = 0U,
};

static flexio_camera_xclk_t s_FlexioXclkConfig = {
    .pinIdx   = FLEXIO_CAMERA_XCLK_PIN_INDEX,
    .timerIdx = 1,
};

static flexio_camera_dma_t s_FlexioDmaConfig = {
    .dmaCh = 2,
};

static flexio_camera_vsync_t s_FlexioVsyncConfig = {
    .portBase    = FLEXIO_CAMERA_VSYNC_GPIO_BASE,
    .pinIdx      = FLEXIO_CAMERA_VSYNC_PIN_INDEX,
    .irq         = FLEXIO_CAMERA_VSYNC_IRQn,
    .irqPriority = configLIBRARY_LOWEST_INTERRUPT_PRIORITY - 1,
};

static flexio_csi_resource_t s_FlexioCsiResource = {
    .flexioCsi   = &s_FlexioCameraConfig,
    .flexioXclk  = &s_FlexioXclkConfig,
    .flexioDma   = &s_FlexioDmaConfig,
    .flexioVsync = &s_FlexioVsyncConfig,
};

static flexio_camera_private_data_t s_FlexioPrivateData;

static camera_receiver_handle_t s_CameraReceiver = {
    .resource    = &s_FlexioCsiResource,
    .ops         = &flexio_csi_ops,
    .privateData = &s_FlexioPrivateData,
};

static gc0308_resource_t gc0308Resource = {
    .i2cSendFunc       = BOARD_FlexioCameraI2CSend,
    .i2cReceiveFunc    = BOARD_FlexioCameraI2CReceive,
    .pullResetPin      = BOARD_FlexioCameraPullResetPin,
    .pullPowerDownPin  = BOARD_FlexioCameraPullPowerDownPin,
    .inputClockFreq_Hz = 12000000,
};

static camera_device_handle_t s_CameraDevice = {
    .resource = &gc0308Resource,
    .ops      = &gc0308_ops,
};

static uint8_t s_CurRGBExposureMode = CAMERA_EXPOSURE_MODE_AUTO;

void FLEXIO_CAMERA_VSYNC_IRQHandler()
{
    uint32_t intPin = GPIO_PortGetInterruptFlags(FLEXIO_CAMERA_VSYNC_GPIO_BASE);

    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        if ((intPin >> FLEXIO_CAMERA_VSYNC_PIN_INDEX) & 0x01)
        {
            GPIO_ClearPinsInterruptFlags(FLEXIO_CAMERA_VSYNC_GPIO_BASE, 1U << FLEXIO_CAMERA_VSYNC_PIN_INDEX);
            FLEXIO_CSI_ADAPTER_Callback(&s_CameraReceiver);
        }
    }
}

static void _HAL_CameraDev_InitResources(void)
{
    const clock_root_config_t flexiorootclk = {
        .clockOff = false,
        .mux      = kCLOCK_FLEXIO2_ClockRoot_MuxSysPll3Div2,
        .div      = 2,
    };

    /* Configure FLEXIO2 using SYS_PLL3_DIV2_CLK */
    CLOCK_SetRootClock(kCLOCK_Root_Flexio2, &flexiorootclk);

    BOARD_FlexioCameraI2CInit();
}

static void _HAL_CameraDev_InitInterface(void)
{
}

static uint8_t _HAL_CameraDev_GetTargetExposureMode(uint8_t curMode, uint8_t direction)
{
    uint8_t mode;
    uint8_t modeMin      = CAMERA_EXPOSURE_MODE_AUTO_LEVEL0;
    uint8_t modeMax      = CAMERA_EXPOSURE_MODE_AUTO_LEVEL3;
    uint8_t modeInterval = 1;
    if (direction)
    {
        mode = curMode + modeInterval;
        if (mode >= modeMax)
            mode = modeMax;
    }
    else
    {
        if (curMode <= (modeInterval + modeMin))
        {
            mode = modeMin;
        }
        else
        {
            mode = curMode - modeInterval;
        }
    }
    LOGI("Camera exposure [curMode:%d][targetMode:%d]", curMode, mode);
    return mode;
}

static void HAL_CameraDev_FlexioGc0308_ReceiverCallback(camera_receiver_handle_t *handle,
                                                        status_t status,
                                                        void *userData)
{
    camera_dev_t *dev = (camera_dev_t *)userData;

    if (dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        camera_event_t event;
        event.eventId = kCameraEvent_SendFrame;
#ifdef ENABLE_CAMERA_DEV_FlexioGc0308_REMOTE_CSC
        /*
         * Need local camera manager to handle the dequeue message,
         * then send remote dequeue message to remote camera manager for CSC.
         */
        event.eventInfo = kEventInfo_DualCore;
#else
        event.eventInfo = kEventInfo_Local;
#endif /* ENABLE_CAMERA_DEV_FlexioGc0308_REMOTE_CSC */

        dev->cap.callback(dev, event, dev->cap.param, fromISR);
    }
}

static hal_camera_status_t _HAL_CameraDev_FlexioGc0308_Init(camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    camera_config_t cameraConfig;
    LOGD("HAL_CameraDev_FlexioGc0308_Init");

    // init flexio receiver
    memset(&cameraConfig, 0, sizeof(cameraConfig));
    cameraConfig.pixelFormat                = kVIDEO_PixelFormatYUYV;
    cameraConfig.bytesPerPixel              = CAMERA_DEV_FlexioGc0308_BPP;
    cameraConfig.resolution                 = FSL_VIDEO_RESOLUTION(dev->config.width, dev->config.height);
    cameraConfig.frameBufferLinePitch_Bytes = dev->config.width * CAMERA_DEV_FlexioGc0308_BPP;
    cameraConfig.interface                  = kCAMERA_InterfaceGatedClock;
    cameraConfig.controlFlags               = CAMERA_RGB_CONTROL_FLAGS;
    cameraConfig.framePerSec                = 15;

    _HAL_CameraDev_InitResources();

    _HAL_CameraDev_InitInterface();

    GPIO_ClearPinsInterruptFlags(FLEXIO_CAMERA_VSYNC_GPIO_BASE, 0xffffffff);

    // init camera receiver
    CAMERA_RECEIVER_Init(&s_CameraReceiver, &cameraConfig, HAL_CameraDev_FlexioGc0308_ReceiverCallback, dev);

    // init camera device
    if (kStatus_Success != CAMERA_DEVICE_Init(&s_CameraDevice, &cameraConfig))
    {
        LOGE("CAMERA_DEVICE_Init!!!!");
        ret = kStatus_HAL_CameraError;
        // assert(0);
    }

    if (CAMERA_DEV_FlexioGc0308_FORMAT == kPixelFormat_UYVY1P422_RGB)
    {
#if defined(ENFORCE_FLEXIO_CAMERA_AS_IR) && ENFORCE_FLEXIO_CAMERA_AS_IR
        CAMERA_DEVICE_Control(&s_CameraDevice, kCAMERA_DeviceMonoMode, CAMERA_MONO_MODE_ENABLED);
#else
        CAMERA_DEVICE_Control(&s_CameraDevice, kCAMERA_DeviceMonoMode, CAMERA_MONO_MODE_DISABLED);
#endif
    }
    else
    {
        CAMERA_DEVICE_Control(&s_CameraDevice, kCAMERA_DeviceMonoMode, CAMERA_MONO_MODE_ENABLED);
    }

    CAMERA_DEVICE_Control(&s_CameraDevice, kCAMERA_DeviceExposureMode, s_CurRGBExposureMode);

    for (int i = 0; i < CAMERA_DEV_FlexioGc0308_BUFFER_COUNT; i++)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&s_CameraReceiver, (uint32_t)s_FrameBuffers[i]);
    }

    return ret;
}

static void _HAL_CameraDev_FlexioGc0308_InitTask(void *pvParameters)
{
    camera_dev_t *dev = (camera_dev_t *)pvParameters;
    hal_camera_status_t ret;
    ret = _HAL_CameraDev_FlexioGc0308_Init(dev);

    if (dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        camera_event_t event;
        event.eventId   = kCameraEvent_CameraDeviceInit;
        event.eventInfo = kEventInfo_Local;
        dev->cap.callback(dev, event, &ret, fromISR);
    }
    vTaskDelete(NULL);
}

hal_camera_status_t HAL_CameraDev_FlexioGc0308_Init(
    camera_dev_t *dev, int width, int height, camera_dev_callback_t callback, void *param)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    UBaseType_t priority    = uxTaskPriorityGet(NULL);
    LOGD("camera_dev_flexio_gc0308_init");

    dev->config.width  = width;
    dev->config.height = height;
    dev->cap.callback  = callback;
    dev->cap.param     = param;

    /* inherit calling task priority */

#if FWK_SUPPORT_ASYNC_CAMERA_INIT
    if (xTaskCreate(_HAL_CameraDev_FlexioGc0308_InitTask, "InitFlexioTask", 512, dev, priority, NULL) != pdPASS)
    {
        LOGE("Init camera_dev_flexio_gc0308_init");
        ret = kStatus_HAL_CameraError;
    }
    else
    {
        ret = kStatus_HAL_CameraNonBlocking;
    }
#else
    ret = _HAL_CameraDev_FlexioGc0308_Init(dev);
#endif
    return ret;
}

static hal_camera_status_t HAL_CameraDev_FlexioGc0308_Deinit(camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    return ret;
}

static hal_camera_status_t HAL_CameraDev_FlexioGc0308_Start(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    LOGD("HAL_CameraDev_FlexioGc0308_Start");
    CAMERA_DEVICE_Start(&s_CameraDevice);
    CAMERA_RECEIVER_Start(&s_CameraReceiver);

    return ret;
}

static hal_camera_status_t HAL_CameraDev_FlexioGc0308_Enqueue(const camera_dev_t *dev, void *data)
{
    LOGI("++HAL_CameraDev_FlexioGc0308_Enqueue");

    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;

    if (s_pCurrentFrameBuffer != NULL)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&s_CameraReceiver, (uint32_t)s_pCurrentFrameBuffer);
        s_pCurrentFrameBuffer = NULL;
    }

    LOGI("--HAL_CameraDev_FlexioGc0308_Enqueue");
    return ret;
}

static hal_camera_status_t HAL_CameraDev_FlexioGc0308_Dequeue(const camera_dev_t *dev,
                                                              void **data,
                                                              pixel_format_t *format)
{
    LOGI("++HAL_CameraDev_FlexioGc0308_Dequeue");

    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    int error;
    error = CAMERA_RECEIVER_GetFullBuffer(&s_CameraReceiver, (uint32_t *)&s_pCurrentFrameBuffer);
    if (kStatus_Success == error)
    {
        *data   = s_pCurrentFrameBuffer;
        *format = CAMERA_DEV_FlexioGc0308_FORMAT;
    }
    else
    {
        ret = kStatus_HAL_CameraError;
    }
    LOGI("--HAL_CameraDev_FlexioGc0308_Dequeue");
    return ret;
}

static hal_camera_status_t HAL_CameraDev_FlexioGc0308_Notify(const camera_dev_t *dev, void *data)
{
    LOGI("++HAL_CameraDev_FlexioGc0308_Notify");
    int error = 0;

    event_base_t eventBase = *(event_base_t *)data;
    switch (eventBase.eventId)
    {
        case kEventID_ControlRGBCamExposure:
        {
            event_common_t event = *(event_common_t *)data;
            uint8_t mode;
            if (event.brightnessControl.enable == true)
            {
                if (event.brightnessControl.direction == 1)
                {
                    CAMERA_DEVICE_Control(&s_CameraDevice, kCAMERA_DeviceBrightnessAdjust, CAMERA_BRIGHTNESS_INCREASE);
                }
                else if (event.brightnessControl.direction == 0)
                {
                    CAMERA_DEVICE_Control(&s_CameraDevice, kCAMERA_DeviceBrightnessAdjust, CAMERA_BRIGHTNESS_DECREASE);
                }
            }
            else
            {
                CAMERA_DEVICE_Control(&s_CameraDevice, kCAMERA_DeviceBrightnessAdjust, CAMERA_BRIGHTNESS_DEFAULT);
            }
        }
        break;
        default:
            break;
    }

    LOGI("--HAL_CameraDev_FlexioGc0308_Notify");

    return error;
}

const static camera_dev_operator_t s_CameraDev_FlexioGc0308Ops = {
    .init        = HAL_CameraDev_FlexioGc0308_Init,
    .deinit      = HAL_CameraDev_FlexioGc0308_Deinit,
    .start       = HAL_CameraDev_FlexioGc0308_Start,
    .enqueue     = HAL_CameraDev_FlexioGc0308_Enqueue,
    .dequeue     = HAL_CameraDev_FlexioGc0308_Dequeue,
    .inputNotify = HAL_CameraDev_FlexioGc0308_Notify,
};

static camera_dev_t s_CameraDev_FlexioGc0308 = {
    .id   = 0,
    .name = CAMERA_NAME,
    .ops  = &s_CameraDev_FlexioGc0308Ops,
    .config =
        {
            .height   = CAMERA_DEV_FlexioGc0308_HEIGHT,
            .width    = CAMERA_DEV_FlexioGc0308_WIDTH,
            .pitch    = CAMERA_DEV_FlexioGc0308_WIDTH * CAMERA_DEV_FlexioGc0308_BPP,
            .left     = CAMERA_DEV_FlexioGc0308_LEFT,
            .top      = CAMERA_DEV_FlexioGc0308_TOP,
            .right    = CAMERA_DEV_FlexioGc0308_RIGHT,
            .bottom   = CAMERA_DEV_FlexioGc0308_BOTTOM,
            .rotate   = CAMERA_DEV_FlexioGc0308_ROTATE,
            .flip     = CAMERA_DEV_FlexioGc0308_FLIP,
            .swapByte = CAMERA_DEV_FlexioGc0308_SWAPBYTE,
        },
    .cap =
        {
            .callback = NULL,
            .param    = NULL,
        },
};

int HAL_CameraDev_FlexioGc0308_Register()
{
    int error = 0;
    LOGD("HAL_CameraDev_FlexioGc0308_Register");
    error = FWK_CameraManager_DeviceRegister(&s_CameraDev_FlexioGc0308);
    return error;
}
#endif /* ENABLE_CAMERA_DEV_FlexioGc0308 */
