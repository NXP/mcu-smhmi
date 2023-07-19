/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief sln face database reference implementation.
 * Simple RAM-based database for reference and algorithm bring up purposes.
 */

#include "board_define.h"
#ifdef ENABLE_FACEDB
#include <FreeRTOS.h>
#include "semphr.h"

#include "fwk_log.h"
#include "fwk_flash.h"
#include "hal_sln_facedb.h"
#include "hal_flash_dev.h"
#include "stdio.h"

#if defined(AUTOSAVE) & (AUTOSAVE == 1)
#warning "A screen flicker might be observed when registering faces if autosave is enabled."
#endif

#define FACE_ENTRY(id) (s_FaceDB + (id * s_FaceEntrySize))

#define FEATURE_VERSION 0x0002
/* TODO this needs to be defined at runtime */
#define MODEL_VERSION 0x0001

#define FACEDB_SLOT_EMPTY 0x0

#define RESERVED_DATA 0x6

#define METADATA_FILE_NAME \
    OASIS_FACE_DB_DIR      \
    "/"                    \
    "Metadata"

#define OASIS_FACEDATABASE \
    OASIS_FACE_DB_DIR      \
    "/"

typedef enum _face_mapping_bitwise
{
    kFaceMappingBitWise_Saved,
    kFaceMappingBitWise_Updated,
    kFaceMappingBitWise_Used,
} face_mapping_bitwise_t;

#define FACE_NOT_USED    0X0
#define FACE_IN_USE      (1 << kFaceMappingBitWise_Used)
#define FACE_NOT_SAVED   0x0
#define FACE_SAVED       (1 << kFaceMappingBitWise_Saved)
#define FACE_NOT_UPDATED 0x0
#define FACE_UPDATED     (1 << kFaceMappingBitWise_Updated)

typedef struct _facedb_metadata
{
    uint32_t featureVersion;
    uint32_t modelVersion;
    uint8_t numberFaces;
    uint16_t faceEntrySize;
    /* RESERVED DATA for future updates */
    uint8_t reservedData[RESERVED_DATA];
    uint8_t faceMapping[MAX_FACE_DB_SIZE];
} facedb_metadata_t;

typedef struct _facedb_entry
{
    char name[FACE_NAME_MAX_LEN + 1];
    /* Just mark the beginning. Face Feature size is dynamic */
    unsigned char face[];
} facedb_entry_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static int _Facedb_Lock();
static void _Facedb_Unlock();
static void _Facedb_SetMetaDataDefault();
static void _Facedb_SetFaceDataDefault();
static facedb_status_t _Facedb_Init();
static sln_flash_status_t _Facedb_Load();
static sln_flash_status_t _Facedb_UpdateMetadata();
static sln_flash_status_t _Facedb_SaveFace(uint16_t id);
static sln_flash_status_t _Facedb_DeleteFace(uint16_t id);
static sln_flash_status_t _Facedb_DeleteAllFaces();

static facedb_status_t HAL_Facedb_Init(uint16_t featureSize);
static facedb_status_t HAL_Facedb_SaveFace(void);
static facedb_status_t HAL_Facedb_AddFace(uint16_t id, char *name, void *face, int size);
static facedb_status_t HAL_Facedb_DelFaceWithID(uint16_t id);
static facedb_status_t HAL_Facedb_DelFaceWithName(char *name);
static facedb_status_t HAL_Facedb_GetIdsAndFaces(uint16_t *face_ids, void **pFace);
static facedb_status_t HAL_Facedb_GetFace(uint16_t id, void **pFace);
static facedb_status_t HAL_Facedb_GenId(uint16_t *new_id);
static facedb_status_t HAL_Facedb_GetIds(uint16_t *face_ids);
static bool HAL_Facedb_GetSaveStatus(uint16_t id);
static int HAL_Facedb_GetCount(void);
static char *HAL_Facedb_GetName(uint16_t id);
static facedb_status_t HAL_Facedb_UpdateName(uint16_t id, char *name);
static facedb_status_t HAL_Facedb_UpdateFace(uint16_t id, char *name, void *face, int size);
static facedb_status_t HAL_Facedb_GetIdWithName(char *name, uint16_t *pId);

