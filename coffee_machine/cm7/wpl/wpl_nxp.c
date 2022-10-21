/*
 * Copyright 2020, 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wpl.h"
#include "webconfig.h"
#include "wlan_bt_fw.h"
#include "wlan.h"
#include "wifi.h"
#include "wm_net.h"
#include <wm_os.h>
#include "dhcp-server.h"
#include <stdio.h>

#define WPL_RETRY_LEAVE_MS (100U)

static TaskHandle_t xInitTaskNotify  = NULL;
static TaskHandle_t xScanTaskNotify  = NULL;
static TaskHandle_t xLeaveTaskNotify = NULL;
static TaskHandle_t xUapTaskNotify   = NULL;
static TaskHandle_t xJoinTaskNotify  = NULL;
sln_wifi_connection_cbs_t s_appCallback;

char ssids_json[2048];

static struct wlan_network sta_network;
static struct wlan_network uap_network;

/* Callback Function passed to WLAN Connection Manager. The callback function
 * gets called when there are WLAN Events that need to be handled by the
 * application.
 */
static int wlan_event_callback(enum wlan_event_reason reason, void *data)
{
    static uint8_t s_connectAttempt = 0;
    wifi_fw_version_ext_t ver;
    char ip[16];

    switch (reason)
    {
        case WLAN_REASON_SUCCESS:
            WPL_GetIP(ip, 1);
            configPRINTF(("Connected to following BSS:"));
            configPRINTF(("SSID = [%s], IP = [%s]", sta_network.ssid, ip));
            if (xJoinTaskNotify != NULL)
            {
                xTaskNotify(xJoinTaskNotify, WPL_SUCCESS, eSetValueWithOverwrite);
                xJoinTaskNotify = NULL;
            }
            else
            {
                /* If there are no task waiting call the app layer */
                if (s_appCallback.connected)
                {
                    s_appCallback.connected();
                }
            }
            s_connectAttempt = 0;
            break;
        case WLAN_REASON_AUTH_SUCCESS:
            configPRINTF(("[!] Network Auth Success"));
            s_connectAttempt = 0;
            break;
        case WLAN_REASON_CONNECT_FAILED:
            configPRINTF(("[!] WLAN: connect failed"));

            wlan_remove_network(sta_network.name);
            if (xJoinTaskNotify != NULL)
            {
                xTaskNotify(xJoinTaskNotify, WPL_ERROR, eSetValueWithOverwrite);
                xJoinTaskNotify = NULL;
            }
            else
            {
                /* If there are no task waiting call the app layer */
                if (s_appCallback.disconnected)
                {
                    s_appCallback.disconnected();
                }
            }
            break;
        case WLAN_REASON_NETWORK_NOT_FOUND:
            configPRINTF(("[!] WLAN: network not found"));
            s_connectAttempt++;
            break;
        case WLAN_REASON_NETWORK_AUTH_FAILED:
            configPRINTF(("[!] Network Auth failed"));
            s_connectAttempt++;
            break;
        case WLAN_REASON_ADDRESS_SUCCESS:
            // configPRINTF("network mgr: DHCP new lease");
            break;
        case WLAN_REASON_ADDRESS_FAILED:
            configPRINTF(("[!] failed to obtain an IP address"));

            if (xJoinTaskNotify != NULL)
            {
                xTaskNotify(xJoinTaskNotify, WPL_ERROR, eSetValueWithOverwrite);
                xJoinTaskNotify = NULL;
            }
            break;

        case WLAN_REASON_LINK_LOST:

            configPRINTF(("[!] Link Lost."));
            s_connectAttempt++;
            break;
        case WLAN_REASON_CHAN_SWITCH:
            break;

        case WLAN_REASON_WPS_DISCONNECT:
            break;
        case WLAN_REASON_USER_DISCONNECT:

            configPRINTF(("Dis-connected from: %s", sta_network.ssid));
            // Remove the network and return
            wlan_remove_network(sta_network.name);

            if (xLeaveTaskNotify != NULL)
            {
                // Notify the WPL_Leave task only if this has been called from a WPL_Leave task
                xTaskNotifyGive(xLeaveTaskNotify);
                // Retset the task notification handle back to NULL
                xLeaveTaskNotify = NULL;
            }
            else
            {
                /* If there are no task waiting call the app layer */
                if (s_appCallback.disconnected)
                {
                    s_appCallback.disconnected();
                }
            }
            s_connectAttempt = 0;
            break;

        case WLAN_REASON_INITIALIZED:
            configPRINTF(("WLAN initialized"));
            /* Print WLAN FW Version */
            wifi_get_device_firmware_version_ext(&ver);
            configPRINTF(("WLAN FW Version: %s", ver.version_str));
            if (xInitTaskNotify != NULL)
            {
                xTaskNotifyGive(xInitTaskNotify);
            }
            break;
        case WLAN_REASON_INITIALIZATION_FAILED:
            configPRINTF(("app_cb: WLAN: initialization failed"));
            break;

        case WLAN_REASON_PS_ENTER:
            break;
        case WLAN_REASON_PS_EXIT:
            break;

        case WLAN_REASON_UAP_SUCCESS:
            configPRINTF(("Soft AP started successfully"));
            if (xUapTaskNotify != NULL)
            {
                xTaskNotifyGive(xUapTaskNotify);
                xUapTaskNotify = NULL;
            }

            break;

        case WLAN_REASON_UAP_CLIENT_ASSOC:
            configPRINTF(("Client => "));
            print_mac((const char *)data);
            configPRINTF(("Associated with Soft AP"));
            break;
        case WLAN_REASON_UAP_CLIENT_DISSOC:
            configPRINTF(("Client => "));
            print_mac((const char *)data);
            configPRINTF(("Dis-Associated from Soft AP"));
            break;

        case WLAN_REASON_UAP_START_FAILED:
            configPRINTF(("[!] Failed to start AP"));
            break;
        case WLAN_REASON_UAP_STOP_FAILED:
            configPRINTF(("[!] Failed to stop AP"));
            break;
        case WLAN_REASON_UAP_STOPPED:
            wlan_remove_network(uap_network.name);
            configPRINTF(("Soft AP stopped successfully"));
            if (xUapTaskNotify != NULL)
            {
                xTaskNotifyGive(xUapTaskNotify);
                xUapTaskNotify = NULL;
            }

            break;
        default:
            configPRINTF(("Unknown Wifi CB Reason %d", reason));
            break;
    }

    if (s_connectAttempt > WLAN_RECONNECT_LIMIT)
    {
        /* we will not receive a disconnect event
         * remove here the network and notify the waiting task */
        wlan_remove_network(sta_network.name);
        if (xJoinTaskNotify != NULL)
        {
            // Notify the WPL_Join task only if this has been called from a WPL_Join task
            xTaskNotify(xJoinTaskNotify, WPL_ERROR, eSetValueWithOverwrite);
            // Retset the task notification handle back to NULL
            xJoinTaskNotify = NULL;
        }
        else if (s_appCallback.disconnected)
        {
            /* If there are no task waiting call the app layer */
            s_appCallback.disconnected();
        }
        s_connectAttempt = 0;
    }

    return WPL_SUCCESS;
}

