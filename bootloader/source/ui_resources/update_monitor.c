/*
 * Copyright 2023 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board_define.h"

#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <stdio.h>

#include "display_support.h"
#include "graphics_util.h"
#include "update_monitor.h"

#include "nxp_logo_240x86.h"

#include "fsl_pxp.h"

typedef struct _update_monitor
{
    updateStage stage;
    uint8_t status;
} update_monitor_t;

#define PXP_DEV PXP

/* LCD input frame buffer is RGB565, converted by PXP. */
#define DISPLAY_BUFFER_BPP     2
#define DISPLAY_BUFFER_COUNT   2
#define DISPLAY_BUFFER_WIDTH   DEMO_BUFFER_WIDTH
#define DISPLAY_BUFFER_HEIGHT  DEMO_BUFFER_HEIGHT
#define DISPLAY_BUFFER_START_X 0
#define DISPLAY_BUFFER_START_Y 0

#define DISPLAY_STACK_SIZE    1024
#define DISPLAY_TASK_PRIORITY configMAX_PRIORITIES - 1
#define DISPLAY_QUEUE_LENGTH  5

#define DISPLAY_ROTATED_BUFFER   1
#define DISPLAY_GENERATED_BUFFER 0

#define NXPLOGO_HEIGHT_RESCALED (NXP_LOGO_H * 1280 / 720 * 1280 / 720)

#define DISPLAY_NXPLOGO_X_START 240
#define DISPLAY_NXPLOGO_Y_START 130

#define DISPLAY_VERTICAL_START (DISPLAY_NXPLOGO_Y_START + NXPLOGO_HEIGHT_RESCALED + 70)
#define DISPLAY_VERTICAL_END   900

#define DISPLAY_LOADING_BAR_Y_POSITION  (DISPLAY_VERTICAL_START + 100)
#define DISPLAY_LOADING_BAR_X_POSITION  75
#define DISPLAY_LOADING_BAR_WIDTH       (DISPLAY_BUFFER_WIDTH - 2 * DISPLAY_LOADING_BAR_X_POSITION)
#define DISPLAY_LOADING_BAR_HEIGHT      30
#define DISPLAY_LOADING_BAR_GRANULARITY (((float)DISPLAY_LOADING_BAR_WIDTH / 100))

#define DISPLAY_TEXT_VERTICAL_START (DISPLAY_VERTICAL_START + 300)

#define COLOR_RGB565_RED   0xF800
#define COLOR_RGB565_GREEN 0x7E0

#define DISPLAY_WAITING_TEXT        "Drag and drop binary"
#define DISPLAY_WAITING_POWER_TEXT  "Do not power off. The board will automatically reset"
#define DISPLAY_DOWNLOAD_TEXT       "Downloading binary..."
#define DISPLAY_PROCESSING_TEXT     "Merging files..."
#define DISPLAY_FINISH_SUCCESS_TEXT "Updated successfully. Board will reset!"
#define DISPLAY_FINISH_ERROR_TEXT   "Updated failed!"

static QueueHandle_t s_queueHandle = NULL;
static dc_fb_info_t s_fbInfo       = {0};
static uint8_t s_newFrameShown     = false;
static gfx_surface_t surface       = {0};

AT_NONCACHEABLE_SECTION_ALIGN_SDRAM(
    static uint8_t s_LcdBuffer[DISPLAY_BUFFER_COUNT][DISPLAY_BUFFER_HEIGHT][DISPLAY_BUFFER_WIDTH * DISPLAY_BUFFER_BPP],
    FRAME_BUFFER_ALIGN);

AT_NONCACHEABLE_SECTION_ALIGN_SDRAM(
    static uint8_t s_NXPLogoRescaled[1][NXP_LOGO_W][NXPLOGO_HEIGHT_RESCALED * DISPLAY_BUFFER_BPP], FRAME_BUFFER_ALIGN);

