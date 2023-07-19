/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.d
 *
 * Brief: uvita neural network algorithm header file
 *
 * Author: michael.wang@nxp.com
 * Date: 2022.10.27
 */

#ifndef _UVITA_H_
#define _UVITA_H_

#include <stdint.h>
/*
 * @brief the supported input pixel format.
 */
typedef enum
{
    UVITA_PRESENT_RED = 0, /**< UVITA present red color */
    UVITA_PRESENT_BLUE,    /**< UVITA present blue color */
    UVITA_PRESENT_GREEN,   /**< UVITA present green color */
    UVITA_PRESENT_YELLOW,  /**< UVITA present yellow color */
    UVITA_PRESENT_BLACK,   /**< UVITA present black color */
    UVITA_PRESENT_WHITE,   /**< UVITA present white color */
    UVITA_PRESENT_PURPLE,  /**< UVITA present purple color */
    UVITA_COLOR_NUM,
} uvita_present_color;

/*
 * @brief the supported input pixel format.
 */
typedef enum
{
    UVITA_IMG_INT8_RGB888 = 0, /**< RGB888 from low address to high */
    UVITA_IMG_INT8_BGR888,     /**< BGR888 from low address to high */
    UVITA_IMG_INT8_GRAY,       /**< Gray8 */
    UVITA_IMG_FLOAT_RGB,       /**< RGB float format */
    UVITA_IMG_FLOAT_BGR,       /**< BGR float format */
    UVITA_IMG_FLOAT_GRAY,      /**< Gray float format */
    UVITA_IMG_INT16_RGB565,    /**< RGB65 format */
    UVITA_IMG_INT16_BGR565,    /**< BGR565 format */
} uvita_image_format;

/*
 * @brief image frame information
 */
typedef struct
{
    uint16_t height;        /**< height */
    uint16_t width;         /**< width */
    uvita_image_format fmt; /**< input image format */
    uint8_t *data;          /**< buffer with input data */
    uint8_t reserved[3];
} uvita_image_frame;

/*
 * @brief rectangle index for left top and right bottom point
 */
typedef enum
{
    RECT_LEFT_X,   /**< X-axis of left-top corner*/
    RECT_TOP_Y,    /**< Y-axis of left-top corner*/
    RECT_RIGHT_X,  /**< X-axis of right-bottom corner*/
    RECT_BOTTOM_Y, /**< Y-axis of right-bottom corner*/
    RECT_NUM,
} uvita_rect_index;

/*
 * @brief bounding box information
 */
typedef struct DetBox
{
    int rect[RECT_NUM]; /**< left, top, right, bottom */
    float score;        /**< detection score */
    float area;         /**< detection area */
    int reserved;
} DetBox_t;

/**
 * @brief uvita parameter
 */
typedef struct
{
    int input_height;    /**< the maximum height of the input frame */
    int input_wdith;     /**< the maximum width of the input frame */
    int mem_size;        /**< memory pool size */
    char *mem_pool;      /**< memory pool pointer */
    int fast_mem_size;   /**< memory pool size */
    char *fast_mem_pool; /**< memory pool pointer */
    void *reserved;      /**< reserved for internal use */
} uvita_param;

/**
 * @brief Error status returned by some functions in the uvita library.
 */
typedef enum
{
    UVITA_SUCCESS         = 0, /**< No error */
    UVITA_ARGUMENT_ERROR  = 1, /**< One or more arguments are incorrect */
    UVITA_MEMORY_ERROR    = 2, /**< Length of data buffer is incorrect */
    UVITA_UNSUPPORT_ERROR = 3, /**< Function is not supported */
    UVITA_NOT_READY       = 4, /**< Function is not ready but planed to support */
} uvita_status;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief uvita initialization
 *
 * @param[in]      para            uvita algorithm initialization parameter
 *
 * @return         function status
 *
 */
uvita_status uvita_init(uvita_param *para);

#ifdef __cplusplus
}

#endif

#endif /* UVITA_H_ */
