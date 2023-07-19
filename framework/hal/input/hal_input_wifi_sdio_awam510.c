/*
 * Copyright 2021-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 *
 */

/*
 * @brief WiFi abstraction hal
 */

#include "board_define.h"
#ifdef ENABLE_INPUT_DEV_WiFiAWAM510
#include "app_config.h"
#include <FreeRTOS.h>
#include <wifi_credentials_api.h>

#include "fwk_output_manager.h"
#include "fwk_input_manager.h"
#include "fwk_common.h"
#include "fwk_log.h"
#include "fwk_task.h"
#include "event_groups.h"
#include "semphr.h"
#include "hal_event_descriptor_common.h"
#include "hal_vision_algo.h"
#if ENABLE_FTP_CLIENT
#include "ftp_client_api.h"
#endif /* ENABLE_FTP_CLIENT */

#if ENABLE_OTA
#include "aws_demo_config.h"
#include "ota_core_mqtt.h"
#endif /* ENABLE_OTA */

#include "wm_net.h"
#include "app_config.h"

#define WIFI_TASK_NAME     "WIFI"
#define WIFI_TASK_STACK    1024
#define WIFI_TASK_PRIORITY 4

#if ENABLE_OTA
static TaskHandle_t s_otaTaskHandler = NULL;
#endif /* ENABLE_OTA */

/**
 * @enum _wifi_events
 * @brief events passed between the OutputManager handler and WiFi task handler.
 *
 */
enum _wifi_events
{
    kWiFi_Join,        /**< kWiFi_Join -> connected to the network specify by the WiFI credentials.*/
    kWiFi_Leave,       /**< kWiFi_Leave -> disconnect from the network.*/
    kWiFi_Credentials, /**< kWiFi_Credentials -> WiFi credentials were changed or erase.*/
    kWiFi_StateChange, /**< kWiFi_StateChange -> Turn off/on the WiFi driver.*/
    kWiFi_Reset,       /**< kWiFi_Reset -> Reset the WiFi.*/
    kWiFi_FTPClient,   /**< kWiFi_FTPClient -> Connect to the FTP Server and load the file.*/
    kWiFi_Scan,        /**< kWiFi_Scan -> Scan for nearby networks.*/
};

/**
 * @enum _wifi_state_machine
 * @brief State in which the WiFi is found
 *
 */
enum _wifi_state_machine
{
    kWiFi_State_None,         /**< kWiFi_State_None -> Not initialized*/
    kWiFi_State_FWNotFound,   /** <kWiFi_State_FWNotFound -> Do nothing but don't report it as an error **/
    kWiFi_State_Connected,    /**< kWiFi_State_Connected -> Connected to a WiFi network*/
    kWiFi_State_Disconnected, /**< kWiFi_State_Disconnected -> Not Connected to a WiFi network caused by a disconnection
                                 or wrong credentials*/
    kWiFi_State_DisconnectedAsync, /**< kWiFi_State_DisconnectedAsync -> Not Connected to a WiFi network caused by the
                                      remote*/
    kWiFi_State_Provisioning,      /**< kWiFi_State_Provisioning -> No credentials.*/
};

#if FWK_SUPPORT_STATIC_ALLOCATION
FWKDATA static StackType_t s_WiFiTaskStack[WIFI_TASK_STACK];
FWKDATA static StaticTask_t s_WiFiTaskTCB;
static void *s_WiFiTaskTCBBReference = (void *)&s_WiFiTaskTCB;
#else
static void *s_WiFiTaskStack         = NULL;
static void *s_WiFiTaskTCBBReference = NULL;
#endif /* FWK_SUPPORT_STATIC_ALLOCATION */

