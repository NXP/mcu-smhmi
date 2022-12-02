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

#include "hal_sln_elevatordb.h"

#define ELEVATOR_DIR "elevator"

#define METADATA_FILE_NAME \
    ELEVATOR_DIR           \
    "/"                    \
    "Metadata"

#define ELEVATOR_DATABASE \
    ELEVATOR_DIR          \
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

static elevatordb_status_t HAL_ElevatorDb_Init(void);
static elevatordb_status_t HAL_ElevatorDb_Deinit(void);
static elevatordb_status_t HAL_ElevatorDb_AddWithId(uint16_t id, elevator_attr_t *attr);
static elevatordb_status_t HAL_ElevatorDb_DelWithId(uint16_t id);
static elevatordb_status_t HAL_ElevatorDb_UpdWithId(uint16_t id, elevator_attr_t *attr);
static elevatordb_status_t HAL_ElevatorDb_GetWithId(uint16_t id, elevator_attr_t *attr);

typedef struct _elevatordb_metadata
{
    uint32_t version;
    uint32_t count;
    uint32_t maxCount;
    uint16_t entrySize;
    uint32_t bufferSize;
    uint8_t reserved[8];
    uint8_t map[ELEVATOR_DB_MAX_SIZE];
} elevatordb_metadata_t;

const elevatordb_ops_t g_elevatordb_ops = {
    .init      = HAL_ElevatorDb_Init,
    .deinit    = HAL_ElevatorDb_Deinit,
    .addWithId = HAL_ElevatorDb_AddWithId,
    .delWithId = HAL_ElevatorDb_DelWithId,
    .updWithId = HAL_ElevatorDb_UpdWithId,
    .getWithId = HAL_ElevatorDb_GetWithId,
};

static elevatordb_metadata_t s_elevatorDbMetaData;
static uint8_t *s_pElevatorDbBuffer       = NULL;
static SemaphoreHandle_t s_elevatorDbLock = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/
static elevatordb_status_t _HAL_ElevatorDb_Lock()
{
    if (NULL == s_elevatorDbLock)
    {
        return kElevatorDBStatus_LockFail;
    }

    if (pdTRUE != xSemaphoreTake(s_elevatorDbLock, portMAX_DELAY))
    {
        return kElevatorDBStatus_LockFail;
    }

    return kElevatorDBStatus_Success;
}

static void _HAL_ElevatorDb_Unlock()
{
    if (s_elevatorDbLock != NULL)
    {
        xSemaphoreGive(s_elevatorDbLock);
    }
}

static void _HAL_ElevatorDb_GeneratePathFromIndex(uint16_t id, char *path)
{
    if (path != NULL)
    {
        char index[4];
        itoa(id, index, 10);
        strcpy(path, ELEVATOR_DATABASE);
        strcat(path, index);
    }
}

static uint32_t _HAL_ElevatorDb_GetEntrySize(void)
{
    return s_elevatorDbMetaData.entrySize;
}

static uint32_t _HAL_ElevatorDb_GetMaxCount(void)
{
    return s_elevatorDbMetaData.maxCount;
}

static uint32_t _HAL_ElevatorDb_GetBufferSize(void)
{
    return s_elevatorDbMetaData.bufferSize;
}

static uint8_t _HAL_ElevatorDb_CheckMapFlag(uint16_t id, uint8_t bitWise)
{
    return (s_elevatorDbMetaData.map[id] & 1 << bitWise);
}

static void _HAL_ElevatorDb_SetMapFlag(uint16_t id, uint8_t flag)
{
    if ((_HAL_ElevatorDb_CheckMapFlag(id, kMappingBitWise_Used) == NOT_USED) &&
        ((flag & (1 << kMappingBitWise_Used)) == IN_USE))
    {
        s_elevatorDbMetaData.map[id] = flag;
        s_elevatorDbMetaData.count++;
    }
    else if ((_HAL_ElevatorDb_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE) &&
             ((flag & (1 << kMappingBitWise_Used)) == NOT_USED))
    {
        s_elevatorDbMetaData.map[id] = flag;
        s_elevatorDbMetaData.count--;
    }
}

#define _HAL_ElevatorDb_AndMapFlag(id, flag)   \
    {                                          \
        s_elevatorDbMetaData.map[id] &= ~flag; \
    }

#define _HAL_ElevatorDb_OrMapFlag(id, flag)   \
    {                                         \
        s_elevatorDbMetaData.map[id] |= flag; \
    }

static elevator_attr_t *_HAL_ElevatorDb_GetEntry(uint16_t id)
{
    return (elevator_attr_t *)(s_pElevatorDbBuffer + _HAL_ElevatorDb_GetEntrySize() * id);
}

