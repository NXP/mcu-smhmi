/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board_define.h"
#ifdef ENABLE_AUDIO_PROCESSING_DEV_Offline
#include "stdint.h"
#include <limits.h>

#include "fwk_audio_processing.h"
#include "fwk_log.h"
#include "fwk_platform.h"
#include "hal_audio_processing_dev.h"
#include "hal_audio_defs.h"
#include "hal_event_descriptor_voice.h"

#include "fwk_output_manager.h"
#include "fwk_lpm_manager.h"
#include "hal_output_dev.h"
#include "hal_voice_algo_asr_local.h"

#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "composite.h"
#include "virtual_com.h"
//#include "sln_afe.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define AUIDO_PROCESSING_OFFLINE_TASK_NAME     "AFE_OFFLINE"
#define AUIDO_PROCESSING_OFFLINE_TASK_STACK    1024
#define AUIDO_PROCESSING_OFFLINE_TASK_PRIORITY 2

#define AFE_PCM_SAMPLE_SIZE_BYTES (2) /* 16bit depth */
#define AFE_BLOCKS_TO_ACCUMULATE  (3)

#define AFE_AUDIO_USB_HEADER_LENGTH     24
#define AFE_AUDIO_USB_MAX_PACKET_LENGTH 960

/* Line coding of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_lineCoding[LINE_CODING_SIZE] = {
    /* E.g. 0x00,0xC2,0x01,0x00 : 0x0001C200 is 115200 bits per second */
    (LINE_CODING_DTERATE >> 0U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 8U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 16U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 24U) & 0x000000FFU,
    LINE_CODING_CHARFORMAT,
    LINE_CODING_PARITYTYPE,
    LINE_CODING_DATABITS};

/* Abstract state of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_abstractState[COMM_FEATURE_DATA_SIZE] = {(STATUS_ABSTRACT_STATE >> 0U) & 0x00FFU,
                                                          (STATUS_ABSTRACT_STATE >> 8U) & 0x00FFU};

/* Country code of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_countryCode[COMM_FEATURE_DATA_SIZE] = {(COUNTRY_SETTING >> 0U) & 0x00FFU,
                                                        (COUNTRY_SETTING >> 8U) & 0x00FFU};

/* CDC ACM information */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static usb_cdc_acm_info_t s_usbCdcAcmInfo = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, 0,
};

/* Data buffer for receiving and sending*/
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_currRecvBuf[DATA_BUFF_SIZE];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_currSendBuf[DATA_BUFF_SIZE];

AT_CACHEABLE_SECTION_ALIGN_OCRAM(static int16_t s_audioOut[AUDIO_PCM_SINGLE_CH_SMPL_COUNT * AFE_BLOCKS_TO_ACCUMULATE],
                                 4);

extern usb_device_endpoint_struct_t g_cdcVcomDicEndpoints_2[];
extern usb_device_endpoint_struct_t g_cdcVcomCicEndpoints_2[];
extern usb_device_class_struct_t g_UsbDeviceCdcVcomConfig[];

typedef enum _hal_afe_audio_ack_reserved_t
{
    AFE_AUDIO_USB_ACK_SUCCESS   = 0,
    AFE_AUDIO_USB_ACK_DUPLICATE = 1,
    AFE_AUDIO_USB_ACK_ERROR     = -1, // 0xffffffffff
} hal_afe_audio_ack_reserved_t;

typedef enum _hal_transfer_packet_type_t
{
    AFE_OFFLINE_START_REQ = 0,
    AFE_OFFLINE_START_RES,
    AFE_OFFLINE_SAMPLE_REQ,
    AFE_OFFLINE_SAMPLE_RES,
    AFE_OFFLINE_END_REQ,
    AFE_OFFLINE_END_RES,
    FIRMWARE_RESPONSE = 0xff,
} hal_afe_audio_transfer_packet_type_t;

typedef enum _hal_afe_audio_usb_packet_t
{
    kHALAFEAUDIOUSBPacket_Wait,
    kHALAFEAUDIOUSBPacket_Header,
    kHALAFEAUDIOUSBPacket_Data,
} hal_afe_audio_usb_packet_t;

typedef enum _hal_afe_audio_usb_status_t
{
    kHALAFEAUDIOUSBStatus_Success,
    kHALAFEAUDIOUSBStatus_TUMagicError,
    kHALAFEAUDIOUSBStatus_TUCRC32Error,
    kHALAFEAUDIOUSBStatus_PacketCRC32Error,
    kHALAFEAUDIOUSBStatus_PacketValid,
    kHALAFEAUDIOUSBStatus_PacketError,
    kHALAFEAUDIOUSBStatus_PacketShort,
} hal_afe_audio_usb_status_t;

typedef struct _hal_afe_audio_usb_transfer_t
{
    uint8_t *nibbleBuf;
    uint8_t *packetBuf;
    int nibbleLen;
    int packetLen;
    int readOffset;
    int packetOffset;
    uint8_t stage;
} hal_afe_audio_usb_transfer_t;

typedef struct _hal_header_transfer_unit_t
{
    uint8_t tuMagic[3];
    uint8_t pktType;
    uint32_t pktLen;
    uint32_t pktId;
    uint32_t pktCrc;
    uint32_t reserved;
    uint32_t tuCrc;
} hal_header_transfer_unit_t;

static uint32_t s_pckID;
static uint32_t s_pckIndex;
static uint32_t table[0x100];

static const uint8_t TU_MAGIC[] = {0x53, 0x79, 0x4c};

static serial_usb_cdc_state_t s_UsbDeviceCDC;
static uint8_t s_has_finished = 1;

