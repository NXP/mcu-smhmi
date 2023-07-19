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

#ifdef ENABLE_DISPLAY_DEV_LVGLIMG
#include <FreeRTOS.h>
#include <queue.h>

#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_display_manager.h"
#include "hal_display_dev.h"

#include "display_support.h"
#include "task.h"
#include "pin_mux.h"
#include "board.h"
#include "lvgl.h"
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"

#define DISPLAY_NAME "LVGLIMG"

/* LCD input frame buffer is RGB565, converted by PXP. */
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_LcdBuffer[DISPLAY_DEV_LVGLIMG_BUFFER_COUNT][DISPLAY_DEV_LVGLIMG_WIDTH]
                                                        [DISPLAY_DEV_LVGLIMG_HEIGHT * DISPLAY_DEV_LVGLIMG_BPP],
                              FRAME_BUFFER_ALIGN);

lv_img_dsc_t frameImage = {
    .header.always_zero = 0,
    .header.w           = DISPLAY_DEV_LVGLIMG_WIDTH,
    .header.h           = DISPLAY_DEV_LVGLIMG_HEIGHT,
    .data_size          = DISPLAY_DEV_LVGLIMG_WIDTH * DISPLAY_DEV_LVGLIMG_HEIGHT * LV_COLOR_SIZE / 8,
    .header.cf          = LV_IMG_CF_TRUE_COLOR,
};

hal_display_status_t HAL_DisplayDev_LVGLIMG_Init(
    display_dev_t *dev, int width, int height, display_dev_callback_t callback, void *param)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("++HAL_DisplayDev_LVGLIMG_Init");

    memset(s_LcdBuffer, 0x0, sizeof(s_LcdBuffer));

    dev->cap.frameBuffer = (void *)s_LcdBuffer[0];

    LOGD("--HAL_DisplayDev_LVGLIMG_Init");
    return ret;
}

hal_display_status_t HAL_DisplayDev_LVGLIMG_Deinit(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    return ret;
}

hal_display_status_t HAL_DisplayDev_LVGLIMG_Start(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("++HAL_DisplayDev_LVGLImg_Start");

    LOGD("--HAL_DisplayDev_LVGLImg_Start");
    return ret;
}

hal_display_status_t HAL_DisplayDev_LVGLIMG_Blit(const display_dev_t *dev, void *frame, int width, int height)
{
    hal_display_status_t ret = kStatus_HAL_DisplayRequestFrame;
    LOGI("++HAL_DisplayDev_LVGLImg_Blit");
    /* Show the new frame. */
    void *lcdFrameAddr = s_LcdBuffer[0];
    frameImage.data    = (const uint8_t *)lcdFrameAddr;
    if (lv_obj_is_valid(guider_ui.home_img_cameraPreview))
    {
        lv_img_set_src(guider_ui.home_img_cameraPreview, &frameImage);
    }
    LOGI("--HAL_DisplayDev_LVGLImg_Blit");
    return ret;
}

const static display_dev_operator_t s_DisplayDev_LVGLIMGOps = {
    .init        = HAL_DisplayDev_LVGLIMG_Init,
    .deinit      = HAL_DisplayDev_LVGLIMG_Deinit,
    .start       = HAL_DisplayDev_LVGLIMG_Start,
    .blit        = HAL_DisplayDev_LVGLIMG_Blit,
    .inputNotify = NULL,
};

static display_dev_t s_DisplayDev_LVGLIMG = {.id   = 0,
                                             .name = DISPLAY_NAME,
                                             .ops  = &s_DisplayDev_LVGLIMGOps,
                                             .cap  = {.width       = DISPLAY_DEV_LVGLIMG_WIDTH,
                                                     .height      = DISPLAY_DEV_LVGLIMG_HEIGHT,
                                                     .pitch       = DISPLAY_DEV_LVGLIMG_WIDTH * DISPLAY_DEV_LVGLIMG_BPP,
                                                     .left        = DISPLAY_DEV_LVGLIMG_LEFT,
                                                     .top         = DISPLAY_DEV_LVGLIMG_TOP,
                                                     .right       = DISPLAY_DEV_LVGLIMG_RIGHT,
                                                     .bottom      = DISPLAY_DEV_LVGLIMG_BOTTOM,
                                                     .rotate      = DISPLAY_DEV_LVGLIMG_ROTATE,
                                                     .format      = DISPLAY_DEV_LVGLIMG_FORMAT,
                                                     .srcFormat   = DISPLAY_DEV_LVGLIMG_SRCFORMAT,
                                                     .frameBuffer = NULL,
                                                     .callback    = NULL,
                                                     .param       = NULL}};

int HAL_DisplayDev_LVGLIMG_Register()
{
    int ret = 0;
    LOGD("HAL_DisplayDev_LVGL_Register");
    ret = FWK_DisplayManager_DeviceRegister(&s_DisplayDev_LVGLIMG);
    return ret;
}

#endif /* ENABLE_DISPLAY_DEV_LVGLIMG */