/* Database buffer */
static uint16_t s_FaceEntrySize;
static uint32_t s_FaceDBSize;
static uint8_t *s_FaceDB              = NULL;
static SemaphoreHandle_t s_FaceDBLock = NULL;

static facedb_metadata_t s_OasisMetadata;
const facedb_ops_t g_facedb_ops = {
    .init            = HAL_Facedb_Init,
    .saveFace        = HAL_Facedb_SaveFace,
    .addFace         = HAL_Facedb_AddFace,
    .delFaceWithId   = HAL_Facedb_DelFaceWithID,
    .delFaceWithName = HAL_Facedb_DelFaceWithName,
    .updNameWithId   = HAL_Facedb_UpdateName,
    .updFaceWithId   = HAL_Facedb_UpdateFace,
    .getFaceWithId   = HAL_Facedb_GetFace,
    .getIdsAndFaces  = HAL_Facedb_GetIdsAndFaces,
    .genId           = HAL_Facedb_GenId,
    .getIds          = HAL_Facedb_GetIds,
    .getSaveStatus   = HAL_Facedb_GetSaveStatus,
    .getFaceCount    = HAL_Facedb_GetCount,
    .getNameWithId   = HAL_Facedb_GetName,
    .getIdWithName   = HAL_Facedb_GetIdWithName,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
// internal function to take the database lock
static int _Facedb_Lock()
{
    if (pdTRUE != xSemaphoreTake(s_FaceDBLock, portMAX_DELAY))
    {
        return kFaceDBStatus_Failed;
    }

    return kFaceDBStatus_Success;
}

/* internal function to release the database lock */
static void _Facedb_Unlock()
{
    if (s_FaceDBLock != NULL)
    {
        xSemaphoreGive(s_FaceDBLock);
    }
}

static void _Facedb_SetMetaDataDefault()
{
    s_OasisMetadata.featureVersion = FEATURE_VERSION;
    s_OasisMetadata.modelVersion   = MODEL_VERSION;
    s_OasisMetadata.numberFaces    = 0;
    s_OasisMetadata.faceEntrySize  = 0;
    memset(s_OasisMetadata.faceMapping, FACEDB_SLOT_EMPTY, MAX_FACE_DB_SIZE * sizeof(uint8_t));
}

static void _Facedb_SetFaceDataDefault()
{
    memset(s_FaceDB, 0, s_FaceDBSize);
}

static void _Facedb_GeneratePathFromIndex(uint16_t id, char *path)
{
    if (path != NULL)
    {
        char index[4];
        itoa(id, index, 10);
        strcpy(path, OASIS_FACEDATABASE);
        strcat(path, index);
    }
}

static sln_flash_status_t _Facedb_UpdateMetadata()
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;
    facedb_metadata_t oasisMetadata;
    oasisMetadata.featureVersion = s_OasisMetadata.featureVersion;
    oasisMetadata.modelVersion   = s_OasisMetadata.modelVersion;
    oasisMetadata.numberFaces    = s_OasisMetadata.numberFaces;
    oasisMetadata.faceEntrySize  = s_OasisMetadata.faceEntrySize;
    for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
    {
        /* Clear the used flag */
        if (s_OasisMetadata.faceMapping[id] == (1 << kFaceMappingBitWise_Used))
        {
            oasisMetadata.numberFaces--;
        }

        oasisMetadata.faceMapping[id] = s_OasisMetadata.faceMapping[id] & (~(1 << kFaceMappingBitWise_Used));
    }

    do
    {
        LOGI("FaceDB: Trying to update metadata in flash...");
        status = FWK_Flash_Save(METADATA_FILE_NAME, &oasisMetadata, sizeof(facedb_metadata_t));
    } while (status != kStatus_HAL_FlashSuccess);

    LOGI("FaceDB: Metadata updated.");

    return status;
}

