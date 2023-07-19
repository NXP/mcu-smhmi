# Flash Devices

The flash HAL device represents an abstraction used to implement a device which handles all operations dealing with flash **[1]** (permanent) storage.

```{note}

**[1]** Even though the word "flash" is used in the terminology of this device,
the user is technically capable of implementing a FS which uses a volatile memory instead.
One potential reason for doing so would be to run logic/sanity checks on the filesystem API's before implementing them on a flash device.

Ultimately, the flash HAL device is useful for abstracting not only flash operations, but memory operations in general.
```

The flash HAL device is primarily used as a wrapper over an underlying filesystem,
be it LittleFS, FatFS, etc.
As a result,
the Flash Manager (refer to "device_managers/flash_manager.md") only allows one flash device to be registered because there is usually no need for multiple file systems operating at the same time.

---
**INFO**

Because only one flash device can be registered at a time,
this means that API calls to the Flash Manager (refer to "device_managers/flash_manager.md") essentially act as wrappers over the flash HAL device's operators.

---

In terms of functionality,
the flash HAL device provides:

- Read/Write operations
- Cleanup methods to handle defragmentation and/or emptying flash sectors during idle time
- Information about underlying flash mapping and flash type

## Device Definition

The HAL device definition for flash devices can be found under "framework/hal_api/hal_flash_dev.h" and is reproduced below:

```c title="framework/hal_api/hal_flash_dev.h"
/*! @brief Attributes of a flash device */
struct _flash_dev
{
    /* unique id */
    int id;
    /* operations */
    const flash_dev_operator_t *ops;
};
```

The device `operators` associated with flash HAL devices are as shown below:

```c
/*! @brief Callback function to timeout check requester list busy status. */
typedef int (*lpm_manager_timer_callback_t)(lpm_dev_t *dev);

/*! @brief Operation that needs to be implemented by a flash device */
typedef struct _flash_dev_operator
{
    sln_flash_status_t (*init)(const flash_dev_t *dev);
    sln_flash_status_t (*deinit)(const flash_dev_t *dev);
    sln_flash_status_t (*format)(const flash_dev_t *dev);
    sln_flash_status_t (*save)(const flash_dev_t *dev, const char *path, void *buf, unsigned int size);
    sln_flash_status_t (*append)(const flash_dev_t *dev, const char *path, void *buf, unsigned int size, bool overwrite);
    sln_flash_status_t (*read)(const flash_dev_t *dev, const char *path, void *buf, unsigned int offset, unsigned int *size);
    sln_flash_status_t (*mkdir)(const flash_dev_t *dev, const char *path);
    sln_flash_status_t (*mkfile)(const flash_dev_t *dev, const char *path, bool encrypt);
    sln_flash_status_t (*rm)(const flash_dev_t *dev, const char *path);
    sln_flash_status_t (*rename)(const flash_dev_t *dev, const char *oldPath, const char *newPath);
    sln_flash_status_t (*cleanup)(const flash_dev_t *dev, unsigned int timeout_ms);
} flash_dev_operator_t;

```

## Operators

Operators are functions which "operate" on a HAL device itself.
Operators are akin to "public methods" in object oriented-languages.

For more information about operators, see `Operators`.

### Init

```c
sln_flash_status_t (*init)(const flash_dev_t *dev);
```

Initialize the flash & filesystem.

`Init` should initialize any hardware resources required by the flash device (pins, ports, clock, etc). **[2]**
In addition to initializing the hardware,
the `init` function should also mount the filesystem. **[3]**

```{warning}

**[2]** An application that runs from flash (does XiP) should not initialize/deinitialize any hardware.
If a hardware change is truly needed,
the change should be performed with caution.
```

```{note}

**[3]** Some lightweight FS may not require mounting and can be prebuilt/preloaded on the flash instead.
Regardless,
the `init` function should result in the filesystem being in a usable state.

```

### Deinit

```c
hal_lpm_status_t (*deinit)(const lpm_dev_t *dev);
```

"Deinitialize" the flash & filesystem.

`DeInit` should release any hardware resources a flash device might use (I/O ports, IRQs, etc.), turn off the hardware, and perform any other shutdown the device requires. **[2]**

### Format

```c
sln_flash_status_t (*format)(const flash_dev_t *dev);
```

Clean and format the filesystem.

### Save

