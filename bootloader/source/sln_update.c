/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "sln_update.h"
#include "flash_ica_driver.h"
#include "sln_flash.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"

#include "sln_auth.h"

#define REVERSE_COPY     -1
#define SHA256_HASH_SIZE 32

static fica_img_type_t s_preparedImgType = FICA_IMG_TYPE_NONE;
static uint32_t s_fileSize               = 0;

/**
 * @brief This function will populate img information from the FICA entry.
 *
 * @param newImg Img type
 * @param newImgSize Size of the new img.
 * @param newImgStartAddr Starting address of the new img
 * @param newImgSig The signature of the new img
 * @return
 */
static status_t SLN_Update_WriteModuleInfo(fica_img_type_t newImg,
                                           uint32_t newImgSize,
                                           uint32_t newImgStartAddr,
                                           uint8_t *newImgSig)
{
    int32_t flashError = SLN_FLASH_NO_ERROR;
    status_t status    = kStatus_Success;

    if (newImgSig != NULL)
    {
        flashError = FICA_write_image_record(newImg, newImgStartAddr, newImgSize, newImgSig);
    }

    if (SLN_FLASH_NO_ERROR != flashError)
    {
        status = kStatus_Fail;
    }

    return status;
}

/**
 * @brief Moves Flash image. The image destination addr must be FLASH_SECTOR_SIZE aligned
 * and the gap between source and destination larger than FLASH_SECTOR_SIZE
 *
 * @param oldImgStartAddr the address from which to copy the img
 * @param newImgStartAddr the address where to copy the img
 * @param imgSize size of the image
 * @return kStatus_InvalidArgument if the address are too close or target address is not aligned.
 * 		   kStatus_Fail if the write operation failed.
 *
 */
