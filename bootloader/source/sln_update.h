/*
 * Copyright 2021-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _SLN_UPDATE_H_
#define _SLN_UPDATE_H_

#include "stdbool.h"
#include "fsl_common.h"
#include "fica_definition.h"

/*! @brief module metadata structure */
typedef struct _mod_meta
{
    fica_img_type_t upd_mod_type; /*!< Module type */
    uint32_t upd_start_addr;      /*!< Module starting addr */
    uint32_t upd_mod_length;      /*!< Module length */
    uint8_t modPkiSig[256];       /*!< Module signature */
} mod_meta_t;

typedef struct _bundle_meta
{
    uint32_t upd_bundle_size;  /*!< Size of the updater bundle */
    uint32_t upd_mod_num;      /*!< Number of modules encapsulated into the bundle */
    uint8_t bundlePkiSig[256]; /*!< Module signature */
} bundle_meta_t;

#if ENABLE_REMAP
/*!
 * @brief Used to move the code App Img from one bank to another.
 * Will also set the app type to newAppImg
 * Only possible when the remapping is enable.
 *
 * @param newAppImg new Img type
 * @return kStatus_Success if the operation was successful
 */
status_t SLN_Update_MoveApp(fica_img_type_t newAppImg);
#endif /* ENABLE_REMAP */

#if FICA_RESOURCES_ENABLE
/*!
 * @brief Used to move the resources image.
 *
 * @param newResource new resource type
 * @return kStatus_Success if the operation was successful
 */
status_t SLN_Update_MoveResources(fica_img_type_t newResource);
#endif /* FICA_RESOURCES_ENABLE */

/*!
 * @brief Call this function after a SLN_Update_WriteImg for a bundle image.
 * This function will parse the bundle written in the specified bank and will move all the application
 * to their specific addr.
 *
 * @param bundleBank Bank in which the bundle is written.
 * @return kStatus_Success if the operation was successful
 */
status_t SLN_Update_ParseBundle(fica_img_bank_t bundleBank);

/*!
 * @brief Verify the signature for the just written image.
 *
 * @param imgType imgType for the application to check the signature
 * @param imgSig New img signature
 * @param imgCert New img certification
 * @return kStatus_InvalidArgument if imgSig or imgCert or if the newly updated image doesn't match with imgType
 * parameter kStatus_Fail if the verification failed.
 */
status_t SLN_Update_VerifyImgBank(fica_img_type_t imgType, uint8_t *imgSig, uint8_t *imgCert);

/*!
 * @brief Prepare app for update.
 * Prepare FICA for an update operation
 * Start erasing the bank.
 *
 * @param imgType Img type for which to do the preparation
 * @param size Size of the image. If the size of the image is not known 0 can be passed.
 * @return
 */
status_t SLN_Update_PrepareImgBank(fica_img_type_t imgType, uint32_t size);

/*!
 * @brief Write the image in the flash. To be called only after SLN_Update_PrepareImgBank
 *
 * @param imgType Img type that needs to be written
 * @param offset Offset of the image. Used when writing the image in chunks.
 * @param buffer Buffer from where to read the new img
 * @param size Size of the current chunk.
 * @return
 */
status_t SLN_Update_WriteImg(fica_img_type_t imgType, uint32_t offset, uint8_t *buffer, uint32_t size);

/*!
 * @brief Finalize the update procedure. Change fica entry.
 *
 * @param imgType Img type that the procedure happended
 * @param uint32_t *imgSig img signature. Null if no img verification
 * @return
 */
status_t SLN_Update_FinalizeUpdate(fica_img_type_t imgType, uint8_t *imgSig);

/*!
 * @brief In case of failure rollback to old imgtype.
 *
 * @return
 */
status_t SLN_Update_Rollback(void);

/**
 * @brief Check if there is an update request pending. This is project/platform related.
 *
 * @return Return true if there is a request waiting, false otherwise.
 */
bool SLN_CheckForUpdate(void);

#endif /* _SLN_UPDATE_H_ */
