/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    sln_smart_tlhmi_cm4.cpp
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MIMXRT1176_cm4.h"
#include "fsl_debug_console.h"
#include "fsl_soc_src.h"
#include "mcmgr.h"

/* TODO: insert other include files here. */
#include "sln_time.h"

#include "lvgl_support.h"
#include "lvgl.h"
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"

#include "fwk_message.h"
#include "fwk_display_manager.h"
#include "fwk_camera_manager.h"
#include "fwk_input_manager.h"
#include "fwk_output_manager.h"
#include "fwk_multicore_manager.h"
#include "fwk_lpm_manager.h"
#include "fwk_log.h"
#include "app_config.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
#include "iot_logging_task.h"
#if defined(__cplusplus)
}
#endif /* __cplusplus */

/* Logging task configuration */
#define LOGGING_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)
#define LOGGING_TASK_STACK_SIZE 512
#define LOGGING_QUEUE_LENGTH    64
AT_SHMEM_SECTION_ALIGN(unsigned char rpmsg_sh_mem[SH_MEM_TOTAL_SIZE], 8);
#if defined(ENABLE_SLAVE) && ENABLE_SLAVE
AT_RES_SHMEM_SECTION_ALIGN(unsigned char res_sh_mem[RES_SHMEM_TOTAL_SIZE], 64);
#else
__attribute__((aligned(64))) unsigned char res_sh_mem[RES_SHMEM_TOTAL_SIZE];
#endif

/* TODO: insert other definitions and declarations here. */
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#if TARGET_BOARD == BOARD_SMART_TLHMI
// HAL_CAMERA_DEV_DECLARE(CsiGc0308);
// HAL_CAMERA_DEV_DECLARE(FlexioGc0308);
HAL_CAMERA_DEV_DECLARE(MipiGc2145);
// HAL_CAMERA_DEV_DECLARE(CsiMt9m114);
HAL_DISPLAY_DEV_DECLARE(LVGLElevator);
HAL_GFX_DEV_DECLARE(Pxp);
HAL_MULTICORE_DEV_DECLARE(MessageBuffer);
HAL_OUTPUT_DEV_DECLARE(RgbLed);
//HAL_LPM_DEV_DECLARE(Standby);
HAL_INPUT_DEV_DECLARE(ShellUsb);
HAL_OUTPUT_DEV_DECLARE(UiElevator);
HAL_INPUT_DEV_DECLARE(PushButtons);
#ifdef ENABLE_OUTPUT_DEV_AudioDump
HAL_OUTPUT_DEV_DECLARE(AudioDump);
#endif /* ENABLE_OUTPUT_DEV_AudioDump */
/* Add new HAL device declarations here */

#elif TARGET_BOARD == BOARD_1170EVK
HAL_CAMERA_DEV_DECLARE(MipiOv5640);
HAL_DISPLAY_DEV_DECLARE(LVGLIMG);
HAL_GFX_DEV_DECLARE(Pxp);
#endif

unsigned int FWK_CurrentTimeUs();
const char *g_coreName = "CM4";

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#define AUDIO_PROCESSING_TASK_PRIORITY    3
#define CAMERA_MANAGER_TASK_PRIORITY      2
#define DISPLAY_MANAGER_TASK_PRIORITY     2
#define VISION_ALGO_MANAGER_TASK_PRIORITY 3
#define VOICE_ALGO_MANAGER_TASK_PRIORITY  3
#define OUTPUT_MANAGER_TASK_PRIORITY      1
#define INPUT_MANAGER_TASK_PRIORITY       2
#define MULTICORE_MANAGER_TASK_PRIORITY   0 /* MUST: Multicore Manager must have the highest priority! */
/*
 * Get the current time in us.
 */
unsigned int FWK_CurrentTimeUs()
{
    unsigned int timeValue = Time_Current();
    unsigned int timeUnit  = Time_Unit();
    return (timeValue * timeUnit);
}

int APP_InitFramework(void)
{
    int ret = 0;

    //FWK_MANAGER_INIT(LpmManager, ret);
    FWK_MANAGER_INIT(CameraManager, ret);
    FWK_MANAGER_INIT(DisplayManager, ret);
#if defined(ENABLE_SLAVE) && ENABLE_SLAVE
    FWK_MANAGER_INIT(MulticoreManager, ret);
#endif /* defined(ENABLE_SLAVE) && ENABLE_SLAVE */
    FWK_MANAGER_INIT(OutputManager, ret);
    FWK_MANAGER_INIT(InputManager, ret);

    return ret;
}

