/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _HAL_MULTICORE_DEV_H_
#define _HAL_MULTICORE_DEV_H_

#include "fwk_common.h"
#include "fwk_message.h"
/**
 * @brief declare the multicore dev ##name
 */
#define HAL_MULTICORE_DEV_DECLARE(name) int HAL_MulticoreDev_##name##_Register()

/**
 * @brief register the multicore dev ##name
 */
#define HAL_MULTICORE_DEV_REGISTER(name, ret)                             \
    {                                                                     \
        ret = HAL_MulticoreDev_##name##_Register();                       \
        if (ret != 0)                                                     \
        {                                                                 \
            LOGE("HAL_MulticoreDev_%s_Register error %d", "##name", ret); \
            return ret;                                                   \
        }                                                                 \
    }

typedef struct _multicore_dev multicore_dev_t;

/*! @brief Type of events that are supported by calling the callback function */
typedef enum _multicore_event_id
{
    /* Multicore hal received a msg from the other core */
    kMulticoreEvent_MsgReceive = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_Multicore, 1),
    kMulticoreEvent_Count
} multicore_event_id_t;

/*! @brief Structure used to define an event. */
typedef struct _multicore_event
{
    /* Eventid from the list above. */
    multicore_event_id_t eventId;
    /* Pointer to a struct of data that needs to be forwarded.*/
    void *data;
    /* Size of the struct that needs to be forwarded. */
    unsigned int size;
} multicore_event_t;

/**
 * @brief callback function to notify multicore manager that an async event took place
 * @param dev Device structure of the multicore device calling this function
 * @param event  the event that took place
 * @param fromISR True if this operation takes place in an irq, 0 otherwise
 * @return 0 if the operation was successfully
 */
typedef int (*multicore_dev_callback_t)(const multicore_dev_t *dev, multicore_event_t event, uint8_t fromISR);

/*! @brief Error codes for multicore hal devices */
typedef enum _hal_multicore_status
{
    kStatus_HAL_MulticoreSuccess = 0, /*!< Successfully */
    kStatus_HAL_MulticoreError =
        MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Multicore, 3), /*!< Error occurs on HAL Multicore */
} hal_multicore_status_t;

/*! @brief Operation that needs to be implemented by a multicore device */
typedef struct _multicore_dev_operator
{
    /* initialize the dev */
    hal_multicore_status_t (*init)(multicore_dev_t *dev, multicore_dev_callback_t callback, void *param);
    /* deinitialize the dev */
    hal_multicore_status_t (*deinit)(const multicore_dev_t *dev);
    /* start the dev */
    hal_multicore_status_t (*start)(const multicore_dev_t *dev);
    /* Multicore Send the message */
    hal_multicore_status_t (*send)(const multicore_dev_t *dev, void *data, unsigned int size);
    /* input notify */
    hal_multicore_status_t (*inputNotify)(const multicore_dev_t *dev, void *data);
} multicore_dev_operator_t;

/*! @brief Structure that characterizes the multicore device. */
typedef struct _multicore_dev_private_capability
{
    /* callback */
    multicore_dev_callback_t callback;

} multicore_dev_private_capability_t;

/*! @brief Attributes of a multicore device. */
struct _multicore_dev
{
    /* unique id which is assigned by multicore manager during the registration */
    int id;
    /* name of the device */
    char name[DEVICE_NAME_MAX_LENGTH];
    /* operations */
    const multicore_dev_operator_t *ops;
    /* private capability */
    multicore_dev_private_capability_t cap;
};

#endif /*_HAL_MULTICORE_DEV_H_*/
