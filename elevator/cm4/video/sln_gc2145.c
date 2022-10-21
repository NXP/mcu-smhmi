/*
 * Copyright 2021-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "sln_gc2145.h"
#include "fsl_video_common.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define GC2145_DelayMs(ms) VIDEO_DelayMs(ms)
#define GC2145_Write(handle, reg, size, value)                                                   \
    VIDEO_I2C_WriteReg(GC2145_I2C_ADDR, kVIDEO_RegAddr8Bit, reg, (video_reg_width_t)size, value, \
                       ((gc2145_resource_t *)(handle->resource))->i2cSendFunc)
#define GC2145_Read(handle, reg, size, value)                                                   \
    VIDEO_I2C_ReadReg(GC2145_I2C_ADDR, kVIDEO_RegAddr8Bit, reg, (video_reg_width_t)size, value, \
                      ((gc2145_resource_t *)(handle->resource))->i2cReceiveFunc)

typedef struct _gc2145_reg_val
{
    uint8_t reg;
    uint8_t value;
} gc2145_reg_val_t;

typedef enum _gc2145_testmode
{
    ktest_image_solidred,
    ktest_image_colorbar,
} gc2145_testmode_t;

typedef enum _gc2145_cam_effect
{
    kcam_effect_enc_normal,    /*!< GC2145 camera effect normal. */
    kcam_effect_enc_grayscale, /*!< GC2145 camera effect grayscale. */
} gc2145_cam_effect_t;

#define GC2145_I2C_ADDR      0x3c
#define GC2145_CHIP_ID       0x2145
#define GC2145_REG_CHIP_ID_H 0xf0
#define GC2145_REG_CHIP_ID_L 0xf1

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static status_t GC2145_Init(camera_device_handle_t *handle, const camera_config_t *config);
static status_t GC2145_Deinit(camera_device_handle_t *handle);
static status_t GC2145_Start(camera_device_handle_t *handle);
static status_t GC2145_Stop(camera_device_handle_t *handle);
static status_t GC2145_Control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg);
static status_t GC2145_InitExt(camera_device_handle_t *handle,
                               const camera_config_t *config,
                               const void *specialConfig);

/*******************************************************************************
 * Variables
 ******************************************************************************/
const camera_device_operations_t gc2145_ops = {
    .init     = GC2145_Init,
    .deinit   = GC2145_Deinit,
    .start    = GC2145_Start,
    .stop     = GC2145_Stop,
    .control  = GC2145_Control,
    .init_ext = GC2145_InitExt,
};

