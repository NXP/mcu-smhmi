/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief hal output device declaration. Output devices receive event notifications via the output manager and react to
 * those events accordingly. For example, an LED output device may want to change colors/brightness based on detection
 * of a face. Output devices can include things like LEDs, speakers, and more.
 */

#ifndef _HAL_OUTPUT_DEV_H_
#define _HAL_OUTPUT_DEV_H_

#include "fwk_common.h"
#include "hal_graphics_dev.h"

/**
 * @brief declare the output dev ##name
 */
#define HAL_OUTPUT_DEV_DECLARE(name) int HAL_OutputDev_##name##_Register();

/**
 * @brief register the output dev ##name
 */
#define HAL_OUTPUT_DEV_REGISTER(name, ret)                             \
    {                                                                  \
        ret = HAL_OutputDev_##name##_Register();                       \
        if (ret != 0)                                                  \
        {                                                              \
            LOGE("HAL_OutputDev_%s_Register error %d", "##name", ret); \
            return ret;                                                \
        }                                                              \
    }

typedef struct _output_dev output_dev_t;

/*! @brief Types of output devices' callback messages */
typedef enum _output_event_id
{
    kOutputEvent_SpeakerToAfeFeedback  = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_Output, 1),
    kOutputEvent_VisionAlgoInputNotify = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_Output, 2),
    kOutputEvent_VoiceAlgoInputNotify  = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_Output, 3),
    kOutputEvent_OutputInputNotify     = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_Output, 4),
    kOutputEvent_DisplayInputNotify    = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_Output, 5),

    kOutputEvent_Count
} output_event_id_t;

/*! @brief Structure used to define an event.*/
typedef struct _output_event
{
    /* Eventid from the list above.*/
    output_event_id_t eventId;
    event_info_t eventInfo;
    /* Pointer to a struct of data that needs to be forwarded. */
    void *data;
    /* Size of the struct that needs to be forwarded. */
    unsigned int size;
    /* If copy is set to 1, the framework will forward a copy of the data. */
    unsigned char copy;
} output_event_t;

/*!
 * @brief Callback function to notify managers the results of Output devices' inference
 * @param devId Device ID
 * @param event Event which took place
 * @param fromISR True if this operation takes place in an irq, 0 otherwise
 * @return 0 if the operation was successfully
 */
typedef int (*output_dev_callback_t)(int devId, output_event_t event, uint8_t fromISR);

/*! @brief Sources for the output messages */
typedef enum _output_algo_source
{
    kOutputAlgoSource_Vision,
    kOutputAlgoSource_Voice,
    kOutputAlgoSource_LPM,
    kOutputAlgoSource_Other,
} output_algo_source_t;

/*! @brief Types of output devices */
typedef enum _output_dev_type
{
    kOutputDevType_UI,
    kOutputDevType_Audio,
    kOutputDevType_Other,
} output_dev_type_t;

/*! @brief Error codes for output hal devices */
typedef enum _hal_output_status
{
    kStatus_HAL_OutputSuccess = 0,                                                       /*!< Successfully */
    kStatus_HAL_OutputError   = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Output, 1), /*!< Error occurs */
} hal_output_status_t;

typedef struct _output_dev_attr_t
{
    output_dev_type_t type;
    union
    {
        gfx_surface_t *pSurface;
        void *reserve;
    };
} output_dev_attr_t;

/*! @brief Operation that needs to be implemented by an output device */
typedef struct _output_dev_operator
{
    /* initialize the dev */
    hal_output_status_t (*init)(output_dev_t *dev, output_dev_callback_t callback);
    /* deinitialize the dev */
    hal_output_status_t (*deinit)(const output_dev_t *dev);
    /* start the dev */
    hal_output_status_t (*start)(const output_dev_t *dev);
    /* start the dev */
    hal_output_status_t (*stop)(const output_dev_t *dev);

} output_dev_operator_t;

typedef struct _output_dev_private_capability
{
    output_dev_callback_t callback;
} output_dev_private_capability_t;

/*! @brief Attributes of an output device */
struct _output_dev
{
    int id;
    char name[DEVICE_NAME_MAX_LENGTH];
    /* attributes */
    output_dev_attr_t attr;
    hal_device_config configs[MAXIMUM_CONFIGS_PER_DEVICE];

    /* operations */
    const output_dev_operator_t *ops;
    /* private capability */
    output_dev_private_capability_t cap;
};

#endif /*_HAL_OUTPUT_DEV_H_*/
