/*
 * Copyright 2021-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief mipi_gc2145 camera module HAL camera driver implementation.
 */

#include "board_define.h"
#ifdef ENABLE_CAMERA_DEV_MipiGc2145
#include <FreeRTOS.h>
#include <task.h>
#include <stdlib.h>

#include "board.h"
#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "fsl_camera_device.h"
#include "fsl_gpio.h"
#include "fsl_csi.h"
#include "fsl_csi_camera_adapter.h"
#include "sln_gc2145.h"
#include "fsl_mipi_csi2rx.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "display_support.h"

#include "fwk_log.h"
#include "fwk_camera_manager.h"
#include "hal_camera_dev.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif
void BOARD_MIPICameraI2CInit(void);
status_t BOARD_MIPICameraI2CSend(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_MIPICameraI2CReceive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
void BOARD_MIPICameraPullPowerDownPin(bool pullUp);
void BOARD_MIPICameraPullResetPin(bool pullUp);
#if defined(__cplusplus)
}
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CAMERA_NAME             "MipiGc2145"
#define CAMERA_DEV_BUFFER_ALIGN 64

#define CAMERA_DEV_FRAME_RATE    15
#define CAMERA_DEV_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)
#define CAMERA_MIPI_CSI_LANE     GC2145MIPI_LANE_NUM

#define DEMO_CSI_CLK_FREQ          (CLOCK_GetFreqFromObs(CCM_OBS_BUS_CLK_ROOT))
#define DEMO_MIPI_CSI2_UI_CLK_FREQ (CLOCK_GetFreqFromObs(CCM_OBS_CSI2_UI_CLK_ROOT))

/*******************************************************************************
 * Variables
 ******************************************************************************/
// AT_NONCACHEABLE_SECTION_ALIGN(
SDK_ALIGN(static uint8_t s_cameraBuffer[CAMERA_DEV_MipiGc2145_BUFFER_COUNT][CAMERA_DEV_MipiGc2145_HEIGHT]
                                       [CAMERA_DEV_MipiGc2145_WIDTH * CAMERA_DEV_MipiGc2145_BPP],
          CAMERA_DEV_BUFFER_ALIGN);
static uint8_t *s_pCurrentFrameBuffer = NULL;

static csi_resource_t s_CsiResource = {
    .csiBase = CSI,
    .dataBus = kCSI_DataBus24Bit,
};

static csi_private_data_t s_CsiPrivateData;

static camera_receiver_handle_t s_CameraReceiver = {
    .resource    = &s_CsiResource,
    .ops         = &csi_ops,
    .privateData = &s_CsiPrivateData,
};

static gc2145_resource_t gc2145Resource = {
    .i2cSendFunc      = BOARD_MIPICameraI2CSend,
    .i2cReceiveFunc   = BOARD_MIPICameraI2CReceive,
    .pullResetPin     = BOARD_MIPICameraPullResetPin,
    .pullPowerDownPin = BOARD_MIPICameraPullPowerDownPin,
};

