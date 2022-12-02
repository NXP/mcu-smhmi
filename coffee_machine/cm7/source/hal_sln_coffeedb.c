/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <FreeRTOS.h>
#include <stdbool.h>
#include <string.h>

#include "fwk_log.h"
#include "fwk_flash.h"

#include "hal_sln_coffeedb.h"

#define COFFEE_DIR "coffee"

#define METADATA_FILE_NAME \
    COFFEE_DIR             \
    "/"                    \
    "Metadata"

#define COFFEE_DATABASE \
    COFFEE_DIR          \
    "/"

typedef enum _mapping_bitwise
{
    kMappingBitWise_Saved,
    kMappingBitWise_Used,
} mapping_bitwise_t;

#define NOT_USED  0X0
#define IN_USE    (1 << kMappingBitWise_Used)
#define NOT_SAVED 0x0
#define SAVED     (1 << kMappingBitWise_Saved)

const char *coffee_typeName[] = {
    "Espresso",
    "Americano",
    "Cappuccino",
    "CoffeeLatte",
};

static coffeedb_status_t HAL_CoffeeDb_Init(void);
static coffeedb_status_t HAL_CoffeeDb_Deinit(void);
static coffeedb_status_t HAL_CoffeeDb_AddWithId(uint16_t id, coffee_attribute_t *attr);
static coffeedb_status_t HAL_CoffeeDb_DelWithId(uint16_t id);
static coffeedb_status_t HAL_CoffeeDb_UpdWithId(uint16_t id, coffee_attribute_t *attr);
static coffeedb_status_t HAL_CoffeeDb_GetWithId(uint16_t id, coffee_attribute_t *attr);

typedef struct _coffeedb_metadata
{
    uint32_t version;
    uint32_t count;
    uint32_t maxCount;
    uint16_t entrySize;
    uint32_t bufferSize;
    uint8_t reserved[8];
    uint8_t map[COFFEE_DB_MAX_SIZE];
} coffeedb_metadata_t;

const coffeedb_ops_t g_coffedb_ops = {
    .init      = HAL_CoffeeDb_Init,
    .deinit    = HAL_CoffeeDb_Deinit,
    .addWithId = HAL_CoffeeDb_AddWithId,
    .delWithId = HAL_CoffeeDb_DelWithId,
    .updWithId = HAL_CoffeeDb_UpdWithId,
    .getWithId = HAL_CoffeeDb_GetWithId,
};

static coffeedb_metadata_t s_coffeeDbMetaData;
static uint8_t *s_pCoffeeDbBuffer       = NULL;
static SemaphoreHandle_t s_CoffeeDbLock = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/
static coffeedb_status_t _HAL_CoffeeDb_Lock()
{
    if (NULL == s_CoffeeDbLock)
    {
        return kCoffeeDBStatus_LockFail;
    }

    if (pdTRUE != xSemaphoreTake(s_CoffeeDbLock, portMAX_DELAY))
    {
        return kCoffeeDBStatus_LockFail;
    }

    return kCoffeeDBStatus_Success;
}

static void _HAL_CoffeeDb_Unlock()
{
    if (s_CoffeeDbLock != NULL)
    {
        xSemaphoreGive(s_CoffeeDbLock);
    }
}

static void _HAL_CoffeeDb_GeneratePathFromIndex(uint16_t id, char *path)
{
    if (path != NULL)
    {
        char index[4];
        itoa(id, index, 10);
        strcpy(path, COFFEE_DATABASE);
        strcat(path, index);
    }
}

static uint32_t _HAL_CoffeeDb_GetEntrySize(void)
{
    return s_coffeeDbMetaData.entrySize;
}

static uint32_t _HAL_CoffeeDb_GetMaxCount(void)
{
    return s_coffeeDbMetaData.maxCount;
}

static uint32_t _HAL_CoffeeDb_GetBufferSize(void)
{
    return s_coffeeDbMetaData.bufferSize;
}