#if TARGET_BOARD == BOARD_SMART_TLHMI
int APP_RegisterHalDevices(void)
{
    int ret = 0;

    HAL_GFX_DEV_REGISTER(Pxp, ret);
    HAL_DISPLAY_DEV_REGISTER(LVGLElevator, ret);
    //    HAL_CAMERA_DEV_REGISTER(CsiGc0308, ret);
    //    HAL_CAMERA_DEV_REGISTER(FlexioGc0308, ret);
    HAL_CAMERA_DEV_REGISTER(MipiGc2145, ret);
    //    HAL_CAMERA_DEV_REGISTER(CsiMt9m114, ret);
#if defined(ENABLE_SLAVE) && ENABLE_SLAVE
    HAL_MULTICORE_DEV_REGISTER(MessageBuffer, ret);
#endif /* defined(ENABLE_SLAVE) && ENABLE_SLAVE */

    HAL_OUTPUT_DEV_REGISTER(RgbLed, ret);
    HAL_INPUT_DEV_REGISTER(ShellUsb, ret);
    HAL_INPUT_DEV_REGISTER(PushButtons, ret);

    HAL_OUTPUT_DEV_REGISTER(UiElevator, ret);
    //HAL_LPM_DEV_REGISTER(Standby, ret);

#ifdef ENABLE_OUTPUT_DEV_AudioDump
    HAL_OUTPUT_DEV_REGISTER(AudioDump, ret);
#endif /* ENABLE_OUTPUT_DEV_AudioDump */

    /* Add new HAL device registrations here */

    return ret;
}
#elif TARGET_BOARD == BOARD_1170EVK
int APP_RegisterHalDevices(void)
{
    int ret = 0;

    HAL_GFX_DEV_REGISTER(Pxp, ret);
    HAL_CAMERA_DEV_REGISTER(MipiOv5640, ret);
    HAL_DISPLAY_DEV_REGISTER(LVGLIMG, ret);

    return ret;
}
#endif

int APP_StartFramework(void)
{
    int ret = 0;

    //FWK_MANAGER_START(LpmManager, 0, ret);
    FWK_MANAGER_START(CameraManager, CAMERA_MANAGER_TASK_PRIORITY, ret);
    FWK_MANAGER_START(DisplayManager, DISPLAY_MANAGER_TASK_PRIORITY, ret);
#if defined(ENABLE_SLAVE) && ENABLE_SLAVE
    FWK_MANAGER_START(MulticoreManager, MULTICORE_MANAGER_TASK_PRIORITY, ret);
#endif /* defined(ENABLE_SLAVE) && ENABLE_SLAVE */
    FWK_MANAGER_START(OutputManager, OUTPUT_MANAGER_TASK_PRIORITY, ret);
    FWK_MANAGER_START(InputManager, INPUT_MANAGER_TASK_PRIORITY, ret);

    return ret;
}

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if defined(ENABLE_SLAVE) && ENABLE_SLAVE
/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* Initialize MCMGR - low level multicore management library. Call this
       function as close to the reset entry as possible to allow CoreUp event
       triggering. The SystemInitHook() weak function overloading is used in this
       application. */
    (void)MCMGR_EarlyInit();
}

#endif /* defined(ENABLE_SLAVE) && ENABLE_SLAVE */

volatile int step = 0;

void APP_BoardInit()
{
    BOARD_ConfigMPU();
    /* TODO: Consider reconfiguring BOARD_BootClockRUN to only initialize CM4 related clocks. */
    BOARD_BootClockRUN();
    /* Init board hardware. */
    BOARD_InitBootPins();

#if defined(ENABLE_SLAVE) && ENABLE_SLAVE
    uint32_t startupData, i;
    mcmgr_status_t status;

    (void)MCMGR_Init();

    /* Get the startup data */
    do
    {
        status = MCMGR_GetStartupData(&startupData);
    } while (status != kStatus_MCMGR_Success);
#endif /* defined(ENABLE_SLAVE) && ENABLE_SLAVE */

    /* When doing debug. Step can be set to 1 to have time to attach on the cm4 core
     * before any critical operation is done.
     * Use debugger to modify the step from 1 -> 0 and than run step by step debuging.
     */
    while (step)
    {
    }

    // BOARD_InitBootClocks();
    BOARD_MIPIPanelTouch_I2C_Init();

    /*
     * Reset the displaymix, otherwise during debugging, the
     * debugger may not reset the display, then the behavior
     * is not right.
     */
    SRC_AssertSliceSoftwareReset(SRC, kSRC_DisplaySlice);

    BOARD_InitEDMA();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    Time_Init(1);
}

/*
 * @brief   Application entry point.
 */
int main(void)
{
    /* init the board */
    APP_BoardInit();

    xLoggingTaskInitialize(LOGGING_TASK_STACK_SIZE, LOGGING_TASK_PRIORITY, LOGGING_QUEUE_LENGTH);

    /* init the framework*/
    APP_InitFramework();

    /* register the hal devices*/
    APP_RegisterHalDevices();

    /* start the framework*/
    APP_StartFramework();

    vTaskStartScheduler();
    for (;;)
    {
    } /* should never get here */

    return 0;
}