static camera_device_handle_t cameraDevice = {
    .resource = &gc2145Resource,
    .ops      = &gc2145_ops,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void _CameraReceiverCallback(camera_receiver_handle_t *handle, status_t status, void *userData)
{
    camera_dev_t *dev = (camera_dev_t *)userData;

    if (dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        camera_event_t event;
        event.eventId   = kCameraEvent_SendFrame;
        event.eventInfo = kEventInfo_Local;
        dev->cap.callback(dev, event, dev->cap.param, fromISR);
    }
}

void MipiGc2145_InitCameraResource(void)
{
    BOARD_MIPICameraI2CInit();
    clock_root_config_t rootCfg = {
        .mux = kCLOCK_CSI_ClockRoot_MuxOscRc48MDiv2,
        .div = 1,
    };

    /* Configure CSI using OSC_RC_48M_DIV2 */
    CLOCK_SetRootClock(kCLOCK_Root_Csi, &rootCfg);
}

static status_t MipiGc2145_VerifyCameraClockSource(void)
{
    status_t status;
    uint32_t srcClkFreq;
    /*
     * The MIPI CSI clk_ui, clk_esc, and core_clk are all from
     * System PLL3 (PLL_480M). Verify the clock source to ensure
     * it is ready to use.
     */
    srcClkFreq = CLOCK_GetPllFreq(kCLOCK_PllSys3);

    if (480 != (srcClkFreq / 1000000))
    {
        status = kStatus_Fail;
    }
    else
    {
        status = kStatus_Success;
    }

    return status;
}

void MipiGc2145_InitMipiCsi(void)
{
    csi2rx_config_t csi2rxConfig = {0};

    /* This clock should be equal or faster than the receive byte clock,
     * D0_HS_BYTE_CLKD, from the RX DPHY. For this board, there are two
     * data lanes, the MIPI CSI pixel format is 16-bit per pixel, the
     * max resolution supported is 720*1280@30Hz, so the MIPI CSI2 clock
     * should be faster than 720*1280*30 = 27.6MHz, choose 60MHz here.
     */
    const clock_root_config_t csi2ClockConfig = {
        .clockOff = false,
        .mux      = 5,
        .div      = 7,
    };

    /* ESC clock should be in the range of 60~80 MHz */
    const clock_root_config_t csi2EscClockConfig = {
        .clockOff = false,
        .mux      = 5,
        .div      = 8,
    };

    /* UI clock should be equal or faster than the input pixel clock.
     * The camera max resolution supported is 720*1280@30Hz, so this clock
     * should be faster than 720*1280*30 = 27.6MHz, choose 60MHz here.
     */
    const clock_root_config_t csi2UiClockConfig = {
        .clockOff = false,
        .mux      = 5,
        .div      = 7,
    };

    if (kStatus_Success != MipiGc2145_VerifyCameraClockSource())
    {
        LOGE("MIPI CSI clock source not valid");
        while (1)
        {
        }
    }

    /* MIPI CSI2 connect to CSI. */
    CLOCK_EnableClock(kCLOCK_Video_Mux);
    VIDEO_MUX->VID_MUX_CTRL.SET = (VIDEO_MUX_VID_MUX_CTRL_CSI_SEL_MASK);

    CLOCK_SetRootClock(kCLOCK_Root_Csi2, &csi2ClockConfig);
    CLOCK_SetRootClock(kCLOCK_Root_Csi2_Esc, &csi2EscClockConfig);
    CLOCK_SetRootClock(kCLOCK_Root_Csi2_Ui, &csi2UiClockConfig);

    /* The CSI clock should be faster than MIPI CSI2 clk_ui. The CSI clock
     * is bus clock.
     */
    if (DEMO_CSI_CLK_FREQ < DEMO_MIPI_CSI2_UI_CLK_FREQ)
    {
        LOGE("CSI clock should be faster than MIPI CSI2 ui clock.");
        while (1)
        {
        }
    }

    /* MIPI DPHY power on and isolation off. */
    PGMC_BPC4->BPC_POWER_CTRL |= (PGMC_BPC_BPC_POWER_CTRL_PSW_ON_SOFT_MASK | PGMC_BPC_BPC_POWER_CTRL_ISO_OFF_SOFT_MASK);

    /*
     * Initialize the MIPI CSI2
     *
     * From D-PHY specification, the T-HSSETTLE should in the range of 85ns+6*UI to 145ns+10*UI
     * UI is Unit Interval, equal to the duration of any HS state on the Clock Lane
     *
     * T-HSSETTLE = csi2rxConfig.tHsSettle_EscClk * (Tperiod of RxClkInEsc)
     *
     * csi2rxConfig.tHsSettle_EscClk setting for camera:
     *
     *    Resolution  |  frame rate  |  T_HS_SETTLE
     *  =============================================
     *     720P       |     30       |     0x12
     *  ---------------------------------------------
     *     720P       |     15       |     0x17
     *  ---------------------------------------------
     *      VGA       |     30       |     0x1F
     *  ---------------------------------------------
     *      VGA       |     15       |     0x24
     *  ---------------------------------------------
     *     QVGA       |     30       |     0x1F
     *  ---------------------------------------------
     *     QVGA       |     15       |     0x24
     *  ---------------------------------------------
     */
    static const uint32_t csi2rxHsSettle[][3] = {
        {
            kVIDEO_Resolution720P,
            30,
            0x12,
        },
        {
            kVIDEO_Resolution720P,
            15,
            0x17,
        },
        {
            kVIDEO_ResolutionVGA,
            30,
            0x1F,
        },
        {
            kVIDEO_ResolutionVGA,
            15,
            0x24,
        },
        {
            kVIDEO_ResolutionQVGA,
            30,
            0x1F,
        },
        {
            kVIDEO_ResolutionQVGA,
            15,
            0x24,
        },
    };

    csi2rxConfig.laneNum          = CAMERA_MIPI_CSI_LANE;
    csi2rxConfig.tHsSettle_EscClk = 0x10;

    for (uint8_t i = 0; i < ARRAY_SIZE(csi2rxHsSettle); i++)
    {
        if ((FSL_VIDEO_RESOLUTION(CAMERA_DEV_MipiGc2145_WIDTH, CAMERA_DEV_MipiGc2145_HEIGHT) == csi2rxHsSettle[i][0]) &&
            (csi2rxHsSettle[i][1] == CAMERA_DEV_FRAME_RATE))
        {
            // csi2rxConfig.tHsSettle_EscClk = csi2rxHsSettle[i][2];
            break;
        }
    }

    CSI2RX_Init(MIPI_CSI2RX, &csi2rxConfig);
}

static hal_camera_status_t HAL_CameraDev_MipiGc2145_Init(
    camera_dev_t *dev, int width, int height, camera_dev_callback_t callback, void *param)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    camera_config_t cameraConfig;
    LOGD("++HAL_CameraDev_MipiGc2145_Init(param[%p])", param);

    dev->config.width  = width;
    dev->config.height = height;
    dev->config.pitch  = CAMERA_DEV_MipiGc2145_WIDTH * CAMERA_DEV_MipiGc2145_BPP;

    dev->cap.callback = callback;
    dev->cap.param    = param;

    memset(&cameraConfig, 0, sizeof(cameraConfig));

    /*TODO
     * RTVZ-160 temporary fix
     */
    BOARD_PrepareDisplayController();

    MipiGc2145_InitCameraResource();

    /* CSI input data bus is 24-bit, and save as XYUV8888.. */
    cameraConfig.pixelFormat   = kVIDEO_PixelFormatXYUV;
    cameraConfig.bytesPerPixel = CAMERA_DEV_MipiGc2145_BPP;
    cameraConfig.resolution    = FSL_VIDEO_RESOLUTION(CAMERA_DEV_MipiGc2145_WIDTH, CAMERA_DEV_MipiGc2145_HEIGHT);
    cameraConfig.frameBufferLinePitch_Bytes = CAMERA_DEV_MipiGc2145_WIDTH * CAMERA_DEV_MipiGc2145_BPP;
    cameraConfig.interface                  = kCAMERA_InterfaceGatedClock;
    cameraConfig.controlFlags               = CAMERA_DEV_CONTROL_FLAGS;
    cameraConfig.framePerSec                = CAMERA_DEV_FRAME_RATE;

    NVIC_SetPriority(CSI_IRQn, configLIBRARY_LOWEST_INTERRUPT_PRIORITY - 1);
    CAMERA_RECEIVER_Init(&s_CameraReceiver, &cameraConfig, _CameraReceiverCallback, dev);

    MipiGc2145_InitMipiCsi();

    cameraConfig.pixelFormat   = kVIDEO_PixelFormatYUYV;
    cameraConfig.bytesPerPixel = 2;
    cameraConfig.resolution    = FSL_VIDEO_RESOLUTION(CAMERA_DEV_MipiGc2145_WIDTH, CAMERA_DEV_MipiGc2145_HEIGHT);
    cameraConfig.interface     = kCAMERA_InterfaceMIPI;
    cameraConfig.controlFlags  = CAMERA_DEV_CONTROL_FLAGS;
    cameraConfig.framePerSec   = CAMERA_DEV_FRAME_RATE;
    cameraConfig.csiLanes      = CAMERA_MIPI_CSI_LANE;
    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);

    CAMERA_DEVICE_Start(&cameraDevice);

    /* Submit the empty frame buffers to buffer queue. */
    for (uint32_t i = 0; i < CAMERA_DEV_MipiGc2145_BUFFER_COUNT; i++)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&s_CameraReceiver, (uint32_t)(s_cameraBuffer[i]));
    }

    LOGD("--HAL_CameraDev_MipiGc2145_Init");
    return ret;
}

