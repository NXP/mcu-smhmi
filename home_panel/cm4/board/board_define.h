/*
 * Copyright 2020-2021 NXP.
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
#define SMART_TLHMI_HOMEPANEL

/*
 *  AQT test: set to 1 to run in AQT test mode.
 */
#define AQT_TEST 0

#if TARGET_BOARD == BOARD_SMART_TLHMI
/*
 * Enablement of the HAL devices
 */
#define ENABLE_GFX_DEV_Pxp
#define ENABLE_DISPLAY_DEV_LVGLHomePanel
//#define ENABLE_CAMERA_DEV_CsiGc0308
//#define ENABLE_CAMERA_DEV_FlexioGc0308
//#define ENABLE_CAMERA_DEV_CsiMt9m114
#define ENABLE_CAMERA_DEV_MipiGc2145
#define ENABLE_OUTPUT_DEV_RgbLed
#if defined(ENABLE_SLAVE) && ENABLE_SLAVE
#define ENABLE_MULTICORE_DEV_MessageBuffer
#endif /* defined(ENABLE_SLAVE) && ENABLE_SLAVE */
#define ENABLE_INPUT_DEV_ShellUsb
#define ENABLE_OUTPUT_DEV_UiHomePanel
#define ENABLE_LPM_DEV_Standby
#define ENABLE_OUTPUT_DEV_IrWhiteLeds
#if AQT_TEST
#define ENABLE_OUTPUT_DEV_AudioDump
#endif /* AQT_TEST */

#define WIFI_ENABLED 1

/* Enable new HAL devices here */
#ifdef ENABLE_OUTPUT_DEV_RgbLed

typedef enum _rgb_led_color
{
    kRGBLedColor_Red,    /*!< LED Red Color */
    kRGBLedColor_Orange, /*!< LED Orange Color */
    kRGBLedColor_Yellow, /*!< LED Yellow Color */
    kRGBLedColor_Green,  /*!< LED Green Color */
    kRGBLedColor_Blue,   /*!< LED Blue Color */
    kRGBLedColor_Purple, /*!< LED Purple Color */
    kRGBLedColor_Cyan,   /*!< LED Cyan Color */
    kRGBLedColor_White,  /*!< LED White Color */
    kRGBLedColor_Off,    /*!< LED Off */
    kRGBLedColor_None,   /*!< LED do nothing */
} rgbLedColor_t;
#endif /* ENABLE_OUTPUT_DEV_RgbLed */
/*
 * Static configurations for HAL devices
 */
#ifdef ENABLE_DISPLAY_DEV_LVGLHomePanel
#define DISPLAY_DEV_LVGLHomePanel_BUFFER_COUNT 1
#define DISPLAY_DEV_LVGLHomePanel_HEIGHT       640
#define DISPLAY_DEV_LVGLHomePanel_WIDTH        480
#define DISPLAY_DEV_LVGLHomePanel_StartX       110
#define DISPLAY_DEV_LVGLHomePanel_StartY       30
#define DISPLAY_DEV_LVGLHomePanel_LEFT         0
#define DISPLAY_DEV_LVGLHomePanel_TOP          0
#define DISPLAY_DEV_LVGLHomePanel_RIGHT        479
#define DISPLAY_DEV_LVGLHomePanel_BOTTOM       639
#define DISPLAY_DEV_LVGLHomePanel_ROTATE       kCWRotateDegree_270
#define DISPLAY_DEV_LVGLHomePanel_FORMAT       kPixelFormat_RGB565
#ifdef ENABLE_CAMERA_DEV_MipiGc2145
#define DISPLAY_DEV_LVGLHomePanel_SRCFORMAT kPixelFormat_YUV1P444_RGB
#else
#define DISPLAY_DEV_LVGLHomePanel_SRCFORMAT kPixelFormat_UYVY1P422_RGB
#endif /* ENABLE_CAMERA_DEV_MipiGc2145 */
#define DISPLAY_DEV_LVGLHomePanel_BPP 2
#endif /* ENABLE_DisplayDev_LVGLHomePanel */