static const gc2145_reg_val_t gc2145InitParam[] = {
    {0xfe, 0xf0},
    {0xfe, 0xf0},
    {0xfe, 0xf0},
    {0xfc, 0x06},
    {0xf6, 0x00},
    {0xf7, 0x1d},
    {0xf8, 0x84},
    {0xfa, 0x00},
    {0xf9, 0x8e},
    {0xf2, 0x00},
    /////////////////////////////////////////////////
    //////////////////Analog & Cisctl//////////////////////
    ////////////////////////////////////////////////////
    {0xfe, 0x00},
    {0x03, 0x04}, // exp time
    {0x04, 0xe2},
    {0x09, 0x00}, // row start
    {0x0a, 0x00}, // 0x00
    {0x0b, 0x00}, // col start
    {0x0c, 0x00}, // 0x00
    {0x0d, 0x04}, // 0x04
    {0x0e, 0xc0}, // 0xc0 1216 //height
    {0x0f, 0x06}, // 0x06
    {0x10, 0x52}, // 0x52 1618 //width
    {0x12, 0x2e}, // sh_delay
    {0x17, 0x15}, // mirror
    {0x18, 0x22}, // sdart mode
    {0x19, 0x0e}, // AD pipe number
    {0x1a, 0x01}, // AD manual switch mode
    {0x1b, 0x4b}, // restg width, SH width
    {0x1c, 0x07}, // TX Width,Space Width
    {0x1d, 0x10}, // double reset
    {0x1e, 0x88}, // Analog Mode1,TX high,Coln_r
    {0x1f, 0x78}, // Analog Mode2,txlow
    {0x20, 0x03}, // Analog Mode3,comv,ad_clk mode
    {0x21, 0x40},
    {0x22, 0xa0}, // Vref vpix  FPNÑÏÖØ
    {0x24, 0x16}, // Pad drv
    {0x25, 0x01}, // col sel
    {0x26, 0x10}, // Analog PGA gain1
    {0x2d, 0x60}, // txl drv mode
    {0x30, 0x01}, // Analog Mode4
    {0x31, 0x90}, // Analog Mode7
    {0x33, 0x06}, // EQ_hstart_width
    {0x34, 0x01},
    /////////////////////////////////////////////////
    //////////////////ISP reg////////////////////
    /////////////////////////////////////////////////
    {0xfe, 0x00},
    {0x80, 0x7f}, // outdoor gamma_en, GAMMA_en, CC_en, EE_en, INTP_en, DN_en, DD_en,LSC_en
    {0x81, 0x26}, // BLK dither mode, ll_y_en ,skin_en, edge SA, new_skin_mode, autogray_en,ll_gamma_en,BFF test image
    {0x82, 0xfa}, // auto_SA, auto_EE, auto_DN, auto_DD, auto_LSC, ABS_en, AWB_en, NA
    {0x83, 0x00}, // special_effect
    {0x84, 0x00}, // output format
    {0x86, 0x02}, // 0x02 //sync mode
    {0x88, 0x03}, //[1]ctl_auto_gating [0]out_auto_gating
    {0x89, 0x03}, // bypass disable
    {0x85, 0x08}, // frame start cut
    {0x8a, 0x00}, // ISP_quiet_mode,close aaa pclk,BLK gate mode,exception,close first pipe clock,close dndd clock,close
                  // intp clock,DIV_gatedclk_en
    {0x8b, 0x00}, //[7:6]BFF_gate_mode,[5]BLK switch gain,[4]protect exp,[3:2]pipe gate mode,[1]not split sram,[0]dark
                  // current update
    {0xb0, 0x55}, // global gain
    {0xc3, 0x00}, //[7:4]auto_exp_gamma_th1[11:8],[3:0]auto_exp_gamma_th2[11:8]
    {0xc4, 0x80}, // auto_exp_gamma_th1[7:0] into
    {0xc5, 0x90}, // auto_exp_gamma_th2[7:0] out //outdoor gamma
    {0xc6, 0x3b}, // auto_gamma_th1
    {0xc7, 0x46}, // auto_gamma_th2
    {0xec, 0x06}, // measure window
    {0xed, 0x04},
    {0xee, 0x60}, // col
    {0xef, 0x90}, // row
    {0xb6, 0x01}, //[0]aec en
    {0x90, 0x01}, // crop
    {0x91, 0x00},
    {0x92, 0x00},
    {0x93, 0x00},
    {0x94, 0x00},
    {0x95, 0x04},
    {0x96, 0xb0},
    {0x97, 0x06},
    {0x98, 0x40},
    /////////////////////////////////////////
    /////////// BLK ////////////////////////
    /////////////////////////////////////////
    {0xfe, 0x00},
    {0x40, 0x42},
    {0x41, 0x00}, // dark row sel
    {0x43, 0x5b}, //[7:4]BLK start not smooth  [3:0]output start frame
    {0x5e, 0x00},
    {0x5f, 0x00},
    {0x60, 0x00},
    {0x61, 0x00},
    {0x62, 0x00},
    {0x63, 0x00},
    {0x64, 0x00},
    {0x65, 0x00},
    {0x66, 0x20},
    {0x67, 0x20},
    {0x68, 0x20},
    {0x69, 0x20},
    {0x76, 0x00},
    {0x6a, 0x08},
    {0x6b, 0x08},
    {0x6c, 0x08},
    {0x6d, 0x08},
    {0x6e, 0x08},
    {0x6f, 0x08},
    {0x70, 0x08},
    {0x71, 0x08}, // manual offset
    {0x76, 0x00}, // add offset
    {0x72, 0xf0}, //[7:4]BLK DD th [3:0]BLK various th
    {0x7e, 0x3c}, // ndark
    {0x7f, 0x00},
    {0xfe, 0x02},
    {0x48, 0x15},
    {0x49, 0x00}, // ASDE OFFSET SLOPE
    {0x4b, 0x0b}, // ASDE y OFFSET SLOPE
    {0xfe, 0x00},
    ////////////////////////////////////////
    /////////// AEC ////////////////////////
    ////////////////////////////////////////
    {0xfe, 0x01},
    {0x01, 0x04}, // AEC X1
    {0x02, 0xc0}, // AEC X2
    {0x03, 0x04}, // AEC Y1
    {0x04, 0x90}, // AEC Y2
    {0x05, 0x30}, // AEC center X1
    {0x06, 0x90}, // AEC center X2
    {0x07, 0x30}, // AEC center Y1
    {0x08, 0x80}, // AEC center Y2
    {0x09, 0x00}, // AEC show mode
    {0x0a, 0x82}, //[7]col gain enable
    {0x0b, 0x11}, // AEC every N
    {0x0c, 0x10}, // AEC_mode3 center weight
    {0x11, 0x10},
    {0x13, 0x7b}, // AEC Y target
    {0x17, 0x00}, // AEC ignore mode
    {0x1c, 0x11},
    {0x1e, 0x61},
    {0x1f, 0x35}, // max pre gain
    {0x20, 0x40}, // max post gain
    {0x22, 0x40}, // AEC outdoor THD
    {0x23, 0x20}, // target_Y_low_limit
    {0xfe, 0x02},
    {0x0f, 0x04},
    {0xfe, 0x01},
    {0x12, 0x35}, //[5:4]group_size [3]slope_disable [2]outdoor_enable [0]histogram_enable
    {0x15, 0xb0}, // target_Y_high_limit
    {0x10, 0x31}, // num_thd_high
    {0x3e, 0x28}, // num_thd_low
    {0x3f, 0xb0}, // luma_thd
    {0x40, 0x90}, // luma_slope
    {0x41, 0x0f}, // color_diff
    /////////////////////////////
    //////// INTPEE /////////////
    /////////////////////////////
    {0xfe, 0x02},
    {0x90, 0x6c}, // eeintp mode1
    {0x91, 0x03}, // eeintp mode2
    {0x92, 0xcb}, // low criteria for direction
    {0x94, 0x33},
    {0x95, 0x84},
    {0x97, 0x65}, // edge effect
    {0xa2, 0x11}, // fix direction
    {0xfe, 0x00},
    /////////////////////////////
    //////// DNDD///////////////
    /////////////////////////////
    {0xfe, 0x02}, //[7]share mode [6]skin mode  [5]is 5x5 mode [1:0]noise value select 0:2  1:2.5  2:3  3:4
    {0x80, 0xc1},
    {0x81, 0x08},
    {0x82, 0x05}, // signal a 0.6
    {0x83, 0x08}, // signal b 2.5
    {0x84, 0x0a}, // dark_DD_TH
    {0x86, 0xf0}, // Y_value_dd_th2
    {0x87, 0x50}, // Y_value_dd_th3
    {0x88, 0x15}, // Y_value_dd_th4
    {0x89, 0xb0}, // asde th2
    {0x8a, 0x30}, // asde th3
    {0x8b, 0x10}, // asde th4
    /////////////////////////////////////////
    /////////// ASDE ////////////////////////
    /////////////////////////////////////////
    {0xfe, 0x01},
    {0x21, 0x04}, // luma_value_div_sel
    {0xfe, 0x02},
    {0xa3, 0x50}, // ASDE_low_luma_value_LSC_th_H
    {0xa4, 0x20}, // ASDE_low_luma_value_LSC_th_L
    {0xa5, 0x40}, // ASDE_LSC_gain_dec_slope_H
    {0xa6, 0x80}, // ASDE_LSC_gain_dec_slope_L
    {0xab, 0x40}, // ASDE_low_luma_value_OT_th
    {0xae, 0x0c}, //[3]EE1_effect_inc_or_dec_high,[2]EE2_effect_inc_or_dec_high,
                  //[1]EE1_effect_inc_or_dec_low,[0]EE2_effect_inc_or_dec_low,  1:inc  0:dec
    {0xb3, 0x46}, // ASDE_EE1_effect_slope_low,ASDE_EE2_effect_slope_low
    {0xb4, 0x64}, // ASDE_EE1_effect_slope_high,ASDE_EE2_effect_slope_high
    {0xb6, 0x38}, // ASDE_auto_saturation_dec_slope
    {0xb7, 0x01}, // ASDE_sub_saturation_slope
    {0xb9, 0x2b}, //[7:0]ASDE_auto_saturation_low_limit
    {0x3c, 0x04}, //[3:0]auto gray_dec_slope
    {0x3d, 0x15}, //[7:0]auto gray_dec_th
    {0x4b, 0x06}, // y offset slope
    {0x4c, 0x20}, // y offset limit
    {0xfe, 0x00},
    /////////////////////////////////////////
    /////////// GAMMA   ////////////////////////
    /////////////////////////////////////////
    ///////////////////gamma1////////////////////

    {0xfe, 0x02},
    {0x10, 0x09},
    {0x11, 0x0d},
    {0x12, 0x13},
    {0x13, 0x19},
    {0x14, 0x27},
    {0x15, 0x37},
    {0x16, 0x45},
    {0x17, 0x53},
    {0x18, 0x69},
    {0x19, 0x7d},
    {0x1a, 0x8f},
    {0x1b, 0x9d},
    {0x1c, 0xa9},
    {0x1d, 0xbd},
    {0x1e, 0xcd},
    {0x1f, 0xd9},
    {0x20, 0xe3},
    {0x21, 0xea},
    {0x22, 0xef},
    {0x23, 0xf5},
    {0x24, 0xf9},
    {0x25, 0xff},
    {0xfe, 0x00},
    {0xc6, 0x20}, // auto gammath1
    {0xc7, 0x2b}, // auto gammath2
    ///////////////////gamma2////////////////////
    {0xfe, 0x02},
    {0x26, 0x0f},
    {0x27, 0x14},
    {0x28, 0x19},
    {0x29, 0x1e},
    {0x2a, 0x27},
    {0x2b, 0x33},
    {0x2c, 0x3b},
    {0x2d, 0x45},
    {0x2e, 0x59},
    {0x2f, 0x69},
    {0x30, 0x7c},
    {0x31, 0x89},
    {0x32, 0x98},
    {0x33, 0xae},
    {0x34, 0xc0},
    {0x35, 0xcf},
    {0x36, 0xda},
    {0x37, 0xe2},
    {0x38, 0xe9},
    {0x39, 0xf3},
    {0x3a, 0xf9},
    {0x3b, 0xff},
    ///////////////////////////////////////////////
    ///////////YCP ///////////////////////
    ///////////////////////////////////////////////
    {0xfe, 0x02},
    {0xd1, 0x32},
    {0xd2, 0x32},
    {0xd3, 0x40},
    {0xd6, 0xf0},
    {0xd7, 0x10},
    {0xd8, 0xda},
    {0xdd, 0x14}, // edge sa
    {0xde, 0x86}, // asde auto gray
    {0xed, 0x80},
    {0xee, 0x00},
    {0xef, 0x3f},
    {0xd8, 0xd8},
    ///////////////////abs/////////////////
    {0xfe, 0x01},
    {0x9f, 0x40},
    /////////////////////////////////////////////
    //////////////////////// LSC ///////////////
    //////////////////////////////////////////
    {0xfe, 0x01},
    {0xc2, 0x14},
    {0xc3, 0x0d},
    {0xc4, 0x0c},
    {0xc8, 0x15},
    {0xc9, 0x0d},
    {0xca, 0x0a},
    {0xbc, 0x24},
    {0xbd, 0x10},
    {0xbe, 0x0b},
    {0xb6, 0x25},
    {0xb7, 0x16},
    {0xb8, 0x15},
    {0xc5, 0x00},
    {0xc6, 0x00},
    {0xc7, 0x00},
    {0xcb, 0x00},
    {0xcc, 0x00},
    {0xcd, 0x00},
    {0xbf, 0x07},
    {0xc0, 0x00},
    {0xc1, 0x00},
    {0xb9, 0x00},
    {0xba, 0x00},
    {0xbb, 0x00},
    {0xaa, 0x01},
    {0xab, 0x01},
    {0xac, 0x00},
    {0xad, 0x05},
    {0xae, 0x06},
    {0xaf, 0x0e},
    {0xb0, 0x0b},
    {0xb1, 0x07},
    {0xb2, 0x06},
    {0xb3, 0x17},
    {0xb4, 0x0e},
    {0xb5, 0x0e},
    {0xd0, 0x09},
    {0xd1, 0x00},
    {0xd2, 0x00},
    {0xd6, 0x08},
    {0xd7, 0x00},
    {0xd8, 0x00},
    {0xd9, 0x00},
    {0xda, 0x00},
    {0xdb, 0x00},
    {0xd3, 0x0a},
    {0xd4, 0x00},
    {0xd5, 0x00},
    {0xa4, 0x00},
    {0xa5, 0x00},
    {0xa6, 0x77},
    {0xa7, 0x77},
    {0xa8, 0x77},
    {0xa9, 0x77},
    {0xa1, 0x80},
    {0xa2, 0x80},
    {0xfe, 0x01},
    {0xdf, 0x0d},
    {0xdc, 0x25},
    {0xdd, 0x30},
    {0xe0, 0x77},
    {0xe1, 0x80},
    {0xe2, 0x77},
    {0xe3, 0x90},
    {0xe6, 0x90},
    {0xe7, 0xa0},
    {0xe8, 0x90},
    {0xe9, 0xa0},
    {0xfe, 0x00},
    ///////////////////////////////////////////////
    /////////// AWB////////////////////////
    ///////////////////////////////////////////////
    {0xfe, 0x01},
    {0x4f, 0x00},
    {0x4f, 0x00},
    {0x4b, 0x01},
    {0x4f, 0x00},
    {0x4c, 0x01}, // D75
    {0x4d, 0x71},
    {0x4e, 0x01},
    {0x4c, 0x01},
    {0x4d, 0x91},
    {0x4e, 0x01},
    {0x4c, 0x01},
    {0x4d, 0x70},
    {0x4e, 0x01},
    {0x4c, 0x02},
    {0x4d, 0x8b},
    {0x4e, 0x01},
    {0x4c, 0x02},
    {0x4d, 0xca},
    {0x4e, 0x01},
    {0x4c, 0x02},
    {0x4d, 0xea},
    {0x4e, 0x01},
    {0x4c, 0x02},
    {0x4d, 0xc9},
    {0x4e, 0x01},
    {0x4c, 0x01}, // D65
    {0x4d, 0x90},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0xb0},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0x8f},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0x6f},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0xaf},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0xd0},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0xf0},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0xcf},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0xef},
    {0x4e, 0x02},
    {0x4c, 0x02},
    {0x4d, 0x8b},
    {0x4e, 0x02},
    {0x4c, 0x02},
    {0x4d, 0xab},
    {0x4e, 0x02},
    {0x4c, 0x02},
    {0x4d, 0xaa},
    {0x4e, 0x02},
    {0x4c, 0x02},
    {0x4d, 0xc9},
    {0x4e, 0x02},
    {0x4c, 0x02},
    {0x4d, 0xe9},
    {0x4e, 0x02},
    {0x4c, 0x01}, // D50
    {0x4d, 0x6e},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x8e},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xae},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xce},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x4d},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x6d},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x8d},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xad},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xcd},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x4c},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x6c},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x8c},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xac},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xcc},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xcb},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x4b},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x6b},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x8b},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xab},
    {0x4e, 0x03},
    {0x4c, 0x02},
    {0x4d, 0x8b},
    {0x4e, 0x03},
    {0x4c, 0x02},
    {0x4d, 0xca},
    {0x4e, 0x03},
    {0x4c, 0x01}, // CWF
    {0x4d, 0x8a},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0xaa},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0xca},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0xca},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0xc9},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0x8a},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0x89},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0xa9},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0x32},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0x51},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0x71},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0x91},
    {0x4e, 0x04},
    {0x4c, 0x02}, // tl84
    {0x4d, 0x0b},
    {0x4e, 0x05},
    {0x4c, 0x02},
    {0x4d, 0x0a},
    {0x4e, 0x05},
    {0x4c, 0x01},
    {0x4d, 0xeb},
    {0x4e, 0x05},
    {0x4c, 0x01},
    {0x4d, 0xea},
    {0x4e, 0x05},
    {0x4c, 0x02},
    {0x4d, 0x09},
    {0x4e, 0x05},
    {0x4c, 0x02},
    {0x4d, 0x29},
    {0x4e, 0x05},
    {0x4c, 0x02},
    {0x4d, 0x2a},
    {0x4e, 0x05},
    {0x4c, 0x02},
    {0x4d, 0x4a},
    {0x4e, 0x05},
    {0x4c, 0x02},
    {0x4d, 0x8a},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0x49},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0x69},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0x89},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0xa9},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0x48},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0x68},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0x69},
    {0x4e, 0x06},
    {0x4c, 0x01},
    {0x4d, 0x12},
    {0x4e, 0x05},
    {0x4c, 0x01},
    {0x4d, 0x31},
    {0x4e, 0x05},
    {0x4c, 0x01},
    {0x4d, 0x51},
    {0x4e, 0x05},
    {0x4c, 0x01},
    {0x4d, 0x71},
    {0x4e, 0x05},
    {0x4c, 0x02}, // H
    {0x4d, 0xca},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xc9},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xe9},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x09},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xc8},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xe8},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xa7},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xc7},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xe7},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x07},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0x72},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0x31},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0x51},
    {0x4e, 0x07},
    {0x4f, 0x01},
    {0x50, 0x80}, // AWB_PRE_mode
    {0x51, 0xa8}, // AWB_pre_THD_min[7:0]
    {0x52, 0x47}, // AWB_pre_THD_min[15:8] Dominiate luma 0.25=639c 0.22=57a8
    {0x53, 0x38}, // AWB_pre_THD_min_MIX[7:0]
    {0x54, 0xc7}, // AWB_pre_THD_min_MIX[15:8] Mix luma 0.5
    {0x56, 0x0e}, // AWB_tone mode
    {0x58, 0x08}, // AWB_C_num_sel,AWB_D_num_sel
    {0x5b, 0x00}, // AWB_mix_mode
    {0x5c, 0x74}, // green_num0[7:0]
    {0x5d, 0x8b}, // green_num0[15:8] 0.35
    {0x61, 0xdb}, // R2G_stand0   OUT BA c0
    {0x62, 0xb8}, // B2G_stand0       B6  c2
    {0x63, 0x86}, // AWB gray mode [7]enable
    {0x64, 0xc0},
    {0x65, 0x04}, // AWB margin
    {0x67, 0xa8}, // R2G_stand3[7:0]  FF/CWF
    {0x68, 0xb0}, // B2G_stand3[7:0]
    {0x69, 0x00}, // R2G_stand4[9:8] B2G_stand4[9:8] R2G_stand3[9:8] B2G_stand3[9:8]
    {0x6a, 0xa8}, // R2G_stand4[7:0]  TL84/TL84&CWF
    {0x6b, 0xb0}, // B2G_stand4[7:0]
    {0x6c, 0xaf}, // R2G_stand5[7:0]  A
    {0x6d, 0x8b}, // B2G_stand5[7:0]
    {0x6e, 0x50}, // AWB_skin_weight R2G_stand5[9:8] B2G_stand5[9:8]
    {0x6f, 0x18}, // AWB_indoor_THD (0x21=17 caculate)
    {0x73, 0xf0}, // AWB_indoor_mode
    {0x70, 0x0d}, // AWB low luma TH
    {0x71, 0x60}, // AWB outdoor TH
    {0x72, 0x80}, // outdoor mode
    {0x74, 0x01}, //[2:0]AWB skip mode 2x2,4x4,4x8,8x8
    {0x75, 0x01}, //[1:0]AWB_every_N
    {0x7f, 0x0c}, //[3]gray world frame start
    {0x76, 0x70}, // R limit
    {0x77, 0x58}, // G limit
    {0x78, 0xa0}, // B limit
    {0x79, 0x5e},
    {0x7a, 0x54},
    {0x7b, 0x58},
    {0xfe, 0x00},
    //////////////////////////////////////////
    ///////////CC////////////////////////
    //////////////////////////////////////////
    {0xfe, 0x02},
    {0xc0, 0x01}, //[5:4] CC mode [0]CCT enable
    {0xc1, 0x44},
    {0xc2, 0xfd},
    {0xc3, 0x04},
    {0xc4, 0xf0},
    {0xc5, 0x48},
    {0xc6, 0xfd},
    {0xc7, 0x46},
    {0xc8, 0xfd},
    {0xc9, 0x02},
    {0xca, 0xe0},
    {0xcb, 0x45},
    {0xcc, 0xec},
    {0xcd, 0x48},
    {0xce, 0xf0},
    {0xcf, 0xf0},
    {0xe3, 0x0c},
    {0xe4, 0x4b},
    {0xe5, 0xe0},
    //////////////////////////////////////////
    ///////////ABS ////////////////////
    //////////////////////////////////////////
    {0xfe, 0x01},
    {0x9f, 0x40},
    {0xfe, 0x00},
    //////////////////////////////////////
    ///////////  OUTPUT   ////////////////
    //////////////////////////////////////
    {0xfe, 0x00},
    {0xf2, 0x00},
    //////////////frame rate 50Hz/////////
    {0xfe, 0x00},
    {0x05, 0x01}, // hb
    {0x06, 0x56},
    {0x07, 0x00}, // vb
    {0x08, 0x32},
    {0xfe, 0x01},
    {0x25, 0x00},
    {0x26, 0xfa},
    {0x27, 0x04},
    {0x28, 0xe2}, // 20fps
    {0x29, 0x06},
    {0x2a, 0xd6}, // 14fps
    {0x2b, 0x07},
    {0x2c, 0xd0}, // 12fps
    {0x2d, 0x0b},
    {0x2e, 0xb8}, // 8fps
    {0xfe, 0x00},
    ///////////////dark sun////////////////////
    {0xfe, 0x02},
    {0x40, 0xbf},
    {0x46, 0xcf},
    {0xfe, 0x00},
    /////////////////////////////////////////////////////
    //////////////////////   MIPI   /////////////////////
    /////////////////////////////////////////////////////
    {0xfe, 0x03},
    {0x02, 0x22},
    {0x03, 0x10}, // 0x12 20140821
    {0x04, 0x10}, // 0x01 //fifo_prog
    {0x05, 0x00}, // fifo_prog
    {0x06, 0x88}, // YUV ISP data
