/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief Application-specific configuration settings.
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "board_define.h"
#include "board.h"
#include "fwk_config.h"
#include "hal_voice_algo_asr_local.h"
#include "hal_vision_algo.h"
#include "fwk_common.h"

/* App task ID offset definition here. App task ID will start from kFWKTaskID_APPStart */
#define APP_TASK_ID(n)    (kFWKTaskID_APPStart + n)
#define MQS_AUDIO_TASK_ID (APP_TASK_ID(1))
#define kAppTaskID_WiFi   (APP_TASK_ID(2))

#ifndef SH_MEM_TOTAL_SIZE
#define SH_MEM_TOTAL_SIZE 0x20000
#endif

extern unsigned char rpmsg_sh_mem[];

typedef struct _smart_tlhmi_config
{
    uint8_t mode;      /* oasis application mode */
    uint8_t irPwm;     /* brightness % (0-100) */
    uint8_t whitePwm;  /* brightness % (0-100) */
    uint8_t sleepMode; /* detect no face timeout sleep */
    uint32_t speakerVolume;
    uint8_t password[6];       /* device password */
    uint32_t faceRecThreshold; /* % (0 - 1000); face rec threshold = faceRecThreshold * 1.0 / 1000 */
#if defined(ENABLE_VOICE)
    asr_voice_config_t asrConfig;
#endif
} smart_tlhmi_config_t;

#define APP_SH_MEM_BASE (uint32_t) & rpmsg_sh_mem

#define BOARD_SHMEM_WRITE (APP_SH_MEM_BASE)
#define BOARD_SHMEM_READ  (APP_SH_MEM_BASE + (SH_MEM_TOTAL_SIZE / 2))

#ifndef RES_SHMEM_TOTAL_SIZE
#define RES_SHMEM_TOTAL_SIZE 0x700000
#endif

extern unsigned char res_sh_mem[];
#define APP_RES_SHMEM_BASE (uint32_t) & res_sh_mem
#define APP_LVGL_IMGS_BASE (APP_RES_SHMEM_BASE)
#define APP_LVGL_IMGS_SIZE 0x59f480
#define APP_ICONS_BASE     (APP_RES_SHMEM_BASE + APP_LVGL_IMGS_SIZE)
#define APP_ICONS_SIZE     0x107c40

#if defined(__cplusplus)
extern "C" {
#endif

hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_Init();

//oasis_lite_mode_t HAL_OutputDev_SmartTlhmiConfig_GetMode();
hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_SetMode(oasis_lite_mode_t mode);
uint8_t HAL_OutputDev_SmartTlhmiConfig_GetIrPwm();
hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_SetIrPwm(uint8_t brightness);
uint8_t HAL_OutputDev_SmartTlhmiConfig_GetWhitePwm();
hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_SetWhitePwm(uint8_t brightness);
hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_GetPassword(uint8_t *password);
hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_SetPassword(uint8_t *password);
uint32_t HAL_OutputDev_SmartTlhmiConfig_GetSpeakerVolume();
hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_SetSpeakerVolume(uint32_t speakerVolume);
uint8_t HAL_OutputDev_SmartTlhmiConfig_GetSleepMode();
hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_SetSleepMode(uint8_t sleepMode);
hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_GetFaceRecThreshold(unsigned int *pThreshold);
hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_SetFaceRecThreshold(unsigned int threshold);

asr_voice_config_t HAL_OutputDev_SmartTlhmiConfig_GetAsrConfig();
hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_SetAsrConfig(asr_voice_config_t asrConfig);
uint32_t HAL_OutputDev_SmartTlhmiConfig_GetAsrTimeoutDuration();
hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_SetAsrTimeoutDuration(uint32_t duration);
asr_followup_t HAL_OutputDev_SmartTlhmiConfig_GetAsrFollowupStatus();
hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_SetAsrFollowupStatus(asr_followup_t enableFollowup);
asr_language_t HAL_OutputDev_SmartTlhmiConfig_GetAsrMultilingualConfig();
hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_SetAsrMultilingualConfig(asr_language_t asrConfig);
asr_inference_t HAL_OutputDev_SmartTlhmiConfig_GetAsrDemo();
hal_config_status_t HAL_OutputDev_SmartTlhmiConfig_SetAsrDemo(asr_inference_t asrConfig);
#if defined(__cplusplus)
}
#endif

#endif /* APP_CONFIG_H */
