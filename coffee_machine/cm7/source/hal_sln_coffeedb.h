/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef HAL_VISION_HAL_SLN_COFFEEDB_H_
#define HAL_VISION_HAL_SLN_COFFEEDB_H_

#include <stdint.h>
#include <stddef.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define COFFEE_DB_VERSION  2
#define COFFEE_DB_MAX_SIZE 100
#define INVALID_ID         0xFFFF

typedef enum _coffeedb_status
{
    kCoffeeDBStatus_Success,
    kCoffeeDBStatus_Failed,
    kCoffeeDBStatus_MallocFail,
    kCoffeeDBStatus_MetaDataFail,
    kCoffeeDBStatus_DbLoadFail,
    kCoffeeDBStatus_LockFail,
    kCoffeeDBStatus_VersionMismatch
} coffeedb_status_t;

typedef enum _coffee_type
{
    Coffee_Espresso,
    Coffee_Americano,
    Coffee_Cappuccino,
    Caffee_Latte,
} coffee_type_t;

typedef enum _coffee_size
{
    Coffee_Small,
    Coffee_Medium,
    Coffee_Large,
} coffee_size_t;

typedef enum _coffee_strength
{
    Coffee_Soft,
    Coffee_Mild,
    Coffee_Strong,
} coffee_strength_t;

typedef struct _coffee_attribute
{
    uint16_t id;
    uint8_t type;
    uint8_t size;
    uint8_t strength;
    uint8_t language;
    uint8_t reserved[15];
} coffee_attribute_t;

extern const char *coffee_typeName[];
#define GetName(type) coffee_typeName[type]

typedef struct _coffeedb_ops
{
    coffeedb_status_t (*init)(void);
    coffeedb_status_t (*deinit)(void);
    coffeedb_status_t (*addWithId)(uint16_t id, coffee_attribute_t *attr);
    coffeedb_status_t (*delWithId)(uint16_t id);
    coffeedb_status_t (*updWithId)(uint16_t id, coffee_attribute_t *attr);
    coffeedb_status_t (*getWithId)(uint16_t id, coffee_attribute_t *attr);
} coffeedb_ops_t;

extern const coffeedb_ops_t g_coffedb_ops;

#endif /* HAL_VISION_HAL_SLN_COFFEEDB_H_ */