static status_t SLN_Update_MoveModuleData(uint32_t oldImgStartAddr, uint32_t newImgStartAddr, uint32_t imgSize)
{
    status_t status       = kStatus_Success;
    int8_t direction      = 1;
    uint32_t offset       = 0;
    uint8_t *tmpImgBuffer = NULL;
    bool sectionOverlap   = true;
    uint8_t *oldHash      = NULL;
    uint8_t *newHash      = NULL;

    if ((newImgStartAddr % FLASH_SECTOR_SIZE) != 0)
    {
        status = kStatus_InvalidArgument;
    }
    else
    {
        int diff = oldImgStartAddr - newImgStartAddr;

        if (diff < 0)
        {
            diff = -diff;
        }

        if (diff < FLASH_SECTOR_SIZE)
        {
            /* we need to have a sector difference between source and destination */
            status = kStatus_InvalidArgument;
        }
        else if (diff > imgSize)
        {
            sectionOverlap = false;
        }
    }

    if (kStatus_Success == status)
    {
        if (sectionOverlap == false)
        {
            configPRINTF(("[SLN_UPDATE] Sections are not overlaping. Do fast erase.\r\n"));
            if (FICA_Erase_Bank(newImgStartAddr, imgSize) != SLN_FLASH_NO_ERROR)
            {
                status = kStatus_Fail;
            }
        }
        else
        {
            /* Section overlap, do reverse copy */
            if (oldImgStartAddr < newImgStartAddr)
            {
                direction = REVERSE_COPY;

                if (imgSize > FLASH_SECTOR_SIZE)
                {
                    offset = imgSize - ((imgSize + newImgStartAddr) % FLASH_SECTOR_SIZE);
                    if (imgSize == offset)
                    {
                        offset -= FLASH_SECTOR_SIZE;
                    }
                }
            }
        }
    }

    tmpImgBuffer = (uint8_t *)pvPortMalloc(FLASH_SECTOR_SIZE);
    oldHash      = (uint8_t *)pvPortMalloc(SHA256_HASH_SIZE);
    newHash      = (uint8_t *)pvPortMalloc(SHA256_HASH_SIZE);

    if ((tmpImgBuffer == NULL) || (oldHash == NULL) || (newHash == NULL))
    {
        status = kStatus_Fail;
    }

    if (kStatus_Success == status)
    {
        /* Do img hash before the copy */
        sln_auth_status_t hash_status = SLN_AUTH_OK;

        hash_status =
            SLN_AUTH_Generate_Hash((uint8_t *)oldImgStartAddr, imgSize, oldHash, SHA256_HASH_SIZE, MBEDTLS_MD_SHA256);
        if (SLN_AUTH_OK != hash_status)
        {
            status = kStatus_Fail;
        }
    }

    if (kStatus_Success == status)
    {
        uint32_t tmpImgSize = imgSize;
        uint32_t sumSize    = 0;

        while (tmpImgSize)
        {
            uint32_t to_read = 0;

            if ((direction == REVERSE_COPY) && (((tmpImgSize + newImgStartAddr) % FLASH_SECTOR_SIZE) != 0))
            {
                /* When doing reverse copy, must align the image first */
                to_read = (tmpImgSize + newImgStartAddr) % FLASH_SECTOR_SIZE;
            }
            else
            {
                to_read = tmpImgSize > FLASH_SECTOR_SIZE ? FLASH_SECTOR_SIZE : tmpImgSize;
            }

            status |= SLN_Read_Flash_At_Address((oldImgStartAddr + offset), tmpImgBuffer, to_read);

            if (sectionOverlap)
            {
                status |= SLN_Erase_Sector(newImgStartAddr + offset);
            }

            if (kStatus_Success == status)
            {
                uint32_t to_write = to_read;
                uint32_t written  = 0;

                while (to_write)
                {
                    uint32_t current_chunk = to_write > FLASH_PAGE_SIZE ? FLASH_PAGE_SIZE : to_write;
                    status                 = SLN_Write_Flash_At_Address(newImgStartAddr + offset + written,
                                                        (uint8_t *)(tmpImgBuffer + written));
                    if (kStatus_Success != status)
                    {
                        break;
                    }
                    to_write -= current_chunk;
                    written += current_chunk;
                }
            }

            if (kStatus_Success == status)
            {
                tmpImgSize -= to_read;

                if ((sumSize >= FLASH_BLOCK_SIZE) || (tmpImgSize == 0))
                {
                    uint32_t source      = oldImgStartAddr + offset - direction * sumSize;
                    uint32_t destination = newImgStartAddr + offset - direction * sumSize;

                    if (tmpImgSize == 0)
                    {
                        sumSize += to_read;
                    }

                    configPRINTF(("[SLN_UPDATE] Moved from %p to address %p a chunk of size %d \r\n", source,
                                  destination, sumSize));
                    sumSize = to_read;
                }
                else
                {
                    sumSize += to_read;
                }

                offset += direction * FLASH_SECTOR_SIZE;
            }
            else
            {
                break;
            }
        }
    }

    if (kStatus_Success == status)
    {
        /* Do img hash after the copy */
        sln_auth_status_t hash_status = SLN_AUTH_OK;

        hash_status =
            SLN_AUTH_Generate_Hash((uint8_t *)newImgStartAddr, imgSize, newHash, SHA256_HASH_SIZE, MBEDTLS_MD_SHA256);

        if (SLN_AUTH_OK == hash_status)
        {
            if (memcmp(newHash, oldHash, SHA256_HASH_SIZE))
            {
                configPRINTF(("[SLN_UPDATE] New hash doesn't match old hash. Error copy \r\n"));
                status = kStatus_Fail;
            }
            else
            {
                configPRINTF(("[SLN_UPDATE] Images hash match. The image was moved correctly\r\n"));
            }
        }
        else
        {
            status = kStatus_Fail;
        }
    }

    if (tmpImgBuffer)
    {
        vPortFree(tmpImgBuffer);
    }

    if (oldHash)
    {
        vPortFree(oldHash);
    }

    if (newHash)
    {
        vPortFree(newHash);
    }

    return status;
}

/**
 * @brief Move module
 *
 * @param oldImgStartAddr Starting addr of the source module
 * @param newImg Destination module
 * @param newImgStartAddr Destination starting addr
 * @param imgSize Module size
 * @param imgSig  Module signature
 * @return
 */
static status_t SLN_Update_MoveModule(
    uint32_t oldImgStartAddr, fica_img_type_t newImg, uint32_t newImgStartAddr, uint32_t imgSize, uint8_t *imgSig)
{
    status_t status = kStatus_Success;

    configPRINTF(("[SLN_UPDATE] Start moving img data %d.\r\n", imgSize));
    status = SLN_Update_MoveModuleData(oldImgStartAddr, newImgStartAddr, imgSize);

    if (kStatus_Success == status)
    {
        configPRINTF(("[SLN_UPDATE] Start moving img info.\r\n"));
        status = SLN_Update_WriteModuleInfo(newImg, imgSize, newImgStartAddr, imgSig);
    }

    return status;
}