static hal_camera_status_t HAL_CameraDev_MipiGc2145_Deinit(camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    return ret;
}

static hal_camera_status_t HAL_CameraDev_MipiGc2145_Start(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    LOGD("++HAL_CameraDev_MipiGc2145_Start");
    CAMERA_RECEIVER_Start(&s_CameraReceiver);
    LOGD("--HAL_CameraDev_MipiGc2145_Start");
    return ret;
}

static hal_camera_status_t HAL_CameraDev_MipiGc2145_Enqueue(const camera_dev_t *dev, void *data)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    LOGI("++HAL_CameraDev_MipiGc2145_Enqueue");

    if (s_pCurrentFrameBuffer != NULL)
    {
        LOGI("Submitting empty buffer");
        CAMERA_RECEIVER_SubmitEmptyBuffer(&s_CameraReceiver, (uint32_t)s_pCurrentFrameBuffer);
        s_pCurrentFrameBuffer = NULL;
    }

    LOGI("--HAL_CameraDev_MipiGc2145_Enqueue");
    return ret;
}

static hal_camera_status_t HAL_CameraDev_MipiGc2145_Dequeue(const camera_dev_t *dev,
                                                            void **data,
                                                            pixel_format_t *format)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    LOGI("++HAL_CameraDev_MipiGc2145_Dequeue");

    // get one frame
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&s_CameraReceiver, (uint32_t *)&s_pCurrentFrameBuffer))
    {
    }

    *data   = s_pCurrentFrameBuffer;
    *format = CAMERA_DEV_MipiGc2145_FORMAT;
    LOGI("--HAL_CameraDev_MipiGc2145_Dequeue");
    return ret;
}