#if (GC2145MIPI_LANE_NUM == 2)
    {0x01, 0x87},
    {0x10, 0x95}, // last bit  lane num
#else
    {0x01, 0x83},
    {0x10, 0x94},
#endif
    {0x11, 0x1e}, // LDI set YUV422
    {0x12, 0x80}, // 0x80 LWC[7:0]
    {0x13, 0x0c}, // 0x0c LWC[15:8]
    {0x15, 0x10}, // DPHYY_MODE read_ready
    {0x17, 0xf0}, // 01wdiv set
    {0x21, 0x10}, // T_LPX
    {0x22, 0x04}, // T_CLK_HS_PREPAE_set
    {0x23, 0x10}, // T_CLK_zero_set
    {0x24, 0x10}, // T_CLK_PRE
    {0x25, 0x10}, // T_CLK_POST
    {0x26, 0x05}, // T_CLK_TRAIL_set
    {0x29, 0x03}, // T_HS_PREPARE_set
    {0x2a, 0x0a}, // T_HS_Zero_set
    {0x2b, 0x06}, // T_HS_TRAIL
    {0xfe, 0x00}};

static const gc2145_reg_val_t gc2145ResParam_VGA[] = {{0xfe, 0x00},
                                                      {0xfd, 0x00},
#if (GC2145MIPI_LANE_NUM == 2)
                                                      {0xfa, 0x00},
#else
                                                      {0xfa, 0x11},
#endif
                                                      //// crop window
                                                      {0xfe, 0x00},
                                                      {0x90, 0x01},
                                                      {0x91, 0x00},
                                                      {0x92, 0x00},
                                                      {0x93, 0x00},
                                                      {0x94, 0x00},
                                                      {0x95, 0x01},
                                                      {0x96, 0xe0},
                                                      {0x97, 0x02},
                                                      {0x98, 0x80},

                                                      {0x99, 0x55},
                                                      {0x9a, 0x06},
                                                      {0x9b, 0x02},
                                                      {0x9c, 0x00},
                                                      {0x9d, 0x00},
                                                      {0x9e, 0x00},
                                                      {0x9f, 0x02},
                                                      {0xa0, 0x00},
                                                      {0xa1, 0x00},
                                                      {0xa2, 0x00},
                                                      //// AWB
                                                      {0xfe, 0x00},
                                                      {0xec, 0x06},
                                                      {0xed, 0x04},
                                                      {0xee, 0x60},
                                                      {0xef, 0x90},
                                                      {0xfe, 0x01},
                                                      {0x74, 0x01},
                                                      //// AEC
                                                      {0xfe, 0x01},
                                                      {0x01, 0x04},
                                                      {0x02, 0xc0},
                                                      {0x03, 0x04},
                                                      {0x04, 0x90},
                                                      {0x05, 0x30},
                                                      {0x06, 0x90},
                                                      {0x07, 0x30},
                                                      {0x08, 0x80},
                                                      {0x0a, 0x82},
#if (GC2145MIPI_LANE_NUM == 2)
                                                      {0xfe, 0x01},
                                                      {0x21, 0x04},
                                                      {0xfe, 0x00},
                                                      {0x20, 0x03},
#else
                                                      {0xfe, 0x01},
                                                      {0x21, 0x15},
                                                      {0xfe, 0x00},
                                                      {0x20, 0x15},
#endif
                                                      //// mipi
                                                      {0xfe, 0x03},
                                                      {0x12, 0x00},
                                                      {0x13, 0x05},
                                                      {0x04, 0x80},
                                                      {0x05, 0x01},
                                                      {0xfe, 0x00},
                                                      {0xfe, 0x00},
                                                      {0x05, 0x01},
                                                      {0x06, 0x56},
                                                      {0x07, 0x00},
                                                      {0x08, 0x14},
                                                      {0xfe, 0x01},
                                                      {0x25, 0x00},
                                                      {0x26, 0xfa},
                                                      {0x27, 0x04},
                                                      {0x28, 0xe2}, // 20fps
                                                      {0x29, 0x06},
                                                      {0x2a, 0xb6}, // 15fps
                                                      {0x2b, 0x07},
                                                      {0x2c, 0xd0}, // 12fps
                                                      {0x2d, 0x0b},
                                                      {0x2e, 0xb8}, // 8fps
                                                      {0xfe, 0x00}};

