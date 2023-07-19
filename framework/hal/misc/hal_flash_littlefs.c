/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board_define.h"
#ifdef ENABLE_FLASH_DEV_Littlefs
#include "hal_flash_dev.h"
#include "lfs.h"
#include "FreeRTOS.h"
#include "sln_flash.h"
#include "sln_flash_littlefs.h"
#include "fsl_cache.h"
#include "pin_mux.h"

#include "fwk_log.h"
#include "fwk_flash.h"
#include "fwk_lpm_manager.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Global Variables
 ******************************************************************************/

static flash_dev_t s_FlashDev_Littlefs;

static hal_lpm_request_t s_LpmReq = {.dev = &s_FlashDev_Littlefs, .name = "Flash Littlefs"};

/*******************************************************************************
 * Code
 ******************************************************************************/

static sln_flash_status_t _lfs_init()
{
    int ret = kStatus_HAL_FlashSuccess;
    sln_flash_fs_status_t status;

    SLN_Flash_Init();
    status = SLN_FLASH_LITTLEFS_Init(false);
    if (status == SLN_FLASH_FS_OK)
    {
        /* Attach callbacks */
        sln_flash_fs_cbs_t littlefsCallbacks;
        memset(&littlefsCallbacks, 0, sizeof(sln_flash_fs_cbs_t));
        status = SLN_FLASH_LITTLEFS_SetCbs(&littlefsCallbacks);
        if (status != SLN_FLASH_FS_OK)
        {
            LOGE("Failed to attach callbacks for Littlefs error %d", status);
            ret = kStatus_HAL_FlashFail;
        }
    }
    else
    {
        LOGE("Failed to init Littlefs, error %d", status);
        ret = kStatus_HAL_FlashFail;
    }

    return ret;
}

static sln_flash_status_t _lfs_cleanupHandler(const flash_dev_t *dev, unsigned int timeout_ms)
{
    int ret = kStatus_HAL_FlashSuccess;
    sln_flash_fs_status_t status;

    if (dev == NULL)
    {
        return kStatus_HAL_FlashInvalidParam;
    }
    status = SLN_FLASH_LITTLEFS_Cleanup(timeout_ms);
    if (status != SLN_FLASH_FS_OK)
    {
        LOGE("Failed to do the cleanup operation, error %d", status);
        ret = kStatus_HAL_FlashFail;
    }
    return ret;
}

static sln_flash_status_t _lfs_formatHandler(const flash_dev_t *dev)
{
    int ret = kStatus_HAL_FlashSuccess;
    sln_flash_fs_status_t status;
    status = SLN_FLASH_LITTLEFS_Format();
    if (status != SLN_FLASH_FS_OK)
    {
        LOGE("Failed to format littlefs, error %d", status);
        ret = kStatus_HAL_FlashFail;
    }
    return ret;
}

static sln_flash_status_t _lfs_rmHandler(const flash_dev_t *dev, const char *path)
{
    int ret = kStatus_HAL_FlashSuccess;
    sln_flash_fs_status_t status;

    if ((dev == NULL) || (path == NULL))
    {
        return kStatus_HAL_FlashInvalidParam;
    }

    status = SLN_FLASH_LITTLEFS_Erase(path);
    if ((status == SLN_FLASH_FS_ENOENTRY2) || (status == SLN_FLASH_FS_ENOENTRY))
    {
        LOGD("Littlefs File %s doesn't exist", path);
        ret = kStatus_HAL_FlashFileNotExist;
    }
    else if (status != SLN_FLASH_FS_OK)
    {
        LOGE("Failed to remove file %s from littlefs, error %d", path, status);
        ret = kStatus_HAL_FlashFail;
    }

    return ret;
}

static sln_flash_status_t _lfs_mkfileHandler(const flash_dev_t *dev, const char *path, bool encrypt)
{
    int ret = kStatus_HAL_FlashSuccess;
    sln_flash_fs_status_t status;

    if ((dev == NULL) || (path == NULL))
    {
        return kStatus_HAL_FlashInvalidParam;
    }

    status = SLN_FLASH_LITTLEFS_Mkfile(path, encrypt);

    if (status == SLN_FLASH_FS_ENOENTRY)
    {
        LOGD("Littlefs base directory path for %s doesn't exist", path);
        ret = kStatus_HAL_FlashFileNotExist;
    }
    else if (status == SLN_FLASH_FS_FILEEXIST)
    {
        ret = kStatus_HAL_FlashFileExist;
    }
    else if (status != SLN_FLASH_FS_OK)
    {
        LOGE("Failed to create dir %s, error %d", path, status);
        ret = kStatus_HAL_FlashFail;
    }

    return ret;
}

static sln_flash_status_t _lfs_mkdirHandler(const flash_dev_t *dev, const char *path)
{
    int ret = kStatus_HAL_FlashSuccess;
    sln_flash_fs_status_t status;

    if ((dev == NULL) || (path == NULL))
    {
        return kStatus_HAL_FlashInvalidParam;
    }

    status = SLN_FLASH_LITTLEFS_Mkdir(path);

    if (status == SLN_FLASH_FS_ENOENTRY)
    {
        LOGD("Littlefs base directory path for %s doesn't exist", path);
        ret = kStatus_HAL_FlashFileNotExist;
    }
    else if (status == SLN_FLASH_FS_FILEEXIST)
    {
        ret = kStatus_HAL_FlashDirExist;
    }
    else if (status != SLN_FLASH_FS_OK)
    {
        LOGE("Failed to create dir %s, error %d", path, status);
        ret = kStatus_HAL_FlashFail;
    }

    return ret;
}