static sln_flash_status_t _Facedb_Load()
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;
    bool updateMetadata       = false;

    for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
    {
        if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Saved)) == FACE_SAVED)
        {
            char path[20];
            _Facedb_GeneratePathFromIndex(id, path);
            status = FWK_Flash_Read(path, (FACE_ENTRY(id)), 0, &s_OasisMetadata.faceEntrySize);
            if (status != kStatus_HAL_FlashSuccess)
            {
                LOGE("FaceDB: Failed to load face database at path \"%s\".", path);
                if (status == kStatus_HAL_FlashFileNotExist)
                {
                    updateMetadata                  = true;
                    s_OasisMetadata.faceMapping[id] = FACEDB_SLOT_EMPTY;
                    s_OasisMetadata.numberFaces--;
                }
                else
                {
                    LOGE("FaceDB: Status \"%d\".", status);
                    break;
                }
            }
            else
            {
                s_OasisMetadata.faceMapping[id] = FACE_SAVED | FACE_IN_USE;
            }
        }
        else if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Updated)) == FACE_UPDATED)
        {
            /* loads older version */
            char path[20];
            LOGD("FaceDB: Update operation not saved on last run for id %d, load older version", id);
            _Facedb_GeneratePathFromIndex(id, path);
            status         = FWK_Flash_Read(path, (FACE_ENTRY(id)), 0, &s_OasisMetadata.faceEntrySize);
            updateMetadata = true;
            if (status != kStatus_HAL_FlashSuccess)
            {
                LOGE("FaceDB: Failed to load face database at path \"%s\".", path);
                if (status == kStatus_HAL_FlashFileNotExist)
                {
                    s_OasisMetadata.faceMapping[id] = FACEDB_SLOT_EMPTY;
                    s_OasisMetadata.numberFaces--;
                }
                else
                {
                    LOGE("FaceDB: Status \"%d\".", status);
                    break;
                }
            }
            else
            {
                s_OasisMetadata.faceMapping[id] = FACE_SAVED | FACE_IN_USE;
            }
        }
        else if (s_OasisMetadata.faceMapping[id] != FACEDB_SLOT_EMPTY)
        {
            LOGE("FaceDB: Wrong metadata for id \"%d\".", id);
            status = kStatus_HAL_FlashFail;
            break;
        }
    }

    if ((status == kStatus_HAL_FlashSuccess) && (updateMetadata == true))
    {
        _Facedb_UpdateMetadata();
    }

    return status;
}

static facedb_status_t _Facedb_Init()
{
    sln_flash_status_t status = FWK_Flash_Mkdir(OASIS_FACE_DB_DIR);
    facedb_status_t ret       = kFaceDBStatus_Success;
    if (status == kStatus_HAL_FlashDirExist)
    {
        /* Already exists assume everything is ok don't over engineer for now */
        unsigned int len = sizeof(facedb_metadata_t);
        status       = FWK_Flash_Read(METADATA_FILE_NAME, &s_OasisMetadata, 0, &len);
        if (status == kStatus_HAL_FlashSuccess)
        {
            if ((s_OasisMetadata.featureVersion != FEATURE_VERSION) || (s_OasisMetadata.modelVersion != MODEL_VERSION))
            {
                LOGE(
                    "FaceDB: Oasis_Version found in flash different from current version. Features might be "
                    "different.");
                /* TODO: Implement a recovery strategy */
                _Facedb_DeleteAllFaces();
                _Facedb_SetMetaDataDefault();
                FWK_Flash_Save(METADATA_FILE_NAME, &s_OasisMetadata, sizeof(facedb_metadata_t));

                ret = kFaceDBStatus_Success;
            }
            else
            {
                /* Same version */
                LOGI("FaceDB: Same version. Number of faces %d.", s_OasisMetadata.numberFaces);
                if (s_OasisMetadata.faceEntrySize > s_FaceEntrySize)
                {
                    LOGE("FaceDB: Existing faces are much bigger than the current allocated memory. Failed to load.");
                    ret = kFaceDBStatus_NotEnoughMemory;
                }
                else
                {
                    status = _Facedb_Load();
                    if (status == kStatus_HAL_FlashSuccess)
                    {
                        ret = kFaceDBStatus_Success;
                    }
                    else
                    {
                        ret = kFaceDBStatus_Failed;
                    }
                }
            }
        }
        else
        {
            LOGE("FaceDB: Failed to open the configuration file.");
            ret = kFaceDBStatus_Failed;
        }
    }
    else if (status == kStatus_HAL_FlashSuccess)
    {
        /* First time write default */
        _Facedb_SetMetaDataDefault();
        FWK_Flash_Save(METADATA_FILE_NAME, &s_OasisMetadata, sizeof(facedb_metadata_t));
        LOGI("FaceDB: Metadata saved. Database is empty.");
        ret = kFaceDBStatus_Success;
    }
    else
    {
        LOGE("FaceDB: Failed to initialize the metadata file.");
        ret = kFaceDBStatus_Failed;
    }

    s_OasisMetadata.faceEntrySize = s_FaceEntrySize;
    return ret;
}