static const gc2145_reg_val_t gc2145ResParam_720P[] = {{0xfe, 0x00},
                                                       {0xfd, 0x00},
#if (GC2145MIPI_LANE_NUM == 2)
                                                       {0xfa, 0x00},
#else
                                                       {0xfa, 0x11},
#endif
                                                       //// crop window
                                                       {0xfe, 0x00},
                                                       {0x90, 0x01},
                                                       {0x91, 0x00},
                                                       {0x92, 0xf0},
                                                       {0x93, 0x00},
                                                       {0x94, 0xa0},
                                                       {0x95, 0x02},
                                                       {0x96, 0xd0},
                                                       {0x97, 0x05},
                                                       {0x98, 0x00},

                                                       {0x99, 0x11},
                                                       {0x9a, 0x06},
                                                       {0x9b, 0x00},
                                                       {0x9c, 0x00},
                                                       {0x9d, 0x00},
                                                       {0x9e, 0x00},
                                                       {0x9f, 0x00},
                                                       {0xa0, 0x00},
                                                       {0xa1, 0x00},
                                                       {0xa2, 0x00},
                                                       //// AWB
                                                       {0xfe, 0x00},
                                                       {0xec, 0x06},
                                                       {0xed, 0x04},
                                                       {0xee, 0x60},
                                                       {0xef, 0x90},
                                                       {0xfe, 0x01},
                                                       {0x74, 0x01},
                                                       //// AEC
                                                       {0xfe, 0x01},
                                                       {0x01, 0x04},
                                                       {0x02, 0xc0},
                                                       {0x03, 0x04},
                                                       {0x04, 0x90},
                                                       {0x05, 0x30},
                                                       {0x06, 0x90},
                                                       {0x07, 0x30},
                                                       {0x08, 0x80},
                                                       {0x0a, 0x82},
#if (GC2145MIPI_LANE_NUM == 2)
                                                       {0xfe, 0x01},
                                                       {0x21, 0x04},
                                                       {0xfe, 0x00},
                                                       {0x20, 0x03},
#else
                                                       {0xfe, 0x01},
                                                       {0x21, 0x15},
                                                       {0xfe, 0x00},
                                                       {0x20, 0x15},
#endif
                                                       //// mipi
                                                       {0xfe, 0x03},
                                                       {0x12, 0x00},
                                                       {0x13, 0x0a},
                                                       {0x04, 0x01},
                                                       {0x05, 0x00},
                                                       {0xfe, 0x00},
                                                       {0xfe, 0x00},
                                                       {0x05, 0x01},
                                                       {0x06, 0x56},
                                                       {0x07, 0x01},
                                                       {0x08, 0xe6},
                                                       {0xfe, 0x01},
                                                       {0x25, 0x00},
                                                       {0x26, 0xfa},
                                                       {0x27, 0x04},
                                                       {0x28, 0xe2}, // 20fps
                                                       {0x29, 0x06},
                                                       {0x2a, 0xb6}, // 15fps
                                                       {0x2b, 0x07},
                                                       {0x2c, 0xd0}, // 12fps
                                                       {0x2d, 0x0b},
                                                       {0x2e, 0xb8}, // 8fps
                                                       {0xfe, 0x00}};

