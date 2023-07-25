/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief board-related macro definitions. Please place each required definition here before compiling.
 */

#ifndef BOARD_DEFINE_H_
#define BOARD_DEFINE_H_

/*
 *  Board define
 */
#define BOARD_1170EVK     0
#define BOARD_SMART_TLHMI 1

#define TARGET_BOARD BOARD_SMART_TLHMI

/*
 *  AQT test: set to 1 to run in AQT test mode.
 */
#define AQT_TEST 0

/*
 * Enablement of the HAL devices
 */
//#define ENABLE_VISIONALGO_DEV_Benchmark

#define ENABLE_OUTPUT_DEV_MqsAudio
#define ENABLE_OUTPUT_DEV_SmartTlhmiConfig
#define ENABLE_INPUT_DEV_WiFiAWAM510

#if defined(ENABLE_MASTER) && ENABLE_MASTER
#define ENABLE_MULTICORE_DEV_MessageBuffer
#endif /* defined(ENABLE_MASTER) && ENABLE_MASTER */

/**********************************************/
/******* AUDIO PROCESSING CONFIGURATION *******/
/**********************************************/

#define ENABLE_INPUT_DEV_PdmMic
#define ENABLE_AUDIO_PROCESSING_DEV_Afe
#define ENABLE_DSMT_ASR
//#define ENABLE_VIT_ASR

#if defined(ENABLE_DSMT_ASR) && defined(ENABLE_VIT_ASR)
#error "ENABLE_DSMT_ASR and ENABLE_VIT_ASR are MUX. Only one can be enabled."

#elif defined(ENABLE_DSMT_ASR)
/* "Hey NXP" and its corresponding translations in other languages may take up to 3s to be spoken. */
#define WAKE_WORD_MAX_LENGTH_MS 3000

/* AMP_LOOPBACK_DISABLED = 0 - AFE performs AEC based on speaker's playback stream.
 * AMP_LOOPBACK_DISABLED = 1 - AFE does not perform AEC. */
#define AMP_LOOPBACK_DISABLED   1

/* Configurations of SELF_WAKE_UP_PROTECTION mechanism. */
#define SELF_WAKE_UP_DISABLED   (0)
#define SELF_WAKE_UP_WW         (1 << 0)
#define SELF_WAKE_UP_VC         (1 << 1)

/* Define SELF_WAKE_UP_PROTECTION in order to enable self wake up protection mechanism.
 * This mechanism will search for "fake" wake words or/and voice commands in amplifier's playback stream.
 * As result, a "fake" wake words or/and voice commands will be rejected.
 * Note: This mechanism will double CPU and RAM usages of ASR.
 * Configurations:
 * #define SELF_WAKE_UP_PROTECTION (SELF_WAKE_UP_DISABLED)             - mechanism disabled.
 * #define SELF_WAKE_UP_PROTECTION (SELF_WAKE_UP_WW)                   - searches for "fake" wake words.
 * #define SELF_WAKE_UP_PROTECTION (SELF_WAKE_UP_VC)                   - searches for "fake" voice commands.
 * #define SELF_WAKE_UP_PROTECTION (SELF_WAKE_UP_WW | SELF_WAKE_UP_VC) - searches for "fake" wake words and voice
 * commands. */
#define SELF_WAKE_UP_PROTECTION SELF_WAKE_UP_DISABLED

/* ENABLE_OUTPUT_DEV_AudioDump: 1 - AFE (mics+amp+clean), 2 - ASR (clean) */
#if AQT_TEST
#define ENABLE_OUTPUT_DEV_AudioDump 1
#endif /* AQT_TEST */

#elif defined(ENABLE_VIT_ASR)
/* "Hey NXP" and its corresponding translations in other languages may take up to 3s to be spoken. */
#define WAKE_WORD_MAX_LENGTH_MS 3000

/* VIT does not support loopback.
 * AMP_LOOPBACK_DISABLED = 1 - AFE does not perform AEC. */
#define AMP_LOOPBACK_DISABLED   1

/* VIT does not support Self Wake Up Protection mechanism.
 * SELF_WAKE_UP_PROTECTION = 0 - mechanism disabled. */
#define SELF_WAKE_UP_PROTECTION 0

/* ENABLE_OUTPUT_DEV_AudioDump: 1 - AFE (mics+amp+clean) */
#if AQT_TEST
#define ENABLE_OUTPUT_DEV_AudioDump 1
#endif /* AQT_TEST */

#else
#error "ENABLE_DSMT_ASR or ENABLE_VIT_ASR must be enabled."
#endif /* defined(ENABLE_DSMT_ASR) && defined(ENABLE_VIT_ASR) */