static sln_flash_status_t _lfs_writeHandler(const flash_dev_t *dev, const char *path, void *buf, unsigned int size)
{
    int ret = kStatus_HAL_FlashSuccess;
    sln_flash_fs_status_t status;

    if ((dev == NULL) || (path == NULL) || (buf == NULL) || (size == 0))
    {
        return kStatus_HAL_FlashInvalidParam;
    }

    status = SLN_FLASH_LITTLEFS_Save(path, buf, size);

    if (status == SLN_FLASH_FS_ENOENTRY2)
    {
        LOGE("Littlefs File %s doesn't exist", path);
        ret = kStatus_HAL_FlashFileNotExist;
    }
    else if (status != SLN_FLASH_FS_OK)
    {
        LOGE("Failed to write file %s, error %d", path, status);
        ret = kStatus_HAL_FlashFail;
    }

    return ret;
}

static sln_flash_status_t _lfs_appendHandler(
    const flash_dev_t *dev, const char *path, void *buf, unsigned int size, bool overwrite)
{
    int ret = kStatus_HAL_FlashSuccess;
    sln_flash_fs_status_t status;

    if ((dev == NULL) || (path == NULL) || (buf == NULL) || (size == 0))
    {
        return kStatus_HAL_FlashInvalidParam;
    }

    if (overwrite == true)
    {
        status = SLN_FLASH_LITTLEFS_Save(path, buf, size);
    }
    else
    {
        status = SLN_FLASH_LITTLEFS_Append(path, buf, size);
    }

    if (status == SLN_FLASH_FS_ENOENTRY2)
    {
        LOGE("Littlefs File %s doesn't exist", path);
        ret = kStatus_HAL_FlashFileNotExist;
    }
    else if (status != SLN_FLASH_FS_OK)
    {
        LOGE("Failed to append to file %s, error %d", path, status);
        ret = kStatus_HAL_FlashFail;
    }

    return ret;
}

static sln_flash_status_t _lfs_readHandler(
    const flash_dev_t *dev, const char *path, void *buf, unsigned int offset, unsigned int *size)
{
    int ret = kStatus_HAL_FlashSuccess;
    sln_flash_fs_status_t status;

    if ((dev == NULL) || (path == NULL) || (size == 0))
    {
        return kStatus_HAL_FlashInvalidParam;
    }

    status = SLN_FLASH_LITTLEFS_Read(path, buf, offset, size);

    if ((status == SLN_FLASH_FS_ENOENTRY2) || (status == SLN_FLASH_FS_ENOENTRY))
    {
        LOGD("Littlefs file %s doesn't exist", path);
        ret = kStatus_HAL_FlashFileNotExist;
    }
    else if (status != SLN_FLASH_FS_OK)
    {
        LOGE("Failed to read from file %s, error %d", path, status);
        ret = kStatus_HAL_FlashFail;
    }

    return ret;
}

static sln_flash_status_t _lfs_renameHandler(const flash_dev_t *dev, const char *OldPath, const char *NewPath)
{
    int ret = kStatus_HAL_FlashSuccess;
    sln_flash_fs_status_t status;

    if ((dev == NULL) || (OldPath == NULL) || (NewPath == NULL))
    {
        return kStatus_HAL_FlashInvalidParam;
    }

    status = SLN_FLASH_LITTLEFS_Rename(OldPath, NewPath);
    if ((status == SLN_FLASH_FS_ENOENTRY2) || (status == SLN_FLASH_FS_ENOENTRY))
    {
        LOGD("Littlefs file doesn't exist");
        ret = kStatus_HAL_FlashFileNotExist;
    }
    else if (status != SLN_FLASH_FS_OK)
    {
        LOGE("Failed to rename file %s, error %d", OldPath, status);
        ret = kStatus_HAL_FlashFail;
    }
    return ret;
}

const static flash_dev_operator_t s_FlashDev_LittlefsOps = {
    .init    = _lfs_init,
    .deinit  = NULL,
    .format  = _lfs_formatHandler,
    .save    = _lfs_writeHandler,
    .append  = _lfs_appendHandler,
    .read    = _lfs_readHandler,
    .mkdir   = _lfs_mkdirHandler,
    .mkfile  = _lfs_mkfileHandler,
    .rm      = _lfs_rmHandler,
    .rename  = _lfs_renameHandler,
    .cleanup = _lfs_cleanupHandler,
};

static flash_dev_t s_FlashDev_Littlefs = {
    .id  = 0,
    .ops = &s_FlashDev_LittlefsOps,
};

int HAL_FlashDev_Littlefs_Register()
{
    int error = 0;
    LOGD("++HAL_FlashDev_Littlefs_Init");
    _lfs_init();

    LOGD("--HAL_FlashDev_Littlefs_Init");
    error = FWK_Flash_DeviceRegister(&s_FlashDev_Littlefs);

    FWK_LpmManager_RegisterRequestHandler(&s_LpmReq);
    return error;
}
#endif /* ENABLE_FLASH_DEV_Littlefs */
