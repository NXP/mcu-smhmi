/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef HAL_VISION_HAL_SLN_ELEVATORDB_H_
#define HAL_VISION_HAL_SLN_ELEVATORDB_H_

#include <stdint.h>
#include <stddef.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ELEVATOR_DB_VERSION  2
#define ELEVATOR_DB_MAX_SIZE 100
#define INVALID_ID           0xFFFF
#define INVALID_FLOOR        0xFFFFFFFF

typedef enum _elevatordb_status
{
    kElevatorDBStatus_Success,
    kElevatorDBStatus_Failed,
    kElevatorDBStatus_MallocFail,
    kElevatorDBStatus_MetaDataFail,
    kElevatorDBStatus_DbLoadFail,
    kElevatorDBStatus_LockFail,
	kElevatorDBStatus_VersionMismatch
} elevatordb_status_t;

typedef struct _elevator_attribute
{
    uint16_t id;
    uint32_t floor;
    uint8_t language;
    uint8_t reserved[15];
} elevator_attr_t;

typedef struct _elevatordb_ops
{
    elevatordb_status_t (*init)(void);
    elevatordb_status_t (*deinit)(void);
    elevatordb_status_t (*addWithId)(uint16_t id, elevator_attr_t *attr);
    elevatordb_status_t (*delWithId)(uint16_t id);
    elevatordb_status_t (*updWithId)(uint16_t id, elevator_attr_t *attr);
    elevatordb_status_t (*getWithId)(uint16_t id, elevator_attr_t *attr);
} elevatordb_ops_t;

extern const elevatordb_ops_t g_elevatordb_ops;

#endif /* HAL_VISION_HAL_SLN_ELEVATORDB_H_ */