int WPL_Init(sln_wifi_connection_cbs_t cb)
{
    int32_t result;
    s_appCallback.connected    = cb.connected;
    s_appCallback.disconnected = cb.disconnected;
    result                     = wlan_init(wlan_fw_bin, wlan_fw_bin_len);
    if (result == WM_SUCCESS)
        return WPL_SUCCESS;
    else
        return result;
}

int WPL_Start()
{
    int32_t result;
    xInitTaskNotify = xTaskGetCurrentTaskHandle();
    result          = wlan_start(wlan_event_callback);

    if (result == WM_SUCCESS)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        return WPL_SUCCESS;
    }
    else
    {
        return result;
    }
}

int WPL_Stop()
{
    int32_t result = wlan_stop();
    if (result == WM_SUCCESS)
    {
        wlan_deinit(0);
    }
    if (result != WM_SUCCESS)
    {
        return WPL_SUCCESS;
    }
    return result;
}

int WPL_Start_AP(char *ssid, char *password, int chan)
{
    int ret;
    uint8_t ssid_len = 0;
    uint8_t psk_len  = 0;
    xUapTaskNotify   = xTaskGetCurrentTaskHandle();

    if (strlen(ssid) > WPL_WIFI_SSID_LENGTH)
    {
        configPRINTF(("[!] SSID is too long!"));
        __BKPT(0);
        return WPL_ERROR;
    }

    if (strlen(password) > WPL_WIFI_PASSWORD_LENGTH)
    {
        configPRINTF(("[!] Password is too long!"));
        __BKPT(0);
        return WPL_ERROR;
    }

    wlan_initialize_uap_network(&uap_network);

    unsigned int uap_ip = ipaddr_addr(WIFI_AP_IP_ADDR);

    /* Set IP address to WIFI_AP_IP_ADDR */
    uap_network.ip.ipv4.address = uap_ip;
    /* Set default gateway to WIFI_AP_IP_ADDR */
    uap_network.ip.ipv4.gw = uap_ip;

    /* Set SSID as passed by the user */
    ssid_len = (strlen(ssid) <= WPL_WIFI_SSID_LENGTH) ? strlen(ssid) : WPL_WIFI_SSID_LENGTH;
    memcpy(uap_network.ssid, ssid, ssid_len);

    uap_network.channel = chan;

    uap_network.security.type = WLAN_SECURITY_WPA2;
    /* Set the passphrase. Max WPA2 passphrase can be upto 64 ASCII chars */
    psk_len = (strlen(password) <= (WLAN_PSK_MAX_LENGTH - 1)) ? strlen(password) : (WLAN_PSK_MAX_LENGTH - 1);
    strncpy(uap_network.security.psk, password, psk_len);
    uap_network.security.psk_len = psk_len;

    ret = wlan_add_network(&uap_network);
    if (ret != WM_SUCCESS)
    {
        return WPL_ERROR;
    }
    ret = wlan_start_network(uap_network.name);
    if (ret != WM_SUCCESS)
    {
        return WPL_ERROR;
    }

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    return WPL_SUCCESS;
}

