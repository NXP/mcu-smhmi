# Flash manager

The Flash manager is used to provide an abstraction for an underlying filesystem implementation.

Due to the unique nature of the filesystem being an abstract "virtual" device,
only one flash device can be registered at a time.
However,
generally there should be no need to have more than one filesystem.
It means the Flash manager's API functions essentially act as wrappers that call the `Operators` of the underlying flash HAL device.

```{warning}
Flash access is exclusive, one request at a time.
```

```{note}
When working with the Flash Manager,
unlike most other managers,
`FWK_Flash_DeviceRegister` should be called _before_ `FWK_Flash_Init`.
```

## Device APIs

### FWK_Flash_DeviceRegister

```c
/**
 * @brief Only one flash device is supported. Registered a flash filesystem device
 * @param dev Pointer to a flash device structure
 * @return int Return 0 if registration was successful
 */
int FWK_Flash_DeviceRegister(const flash_dev_t *dev);
```

Unlike the flow for most other managers,
this function must be called _before_ `FWK_Flash_Init`.

### FWK_Flash_Init

```c
/**
 * @brief Init internal structures for flash.
 * @return int Return 0 if the init process was successful
 */
sln_flash_status_t FWK_Flash_Init();
```

### FWK_Flash_Deinit

```c
/**
 * @brief Deinit internal structures for flash.
 * @return int Return 0 if the init process was successful
 */
sln_flash_status_t FWK_Flash_Deinit();
```

## Operations APIs

The Flash Manager and underlying flash HAL device define only a few operations in order to keep the API simple and easy to implement.
These API functions include:

- Format
- Save
- Delete
- Read
- Make Directory
- Make File
- Append
- Rename
- Cleanup

While it might limit filesystem functionality,
it also helps to keep the code readable, portable, and maintainable.

```{note}
If the default list of APIs does not satisfy the requirements of a use-case,
the API can always be extended or bypassed in the code directly.
```

### FWK_Flash_Format

```c
/**
 * @brief Format the filesystem
 * @return the status of formatting operation
 */
sln_flash_status_t FWK_Flash_Format();
```

### FWK_Flash_Save

```c
/**
 * @brief Save the data into a file from the file system
 * @param path Path of the file in the file system
 * @param buf  Buffer which contains the data that is going to be saved
 * @param size Size of the buffer
 * @return the status of save operation
 */
sln_flash_status_t FWK_Flash_Save(const char *path, void *buf, unsigned int size);
```

### FWK_Flash_Append

```c
/**
 * @brief Append the data to an existing file.
 * @param path Path of the file in the file system
 * @param buf  Buffer which contains the data that is going to be append
 * @param size Size of the buffer
 * @param overwrite Boolean parameter. If true the existing file will be truncated. Similar to SLN_flash_save
 * @return the status of append operation
 */
 sln_flash_status_t FWK_Flash_Append(const char *path, void *buf, unsigned int size, bool overwrite);
```

### FWK_Flash_Read

```c
/**
 * @brief Read from a file
 * @param path Path of the file in the file system
 * @param buf  Buffer in which to store the read value
 * @param offset If reading in chunks, set offset to file current position
 * @param size Size that was read.
 * @return the status of read operation
 */
sln_flash_status_t FWK_Flash_Read(const char *path, void *buf, unsigned int offset, unsigned int *size);

```

### FWK_Flash_Mkdir

```c
/**
 * @brief Make directory operation
 * @param path Path of the directory in the file system
 * @return the status of mkdir operation
 */
sln_flash_status_t FWK_Flash_Mkdir(const char *path);
```

### FWK_Flash_Mkfile

```c
/**
 * @brief Make file with specific attributes
 * @param path Path of the file in the file system
 * @param encrypt Specify if the files should be encrypted. Based on FS implementation
 * this param can be neglected
 * @return the status of mkfile operation
 */
sln_flash_status_t FWK_Flash_Mkfile(const char *path, bool encrypt);
```

### FWK_Flash_Rm

```c
/**
 * @brief Remove file
 * @param path Path of the file that shall be removed
 * @return the status of rm operation
 */
sln_flash_status_t FWK_Flash_Rm(const char *path);
```

### FWK_Flash_Rename

```c
/**
 * @brief Rename existing file
 * @param OldPath Path of the file that is renamed
 * @param NewPath New Path of the file
 * @return status of rename operation
 */
sln_flash_status_t FWK_Flash_Rename(const char *oldPath, const char *newPath);
```

### FWK_Flash_Cleanup

```c
/**
 * @brief Cleanup function. Might imply defragmentation, erased unused sectors etc.
 *
 * @param timeout Time consuming operation. Set a time constrain to be sure that is not disturbing the system.
 *               Timeout = 0 means no timeout
 * @return status of cleanup operation
 */
sln_flash_status_t FWK_Flash_Cleanup(uint32_t timeout);
```
