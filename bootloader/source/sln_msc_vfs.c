/*
 * Copyright 2019, 2021-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include "fsl_common.h"
#include "sln_msc_vfs.h"
#include "flash_ica_driver.h"
#include "sln_update.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _file_name
{
    const char *file_name;
    fica_img_type_t image_type;
} file_name_t;

#define MSD_FILE_MINIMUM_SIZE 4096
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t s_startOffset = 0;
static uint32_t s_fileLength  = 0;
static uint32_t s_dataWritten = 0;
static uint32_t s_lbaLength   = 0;

static const file_name_t s_fileListName[] = {
#if FICA_RESOURCES_ENABLE
    {"RES", FICA_IMG_TYPE_RESOURCES_A},
#endif /* FICA_RESOURCES_ENABLE */

#if FICA_BUNDLE_ENABLE
    {"BUNDLE", FICA_IMG_TYPE_BUNDLE}
#endif /* FICA_RESOURCES_BUNDLE */
};

static int32_t s_imgType = FICA_IMG_TYPE_NONE;

static msc_vfs_state_t s_transferState = TRANSFER_IDLE;

static TaskHandle_t *s_usbAppTaskHandle = NULL;

const char ext[] = "BIN";

static const fat_mbr_t s_fatMbrInit = {.jump_instr              = {0xEB, 0x3C, 0x90},
                                       .oem_name                = {'M', 'S', 'D', '0', 'S', '5', '.', '0'},
                                       .bytes_per_sector        = 0x0200,
                                       .sectors_per_cluster     = 0x10,
                                       .reserved_sectors        = 0x0008,
                                       .num_fats                = 0x02,
                                       .root_entries            = 0x0200,
                                       .num_logical_sectors     = FICA_IMG_BANK_SIZE / 0x0200,
                                       .media_type              = 0xf0,
                                       .logical_sectors_per_fat = 0x0008,
                                       .sectors_per_track       = 0x003F,
                                       .num_heads               = 0x00FF,
                                       .hidden_sectors          = 0x00000000,
                                       .large_sectors           = 0x00000000,
                                       .physical_drive_number   = 0x08,
                                       .not_used                = 0x00,
                                       .boot_record_signature   = 0x29,
                                       .volume_sn               = 0x3FF6BD53,
                                       .volume_label = {'S', 'L', 'N', '-', 'B', 'O', 'O', 'T', 0x00, 0x00, 0x00},
                                       .fat_type     = {'F', 'A', 'T', '1', '6', ' ', ' ', ' '},
                                       .bootstrap    = {0x00},
                                       .signature    = 0xAA55};

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t MSC_VFS_Init(uint8_t *storageDisk, TaskHandle_t *usbAppTask, uint32_t lbaLength)
{
    status_t status = kStatus_Fail;

    if ((NULL != storageDisk) && (NULL != usbAppTask) && (0 != lbaLength))
    {
        status = kStatus_Success;

        memcpy(&storageDisk[0], &s_fatMbrInit, sizeof(fat_mbr_t));

        s_usbAppTaskHandle = usbAppTask;

        s_lbaLength = lbaLength;

        s_fileLength    = 0;
        s_dataWritten   = 0;
        s_transferState = TRANSFER_IDLE;
    }

    return status;
}

int32_t MSC_VFS_GetImgType(void)
{
    return s_imgType;
}

msc_vfs_state_t MSC_VFS_GetTransferState(void)
{
    return s_transferState;
}

void MSC_VFS_SetTransferState(msc_vfs_state_t transferState)
{
    s_transferState = transferState;
}

