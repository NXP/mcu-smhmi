/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef HAL_VISION_HAL_SLN_THERMOSTATDB_H_
#define HAL_VISION_HAL_SLN_THERMOSTATDB_H_

#include <stdint.h>
#include <stddef.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define THERMOSTAT_DB_VERSION  1
#define THERMOSTAT_MAX_SIZE 100
#define INVALID_ID        0xFFFF
#define TEMPERATURE_VALUE_SIZE 6

typedef enum _thermostatdb_status
{
    kThermostatDBStatus_Success,
    kThermostatDBStatus_Failed,
    kThermostatDBStatus_MallocFail,
    kThermostatDBStatus_MetaDataFail,
    kThermostatDBStatus_DbLoadFail,
    kThermostatDBStatus_LockFail
} thermostatdb_status_t;

typedef enum _thermostat_mode
{
    kThermostat_Auto,
    kThermostat_Cold,
    kThermostat_Heat,
} thermostat_mode_t;

typedef enum _thermostat_speed
{
    kThermostatFan_On,
    kThermostatFan_Off,
    kThermostatFan_Low,
    kThermostatFan_Med,
    kThermostatFan_High,
} thermostat_fan_speed_t;

typedef struct _thermostat_attribute
{
    uint16_t id;
    uint8_t mode;
    uint8_t fan_speed;
    uint8_t temperature[TEMPERATURE_VALUE_SIZE];
    uint8_t reserved[16];
} thermostat_attribute_t;

typedef struct _thermostatdb_ops
{
    thermostatdb_status_t (*init)(void);
    thermostatdb_status_t (*deinit)(void);
    thermostatdb_status_t (*addWithId)(uint16_t id, thermostat_attribute_t *attr);
    thermostatdb_status_t (*delWithId)(uint16_t id);
    thermostatdb_status_t (*updWithId)(uint16_t id, thermostat_attribute_t *attr);
    thermostatdb_status_t (*getWithId)(uint16_t id, thermostat_attribute_t *attr);
} thermostatdb_ops_t;

extern const thermostatdb_ops_t g_thermostatdb_ops;

#endif /* HAL_VISION_HAL_SLN_THERMOSTATDB_H_ */