static void Display_DrawLoadingBar(updateStage stage)
{
    char text[50] = {0};

    if (stage == UPDATE_DOWNLOAD)
    {
        sprintf(text, DISPLAY_DOWNLOAD_TEXT);
    }
    else if (stage == UPDATE_PROCESSING)
    {
        sprintf(text, DISPLAY_PROCESSING_TEXT);
    }

    uint16_t StartPosition_X = (DISPLAY_BUFFER_WIDTH - get_stringwidth(text, kFont_OpenSans36)) / 2;
    Graphic_DrawText(&surface, StartPosition_X, DISPLAY_TEXT_VERTICAL_START, 0xFFFF, 0x0, kFont_OpenSans36, text);

    Graphic_DrawRect(&surface, DISPLAY_LOADING_BAR_X_POSITION, DISPLAY_LOADING_BAR_Y_POSITION,
                     DISPLAY_LOADING_BAR_WIDTH, DISPLAY_LOADING_BAR_HEIGHT, 0xFFFF);
    Graphic_DrawRect(&surface, DISPLAY_LOADING_BAR_X_POSITION + 2, DISPLAY_LOADING_BAR_Y_POSITION + 2,
                     DISPLAY_LOADING_BAR_WIDTH - 4, DISPLAY_LOADING_BAR_HEIGHT - 4, 0x0);
}

static void Display_ClearTextArea()
{
    uint32_t size = (DISPLAY_VERTICAL_END - DISPLAY_VERTICAL_START) * DISPLAY_BUFFER_WIDTH * DISPLAY_BUFFER_BPP;
    memset((char *)s_LcdBuffer[DISPLAY_GENERATED_BUFFER][DISPLAY_VERTICAL_START], 0x00, size);
}

static void Display_PXP_Init()
{
    PXP_Init(PXP_DEV);

    PXP_SetProcessSurfaceBackGroundColor(PXP_DEV, 0U);
    // disable the AS
    PXP_SetAlphaSurfacePosition(PXP_DEV, 0xFFFFU, 0xFFFFU, 0U, 0U);
    PXP_EnableCsc1(PXP_DEV, false);
}

static void Display_Rescale(
    const char *in_buffer, char *out_buffer, uint32_t inHeight, uint32_t inWidth, uint32_t outHeight, uint32_t outWidth)
{
    pxp_ps_buffer_config_t psBufferConfig;
    pxp_output_buffer_config_t outputBufferConfig;

    psBufferConfig.pixelFormat = kPXP_PsPixelFormatRGB565;
    psBufferConfig.swapByte    = false;
    psBufferConfig.bufferAddrU = 0;
    psBufferConfig.bufferAddrV = 0;
    psBufferConfig.pitchBytes  = inWidth * DISPLAY_BUFFER_BPP;
    psBufferConfig.bufferAddr  = (uint32_t)in_buffer;

    PXP_SetProcessSurfaceBufferConfig(PXP_DEV, &psBufferConfig);

    PXP_SetProcessSurfacePosition(PXP_DEV, 0, 0, outWidth - 1, outWidth - 1);
    PXP_SetProcessSurfaceScaler(PXP_DEV, inWidth, inHeight, outWidth, outHeight);

    PXP_SetRotateConfig(PXP_DEV, kPXP_RotateOutputBuffer, kPXP_Rotate0, kPXP_FlipDisable);

    outputBufferConfig.pixelFormat    = kPXP_OutputPixelFormatRGB565;
    outputBufferConfig.interlacedMode = kPXP_OutputProgressive;
    outputBufferConfig.buffer1Addr    = 0U;
    outputBufferConfig.pitchBytes     = outWidth * DISPLAY_BUFFER_BPP;
    outputBufferConfig.width          = outWidth;
    outputBufferConfig.height         = outHeight;

    outputBufferConfig.buffer0Addr = (uint32_t)out_buffer;
    PXP_SetOutputBufferConfig(PXP_DEV, &outputBufferConfig);

    PXP_Start(PXP_DEV);
    while ((kPXP_CompleteFlag & PXP_GetStatusFlags(PXP_DEV)) == 0)
    {
    }
    PXP_ClearStatusFlags(PXP_DEV, kPXP_CompleteFlag);
}

