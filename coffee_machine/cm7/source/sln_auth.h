/*
 * Copyright 2018-2019, 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _SLN_AUTH_H_
#define _SLN_AUTH_H_ 1

#include "fica_definition.h"
#include "fsl_common.h"
#include "mbedtls/md.h"

#define MAX_CERT_LEN 2048
#define RSA_SIG_LEN  256

#define BACKUP_REGION FICA_CRYPTO_BACKUP_ADDR

/*!
 * @brief Authentication functions status return codes
 *
 */
typedef enum _sln_auth_status
{
    SLN_AUTH_OK           = kStatus_Success,
    SLN_AUTH_NULL_PTR     = -1,
    SLN_AUTH_BAD_CERT     = -2,
    SLN_AUTH_BAD_CERT2    = -3,
    SLN_AUTH_INVALID_CERT = -4,
    SLN_AUTH_INVALID_SIG  = -5,
    SLN_AUTH_NO_MEM       = -6,
    SLN_AUTH_NO_CONTEXT   = -7,
    SLN_AUTH_INVALID_ARG  = -8,
    SLN_AUTH_ERR          = -9,
} sln_auth_status_t;

typedef enum _sln_crypto_ctx_idx
{
    SLN_CRYPTO_CTX_NONE = -1,
    SLN_CRYPTO_CTX_0,
    SLN_CRYPTO_CTX_1,
    SLN_CRYPTO_CTX_COUNT,
} sln_crypto_ctx_idx_t;

/*!
 * @brief Check for crypto context
 *
 */
int32_t SLN_AUTH_check_context(int32_t context);

/*!
 * @brief Backup crypto context
 *
 */
int32_t SLN_AUTH_backup_context(int32_t context);

/*!
 * @brief Swaps crypto context
 *
 */
int32_t SLN_AUTH_swap_context(int32_t context);

/*!
 * @brief Restore crypto context
 *
 */
int32_t SLN_AUTH_restore_context(int32_t context);

/*!
 * @brief Save context
 *
 */
int32_t SLN_AUTH_save_context(int32_t context, uint32_t *buffer);

/*!
 * @brief Full context save of both contexts
 *
 */
int32_t SLN_AUTH_full_save_context(uint32_t *ctx0, uint32_t *ctx1);

/*!
 * @brief Verify certificate against CA root
 *
 */
int32_t SLN_AUTH_Verify_Cert(uint8_t *caPem, uint8_t *vfPem);

/*!
 * @brief Verify RSA signature against given certificate
 *
 */
int32_t SLN_AUTH_Verify_Signature(uint8_t *vfPem, uint8_t *msg, size_t msglen, uint8_t *msgsig);

/*!
 * @brief Generates the Hash for a specific message found in the memory.
 * No copy in intermediate buffer will be done.
 * If the message is in FLASH, this function leverages the XIP mechanism so data will not be hard copied inside internal
 * local memory. Be sure that the address of the message is available at all time. Do not use it when reading from
 * remapped address space!!
 *
 * @param msg Address of the message
 * @param msglen Size of the message
 * @param hash Pointer to where the function should store the digested hash
 * @param hashSize Size of the hash
 * @param mdType Type of message digest operation to do
 * @return SLN_AUTH_NULL_PTR if msg or hash are NULL
 * SLN_AUTH_NO_MEM if the hashSize is less than required hash memory specified by mdType
 * SLN_AUTH_ERR if the message digest operation failed
 * SLN_AUTH_OK otherwise
 */
int32_t SLN_AUTH_Generate_HashFast(
    uint8_t *msg, size_t msglen, uint8_t *hash, uint32_t hashSize, mbedtls_md_type_t mdType);

/*!
 * @brief Generates the Hash for a specific message found in FLASH.
 * The message will be copied in internal memory so the message digest process is slower
 *
 * @param msg Flash address offset relative to flash starting addr
 * @param msglen Size of the message
 * @param hash Pointer to where the function should store the digested hash
 * @param hashSize Size of the hash
 * @param mdType Type of message digest operation to do
 * @return SLN_AUTH_NULL_PTR if msg or hash are NULL
 * SLN_AUTH_NO_MEM if the hashSize is less than required hash memory specified by mdType or if failed to allocated
 * memory for READ operation. SLN_AUTH_ERR if the message digest operation failed SLN_AUTH_OK otherwise
 */
int32_t SLN_AUTH_Generate_Hash(uint8_t *msg, size_t msglen, uint8_t *hash, uint32_t hashSize, mbedtls_md_type_t mdType);

#if UPDATER_SUPPORT_ENABLED
/*!
 * @brief Parse the certificate received as argument, to check if it has the proper format
 *
 */
int32_t SLN_AUTH_Parse_Cert(uint8_t *certPem);

/*!
 * @brief Verify if the hash of the image starting at msg and having length msglen matches pre_write_hash
 *
 */
int32_t SLN_AUTH_Verify_Hash(uint8_t *msg, size_t msglen, uint8_t *pre_write_hash);

/*!
 * @brief Verify RSA signature against given certificate. The image is encrypted,
 *        so some decryption operations must be performed for obtaining the hash
 *
 */
int32_t SLN_AUTH_Verify_Signature_Encrypted(uint8_t *vfPem, uint8_t *msg, size_t msglen, uint8_t *msgsig);

#endif /* UPDATER_SUPPORT_ENABLED */

#endif /* _SLN_AUTH_H_ */