static const gc2145_reg_val_t gc2145ResParam_SVGA[] = {{0xfe, 0x00},
                                                       {0xfd, 0x01},
#if (GC2145MIPI_LANE_NUM == 2)
                                                       {0xfa, 0x00},
#else
                                                       {0xfa, 0x11},
#endif
                                                       //// crop window
                                                       {0xfe, 0x00},
                                                       {0x90, 0x01},
                                                       {0x91, 0x00},
                                                       {0x92, 0x00},
                                                       {0x93, 0x00},
                                                       {0x94, 0x00},
                                                       {0x95, 0x02},
                                                       {0x96, 0x58},
                                                       {0x97, 0x03},
                                                       {0x98, 0x20},

                                                       {0x99, 0x11},
                                                       {0x9a, 0x06},
                                                       {0x9b, 0x00},
                                                       {0x9c, 0x00},
                                                       {0x9d, 0x00},
                                                       {0x9e, 0x00},
                                                       {0x9f, 0x00},
                                                       {0xa0, 0x00},
                                                       {0xa1, 0x00},
                                                       {0xa2, 0x00},
                                                       //// AWB
                                                       {0xfe, 0x00},
                                                       {0xec, 0x02},
                                                       {0xed, 0x02},
                                                       {0xee, 0x30},
                                                       {0xef, 0x48},
                                                       {0xfe, 0x02},
                                                       {0x9d, 0x0b},
                                                       {0xfe, 0x01},
                                                       {0x74, 0x00},
                                                       //// AEC
                                                       {0xfe, 0x01},
                                                       {0x01, 0x04},
                                                       {0x02, 0x60},
                                                       {0x03, 0x02},
                                                       {0x04, 0x48},
                                                       {0x05, 0x18},
                                                       {0x06, 0x50},
                                                       {0x07, 0x10},
                                                       {0x08, 0x38},
                                                       {0x0a, 0x80},
#if (GC2145MIPI_LANE_NUM == 2)
                                                       {0xfe, 0x01},
                                                       {0x21, 0x04},
                                                       {0xfe, 0x00},
                                                       {0x20, 0x03},
#else
                                                       {0xfe, 0x01},
                                                       {0x21, 0x15},
                                                       {0xfe, 0x00},
                                                       {0x20, 0x15},
#endif

                                                       //// mipi
                                                       {0xfe, 0x03},
                                                       {0x12, 0x40},
                                                       {0x13, 0x06},
#if (GC2145MIPI_LANE_NUM == 2)
                                                       {0x04, 0x90},
                                                       {0x05, 0x01},
#else
                                                       {0x04, 0x01},
                                                       {0x05, 0x00},
#endif
                                                       {0xfe, 0x00},
                                                       {0xfe, 0x00},
                                                       {0x05, 0x01},
                                                       {0x06, 0x56},
                                                       {0x07, 0x00},
                                                       {0x08, 0x32},
                                                       {0xfe, 0x01},
                                                       {0x25, 0x00},
                                                       {0x26, 0xfa},
                                                       {0x27, 0x04},
                                                       {0x28, 0xe2}, // 20fps
                                                       {0x29, 0x06},
                                                       {0x2a, 0xd6}, // 14fps
                                                       {0x2b, 0x07},
                                                       {0x2c, 0xd0}, // 12fps
                                                       {0x2d, 0x0b},
                                                       {0x2e, 0xb8}, // 8fps
                                                       {0xfe, 0x00}};

