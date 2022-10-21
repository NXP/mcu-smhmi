/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief hal input device declaration. Input devices are used to generate input events which other devices can react to
 * by receiving an "InputNotify" message from their respective managers. Examples of input devices include push buttons,
 * PIR sensors, and USB and UART Shell implementations.
 */

#ifndef _HAL_INPUT_DEV_H_
#define _HAL_INPUT_DEV_H_

#include "fwk_common.h"

/**
 * @brief declare the input dev ##name
 */
#define HAL_INPUT_DEV_DECLARE(name) int HAL_InputDev_##name##_Register();

/**
 * @brief register the input dev ##name
 */
#define HAL_INPUT_DEV_REGISTER(name, ret)                             \
    {                                                                 \
        ret = HAL_InputDev_##name##_Register();                       \
        if (ret != 0)                                                 \
        {                                                             \
            LOGE("HAL_InputDev_%s_Register error %d", "##name", ret); \
            return ret;                                               \
        }                                                             \
    }

typedef struct _input_dev input_dev_t;

typedef enum _input_event_id
{
    kInputEventID_Recv,
    kInputEventID_AudioRecv,
    kInputEventID_FrameworkRecv,
} input_event_id_t;

typedef struct _input_event
{
    input_event_id_t eventId;
    uint32_t eventInfo;
    unsigned int size;
    union
    {
        /* Valid when message is kInputEventID_RECV */
        struct input_data
        {
            /* Pointer to a struct of data that needs to be forwarded.*/
            void *data;
            /* If copy is set to 1, the framework will forward a copy of the data. */
            unsigned char copy;
            /* List of tasks that need to receive this message eg receiverList = 1 << kFWKTaskID_VisionAlgo */
            unsigned int receiverList;
        } inputData;

        /* Valid when eventId is kInputEventID_AudioRECV */
        void *audioData;

        /* Valid when framework information is needed GET_FRAMEWORK_INFO*/
        framework_request_t *frameworkRequest;
    } u;
} input_event_t;

/**
 * @brief callback function to notify input manager with an async event
 * @param dev Device structure
 * @param event Event struct.
 * @param fromISR True if this operation takes place in an irq, 0 otherwise
 * @return 0 if the operation was successfully
 */
typedef int (*input_dev_callback_t)(const input_dev_t *dev, input_event_t *event, uint8_t fromISR);

/*! @brief Error codes for input hal devices */
typedef enum _hal_input_status
{
    kStatus_HAL_InputSuccess = 0,                                                      /*!< Successfully */
    kStatus_HAL_InputError   = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Input, 1), /*!< Error occurs */
} hal_input_status_t;

/*! @brief Operation that needs to be implemented by an input device */
typedef struct
{
    /* initialize the dev */
    hal_input_status_t (*init)(input_dev_t *dev, input_dev_callback_t callback);
    /* deinitialize the dev */
    hal_input_status_t (*deinit)(const input_dev_t *dev);
    /* start the dev */
    hal_input_status_t (*start)(const input_dev_t *dev);
    /* start the dev */
    hal_input_status_t (*stop)(const input_dev_t *dev);
    /* notify the input_dev */
    hal_input_status_t (*inputNotify)(const input_dev_t *dev, void *param);
} input_dev_operator_t;

typedef struct
{
    /* callback */
    input_dev_callback_t callback;
} input_dev_private_capability_t;

/*! @brief Attributes of an input device */
typedef struct _input_dev
{
    /* unique id which is assigned by input manager during the registration */
    int id;
    /* name of the device */
    char name[DEVICE_NAME_MAX_LENGTH];
    /* operations */
    const input_dev_operator_t *ops;
    /* private capability */
    input_dev_private_capability_t cap;
} input_dev_t;

#endif /*_HAL_INPUT_DEV_H_*/