static sln_flash_status_t _Facedb_SaveFace(uint16_t id)
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;

    /* Save in a file */
    char path[20];
    _Facedb_GeneratePathFromIndex(id, path);

    status = FWK_Flash_Save(path, (FACE_ENTRY(id)), s_FaceEntrySize);
    if (status == kStatus_HAL_FlashSuccess)
    {
        /* Update metadata */
        s_OasisMetadata.faceMapping[id] = FACE_IN_USE | FACE_SAVED;
    }
    else
    {
        LOGE("FaceDB: Failed to save face.");
    }

    return status;
}

static sln_flash_status_t _Facedb_DeleteFaceFromFlash(uint16_t id)
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;
    char path[20];

    LOGD("FaceDB: delete file from flash id %d", id);
    _Facedb_GeneratePathFromIndex(id, path);
    status = FWK_Flash_Rm(path);
    if (status == kStatus_HAL_FlashSuccess || status == kStatus_HAL_FlashFileNotExist)
    {
        s_OasisMetadata.faceMapping[id] &= ~FACE_SAVED;
        s_OasisMetadata.faceMapping[id] &= ~FACE_UPDATED;
    }

    return status;
}

static sln_flash_status_t _Facedb_DeleteAllFaces()
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;
    uint8_t updateMetadata    = false;

    for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
    {
        /* Try to delete gracefully */
        if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
        {
            /* Delete from RAM */
            memset((FACE_ENTRY(id)), 0, s_FaceEntrySize);
            s_OasisMetadata.faceMapping[id] &= ~(1 << kFaceMappingBitWise_Used);
            s_OasisMetadata.numberFaces--;

            if (((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Saved)) == FACE_SAVED) ||
                (s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Updated)) == FACE_UPDATED)
            {
                status = _Facedb_DeleteFaceFromFlash(id);
                if (status != kStatus_HAL_FlashSuccess && status != kStatus_HAL_FlashFileNotExist)
                {
                    LOGE("FaceDB: Failed to remove face from database.");
                    continue;
                }
                updateMetadata = true;
            }
        }
    }

    if ((s_OasisMetadata.numberFaces != 0) || (status != kStatus_HAL_FlashSuccess))
    {
        /* Something went wrong delete all from RAM and clean metadata */
        LOGE("FaceDB: Something went wrong delete all from RAM and clean metadata")
        _Facedb_SetFaceDataDefault();
        _Facedb_SetMetaDataDefault();
        updateMetadata = true;
    }
    else
    {
        LOGI("FaceDb: Metadata is empty. Updating metadata in flash...");
    }

    if ((status == kStatus_HAL_FlashSuccess) && (updateMetadata == true))
    {
        status = _Facedb_UpdateMetadata();
    }

    return status;
}

static sln_flash_status_t _Facedb_DeleteFace(uint16_t id)
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;

    if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
    {
        /* Delete from RAM */
        LOGD("FaceDb: delete file from ram id %d", id);
        memset((FACE_ENTRY(id)), 0, s_FaceEntrySize);
        s_OasisMetadata.faceMapping[id] &= ~(1 << kFaceMappingBitWise_Used);
        s_OasisMetadata.numberFaces--;

        /* Delete from flash */
        if (((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Saved)) == FACE_SAVED) ||
            (s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Updated)) == FACE_UPDATED)
        {
            status = _Facedb_DeleteFaceFromFlash(id);

            if ((status != kStatus_HAL_FlashSuccess) && (status != kStatus_HAL_FlashFileNotExist))
            {
                LOGE("FaceDB: Failed to delete face from id: %d.", id);
                status = kStatus_HAL_FlashFail;
            }
            else
            {
                status = _Facedb_UpdateMetadata();
            }
        }
    }

    return status;
}