static const gc2145_reg_val_t gc2145ResParam_UXGA[] = {{0xfe, 0x00},
                                                       {0xfd, 0x00},
#if (GC2145MIPI_LANE_NUM == 2)
                                                       {0xfa, 0x00},
#else
                                                       {0xfa, 0x11},
#endif
                                                       //// crop window
                                                       {0xfe, 0x00},
                                                       {0x90, 0x01},
                                                       {0x91, 0x00},
                                                       {0x92, 0x00},
                                                       {0x93, 0x00},
                                                       {0x94, 0x00},
                                                       {0x95, 0x04},
                                                       {0x96, 0xb0},
                                                       {0x97, 0x06},
                                                       {0x98, 0x40},
                                                       {0x99, 0x11},
                                                       {0x9a, 0x06},
                                                       {0x9b, 0x00},
                                                       {0x9c, 0x00},
                                                       {0x9d, 0x00},
                                                       {0x9e, 0x00},

                                                       {0x9f, 0x00},
                                                       {0xa0, 0x00},
                                                       {0xa1, 0x00},
                                                       {0xa2, 0x00},
                                                       //// AWB
                                                       {0xfe, 0x00},
                                                       {0xec, 0x06},
                                                       {0xed, 0x04},
                                                       {0xee, 0x60},
                                                       {0xef, 0x90},
                                                       {0xfe, 0x01},
                                                       {0x74, 0x01},
                                                       //// AEC
                                                       {0xfe, 0x01},
                                                       {0x01, 0x04},
                                                       {0x02, 0xc0},
                                                       {0x03, 0x04},
                                                       {0x04, 0x90},
                                                       {0x05, 0x30},
                                                       {0x06, 0x90},
                                                       {0x07, 0x30},
                                                       {0x08, 0x80},
                                                       {0x0a, 0x82},
#if (GC2145MIPI_LANE_NUM == 2)
                                                       {0xfe, 0x01},
                                                       {0x21, 0x04},
                                                       {0xfe, 0x00},
                                                       {0x20, 0x03},
#else
                                                       {0xfe, 0x01},
                                                       {0x21, 0x15},
                                                       {0xfe, 0x00},
                                                       {0x20, 0x15},
#endif
                                                       //// mipi
                                                       {0xfe, 0x03},
                                                       {0x12, 0x80},
                                                       {0x13, 0x0c},
                                                       {0x04, 0x01},
                                                       {0x05, 0x00},
                                                       {0xfe, 0x00},

                                                       {0xfe, 0x00},
                                                       {0x05, 0x01},
                                                       {0x06, 0x56},
                                                       {0x07, 0x00},
                                                       {0x08, 0x32},
                                                       {0xfe, 0x01},
                                                       {0x25, 0x00},
                                                       {0x26, 0xfa},

                                                       {0x27, 0x04},
                                                       {0x28, 0xe2}, // 20fps
                                                       {0x29, 0x06},
                                                       {0x2a, 0xd6}, // 14fps
                                                       {0x2b, 0x07},
                                                       {0x2c, 0xd0}, // 12fps
                                                       {0x2d, 0x0b},
                                                       {0x2e, 0xb8}, // 8fps
                                                       {0xfe, 0x00}};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief send multiple registers via I2C bus.
 * @param handle Handle to the camera pointer.
 * @param regs registers table.
 * @param num number of registers to be sent
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static status_t GC2145_MultiWrite(camera_device_handle_t *handle, const gc2145_reg_val_t regs[], uint32_t num)
{
    status_t status = kStatus_Success;

    for (uint32_t i = 0; i < num; i++)
    {
        status = GC2145_Write(handle, regs[i].reg, 1U, regs[i].value);

        if (kStatus_Success != status)
        {
            break;
        }
    }
    return status;
}