#if ENABLE_REMAP
status_t SLN_Update_MoveApp(fica_img_type_t newAppImg)
{
    status_t status          = kStatus_Success;
    int32_t flashError       = SLN_FLASH_NO_ERROR;
    uint32_t oldImgStartAddr = 0;
    uint32_t newImgStartAddr = 0;
    uint32_t imgSize         = 0;
    uint8_t *imgSig          = NULL;
    fica_img_type_t oldAppImg;

    if ((newAppImg != FICA_IMG_TYPE_APP_A) && (newAppImg != FICA_IMG_TYPE_APP_B))
    {
        status = kStatus_InvalidArgument;
    }
    else if (newAppImg == FICA_IMG_TYPE_APP_A)
    {
        oldAppImg = FICA_IMG_TYPE_APP_B;
    }
    else
    {
        oldAppImg = FICA_IMG_TYPE_APP_A;
    }

    if (kStatus_Success == status)
    {
        flashError |= FICA_get_app_img_start_addr(oldAppImg, &oldImgStartAddr);
        flashError |= FICA_get_app_img_start_addr(newAppImg, &newImgStartAddr);
        flashError |= FICA_GetImgLength(oldAppImg, &imgSize);
        flashError |= FICA_GetImgSig(oldAppImg, &imgSig);

        if ((SLN_FLASH_NO_ERROR == flashError) && (imgSize == 0))
        {
            flashError |= FICA_get_app_img_max_size(oldAppImg, &imgSize);
        }

        if (SLN_FLASH_NO_ERROR != flashError)
        {
            status = kStatus_Fail;
        }
    }

    if (kStatus_Success == status)
    {
        configPRINTF(("[SLN_UPDATE] Start moving app img.\r\n"));
        status = SLN_Update_MoveModule(oldImgStartAddr, newAppImg, newImgStartAddr, imgSize, imgSig);
    }

    if (kStatus_Success == status)
    {
        FICA_SetCurAppStartType(newAppImg);
        configPRINTF(("[SLN_UPDATE] Image app moved successfully. New app is %d\r\n", newAppImg));
    }
    else
    {
        configPRINTF(("[SLN_UPDATE] Failed to move img app, %d\r\n", status));
    }

    return status;
}
#endif /* ENABLE_REMAP */

#if FICA_RESOURCES_ENABLE
status_t SLN_Update_MoveResources(fica_img_type_t newResource)
{
    status_t status          = kStatus_Success;
    int32_t flashError       = SLN_FLASH_NO_ERROR;
    uint32_t oldImgStartAddr = 0;
    uint32_t newImgStartAddr = 0;
    uint32_t imgSize         = 0;
    uint8_t *imgSig          = NULL;
    fica_img_type_t oldResource;

    if ((newResource != FICA_IMG_TYPE_RESOURCES_A) && (newResource != FICA_IMG_TYPE_RESOURCES_B))
    {
        status = kStatus_InvalidArgument;
    }
    else if (newResource == FICA_IMG_TYPE_RESOURCES_A)
    {
        oldResource = FICA_IMG_TYPE_RESOURCES_B;
    }
    else
    {
        oldResource = FICA_IMG_TYPE_RESOURCES_A;
    }

    if (kStatus_Success == status)
    {
        flashError |= FICA_get_app_img_start_addr(oldResource, &oldImgStartAddr);
        flashError |= FICA_get_app_img_start_addr(newResource, &newImgStartAddr);
        flashError |= FICA_GetImgLength(oldResource, &imgSize);
        flashError |= FICA_GetImgSig(oldResource, &imgSig);

        if ((SLN_FLASH_NO_ERROR == flashError) && (imgSize == 0))
        {
            flashError |= FICA_get_app_img_max_size(oldResource, &imgSize);
        }
        else if (SLN_FLASH_NO_ERROR != flashError)
        {
            status = kStatus_Fail;
        }
    }

    if (kStatus_Success == status)
    {
        status = SLN_Update_MoveModule(oldImgStartAddr, newResource, newImgStartAddr, imgSize, imgSig);
    }

    if (kStatus_Success == status)
    {
        configPRINTF(("[SLN_UPDATE] Image resources moved successfully.\r\n"));
    }
    else
    {
        configPRINTF(("[SLN_UPDATE] Failed to move img resources \r\n"));
    }

    return status;
}
#endif /* FICA_RESOURCES_ENABLE */

