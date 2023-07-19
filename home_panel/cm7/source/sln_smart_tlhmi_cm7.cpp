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
 * @file    sln_smart_tlhmi_cm7.cpp
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MIMXRT1176_cm7.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ksdk_mbedtls.h"

#include "sln_time.h"
#include "fsl_sema4.h"
#include "mcmgr.h"
#include "fica_definition.h"
#include "sln_flash_config.h"

#include "fwk_audio_processing.h"
#include "fwk_input_manager.h"
#include "fwk_message.h"
#include "fwk_output_manager.h"
#include "fwk_vision_algo_manager.h"
#include "fwk_voice_algo_manager.h"
#include "fwk_camera_manager.h"
#include "fwk_flash.h"
#include "fwk_multicore_manager.h"
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
#define LOGGING_QUEUE_LENGTH    128

AT_SHMEM_SECTION_ALIGN(unsigned char rpmsg_sh_mem[SH_MEM_TOTAL_SIZE], 8);
AT_RES_SHMEM_SECTION_ALIGN(unsigned char res_sh_mem[RES_SHMEM_TOTAL_SIZE], 64);

#if defined(ENABLE_MASTER) && ENABLE_MASTER
/* TODO: insert other include files here. */
#define CORE1_BOOT_ADDRESS (void *)0x80000000

/* TODO: insert other definitions and declarations here. */
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
#endif /* defined(ENABLE_MASTER) && ENABLE_MASTER */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

HAL_AUDIO_PROCESSING_DEV_DECLARE(Afe);
HAL_FLASH_DEV_DECLARE(Littlefs);
HAL_INPUT_DEV_DECLARE(PdmMic);
HAL_INPUT_DEV_DECLARE(WiFiAWAM510);
HAL_MULTICORE_DEV_DECLARE(MessageBuffer);
HAL_OUTPUT_DEV_DECLARE(MqsAudio_Streamer);
HAL_VALGO_DEV_DECLARE(Benchmark);
HAL_VALGO_DEV_DECLARE(UvitaGestureRecognition);
HAL_VOICEALGO_DEV_DECLARE(Asr);
HAL_VOICEALGO_DEV_DECLARE(Asr_VIT);
HAL_VALGO_DEV_DECLARE(OasisHomePanel);
HAL_CAMERA_DEV_DECLARE(2DSim);
HAL_CAMERA_DEV_DECLARE(FlexioGc0308);
/* Add new HAL device declarations here */

unsigned int FWK_CurrentTimeUs();
const char *g_coreName = "CM7";

void LoadAudioPrompts(void *base);
void LoadVITModels(void *base);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#define MULTICORE_MANAGER_TASK_PRIORITY   0
#define VISION_ALGO_MANAGER_TASK_PRIORITY 3
#define INPUT_MANAGER_TASK_PRIORITY       1
#define AUDIO_PROCESSING_TASK_PRIORITY    2
#define VOICE_ALGO_MANAGER_TASK_PRIORITY  3
#define OUTPUT_MANAGER_TASK_PRIORITY      3
#define CAMERA_MANAGER_TASK_PRIORITY      2

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

    // TODO: Fix need for specific ordering of these funcs
    HAL_FLASH_DEV_REGISTER(Littlefs, ret);

    // TODO: Make this API call conform to HAL_..._DEV_REGISTER notation
    ret = HAL_OutputDev_SmartTlhmiConfig_Init();
    if (ret != 0)
    {
        LOGE("HAL_OutputDev_SmartLockConfig_Init error %d", ret);
        return ret;
    }
    FWK_MANAGER_INIT(VisionAlgoManager, ret);
    FWK_MANAGER_INIT(OutputManager, ret);
    FWK_MANAGER_INIT(AudioProcessing, ret);
    FWK_MANAGER_INIT(InputManager, ret);
    FWK_MANAGER_INIT(VoiceAlgoManager, ret);
    FWK_MANAGER_INIT(CameraManager, ret);
#if defined(ENABLE_MASTER) && ENABLE_MASTER
    FWK_MANAGER_INIT(MulticoreManager, ret);
#endif /* defined(ENABLE_MASTER) && ENABLE_MASTER */

    return ret;
}