int WPL_Stop_AP()
{
    int ret;
    xUapTaskNotify = xTaskGetCurrentTaskHandle();

    ret = wlan_stop_network(uap_network.name);
    if (ret != WM_SUCCESS)
    {
        return WPL_ERROR;
    }

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    return WPL_SUCCESS;
}

static int WLP_process_results(unsigned int count)
{
    struct wlan_scan_result scan_result;
    char network_buf[512];

    /* Start building JSON */
    strcpy(ssids_json, "[");

    for (int i = 0; i < count; i++)
    {
        wlan_get_scan_result(i, &scan_result);

        configPRINTF(("     SSID:%s", scan_result.ssid));
        configPRINTF(("     BSSID         : %02X:%02X:%02X:%02X:%02X:%02X", (unsigned int)scan_result.bssid[0],
                      (unsigned int)scan_result.bssid[1], (unsigned int)scan_result.bssid[2],
                      (unsigned int)scan_result.bssid[3], (unsigned int)scan_result.bssid[4],
                      (unsigned int)scan_result.bssid[5]));
        configPRINTF(("     RSSI          : -%ddBm", (int)scan_result.rssi));
        configPRINTF(("     Channel       : %d", (int)scan_result.channel));

        char security[64];
        security[0] = '\0';

        if (scan_result.wpa2_entp)
        {
            strcat(security, "WPA2_ENTP ");
        }
        if (scan_result.wep)
        {
            strcat(security, "WEP ");
        }
        if (scan_result.wpa)
        {
            strcat(security, "WPA ");
        }
        if (scan_result.wpa2)
        {
            strcat(security, "WPA2 ");
        }
        if (scan_result.wpa3_sae)
        {
            strcat(security, "WPA3_SAE ");
        }
        configPRINTF(("     Security      : %s", security));

        if (strlen(ssids_json) + 256 > sizeof(ssids_json))
        {
            configPRINTF(("[!] The SSID_JSON is too small, can not fill all the SSIDS "));
            xTaskNotify(xScanTaskNotify, WPL_ERROR, eSetValueWithOverwrite);
            return WPL_ERROR;
        }

        if (i != 0)
        {
            // Add , separator after first entry
            strcat(ssids_json, ",\r\n");
        }

        snprintf(network_buf, sizeof(network_buf) - 1,
                 "{\r\n \"ssid\":\"%s\",\r\n \"signal\":\"%ddBm\",\r\n \"channel\":%d\r\n}", scan_result.ssid,
                 -(int)scan_result.rssi, (int)scan_result.channel);
        strcat(ssids_json, network_buf);
    }

    strcat(ssids_json, "]");
    xTaskNotify(xScanTaskNotify, WPL_SUCCESS, eSetValueWithOverwrite);
    return WPL_SUCCESS;
}