static uint8_t s_AfeAudioUSBNibbleBuffer[AFE_AUDIO_USB_HEADER_LENGTH];
static uint8_t s_AfeAudioUSBPacketBuffer[AFE_AUDIO_USB_MAX_PACKET_LENGTH];
static uint8_t s_CurAfeAudioToAsr[AFE_AUDIO_USB_MAX_PACKET_LENGTH];
static hal_afe_audio_usb_transfer_t s_AfeAudioUSBTransfer;
static hal_header_transfer_unit_t s_HeaderTransferUnit = {0};
static TaskHandle_t s_AfeAudioTask;

static uint8_t s_pktRes[AFE_AUDIO_USB_HEADER_LENGTH];

#define DATA_START_ADDR 0x30700000
// silence_aqt_NF_yunyang_asr.bin
#define DATA_TOTAL_SIZE 13473600

static int s_totalSamples;
static int s_curSample = 0;
static int16_t *s_pCur = NULL;
static audio_processing_dev_t *p_Dev;

/* Performance Statistics. */
volatile uint32_t s_afeDataProcessed = 0;

static void _VoiceSession_Check();

static _system_reset(void)
{
    __NVIC_SystemReset();
}

static uint32_t crc32_for_byte(uint32_t r)
{
    for (int j = 0; j < 8; ++j)
    {
        r = (r & 1 ? 0 : (uint32_t)0xEDB88320L) ^ r >> 1;
    }

    return r ^ (uint32_t)0xFF000000L;
}

static void crc32(const void *data, size_t numBytes, uint32_t *crc)
{
    memset(table, 0x00, sizeof(table));

    if (!*table)
    {
        for (size_t i = 0; i < 0x100; ++i)
        {
            table[i] = crc32_for_byte(i);
        }
    }

    for (size_t i = 0; i < numBytes; ++i)
    {
        *crc = table[(uint8_t)*crc ^ ((uint8_t *)data)[i]] ^ *crc >> 8;
    }
}

static hal_afe_audio_usb_status_t SLN_AFEAUDIOUSBParseHeader(uint8_t *headerBuf, hal_header_transfer_unit_t *pHtUnit)
{
    hal_afe_audio_usb_status_t status = kHALAFEAUDIOUSBStatus_Success;
    uint32_t crc_res                  = 0;

    /* transfer magic */
    if (memcmp(headerBuf, TU_MAGIC, sizeof(TU_MAGIC)) != 0)
    {
        return kHALAFEAUDIOUSBStatus_TUMagicError;
    }
    memcpy(pHtUnit->tuMagic, headerBuf, 3);

    /* packet type */
    pHtUnit->pktType = headerBuf[3];

    /* packet length */
    pHtUnit->pktLen = *((uint32_t *)(headerBuf + 4));

    /* packet id */
    pHtUnit->pktId = *((uint32_t *)(headerBuf + 8));

    /* packet crc */
    pHtUnit->pktCrc = *((uint32_t *)(headerBuf + 12));

    /* packet reserved */
    pHtUnit->reserved = *((uint32_t *)(headerBuf + 16));

    /* transfer unit crc */
    pHtUnit->tuCrc = *((uint32_t *)(headerBuf + 20));
    crc32(headerBuf, AFE_AUDIO_USB_HEADER_LENGTH - 4, &crc_res);
    if (crc_res != pHtUnit->tuCrc)
    {
        return kHALAFEAUDIOUSBStatus_TUCRC32Error;
    }

    return status;
}

static void SLN_AFEAUDIOUSBCreateHeader(
    uint8_t *htUnit, uint8_t *data, uint32_t len, uint8_t type, uint32_t pktId, uint32_t result)
{
    uint32_t crc_res = 0;

    /* transfer magic */
    memcpy(htUnit, TU_MAGIC, sizeof(TU_MAGIC));

    /* packet type */
    htUnit[3] = type;

    /* packet length */
    *((uint32_t *)(htUnit + 4)) = len;

    /* packet id */
    *((uint32_t *)(htUnit + 8)) = pktId;

    if (len > 0)
    {
        /* packet crc */
        crc32(data, len, &crc_res);
        *((uint32_t *)(htUnit + 12)) = crc_res;
    }
    else
    {
        *((uint32_t *)(htUnit + 12)) = 0;
    }

    /* packet reserved */
    *((uint32_t *)(htUnit + 16)) = result;

    /* transfer unit crc */
    crc_res = 0;
    crc32(htUnit, AFE_AUDIO_USB_HEADER_LENGTH - 4, &crc_res);
    *((uint32_t *)(htUnit + 20)) = crc_res;
}

static hal_afe_audio_usb_status_t SLN_AFEAudioUSBSendPacket(
    uint8_t *data, uint32_t len, uint8_t type, uint32_t pktId, uint32_t result)
{
    hal_afe_audio_usb_status_t status = kHALAFEAUDIOUSBStatus_Success;

    // PRINTF("SLN_AFEAudioUSBSendPacket++\r\n");
    // uint8_t htUnit[AFE_AUDIO_USB_HEADER_LENGTH] = {0};

    SLN_AFEAUDIOUSBCreateHeader(s_currSendBuf, data, len, type, pktId, result);

    status = USB_DeviceCdcAcmSend(s_UsbDeviceCDC.cdcAcmHandle, s_UsbDeviceCDC.bulkInEndpoint, s_currSendBuf,
                                  AFE_AUDIO_USB_HEADER_LENGTH);

    // PRINTF("SLN_AFEAudioUSBSendPacket [%d]--\r\n", status);
    return status;
}

static unsigned int recAfeOfflineSampleCnt = 0;
static unsigned int startTime                     = 0;
static unsigned int endTime                       = 0;

