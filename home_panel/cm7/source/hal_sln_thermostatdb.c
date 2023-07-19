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

#include "hal_sln_thermostatdb.h"

#define THERMOSTAT_DIR "thermostat"

#define METADATA_FILE_NAME \
    THERMOSTAT_DIR             \
    "/"                    \
    "Metadata"

#define THERMOSTAT_DATABASE \
    THERMOSTAT_DIR          \
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

static thermostatdb_status_t HAL_Db_Init(void);
static thermostatdb_status_t HAL_Db_Deinit(void);
static thermostatdb_status_t HAL_Db_AddWithId(uint16_t id, thermostat_attribute_t *attr);
static thermostatdb_status_t HAL_Db_DelWithId(uint16_t id);
static thermostatdb_status_t HAL_Db_UpdWithId(uint16_t id, thermostat_attribute_t *attr);
static thermostatdb_status_t HAL_Db_GetWithId(uint16_t id, thermostat_attribute_t *attr);

typedef struct
{
    uint32_t version;
    uint32_t count;
    uint32_t maxCount;
    uint16_t entrySize;
    uint32_t bufferSize;
    uint8_t reserved[8];
    uint8_t map[THERMOSTAT_MAX_SIZE];
} metadata_t;

const thermostatdb_ops_t g_thermostatdb_ops = {
    .init      = HAL_Db_Init,
    .deinit    = HAL_Db_Deinit,
    .addWithId = HAL_Db_AddWithId,
    .delWithId = HAL_Db_DelWithId,
    .updWithId = HAL_Db_UpdWithId,
    .getWithId = HAL_Db_GetWithId,
};

static metadata_t s_MetaData;
static uint8_t *s_pDbBuffer = NULL;
static SemaphoreHandle_t s_Lock = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/
static thermostatdb_status_t _HAL_Db_Lock()
{
    if (NULL == s_Lock)
    {
        return kThermostatDBStatus_LockFail;
    }

    if (pdTRUE != xSemaphoreTake(s_Lock, portMAX_DELAY))
    {
        return kThermostatDBStatus_LockFail;
    }

    return kThermostatDBStatus_Success;
}

static void _HAL_Db_Unlock()
{
    if (s_Lock != NULL)
    {
        xSemaphoreGive(s_Lock);
    }
}

static void _HAL_Db_GeneratePathFromIndex(uint16_t id, char *path)
{
    if (path != NULL)
    {
        char index[4];
        itoa(id, index, 10);
        strcpy(path, THERMOSTAT_DATABASE);
        strcat(path, index);
    }
}

static uint32_t _HAL_Db_GetEntrySize(void)
{
    return s_MetaData.entrySize;
}

static uint32_t _HAL_Db_GetMaxCount(void)
{
    return s_MetaData.maxCount;
}

static uint32_t _HAL_Db_GetBufferSize(void)
{
    return s_MetaData.bufferSize;
}

static uint8_t _HAL_Db_CheckMapFlag(uint16_t id, uint8_t bitWise)
{
    return (s_MetaData.map[id] & 1 << bitWise);
}

static void _HAL_Db_SetMapFlag(uint16_t id, uint8_t flag)
{
    if ((_HAL_Db_CheckMapFlag(id, kMappingBitWise_Used) == NOT_USED) &&
        ((flag & (1 << kMappingBitWise_Used)) == IN_USE))
    {
        s_MetaData.map[id] = flag;
        s_MetaData.count++;
    }
    else if ((_HAL_Db_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE) &&
             ((flag & (1 << kMappingBitWise_Used)) == NOT_USED))
    {
        s_MetaData.map[id] = flag;
        s_MetaData.count--;
    }
}

#define _HAL_Db_AndMapFlag(id, flag) \
    {                                \
        s_MetaData.map[id] &= ~flag; \
    }

#define _HAL_Db_OrMapFlag(id, flag)  \
    {                                \
        s_MetaData.map[id] |= flag;  \
    }