status_t SLN_Update_ParseBundle(fica_img_bank_t bundleBank)
{
    status_t status          = kStatus_Success;
    int32_t flashError       = SLN_FLASH_NO_ERROR;
    uint32_t bundleSize      = 0;
    uint32_t bundleStartAddr = 0;
    bundle_meta_t bundleMeta;

    if (s_preparedImgType != FICA_IMG_TYPE_BUNDLE)
    {
        status = kStatus_InvalidArgument;
    }

    if ((bundleBank >= FICA_IMG_BANK_NUMBERS) || (bundleBank <= FICA_IMG_BANK_NONE))
    {
        status = kStatus_InvalidArgument;
    }

    if (kStatus_Success == status)
    {
        flashError |= FICA_GetNewAppSize(&bundleSize);
        flashError |= FICA_GetBankStartingAddr(bundleBank, &bundleStartAddr);

        if ((SLN_FLASH_NO_ERROR == flashError) && (bundleStartAddr != 0))
        {
            status = SLN_Read_Flash_At_Address((bundleStartAddr + bundleSize - sizeof(bundle_meta_t)),
                                               (uint8_t *)&bundleMeta, sizeof(bundle_meta_t));
        }
        else
        {
            status = kStatus_Fail;
        }
    }

    if (kStatus_Success == status)
    {
        uint32_t bundleParsed = sizeof(bundle_meta_t);

        /* Bootable application should be placed ok, parsed for other modules */
        for (int mod_num = 1; mod_num < bundleMeta.upd_mod_num; mod_num++)
        {
            mod_meta_t moduleMeta;
            uint32_t moduleMetaStartingAddr = bundleStartAddr + bundleSize - bundleParsed - sizeof(moduleMeta);

            memset(&moduleMeta, 0, sizeof(moduleMeta));
            status = SLN_Read_Flash_At_Address(moduleMetaStartingAddr, (uint8_t *)&moduleMeta, sizeof(mod_meta_t));

            if (kStatus_Success == status)
            {
                uint32_t moduleStartingAddr       = 0;
                uint32_t moduleBundleStartingAddr = bundleStartAddr + moduleMeta.upd_start_addr;
                int32_t moduleImgType             = moduleMeta.upd_mod_type;

#if ENABLE_REMAP
                flashError = FICA_GetFreeImgSlot(&moduleImgType);
#endif /* ENABLE_REMAP */

                if (SLN_FLASH_NO_ERROR == flashError)
                {
                    flashError = FICA_get_app_img_start_addr(moduleImgType, &moduleStartingAddr);
                }

                if (SLN_FLASH_NO_ERROR == flashError)
                {
                    status = SLN_Update_MoveModuleData(moduleBundleStartingAddr, moduleStartingAddr,
                                                       moduleMeta.upd_mod_length);
                    if (kStatus_Success == status)
                    {
                        status = SLN_Update_WriteModuleInfo(moduleImgType, moduleBundleStartingAddr,
                                                            moduleMeta.upd_mod_length, moduleMeta.modPkiSig);
                    }
                }
                else
                {
                    status = kStatus_Fail;
                }

                if (kStatus_Success == status)
                {
                    configPRINTF(("[SLN_UPDATE] Module %d moved successfully.\r\n", moduleImgType));
                    bundleParsed += moduleMeta.upd_mod_length + sizeof(mod_meta_t);
                }
                else
                {
                    configPRINTF(("[SLN_UPDATE] Failed to move module %d \r\n", moduleImgType));
                    break;
                }
            }
        }
    }

    return status;
}