#ifdef ENABLE_DISPLAY_DEV_LVGLIMG
#define DISPLAY_DEV_LVGLIMG_BUFFER_COUNT 1
#define DISPLAY_DEV_LVGLIMG_HEIGHT       480
#define DISPLAY_DEV_LVGLIMG_WIDTH        640
#define DISPLAY_DEV_LVGLIMG_LEFT         0
#define DISPLAY_DEV_LVGLIMG_TOP          0
#define DISPLAY_DEV_LVGLIMG_RIGHT        639
#define DISPLAY_DEV_LVGLIMG_BOTTOM       479
#define DISPLAY_DEV_LVGLIMG_ROTATE       kCWRotateDegree_0
#define DISPLAY_DEV_LVGLIMG_FORMAT       kPixelFormat_RGB565
#define DISPLAY_DEV_LVGLIMG_SRCFORMAT    kPixelFormat_UYVY1P422_RGB // kPixelFormat_YUV1P444_RGB
#define DISPLAY_DEV_LVGLIMG_BPP          2
#endif /* ENABLE_DisplayDev_LVGLIMG */

#ifdef ENABLE_CAMERA_DEV_CsiGc0308
#define CAMERA_DEV_CsiGc0308_BUFFER_COUNT 4
#define CAMERA_DEV_CsiGc0308_HEIGHT       480
#define CAMERA_DEV_CsiGc0308_WIDTH        640
#define CAMERA_DEV_CsiGc0308_LEFT         0
#define CAMERA_DEV_CsiGc0308_TOP          0
#define CAMERA_DEV_CsiGc0308_RIGHT        639
#define CAMERA_DEV_CsiGc0308_BOTTOM       479
#define CAMERA_DEV_CsiGc0308_ROTATE       kCWRotateDegree_0
#define CAMERA_DEV_CsiGc0308_FLIP         kFlipMode_None
#define CAMERA_DEV_CsiGc0308_SWAPBYTE     1
#define CAMERA_DEV_CsiGc0308_FORMAT       kPixelFormat_UYVY1P422_RGB
#define CAMERA_DEV_CsiGc0308_BPP          2
#endif /* ENABLE_CAMERA_DEV_CsiGc0308 */

#ifdef ENABLE_CAMERA_DEV_FlexioGc0308
#define CAMERA_DEV_FlexioGc0308_BUFFER_COUNT 2
#define CAMERA_DEV_FlexioGc0308_HEIGHT       480
#define CAMERA_DEV_FlexioGc0308_WIDTH        640
#define CAMERA_DEV_FlexioGc0308_LEFT         0
#define CAMERA_DEV_FlexioGc0308_TOP          0
#define CAMERA_DEV_FlexioGc0308_RIGHT        639
#define CAMERA_DEV_FlexioGc0308_BOTTOM       479
#define CAMERA_DEV_FlexioGc0308_ROTATE       kCWRotateDegree_0
#define CAMERA_DEV_FlexioGc0308_FLIP         kFlipMode_None
#define CAMERA_DEV_FlexioGc0308_SWAPBYTE     1
#define CAMERA_DEV_FlexioGc0308_FORMAT       kPixelFormat_UYVY1P422_Gray
#define CAMERA_DEV_FlexioGc0308_BPP          2
#endif /* ENABLE_CAMERA_DEV_FlexioGc0308 */

#ifdef ENABLE_CAMERA_DEV_CsiMt9m114
#define CAMERA_DEV_CsiMt9m114_BUFFER_COUNT 4
#define CAMERA_DEV_CsiMt9m114_HEIGHT       480 // 720
#define CAMERA_DEV_CsiMt9m114_WIDTH        640 // 1280
#define CAMERA_DEV_CsiMt9m114_LEFT         0
#define CAMERA_DEV_CsiMt9m114_TOP          0
#define CAMERA_DEV_CsiMt9m114_RIGHT        639 // 1279
#define CAMERA_DEV_CsiMt9m114_BOTTOM       479 // 719
#define CAMERA_DEV_CsiMt9m114_ROTATE       kCWRotateDegree_0
#define CAMERA_DEV_CsiMt9m114_FLIP         kFlipMode_None
#define CAMERA_DEV_CsiMt9m114_SWAPBYTE     0
#define CAMERA_DEV_CsiMt9m114_FORMAT       kPixelFormat_UYVY1P422_RGB
#define CAMERA_DEV_CsiMt9m114_BPP          2
#endif /* ENABLE_CAMERA_DEV_CsiMt9m114 */