static uint8_t _HAL_CoffeeDb_CheckMapFlag(uint16_t id, uint8_t bitWise)
{
    return (s_coffeeDbMetaData.map[id] & 1 << bitWise);
}

static void _HAL_CoffeeDb_SetMapFlag(uint16_t id, uint8_t flag)
{
    if ((_HAL_CoffeeDb_CheckMapFlag(id, kMappingBitWise_Used) == NOT_USED) &&
        ((flag & (1 << kMappingBitWise_Used)) == IN_USE))
    {
        s_coffeeDbMetaData.map[id] = flag;
        s_coffeeDbMetaData.count++;
    }
    else if ((_HAL_CoffeeDb_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE) &&
             ((flag & (1 << kMappingBitWise_Used)) == NOT_USED))
    {
        s_coffeeDbMetaData.map[id] = flag;
        s_coffeeDbMetaData.count--;
    }
}

#define _HAL_CoffeeDb_AndMapFlag(id, flag)   \
    {                                        \
        s_coffeeDbMetaData.map[id] &= ~flag; \
    }

#define _HAL_CoffeeDb_OrMapFlag(id, flag)   \
    {                                       \
        s_coffeeDbMetaData.map[id] |= flag; \
    }

static coffee_attribute_t *_HAL_CoffeeDb_GetEntry(uint16_t id)
{
    return (coffee_attribute_t *)(s_pCoffeeDbBuffer + _HAL_CoffeeDb_GetEntrySize() * id);
}

static coffeedb_status_t _HAL_CoffeeDb_SaveMetaData(void)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;
    sln_flash_status_t ret   = FWK_Flash_Save(METADATA_FILE_NAME, &s_coffeeDbMetaData, sizeof(coffeedb_metadata_t));

    if (kStatus_HAL_FlashSuccess != ret)
    {
        status = kCoffeeDBStatus_MetaDataFail;
    }

    return status;
}

static coffeedb_status_t _HAL_CoffeeDb_SetMetaDataDefault(void)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;

    memset((void *)&s_coffeeDbMetaData, 0x0, sizeof(coffeedb_metadata_t));
    s_coffeeDbMetaData.version    = COFFEE_DB_VERSION;
    s_coffeeDbMetaData.maxCount   = COFFEE_DB_MAX_SIZE;
    s_coffeeDbMetaData.bufferSize = sizeof(coffee_attribute_t) * COFFEE_DB_MAX_SIZE;
    s_coffeeDbMetaData.entrySize  = sizeof(coffee_attribute_t);

    return status;
}

static coffeedb_status_t _HAL_CoffeeDb_SetDefault(uint8_t *pDbBuf)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;

    uint32_t bufferSize = _HAL_CoffeeDb_GetBufferSize();
    memset(pDbBuf, 0x0, bufferSize);

    return status;
}

static coffeedb_status_t _HAL_CoffeeDb_EraseAll(void)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;

    for (uint16_t id = 0; id < COFFEE_DB_MAX_SIZE; id++)
    {
        if ((s_coffeeDbMetaData.map[id] & (1 << kMappingBitWise_Used)) == IN_USE)
        {
            s_coffeeDbMetaData.map[id] &= ~(1 << kMappingBitWise_Used);
            s_coffeeDbMetaData.count--;

            char path[20];
            _HAL_CoffeeDb_GeneratePathFromIndex(id, path);
            sln_flash_status_t ret = FWK_Flash_Rm(path);
            if (ret != kStatus_HAL_FlashSuccess && ret != kStatus_HAL_FlashFileNotExist)
            {
                LOGE("CoffeeDB: Failed to remove from database.");
                continue;
            }
        }
    }

    return status;
}