status_t MSC_VFS_WriteResponse(uint32_t offset, uint32_t size, uint8_t *buffer)
{
    int32_t flashError = SLN_FLASH_NO_ERROR;
    status_t error     = kStatus_Success;

    if (0 == size)
    {
        configPRINTF(("[Write Response] Empty write response!\r\n"));
        return error;
    }

    if (TRANSFER_IDLE == s_transferState)
    {
        static char fileName[12] = {0};
        static bool fileFound    = false;
        fat_file_t *file         = NULL;
        s_imgType                = FICA_IMG_TYPE_NONE;

        if (!fileFound)
        {
            /* Identify that it is a bin file  */
            for (uint32_t idx = 0; idx < size; idx++)
            {
                if (0 == memcmp(&buffer[idx], ext, sizeof(ext) - 1))
                {
                    uint8_t fileMaxName = sizeof(((fat_file_t *)0)->name);
                    file                = (fat_file_t *)&buffer[idx - fileMaxName + sizeof(ext) - 1];

                    break;
                }
            }

            if (file->size != 0)
            {
                if (file->size < MSD_FILE_MINIMUM_SIZE)
                {
                    configPRINTF(("[Error] File size %d B is too small for minimal admitted %d B\r\n", file->size,
                                  MSD_FILE_MINIMUM_SIZE));

                    s_transferState = TRANSFER_ERROR;
                    error           = kStatus_Fail;
                }
                else
                {
                    memcpy(fileName, file->name, 11);
                    configPRINTF(("[Write Response] File Attributes: Name - %s, Size - %d\r\n", fileName, file->size));
                    s_fileLength = file->size;
                    fileFound    = true;
                }
            }
        }
        else
        {
            uint32_t numberOfFiles = sizeof(s_fileListName) / sizeof(file_name_t);

            for (uint32_t idx = 0; idx < numberOfFiles; idx++)
            {
                uint32_t len = strlen(s_fileListName[idx].file_name);
                if (0 == memcmp(fileName, s_fileListName[idx].file_name, len))
                {
                    configPRINTF(("[Write Response] File in the list of known files: %s\r\n", fileName));
                    s_imgType       = s_fileListName[idx].image_type;
                    s_transferState = TRANSFER_START;
                    break;
                }
            }

            /*
             * Check to see if it is a Flash image
             * Determine the base programming address from the passed image reset vector
             */
            if (s_imgType == FICA_IMG_TYPE_NONE)
            {
                uint32_t *resetHandler = (uint32_t *)(&buffer[4]);
                bool isFlash           = ((FLASH_BYTE4_UPPER_NIBBLE & *resetHandler) == FLEXSPI_AMBA_BASE);
                bool isValid           = ((FLASH_BYTE3 & *resetHandler) == FICA_IMG_APP_A_ADDR) ||
                               ((FLASH_BYTE3 & *resetHandler) == FICA_IMG_APP_B_ADDR);

                if (isFlash && isValid)
                {
                    uint32_t imgBaseAddr = (uint32_t)(*resetHandler & FLASH_BYTE3);

                    s_transferState = TRANSFER_START;

                    configPRINTF(("[Write Response] Reset Handler: 0x%X\r\n", *resetHandler));

                    FICA_GetImgTypeFromAddr(imgBaseAddr, (int32_t *)&s_imgType);

                    if ((FICA_IMG_TYPE_NONE >= s_imgType) || (FICA_NUM_IMG_TYPES <= s_imgType))
                    {
                        error = kStatus_Fail;
                    }
                }
                else
                {
                    configPRINTF(("[Write Response] Wrong image received."));
                    s_transferState = TRANSFER_ERROR;
                    vTaskResume(*s_usbAppTaskHandle);
                }
            }
        }

        /* Found and img type prepare for update */
        if (s_transferState == TRANSFER_START)
        {
            error = SLN_Update_PrepareImgBank(s_imgType, s_fileLength);

            if (kStatus_Success == error)
            {
                // Rest of offsets coming in will be relative to the startoffset
                fileFound = false;
                memset(fileName, 0, sizeof(fileName));
                s_startOffset   = offset;
                s_transferState = TRANSFER_ACTIVE;
            }
        }

        if (kStatus_Success != error)
        {
            fileFound = false;
            memset(fileName, 0, sizeof(fileName));
            configPRINTF(("[Write Response] Unable to begin transfer of file!\r\n"));
            s_transferState = TRANSFER_ERROR;

            // Wake up application task to handle this error
            vTaskResume(*s_usbAppTaskHandle);
        }
    }

    if (TRANSFER_ACTIVE == s_transferState)
    {
        if (offset >= s_startOffset)
        {
            uint32_t imgOffset = ((offset - s_startOffset) * s_lbaLength);

            /* Usb adds some padding */
            size = ((s_dataWritten + size) > s_fileLength) ? (s_fileLength - s_dataWritten) : size;

            error = SLN_Update_WriteImg(s_imgType, imgOffset, buffer, size);

            if (kStatus_Success == error)
            {
                configPRINTF(("[Write Response] Saving %d of data to 0x%X...\r\n", size, imgOffset));
                s_dataWritten += size;
            }
            else
            {
                s_transferState = TRANSFER_ERROR;
                configPRINTF(("[Write Response] ...save failed!!!\r\n", size, imgOffset));

                // Wake up application task to handle this error
                vTaskResume(*s_usbAppTaskHandle);
            }
        }

        if ((s_fileLength > 0) && (s_dataWritten >= s_fileLength))
        {
            s_transferState = TRANSFER_PENDING;
            // Wake up the application task to finalize transfer
            vTaskResume(*s_usbAppTaskHandle);
        }
    }

    return error;
}