```c
sln_flash_status_t (*save)(const flash_dev_t *dev, const char *path, void *buf, unsigned int size);
```

Save a file with the contents of `buf` to `path` in the filesystem.

### Append

```c
sln_flash_status_t (*append)(const flash_dev_t *dev, const char *path, void *buf, unsigned int size, bool overwrite);
```

Append the contents of `buf` to an existing file located at `path`.

Setting `overwrite` equal to true will cause append from the beginning of the file instead. **[4]**

```{note}

**[4]** `overwrite == true` makes this function nearly equivalent to the save function,
the only difference being that this will not create a new file.

```

### Read

```c
sln_flash_status_t (*read)(const flash_dev_t *dev, const char *path, void *buf, unsigned int offset, unsigned int *size);
```

Read a file from the filesystem located at `path` and store the contents in `buf`. **[5]**

In order to find the needed space for the buf, call read with `buf` set to NULL.
In case there is not enough space in memory to read the whole file,
read with offset can be use while specifying the chunk size.

```{note}

**[5]** It is up to the user to guarantee that the buffer supplied will fit the contents of the file being read.

```

### Make Directory

```c
sln_flash_status_t (*mkdir)(const flash_dev_t *dev, const char *path);
```

Create a directory located at `path`.

```{note}

If the filesystem in use does not support directories,
this operator can be set to `NULL`.

```

### Make File
```c
sln_flash_status_t (*mkfile)(const flash_dev_t *dev, const char *path, bool encrypt);
```

Creates the file mentioned by the path. If the information needs to stored not in plain text, encryption can be enabled.

### Remove

```c
sln_flash_status_t (*rm)(const flash_dev_t *dev, const char *path);
```

Remove the file located at `path`.

```{note}

If the filesystem in use does not support directories,
this operator can be set to `NULL`.

```

### Rename

```c
sln_flash_status_t (*rename)(const flash_dev_t *dev, const char *oldPath, const char *newPath);
```

Rename/move a file from `oldPath` to `newPath`.

### Cleanup

```c
sln_flash_status_t (*cleanup)(const flash_dev_t *dev, unsigned int timeout_ms);
```

Clean up the filesystem.

This function is used to help minimize delays introduced by things like fragmentation caused during "erase sector" operations
which can lead to unwanted delays when searching for the next available sector.

`timeout_ms` specifies how much time to wait while performing cleanup.
This helps prevent against multiple HAL devices calling `cleanup` and stalling the filesystem.

## Example

Because only one flash device can be registered at a time per the design of the framework,
the project has only one filesystem implemented.

The source file for this flash HAL device can be found at "framework/hal/misc/hal_flash_littlefs.c".