static coffeedb_status_t _HAL_CoffeeDb_LoadMetaData(void)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;
    sln_flash_status_t ret   = FWK_Flash_Mkdir(COFFEE_DIR);

    if (ret == kStatus_HAL_FlashDirExist)
    {
        uint32_t len = sizeof(coffeedb_metadata_t);
        ret          = FWK_Flash_Read(METADATA_FILE_NAME, &s_coffeeDbMetaData, 0, &len);
        if (ret == kStatus_HAL_FlashSuccess)
        {
            if ((s_coffeeDbMetaData.version != COFFEE_DB_VERSION))
            {
                if (s_coffeeDbMetaData.version == 1)
                {
                    LOGD(
                        "CoffeeDB: Version found in flash different from current version. Version is backward "
                        "compatible");
                    s_coffeeDbMetaData.version = COFFEE_DB_VERSION;
                    _HAL_CoffeeDb_SaveMetaData();
                }
                else
                {
                    LOGE("CoffeeDB: Version found in flash different from current version.");
                    _HAL_CoffeeDb_EraseAll();
                    _HAL_CoffeeDb_SetMetaDataDefault();
                    FWK_Flash_Save(METADATA_FILE_NAME, &s_coffeeDbMetaData, sizeof(coffeedb_metadata_t));
                    status = kCoffeeDBStatus_VersionMismatch;
                }
            }
        }
        else
        {
            LOGE("CoffeeDB: Failed to open the configuration file.");
            status = kCoffeeDBStatus_MetaDataFail;
        }
    }
    else if (ret == kStatus_HAL_FlashSuccess)
    {
        _HAL_CoffeeDb_SetMetaDataDefault();
        FWK_Flash_Save(METADATA_FILE_NAME, &s_coffeeDbMetaData, sizeof(coffeedb_metadata_t));
        LOGI("CoffeeDB: Metadata saved. Database is empty.");
        status = kCoffeeDBStatus_Success;
    }
    else
    {
        LOGE("FaceDB: Failed to initialize the metadata file.");
        status = kCoffeeDBStatus_MetaDataFail;
    }

    return status;
}

static coffeedb_status_t _HAL_CoffeeDb_SaveEntryData(uint16_t id)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;
    char path[20];

    _HAL_CoffeeDb_GeneratePathFromIndex(id, path);
    sln_flash_status_t ret = FWK_Flash_Save(path, _HAL_CoffeeDb_GetEntry(id), _HAL_CoffeeDb_GetEntrySize());
    if (kStatus_HAL_FlashSuccess != ret)
    {
        status = kCoffeeDBStatus_Failed;
    }

    return status;
}

static coffeedb_status_t _HAL_CoffeeDb_DelEntryData(uint16_t id)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;
    char path[20];

    _HAL_CoffeeDb_GeneratePathFromIndex(id, path);
    sln_flash_status_t ret = FWK_Flash_Rm(path);
    if (kStatus_HAL_FlashSuccess != ret && kStatus_HAL_FlashFileNotExist != ret)
    {
        status = kCoffeeDBStatus_Failed;
    }

    return status;
}

static coffeedb_status_t _HAL_CoffeeDb_InitMetaData(void)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;

    status = _HAL_CoffeeDb_LoadMetaData();

    if ((kCoffeeDBStatus_VersionMismatch != status) && (kCoffeeDBStatus_Success != status))
    {
        status = _HAL_CoffeeDb_SetMetaDataDefault();
        if (kCoffeeDBStatus_Success != _HAL_CoffeeDb_SaveMetaData())
        {
            LOGI("Coffee Metadata Save Fail");
        }
    }

    return status;
}

