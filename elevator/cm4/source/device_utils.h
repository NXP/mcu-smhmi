/*
 * Copyright 2019-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef DEVICE_UTILS_H_
#define DEVICE_UTILS_H_

#include <stdbool.h>

/* Add a prefix to be sure our unique id is unique among NXP products */
#ifndef BOARD_PREFIX
#define BOARD_PREFIX ""
#endif /* BOARD_PREFIX */

/* Hex serial number length for int64 id, without 0x part */
#define INT64_UNIQUE_ID_HEX_STRING_LEN 16

/*!
 * @brief Generates a unique ID for the client, based on device specific ID. Developer is responsible for freeing
 *        memory allocated by this function.
 *
 * @param cloudSafe boolean declaring whether to generate a cloud safe ID that does not contain special characters
 * @param **uniqueID Pointer to string containing the generated client ID
 *
 */
void APP_GetUniqueID(char **uniqueID, bool removeSpecialCharacters);

/*!
 * @brief Generates a hex unique ID for the client, based on device specific ID. Developer is responsible for freeing
 *        memory allocated by this function.
 *
 * @param **uniqueID Pointer to string containing the generated client ID
 *
 */
void APP_GetHexUniqueID(char **uniqueID);

/*!
 * @brief Generates a unique passphrase for PKCS11 private key decryption
 *
 * @param keyPhrase Output to ascii string passphrase
 */
void PKCS11_KeyGen(char **keyPhrase);

/**
 * @brief Check if the button that trigger the MSD is pressed
 *
 * @return True if the button was pressed, false otherwise
 */
uint8_t BUTTON_MSDPressed(void);

/**
 * @brief Check if the button that trigger the FWUPDATE OTW is pressed
 *
 * @return True if the button was pressed, false otherwise
 */
uint8_t BUTTON_OTWPressed(void);

#endif /* DEVICE_UTILS_H_ */
