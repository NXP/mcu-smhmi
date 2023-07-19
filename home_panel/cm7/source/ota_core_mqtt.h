/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _OTA_CORE_MQTT_H_
#define _OTA_CORE_MQTT_H_

#include "iot_network.h"
/*-----------------------------------------------------------*/

/**
 * @brief
 *
 * This example initializes the OTA library to enable OTA updates via the
 * MQTT broker. It simply connects to the MQTT broker with the users
 * credentials and spins in an indefinite loop to allow MQTT messages to be
 * forwarded to the OTA agent for possible processing. The OTA agent does all
 * of the real work; checking to see if the message topic is one destined for
 * the OTA agent. If not, it is simply ignored.
 *
 * @param[in] xAwsIotMqttMode Specify if this demo is running with the AWS IoT
 * MQTT server. Set this to `false` if using another MQTT server.
 * @param[in] pIdentifier NULL-terminated MQTT client identifier.
 * @param[in] pNetworkServerInfo Passed to the MQTT connect function when
 * establishing the MQTT connection.
 * @param[in] pNetworkCredentialInfo Passed to the MQTT connect function when
 * establishing the MQTT connection.
 * @param[in] pxNetworkInterface The network interface to use for this demo.
 *
 * @return `EXIT_SUCCESS` if the demo completes successfully; `EXIT_FAILURE`
 * otherwise.
 *
 */
int RunOtaCoreMqttDemo(bool xAwsIotMqttMode,
                       const char *pIdentifier,
                       void *pNetworkServerInfo,
                       void *pNetworkCredentialInfo,
                       const IotNetworkInterface_t *pxNetworkInterface);

/*!
 * @brief Shares otaDoneTask handle with OTA PAL layer, for inter-task communication
 *
 * @param handle FreeRTOS task handle of otaDoneTask
 */
void otaPalOtaDoneTaskSet(TaskHandle_t handle);

/**
 * @brief Runs the RunOtaCoreMqttDemo function with NULL parameters.
 * Used to be used as a thread wrapper over xTaskCreate();
 *
 * @param pArgument
 */
void ota_task(void *pArgument);

#endif /* _OTA_CORE_MQTT_H_ */