static coffeedb_status_t _HAL_CoffeeDb_Load(uint8_t *pDbBuf)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;
    uint32_t maxCount        = _HAL_CoffeeDb_GetMaxCount();
    sln_flash_status_t ret   = kStatus_HAL_FlashSuccess;
    uint8_t needUpdate       = 0;

    for (int id = 0; id < maxCount; id++)
    {
        if (_HAL_CoffeeDb_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE)
        {
            char path[20];
            uint32_t lenEntry = _HAL_CoffeeDb_GetEntrySize();
            _HAL_CoffeeDb_GeneratePathFromIndex(id, path);

            ret = FWK_Flash_Read(path, _HAL_CoffeeDb_GetEntry(id), 0, &lenEntry);
            if (ret != kStatus_HAL_FlashSuccess)
            {
                if (ret == kStatus_HAL_FlashFileNotExist)
                {
                    _HAL_CoffeeDb_SetMapFlag(id, 0);
                    needUpdate = 1;
                }
                else
                {
                    status = kCoffeeDBStatus_DbLoadFail;
                }
            }
        }
    }

    if (needUpdate)
    {
        _HAL_CoffeeDb_SaveMetaData();
    }

    return status;
}

static coffeedb_status_t _HAL_CoffeeDb_Init(uint8_t *pDbBuf)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;

    status = _HAL_CoffeeDb_Load(pDbBuf);

    if (kCoffeeDBStatus_Success != status)
    {
        _HAL_CoffeeDb_SetDefault(pDbBuf);
    }

    return status;
}

static coffeedb_status_t HAL_CoffeeDb_Init(void)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;

    status = _HAL_CoffeeDb_InitMetaData();

    if (status != kCoffeeDBStatus_Success)
    {
        return status;
    }

    uint32_t maxCount           = _HAL_CoffeeDb_GetMaxCount();
    uint32_t coffeeDbEntrySize  = sizeof(coffee_attribute_t);
    uint32_t coffeeDbBufferSize = coffeeDbEntrySize * maxCount;
    s_pCoffeeDbBuffer           = pvPortMalloc(coffeeDbBufferSize);

    if (NULL == s_pCoffeeDbBuffer)
    {
        LOGE("Coffee DB Entry Malloc Fail");
        return kCoffeeDBStatus_MallocFail;
    }
    _HAL_CoffeeDb_Init(s_pCoffeeDbBuffer);

    if (NULL == s_CoffeeDbLock)
    {
        s_CoffeeDbLock = xSemaphoreCreateMutex();

        if (NULL == s_CoffeeDbLock)
        {
            LOGE("CoffeeDb: Failed to create DB lock semaphore");
            vPortFree(s_pCoffeeDbBuffer);
            status = kCoffeeDBStatus_LockFail;
        }
    }

    return status;
}

static coffeedb_status_t HAL_CoffeeDb_Deinit(void)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;

    if (NULL != s_pCoffeeDbBuffer)
    {
        vPortFree(s_pCoffeeDbBuffer);
    }

    return status;
}

static coffeedb_status_t HAL_CoffeeDb_AddWithId(uint16_t id, coffee_attribute_t *attr)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;

    if (NULL == s_pCoffeeDbBuffer)
    {
        return kCoffeeDBStatus_MallocFail;
    }

    if (kCoffeeDBStatus_Success != _HAL_CoffeeDb_Lock())
    {
        return kCoffeeDBStatus_LockFail;
    }

    uint32_t maxCount = _HAL_CoffeeDb_GetMaxCount();
    if (id > maxCount)
    {
        _HAL_CoffeeDb_Unlock();
        return kCoffeeDBStatus_Failed;
    }

    if (_HAL_CoffeeDb_CheckMapFlag(id, kMappingBitWise_Used) == NOT_USED)
    {
        coffee_attribute_t *pCoffeeAttrEntry = _HAL_CoffeeDb_GetEntry(id);
        memcpy(pCoffeeAttrEntry, attr, _HAL_CoffeeDb_GetEntrySize());

        _HAL_CoffeeDb_SetMapFlag(id, IN_USE);

        _HAL_CoffeeDb_SaveEntryData(id);
        _HAL_CoffeeDb_SaveMetaData();
    }
    else
    {
        // TODO: IN_USE
    }

    _HAL_CoffeeDb_Unlock();

    return status;
}