static elevatordb_status_t _HAL_ElevatorDb_SetMetaDataDefault(void)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;

    memset((void *)&s_elevatorDbMetaData, 0x0, sizeof(elevatordb_metadata_t));
    s_elevatorDbMetaData.version    = ELEVATOR_DB_VERSION;
    s_elevatorDbMetaData.maxCount   = ELEVATOR_DB_MAX_SIZE;
    s_elevatorDbMetaData.bufferSize = sizeof(elevator_attr_t) * ELEVATOR_DB_MAX_SIZE;
    s_elevatorDbMetaData.entrySize  = sizeof(elevator_attr_t);

    return status;
}

static elevatordb_status_t _HAL_ElevatorDb_SaveMetaData(void)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;
    sln_flash_status_t ret = FWK_Flash_Save(METADATA_FILE_NAME, &s_elevatorDbMetaData, sizeof(elevatordb_metadata_t));

    if (kStatus_HAL_FlashSuccess != ret)
    {
        status = kElevatorDBStatus_MetaDataFail;
    }

    return status;
}

static elevatordb_status_t _HAL_ElevatorDb_SetDefault(uint8_t *pDbBuf)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;

    uint32_t bufferSize = _HAL_ElevatorDb_GetBufferSize();
    memset(pDbBuf, 0x0, bufferSize);

    return status;
}

static elevatordb_status_t _HAL_ElevatorDb_EraseAll(void)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;

    for (uint16_t id = 0; id < ELEVATOR_DB_MAX_SIZE; id++)
    {
        if ((s_elevatorDbMetaData.map[id] & (1 << kMappingBitWise_Used)) == IN_USE)
        {
            s_elevatorDbMetaData.map[id] &= ~(1 << kMappingBitWise_Used);
            s_elevatorDbMetaData.count--;

            char path[20];
            _HAL_ElevatorDb_GeneratePathFromIndex(id, path);
            sln_flash_status_t ret = FWK_Flash_Rm(path);
            if (ret != kStatus_HAL_FlashSuccess && ret != kStatus_HAL_FlashFileNotExist)
            {
                LOGE("ElevatorDb: Failed to remove from database.");
                continue;
            }
        }
    }

    return status;
}

static elevatordb_status_t _HAL_ElevatorDb_LoadMetaData(void)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;
    sln_flash_status_t ret     = FWK_Flash_Mkdir(ELEVATOR_DIR);

    if (ret == kStatus_HAL_FlashDirExist)
    {
        uint32_t len = sizeof(elevatordb_metadata_t);
        ret          = FWK_Flash_Read(METADATA_FILE_NAME, &s_elevatorDbMetaData, 0, &len);
        if (ret == kStatus_HAL_FlashSuccess)
        {
            if ((s_elevatorDbMetaData.version != ELEVATOR_DB_VERSION))
            {
            	if (s_elevatorDbMetaData.version == 1)
            	{
            		LOGD("ElevatorDB: Version found in flash different from current version. Version is backward compatible");
            		s_elevatorDbMetaData.version = ELEVATOR_DB_VERSION;
            		_HAL_ElevatorDb_SaveMetaData();
            	}
            	else
            	{
                    LOGE("ElevatorDB: Version found in flash different from current version.");
                    _HAL_ElevatorDb_EraseAll();
                    _HAL_ElevatorDb_SetMetaDataDefault();
                    FWK_Flash_Save(METADATA_FILE_NAME, &s_elevatorDbMetaData, sizeof(elevatordb_metadata_t));
                    status = kElevatorDBStatus_VersionMismatch;
            	}
            }

        }
        else
        {
            LOGE("ElevatorDb: Failed to open the configuration file.");
            status = kElevatorDBStatus_MetaDataFail;
        }
    }
    else if (ret == kStatus_HAL_FlashSuccess)
    {
        _HAL_ElevatorDb_SetMetaDataDefault();
        FWK_Flash_Save(METADATA_FILE_NAME, &s_elevatorDbMetaData, sizeof(elevatordb_metadata_t));
        LOGI("ElevatorDb: Metadata saved. Database is empty.");
        status = kElevatorDBStatus_Success;
    }
    else
    {
        LOGE("ElevatorDb: Failed to initialise the metadata file.");
        status = kElevatorDBStatus_MetaDataFail;
    }

    return status;
}

static elevatordb_status_t _HAL_ElevatorDb_SaveEntryData(uint16_t id)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;
    char path[20];

    _HAL_ElevatorDb_GeneratePathFromIndex(id, path);
    sln_flash_status_t ret = FWK_Flash_Save(path, _HAL_ElevatorDb_GetEntry(id), _HAL_ElevatorDb_GetEntrySize());
    if (kStatus_HAL_FlashSuccess != ret)
    {
        status = kElevatorDBStatus_Failed;
    }

    return status;
}