status_t SLN_Update_VerifyImgBank(fica_img_type_t imgType, uint8_t *imgSig, uint8_t *imgCert)
{
#if !DISABLE_IMAGE_VERIFICATION
    uint32_t imgStartingAddr = 0;
    uint32_t imgSize         = 0;
    status_t status          = kStatus_Success;
    int32_t fica_error       = SLN_FLASH_NO_ERROR;

    if ((imgSig == NULL) || (imgCert == NULL) || (imgType != s_preparedImgType))
    {
        status = kStatus_InvalidArgument;
    }

    if (kStatus_Success == status)
    {
        fica_error |= FICA_GetNewAppStartAddr(&imgStartingAddr);
        fica_error |= FICA_GetNewAppSize(&imgSize);

        // Verify this certificate
        fica_error |= FICA_Verify_Certificate_From_Buffer(imgCert);

        if (SLN_FLASH_NO_ERROR == fica_error)
        {
            fica_error = SLN_AUTH_Verify_Signature(imgCert, (uint8_t *)imgStartingAddr, imgSize, (uint8_t *)imgSig);
        }
    }

    if (SLN_FLASH_NO_ERROR != fica_error)
    {
        status = kStatus_Fail;
    }

    return status;

#else
    return kStatus_Success;
#endif /* !DISABLE_IMAGE_VERIFICATION */
}

status_t SLN_Update_PrepareImgBank(fica_img_type_t imgType, uint32_t size)
{
    int32_t flashError = SLN_FLASH_NO_ERROR;
    status_t status    = kStatus_Success;

    flashError = FICA_initialize();

    switch (imgType)
    {
        case FICA_IMG_TYPE_APP_A:
        case FICA_IMG_TYPE_APP_B:
        {
            if (size > FICA_IMG_APP_A_SIZE)
            {
                status = kStatus_Fail;
            }
            else
            {
                flashError = FICA_app_program_ext_init(imgType);
            }
        }
        break;
#if FICA_RESOURCES_ENABLE
        case FICA_IMG_TYPE_RESOURCES_A:
        case FICA_IMG_TYPE_RESOURCES_B:
        {
            if (size > FICA_IMG_RESOURCE_SIZE)
            {
                status = kStatus_Fail;
            }
            else
            {
                // Init FICA to be ready for the new application
                flashError = FICA_app_program_ext_init(imgType);
            }
        }
        break;
#endif /* FICA_RESOURCES_ENABLE */

#if FICA_BUNDLE_ENABLE
        case FICA_IMG_TYPE_BUNDLE:
        {
            int32_t curBank;
            flashError = FICA_GetCurBank(&curBank);

            if (size > FICA_IMG_BANK_SIZE)
            {
                status = kStatus_Fail;
            }
            else if (SLN_FLASH_NO_ERROR == flashError)
            {
                if (curBank == FICA_IMG_BANK_A)
                {
                    // Init FICA to be ready for the new application
                    flashError = FICA_bank_program_ext_init(FICA_IMG_BANK_B);
                }
                else if (curBank == FICA_IMG_BANK_B)
                {
                    flashError = FICA_bank_program_ext_init(FICA_IMG_BANK_A);
                }
            }
        }
        break;
#endif /* FICA_BUNDLE_ENABLE */

        default:
            status = kStatus_InvalidArgument;
            break;
    }

    if (SLN_FLASH_NO_ERROR != flashError)
    {
        status = kStatus_Fail;
    }

    if (kStatus_Success == status)
    {
        s_preparedImgType = imgType;
        s_fileSize        = size;
    }

    return status;
}

status_t SLN_Update_WriteImg(fica_img_type_t imgType, uint32_t offset, uint8_t *buffer, uint32_t size)
{
    status_t status    = kStatus_Success;
    int32_t flashError = SLN_FLASH_NO_ERROR;

    if ((buffer == NULL) || (size == 0) || (imgType != s_preparedImgType))
    {
        return kStatus_InvalidArgument;
    }

    switch (imgType)
    {
        case FICA_IMG_TYPE_APP_A:
        case FICA_IMG_TYPE_APP_B:
#if FICA_RESOURCES_ENABLE
        case FICA_IMG_TYPE_RESOURCES_A:
        case FICA_IMG_TYPE_RESOURCES_B:
#endif /* FICA_RESOURCES_ENABLE */
#if FICA_BUNDLE_ENABLE
        case FICA_IMG_TYPE_BUNDLE:
#endif /* FICA_BUNDLE_ENABLE */
        {
            flashError = FICA_app_program_ext_abs(offset, buffer, size);
        }
        break;
        default:
            status = kStatus_InvalidArgument;
            break;
    }

    if (SLN_FLASH_NO_ERROR != flashError)
    {
        status = kStatus_Fail;
    }

    return status;
}