static coffeedb_status_t HAL_CoffeeDb_DelWithId(uint16_t id)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;

    if (NULL == s_pCoffeeDbBuffer)
    {
        return kCoffeeDBStatus_MallocFail;
    }

    if (kCoffeeDBStatus_Success != _HAL_CoffeeDb_Lock())
    {
        return kCoffeeDBStatus_LockFail;
    }

    if (id == INVALID_ID)
    {
        _HAL_CoffeeDb_EraseAll();
        _HAL_CoffeeDb_SetMetaDataDefault();
        FWK_Flash_Save(METADATA_FILE_NAME, &s_coffeeDbMetaData, sizeof(coffeedb_metadata_t));
        _HAL_CoffeeDb_Unlock();
        return kCoffeeDBStatus_Success;
    }

    uint32_t maxCount = _HAL_CoffeeDb_GetMaxCount();
    if (id > maxCount)
    {
        _HAL_CoffeeDb_Unlock();
        return kCoffeeDBStatus_Failed;
    }

    if (_HAL_CoffeeDb_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE)
    {
        coffee_attribute_t *pCoffeeAttrEntry = _HAL_CoffeeDb_GetEntry(id);
        memset(pCoffeeAttrEntry, 0, _HAL_CoffeeDb_GetEntrySize());

        _HAL_CoffeeDb_SetMapFlag(id, NOT_USED);

        _HAL_CoffeeDb_DelEntryData(id);
        _HAL_CoffeeDb_SaveMetaData();
    }

    _HAL_CoffeeDb_Unlock();
    return status;
}

static coffeedb_status_t HAL_CoffeeDb_UpdWithId(uint16_t id, coffee_attribute_t *attr)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;

    if (NULL == s_pCoffeeDbBuffer)
    {
        return kCoffeeDBStatus_MallocFail;
    }

    if (kCoffeeDBStatus_Success != _HAL_CoffeeDb_Lock())
    {
        return kCoffeeDBStatus_LockFail;
    }

    uint32_t maxCount = _HAL_CoffeeDb_GetMaxCount();
    if (id > maxCount)
    {
        _HAL_CoffeeDb_Unlock();
        return kCoffeeDBStatus_Failed;
    }

    if (_HAL_CoffeeDb_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE)
    {
        coffee_attribute_t *pCoffeeAttrEntry = _HAL_CoffeeDb_GetEntry(id);
        memcpy(pCoffeeAttrEntry, attr, _HAL_CoffeeDb_GetEntrySize());

        _HAL_CoffeeDb_SetMapFlag(id, IN_USE);

        _HAL_CoffeeDb_SaveEntryData(id);
        _HAL_CoffeeDb_SaveMetaData();
    }

    _HAL_CoffeeDb_Unlock();
    return status;
}

static coffeedb_status_t HAL_CoffeeDb_GetWithId(uint16_t id, coffee_attribute_t *attr)
{
    coffeedb_status_t status = kCoffeeDBStatus_Success;

    if (NULL == s_pCoffeeDbBuffer)
    {
        return kCoffeeDBStatus_MallocFail;
    }

    if (kCoffeeDBStatus_Success != _HAL_CoffeeDb_Lock())
    {
        return kCoffeeDBStatus_LockFail;
    }

    uint32_t maxCount = _HAL_CoffeeDb_GetMaxCount();
    if (id > maxCount)
    {
        _HAL_CoffeeDb_Unlock();
        return kCoffeeDBStatus_Failed;
    }

    if (_HAL_CoffeeDb_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE)
    {
        coffee_attribute_t *pCoffeeAttrEntry = _HAL_CoffeeDb_GetEntry(id);
        memcpy(attr, pCoffeeAttrEntry, _HAL_CoffeeDb_GetEntrySize());
    }

    _HAL_CoffeeDb_Unlock();
    return status;
}