/*!
 * @brief Reset GC2145 camera.
 * This function pulls the camera reset line during 50ms.
 * @param handle Handle to the camera pointer.
 */
static status_t GC2145_Reset(camera_device_handle_t *handle)
{
    /* Hard reset */
    // The function that pullResetPin points to does nothing. No need to wait for 60ms.
    //((gc2145_resource_t *)(handle->resource))->pullResetPin(false);
    // GC2145_DelayMs(50);
    ((gc2145_resource_t *)(handle->resource))->pullResetPin(true);
    GC2145_DelayMs(10);

    return kStatus_Success;
}

/*!
 * @brief Enable / Disable GC2145 power-down.
 * @param handle Handle to the camera pointer.
 * @param powerdown 1 to activate the power-down mode, 0 to activate the normal mode
 */
static status_t GC2145_PowerDown(camera_device_handle_t *handle, bool powerdown)
{
    ((gc2145_resource_t *)(handle->resource))->pullPowerDownPin(powerdown);

    return kStatus_Success;
}

/*!
 * @brief Applies the initial settings to GC2145 sensor.
 * Note that GC2145 initial registers should not be changed here.
 * @param handle Handle to the camera pointer.
 */
static void GC2145_SensorInitialSetting(camera_device_handle_t *handle)
{
    GC2145_MultiWrite(handle, gc2145InitParam, ARRAY_SIZE(gc2145InitParam));
}

/*!
 * @brief select and enable a picture effect.
 * @param handle Handle to the camera pointer.
 * @param white_balance gc2145_cam_effect_t configuration.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static status_t GC2145_SetEffect(camera_device_handle_t *handle, gc2145_cam_effect_t effect)
{
    GC2145_Write(handle, 0xfe, 1U, 0x00); /* set page0 */
    switch (effect)
    {
        case kcam_effect_enc_normal:
            GC2145_Write(handle, 0x83, 1U, 0x00);
            break;
        case kcam_effect_enc_grayscale:
            GC2145_Write(handle, 0x83, 1U, 0x01);
            break;
        default:
            GC2145_Write(handle, 0x83, 1U, 0x00);
            break;
    }
    return kStatus_Success;
}

/*!
 * @brief Enable test patterns.
 *  This function allows to output test patterns(VGA) supported by the GC2145.
 * @param handle Handle to the camera pointer.
 * @param mode pattern mode to be used.
 * @param enable enable / disable the test mode.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static status_t GC2145_SetVGATestMode(camera_device_handle_t *handle, gc2145_testmode_t mode, bool enable)
{
    status_t status = kStatus_Success;
    uint8_t test1, test2;

    status = GC2145_Write(handle, 0xfe, 1U, 0x00); /* set page0 */
    if (kStatus_Success != status)
    {
        return status;
    }

    if (enable)
    {
        if (mode == ktest_image_solidred)
        {
            test1 = 0x04;
            test2 = (5 << 4) | 0x08;
        }
        else // ktest_image_colorbar
        {
            test1 = 0x04;
            test2 = 0x01;
        }
    }
    else
    {
        test1 = 0x00;
        test2 = 0x01;
    }

    GC2145_Write(handle, 0x8c, 1U, test1);
    GC2145_Write(handle, 0x8d, 1U, test2);
    return status;
}