static hal_afe_audio_usb_status_t SLN_AFEAUDIOUSBParseData(uint8_t *dataBuf, hal_header_transfer_unit_t *pHtUnit)
{
    hal_afe_audio_usb_status_t status = kHALAFEAUDIOUSBStatus_Success;
    uint32_t crc_res                  = 0;
    crc32(dataBuf, pHtUnit->pktLen, &crc_res);
    if (crc_res != pHtUnit->pktCrc)
    {
        return kHALAFEAUDIOUSBStatus_TUCRC32Error;
    }

    s_pckID    = pHtUnit->pktId;
    s_pckIndex = pHtUnit->reserved;
    /* handle all kinds of packet type */
    switch (pHtUnit->pktType)
    {
        case AFE_OFFLINE_START_REQ:
        {
            startTime = Time_Current();
            LOGD("\r\n");
            LOGD(":VOICE:CLIP_START:%s:[OK]", dataBuf);
            LOGD("[%d]:Receive start packet [%d]\r\n", startTime, recAfeOfflineSampleCnt);
            recAfeOfflineSampleCnt = 0;
            SLN_AFEAudioUSBSendPacket(NULL, 0, AFE_OFFLINE_START_RES, pHtUnit->pktId, AFE_AUDIO_USB_ACK_SUCCESS);
        }
        break;

        case AFE_OFFLINE_SAMPLE_REQ:
        {
            recAfeOfflineSampleCnt++;
            // notify ASR to process
            memcpy(s_CurAfeAudioToAsr, dataBuf, AFE_AUDIO_USB_MAX_PACKET_LENGTH);
            if (p_Dev->cap.callback != NULL)
            {
                audio_processing_event_t audio_processing = {0};
                uint8_t fromISR                           = __get_IPSR();
                audio_processing.eventId                  = kAudioProcessingEvent_Done;
                audio_processing.eventInfo                = kEventInfo_Local;
                audio_processing.data                     = s_CurAfeAudioToAsr;
                audio_processing.size                     = AUDIO_PCM_SINGLE_CH_SMPL_COUNT * AFE_BLOCKS_TO_ACCUMULATE;
                audio_processing.copy                     = 0;
                p_Dev->cap.callback(p_Dev, audio_processing, fromISR);
            }
        }
        break;

        case AFE_OFFLINE_END_REQ:
        {
            endTime = Time_Current();
            LOGD("[VOICE]:[CLIP_END]:[%s] [OK]", dataBuf);
            LOGD("[%d]:[%d.%d.%d]Receive end packet [%d]\r\n", endTime, (endTime - startTime) / 1000000,
                 ((endTime - startTime) / 1000) % 1000, ((endTime - startTime) % 1000), recAfeOfflineSampleCnt);
            vTaskDelay(pdMS_TO_TICKS(100));
            _system_reset();
        }

        break;
        default:
            break;
    }

    return status;
}