static status_t _getIPAddress(char *valueToString);
static status_t _getNetworkSSID(char *valueToString);
static status_t _getNetworkPassword(char *valueToString);
#if ENABLE_FTP_CLIENT
static status_t _getFTPServerInfo(char *valueToString);
#endif /* ENABLE_FTP_CLIENT
*/
static hal_output_status_t HAL_OutputDev_WiFiAWAM510_Init(output_dev_t *dev, output_dev_callback_t callback);
static hal_output_status_t HAL_OutputDev_WiFiAWAM510_Deinit(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_WiFiAWAM510_Start(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_WiFiAWAM510_Stop(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_WiFiAWAM510_InputNotify(const output_dev_t *dev, void *param);
static hal_output_status_t HAL_OutputDev_WiFiAWAM510_InferenceComplete(const output_dev_t *dev,
                                                                       output_algo_source_t source,
                                                                       void *inferResult);

static hal_input_status_t HAL_InputDev_WiFiAWAM510_Init(input_dev_t *dev, input_dev_callback_t callback);
static hal_input_status_t HAL_InputDev_WiFiAWAM510_Deinit(const input_dev_t *dev);
static hal_input_status_t HAL_InputDev_WiFiAWAM510_Start(const input_dev_t *dev);
static hal_input_status_t HAL_InputDev_WiFiAWAM510_Stop(const input_dev_t *dev);
static hal_input_status_t HAL_InputDev_WiFiAWAM510_InputNotify(const input_dev_t *dev, void *param);

typedef struct _wifi_task
{
    fwk_task_t task;
    fwk_task_data_t data;
} wifi_task_t;

const static output_dev_operator_t s_OutputDev_WiFiAWAM510Ops = {
    .init   = HAL_OutputDev_WiFiAWAM510_Init,
    .deinit = HAL_OutputDev_WiFiAWAM510_Deinit,
    .start  = HAL_OutputDev_WiFiAWAM510_Start,
    .stop   = HAL_OutputDev_WiFiAWAM510_Stop,
};

const static input_dev_operator_t s_InputDev_WiFiAWAM510Ops = {
    .init        = HAL_InputDev_WiFiAWAM510_Init,
    .deinit      = HAL_InputDev_WiFiAWAM510_Deinit,
    .start       = HAL_InputDev_WiFiAWAM510_Start,
    .stop        = HAL_InputDev_WiFiAWAM510_Stop,
    .inputNotify = HAL_InputDev_WiFiAWAM510_InputNotify,
};

const static output_dev_event_handler_t s_OutputDev_WiFiAWAM510Handler = {
    .inferenceComplete = HAL_OutputDev_WiFiAWAM510_InferenceComplete,
    .inputNotify       = HAL_OutputDev_WiFiAWAM510_InputNotify,
};

static output_dev_t s_OutputDev_WiFiAWAM510 = {
    .name         = "wifi_awam510",
    .attr.type    = kOutputDevType_Other,
    .attr.reserve = NULL,
    .ops          = &s_OutputDev_WiFiAWAM510Ops,
    .configs =
        {
            [0] = {.name          = "IP-Address",
                   .expectedValue = "x.x.x.x",
                   .description   = "IP - 0.0.0.0 if not connected",
                   .get           = _getIPAddress},
            [1] = {.name = "SSID", .expectedValue = "NameOfTheNetwork", .description = "SSID", .get = _getNetworkSSID},
            [2] = {.name          = "Password",
                   .expectedValue = "PassOfTheNetwork",
                   .description   = "Password",
                   .get           = _getNetworkPassword},
#if ENABLE_FTP_CLIENT
            [3] = {.name          = "FTP_IP/Port",
                   .expectedValue = "x.x.x.x/Port",
                   .description   = "IP - 0.0.0.0/0 if not set",
                   .get           = _getFTPServerInfo},
#endif /* ENABLE_FTP_CLIENT */
        },
    .cap.callback = NULL,
};

static input_dev_t s_InputDev_WiFiAWAM510 = {
    .id = 1, .name = "WIFIAWAM510", .ops = &s_InputDev_WiFiAWAM510Ops, .cap = {.callback = NULL}};

static wifi_task_t s_WiFiTask = {0};
static fwk_message_t s_WiFiMessage;
static uint8_t s_WiFiState = kWiFi_State_None;
static SemaphoreHandle_t s_WiFiMutex;
static wifi_cred_t s_WiFiCred;
static EventGroupHandle_t s_WiFiEvents;

/**
 * @fn void _advertiseWiFiConnectionStatus(bool)
 * @brief Advertise in the system the connection status
 *
 * @param isConnected
 */
static void _advertiseWiFiConnectionStatus(bool isConnected)
{
    uint32_t receiverList = 1 << kFWKTaskID_Output;
    if (s_InputDev_WiFiAWAM510.cap.callback != NULL)
    {
        input_event_t inputEvent;
        event_common_t eventConnection;
        uint8_t fromISR = __get_IPSR();
        /* Build event */
        memset(&eventConnection, 0, sizeof(event_common_t));
        eventConnection.eventBase.eventId = kEventID_WiFiConnected;
        eventConnection.wifi.isConnected  = isConnected;

        /* Build the input event */
        inputEvent.eventId                  = kInputEventID_Recv;
        inputEvent.size                     = sizeof(event_common_t);
        inputEvent.u.inputData.data         = &eventConnection;
        inputEvent.u.inputData.copy         = 1;
        inputEvent.u.inputData.receiverList = receiverList;

        s_InputDev_WiFiAWAM510.cap.callback(&s_InputDev_WiFiAWAM510, &inputEvent, fromISR);
    }
}

/**
 * @fn void __advertiseWiFiOTAStatus(uint8_t)
 * @brief Advertise wifi ota status
 */
void _advertiseWiFiOTAStatus(uint8_t percentage)
{
#if ENABLE_OTA
    /* Let the function exist not to break the build, but do nothing */
    uint32_t receiverList = (1 << kFWKTaskID_Output) | (1 << kFWKTaskID_VisionAlgo) | (1 << kFWKTaskID_Audio);

    if (s_InputDev_WiFiAWAM510.cap.callback != NULL)
    {
        input_event_t inputEvent = {0};
        event_common_t eventOTA  = {0};

        uint8_t fromISR = __get_IPSR();
        /* Build event */

        if (percentage == 0)
        {
            eventOTA.eventBase.eventId = kEventID_OTAStart;
        }
        else if (percentage >= 100)
        {
            eventOTA.eventBase.eventId = kEventID_OTAStop;
        }
        else
        {
            eventOTA.eventBase.eventId    = kEventID_OTAStatus;
            eventOTA.otaStatus.percentage = percentage;
        }

        /* Build the input event */
        inputEvent.eventId                  = kInputEventID_Recv;
        inputEvent.size                     = sizeof(event_common_t);
        inputEvent.u.inputData.data         = &eventOTA;
        inputEvent.u.inputData.copy         = 1;
        inputEvent.u.inputData.receiverList = receiverList;

        s_InputDev_WiFiAWAM510.cap.callback(&s_InputDev_WiFiAWAM510, &inputEvent, fromISR);
    }
#endif /* ENABLE_OTA */
}

/**
 * @fn void _setWiFiState(enum _wifi_state_machine)
 * @brief Set the state of the wifi connection Connected/Disconnected/None/Provisioning and advertise change of state
 *
 * @param state
 */
static void _setWiFiState(enum _wifi_state_machine newState)
{
    xSemaphoreTake(s_WiFiMutex, portMAX_DELAY);

    if (s_WiFiState != newState)
    {
        s_WiFiState = newState;

        /* Advertise change of state */
        if (newState == kWiFi_State_Connected)
        {
            _advertiseWiFiConnectionStatus(true);
        }
        else if (newState != kWiFi_State_Provisioning)
        {
            _advertiseWiFiConnectionStatus(false);
        }
    }

    xSemaphoreGive(s_WiFiMutex);
}

#if ENABLE_FTP_CLIENT
/**
 * @fn status_t ___getFTPServerInfo(char*)
 * @brief Function that converts the ftp server ip and port into a readable string
 *
 * @param valueToString Buffer in which the value is stringify.
 * @return kStatus_Success if the conversion operation was a success
 */
static status_t _getFTPServerInfo(char *valueToString)
{
    status_t status = kStatus_Success;
    if (valueToString == NULL)
    {
        status = kStatus_Fail;
    }
    else
    {
        ftp_info_t ftpInfo;
        if (FTP_GetInfo(&ftpInfo) == kStatus_Success)
        {
            char port[6];
            net_inet_ntoa(ftpInfo.serverInfo.serverIP, valueToString);
            strcat(valueToString, "/");
            itoa(ftpInfo.serverInfo.serverPort, port, 10);
            strcat(valueToString, port);
        }
        else
        {
            strcpy(valueToString, "0.0.0.0/0");
        }
    }
    return status;
}
#endif /* ENABLE_FTP_CLIENT */

/**
 * @fn status_t __getNetworkPassword(char*)
 * @brief Function that converts the network password into a string.
 *
 * @param valueToString Buffer in which the value is stringify.
 * @return kStatus_Success if the conversion operation was a success
 */
static status_t _getNetworkPassword(char *valueToString)
{
    status_t status = kStatus_Success;
    if (valueToString == NULL)
    {
        status = kStatus_Fail;
    }
    else
    {
        wifi_cred_t wifi_cred;
        if (WiFi_GetCredentials(&wifi_cred) == kStatus_Success)
        {
            strcpy(valueToString, (char *)wifi_cred.password.value);
        }
        else
        {
            strcpy(valueToString, "NotSet");
        }
    }
    return status;
}

/**
 * @fn status_t _getNetworkSSID(char*)
 * @brief Function that converts the network SSID into a string.
 *
 * @param valueToString Buffer in which the value is stringify
 * @return kStatus_Success if the conversion operation was a success
 */
static status_t _getNetworkSSID(char *valueToString)
{
    status_t status = kStatus_Success;
    if (valueToString == NULL)
    {
        status = kStatus_Fail;
    }
    else
    {
        wifi_cred_t wifi_cred;
        if (WiFi_GetCredentials(&wifi_cred) == kStatus_Success)
        {
            strcpy(valueToString, (char *)wifi_cred.ssid.value);
        }
        else
        {
            strcpy(valueToString, "NotSet");
        }
    }
    return status;
}

static status_t _getIPAddress(char *valueToString)
{
    status_t status = kStatus_Success;

    if (valueToString == NULL)
    {
        status = kStatus_Fail;
    }
    else if (WPL_GetIP(valueToString, 1) == WPL_ERROR)
    {
        strcpy(valueToString, "0.0.0.0");
    }

    return status;
}

/**
 * @fn void _wifiAsyncConnect(void)
 * @brief Called from WiFi layer if the connection was a success
 * layer
 *
 */
static void _wifiAsyncConnect(void)
{
    _setWiFiState(kWiFi_State_Connected);
}

/**
 * @fn void _wifiAsyncDisconnect(enum void)
 * @brief  Called from WiFi layer if a disconnection occurred
 * layer
 *
 */
static void _wifiAsyncDisconnect(void)
{
    fwk_message_t *pWiFiJoinMsg;
    _setWiFiState(kWiFi_State_DisconnectedAsync);
    pWiFiJoinMsg     = &s_WiFiMessage;
    pWiFiJoinMsg->id = kWiFi_Join;
    FWK_Message_Put(kAppTaskID_WiFi, &pWiFiJoinMsg);
}

/* Not used for now */
/**
 * @fn void _doDeinit(void)
 * @brief  Blocking deinit of the WiFi driver
 *
 */
static void _doDeinit(void)
{
    LOGD("Start deinit.");
    int result = WPL_Stop();
    if (result == WPL_SUCCESS)
    {
        _setWiFiState(kWiFi_State_None);
    }
    else
    {
        LOGE("Failed to deinitialized the WiFi module")
    }
}

/**
 * @fn void _doInit(void)
 * @brief Does a blocking init of the wifi driver.
 *
 */
static void _doInit(void)
{
    LOGD("Start init ");
    sln_wifi_connection_cbs_t cb = {.connected = _wifiAsyncConnect, .disconnected = _wifiAsyncDisconnect};
    int result                   = WPL_Init(cb);

    if (result == WPL_SUCCESS)
    {
        result = WPL_Start();
        if (result == WPL_SUCCESS)
        {
            _setWiFiState(kWiFi_State_Disconnected);
            LOGD("Successfully initialized WiFi module.");
            return;
        }
        else
        {
            LOGE("Failed to start WiFi module.");
        }
    }
    else if (result == WM_E_NOENT)
    {
        _setWiFiState(kWiFi_State_FWNotFound);
    }
    else
    {
        LOGE("Failed to initialized WiFi module.");
    }
}

/**
 * @fn void _doDisconect(void)
 * @brief  Does a blocking disconnect operation
 *
 */
static void _doDisconect(void)
{
    /* Try to disconnect */
    if ((s_WiFiState == kWiFi_State_Connected) || (s_WiFiState == kWiFi_State_DisconnectedAsync))
    {
        LOGD("Start disconnecting. ");
        int result = WPL_Leave();

        if (result == WPL_SUCCESS)
        {
            _setWiFiState(kWiFi_State_Disconnected);
            LOGD("Successfully left the network");
        }
        else
        {
            LOGE("Failed to leave the network");
        }
    }
    else
    {
        LOGI("No network to disconnect from.");
    }
}

/**
 * @fn void _doConnect(wifi_cred_t)
 * @brief Blocking call to try connecting to a WiFi network
 *
 * @param wifiCred WiFi network credentials SSID/ Password
 */
static void _doConnect(wifi_cred_t wifiCred)
{
    /* Try to connect */
    if ((s_WiFiState == kWiFi_State_Disconnected) || (s_WiFiState == kWiFi_State_DisconnectedAsync))
    {
        LOGD("Start connect.");
        int result = WPL_Join((char *)wifiCred.ssid.value, (char *)wifiCred.password.value);

        if (result != WPL_SUCCESS)
        {
            fwk_message_t *pWiFiJoinMsg;
            pWiFiJoinMsg     = &s_WiFiMessage;
            pWiFiJoinMsg->id = kWiFi_Join;
            FWK_Message_Put(kAppTaskID_WiFi, &pWiFiJoinMsg);
        }
        else
        {
            _setWiFiState(kWiFi_State_Connected);
#if ENABLE_OTA
            vTaskResume(s_otaTaskHandler);
#endif /* ENABLE_OTA */
        }
    }
    else
    {
        LOGD("State is %d no need to do connect.", s_WiFiState);
    }
}

static int _HAL_InputDev_WiFiAWAM510_Init(fwk_task_data_t *arg)
{
    LOGD("Starting WiFi \r\n");

    _doInit();

    if (WiFi_GetState() == kWiFi_On)
    {
        if (s_WiFiState == kWiFi_State_Disconnected)
        {
            fwk_message_t *pWiFiInitMsg;
            pWiFiInitMsg     = &s_WiFiMessage;
            pWiFiInitMsg->id = kWiFi_Join;
            FWK_Message_Put(kAppTaskID_WiFi, &pWiFiInitMsg);
        }
        else if (s_WiFiState != kWiFi_State_FWNotFound)
        {
            return -1;
        }
    }

    if (WiFi_CredentialsPresent())
    {
        if (WiFi_GetCredentials(&s_WiFiCred) != kStatus_Success)
        {
            return -1;
        }
    }
    else
    {
        if (s_WiFiState != kWiFi_State_FWNotFound)
        {
            LOGD("WiFi state is provisioning.");
            _setWiFiState(kWiFi_State_Provisioning);
        }
    }

    return 0;
}

static void _HAL_InputDev_WiFiAWAM510_MessageHandler(fwk_message_t *pMsg, fwk_task_data_t *pTaskData)
{
    if (pMsg == NULL)
        return;

    if (s_WiFiState == kWiFi_State_FWNotFound)
    {
        LOGE("WiFi fw not found, load the firmware");
        return;
    }

    switch (pMsg->id)
    {
        case kWiFi_Join:
        {
            if (WiFi_GetState() == kWiFi_On)
            {
                _doConnect(s_WiFiCred);
            }
            else
            {
                LOGD("WiFi state is off");
            }
        }
        break;
        case kWiFi_Leave:
        {
            _doDisconect();
        }
        break;
        case kWiFi_Credentials:
        {
            if (WiFi_CredentialsPresent())
            {
                if (WiFi_GetCredentials(&s_WiFiCred) != kStatus_Success)
                {
                    LOGD("Failed to get the WiFi credentials");
                }
                else if (s_WiFiState == kWiFi_State_Provisioning)
                {
                    _setWiFiState(kWiFi_State_Disconnected);
                    if (WiFi_GetState() == kWiFi_On)
                    {
                        _doConnect(s_WiFiCred);
                    }
                }
            }
            else
            {
                LOGD("No WiFi credentials. Maybe erased.");

                _doDisconect();
                _setWiFiState(kWiFi_State_Provisioning);
            }
        }
        break;
        case kWiFi_Reset:
        {
            if (WiFi_GetState() == kWiFi_On)
            {
                LOGD("Try reset ...");

                _doDisconect();
                _doConnect(s_WiFiCred);

                LOGD("Reset done.");
            }
            else
            {
                LOGD("WiFi state is off");
            }
        }
        break;
        case kWiFi_StateChange:
        {
            if (WiFi_GetState() == kWiFi_Off)
            {
                _doDisconect();
            }
            else if (WiFi_GetState() == kWiFi_On)
            {
                _doConnect(s_WiFiCred);
            }
        }
        break;
        case kWiFi_Scan:
        {
            char *scan_result;
            if (pMsg->payload.data != NULL)
            {
                int (*s_DelayResponseCallback)(uint32_t, void *, event_status_t, unsigned char) = pMsg->payload.data;
                s_DelayResponseCallback(kEventID_WiFiScan, (void *)1, kEventStatus_NonBlocking, false);
                if (WPL_Scan() == 0)
                {
                    scan_result = WPL_getSSIDs();
                    s_DelayResponseCallback(kEventID_WiFiScan, scan_result, kEventStatus_Ok, true);
                }
                else
                {
                    s_DelayResponseCallback(kEventID_WiFiScan, (void *)1, kEventStatus_Error, true);
                }
            }
            else
            {
                WPL_Scan();
            }
        }
        break;

#if ENABLE_FTP_CLIENT
        case kWiFi_FTPClient:
        {
            if (s_WiFiState == kWiFi_State_Connected)
            {
                ftp_session_handle_t handle = FTP_ConnectBlocking();
                if (handle != NULL)
                {
                    char remote_path[25];
                    char current_time[8];
                    h264_result_t result = *(h264_result_t *)pMsg->raw.data;
                    itoa(FWK_CurrentTimeUs() / 1000, current_time, 10);
                    strcpy(remote_path, "VIZN3D/rec");
                    strcat(remote_path, current_time);
                    strcat(remote_path, ".h264");
                    FTP_StoreBlocking(handle, remote_path, (char *)result.recordedDataAddress, result.recordedDataSize);
                    FTP_DisconnectBlocking(handle);
                }
                else
                {
                    LOGE("Not connected to the server. Can't start FTP client.");
                }
            }
            else
            {
                LOGD("Not connected to a network.");
            }
        }
        break;
#endif /* ENABLE_FTP_CLIENT */

        default:
            break;
    }
}

static hal_input_status_t HAL_InputDev_WiFiAWAM510_Init(input_dev_t *dev, input_dev_callback_t callback)
{
    hal_input_status_t error = kStatus_HAL_InputSuccess;

    s_InputDev_WiFiAWAM510.cap.callback = callback;

    if (WiFi_LoadCredentials() == kStatus_Success)
    {
        s_WiFiEvents = xEventGroupCreate();

        if (s_WiFiEvents == NULL)
        {
            LOGE("Failed to create s_WiFiEvents.");
            error = kStatus_HAL_InputError;
        }

        if (error == kStatus_HAL_InputSuccess)
        {
            s_WiFiMutex = xSemaphoreCreateBinary();
            if (s_WiFiMutex == NULL)
            {
                LOGE("Failed to create wifi mutex.");
                vEventGroupDelete(s_WiFiEvents);
                s_WiFiEvents = NULL;
                error        = kStatus_HAL_InputError;
            }
            else
            {
                xSemaphoreGive(s_WiFiMutex);
            }
        }
    }
    else
    {
        LOGE("Failed to initialize the WiFi.");
        error = kStatus_HAL_InputError;
    }
#if ENABLE_FTP_CLIENT
    if (error == kStatus_HAL_InputSuccess)
    {
        if (FTP_Init() != kStatus_Success)
        {
            LOGE("Failed to init FTP.");
            vEventGroupDelete(s_WiFiEvents);
            s_WiFiEvents = NULL;
            error        = kStatus_HAL_InputError;
        }
    }
#endif /* ENABLE_FTP_CLIENT */
#if ENABLE_OTA
    if (error == kStatus_HAL_InputSuccess)
    {
        if (xTaskCreate(ota_task, "RunOtaCoreMqttDemo", democonfigDEMO_STACKSIZE, NULL, (WIFI_TASK_PRIORITY),
                        &s_otaTaskHandler) != pdPASS)
        {
            LOGE("Failed to create ota_task.");
            vEventGroupDelete(s_WiFiEvents);
            error        = kStatus_HAL_InputError;
            s_WiFiEvents = NULL;
        }
    }
#endif /* ENABLE_OTA */

    return error;
}

static hal_input_status_t HAL_InputDev_WiFiAWAM510_Deinit(const input_dev_t *dev)
{
    hal_input_status_t error = kStatus_HAL_InputSuccess;
    return error;
}

static hal_input_status_t HAL_InputDev_WiFiAWAM510_Start(const input_dev_t *dev)
{
    hal_input_status_t error = kStatus_HAL_InputSuccess;
    LOGD("++HAL_InputDev_WiFiAWAM510_start");

    /* Create the main Task */
    s_WiFiTask.task.msgHandle  = _HAL_InputDev_WiFiAWAM510_MessageHandler;
    s_WiFiTask.task.taskInit   = _HAL_InputDev_WiFiAWAM510_Init;
    s_WiFiTask.task.data       = (fwk_task_data_t *)&(s_WiFiTask.data);
    s_WiFiTask.task.taskId     = kAppTaskID_WiFi;
    s_WiFiTask.task.delayMs    = 1;
    s_WiFiTask.task.taskStack  = s_WiFiTaskStack;
    s_WiFiTask.task.taskBuffer = s_WiFiTaskTCBBReference;
    FWK_Task_Start((fwk_task_t *)&s_WiFiTask, WIFI_TASK_NAME, WIFI_TASK_STACK, WIFI_TASK_PRIORITY);

    LOGD("--HAL_InputDev_WiFiAWAM510_start");
    return error;
}

static hal_input_status_t HAL_InputDev_WiFiAWAM510_Stop(const input_dev_t *dev)
{
    hal_input_status_t error = 0;
    LOGD("++HAL_InputDev_WiFiAWAM510_stop");

    LOGD("--HAL_InputDev_WiFiAWAM510_stop");
    return error;
}

static hal_input_status_t HAL_InputDev_WiFiAWAM510_InputNotify(const input_dev_t *dev, void *param)
{
    hal_input_status_t error = kStatus_HAL_InputSuccess;
    event_base_t eventBase   = *(event_base_t *)param;
    switch (eventBase.eventId)
    {
        case kEventID_WiFiSetCredentials:
        {
            wifi_event_t wifiEvent     = ((event_common_t *)param)->wifi;
            event_status_t eventStatus = kEventStatus_Ok;
            if (WiFi_SetCredentials(&wifiEvent.wifiCred) == kStatus_Success)
            {
                fwk_message_t *pWiFiSetCredMsg = pvPortMalloc(sizeof(fwk_message_t));
                if (pWiFiSetCredMsg != NULL)
                {
                    pWiFiSetCredMsg->id                = kWiFi_Credentials;
                    pWiFiSetCredMsg->freeAfterConsumed = true;
                    FWK_Message_Put(kAppTaskID_WiFi, &pWiFiSetCredMsg);
                }
                else
                {
                    LOGE("Failed to allocate memory for message pWiFiSetCredMsg");
                    eventStatus = kEventStatus_Error;
                }
            }
            else
            {
                LOGE("Failed to set credentials");
                eventStatus = kEventStatus_Error;
            }

            if (eventBase.respond != NULL)
            {
                eventBase.respond(eventBase.eventId, &wifiEvent, eventStatus, true);
            }
        }
        break;
        case kEventID_WiFiGetCredentials:
        {
            wifi_event_t wifiEvent = ((event_common_t *)param)->wifi;
            status_t status        = WiFi_GetCredentials(&wifiEvent.wifiCred);

            if (kStatus_Success == status)
            {
                LOGD("WiFi Credentials SSID %s PASS %s.", wifiEvent.wifiCred.ssid.value,
                     wifiEvent.wifiCred.password.value);
            }
            else
            {
                LOGE("Failed to get credentials");
            }

            if (eventBase.respond != NULL)
            {
                if (kStatus_Success == status)
                {
                    eventBase.respond(eventBase.eventId, &wifiEvent, kEventStatus_Ok, true);
                }
                else
                {
                    eventBase.respond(eventBase.eventId, &wifiEvent, kEventStatus_Error, true);
                }
            }
        }
        break;
        case kEventID_WiFiEraseCredentials:
        {
            wifi_event_t wifiEvent     = ((event_common_t *)param)->wifi;
            status_t status            = WiFi_EraseCredentials();
            event_status_t eventStatus = kEventStatus_Ok;

            if (status == kStatus_Success)
            {
                LOGD("WiFi Credentials erased successfully");
                fwk_message_t *pWiFiEraseCredMsg = pvPortMalloc(sizeof(fwk_message_t));
                if (pWiFiEraseCredMsg != NULL)
                {
                    pWiFiEraseCredMsg->id                = kWiFi_Credentials;
                    pWiFiEraseCredMsg->freeAfterConsumed = true;
                    FWK_Message_Put(kAppTaskID_WiFi, &pWiFiEraseCredMsg);
                }
                else
                {
                    LOGE("Failed to allocate memory for message pWiFiEraseCredMsg");
                    eventStatus = kEventStatus_Error;
                }
            }
            else
            {
                LOGE("Failed to erase credentials");
                eventStatus = kEventStatus_Error;
            }

            if (eventBase.respond != NULL)
            {
                eventBase.respond(eventBase.eventId, &wifiEvent, eventStatus, true);
            }
        }
        break;
        case kEventID_WiFiScan:
        {
            wifi_event_t wifiEvent          = ((event_common_t *)param)->wifi;
            fwk_message_t *pWiFiScanCredMsg = pvPortMalloc(sizeof(fwk_message_t));
            if (pWiFiScanCredMsg != NULL)
            {
                pWiFiScanCredMsg->id                = kWiFi_Scan;
                pWiFiScanCredMsg->freeAfterConsumed = true;
                pWiFiScanCredMsg->payload.data      = (void *)eventBase.respond;
                FWK_Message_Put(kAppTaskID_WiFi, &pWiFiScanCredMsg);
            }
            else
            {
                LOGE("Failed to to allocate memory for message wifiScan");
                if (eventBase.respond != NULL)
                {
                    eventBase.respond(eventBase.eventId, &wifiEvent, kEventStatus_Error, true);
                }
            }
        }
        break;
        case kEventID_WiFiSetState:
        {
            wifi_event_t wifiEvent     = ((event_common_t *)param)->wifi;
            status_t status            = kStatus_Success;
            event_status_t eventStatus = kEventStatus_Ok;

            if (WiFi_GetState() != wifiEvent.state)
            {
                status = WiFi_SetState(wifiEvent.state);

                if (status == kStatus_Success)
                {
                    fwk_message_t *pWiFiSetStateMsg = pvPortMalloc(sizeof(fwk_message_t));
                    if (pWiFiSetStateMsg != NULL)
                    {
                        pWiFiSetStateMsg->id                = kWiFi_StateChange;
                        pWiFiSetStateMsg->freeAfterConsumed = true;
                        FWK_Message_Put(kAppTaskID_WiFi, &pWiFiSetStateMsg);
                        LOGD("WiFi state set with success");
                    }
                    else
                    {
                        LOGE("Failed to allocate memory for message pWiFiSetStateMsg");
                        eventStatus = kEventStatus_Error;
                    }
                }
                else
                {
                    LOGE("WiFi set state failed");
                    eventStatus = kEventStatus_Error;
                }
            }

            if (eventBase.respond != NULL)
            {
                eventBase.respond(eventBase.eventId, &wifiEvent, eventStatus, true);
            }
        }
        break;
        case kEventID_WiFiGetState:
        {
            wifi_event_t wifiEvent = ((event_common_t *)param)->wifi;
            wifiEvent.state        = WiFi_GetState();

            LOGD("WiFi state is %s.", (wifiEvent.state == kWiFi_On) ? "On" : "Off");
            if (eventBase.respond != NULL)
            {
                eventBase.respond(eventBase.eventId, &wifiEvent, kEventStatus_Ok, true);
            }
        }
        break;
        case kEventID_WiFiReset:
        {
            fwk_message_t *pWiFiResetMsg = pvPortMalloc(sizeof(fwk_message_t));
            if (pWiFiResetMsg != NULL)
            {
                pWiFiResetMsg->id                = kWiFi_Reset;
                pWiFiResetMsg->freeAfterConsumed = true;
                FWK_Message_Put(kAppTaskID_WiFi, &pWiFiResetMsg);
            }
            else
            {
                LOGE("Failed to allocate memory for message pWiFiResetMsg");
            }
        }
        break;

        case kEventID_WiFiGetIP:
        {
            wifi_event_t wifiEvent = ((event_common_t *)param)->wifi;
            char ip[20];
            _getIPAddress(ip);
            wifiEvent.ip = ip;

            LOGD("WiFi IP is %s", ip);

            if (eventBase.respond != NULL)
            {
                eventBase.respond(eventBase.eventId, &wifiEvent, kEventStatus_Ok, true);
            }
        }
        break;

#ifdef ENABLE_FTP_CLIENT
        case kEventID_FTPSetServerInfo:
        {
            wifi_event_t wifiEvent     = ((event_common_t *)param)->wifi;
            status_t status            = FTP_SetServerInfo(wifiEvent.ftpServerInfo);
            event_status_t eventStatus = kEventStatus_Ok;

            if (status != kStatus_Success)
            {
                eventStatus = kEventStatus_Error;
            }

            if (eventBase.respond != NULL)
            {
                eventBase.respond(eventBase.eventId, &wifiEvent, eventStatus, true);
            }
        }
        break;
        case kEventID_FTPGetServerInfo:
        {
            char serverInfo[25];
            wifi_event_t wifiEvent = ((event_common_t *)param)->wifi;

            if (_getFTPServerInfo(serverInfo) == kStatus_Success)
            {
                wifiEvent.ftpServerInfoSerialized = serverInfo;
            }

            if (eventBase.respond != NULL)
            {
                eventBase.respond(eventBase.eventId, &wifiEvent, kEventStatus_Ok, true);
            }
        }
        break;
        case kEventID_FTPSetServerIP:
        {
            wifi_event_t wifiEvent     = ((event_common_t *)param)->wifi;
            status_t status            = FTP_SetServerIP(wifiEvent.ftpServerInfo.serverIP);
            event_status_t eventStatus = kEventStatus_Ok;

            if (status != kStatus_Success)
            {
                eventStatus = kEventStatus_Error;
            }

            if (eventBase.respond != NULL)
            {
                eventBase.respond(eventBase.eventId, &wifiEvent, eventStatus, true);
            }
        }
        break;
        case kEventID_FTPGetServerIP:
        {
            ftp_info_t ftpInfo;
            char ip[20];
            wifi_event_t wifiEvent     = ((event_common_t *)param)->wifi;
            status_t status            = FTP_GetInfo(&ftpInfo);
            event_status_t eventStatus = kEventStatus_Ok;

            if (status != kStatus_Success)
            {
                eventStatus = kEventStatus_Error;
            }
            else
            {
                net_inet_ntoa(ftpInfo.serverInfo.serverIP, ip);
                wifiEvent.ip = ip;
            }

            if (eventBase.respond != NULL)
            {
                eventBase.respond(eventBase.eventId, &wifiEvent, eventStatus, true);
            }
        }
        break;

        case kEventID_FTPSetServerPort:
        {
            wifi_event_t wifiEvent     = ((event_common_t *)param)->wifi;
            status_t status            = FTP_SetServerPort(wifiEvent.ftpServerInfo.serverPort);
            event_status_t eventStatus = kEventStatus_Ok;
            if (status != kStatus_Success)
            {
                eventStatus = kEventStatus_Error;
            }

            if (eventBase.respond != NULL)
            {
                eventBase.respond(eventBase.eventId, &wifiEvent, eventStatus, true);
            }
        }
        break;
        case kEventID_FTPGetServerPort:
        {
            ftp_info_t ftpInfo;
            wifi_event_t wifiEvent     = ((event_common_t *)param)->wifi;
            status_t status            = FTP_GetInfo(&ftpInfo);
            event_status_t eventStatus = kEventStatus_Ok;
            if (status != kStatus_Success)
            {
                eventStatus = kEventStatus_Error;
            }
            else
            {
                wifiEvent.ftpServerInfo.serverIP = ftpInfo.serverInfo.serverPort;
            }

            if (eventBase.respond != NULL)
            {
                eventBase.respond(eventBase.eventId, &wifiEvent, eventStatus, true);
            }
        }
        break;
        case kEventID_FTPSetServerAuth:
        {
            /* TODO add support for auth */
        }
        break;
        case kEventID_FTPGetServerAuth:
        {
        }
        break;
#endif /* ENABLE_FTP_CLIENT */
        default:
            break;
    }
    return error;
}

static hal_output_status_t HAL_OutputDev_WiFiAWAM510_Init(output_dev_t *dev, output_dev_callback_t callback)
{
    hal_output_status_t status = kStatus_HAL_OutputSuccess;

    /* TODO */
    dev->cap.callback = callback;

    return status;
}
static hal_output_status_t HAL_OutputDev_WiFiAWAM510_Deinit(const output_dev_t *dev)
{
    hal_output_status_t status = kStatus_HAL_OutputSuccess;
    /* TODO */
    return status;
}

static hal_output_status_t HAL_OutputDev_WiFiAWAM510_Stop(const output_dev_t *dev)
{
    hal_output_status_t status = kStatus_HAL_OutputSuccess;
    /* TODO */
    return status;
}

static hal_output_status_t HAL_OutputDev_WiFiAWAM510_Start(const output_dev_t *dev)
{
    hal_output_status_t status = kStatus_HAL_OutputSuccess;
    if (FWK_OutputManager_RegisterEventHandler(dev, &s_OutputDev_WiFiAWAM510Handler) != 0)
        status = kStatus_HAL_OutputError;
    return status;
}

static hal_output_status_t HAL_OutputDev_WiFiAWAM510_InferenceComplete(const output_dev_t *dev,
                                                                       output_algo_source_t source,
                                                                       void *inferResult)
{
    hal_output_status_t status = kStatus_HAL_OutputSuccess;
    /* TODO */

    static recording_state_t s_LastRecordingState = kRecordingState_Start;
    vision_algo_result_t *visionAlgoResult        = (vision_algo_result_t *)inferResult;
    recording_state_t recordingState;

    if (visionAlgoResult != NULL)
    {
#ifdef ENABLE_FTP_CLIENT
        if (visionAlgoResult->id == kVisionAlgoID_H264Recording)
        {
            recordingState = (visionAlgoResult->h264Recording.state);
            if ((recordingState == kRecordingState_Stop) && (s_LastRecordingState == kRecordingState_Start))
            {
                fwk_message_t *pWiFiStartFTPClient = pvPortMalloc(sizeof(fwk_message_t));
                if (pWiFiStartFTPClient != NULL)
                {
                    static h264_result_t result;
                    result                                     = visionAlgoResult->h264Recording;
                    pWiFiStartFTPClient->id                    = kWiFi_FTPClient;
                    pWiFiStartFTPClient->freeAfterConsumed     = true;
                    pWiFiStartFTPClient->raw.data              = &result;
                    pWiFiStartFTPClient->raw.freeAfterConsumed = false;
                    FWK_Message_Put(kAppTaskID_WiFi, &pWiFiStartFTPClient);
                }
                else
                {
                    LOGE("Failed to allocate memory for Start FTP client message");
                }
            }
            s_LastRecordingState = recordingState;
        }
#endif /* ENABLE_FTP_CLIENT */
    }

    return status;
}
static hal_output_status_t HAL_OutputDev_WiFiAWAM510_InputNotify(const output_dev_t *dev, void *param)
{
    hal_output_status_t status = kStatus_HAL_OutputSuccess;
    return status;
}

int HAL_OutputDev_WiFiAWAM510_Register(void)
{
    int error = 0;
    error     = FWK_OutputManager_DeviceRegister(&s_OutputDev_WiFiAWAM510);
    return error;
}

int HAL_InputDev_WiFiAWAM510_Register(void)
{
    int error = 0;
    error     = FWK_InputManager_DeviceRegister(&s_InputDev_WiFiAWAM510);
    return error;
}

int HAL_WiFiAWAM510_Register()
{
    int error = 0;

    error = HAL_InputDev_WiFiAWAM510_Register();
    error += HAL_OutputDev_WiFiAWAM510_Register();

    return error;
}
#endif /* ENABLE_INPUT_DEV_WiFiAWAM510 */
