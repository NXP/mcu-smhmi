/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief Framework task declaration.
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "board_define.h"
#include "board.h"

#define TLHMI_FW_BUILD          "0"
#define TLHMI_FW_VERSION_MAJOR  1
#define TLHMI_FW_VERSION_MINOR  2
#define TLHMI_FW_VERSION_HOTFIX 0
#define SDK_VERSION             "SDK 2.13.0"

#ifndef SH_MEM_TOTAL_SIZE
#define SH_MEM_TOTAL_SIZE 0x8000
#endif

extern unsigned char rpmsg_sh_mem[];

#define APP_SH_MEM_BASE (uint32_t) & rpmsg_sh_mem

#define BOARD_SHMEM_WRITE (APP_SH_MEM_BASE + (SH_MEM_TOTAL_SIZE / 2))
#define BOARD_SHMEM_READ  (APP_SH_MEM_BASE)

#ifndef RES_SHMEM_TOTAL_SIZE
#define RES_SHMEM_TOTAL_SIZE 0x7f0000
#endif

#ifndef USR_SHMEM_TOTAL_SIZE
#define USR_SHMEM_TOTAL_SIZE 0x10000
#endif

extern unsigned char res_sh_mem[];
#define APP_RES_SHMEM_BASE (uint32_t) & res_sh_mem
#define APP_LVGL_IMGS_BASE (APP_RES_SHMEM_BASE)
#define APP_LVGL_IMGS_SIZE 0x007e8680
#define APP_ICONS_BASE     (APP_RES_SHMEM_BASE + APP_LVGL_IMGS_SIZE)
#define APP_ICONS_SIZE     0x00001a40

#endif /* APP_CONFIG_H */