static usb_status_t USB_DeviceCdcVcomCallback(class_handle_t handle, uint32_t event, void *param)
{
    uint32_t len;
    uint8_t *uartBitmap;
    usb_cdc_acm_info_t *acmInfo;
    usb_device_cdc_acm_request_param_struct_t *acmReqParam;
    usb_device_endpoint_callback_message_struct_t *epCbParam;
    usb_status_t error = kStatus_USB_Error;
    acmReqParam        = (usb_device_cdc_acm_request_param_struct_t *)param;
    epCbParam          = (usb_device_endpoint_callback_message_struct_t *)param;

    if (handle != s_UsbDeviceCDC.cdcAcmHandle)
    {
        return error;
    }

    acmInfo = s_UsbDeviceCDC.usbCdcAcmInfo;
    switch (event)
    {
        case kUSB_DeviceCdcEventSendResponse:
        {
            if ((epCbParam->length != 0) && (!(epCbParam->length % s_UsbDeviceCDC.bulkInEndpointMaxPacketSize)))
            {
                /* If the last packet is the size of endpoint, then send also zero-ended packet,
                 ** meaning that we want to inform the host that we do not have any additional
                 ** data, so it can flush the output.
                 */
                error = USB_DeviceCdcAcmSend(handle, s_UsbDeviceCDC.bulkInEndpoint, NULL, 0);
            }
            else if ((1 == s_UsbDeviceCDC.attach) && (1 == s_UsbDeviceCDC.startTransactions))
            {
                if ((epCbParam->buffer != NULL) || ((epCbParam->buffer == NULL) && (epCbParam->length == 0)))
                {
                    s_has_finished = 1;
                }
            }
            else
            {
            }
        }
        break;
        case kUSB_DeviceCdcEventRecvResponse:
        {
            hal_afe_audio_usb_status_t status = kHALAFEAUDIOUSBStatus_PacketValid;
            uint32_t ulStatusRegister         = 0x1;

            if ((1 == s_UsbDeviceCDC.attach) && (1 == s_UsbDeviceCDC.startTransactions))
            {
                if (s_AfeAudioTask != NULL)
                {
                    // PRINTF("Notify: recv[%d] [%p] [%p]\r\n", epCbParam->length, epCbParam->buffer,
                    // s_UsbDeviceCDC.currRecvBuf);
                    BaseType_t xHigherPriorityTaskWoken;
                    xHigherPriorityTaskWoken = pdFALSE;
                    ulStatusRegister         = epCbParam->length;
                    s_UsbDeviceCDC.recvSize  = epCbParam->length;
                    xTaskNotifyIndexedFromISR(s_AfeAudioTask, 0, ulStatusRegister, eSetBits, &xHigherPriorityTaskWoken);
                }
            }
        }
        break;
        case kUSB_DeviceCdcEventSerialStateNotif:
            ((usb_device_cdc_acm_struct_t *)handle)->hasSentState = 0;
            error                                                 = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventSendEncapsulatedCommand:
            break;
        case kUSB_DeviceCdcEventGetEncapsulatedResponse:
            break;
        case kUSB_DeviceCdcEventSetCommFeature:
            if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == acmReqParam->setupValue)
            {
                if (1 == acmReqParam->isSetup)
                {
                    *(acmReqParam->buffer) = s_UsbDeviceCDC.abstractState;
                }
                else
                {
                    *(acmReqParam->length) = 0;
                }
            }
            else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == acmReqParam->setupValue)
            {
                if (1 == acmReqParam->isSetup)
                {
                    *(acmReqParam->buffer) = s_UsbDeviceCDC.countryCode;
                }
                else
                {
                    *(acmReqParam->length) = 0;
                }
            }
            else
            {
            }
            error = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventGetCommFeature:
            if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == acmReqParam->setupValue)
            {
                *(acmReqParam->buffer) = s_UsbDeviceCDC.abstractState;
                *(acmReqParam->length) = COMM_FEATURE_DATA_SIZE;
            }
            else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == acmReqParam->setupValue)
            {
                *(acmReqParam->buffer) = s_UsbDeviceCDC.countryCode;
                *(acmReqParam->length) = COMM_FEATURE_DATA_SIZE;
            }
            else
            {
            }
            error = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventClearCommFeature:
            break;
        case kUSB_DeviceCdcEventGetLineCoding:
            *(acmReqParam->buffer) = s_UsbDeviceCDC.lineCoding;
            *(acmReqParam->length) = LINE_CODING_SIZE;
            error                  = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventSetLineCoding:
        {
            if (1 == acmReqParam->isSetup)
            {
                *(acmReqParam->buffer) = s_UsbDeviceCDC.lineCoding;
            }
            else
            {
                *(acmReqParam->length) = 0;
            }
        }
            error = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventSetControlLineState:
        {
            s_UsbDeviceCDC.usbCdcAcmInfo->dteStatus = acmReqParam->setupValue;
            /* activate/deactivate Tx carrier */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION)
            {
                acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
            }
            else
            {
                acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
            }

            /* activate carrier and DTE. Com port of terminal tool running on PC is open now */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE)
            {
                acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
            }
            /* Com port of terminal tool running on PC is closed now */
            else
            {
                acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
            }

            /* Indicates to DCE if DTE is present or not */
            acmInfo->dtePresent = (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) ? true : false;

            /* Initialize the serial state buffer */
            acmInfo->serialStateBuf[0] = NOTIF_REQUEST_TYPE;                /* bmRequestType */
            acmInfo->serialStateBuf[1] = USB_DEVICE_CDC_NOTIF_SERIAL_STATE; /* bNotification */
            acmInfo->serialStateBuf[2] = 0x00;                              /* wValue */
            acmInfo->serialStateBuf[3] = 0x00;
            acmInfo->serialStateBuf[4] = 0x00; /* wIndex */
            acmInfo->serialStateBuf[5] = 0x00;
            acmInfo->serialStateBuf[6] = UART_BITMAP_SIZE; /* wLength */
            acmInfo->serialStateBuf[7] = 0x00;
            /* Notify to host the line state */
            acmInfo->serialStateBuf[4] = acmReqParam->interfaceIndex;
            /* Lower byte of UART BITMAP */
            uartBitmap    = (uint8_t *)&acmInfo->serialStateBuf[NOTIF_PACKET_SIZE + UART_BITMAP_SIZE - 2];
            uartBitmap[0] = acmInfo->uartState & 0xFFu;
            uartBitmap[1] = (acmInfo->uartState >> 8) & 0xFFu;
            len           = (uint32_t)(NOTIF_PACKET_SIZE + UART_BITMAP_SIZE);
            if (0 == ((usb_device_cdc_acm_struct_t *)handle)->hasSentState)
            {
                error = USB_DeviceCdcAcmSend(handle, s_UsbDeviceCDC.interruptEndpoint, acmInfo->serialStateBuf, len);
                if (kStatus_USB_Success != error)
                {
                    usb_echo("kUSB_DeviceCdcEventSetControlLineState error!");
                }
                ((usb_device_cdc_acm_struct_t *)handle)->hasSentState = 1;
            }

            /* Update status */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION)
            {
                /*  To do: CARRIER_ACTIVATED */
            }
            else
            {
                /* To do: CARRIER_DEACTIVATED */
            }
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE)
            {
                /* DTE_ACTIVATED */
                if (1 == s_UsbDeviceCDC.attach)
                {
                    s_UsbDeviceCDC.startTransactions = 1;
                }
            }
            else
            {
                /* DTE_DEACTIVATED */
                if (1 == s_UsbDeviceCDC.attach)
                {
                    s_has_finished                   = 1;
                    s_UsbDeviceCDC.startTransactions = 0;
                }
            }
        }
        break;
        case kUSB_DeviceCdcEventSendBreak:
            break;
        default:
            break;
    }

    return error;
}

/*!
 * @brief Virtual COM device set configuration function.
 * This function sets configuration for CDC class.
 * @param handle The CDC ACM class handle.
 * @param configure The CDC ACM class configure index.
 * @return A USB error code or kStatus_USB_Success.
 */
static usb_status_t USB_DeviceCdcVcomSetConfigure(class_handle_t handle, uint8_t configure)
{
    if (USB_COMPOSITE_CONFIGURE_INDEX == configure)
    {
        /*endpoint information for cdc 1*/
        s_UsbDeviceCDC.attach = 1;

        s_UsbDeviceCDC.interruptEndpoint              = USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT_2;
        s_UsbDeviceCDC.interruptEndpointMaxPacketSize = g_cdcVcomCicEndpoints_2[0].maxPacketSize;

        s_UsbDeviceCDC.bulkInEndpoint              = USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT_2;
        s_UsbDeviceCDC.bulkInEndpointMaxPacketSize = g_cdcVcomDicEndpoints_2[0].maxPacketSize;

        s_UsbDeviceCDC.bulkOutEndpoint              = USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT_2;
        s_UsbDeviceCDC.bulkOutEndpointMaxPacketSize = g_cdcVcomDicEndpoints_2[1].maxPacketSize;

        /* Schedule buffer for receive */
        USB_DeviceCdcAcmRecv(s_UsbDeviceCDC.cdcAcmHandle, s_UsbDeviceCDC.bulkOutEndpoint, s_currRecvBuf,
                             s_UsbDeviceCDC.bulkOutEndpointMaxPacketSize);
    }
    return kStatus_USB_Success;
}