int WPL_Scan()
{
    xScanTaskNotify = xTaskGetCurrentTaskHandle();
    configPRINTF(("\nInitiating scan...\n\n"));
    wlan_scan(WLP_process_results);
    return ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

int WPL_Join(char *ssid, char *password)
{
    int ret;
    uint32_t pwd_len = 0;
    // Note down the Join task so that
    xJoinTaskNotify = xTaskGetCurrentTaskHandle();

    if (strlen(ssid) > WPL_WIFI_SSID_LENGTH)
    {
        configPRINTF(("[!] SSID is too long!"));
        return WPL_ERROR;
    }

    pwd_len = strlen(password);
    if (pwd_len > WPL_WIFI_PASSWORD_LENGTH)
    {
        configPRINTF(("[!] Password is too long!"));
        __BKPT(0);
        return WPL_ERROR;
    }

    memset(&sta_network, 0, sizeof(struct wlan_network));

    strcpy(sta_network.name, "sta_network");

    memcpy(sta_network.ssid, (const char *)ssid, strlen(ssid));
    sta_network.ip.ipv4.addr_type = ADDR_TYPE_DHCP;
    sta_network.ssid_specific     = 1;

    sta_network.security.type = WLAN_SECURITY_NONE;
    if (pwd_len > 0)
    {
        sta_network.security.type = WLAN_SECURITY_WILDCARD;
        strncpy(sta_network.security.psk, password, pwd_len);
        sta_network.security.psk_len = pwd_len;
    }

    ret = wlan_add_network(&sta_network);
    if (ret != WM_SUCCESS)
    {
        return WPL_ERROR;
    }
    ret = wlan_connect(sta_network.name);

    if (ret != WM_SUCCESS)
    {
        configPRINTF(("Failed to connect %d", ret));
        return WPL_ERROR;
    }

    // Wait for response
    if (WPL_SUCCESS == ulTaskNotifyTake(pdTRUE, portMAX_DELAY))
    {
        return WPL_SUCCESS;
    }
    else
    {
        configPRINTF(("Failed to connect"));
        return WPL_ERROR;
    }
}

int WPL_Leave()
{
    int ret                          = 0;
    enum wlan_connection_state state = WLAN_DISCONNECTED;

    do
    {
        if (wlan_get_connection_state(&state) == WM_SUCCESS)
        {
            if (WLAN_CONNECTED == state)
            {
                // Note down the current task handle so that it can be notified after Leave is done
                xLeaveTaskNotify = xTaskGetCurrentTaskHandle();
                int wlan_ret     = wlan_disconnect();

                if (wlan_ret != WM_SUCCESS)
                {
                    ret = WPL_ERROR;
                }

                ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            }
            else if (WLAN_DISCONNECTED != state)
            {
                vTaskDelay(pdMS_TO_TICKS(WPL_RETRY_LEAVE_MS));
            }
        }
        else
        {
            ret = WPL_ERROR;
            break;
        }
    } while ((state != WLAN_CONNECTED) && (state != WLAN_DISCONNECTED));

    return ret;
}

int WPL_StartDHCPServer(char *ip, char *net)
{
    // The IP and Net parameter are not used on NXP Wi-Fi
    // It is instead extracted form the uap net handle
    char current_ip[16];
    WPL_GetIP(current_ip, 0);
    configPRINTF(("This also starts DHCP Server with IP %s", current_ip));
    if (dhcp_server_start(net_get_uap_handle()))
    {
        configPRINTF(("Error in starting dhcp server"));
        return WPL_ERROR;
    }
    return WPL_SUCCESS;
}

int WPL_StopDHCPServer()
{
    dhcp_server_stop();
    return WPL_SUCCESS;
}

char *WPL_getSSIDs()
{
    return ssids_json;
}

int WPL_GetIP(char *ip, int client)
{
    struct wlan_ip_config addr;
    int ret;

    if (client)
        ret = wlan_get_address(&addr);
    else
        ret = wlan_get_uap_address(&addr);

    if (ret != WM_SUCCESS)
    {
        return WPL_ERROR;
    }

    net_inet_ntoa(addr.ipv4.address, ip);

    return WPL_SUCCESS;
}
