/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief vision and voice algorithms device declaration. Vision + voice algorithm devices include ML/AI models like the
 * OasisLite runtime engine for face recognition.
 */

#ifndef _HAL_VALGO_DEV_H_
#define _HAL_VALGO_DEV_H_

#include "fwk_common.h"

/**
 * @brief declare the vision algorithm dev ##name
 */
#define HAL_VALGO_DEV_DECLARE(name) int HAL_VisionAlgoDev_##name##_Register();

/**
 * @brief register the vision algorithm dev ##name
 */
#define HAL_VALGO_DEV_REGISTER(name, ret)                                  \
    {                                                                      \
        ret = HAL_VisionAlgoDev_##name##_Register();                       \
        if (ret != 0)                                                      \
        {                                                                  \
            LOGE("HAL_VisionAlgoDev_%s_Register error %d", "##name", ret); \
            return ret;                                                    \
        }                                                                  \
    }

/**
 * @brief declare the voice algorithm dev ##name
 */
#define HAL_VOICEALGO_DEV_DECLARE(name) int HAL_VoiceAlgoDev_##name##_Register();

/**
 * @brief register the voice algorithm dev ##name
 */
#define HAL_VOICEALGO_DEV_REGISTER(name, ret)                             \
    {                                                                     \
        ret = HAL_VoiceAlgoDev_##name##_Register();                       \
        if (ret != 0)                                                     \
        {                                                                 \
            LOGE("HAL_VoiceAlgoDev_%s_Register error %d", "##name", ret); \
            return ret;                                                   \
        }                                                                 \
    }

typedef enum _valgo_event_id
{
    /* Vision and voice algorithms result update */
    kVAlgoEvent_VisionResultUpdate = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_VAlgo, 1),
    kVAlgoEvent_VoiceResultUpdate  = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_VAlgo, 2),
    /* Vision algorithm tries to control LED PWM or CAMERA EXPOSURE */
    kVAlgoEvent_VisionLedPwmControl = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_VAlgo, 3),
    kVAlgoEvent_VisionCamExpControl = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_VAlgo, 4),
    kVAlgoEvent_VisionRecordControl = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_VAlgo, 5),

    kVAlgoEvent_RequestFrame = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_VAlgo, 6),

    /* Voice algo should notify AFE about detected wake words */
    kVAlgoEvent_AsrToAfeFeedback = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_VAlgo, 7),
    kVAlgoEvent_AsrToAudioDump   = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_VAlgo, 8),

    /* Voice algo notify AFE about samples processed */
    kVAlgoEvent_AsrToAfeProcessed = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_VAlgo, 9),

    kVAlgoEvent_Count
} valgo_event_id_t;

/*! @brief Structure used to define an event.*/
typedef struct _valgo_event
{
    /* Eventid from the list above.*/
    valgo_event_id_t eventId;
    event_info_t eventInfo;
    /* Pointer to a struct of data that needs to be forwarded. */
    void *data;
    /* Size of the struct that needs to be forwarded. */
    unsigned int size;
    /* If copy is set to 1, the framework will forward a copy of the data. */
    unsigned char copy;
} valgo_event_t;

/*!
 * @brief Callback function to notify managers the results of inference
 * @param valgo_dev* dev Pointer to an algorithm device
 * @param valgo_event_t event Event which took place
 * @param fromISR True if this operation takes place in an irq, 0 otherwise
 */
typedef int (*valgo_dev_callback_t)(int devId, valgo_event_t event, uint8_t fromISR);

/*! @brief Valgo Error codes for hal operations */
typedef enum _hal_valgo_status
{
    kStatus_HAL_ValgoSuccess = 0, /*!< Successfully */

    kStatus_HAL_ValgoMallocError =
        MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_VAlgo, 1), /*!< memory allocation failed for HAL algorithm */
    kStatus_HAL_ValgoInitError =
        MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_VAlgo, 2), /*!< algorithm initialization error */
    kStatus_HAL_ValgoError =
        MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_VAlgo, 3), /*!< Error occurs in HAL algorithm */

    kStatus_HAL_ValgoStop = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_VAlgo, 4), /*!< HAL algorithm stop */

} hal_valgo_status_t;

typedef struct _valgo_dev_private_capability
{
    /* callback */
    valgo_dev_callback_t callback;
    /* param for the callback */
    void *param;
} valgo_dev_private_capability_t;

/*! @brief Characteristics that need to be defined by a vision algo. */

typedef struct _vision_frame
{
    /* is supported by the device for this type of frame */
    /* vision algo manager will only request the supported frame for this device */
    int is_supported;

    /* frame resolution */
    int height;
    int width;
    int pitch;

    /* rotate degree */
    cw_rotate_degree_t rotate;
    flip_mode_t flip;
    /* swap byte per two bytes */
    int swapByte;

    /* pixel format */
    pixel_format_t format;

    /* the source pixel format of the requested frame */
    pixel_format_t srcFormat;
    void *data;
} vision_frame_t;

typedef struct
{
    int autoStart;
    /* frame type definition */
    vision_frame_t frames[kVAlgoFrameID_Count];
} vision_algo_private_data_t;

typedef struct _vision_algo_dev vision_algo_dev_t;

/*! @brief Operation that needs to be implemented by a vision algorithm device */
typedef struct
{
    /* initialize the dev */
    hal_valgo_status_t (*init)(vision_algo_dev_t *dev, valgo_dev_callback_t callback, void *param);
    /* deinitialize the dev */
    hal_valgo_status_t (*deinit)(vision_algo_dev_t *dev);
    /* start the dev */
    hal_valgo_status_t (*run)(const vision_algo_dev_t *dev, void *data);
    /* recv events */
    hal_valgo_status_t (*inputNotify)(const vision_algo_dev_t *receiver, void *data);

} vision_algo_dev_operator_t;

/*! @brief Attributes of a vision algo device */
struct _vision_algo_dev
{
    /* unique id which is assigned by algorithm manager during the registration */
    int id;
    /* name to identify */
    char name[DEVICE_NAME_MAX_LENGTH];
    /* private capability */
    valgo_dev_private_capability_t cap;
    /* operations */
    vision_algo_dev_operator_t *ops;
    /* private data */
    vision_algo_private_data_t data;
};

typedef struct _voice_algo_private_data
{
} voice_algo_private_data_t;

typedef struct _voice_algo_dev voice_algo_dev_t;

/*! @brief Operation that needs to be implemented by a voice algorithm device */
typedef struct voice_algo_dev_operator_t
{
    /* initialize the dev */
    hal_valgo_status_t (*init)(voice_algo_dev_t *dev, valgo_dev_callback_t callback, void *param);
    /* deinitialize the dev */
    hal_valgo_status_t (*deinit)(voice_algo_dev_t *dev);
    /* start the dev */
    hal_valgo_status_t (*run)(const voice_algo_dev_t *dev, void *data);
    /* recv events */
    hal_valgo_status_t (*inputNotify)(const voice_algo_dev_t *receiver, void *data);

} voice_algo_dev_operator_t;

/*! @brief Attributes of a voice algo device */
struct _voice_algo_dev
{
    /* unique id which is assigned by algorithm manager during the registration */
    int id;
    /* name to identify */
    char name[DEVICE_NAME_MAX_LENGTH];
    /* private capability */
    valgo_dev_private_capability_t cap;
    /* operations */
    voice_algo_dev_operator_t *ops;
    /* private data */
    voice_algo_private_data_t data;
};

#endif /*_HAL_VALGO_DEV_H_*/