static thermostat_attribute_t *_HAL_Db_GetEntry(uint16_t id)
{
    return (thermostat_attribute_t *)(s_pDbBuffer + _HAL_Db_GetEntrySize() * id);
}

static thermostatdb_status_t _HAL_Db_SaveMetaData(void)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;
    sln_flash_status_t ret   = FWK_Flash_Save(METADATA_FILE_NAME, &s_MetaData, sizeof(metadata_t));

    if (kStatus_HAL_FlashSuccess != ret)
    {
        status = kThermostatDBStatus_MetaDataFail;
    }

    return status;
}
static thermostatdb_status_t _HAL_Db_SetMetaDataDefault(void)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;

    memset((void *)&s_MetaData, 0x0, sizeof(metadata_t));
    s_MetaData.version    = THERMOSTAT_DB_VERSION;
    s_MetaData.maxCount   = THERMOSTAT_MAX_SIZE;
    s_MetaData.bufferSize = sizeof(thermostat_attribute_t) * THERMOSTAT_MAX_SIZE;
    s_MetaData.entrySize  = sizeof(thermostat_attribute_t);

    return status;
}

static thermostatdb_status_t _HAL_Db_SetDefault(uint8_t *pDbBuf)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;

    uint32_t bufferSize = _HAL_Db_GetBufferSize();
    memset(pDbBuf, 0x0, bufferSize);

    return status;
}

static thermostatdb_status_t _HAL_Db_EraseAll(void)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;

    for (uint16_t id = 0; id < THERMOSTAT_MAX_SIZE; id++)
    {
        if ((s_MetaData.map[id] & (1 << kMappingBitWise_Used)) == IN_USE)
        {
            s_MetaData.map[id] &= ~(1 << kMappingBitWise_Used);
            s_MetaData.count--;

            char path[20];
            _HAL_Db_GeneratePathFromIndex(id, path);
            sln_flash_status_t ret = FWK_Flash_Rm(path);
            if (ret != kStatus_HAL_FlashSuccess && ret != kStatus_HAL_FlashFileNotExist)
            {
                LOGE("Failed to remove from database.");
                continue;
            }
        }
    }

    return status;
}

static thermostatdb_status_t _HAL_Db_LoadMetaData(void)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;
    sln_flash_status_t ret   = FWK_Flash_Mkdir(THERMOSTAT_DIR);

    if (ret == kStatus_HAL_FlashDirExist)
    {
        unsigned int len = sizeof(metadata_t);
        ret          = FWK_Flash_Read(METADATA_FILE_NAME, &s_MetaData, 0, &len);
        if (ret == kStatus_HAL_FlashSuccess)
        {
            if ((s_MetaData.version != THERMOSTAT_DB_VERSION))
            {
                LOGE("Version found in flash different from current version.");
                _HAL_Db_EraseAll();
                _HAL_Db_SetMetaDataDefault();
                FWK_Flash_Save(METADATA_FILE_NAME, &s_MetaData, sizeof(metadata_t));
            }

            status = kThermostatDBStatus_Success;
        }
        else
        {
            LOGE("Failed to open the configuration file.");
            status = kThermostatDBStatus_MetaDataFail;
        }
    }
    else if (ret == kStatus_HAL_FlashSuccess)
    {
        _HAL_Db_SetMetaDataDefault();
        FWK_Flash_Save(METADATA_FILE_NAME, &s_MetaData, sizeof(metadata_t));
        LOGI("Metadata saved. Database is empty.");
        status = kThermostatDBStatus_Success;
    }
    else
    {
        LOGE("Failed to initialize the metadata file.");
        status = kThermostatDBStatus_MetaDataFail;
    }

    return status;
}


static thermostatdb_status_t _HAL_Db_SaveEntryData(uint16_t id)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;
    char path[20];

    _HAL_Db_GeneratePathFromIndex(id, path);
    sln_flash_status_t ret = FWK_Flash_Save(path, _HAL_Db_GetEntry(id), _HAL_Db_GetEntrySize());
    if (kStatus_HAL_FlashSuccess != ret)
    {
        status = kThermostatDBStatus_Failed;
    }

    return status;
}