static facedb_status_t _Facedb_GetIdFromName(char *name, uint16_t *pId)
{
    *pId = INVALID_ID;
    for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
    {
        facedb_entry_t *faceEntry = (facedb_entry_t *)(FACE_ENTRY(id));
        if (strcmp(name, faceEntry->name) == 0)
        {
            *pId = id;
            return kFaceDBStatus_Success;
        }
    }

    return kFaceDBStatus_Failed;
}

/* face database init */
facedb_status_t HAL_Facedb_Init(uint16_t featureSize)
{
    facedb_status_t status = kFaceDBStatus_Success;

    if (NULL == s_FaceDB)
    {
        if (featureSize == 0)
        {
            status = kFaceDBStatus_NotEnoughMemory;
        }
        else
        {
            s_FaceDBSize = (featureSize + FACE_NAME_MAX_LEN) * MAX_FACE_DB_SIZE;
            s_FaceDB     = pvPortMalloc(s_FaceDBSize);

            if (NULL == s_FaceDB)
            {
                LOGE("FaceDb: Failed to allocate face DB buffer");
                status = kFaceDBStatus_NotEnoughMemory;
            }
            s_FaceEntrySize = featureSize + sizeof(((facedb_entry_t *)0)->name);
        }
    }

    if ((status == kFaceDBStatus_Success) && (NULL == s_FaceDBLock))
    {
        s_FaceDBLock = xSemaphoreCreateMutex();

        if (NULL == s_FaceDBLock)
        {
            LOGE("FaceDb: Failed to create DB lock semaphore");
            vPortFree(s_FaceDB);
            s_FaceDB = NULL;
            status   = kFaceDBStatus_NotEnoughMemory;
        }
    }

    if (status == kFaceDBStatus_Success)
    {
        status = _Facedb_Init();
    }

    return status;
}

/* get the face item count in the database */
int HAL_Facedb_GetCount(void)
{
    return s_OasisMetadata.numberFaces;
}

facedb_status_t HAL_Facedb_SaveFace(void)
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;
    facedb_status_t ret       = kFaceDBStatus_Success;
    uint8_t updateMetadata    = false;

    if ((s_FaceDB == NULL) || (s_FaceDBLock == NULL))
    {
        ret = kFaceDBStatus_NotInit;
    }
    else
    {
        ret = _Facedb_Lock();
    }

    if (ret != kFaceDBStatus_Success)
    {
        return ret;
    }

    for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
    {
        /* Try to save */
        if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
        {
            if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Saved)) == FACE_NOT_SAVED)
            {
                status = _Facedb_SaveFace(id);
                if (status == kStatus_HAL_FlashSuccess)
                {
                    LOGD("FaceDb: Face with id \"%d\" was successfully saved to flash.", id);
                    updateMetadata = true;
                }
                else
                {
                    LOGE("FaceDb: Saving face with id \"%d\" failed with error code \"%d\".", id, status);
                }
            }
        }
    }

    LOGI("FaceDb: Finished saving faces to flash.");

    /* Update Flash metadata */
    if ((status == kStatus_HAL_FlashSuccess) && (updateMetadata == true))
    {
        status = _Facedb_UpdateMetadata();
    }

    if (status != kStatus_HAL_FlashSuccess)
    {
        ret = kFaceDBStatus_Failed;
    }

    return ret;
}