/* In order to use SELF_WAKE_UP_PROTECTION, it is mandatory to define AMP_LOOPBACK_DISABLED to 0 */
#if (SELF_WAKE_UP_PROTECTION) && (AMP_LOOPBACK_DISABLED)
#error "For SELF_WAKE_UP_PROTECTION 1, it is mandatory to have AMP_LOOPBACK_DISABLED 0"
#endif /* (SELF_WAKE_UP_PROTECTION) && (AMP_LOOPBACK_DISABLED) */

/**********************************************/

/*
 * Please check the camera enabled macro in board_define.h of CM4.
 * If ENABLE_CAMERA_DEV_MipiGc2145 is defined in CM4, need to open USE_CAMERA_MipiGc2145 in CM7.
 */
#define USE_CAMERA_MipiGc2145

//#define ENABLE_CAMERA_DEV_2DSim
#define ENABLE_VISIONALGO_DEV_Oasis_CoffeeMachine

#define ENABLE_FLASH_DEV_Littlefs
#define ENABLE_FACEDB

#ifdef ENABLE_FACEDB
#define OASIS_FACE_DB_DIR "oasis_coffee_machine"
#endif /* ENABLE_FACEDB */

#ifdef ENABLE_INPUT_DEV_WiFiAWAM510
#define WIFI_IW416_BOARD_AW_AM457_USD
#define ENABLE_OTA 1
#endif /* ENABLE_INPUT_DEV_WiFiAWAM510 */

#if defined(WIFI_IW416_BOARD_AW_AM457_USD) || defined(WIFI_IW416_BOARD_AW_AM457MA)

#define WIFI_ENABLED 1
#define ENABLE_WIFI_CREDENTIALS
#define SD8978
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define WIFI_BT_USE_M2_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x9, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xC \
    }
#include "wifi_config.h"
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW.h"

#endif /* WIFI_IW416_BOARD_AW_AM457_USD */

/* Memory regions definitions */
#define AT_NONCACHEABLE_SECTION_ALIGN_DTC(var, alignbytes) \
    __attribute__((section(".bss.$SRAM_DTC_cm7,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))
#define AT_CACHEABLE_SECTION_ALIGN_OCRAM(var, alignbytes) \
    __attribute__((section(".bss.$SRAM_OCRAM_CACHED,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))
#define AT_NONCACHEABLE_SECTION_ALIGN_OCRAM(var, alignbytes) \
    __attribute__((section(".bss.$SRAM_OCRAM_NCACHED,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))
#define AT_SHMEM_SECTION_ALIGN(var, alignbytes) \
    __attribute__((section(".bss.$rpmsg_sh_mem,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))
#define AT_FB_SHMEM_SECTION_ALIGN(var, alignbytes) \
    __attribute__((section(".bss.$fb_sh_mem,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))
#define AT_RES_SHMEM_SECTION_ALIGN(var, alignbytes) \
    __attribute__((section(".noinit.$res_sh_mem,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))

#define AT_NONCACHEABLE_SECTION_ALIGN_SDRAM(var, alignbytes) \
    __attribute__((section(".bss.$NCACHE_REGION,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))

/* OASIS definitions*/
#if defined(SMART_LOCK_3D)
#define OASIS_FRAME_WIDTH  540
#define OASIS_FRAME_HEIGHT 640
#elif defined(SMART_LOCK_2D) || defined(SMART_ACCESS_2D)
#define OASIS_RGB_FRAME_WIDTH          480
#define OASIS_RGB_FRAME_HEIGHT         640
#define OASIS_RGB_FRAME_BYTE_PER_PIXEL 3

#define OASIS_IR_FRAME_WIDTH          480
#define OASIS_IR_FRAME_HEIGHT         640
#define OASIS_IR_FRAME_BYTE_PER_PIXEL 3
#elif defined(SMART_TLHMI_2D)
#ifdef USE_CAMERA_MipiGc2145
#define OASIS_RGB_FRAME_WIDTH      800
#define OASIS_RGB_FRAME_HEIGHT     600
#define OASIS_RGB_FRAME_SRC_FORMAT kPixelFormat_YUV1P444_RGB
#else
#define OASIS_RGB_FRAME_WIDTH      640
#define OASIS_RGB_FRAME_HEIGHT     480
#define OASIS_RGB_FRAME_SRC_FORMAT kPixelFormat_UYVY1P422_RGB
#endif /* SMART_LOCK_3D */
#define OASIS_RGB_FRAME_BYTE_PER_PIXEL 3
#endif /* SMART_LOCK_3D */

#endif /* BOARD_DEFINE_H_ */