static thermostatdb_status_t _HAL_Db_DelEntryData(uint16_t id)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;
    char path[20];

    _HAL_Db_GeneratePathFromIndex(id, path);
    sln_flash_status_t ret = FWK_Flash_Rm(path);
    if (kStatus_HAL_FlashSuccess != ret && kStatus_HAL_FlashFileNotExist != ret)
    {
        status = kThermostatDBStatus_Failed;
    }

    return status;
}

static thermostatdb_status_t _HAL_Db_InitMetaData(void)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;

    status = _HAL_Db_LoadMetaData();

    if (kThermostatDBStatus_Success != status)
    {
        status = _HAL_Db_SetMetaDataDefault();
        if (kThermostatDBStatus_Success != _HAL_Db_SaveMetaData())
        {
            LOGI("Metadata Save Fail");
        }
    }

    return status;
}

static thermostatdb_status_t _HAL_Db_Load(uint8_t *pDbBuf)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;
    uint32_t maxCount        = _HAL_Db_GetMaxCount();
    sln_flash_status_t ret   = kStatus_HAL_FlashSuccess;
    uint8_t needUpdate       = 0;

    for (int id = 0; id < maxCount; id++)
    {
        if (_HAL_Db_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE)
        {
            char path[20];
            unsigned int lenEntry = _HAL_Db_GetEntrySize();
            _HAL_Db_GeneratePathFromIndex(id, path);

            ret = FWK_Flash_Read(path, _HAL_Db_GetEntry(id), 0, &lenEntry);
            if (ret != kStatus_HAL_FlashSuccess)
            {
                if (ret == kStatus_HAL_FlashFileNotExist)
                {
                    _HAL_Db_SetMapFlag(id, 0);
                    needUpdate = 1;
                }
                else
                {
                    status = kThermostatDBStatus_DbLoadFail;
                }
            }
        }
    }

    if (needUpdate)
    {
        _HAL_Db_SaveMetaData();
    }

    return status;
}

static thermostatdb_status_t _HAL_Db_Init(uint8_t *pDbBuf)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;

    status = _HAL_Db_Load(pDbBuf);

    if (kThermostatDBStatus_Success != status)
    {
        _HAL_Db_SetDefault(pDbBuf);
    }

    return status;
}

static thermostatdb_status_t HAL_Db_Init(void)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;

    status = _HAL_Db_InitMetaData();

    if (status != kThermostatDBStatus_Success)
    {
        return status;
    }

    uint32_t maxCount     = _HAL_Db_GetMaxCount();
    uint32_t dbEntrySize  = sizeof(thermostat_attribute_t);
    uint32_t dbBufferSize = dbEntrySize * maxCount;
    s_pDbBuffer           = pvPortMalloc(dbBufferSize);

    if (NULL == s_pDbBuffer)
    {
        LOGE("DB Entry Malloc Fail");
        return kThermostatDBStatus_MallocFail;
    }
    _HAL_Db_Init(s_pDbBuffer);

    if (NULL == s_Lock)
    {
        s_Lock = xSemaphoreCreateMutex();

        if (NULL == s_Lock)
        {
            LOGE("Failed to create DB lock semaphore");
            vPortFree(s_pDbBuffer);
            status = kThermostatDBStatus_LockFail;
        }
    }

    return status;
}

static thermostatdb_status_t HAL_Db_Deinit(void)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;

    if (NULL != s_pDbBuffer)
    {
        vPortFree(s_pDbBuffer);
    }

    return status;
}

