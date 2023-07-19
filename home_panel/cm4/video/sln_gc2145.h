/*
 * Copyright 2021-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef SLN_GC2145_H_
#define SLN_GC2145_H_

#include "fsl_common.h"
#include "fsl_camera_device.h"
#include "fsl_video_i2c.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define GC2145MIPI_LANE_NUM 2
/*!
 * @brief GC2145 resource.
 *
 * Before initialize the GC2145, the resource must be initialized that the
 * Two-Wire-Serial-Interface I2C could start to work.
 */
typedef struct _gc2145_resource
{
    video_i2c_send_func_t i2cSendFunc;        /*!< I2C send function. */
    video_i2c_receive_func_t i2cReceiveFunc;  /*!< I2C receive function. */
    void (*pullResetPin)(bool pullUp);        /*!< Function to pull reset pin high or low. */
    void (*pullPowerDownPin)(bool powerDown); /*!< Function activate / deactivate the sensor power down. */
} gc2145_resource_t;

/*! @brief GC2145 operation functions. */
extern const camera_device_operations_t gc2145_ops;

#endif /* SLN_GC2145_H_ */
