/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By explittlefs_ressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _SLN_FLASH_FILES_H_
#define _SLN_FLASH_FILES_H_

/* Used to include app specific files */

#include "sln_flash_files_ext.h"

/* Save WiFi credentials */
#define WIFI_CRED_FILE_NAME "wifi.dat"

/* Device configuration file */
#define DEVICE_CONFIG_FILE_NAME "dev_cfg.dat"

/* Authentication certificates.*/
#define ROOT_CA_CERT       "ca_root.dat"
#define APP_A_SIGNING_CERT "app_a_sign_cert.dat"
#define APP_B_SIGNING_CERT "app_b_sign_cert.dat"
#define CRED_SIGNING_CERT  "cred_sign_cert.dat"

/* Save registers in case of failure */
#define FAULT_STATUSREG_LOG_FILE_NAME "fault_statusreg_log.dat"

#endif /* _SLN_FLASH_FILES_H_ */