facedb_status_t HAL_Facedb_AddFace(uint16_t id, char *name, void *face, int size)
{
    facedb_status_t ret = kFaceDBStatus_Success;

    if ((s_FaceDB == NULL) || (s_FaceDBLock == NULL))
    {
        ret = kFaceDBStatus_NotInit;
    }
    else if ((size >= s_FaceEntrySize) || (id >= MAX_FACE_DB_SIZE) || (face == NULL))
    {
        ret = kFaceDBStatus_WrongParam;
    }
    else
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_NOT_USED)
        {
            /* Update RAM face */
            facedb_entry_t *faceEntry = (facedb_entry_t *)(FACE_ENTRY(id));
            if (name != NULL)
            {
                strcpy(faceEntry->name, name);
            }
            else
            {
                sprintf(faceEntry->name, "user_%03d", id);
                faceEntry->name[FACE_NAME_MAX_LEN - 1] = 0;
            }

            memcpy(faceEntry->face, face, size);

            s_OasisMetadata.faceMapping[id] = FACE_IN_USE;
            s_OasisMetadata.numberFaces++;

            LOGD("FaceDb: Added face to RAM successfully :%d %s.", id, faceEntry->name);

#if AUTOSAVE
            /* Save to Flash */
            sln_flash_status_t status = kStatus_HAL_FlashSuccess;
            status                    = _Facedb_SaveFace(id);
            if (status == kStatus_HAL_FlashSuccess)
            {
                LOGD("FaceDb: Added face to flash successfully :%d %s.", id, faceEntry->name);

                /* Update Flash metadata */
                _Facedb_UpdateMetadata();

                ret = kFaceDBStatus_Success;
            }
            else
            {
                LOGE("FaceDb: Failed to save face into flash.");
                ret = kFaceDBStatus_Failed;
            }
#endif /* AUTOSAVE */
        }
        else
        {
            LOGE("FaceDb: Face slot for ID %d already in use.", id);
        }
        _Facedb_Unlock();
    }
    return ret;
}

facedb_status_t HAL_Facedb_DelFaceWithName(char *name)
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;
    facedb_status_t ret       = kFaceDBStatus_Success;

    if ((s_FaceDB == NULL) || (s_FaceDBLock == NULL))
    {
        ret = kFaceDBStatus_NotInit;
    }
    else if (name == NULL)
    {
        ret = kFaceDBStatus_WrongParam;
    }
    else
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        uint16_t id;
        _Facedb_GetIdFromName(name, &id);
        if (id != INVALID_ID)
        {
            /* Delete from Flash */
            status = _Facedb_DeleteFace(id);
            if (status == kStatus_HAL_FlashSuccess)
            {
                LOGI("FaceDb: Successfully deleted face:%d \"%s\".", id, name);
                ret = kFaceDBStatus_Success;
            }
            else
            {
                ret = kFaceDBStatus_Failed;
            }
        }

        _Facedb_Unlock();
    }
    return ret;
}

/*  Delete a face item in the database */
facedb_status_t HAL_Facedb_DelFaceWithID(uint16_t id)
{
    sln_flash_status_t status = kStatus_HAL_FlashSuccess;
    facedb_status_t ret       = kFaceDBStatus_Success;

    if ((s_FaceDB == NULL) || (s_FaceDBLock == NULL))
    {
        ret = kFaceDBStatus_NotInit;
    }
    else
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        if (id == INVALID_ID)
        {
            /* Delete all maybe ? */
            status = _Facedb_DeleteAllFaces();
            if (status == kStatus_HAL_FlashSuccess)
            {
                LOGD("FaceDb: Successfully deleted all registered faces.");
                ret = kFaceDBStatus_Success;
            }
            else
            {
                ret = kFaceDBStatus_Failed;
            }
        }
        else if (id >= MAX_FACE_DB_SIZE)
        {
            ret = kFaceDBStatus_WrongID;
        }
        else if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
        {
            /* Delete from Flash */
            status = _Facedb_DeleteFace(id);
            if (status == kStatus_HAL_FlashSuccess)
            {
                // TODO: Add name of deleted face
                LOGI("FaceDb: Successfully deleted face: %d.", id);
                ret = kFaceDBStatus_Success;
            }
            else
            {
                ret = kFaceDBStatus_Failed;
            }
        }
        else
        {
            LOGE("FaceDb: No face associated with the specified ID \"%d\".", id);
        }

        _Facedb_Unlock();
    }

    return ret;
}

facedb_status_t HAL_Facedb_GetIdsAndFaces(uint16_t *face_ids, void **pFace)
{
    facedb_status_t ret = kFaceDBStatus_Success;
    uint16_t index      = 0;

    if ((s_FaceDB == NULL) || (s_FaceDBLock == NULL))
    {
        ret = kFaceDBStatus_NotInit;
    }
    else if ((face_ids == NULL) || (pFace == NULL))
    {
        ret = kFaceDBStatus_WrongParam;
    }
    else
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
        {
            facedb_entry_t *faceEntry = (facedb_entry_t *)(FACE_ENTRY(id));
            if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
            {
                face_ids[index]  = id;
                *(pFace + index) = &faceEntry->face;
                index++;
            }
        }
        _Facedb_Unlock();
    }

    return ret;
}