/*!
 * @brief Virtual COM device initialization function.
 * This function initializes the device with the composite device class information.
 * @param deviceComposite The pointer to the composite device structure.
 * @return A USB error code or kStatus_USB_Success.
 */
static usb_status_t USB_DeviceCdcVcomInit()
{
    s_UsbDeviceCDC.instance      = CONTROLLER_ID;
    s_UsbDeviceCDC.lineCoding    = (uint8_t *)s_lineCoding;
    s_UsbDeviceCDC.abstractState = (uint8_t *)s_abstractState;
    s_UsbDeviceCDC.countryCode   = (uint8_t *)s_countryCode;
    s_UsbDeviceCDC.usbCdcAcmInfo = &s_usbCdcAcmInfo;
    s_UsbDeviceCDC.currRecvBuf   = (uint8_t *)&s_currRecvBuf[0];

    return kStatus_USB_Success;
}

static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Success;
    uint8_t *temp8     = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceClassInit:
        {
            s_UsbDeviceCDC.deviceHandle = handle;
            s_UsbDeviceCDC.cdcAcmHandle = (class_handle_t)param;
            USB_DeviceCdcVcomInit();
        }
        break;
        case kUSB_DeviceEventBusReset:
        {
            error                               = kStatus_USB_Success;
            s_UsbDeviceCDC.attach               = 0;
            s_UsbDeviceCDC.currentConfiguration = 0U;
            s_UsbDeviceCDC.recvSize             = 0;
            s_UsbDeviceCDC.sendSize             = 0;
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (0U == (*temp8))
            {
                s_UsbDeviceCDC.attach               = 0U;
                s_UsbDeviceCDC.currentConfiguration = 0U;
                s_UsbDeviceCDC.recvSize             = 0;
                s_UsbDeviceCDC.sendSize             = 0;
            }
            else if (USB_COMPOSITE_CONFIGURE_INDEX == (*temp8))
            {
                USB_DeviceCdcVcomSetConfigure(s_UsbDeviceCDC.cdcAcmHandle, *temp8);
                s_UsbDeviceCDC.currentConfiguration = *temp8;
                error                               = kStatus_USB_Success;
            }
            else
            {
                error = kStatus_USB_InvalidRequest;
            }
            break;
        case kUSB_DeviceEventSetInterface:
            error = kStatus_USB_Success;
            break;
        case kUSB_DeviceEventGetConfiguration:
            error = kStatus_USB_Success;
            break;
        case kUSB_DeviceEventGetInterface:
            error = kStatus_USB_Success;
            break;
        case kUSB_DeviceEventGetDeviceDescriptor:
            error = kStatus_USB_Success;
            break;
        case kUSB_DeviceEventGetConfigurationDescriptor:
            error = kStatus_USB_Success;
            break;
#if (defined(USB_DEVICE_CONFIG_CV_TEST) && (USB_DEVICE_CONFIG_CV_TEST > 0U))
        case kUSB_DeviceEventGetDeviceQualifierDescriptor:
            if (param)
            {
                /* Get device descriptor request */
                error = USB_DeviceGetDeviceQualifierDescriptor(
                    handle, (usb_device_get_device_qualifier_descriptor_struct_t *)param);
            }
            break;
#endif
        case kUSB_DeviceEventGetStringDescriptor:
            error = kStatus_USB_Success;
            break;
        default:
            break;
    }

    return error;
}

static void _SimTimer_Callback(TimerHandle_t xTimer)
{
    static int delay = 0;
    if (delay < 20)
    {
        delay++;
        return;
    }

    unsigned int sampleBytes = AUDIO_PCM_SINGLE_CH_SMPL_COUNT * AFE_PCM_SAMPLE_SIZE_BYTES * AFE_BLOCKS_TO_ACCUMULATE;

    if (s_curSample < s_totalSamples)
    {
        // read one sample
        memcpy(s_audioOut, (uint8_t *)s_pCur, sampleBytes);

        // notify ASR to process
        if (p_Dev->cap.callback != NULL)
        {
            audio_processing_event_t audio_processing = {0};
            uint8_t fromISR                           = __get_IPSR();
            audio_processing.eventId                  = kAudioProcessingEvent_Done;
            audio_processing.eventInfo                = kEventInfo_Local;
            audio_processing.data                     = s_audioOut;
            audio_processing.size                     = AUDIO_PCM_SINGLE_CH_SMPL_COUNT * AFE_BLOCKS_TO_ACCUMULATE;
            audio_processing.copy                     = 0;
            p_Dev->cap.callback(p_Dev, audio_processing, fromISR);
        }
        s_curSample++;
        s_pCur += 480;
    }
}

static void _SimTimer_Start()
{
    static TimerHandle_t s_simTimer = NULL;
    if (s_simTimer == NULL)
    {
        // create the timer
        s_simTimer = xTimerCreate("SimTimer", (TickType_t)pdMS_TO_TICKS(20), pdTRUE, NULL,
                                  (TimerCallbackFunction_t)_SimTimer_Callback);
        if (s_simTimer == NULL)
        {
            LOGE("[UI] Failed to create \"SimTimer\" timer.");
            return;
        }
    }

    if (xTimerStart(s_simTimer, 0) != pdPASS)
    {
        LOGE("[UI] Failed to start \"SimTimer\" timer.");
    }
}