static void Display_Rotate270(const char *in_buffer, char *out_buffer)
{
    pxp_ps_buffer_config_t psBufferConfig;
    pxp_output_buffer_config_t outputBufferConfig;

    psBufferConfig.pixelFormat = kPXP_PsPixelFormatRGB565;
    psBufferConfig.swapByte    = false;
    psBufferConfig.bufferAddrU = 0;
    psBufferConfig.bufferAddrV = 0;
    psBufferConfig.pitchBytes  = DISPLAY_BUFFER_WIDTH * DISPLAY_BUFFER_BPP;
    psBufferConfig.bufferAddr  = (uint32_t)in_buffer;

    PXP_SetProcessSurfaceBackGroundColor(PXP_DEV, 0U);
    PXP_SetProcessSurfaceBufferConfig(PXP_DEV, &psBufferConfig);

    PXP_SetRotateConfig(PXP_DEV, kPXP_RotateOutputBuffer, kPXP_Rotate270, kPXP_FlipDisable);

    PXP_SetProcessSurfacePosition(PXP_DEV, 0, 0, DISPLAY_BUFFER_HEIGHT - 1, DISPLAY_BUFFER_WIDTH - 1);
    PXP_SetProcessSurfaceScaler(PXP_DEV, DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT, DISPLAY_BUFFER_HEIGHT,
                                DISPLAY_BUFFER_WIDTH);

    outputBufferConfig.pixelFormat    = kPXP_OutputPixelFormatRGB565;
    outputBufferConfig.interlacedMode = kPXP_OutputProgressive;
    outputBufferConfig.buffer1Addr    = 0U;
    outputBufferConfig.pitchBytes     = DISPLAY_BUFFER_WIDTH * DISPLAY_BUFFER_BPP;
    outputBufferConfig.width          = DISPLAY_BUFFER_HEIGHT;
    outputBufferConfig.height         = DISPLAY_BUFFER_WIDTH;

    outputBufferConfig.buffer0Addr = (uint32_t)out_buffer;
    PXP_SetOutputBufferConfig(PXP_DEV, &outputBufferConfig);

    PXP_Start(PXP_DEV);
    while ((kPXP_CompleteFlag & PXP_GetStatusFlags(PXP_DEV)) == 0)
    {
    }
    PXP_ClearStatusFlags(PXP_DEV, kPXP_CompleteFlag);
}

static void DISPLAY_BufferSwitchOffCallback(void *param, void *switchOffBuffer)
{
    s_newFrameShown = true;
}

static void SLN_UpdateMonitor_Waiting()
{
    configPRINTF(("Waiting"));

    Display_ClearTextArea();

    uint16_t StartPosition_X = (DISPLAY_BUFFER_WIDTH - get_stringwidth(DISPLAY_WAITING_TEXT, kFont_OpenSans36)) / 2;
    uint16_t StartPosition_Y = DISPLAY_VERTICAL_START;

    Graphic_DrawText(&surface, StartPosition_X, StartPosition_Y, 0xFFFF, 0x0, kFont_OpenSans36, DISPLAY_WAITING_TEXT);

    StartPosition_X = (DISPLAY_BUFFER_WIDTH - get_stringwidth(DISPLAY_WAITING_POWER_TEXT, kFont_OpenSans20)) / 2;
    StartPosition_Y = DISPLAY_LOADING_BAR_Y_POSITION;

    Graphic_DrawText(&surface, StartPosition_X, StartPosition_Y, 0xFFFF, 0x0, kFont_OpenSans20,
                     DISPLAY_WAITING_POWER_TEXT);

    Display_Rotate270((char *)s_LcdBuffer[DISPLAY_GENERATED_BUFFER], (char *)s_LcdBuffer[DISPLAY_ROTATED_BUFFER]);
    g_dc.ops->setFrameBuffer(&g_dc, 0, s_LcdBuffer[DISPLAY_ROTATED_BUFFER]);
}