/* get the face item pointer with the specified face id from the database */
facedb_status_t HAL_Facedb_GetFace(uint16_t id, void **pFace)
{
    facedb_status_t ret = kFaceDBStatus_Success;

    if ((s_FaceDB == NULL) || (s_FaceDBLock == NULL))
    {
        ret = kFaceDBStatus_NotInit;
    }
    else if (pFace == NULL)
    {
        ret = kFaceDBStatus_WrongParam;
    }
    else
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        if (id >= MAX_FACE_DB_SIZE)
        {
            ret    = kFaceDBStatus_WrongID;
            *pFace = NULL;
        }
        else if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
        {
            facedb_entry_t *faceEntry = (facedb_entry_t *)(FACE_ENTRY(id));
            *pFace                    = &(faceEntry->face);
        }
        else
        {
            ret    = kFaceDBStatus_Failed;
            *pFace = NULL;
        }
        _Facedb_Unlock();
    }

    return ret;
}

bool HAL_Facedb_GetSaveStatus(uint16_t id)
{
    bool isSaved = true;

    if ((id < MAX_FACE_DB_SIZE) || (id == INVALID_ID))
    {
        if (id == INVALID_ID)
        {
            for (uint16_t i = 0; i < MAX_FACE_DB_SIZE; i++)
            {
                if (((s_OasisMetadata.faceMapping[i] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE) &&
                    ((s_OasisMetadata.faceMapping[i] & (1 << kFaceMappingBitWise_Saved)) == FACE_NOT_SAVED))
                {
                    isSaved = false;
                    break;
                }
            }
        }
        else
        {
            if (((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE) &&
                ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Saved)) == FACE_NOT_SAVED))
            {
                isSaved = false;
            }
        }
    }
    else
    {
        isSaved = false;
    }

    return isSaved;
}

/* get the name of the face item with the specified face id from the database */
char *HAL_Facedb_GetName(uint16_t id)
{
    char *face_name     = NULL;
    facedb_status_t ret = kFaceDBStatus_Success;

    if ((s_FaceDB == NULL) || (s_FaceDBLock == NULL))
    {
        ret = kFaceDBStatus_NotInit;
    }
    else if (id >= MAX_FACE_DB_SIZE)
    {
        ret = kFaceDBStatus_WrongParam;
    }
    else
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        // lock success
        if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
        {
            facedb_entry_t *faceEntry = (facedb_entry_t *)(FACE_ENTRY(id));
            face_name                 = faceEntry->name;
        }
        else
        {
            LOGE("FaceDb: No face associated with the specified ID \"%d\"", id);
        }

        _Facedb_Unlock();
    }

    return face_name;
}

/* update the name of the face item with the specified face id from the database */
facedb_status_t HAL_Facedb_UpdateName(uint16_t id, char *name)
{
    facedb_status_t ret = kFaceDBStatus_Success;

    if ((s_FaceDB == NULL) || (s_FaceDBLock == NULL))
    {
        ret = kFaceDBStatus_NotInit;
    }
    else if ((id >= MAX_FACE_DB_SIZE) || (name == NULL))
    {
        ret = kFaceDBStatus_WrongParam;
    }
    else
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        /* lock success */
        if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
        {
            facedb_entry_t *faceEntry = (facedb_entry_t *)(FACE_ENTRY(id));
            uint8_t nameSize = (FACE_NAME_MAX_LEN < strlen(name)) ? (FACE_NAME_MAX_LEN + 1) : (strlen(name) + 1);
            memcpy(faceEntry->name, name, nameSize);
#if AUTOSAVE
            sln_flash_status_t status = kStatus_HAL_FlashSuccess;

            /* Save to Flash */
            status = _Facedb_SaveFace(id);
            if (status == kStatus_HAL_FlashSuccess)
            {
                LOGD("FaceDb: Added Flash success :%d %s \r\n", id, name);
            }
#else
            if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Saved)) == FACE_SAVED)
            {
                s_OasisMetadata.faceMapping[id] &= ~FACE_SAVED;
                s_OasisMetadata.faceMapping[id] |= FACE_UPDATED;
            }