#ifdef ENABLE_CAMERA_DEV_MipiGc2145
#define CAMERA_DEV_MipiGc2145_BUFFER_COUNT 2
#define CAMERA_DEV_MipiGc2145_HEIGHT       600 // 720
#define CAMERA_DEV_MipiGc2145_WIDTH        800 // 1280
#define CAMERA_DEV_MipiGc2145_LEFT         0
#define CAMERA_DEV_MipiGc2145_TOP          0
#define CAMERA_DEV_MipiGc2145_RIGHT        799 // 1279
#define CAMERA_DEV_MipiGc2145_BOTTOM       599 // 719
#define CAMERA_DEV_MipiGc2145_ROTATE       kCWRotateDegree_0
#define CAMERA_DEV_MipiGc2145_FLIP         kFlipMode_None
#define CAMERA_DEV_MipiGc2145_SWAPBYTE     0
#define CAMERA_DEV_MipiGc2145_FORMAT       kPixelFormat_YUV1P444_RGB
#define CAMERA_DEV_MipiGc2145_BPP          4
#endif /* ENABLE_CAMERA_DEV_MipiGc2145 */

#ifdef ENABLE_OUTPUT_DEV_UiHomePanel
#define STREAMER_AUDIO_DEFAULT_VOLUME 40
#endif /* ENABLE_OUTPUT_DEV_UiHomePanel */

#elif TARGET_BOARD == BOARD_1170EVK
/*
 * Enablement of the HAL devices
 */
#define ENABLE_GFX_DEV_Pxp
#define ENABLE_CAMERA_DEV_MipiOv5640
#define ENABLE_DISPLAY_DEV_LVGLIMG

/*
 * Static configurations for HAL devices
 */
#ifdef ENABLE_DISPLAY_DEV_LVGLIMG
#define DISPLAY_DEV_LVGLIMG_BUFFER_COUNT 1
#define DISPLAY_DEV_LVGLIMG_HEIGHT       640
#define DISPLAY_DEV_LVGLIMG_WIDTH        480
#define DISPLAY_DEV_LVGLIMG_LEFT         0
#define DISPLAY_DEV_LVGLIMG_TOP          0
#define DISPLAY_DEV_LVGLIMG_RIGHT        479
#define DISPLAY_DEV_LVGLIMG_BOTTOM       639
#define DISPLAY_DEV_LVGLIMG_ROTATE       kCWRotateDegree_0
#define DISPLAY_DEV_LVGLIMG_FORMAT       kPixelFormat_RGB565
#define DISPLAY_DEV_LVGLIMG_SRCFORMAT    kPixelFormat_YUV1P444_RGB
#define DISPLAY_DEV_LVGLIMG_BPP          2
#endif /* ENABLE_DisplayDev_LVGLIMG */

#ifdef ENABLE_CAMERA_DEV_MipiOv5640
#define CAMERA_DEV_MipiOv5640_BUFFER_COUNT 3
#define CAMERA_DEV_MipiOv5640_HEIGHT       480
#define CAMERA_DEV_MipiOv5640_WIDTH        640
#define CAMERA_DEV_MipiOv5640_LEFT         0
#define CAMERA_DEV_MipiOv5640_TOP          0
#define CAMERA_DEV_MipiOv5640_RIGHT        639
#define CAMERA_DEV_MipiOv5640_BOTTOM       479
#define CAMERA_DEV_MipiOv5640_ROTATE       kCWRotateDegree_90
#define CAMERA_DEV_MipiOv5640_FLIP         kFlipMode_None
#define CAMERA_DEV_MipiOv5640_SWAPBYTE     0
#define CAMERA_DEV_MipiOv5640_FORMAT       kPixelFormat_YUV1P444_RGB
#define CAMERA_DEV_MipiOv5640_BPP          4
#endif /* ENABLE_CAMERA_DEV_MipiOv5640 */
#endif

#define AT_SHMEM_SECTION_ALIGN(var, alignbytes) \
    __attribute__((section(".bss.$rpmsg_sh_mem,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))

#define AT_RES_SHMEM_SECTION_ALIGN(var, alignbytes) \
    __attribute__((section(".noinit.$res_sh_mem,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))

#define AT_USR_SHMEM_SECTION_ALIGN(var, alignbytes) \
    __attribute__((section(".noinit.$usr_sh_mem,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))

#endif /* BOARD_DEFINE_H_ */