const static camera_dev_operator_t camera_dev_mipi_gc2145_ops = {
    .init        = HAL_CameraDev_MipiGc2145_Init,
    .deinit      = HAL_CameraDev_MipiGc2145_Deinit,
    .start       = HAL_CameraDev_MipiGc2145_Start,
    .enqueue     = HAL_CameraDev_MipiGc2145_Enqueue,
    .dequeue     = HAL_CameraDev_MipiGc2145_Dequeue,
    .inputNotify = NULL,
};

static camera_dev_t camera_dev_mipi_gc2145 = {
    .id   = 1,
    .name = CAMERA_NAME,
    .ops  = &camera_dev_mipi_gc2145_ops,
    .config =
        {
            .height   = CAMERA_DEV_MipiGc2145_HEIGHT,
            .width    = CAMERA_DEV_MipiGc2145_WIDTH,
            .pitch    = CAMERA_DEV_MipiGc2145_WIDTH * CAMERA_DEV_MipiGc2145_BPP,
            .left     = CAMERA_DEV_MipiGc2145_LEFT,
            .top      = CAMERA_DEV_MipiGc2145_TOP,
            .right    = CAMERA_DEV_MipiGc2145_RIGHT,
            .bottom   = CAMERA_DEV_MipiGc2145_BOTTOM,
            .rotate   = CAMERA_DEV_MipiGc2145_ROTATE,
            .flip     = CAMERA_DEV_MipiGc2145_FLIP,
            .swapByte = CAMERA_DEV_MipiGc2145_SWAPBYTE,
        },
    .cap = {.callback = NULL, .param = NULL},
};

int HAL_CameraDev_MipiGc2145_Register()
{
    int error = 0;
    LOGD("HAL_CameraDev_MipiGc2145_Register");
    error = FWK_CameraManager_DeviceRegister(&camera_dev_mipi_gc2145);
    return error;
}
#endif /* ENABLE_CAMERA_DEV_MipiGc2145 */