#endif /* AUTOSAVE */
        }
        else
        {
            LOGE("FaceDb: FaceDb: associated with the specified ID \"%d\"", id);
        }

        _Facedb_Unlock();
    }

    return ret;
}

/* update old face item into the database */
facedb_status_t HAL_Facedb_UpdateFace(uint16_t id, char *name, void *face, int size)
{
    facedb_status_t ret = kFaceDBStatus_Success;

    if ((s_FaceDB == NULL) || (s_FaceDBLock == NULL))
    {
        ret = kFaceDBStatus_NotInit;
    }
    else if ((size >= s_FaceEntrySize) || (id >= MAX_FACE_DB_SIZE) || (face == NULL) || (name == NULL))
    {
        ret = kFaceDBStatus_WrongParam;
    }
    else
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
        {
            /* Update RAM face */
            facedb_entry_t *faceEntry = (facedb_entry_t *)(FACE_ENTRY(id));
            strcpy(faceEntry->name, name);
            memcpy(&(faceEntry->face), face, size);

            LOGD("FaceDb: Successfully saved face to RAM:%d %s \r\n", id, name);
#if AUTOSAVE
            sln_flash_status_t status = kStatus_HAL_FlashSuccess;

            /* Save to Flash */
            status = _Facedb_SaveFace(id);
            if (status == kStatus_HAL_FlashSuccess)
            {
                LOGD("FaceDb: Successfully saved face to Flash:%d %s \r\n", id, name);
            }
#else
            if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Saved)) == FACE_SAVED)
            {
                s_OasisMetadata.faceMapping[id] &= ~FACE_SAVED;
                s_OasisMetadata.faceMapping[id] |= FACE_UPDATED;
            }
#endif /* AUTOSAVE */
        }
        else
        {
            LOGE("FaceDb: slot is not used for this id:%d.", id);
        }
        _Facedb_Unlock();
    }
    return ret;
}

facedb_status_t HAL_Facedb_GetIds(uint16_t *face_ids)
{
    facedb_status_t ret = kFaceDBStatus_Success;
    uint8_t index       = 0;

    if ((s_FaceDB == NULL) || (s_FaceDBLock == NULL))
    {
        ret = kFaceDBStatus_NotInit;
    }
    else if (face_ids == NULL)
    {
        ret = kFaceDBStatus_WrongParam;
    }
    else
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
        {
            if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_IN_USE)
            {
                face_ids[index++] = id;
            }
        }
        _Facedb_Unlock();
    }

    return ret;
}

/* Generate the unique face id for the new face */
facedb_status_t HAL_Facedb_GenId(uint16_t *new_id)
{
    facedb_status_t ret = kFaceDBStatus_Success;

    if ((s_FaceDB == NULL) || (s_FaceDBLock == NULL))
    {
        ret = kFaceDBStatus_NotInit;
    }
    else if (new_id == NULL)
    {
        ret = kFaceDBStatus_WrongParam;
    }
    else
    {
        ret = _Facedb_Lock();
    }

    if (ret == kFaceDBStatus_Success)
    {
        *new_id = INVALID_ID;

        if (s_OasisMetadata.numberFaces == MAX_FACE_DB_SIZE)
        {
            ret = kFaceDBStatus_Full;
        }

        if (ret == kFaceDBStatus_Success)
        {
            for (uint16_t id = 0; id < MAX_FACE_DB_SIZE; id++)
            {
                if ((s_OasisMetadata.faceMapping[id] & (1 << kFaceMappingBitWise_Used)) == FACE_NOT_USED)
                {
                    *new_id = id;
                    break;
                }
            }
        }
        _Facedb_Unlock();
    }

    return ret;
}

facedb_status_t HAL_Facedb_GetIdWithName(char *name, uint16_t *pId)
{
    return _Facedb_GetIdFromName(name, pId);
}

#endif /* ENABLE_FACEDB */