static elevatordb_status_t _HAL_ElevatorDb_DelEntryData(uint16_t id)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;
    char path[20];

    _HAL_ElevatorDb_GeneratePathFromIndex(id, path);
    sln_flash_status_t ret = FWK_Flash_Rm(path);
    if (kStatus_HAL_FlashSuccess != ret && kStatus_HAL_FlashFileNotExist != ret)
    {
        status = kElevatorDBStatus_Failed;
    }

    return status;
}

static elevatordb_status_t _HAL_ElevatorDb_InitMetaData(void)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;

    status = _HAL_ElevatorDb_LoadMetaData();

    if ((kElevatorDBStatus_VersionMismatch != status) && (kElevatorDBStatus_Success != status))
    {
        status = _HAL_ElevatorDb_SetMetaDataDefault();
        if (kElevatorDBStatus_Success != _HAL_ElevatorDb_SaveMetaData())
        {
            LOGI("Elevator Metadata Save Fail");
        }
    }

    return status;
}

static elevatordb_status_t _HAL_ElevatorDb_Load(uint8_t *pDbBuf)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;
    uint32_t maxCount          = _HAL_ElevatorDb_GetMaxCount();
    sln_flash_status_t ret     = kStatus_HAL_FlashSuccess;
    uint8_t needUpdate         = 0;

    for (int id = 0; id < maxCount; id++)
    {
        if (_HAL_ElevatorDb_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE)
        {
            char path[20];
            uint32_t lenEntry = _HAL_ElevatorDb_GetEntrySize();
            _HAL_ElevatorDb_GeneratePathFromIndex(id, path);

            ret = FWK_Flash_Read(path, _HAL_ElevatorDb_GetEntry(id), 0, &lenEntry);
            if (ret != kStatus_HAL_FlashSuccess)
            {
                if (ret == kStatus_HAL_FlashFileNotExist)
                {
                    _HAL_ElevatorDb_SetMapFlag(id, 0);
                    needUpdate = 1;
                }
                else
                {
                    status = kElevatorDBStatus_DbLoadFail;
                }
            }
        }
    }

    if (needUpdate)
    {
        _HAL_ElevatorDb_SaveMetaData();
    }

    return status;
}

static elevatordb_status_t _HAL_ElevatorDb_Init(uint8_t *pDbBuf)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;

    status = _HAL_ElevatorDb_Load(pDbBuf);

    if (kElevatorDBStatus_Success != status)
    {
        _HAL_ElevatorDb_SetDefault(pDbBuf);
    }

    return status;
}

static elevatordb_status_t HAL_ElevatorDb_Init(void)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;

    status = _HAL_ElevatorDb_InitMetaData();

    if (status != kElevatorDBStatus_Success)
    {
        return status;
    }

    uint32_t maxCount             = _HAL_ElevatorDb_GetMaxCount();
    uint32_t elevatorDbEntrySize  = sizeof(elevator_attr_t);
    uint32_t elevatorDbBufferSize = elevatorDbEntrySize * maxCount;
    s_pElevatorDbBuffer           = pvPortMalloc(elevatorDbBufferSize);

    if (NULL == s_pElevatorDbBuffer)
    {
        LOGE("Elevator DB Entry Malloc Fail");
        return kElevatorDBStatus_MallocFail;
    }
    _HAL_ElevatorDb_Init(s_pElevatorDbBuffer);

    if (NULL == s_elevatorDbLock)
    {
        s_elevatorDbLock = xSemaphoreCreateMutex();

        if (NULL == s_elevatorDbLock)
        {
            LOGE("Elevator: Failed to create DB lock semaphore");
            vPortFree(s_pElevatorDbBuffer);
            status = kElevatorDBStatus_LockFail;
        }
    }

    return status;
}

static elevatordb_status_t HAL_ElevatorDb_Deinit(void)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;

    if (NULL != s_pElevatorDbBuffer)
    {
        vPortFree(s_pElevatorDbBuffer);
    }

    return status;
}