hal_audio_processing_status_t audio_processing_init(audio_processing_dev_t *dev,
                                                    audio_processing_dev_callback_t callback)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;

    dev->cap.callback = callback;

    if (USB_DeviceApplicationInit())
    {
        LOGE("Audio processing cdc failed");
        error = kStatus_HAL_AudioProcessingError;
    }

    return error;
}

hal_audio_processing_status_t audio_processing_deinit(const audio_processing_dev_t *dev)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    return error;
}

static void _audio_process_handle(void *param)
{
    LOGD("[AudioProcessingOffline] start.");
    uint32_t curRecSize;
    hal_afe_audio_usb_status_t status = kHALAFEAUDIOUSBStatus_PacketValid;
    usb_status_t error                = kStatus_USB_Error;

    while (1)
    {
        xTaskNotifyWaitIndexed(0, 0x00, ULONG_MAX, &curRecSize, portMAX_DELAY);

        switch (s_AfeAudioUSBTransfer.stage)
        {
            case kHALAFEAUDIOUSBPacket_Header:
            {
                if (curRecSize >= s_AfeAudioUSBTransfer.nibbleLen)
                {
                    // copy the header
                    memcpy(s_AfeAudioUSBTransfer.nibbleBuf, s_UsbDeviceCDC.currRecvBuf,
                           s_AfeAudioUSBTransfer.nibbleLen);
                    status = SLN_AFEAUDIOUSBParseHeader(s_AfeAudioUSBTransfer.nibbleBuf, &s_HeaderTransferUnit);
                    // PRINTF("HEADER_PACKET: [%d] curRecSize:%d\r\n", status, curRecSize);

                    if (status == kHALAFEAUDIOUSBStatus_Success)
                    {
                        s_AfeAudioUSBTransfer.readOffset = 0;
                        s_AfeAudioUSBTransfer.stage      = kHALAFEAUDIOUSBPacket_Data;
                        s_AfeAudioUSBTransfer.packetLen  = s_HeaderTransferUnit.pktLen;
                        int payload_len                  = curRecSize - s_AfeAudioUSBTransfer.nibbleLen;
                        // PRINTF("  pktlen:%d  nibbleLen:%d payload_len:%d\r\n", s_HeaderTransferUnit.pktLen,
                        // s_AfeAudioUSBTransfer.nibbleLen, payload_len);

                        if (payload_len > 0)
                        {
                            s_AfeAudioUSBTransfer.packetOffset = payload_len;
                            memcpy(s_AfeAudioUSBTransfer.packetBuf,
                                   s_UsbDeviceCDC.currRecvBuf + s_AfeAudioUSBTransfer.nibbleLen, payload_len);
                        }

                        if (curRecSize == (s_HeaderTransferUnit.pktLen + s_AfeAudioUSBTransfer.nibbleLen))
                        {
                            // PRINTF("Completed pkt[%d] received\r\n", s_HeaderTransferUnit.pktType);
                            status = SLN_AFEAUDIOUSBParseData(s_AfeAudioUSBTransfer.packetBuf, &s_HeaderTransferUnit);

                            s_AfeAudioUSBTransfer.stage        = kHALAFEAUDIOUSBPacket_Header;
                            s_AfeAudioUSBTransfer.packetLen    = 0;
                            s_AfeAudioUSBTransfer.packetOffset = 0;
                        }
                        else
                        {
                            // PRINTF("Partial pkt[%d] received\r\n", s_HeaderTransferUnit.pktType);
                        }
                    }
                    else
                    {
                        curRecSize = 0;
                        memset(s_AfeAudioUSBTransfer.nibbleBuf, 0, s_AfeAudioUSBTransfer.nibbleLen);
                        PRINTF("HEADER PACKET PARSE FAILED[%d]\r\n", s_AfeAudioUSBTransfer.nibbleLen);
                        break;
                    }
                }
            }
            break;

            case kHALAFEAUDIOUSBPacket_Data:
            {
                // PRINTF("DATA_PACKET: [%d] recvSize:%d packetOffset:%d type:%d\r\n", status, epCbParam->length,
                // s_AfeAudioUSBTransfer.packetOffset, s_HeaderTransferUnit.pktType);
                if ((curRecSize + s_AfeAudioUSBTransfer.packetOffset) == s_AfeAudioUSBTransfer.packetLen)
                {
                    // PRINTF("Completed pkt[%d] received\r\n", s_HeaderTransferUnit.pktType);
                    memcpy(s_AfeAudioUSBTransfer.packetBuf + s_AfeAudioUSBTransfer.packetOffset,
                           s_UsbDeviceCDC.currRecvBuf, curRecSize);
                    status = SLN_AFEAUDIOUSBParseData(s_AfeAudioUSBTransfer.packetBuf, &s_HeaderTransferUnit);
                    s_AfeAudioUSBTransfer.stage = kHALAFEAUDIOUSBPacket_Wait;
                }
                else
                {
                    PRINTF("Disorder pkt\r\n");
                }
            }
            break;

            case kHALAFEAUDIOUSBPacket_Wait:
            {
                // send the response
                SLN_AFEAudioUSBSendPacket(s_pktRes, 0, AFE_OFFLINE_SAMPLE_RES, s_HeaderTransferUnit.pktId,
                                          AFE_AUDIO_USB_ACK_SUCCESS);

                // reset the the receive state
                s_AfeAudioUSBTransfer.stage        = kHALAFEAUDIOUSBPacket_Header;
                s_AfeAudioUSBTransfer.packetLen    = 0;
                s_AfeAudioUSBTransfer.packetOffset = 0;
            }
            break;

            default:
                break;
        }

        if (curRecSize)
        {
            /* Schedule buffer for next receive event */
            error = USB_DeviceCdcAcmRecv(s_UsbDeviceCDC.cdcAcmHandle, s_UsbDeviceCDC.bulkOutEndpoint,
                                         s_UsbDeviceCDC.currRecvBuf, s_UsbDeviceCDC.bulkOutEndpointMaxPacketSize);
            if (error != kStatus_USB_Success)
            {
                LOGE("ERROR: schedule usb receive buffer %d\r\n", error);
            }
        }
    }
}