int APP_RegisterHalDevices(void)
{
    int ret = 0;

    // HAL_VALGO_DEV_REGISTER(Benchmark, ret);
   // HAL_OUTPUT_DEV_REGISTER(MqsAudio, ret);
    HAL_OUTPUT_DEV_REGISTER(MqsAudio_Streamer, ret);
    HAL_AUDIO_PROCESSING_DEV_REGISTER(Afe, ret);
    HAL_INPUT_DEV_REGISTER(PdmMic, ret);
    HAL_INPUT_DEV_REGISTER(WiFiAWAM510, ret);
    HAL_VOICEALGO_DEV_REGISTER(Asr_VIT, ret)
    // HAL_CAMERA_DEV_REGISTER(2DSim, ret);
    HAL_CAMERA_DEV_REGISTER(FlexioGc0308, ret);
#ifdef ENABLE_VISIONALGO_DEV_Uvita_Gesture
    HAL_VALGO_DEV_REGISTER(UvitaGestureRecognition, ret);
#endif /* ENABLE_VISIONALGO_DEV_Uvita_Gesture */
    HAL_VALGO_DEV_REGISTER(OasisHomePanel, ret);
#if defined(ENABLE_MASTER) && ENABLE_MASTER
    HAL_MULTICORE_DEV_REGISTER(MessageBuffer, ret);
#endif /* defined(ENABLE_MASTER) && ENABLE_MASTER */
    /* Add new HAL device registration here */

    return ret;
}

int APP_StartFramework(void)
{
    int ret = 0;

    FWK_MANAGER_START(VisionAlgoManager, VISION_ALGO_MANAGER_TASK_PRIORITY, ret);
    FWK_MANAGER_START(OutputManager, OUTPUT_MANAGER_TASK_PRIORITY, ret);
    FWK_MANAGER_START(AudioProcessing, AUDIO_PROCESSING_TASK_PRIORITY, ret);
    FWK_MANAGER_START(InputManager, INPUT_MANAGER_TASK_PRIORITY, ret);
    FWK_MANAGER_START(VoiceAlgoManager, VOICE_ALGO_MANAGER_TASK_PRIORITY, ret);
    FWK_MANAGER_START(CameraManager, CAMERA_MANAGER_TASK_PRIORITY, ret);
#if defined(ENABLE_MASTER) && ENABLE_MASTER
    FWK_MANAGER_START(MulticoreManager, MULTICORE_MANAGER_TASK_PRIORITY, ret);
#endif /* defined(ENABLE_MASTER) && ENABLE_MASTER */

    return ret;
}

void APP_LoadResource(void)
{
    /* load the lvgl images and icons for the cm4 core from the flash resource region */
    void *pLvglImages = (void *)(FLEXSPI_AMBA_BASE + FICA_IMG_RESOURCE_ADDR);
    memcpy((void *)APP_LVGL_IMGS_BASE, pLvglImages, APP_LVGL_IMGS_SIZE + APP_ICONS_SIZE);

    /* load audio prompts */
    void *pAudioPrompts = (void *)(FLEXSPI_AMBA_BASE + FICA_IMG_RESOURCE_ADDR + APP_LVGL_IMGS_SIZE + APP_ICONS_SIZE);
    LoadAudioPrompts(pAudioPrompts);

    /* load VIT models */
    void *pVITModels = (void *)(FLEXSPI_AMBA_BASE + FICA_IMG_RESOURCE_ADDR + APP_LVGL_IMGS_SIZE + APP_ICONS_SIZE + APP_PROMPTS_SIZE);
    LoadVITModels(pVITModels);
}

void APP_BoardInit()
{
    /* Init board hardware. */
    BOARD_RelocateVectorTableToRam();
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    /* TODO: Consider not calling BOARD_InitBootClocks when running with Bootloader. */
    BOARD_InitBootClocks();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    Time_Init(1);

    BOARD_FlexioCameraConfigDMA();

    APP_LoadResource();

    /* Initialize the HW Semaphore */
    SEMA4_Init(BOARD_SEM4_BASE);

#if defined(ENABLE_MASTER) && ENABLE_MASTER
    configPRINT_STRING("Starting CM4 core.");

    /* Initialize MCMGR before calling its API */
    (void)MCMGR_Init();

    /* Boot Secondary core application */
    (void)MCMGR_StartCore(kMCMGR_Core1, (void *)(char *)CORE1_BOOT_ADDRESS, 0, kMCMGR_Start_Synchronous);

    configPRINT_STRING("CM4 core started.");
#endif /* defined(ENABLE_MASTER) && ENABLE_MASTER */
}

/*
 * @brief   Application entry point.
 */
int main(void)
{
    APP_BoardInit();

    CRYPTO_InitHardware();

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
    }
}
