/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief display dev HAL driver implementation for LVGL imge widget.
 */

#include "board_define.h"

#ifdef ENABLE_DISPLAY_DEV_LVGLElevator

#include <FreeRTOS.h>
#include <queue.h>

#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_display_manager.h"
#include "fwk_lpm_manager.h"
#include "hal_display_dev.h"
#include "app_config.h"
#include "smart_tlhmi_event_descriptor.h"

#include "display_support.h"
#include "task.h"
#include "pin_mux.h"
#include "board.h"
#include "lvgl.h"
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"

#include "lvgl_support.h"

#define DISPLAY_NAME         "LVGLElevator"
#define LVGL_TASK_PRIORITY   (configMAX_PRIORITIES - 1)
#define LVGL_TASK_STACK_SIZE 1024

#if LVGL_MULTITHREAD_LOCK
#define LVGL_LOCK()   _takeLVGLMutex()
#define LVGL_UNLOCK() _giveLVGLMutex()
#else
#define LVGL_LOCK()
#define LVGL_UNLOCK()
#endif /* LVGL_MULTITHREAD_LOCK */

/* LCD input frame buffer is RGB565, converted by PXP. */
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_LcdBuffer[DISPLAY_DEV_LVGLElevator_BUFFER_COUNT][DISPLAY_DEV_LVGLElevator_WIDTH]
                              [DISPLAY_DEV_LVGLElevator_HEIGHT * DISPLAY_DEV_LVGLElevator_BPP],
    FRAME_BUFFER_ALIGN);
volatile bool g_LvglInitialized = false;
lv_ui guider_ui;

extern preview_mode_t g_PreviewMode;

#if LV_USE_LOG
static void _PrintCb(const char *buf)
{
    LOGD("%s", buf);
}
#endif /* LV_USE_LOG */

static void _LvglTask(void *param)
{
#if LV_USE_LOG
    lv_log_register_print_cb(_PrintCb);
#endif /* LV_USE_LOG */

    lv_port_pre_init();
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    g_LvglInitialized = true;

    setup_imgs((unsigned char *)APP_LVGL_IMGS_BASE);
#if AQT_TEST
    gui_set_home();
#else
    setup_ui(&guider_ui);
#endif /* AQT_TEST */
    events_init(&guider_ui);
    custom_init(&guider_ui);
    while (1)
    {
        LVGL_LOCK();
        lv_task_handler();
        LVGL_UNLOCK();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

hal_display_status_t HAL_DisplayDev_LVGLElevator_Init(
    display_dev_t *dev, int width, int height, display_dev_callback_t callback, void *param)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("++HAL_DisplayDev_LVGLElevator_Init");

    memset(s_LcdBuffer, 0x0, sizeof(s_LcdBuffer));

    dev->cap.frameBuffer = (void *)s_LcdBuffer[0];

    BaseType_t stat = xTaskCreate(_LvglTask, "LVGL", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, NULL);

    if (pdPASS != stat)
    {
        LOGE("Failed to create LVGL task");
        while (1)
            ;
    }

    LOGD("--HAL_DisplayDev_LVGLElevator_Init");
    return ret;
}

hal_display_status_t HAL_DisplayDev_LVGLElevator_Deinit(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("++HAL_DisplayDev_LVGLElevator_Deinit");

    LOGD("--HAL_DisplayDev_LVGLElevator_Deinit");
    return ret;
}

hal_display_status_t HAL_DisplayDev_LVGLElevator_Start(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("++HAL_DisplayDev_LVGLElevator_Start");

    LOGD("--HAL_DisplayDev_LVGLElevator_Start");
    return ret;
}

hal_display_status_t HAL_DisplayDev_LVGLElevator_Blit(const display_dev_t *dev, void *frame, int width, int height)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGI("++HAL_DisplayDev_LVGLElevator_Blit");

    LOGI("--HAL_DisplayDev_LVGLElevator_Blit");
    return ret;
}

const static display_dev_operator_t s_DisplayDev_LVGLElevatorOps = {
    .init        = HAL_DisplayDev_LVGLElevator_Init,
    .deinit      = HAL_DisplayDev_LVGLElevator_Deinit,
    .start       = HAL_DisplayDev_LVGLElevator_Start,
    .blit        = HAL_DisplayDev_LVGLElevator_Blit,
    .inputNotify = NULL,
};

static display_dev_t s_DisplayDev_LVGLElevator = {
    .id   = 0,
    .name = DISPLAY_NAME,
    .ops  = &s_DisplayDev_LVGLElevatorOps,
    .cap  = {.width       = DISPLAY_DEV_LVGLElevator_WIDTH,
            .height      = DISPLAY_DEV_LVGLElevator_HEIGHT,
            .pitch       = DISPLAY_DEV_LVGLElevator_WIDTH * DISPLAY_DEV_LVGLElevator_BPP,
            .left        = DISPLAY_DEV_LVGLElevator_LEFT,
            .top         = DISPLAY_DEV_LVGLElevator_TOP,
            .right       = DISPLAY_DEV_LVGLElevator_RIGHT,
            .bottom      = DISPLAY_DEV_LVGLElevator_BOTTOM,
            .rotate      = DISPLAY_DEV_LVGLElevator_ROTATE,
            .format      = DISPLAY_DEV_LVGLElevator_FORMAT,
            .srcFormat   = DISPLAY_DEV_LVGLElevator_SRCFORMAT,
            .frameBuffer = NULL,
            .callback    = NULL,
            .param       = NULL}};

static hal_lpm_request_t s_LpmReq = {.dev = &s_DisplayDev_LVGLElevator, .name = "LVGLElevator"};

int HAL_DisplayDev_LVGLElevator_Register()
{
    int ret = 0;
    LOGD("++HAL_DisplayDev_LVGLElevator_Register");

    ret = FWK_DisplayManager_DeviceRegister(&s_DisplayDev_LVGLElevator);

    FWK_LpmManager_RegisterRequestHandler(&s_LpmReq);
    LOGD("--HAL_DisplayDev_LVGLElevator_Register");
    return ret;
}

#endif /* ENABLE_DISPLAY_DEV_LVGLElevator */