static elevatordb_status_t HAL_ElevatorDb_AddWithId(uint16_t id, elevator_attr_t *attr)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;

    if (NULL == s_pElevatorDbBuffer)
    {
        return kElevatorDBStatus_MallocFail;
    }

    if (kElevatorDBStatus_Success != _HAL_ElevatorDb_Lock())
    {
        return kElevatorDBStatus_LockFail;
    }

    uint32_t maxCount = _HAL_ElevatorDb_GetMaxCount();
    if (id > maxCount)
    {
        _HAL_ElevatorDb_Unlock();
        return kElevatorDBStatus_Failed;
    }

    if (_HAL_ElevatorDb_CheckMapFlag(id, kMappingBitWise_Used) == NOT_USED)
    {
        elevator_attr_t *pCoffeeAttrEntry = _HAL_ElevatorDb_GetEntry(id);
        memcpy(pCoffeeAttrEntry, attr, _HAL_ElevatorDb_GetEntrySize());

        _HAL_ElevatorDb_SetMapFlag(id, IN_USE);

        _HAL_ElevatorDb_SaveEntryData(id);
        _HAL_ElevatorDb_SaveMetaData();
    }
    else
    {
        // TODO: IN_USE
    }

    _HAL_ElevatorDb_Unlock();

    return status;
}

static elevatordb_status_t HAL_ElevatorDb_DelWithId(uint16_t id)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;

    if (NULL == s_pElevatorDbBuffer)
    {
        return kElevatorDBStatus_MallocFail;
    }

    if (kElevatorDBStatus_Success != _HAL_ElevatorDb_Lock())
    {
        return kElevatorDBStatus_LockFail;
    }

    if (id == INVALID_ID)
    {
        _HAL_ElevatorDb_EraseAll();
        _HAL_ElevatorDb_SetMetaDataDefault();
        FWK_Flash_Save(METADATA_FILE_NAME, &s_elevatorDbMetaData, sizeof(elevatordb_metadata_t));
        _HAL_ElevatorDb_Unlock();
        return kElevatorDBStatus_Success;
    }

    uint32_t maxCount = _HAL_ElevatorDb_GetMaxCount();
    if (id > maxCount)
    {
        _HAL_ElevatorDb_Unlock();
        return kElevatorDBStatus_Failed;
    }

    if (_HAL_ElevatorDb_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE)
    {
        elevator_attr_t *pElevatorAttrEntry = _HAL_ElevatorDb_GetEntry(id);
        memset(pElevatorAttrEntry, 0, _HAL_ElevatorDb_GetEntrySize());

        _HAL_ElevatorDb_SetMapFlag(id, NOT_USED);

        _HAL_ElevatorDb_DelEntryData(id);
        _HAL_ElevatorDb_SaveMetaData();
    }

    _HAL_ElevatorDb_Unlock();
    return status;
}

static elevatordb_status_t HAL_ElevatorDb_UpdWithId(uint16_t id, elevator_attr_t *attr)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;

    if (NULL == s_pElevatorDbBuffer)
    {
        return kElevatorDBStatus_MallocFail;
    }

    if (kElevatorDBStatus_Success != _HAL_ElevatorDb_Lock())
    {
        return kElevatorDBStatus_LockFail;
    }

    uint32_t maxCount = _HAL_ElevatorDb_GetMaxCount();
    if (id > maxCount)
    {
        _HAL_ElevatorDb_Unlock();
        return kElevatorDBStatus_Failed;
    }

    if (_HAL_ElevatorDb_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE)
    {
        elevator_attr_t *pElevatorAttrEntry = _HAL_ElevatorDb_GetEntry(id);
        memcpy(pElevatorAttrEntry, attr, _HAL_ElevatorDb_GetEntrySize());

        _HAL_ElevatorDb_SetMapFlag(id, IN_USE);

        _HAL_ElevatorDb_SaveEntryData(id);
        _HAL_ElevatorDb_SaveMetaData();
    }

    _HAL_ElevatorDb_Unlock();
    return status;
}

static elevatordb_status_t HAL_ElevatorDb_GetWithId(uint16_t id, elevator_attr_t *attr)
{
    elevatordb_status_t status = kElevatorDBStatus_Success;

    if (NULL == s_pElevatorDbBuffer)
    {
        return kElevatorDBStatus_MallocFail;
    }

    if (kElevatorDBStatus_Success != _HAL_ElevatorDb_Lock())
    {
        return kElevatorDBStatus_LockFail;
    }

    uint32_t maxCount = _HAL_ElevatorDb_GetMaxCount();
    if (id > maxCount)
    {
        _HAL_ElevatorDb_Unlock();
        return kElevatorDBStatus_Failed;
    }

    if (_HAL_ElevatorDb_CheckMapFlag(id, kMappingBitWise_Used) == IN_USE)
    {
        elevator_attr_t *pElevatorAttrEntry = _HAL_ElevatorDb_GetEntry(id);
        memcpy(attr, pElevatorAttrEntry, _HAL_ElevatorDb_GetEntrySize());
    }

    _HAL_ElevatorDb_Unlock();
    return status;
}