static void SLN_UpdateMonitor_Finish(uint8_t status)
{
    configPRINTF(("FINISH status %d", status));

    char text[50]  = {0};
    uint16_t color = 0x0;

    if (status == 0)
    {
        sprintf(text, DISPLAY_FINISH_SUCCESS_TEXT);
        color = COLOR_RGB565_GREEN;
    }
    else
    {
        sprintf(text, DISPLAY_FINISH_ERROR_TEXT);
        color = COLOR_RGB565_RED;
    }

    uint16_t StartPosition_X = (DISPLAY_BUFFER_WIDTH - get_stringwidth(text, kFont_OpenSans36)) / 2;
    uint16_t StartPosition_Y = DISPLAY_VERTICAL_START;

    Graphic_DrawText(&surface, StartPosition_X, StartPosition_Y, color, 0x0, kFont_OpenSans36, text);

    Display_Rotate270((char *)s_LcdBuffer[DISPLAY_GENERATED_BUFFER], (char *)s_LcdBuffer[DISPLAY_ROTATED_BUFFER]);
    g_dc.ops->setFrameBuffer(&g_dc, 0, s_LcdBuffer[DISPLAY_ROTATED_BUFFER]);
}

static void SLN_UpdateMonitor_Download(uint8_t status)
{
    configPRINTF(("Download status %d", status));

    if (status)
    {
        Graphic_DrawRect(&surface, DISPLAY_LOADING_BAR_X_POSITION + 2, DISPLAY_LOADING_BAR_Y_POSITION + 2,
                         DISPLAY_LOADING_BAR_GRANULARITY * status - 4, DISPLAY_LOADING_BAR_HEIGHT - 4, 0xFFFF);
    }

    Display_Rotate270((char *)s_LcdBuffer[DISPLAY_GENERATED_BUFFER], (char *)s_LcdBuffer[DISPLAY_ROTATED_BUFFER]);
    g_dc.ops->setFrameBuffer(&g_dc, 0, s_LcdBuffer[DISPLAY_ROTATED_BUFFER]);
}

static void SLN_UpdateMonitor_Processing(uint8_t status)
{
    configPRINTF(("Processing status %d", status));

    if (status)
    {
        Graphic_DrawRect(&surface, DISPLAY_LOADING_BAR_X_POSITION + 2, DISPLAY_LOADING_BAR_Y_POSITION + 2,
                         DISPLAY_LOADING_BAR_GRANULARITY * status - 4, DISPLAY_LOADING_BAR_HEIGHT - 4, 0xFFFF);
    }

    Display_Rotate270((char *)s_LcdBuffer[DISPLAY_GENERATED_BUFFER], (char *)s_LcdBuffer[DISPLAY_ROTATED_BUFFER]);
    g_dc.ops->setFrameBuffer(&g_dc, 0, s_LcdBuffer[DISPLAY_ROTATED_BUFFER]);
}