In this example,
we will demonstrate a way to integrate the well known [Littlefs](https://github.com/littlefs-project/littlefs) in our framework.

Littlefs is a lightweight file-system that is designed to handle random power failures.
The architecture of the file-system allows having directories and files.
As a result, this example uses the following file layout:

```bash title="Littlefs file layout"
root-directory
├── cfg
│   ├── Metadata
│   ├── fwk_cfg - stores framework related information.
│   └── app_cfg - stores app specific information.
├── oasis
│   ├── Metadata
│   └── faceFiles - the number of files that stores faces are up to 100
├── app_specific
└── wifi_info
    └── wifi_info
```

### Littlefs Device

```c title="framework/hal/misc/hal_flash_littlefs.c"
static sln_flash_status_t _lfs_init()
{
    int res = kStatus_HAL_FlashSuccess;
    if (s_LittlefsHandler.lfsMounted)
    {
        return kStatus_HAL_FlashSuccess;
    }
    s_LittlefsHandler.lock = xSemaphoreCreateMutex();
    if (s_LittlefsHandler.lock == NULL)
    {
        LOGE("Littlefs create lock failed");
        return kStatus_HAL_FlashFail;
    }

    _lfs_get_default_config(&s_LittlefsHandler.cfg);
#if DEBUG
    BOARD_InitFlashResources();
#endif
    SLN_Flash_Init();
    if (res)
    {
        LOGE("Littlefs storage init failed: %i", res);
        return kStatus_HAL_FlashFail;
    }

    res = lfs_mount(&s_LittlefsHandler.lfs, &s_LittlefsHandler.cfg);
    if (res == 0)
    {
        s_LittlefsHandler.lfsMounted = 1;
        LOGD("Littlefs mount success");
    }
    else if (res == LFS_ERR_CORRUPT)
    {
        LOGE("Littlefs corrupt");
        lfs_format(&s_LittlefsHandler.lfs, &s_LittlefsHandler.cfg);
        LOGD("Littlefs attempting to mount after reformatting...");
        res = lfs_mount(&s_LittlefsHandler.lfs, &s_LittlefsHandler.cfg);
        if (res == 0)
        {
            s_LittlefsHandler.lfsMounted = 1;
            LOGD("Littlefs mount success");
        }
        else
        {
            LOGE("Littlefs mount failed again");
            return kStatus_HAL_FlashFail;
        }
    }
    else
    {
        LOGE("Littlefs error while mounting");
    }

    return res;
}

static sln_flash_status_t _lfs_cleanupHandler(const flash_dev_t *dev,
                                                             unsigned int timeout_ms)
{
    sln_flash_status_t status              = kStatus_HAL_FlashSuccess;
    uint32_t usedBlocks[LFS_SECTORS/32]    = {0};
    uint32_t emptyBlocks                   = 0;
    uint32_t startTime                     = 0;
    uint32_t currentTime                   = 0;

    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }

    /* create used block list */
    lfs_fs_traverse(&s_LittlefsHandler.lfs, _lfs_traverse_create_used_blocks,
                                 &usedBlocks);

    startTime = sln_current_time_us();

    /* find next block starting from free.i */
    for (int i = 0; i < LFS_SECTORS; i++)
    {
        currentTime = sln_current_time_us();
        /* Check timeout */
        if ((timeout_ms) && (currentTime >= (startTime + timeout_ms * 1000)))
        {
            break;
        }

        lfs_block_t block = (s_LittlefsHandler.lfs.free.i + i) % LFS_SECTORS;

        /* take next unused marked block */
        if (!_is_blockBitSet(usedBlocks, block))
        {
            /* If the block is marked as free but not yet erased, try to erase it */
            LOGD("Block %i is unused, try to erase it", block);
            _lfs_qspiflash_erase(&s_LittlefsConfigDefault, block);
            emptyBlocks += 1;
        }
    }

    LOGI("%i empty_blocks starting from %i available in %ims",
                 emptyBlocks, s_LittlefsHandler.lfs.free.i, (sln_current_time_us() - startTime)/1000);

    _unlock();
    return status;
}

static sln_flash_status_t _lfs_formatHandler(const flash_dev_t *dev)
{
    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }
    lfs_format(&s_LittlefsHandler.lfs, &s_LittlefsHandler.cfg);
    _unlock();
    return kStatus_HAL_FlashSuccess;
}

static sln_flash_status_t _lfs_rmHandler(const flash_dev_t *dev, const char *path)
{
    int res;

    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }

    res = lfs_remove(&s_LittlefsHandler.lfs, path);
    if (res)
    {
        LOGE("Littlefs while removing: %i", res);
        _unlock();
        if (res == LFS_ERR_NOENT)
        {
            return kStatus_HAL_FlashFileNotExist;
        }

        return kStatus_HAL_FlashFail;
    }
    _unlock();
    return kStatus_HAL_FlashSuccess;
}

static sln_flash_status_t _lfs_mkdirHandler(const flash_dev_t *dev, const char *path)
{
    int res;

    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }

    res = lfs_mkdir(&s_LittlefsHandler.lfs, path);

    if (res == LFS_ERR_EXIST)
    {
        LOGD("Littlefs directory exists: %i", res);
        _unlock();
        return kStatus_HAL_FlashDirExist;
    }
    else if (res)
    {
        LOGE("Littlefs creating directory: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }
    _unlock();
    return kStatus_HAL_FlashSuccess;
}

static sln_flash_status_t _lfs_writeHandler(const flash_dev_t *dev, const char *path, void *buf, unsigned int size)
{
    int res;
    lfs_file_t file;

    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }

    res = lfs_file_opencfg(&s_LittlefsHandler.lfs, &file, path, LFS_O_CREAT, &s_FileDefault);
    if (res)
    {
        LOGE("Littlefs opening file: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }

    res = lfs_file_write(&s_LittlefsHandler.lfs, &file, buf, size);
    if (res < 0)
    {
        LOGE("Littlefs writing file: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }

    res = lfs_file_close(&s_LittlefsHandler.lfs, &file);
    if (res)
    {
        LOGE("Littlefs closing file: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }

    _unlock();
    return kStatus_HAL_FlashSuccess;
}

static sln_flash_status_t _lfs_appendHandler(const flash_dev_t *dev,
                                                            const char *path,
                                                            void *buf,
                                                            unsigned int size,
                                                            bool overwrite)
{
    int res;
    lfs_file_t file;

    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }

    res = lfs_file_opencfg(&s_LittlefsHandler.lfs, &file, path, LFS_O_APPEND, &s_FileDefault);
    if (res)
    {
        LOGE("Littlefs opening file: %i", res);
        _unlock();
        if (res == LFS_ERR_NOENT)
        {
            return kStatus_HAL_FlashFileNotExist;
        }
        return kStatus_HAL_FlashFail;
    }

    if (overwrite == true)
    {
        res = lfs_file_truncate(&s_LittlefsHandler.lfs, &file, 0);

        if (res < 0)
        {
            LOGE("Littlefs truncate file: %i", res);
            _unlock();
            return kStatus_HAL_FlashFail;
        }
    }

    res = lfs_file_write(&s_LittlefsHandler.lfs, &file, buf, size);
    if (res < 0)
    {
        LOGE("Littlefs writing file: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }

    res = lfs_file_close(&s_LittlefsHandler.lfs, &file);
    if (res)
    {
        LOGE("Littlefs closing file: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }

    _unlock();
    return kStatus_HAL_FlashSuccess;
}

static sln_flash_status_t _lfs_readHandler(const flash_dev_t *dev, const char *path, void *buf, unsigned int size)
{
    int res;
    int offset = 0;
    lfs_file_t file;

    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }
    res = lfs_file_opencfg(&s_LittlefsHandler.lfs, &file, path, LFS_O_RDONLY, &s_FileDefault);
    if (res)
    {
        LOGE("Littlefs opening file: %i", res);
        _unlock();
        if (res == LFS_ERR_NOENT)
        {
            return kStatus_HAL_FlashFileNotExist;
        }
        return kStatus_HAL_FlashFail;
    }

    do
    {
        res = lfs_file_read(&s_LittlefsHandler.lfs, &file, (buf + offset), size);
        if (res < 0)
        {
            LOGE("Littlefs reading file: %i", res);
            _unlock();
            return kStatus_HAL_FlashFail;
        }
        else if (res == 0)
        {
            LOGD("Littlefs reading file \"%s\": Read only %d. %d bytes not found ", path, offset, size);
            break;
        }

        offset += res;
        size -= res;
    } while (size > 0);

    res = lfs_file_close(&s_LittlefsHandler.lfs, &file);
    if (res)
    {
        LOGE("Littlefs closing file: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }

    _unlock();
    return kStatus_HAL_FlashSuccess;
}

static sln_flash_status_t _lfs_renameHandler(const flash_dev_t *dev, const char *OldPath, const char *NewPath)
{
    int res;

    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }

    res = lfs_rename(&s_LittlefsHandler.lfs, OldPath, NewPath);
    if (res)
    {
        LOGE("Littlefs renaming file: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }
    _unlock();
    return kStatus_HAL_FlashSuccess;
}

const static flash_dev_operator_t s_FlashDev_LittlefsOps = {
    .init   = _lfs_init,
    .deinit = NULL,
    .format = _lfs_formatHandler,
    .append = _lfs_appendHandler,
    .save   = _lfs_writeHandler,
    .read   = _lfs_readHandler,
    .mkdir  = _lfs_mkdirHandler,
    .rm     = _lfs_rmHandler,
    .rename = _lfs_renameHandler,
    .cleanup= _lfs_cleanupHandler,
};

static flash_dev_t s_FlashDev_Littlefs = {
    .id  = 0,
    .ops = &s_FlashDev_LittlefsOps,
};

int HAL_FlashDev_Littlefs_Init()
{
    int error = 0;
    LOGD("++HAL_FlashDev_Littlefs_Init");
    _lfs_init();

    LOGD("--HAL_FlashDev_Littlefs_Init");
    error = FWK_Flash_DeviceRegister(&s_FlashDev_Littlefs);

    FWK_LpmManager_RegisterRequestHandler(&s_LpmReq);
    return error;
}
```

```{note}
What was presented here shows only the operators described above.
For more information regarding Littlefs configuration,
FlexSPI configuration, optimization done.
Don't forget to check the full code base.
```
