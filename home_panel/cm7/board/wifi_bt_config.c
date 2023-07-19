/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wifi_bt_config.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_WIFI_BT_Enable(bool enable)
{
    if (enable)
    {
        /* Set SDIO_RST to 1 */
        GPIO_PortSet(BOARD_WIFI_AWAM510_WLAN_ENABLE_GPIO, 1 << BOARD_WIFI_AWAM510_WLAN_ENABLE_PIN);
        vTaskDelay(pdMS_TO_TICKS(100));
        /* Set SDIO_RST to 1 */
        GPIO_PortSet(BOARD_WIFI_AWAM510_BT_ENABLE_GPIO, 1 << BOARD_WIFI_AWAM510_BT_ENABLE_PIN);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    else
    {
        /* Set SDIO_RST to 1 */
        GPIO_PortClear(BOARD_WIFI_AWAM510_WLAN_ENABLE_GPIO, 1 << BOARD_WIFI_AWAM510_WLAN_ENABLE_PIN);
        vTaskDelay(pdMS_TO_TICKS(100));
        /* Set SDIO_RST to 1 */
        GPIO_PortClear(BOARD_WIFI_AWAM510_BT_ENABLE_GPIO, 1 << BOARD_WIFI_AWAM510_BT_ENABLE_PIN);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void BOARD_WIFI_BT_Config(void *card, sdio_int_t cardInt)
{
    BOARD_SDIO_Config(card, NULL, BOARD_SDMMC_SDIO_HOST_IRQ_PRIORITY, cardInt);
    BOARD_WIFI_BT_Enable(false);
}