/*!
 * @brief Initialize GC2145 sensor.
 * Reset the sensor, disable the power-down mode and write the sensor settings via I2C.
 * @param handle Handle to the camera pointer.
 * @param config pointer to camera_config_t configuration.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static status_t GC2145_Init(camera_device_handle_t *handle, const camera_config_t *config)
{
    status_t status;
    uint16_t chip_id;

    /* Only support 2 data lanes. */
    if ((kCAMERA_InterfaceMIPI == config->interface) && (2U != config->csiLanes))
    {
        return kStatus_InvalidArgument;
    }

    /* support 640x480 (VGA), 800x600(SVGA), 1280x720(720p), 1600x1200(UXGA) resolutions */
    if ((kVIDEO_ResolutionVGA != config->resolution) && (FSL_VIDEO_RESOLUTION(800, 600) != config->resolution) &&
        (kVIDEO_Resolution720P != config->resolution) && (FSL_VIDEO_RESOLUTION(1600, 1200) != config->resolution))
    {
        return kStatus_InvalidArgument;
    }

    /* support 5~30 fps. */
    if (15 != config->framePerSec && 20 != config->framePerSec && 30 != config->framePerSec &&
        10 != config->framePerSec && 5 != config->framePerSec)
    {
        return kStatus_InvalidArgument;
    }

    /* support YUV422 */
    if ((kVIDEO_PixelFormatYUYV != config->pixelFormat) && (kVIDEO_PixelFormatYVYU != config->pixelFormat) &&
        (kVIDEO_PixelFormatUYVY != config->pixelFormat) && (kVIDEO_PixelFormatVYUY != config->pixelFormat))
    {
        return kStatus_InvalidArgument;
    }

    GC2145_Reset(handle);

    /* it is mandatory to deactivate the power-down, otherwise the sensor does not answer on the I2C bus */
    GC2145_PowerDown(handle, false);

    /* Identify the device. */
    status = GC2145_Read(handle, GC2145_REG_CHIP_ID_H, 2u, &chip_id);
    if (kStatus_Success != status)
    {
        return status;
    }

    if (GC2145_CHIP_ID != chip_id)
    {
        return kStatus_Fail;
    }

    GC2145_SensorInitialSetting(handle);

    switch (config->resolution)
    {
        case FSL_VIDEO_RESOLUTION(1600, 1200):
            status = GC2145_MultiWrite(handle, gc2145ResParam_UXGA, ARRAY_SIZE(gc2145ResParam_UXGA));
            break;
        case kVIDEO_Resolution720P:
            status = GC2145_MultiWrite(handle, gc2145ResParam_720P, ARRAY_SIZE(gc2145ResParam_720P));
            break;
        case kVIDEO_ResolutionVGA:
            status = GC2145_MultiWrite(handle, gc2145ResParam_VGA, ARRAY_SIZE(gc2145ResParam_VGA));
            // GC2145_SetVGATestMode(handle, ktest_image_solidred, true);
            break;
        case FSL_VIDEO_RESOLUTION(800, 600):
        default:
            status = GC2145_MultiWrite(handle, gc2145ResParam_SVGA, ARRAY_SIZE(gc2145ResParam_SVGA));
            break;
    }

    if (kStatus_Success != status)
    {
        return status;
    }

    return kStatus_Success;
}

/*!
 * @brief Deinitialize GC2145 sensor.
 * Reset the sensor and enable the power-down mode
 * @param handle Handle to the camera pointer.
 * @return Allways returns @ref kStatus_Success.
 */
static status_t GC2145_Deinit(camera_device_handle_t *handle)
{
    GC2145_Reset(handle);
    GC2145_PowerDown(handle, true);

    return kStatus_Success;
}

/*!
 * @brief Start GC2145 sensor.
 * Actually does nothing !
 * @param handle Handle to the camera pointer.
 * @return Allways returns @ref kStatus_Success.
 */
static status_t GC2145_Start(camera_device_handle_t *handle)
{
    GC2145_PowerDown(handle, false);
    return kStatus_Success;
}

/*!
 * @brief Stop GC2145 sensor.
 * Actually does nothing !
 * @param handle Handle to the camera pointer.
 * @return Allways returns @ref kStatus_Success.
 */
static status_t GC2145_Stop(camera_device_handle_t *handle)
{
    GC2145_PowerDown(handle, true);
    return kStatus_Success;
}

/*!
 * @brief Control GC2145 sensor.
 * Actually does nothing !
 * @param handle Handle to the camera pointer.
 * @return Allways returns @ref kStatus_Success.
 */
static status_t GC2145_Control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg)
{
    if (cmd == kCAMERA_DeviceMonoMode)
    {
        if (arg)
        {
            GC2145_SetEffect(handle, kcam_effect_enc_grayscale);
        }
        else
        {
            GC2145_SetEffect(handle, kcam_effect_enc_normal);
        }
        return kStatus_Success;
    }
    else if (cmd == kCAMERA_DeviceExposureMode)
    {
        GC2145_Write(handle, 0xfe, 1U, 0x00); /* set page0 */
        switch (arg)
        {
            case CAMERA_EXPOSURE_MODE_AUTO:
                GC2145_Write(handle, 0xb6, 1U, 0x01); /* enable AEC */
                break;
            case CAMERA_EXPOSURE_MODE_MANU:
                GC2145_Write(handle, 0xb6, 1U, 0x00); /* close AEC */
                break;
            default:
                break;
        }
        return kStatus_Success;
    }
    else if (cmd == kCAMERA_DeviceBrightnessAdjust)
    {
#define GC2145_BRIGHTNESS_EXPOSURE_DEFAULT       0x96
#define GC2145_BRIGHTNESS_EXPOSURE_MAX           ((1UL << 12) - 1)
#define GC2145_BRIGHTNESS_EXPOSURE_INC_DEC_RATIO (0.25f)
        // adjust brightness manually
        uint8_t exposureH, exposureL;
        GC2145_Write(handle, 0xfe, 1U, 0x00); /* set page0 */
        GC2145_Write(handle, 0xb6, 1U, 0x00); /* close AEC */
        GC2145_Read(handle, 0x3, 1u, &exposureH);
        GC2145_Read(handle, 0x4, 1u, &exposureL);
        uint16_t exposure = exposureH & 0xF;
        exposure          = (exposure << 8) | exposureL;
        switch (arg)
        {
            case CAMERA_BRIGHTNESS_DECREASE:
                exposure *= (1 - GC2145_BRIGHTNESS_EXPOSURE_INC_DEC_RATIO);
                GC2145_Write(handle, 0x4, 1U, exposure & 0xFF);
                GC2145_Write(handle, 0x3, 1U, exposure >> 8);
                break;
            case CAMERA_BRIGHTNESS_INCREASE:
                exposure *= (1 + GC2145_BRIGHTNESS_EXPOSURE_INC_DEC_RATIO);
                if (exposure >= GC2145_BRIGHTNESS_EXPOSURE_MAX)
                {
                    exposure = GC2145_BRIGHTNESS_EXPOSURE_MAX;
                }
                GC2145_Write(handle, 0x4, 1U, exposure & 0xFF);
                GC2145_Write(handle, 0x3, 1U, exposure >> 8);
                break;
            case CAMERA_BRIGHTNESS_DEFAULT:
                GC2145_Write(handle, 0x4, 1U, GC2145_BRIGHTNESS_EXPOSURE_DEFAULT & 0xFF);
                GC2145_Write(handle, 0x3, 1U, GC2145_BRIGHTNESS_EXPOSURE_DEFAULT >> 8);
                break;
            default:
                break;
        }
        return kStatus_Success;
    }
    else
    {
        return kStatus_InvalidArgument;
    }
}

/*!
 * @brief Initialize GC2145 sensor.
 * Launch GC2145_Init function.
 * @param handle Handle to the camera pointer.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t GC2145_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig)
{
    return GC2145_Init(handle, config);
}
