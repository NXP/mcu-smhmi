/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.d
 *
 * Brief: uvita hand gesture recognition solution
 * Version: V.0.2
 * Date: 2022.12.20
 */

#ifndef _UVITA_GESTURE_H_
#define _UVITA_GESTURE_H_

#include "uvita.h"

#define HAND_LANDMARK_NUMBER 21

/*
 * @brief the left or right hand.
 */
typedef enum
{
    LEFT_HAND  = 0, /**< The left hand */
    RIGHT_HAND = 1, /**< The right hand */
    NONE_HAND  = 2, /**< No hand */
} hand_lr;

/*
 * @brief the gesture definition
 */
typedef enum
{
    GESTURE_OK = 0,
    GESTURE_FIST,
    GESTURE_ONE,
    GESTURE_VICTORY,
    GESTURE_THREE,
    GESTURE_FOUR,
    GESTURE_PALM,
    GESTURE_THUMB_UP,
    GESTURE_PINKY_UP,
    GESTURE_ROCK_ROLL,
    GESTURE_CALL_ME,
    GESTURE_PREVIOUS,
    GESTURE_NEXT,
    GESTURE_NONE
} gesture_type;

/*
 * @brief uvita hand gesture information
 */
typedef struct
{
    bool has_hand;                            /**< whether the hand is detected or not; */
    DetBox_t box;                             /**< hand location: left, top, right, bottom. */
    float conf;                               /**< hand confidence. */
    hand_lr left_right;                       /**< left hand or right hand. */
    gesture_type gtype;                       /**< gesture type. */
    float handmark[HAND_LANDMARK_NUMBER * 3]; /**< hand landmark location: x1, y1, z1, x2, y2, z2... */
} uvita_gesture_out;

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief calculate the needed buffer size for uvita gesture
 *
 * @param[in]      height            the maximum height of the input frame
 * @param[in]      width             the maximum width of the input frame
 *
 * @return         buffer size
 *
 */
int uvita_gesture_get_buf_size(int height, int width);

/**
 * @brief uvita gesture initialization
 * note that if the gesture_init is used, the uvita_init is not needed anymore.
 *
 * @param[in]      para            uvita algorithm initialization parameter
 *
 * @return         function status
 *
 */
uvita_status uvita_gesture_init(uvita_param *para);

/**
 * @brief uvita runtime function
 *
 * @param[in]      full_frame      original frame
 * @param[in]      det_frame       frame for detection faster with smaller input, could be the same as full_frame
 * @param[out]     uout            uvita gesture algorithm output
 *
 * @return         function status
 *
 */
uvita_status uvita_gesture_run(uvita_image_frame *full_frame, uvita_image_frame *det_frame, uvita_gesture_out *hout);

#ifdef __cplusplus
}
#endif

#endif /*_UVITA_HAND_LANDMARK_H_*/