hal_audio_processing_status_t audio_processing_start(const audio_processing_dev_t *dev)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    LOGD("audio_processing_start");
    p_Dev = dev;

    s_AfeAudioUSBTransfer.nibbleBuf  = s_AfeAudioUSBNibbleBuffer;
    s_AfeAudioUSBTransfer.nibbleLen  = sizeof(s_AfeAudioUSBNibbleBuffer);
    s_AfeAudioUSBTransfer.packetBuf  = s_AfeAudioUSBPacketBuffer;
    s_AfeAudioUSBTransfer.packetLen  = 0;
    s_AfeAudioUSBTransfer.readOffset = 0;
    s_AfeAudioUSBTransfer.stage      = kHALAFEAUDIOUSBPacket_Header;

    USB_DeviceIsrEnable();
    vTaskDelay(200);
    USB_DeviceRun(s_UsbDeviceCDC.deviceHandle);

    //    _SimTimer_Start();

    if (xTaskCreate(_audio_process_handle, AUIDO_PROCESSING_OFFLINE_TASK_NAME, AUIDO_PROCESSING_OFFLINE_TASK_STACK,
                    &s_AfeAudioUSBTransfer, AUIDO_PROCESSING_OFFLINE_TASK_PRIORITY, &s_AfeAudioTask) != pdPASS)
    {
        LOGE("[AudioProcessingOffline] Task creation failed!.");
        while (1)
            ;
    }

    return error;
}

hal_audio_processing_status_t audio_processing_stop(const audio_processing_dev_t *dev)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    return error;
}

hal_audio_processing_status_t audio_processing_run(const audio_processing_dev_t *dev, void *param)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    LOGD("audio_processing_run");

    return error;
}

hal_audio_processing_status_t audio_processing_afe_notify(const audio_processing_dev_t *dev, void *param)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    event_voice_t event                 = *(event_voice_t *)param;

    uint32_t ulStatusRegister = 0;

    switch (event.event_base.eventId)
    {
        case ASR_TO_AFE_PROCESSED:
            // notify the sample has been processed
            _VoiceSession_Check();
            xTaskNotifyIndexed(s_AfeAudioTask, 0, ulStatusRegister, eSetBits);
            break;

        default:
            break;
    }

    return error;
}

const static audio_processing_dev_operator_t audio_processing_ops = {
    .init        = audio_processing_init,
    .deinit      = audio_processing_deinit,
    .start       = audio_processing_start,
    .stop        = audio_processing_stop,
    .run         = audio_processing_run,
    .inputNotify = audio_processing_afe_notify,
};

static audio_processing_dev_t audio_processing = {
    .id = 1, .name = "Offline", .ops = &audio_processing_ops, .cap = {.callback = NULL}};