status_t SLN_Update_FinalizeUpdate(fica_img_type_t imgType, uint8_t *imgSig)
{
    status_t status          = kStatus_Success;
    int32_t fica_status      = SLN_FLASH_NO_ERROR;
    int32_t imgBank          = FICA_IMG_BANK_NONE;
    uint32_t imgStartingAddr = 0;
    uint32_t imgSize         = 0;

    fica_status = FICA_app_program_ext_finalize();
    fica_status |= FICA_GetNewAppStartAddr(&imgStartingAddr);
    fica_status |= FICA_GetNewAppSize(&imgSize);
    fica_status |= FICA_GetCurBank(&imgBank);

    if (SLN_FLASH_NO_ERROR != fica_status)
    {
        return kStatus_Fail;
    }

    switch (imgType)
    {
        case FICA_IMG_TYPE_APP_A:
        case FICA_IMG_TYPE_APP_B:
        {
#if FICA_RESOURCES_ENABLE
            if (imgBank == FICA_IMG_BANK_A)
            {
                imgType = FICA_IMG_TYPE_APP_B;
                status  = SLN_Update_MoveResources(FICA_IMG_TYPE_RESOURCES_B);
            }
            else if (imgBank == FICA_IMG_BANK_B)
            {
                imgType = FICA_IMG_TYPE_APP_A;
                status  = SLN_Update_MoveResources(FICA_IMG_TYPE_RESOURCES_A);
            }
#endif /* FICA_RESOURCES_ENABLE */
            if (kStatus_Success == status)
            {
                status = SLN_Update_WriteModuleInfo(imgType, imgSize, imgStartingAddr, imgSig);

                if (kStatus_Success == status)
                {
                    fica_status = FICA_app_program_ext_set_reset_vector();
                }
            }
        }
        break;
#if FICA_RESOURCES_ENABLE
        case FICA_IMG_TYPE_RESOURCES_A:
        case FICA_IMG_TYPE_RESOURCES_B:
        {
            if (imgBank == FICA_IMG_BANK_A)
            {
                imgType = FICA_IMG_TYPE_RESOURCES_B;
                status  = SLN_Update_MoveApp(FICA_IMG_TYPE_APP_B);
            }
            else if (imgBank == FICA_IMG_BANK_B)
            {
                imgType = FICA_IMG_TYPE_RESOURCES_A;
                status  = SLN_Update_MoveApp(FICA_IMG_TYPE_APP_A);
            }

            if (kStatus_Success == status)
            {
                status = SLN_Update_WriteModuleInfo(imgType, imgSize, imgStartingAddr, imgSig);
            }
        }
        break;
#endif /* FICA_RESOURCES_ENABLE */
#if FICA_BUNDLE_ENABLE
        case FICA_IMG_TYPE_BUNDLE:
        {
            if (imgBank == FICA_IMG_BANK_A)
            {
                status = SLN_Update_ParseBundle(FICA_IMG_BANK_B);
            }
            else if (imgBank == FICA_IMG_BANK_B)
            {
                status = SLN_Update_ParseBundle(FICA_IMG_BANK_A);
            }

            if (kStatus_Success == status)
            {
                int32_t newAppImg = FICA_IMG_TYPE_APP_A;
                fica_status       = FICA_GetFreeImgSlot(&newAppImg);
                if (SLN_FLASH_NO_ERROR == fica_status)
                {
                    fica_status = FICA_SetCurAppStartType(newAppImg);
                }
            }
        }
        break;
#endif /* FICA_BUNDLE_ENABLE */
        default:
            status = kStatus_InvalidArgument;
            break;
    }

    if (SLN_FLASH_NO_ERROR != fica_status)
    {
        status = kStatus_Fail;
    }

    return status;
}

status_t SLN_Update_Rollback()
{
    int32_t currentBank = FICA_IMG_BANK_NONE;
    status_t status     = kStatus_Success;

    FICA_GetCurBank(&currentBank);

    if (currentBank == FICA_IMG_BANK_A)
    {
        status = FICA_SetCurAppStartType(FICA_IMG_TYPE_APP_B);
    }
    else if (currentBank == FICA_IMG_BANK_B)
    {
        status = FICA_SetCurAppStartType(FICA_IMG_TYPE_APP_A);
    }

    return status;
}