static void _DisplayTask(void *pvParameters)
{
    status_t status         = kStatus_Success;
    update_monitor_t update = {0};
    updateStage lastStage   = UPDATE_INVALID;

    BOARD_PrepareDisplayController();
    status = g_dc.ops->init(&g_dc);

    if (kStatus_Success != status)
    {
        configPRINTF(("Display initialization failed"));
        assert(0);
    }

    g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &s_fbInfo);
    s_fbInfo.pixelFormat = kVIDEO_PixelFormatRGB565;
    s_fbInfo.width       = DISPLAY_BUFFER_WIDTH;
    s_fbInfo.height      = DISPLAY_BUFFER_HEIGHT;
    s_fbInfo.startX      = DISPLAY_BUFFER_START_X;
    s_fbInfo.startY      = DISPLAY_BUFFER_START_Y;
    s_fbInfo.strideBytes = DISPLAY_BUFFER_WIDTH * DISPLAY_BUFFER_BPP;
    g_dc.ops->setLayerConfig(&g_dc, 0, &s_fbInfo);
    g_dc.ops->setCallback(&g_dc, 0, DISPLAY_BufferSwitchOffCallback, NULL);

    surface.buf    = s_LcdBuffer[DISPLAY_GENERATED_BUFFER];
    surface.height = DISPLAY_BUFFER_HEIGHT;
    surface.width  = DISPLAY_BUFFER_WIDTH;
    surface.pitch  = DISPLAY_BUFFER_WIDTH * DISPLAY_BUFFER_BPP;

    Display_Rescale(nxp_logo_240x86, (char *)s_NXPLogoRescaled, NXP_LOGO_H, NXP_LOGO_W, NXPLOGO_HEIGHT_RESCALED, NXP_LOGO_W);

    Graphic_DrawPicture(&surface, DISPLAY_NXPLOGO_X_START, DISPLAY_NXPLOGO_Y_START, NXP_LOGO_W, NXPLOGO_HEIGHT_RESCALED,
                        0xFFFF, (char *)s_NXPLogoRescaled);

    Display_Rotate270((char *)s_LcdBuffer[DISPLAY_GENERATED_BUFFER], (char *)s_LcdBuffer[DISPLAY_ROTATED_BUFFER]);

    g_dc.ops->setFrameBuffer(&g_dc, 0, s_LcdBuffer[DISPLAY_ROTATED_BUFFER]);

    /* For the DBI interface display, application must wait for the first
     * frame buffer sent to the panel.
     */
    if ((g_dc.ops->getProperty(&g_dc) & kDC_FB_ReserveFrameBuffer) == 0)
    {
        while (s_newFrameShown == false)
        {
        }
    }

    s_newFrameShown = true;

    g_dc.ops->enableLayer(&g_dc, 0);

    while (1)
    {
        xQueueReceive(s_queueHandle, &update, portMAX_DELAY);

        if (lastStage != update.stage)
        {
            Display_ClearTextArea();

            if ((update.stage != UPDATE_WAITING) && (update.stage != UPDATE_FINISH))
            {
                Display_DrawLoadingBar(update.stage);
            }

            lastStage = update.stage;
        }

        if (update.stage == UPDATE_WAITING)
        {
            SLN_UpdateMonitor_Waiting();
        }
        else if (update.stage == UPDATE_FINISH)
        {
            SLN_UpdateMonitor_Finish(update.status);
            break;
        }
        else if (update.stage == UPDATE_DOWNLOAD)
        {
            SLN_UpdateMonitor_Download(update.status);
        }
        else if (update.stage == UPDATE_PROCESSING)
        {
            SLN_UpdateMonitor_Processing(update.status);
        }
    }

    vQueueDelete(s_queueHandle);
    s_queueHandle = NULL;
    vTaskDelete(NULL);
}

void SLN_UpdateMonitor_Callback(updateStage stage, uint8_t status)
{
    if (s_queueHandle)
    {
        update_monitor_t update;
        update.stage  = stage;
        update.status = status;
        xQueueSend(s_queueHandle, &update, (TickType_t)0);
    }
}

void SLN_UpdateMonitor_Init()
{
    Display_PXP_Init();

    s_queueHandle = xQueueCreate(DISPLAY_QUEUE_LENGTH, sizeof(update_monitor_t));
    if (s_queueHandle == NULL)
    {
        configPRINTF(("Failed to create display queue"));
        goto error;
    }

    BaseType_t status =
        xTaskCreate(_DisplayTask, "DisplayTask", DISPLAY_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL);
    if (status != pdPASS)
    {
        configPRINTF(("Failed to create display task"));
        goto error;
    }

    return;

error:
    while (1)
        ;
}