/*Offline output device */
static hal_output_status_t HAL_OutputDev_Offline_Init(output_dev_t *dev, output_dev_callback_t callback);
static hal_output_status_t HAL_OutputDev_Offline_Deinit(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_Offline_Start(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_Offline_Stop(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_Offline_InferComplete(const output_dev_t *dev,
                                                               output_algo_source_t source,
                                                               void *inferResult);

const static output_dev_operator_t s_OutputDev_OfflineOps = {
    .init   = HAL_OutputDev_Offline_Init,
    .deinit = HAL_OutputDev_Offline_Deinit,
    .start  = HAL_OutputDev_Offline_Start,
    .stop   = HAL_OutputDev_Offline_Stop,
};

static output_dev_t s_OutputDev_Offline = {
    .name      = "Offline",
    .attr.type = kOutputDevType_Other,
    .ops       = &s_OutputDev_OfflineOps,
};

const static output_dev_event_handler_t s_OutputDev_OfflineHandler = {
    .inferenceComplete = HAL_OutputDev_Offline_InferComplete,
};

#define VOICE_SESSION_TIMER_IN_MS (15000)

static asr_language_t s_CurLanguage = ASR_ENGLISH;
static asr_inference_t s_CurDemo    = ASR_CMD_COFFEE_MACHINE;
static event_voice_t s_VoiceEvent;
static unsigned int s_VoiceSessionSamples = 0;
static bool s_VoiceSessionStart           = 0;

static const char *s_CoffeeMachineCmds[] = {"Start",      "Cancel", "Confirm", "Espresso",  "Americano",
                                            "Cappuccino", "Latte",  "Small",   "Medium",    "Large",
                                            "Soft",       "Mild",   "Strong",  "DeleteUser"};

static const char *s_ElevatorCmds[] = {"FloorOne", "FloorTwo", "FloorThree", "FloorFour", "FloorFive",
                                       "FloorSix", "Confirm",  "Cancel",     "DeleteUser"};

static const char *_VoiceLanguageName_Get(asr_language_t language)
{
    const char *language_str = NULL;

    switch (language)
    {
        case ASR_ENGLISH:
            language_str = "ENGLISH";
            break;
        case ASR_CHINESE:
            language_str = "CHINESE";
            break;
        case ASR_GERMAN:
            language_str = "GERMAN";
            break;
        case ASR_FRENCH:
            language_str = "FRENCH";
            break;

        default:
            language_str = "-----";
            break;
    }

    return language_str;
}

static const char *_VoiceCmdName_Get(int32_t keywordID)
{
    const char *cmd_name = NULL;

#if ENABLE_COFFEE_MACHINE
    int32_t cmd_count = sizeof(s_CoffeeMachineCmds) / sizeof(s_CoffeeMachineCmds[0]);
    if (keywordID < cmd_count)
    {
        cmd_name = s_CoffeeMachineCmds[keywordID];
    }
#elif ENABLE_ELEVATOR
    int32_t cmd_count = sizeof(s_ElevatorCmds) / sizeof(s_ElevatorCmds[0]);
    if (keywordID < cmd_count)
    {
        cmd_name = s_ElevatorCmds[keywordID];
    }
#else
#error "CURRENT_DEMO is not defined"
#endif

    return cmd_name;
}

static void _VoiceModel_Set(asr_inference_t modelId, asr_language_t lang, uint8_t ptt)
{
    LOGD("[Offline] Set voice model:%d, language:%d, ptt:%d", modelId, lang, ptt);

    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_VoiceAlgoInputNotify;
    output_event.data      = &s_VoiceEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_VoiceEvent);
    output_event.eventInfo = kEventInfo_Local;

    s_VoiceEvent.event_base.eventId   = SET_VOICE_MODEL;
    s_VoiceEvent.event_base.eventInfo = kEventInfo_Local;
    s_VoiceEvent.set_asr_config.demo  = modelId;
    s_VoiceEvent.set_asr_config.lang  = lang;
    s_VoiceEvent.set_asr_config.ptt   = ptt;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_Offline.cap.callback(s_OutputDev_Offline.id, output_event, fromISR);
}

static void _VoiceSession_Start()
{
    s_VoiceSessionStart   = 1;
    s_VoiceSessionSamples = 0;
}

static void _VoiceSession_Stop()
{
    s_VoiceSessionStart   = 0;
    s_VoiceSessionSamples = 0;
}

static void _VoiceSession_Check()
{
    if (s_VoiceSessionStart)
    {
        s_VoiceSessionSamples++;
        if (s_VoiceSessionSamples >= (16 * 2 * VOICE_SESSION_TIMER_IN_MS) / 960)
        {
            s_VoiceSessionStart   = 0;
            s_VoiceSessionSamples = 0;
            LOGD("[Offline] Timeout");
            _VoiceModel_Set(ASR_WW, UNDEFINED_LANGUAGE, 0);
        }
    }
}

static hal_output_status_t HAL_OutputDev_Offline_Init(output_dev_t *dev, output_dev_callback_t callback)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_Offline_Init");

    dev->cap.callback = callback;

    LOGD("--HAL_OutputDev_Offline_Init");
    return error;
}

static hal_output_status_t HAL_OutputDev_Offline_Deinit(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_Offline_Deinit");

    /* Add de-initialization code here */

    LOGD("--HAL_OutputDev_Offline_Deinit");
    return error;
}

static hal_output_status_t HAL_OutputDev_Offline_Start(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_Offline_Start");

    /* Add start code here */
    if (FWK_OutputManager_RegisterEventHandler(dev, &s_OutputDev_OfflineHandler) != 0)
    {
        error = kStatus_HAL_OutputError;
    }

    /* Set the voice to wake word state */
    _VoiceModel_Set(ASR_WW, UNDEFINED_LANGUAGE, 0);

    LOGD("--HAL_OutputDev_Offline_Start");
    return error;
}

static hal_output_status_t HAL_OutputDev_Offline_Stop(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    LOGD("++HAL_OutputDev_Offline_Stop");

    /* Add stop code here */

    LOGD("--HAL_OutputDev_Offline_Stop");
    return error;
}

static hal_output_status_t _InferComplete_Voice(const output_dev_t *dev, void *inferResult)
{
    hal_output_status_t error               = kStatus_HAL_OutputSuccess;
    asr_inference_result_t *voiceAlgoResult = (asr_inference_result_t *)inferResult;

    // LOGD("[PT] voice status:%d cmd:%d", voiceAlgoResult->status, voiceAlgoResult->keywordID);
    if ((voiceAlgoResult->status == ASR_WW_DETECT))
    {
        s_CurLanguage             = voiceAlgoResult->language;
        const char *language_name = _VoiceLanguageName_Get(s_CurLanguage);
        LOGD(":VOICE:WakeWord:%s:[OK]", (language_name == NULL) ? "" : language_name);
        _VoiceModel_Set(s_CurDemo, s_CurLanguage, 0);
        _VoiceSession_Start();
    }
    else if ((voiceAlgoResult->status == ASR_CMD_DETECT) && (voiceAlgoResult->keywordID > -1))
    {
        const char *cmd_name      = _VoiceCmdName_Get(voiceAlgoResult->keywordID);
        const char *language_name = _VoiceLanguageName_Get(s_CurLanguage);
        LOGD(":VOICE:Command:%s:%s:%d:[OK]", (language_name == NULL) ? "" : language_name,
             (cmd_name == NULL) ? "" : cmd_name, voiceAlgoResult->keywordID);
        _VoiceSession_Stop();
        _VoiceModel_Set(ASR_WW, UNDEFINED_LANGUAGE, 0);
    }

    return error;
}

static hal_output_status_t HAL_OutputDev_Offline_InferComplete(const output_dev_t *dev,
                                                               output_algo_source_t source,
                                                               void *inferResult)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    if (inferResult == NULL)
    {
        return error;
    }

    if (source == kOutputAlgoSource_Voice)
    {
        _InferComplete_Voice(dev, inferResult);
    }

    return error;
}

int HAL_AudioProcessingDev_Offline_Register()
{
    int error = 0;

    s_UsbDeviceCDC.cdcAcmHandle = NULL;
    USB_RegisterCDC2();
    USB_CompositeClassRegister(USB_DeviceCallback, USB_DeviceCdcVcomCallback, &g_UsbDeviceCdcVcomConfig[1]);

    LOGD("HAL_AudioProcessingDev_Offline_Register");
    error = FWK_AudioProcessing_DeviceRegister(&audio_processing);
    if (!error)
    {
        error = FWK_OutputManager_DeviceRegister(&s_OutputDev_Offline);
    }

    return error;
}
#endif /* ENABLE_AUDIO_PROCESSING_DEV_Offline */