static thermostatdb_status_t HAL_Db_AddWithId(uint16_t id, thermostat_attribute_t *attr)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;

    if (NULL == s_pDbBuffer)
    {
        return kThermostatDBStatus_MallocFail;
    }

    if (kThermostatDBStatus_Success != _HAL_Db_Lock())
    {
        return kThermostatDBStatus_LockFail;
    }

    uint32_t maxCount = _HAL_Db_GetMaxCount();
    if (id > maxCount)
    {
        _HAL_Db_Unlock();
        return kThermostatDBStatus_Failed;
    }

    if (_HAL_Db_CheckMapFlag(id, kMappingBitWise_Used) == NOT_USED)
    {
        thermostat_attribute_t *pAttrEntry = _HAL_Db_GetEntry(id);
        memcpy(pAttrEntry, attr, _HAL_Db_GetEntrySize());

        _HAL_Db_SetMapFlag(id, IN_USE);

        _HAL_Db_SaveEntryData(id);
        _HAL_Db_SaveMetaData();
    }
    else
    {
        // TODO: IN_USE
    }

    _HAL_Db_Unlock();

    return status;
}

static thermostatdb_status_t HAL_Db_DelWithId(uint16_t id)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;

    if (NULL == s_pDbBuffer)
    {
        return kThermostatDBStatus_MallocFail;
    }

    if (kThermostatDBStatus_Success != _HAL_Db_Lock())
    {
        return kThermostatDBStatus_LockFail;
    }

    if (id == INVALID_ID)
    {
        _HAL_Db_EraseAll();
        _HAL_Db_SetMetaDataDefault();
        FWK_Flash_Save(METADATA_FILE_NAME, &s_MetaData, sizeof(metadata_t));
        _HAL_Db_Unlock();
        return kThermostatDBStatus_Success;
    }

    uint32_t maxCount = _HAL_Db_GetMaxCount();
    if (id > maxCount)
    {
        _HAL_Db_Unlock();
        return kThermostatDBStatus_Failed;
    }

    if (_HAL_Db_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE)
    {
        thermostat_attribute_t *pAttrEntry = _HAL_Db_GetEntry(id);
        memset(pAttrEntry, 0, _HAL_Db_GetEntrySize());

        _HAL_Db_SetMapFlag(id, NOT_USED);

        _HAL_Db_DelEntryData(id);
        _HAL_Db_SaveMetaData();
    }

    _HAL_Db_Unlock();
    return status;
}

static thermostatdb_status_t HAL_Db_UpdWithId(uint16_t id, thermostat_attribute_t *attr)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;

    if (NULL == s_pDbBuffer)
    {
        return kThermostatDBStatus_MallocFail;
    }

    if (kThermostatDBStatus_Success != _HAL_Db_Lock())
    {
        return kThermostatDBStatus_LockFail;
    }

    uint32_t maxCount = _HAL_Db_GetMaxCount();
    if (id > maxCount)
    {
        _HAL_Db_Unlock();
        return kThermostatDBStatus_Failed;
    }

    if (_HAL_Db_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE)
    {
        thermostat_attribute_t *pAttrEntry = _HAL_Db_GetEntry(id);
        memcpy(pAttrEntry, attr, _HAL_Db_GetEntrySize());

        _HAL_Db_SetMapFlag(id, IN_USE);

        _HAL_Db_SaveEntryData(id);
        _HAL_Db_SaveMetaData();
    }

    _HAL_Db_Unlock();
    return status;
}

static thermostatdb_status_t HAL_Db_GetWithId(uint16_t id, thermostat_attribute_t *attr)
{
    thermostatdb_status_t status = kThermostatDBStatus_Success;

    if (NULL == s_pDbBuffer)
    {
        return kThermostatDBStatus_MallocFail;
    }

    if (kThermostatDBStatus_Success != _HAL_Db_Lock())
    {
        return kThermostatDBStatus_LockFail;
    }

    uint32_t maxCount = _HAL_Db_GetMaxCount();
    if (id > maxCount)
    {
        _HAL_Db_Unlock();
        return kThermostatDBStatus_Failed;
    }

    if (_HAL_Db_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE)
    {
        thermostat_attribute_t *pAttrEntry = _HAL_Db_GetEntry(id);
        memcpy(attr, pAttrEntry, _HAL_Db_GetEntrySize());
    }

    _HAL_Db_Unlock();
    return status;
}
