/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board_define.h"
#ifdef ENABLE_MULTICORE_DEV_MessageBuffer
#include "hal_multicore_dev.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "pin_mux.h"
#include "fwk_log.h"
#include "fwk_multicore_manager.h"
#include "app_config.h"
#include "message_buffer.h"
#include "mcmgr.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

enum _multicore_events
{
    kMulticore_DataEvent,
};

#define MB_STRUCT_SIZE           (sizeof(StaticStreamBuffer_t))
#define MB_STORAGE_BUFFER_SIZE   (0x1000)
#define SH_MEM_MB_OFFSET         (0x0u)
#define SH_MEM_MB_STRUCT_OFFSET  (SH_MEM_MB_OFFSET + 0x10)
#define SH_MEM_MB_STORAGE_OFFSET (SH_MEM_MB_STRUCT_OFFSET + MB_STRUCT_SIZE)

/* MessageBuffer structures */
#define xWriteMessageBuffer         (*(MessageBufferHandle_t *)(BOARD_SHMEM_WRITE))
#define xWriteMessageBufferStruct   (*(StaticStreamBuffer_t *)(BOARD_SHMEM_WRITE + SH_MEM_MB_STRUCT_OFFSET))
#define ucWriteMessageBufferStorage (*(uint8_t *)(BOARD_SHMEM_WRITE + SH_MEM_MB_STORAGE_OFFSET))

#define xReadMessageBuffer         (*(MessageBufferHandle_t *)(BOARD_SHMEM_READ))
#define xReadMessageBufferStruct   (*(StaticStreamBuffer_t *)(BOARD_SHMEM_READ + SH_MEM_MB_STRUCT_OFFSET))
#define ucReadMessageBufferStorage (*(uint8_t *)(BOARD_SHMEM_READ + SH_MEM_MB_STORAGE_OFFSET))

#define MULTICORE_RCV_NAME       "MessageBuffer"
#define MULTICORE_RCV_TASK_NAME  "multicore_rcv_task"
#define MULTICORE_RCV_TASK_STACK (MB_STORAGE_BUFFER_SIZE / sizeof(StackType_t) + 1024)
#if FWK_SUPPORT_STATIC_ALLOCATION
FWKDATA static StackType_t s_MulticoreRcvTaskStack[MULTICORE_RCV_TASK_STACK];
FWKDATA static StaticTask_t s_MulticoreRvcTaskTCB;
static void *s_MulticoreRcvTaskTCBReference = (void *)&s_MulticoreRvcTaskTCB;
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static hal_multicore_status_t HAL_MulticoreDev_MessageBuffer_Deinit(const multicore_dev_t *dev);
static hal_multicore_status_t HAL_MulticoreDev_MessageBuffer_Send(const multicore_dev_t *dev,
                                                                  void *data,
                                                                  unsigned int size);
static hal_multicore_status_t HAL_MulticoreDev_MessageBuffer_InputNotify(const multicore_dev_t *dev, void *data);
static hal_multicore_status_t HAL_MulticoreDev_MessageBuffer_Start(const multicore_dev_t *dev);
static hal_multicore_status_t HAL_MulticoreDev_MessageBuffer_Init(multicore_dev_t *dev,
                                                                  multicore_dev_callback_t callback,
                                                                  void *param);
/*******************************************************************************
 * Global Variables
 ******************************************************************************/
volatile static bool s_SecondCoreReady;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* initialize the dev */
hal_multicore_status_t (*init)(
    multicore_dev_t *dev, int width, int height, multicore_dev_callback_t callback, void *param);
/* deinitialize the dev */
hal_multicore_status_t (*deinit)(const multicore_dev_t *dev);
/* start the dev */
hal_multicore_status_t (*start)(const multicore_dev_t *dev);
/* Multicore Send the message */
hal_multicore_status_t (*send)(const multicore_dev_t *dev, void *data, uint32_t size);
/* input notify */
hal_multicore_status_t (*inputNotify)(const multicore_dev_t *dev, void *data);

const static multicore_dev_operator_t s_MulticoreDev_MessageBufferOps = {
    .init        = HAL_MulticoreDev_MessageBuffer_Init,
    .deinit      = HAL_MulticoreDev_MessageBuffer_Deinit,
    .start       = HAL_MulticoreDev_MessageBuffer_Start,
    .send        = HAL_MulticoreDev_MessageBuffer_Send,
    .inputNotify = HAL_MulticoreDev_MessageBuffer_InputNotify,
};

static multicore_dev_t s_MulticoreDev_MessageBuffer = {
    .name = "MessageBuffer",
    .ops  = &s_MulticoreDev_MessageBufferOps,
};

void vGenerateMulticoreInterrupt(void *xUpdatedMessageBuffer)
{
    /* Trigger the inter-core interrupt using the MCMGR component.
       Pass the APP_MESSAGE_BUFFER_EVENT_DATA as data that accompany
       the kMCMGR_FreeRtosMessageBuffersEvent event. */
    (void)MCMGR_TriggerEventForce(kMCMGR_FreeRtosMessageBuffersEvent, kMulticore_DataEvent);
}

static void RemoteAppReadyEventHandler(uint16_t eventData, void *context)
{
    *(bool *)context = (bool)eventData;
}

static void FreeRtosMessageBuffersEventHandler(uint16_t eventData, void *context)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Make sure the message has been addressed to us. Using eventData that accompany
       the event of the kMCMGR_FreeRtosMessageBuffersEvent type, we can distinguish
       different consumers. */
    if (kMulticore_DataEvent == eventData)
    {
        /* Call the API function that sends a notification to any task that is
    blocked on the xUpdatedMessageBuffer message buffer waiting for data to
    arrive. */
        (void)xMessageBufferSendCompletedFromISR(xReadMessageBuffer, &xHigherPriorityTaskWoken);
    }

    /* Normal FreeRTOS "yield from interrupt" semantics, where
    HigherPriorityTaskWoken is initialzed to pdFALSE and will then get set to
    pdTRUE if the interrupt unblocks a task that has a priority above that of
    the currently executing task. */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    /* No need to clear the interrupt flag here, it is handled by the mcmgr. */
}

static void _HAL_MulticoreDev_MessageBuffer_RcvMsgHandler(void *param)
{
    /* Size to cover on MAX message. Can be lowered if we know what we send */
    uint8_t pMessageBufferRcv[MB_STORAGE_BUFFER_SIZE];

    while (1)
    {
        size_t xReceivedBytes = xMessageBufferReceive(xReadMessageBuffer, (void *)pMessageBufferRcv,
                                                      sizeof(pMessageBufferRcv), portMAX_DELAY);

        LOGI("Remote Message receive, size = %d", xReceivedBytes);
        if ((xReceivedBytes != 0) && (s_MulticoreDev_MessageBuffer.cap.callback != NULL))
        {
            multicore_event_t multicore_event;
            multicore_event.eventId = kMulticoreEvent_MsgReceive;
            multicore_event.data    = pMessageBufferRcv;
            multicore_event.size    = xReceivedBytes;
            s_MulticoreDev_MessageBuffer.cap.callback(&s_MulticoreDev_MessageBuffer, multicore_event, false);
        }
    }
}

static hal_multicore_status_t HAL_MulticoreDev_MessageBuffer_Deinit(const multicore_dev_t *dev)
{
    hal_multicore_status_t status = kStatus_HAL_MulticoreSuccess;

    return status;
}

static hal_multicore_status_t HAL_MulticoreDev_MessageBuffer_Send(const multicore_dev_t *dev, void *data, unsigned int size)
{
    hal_multicore_status_t status = kStatus_HAL_MulticoreSuccess;

    if ((data != NULL) && (size != 0))
    {
        uint32_t streamFreeSpace = xStreamBufferSpacesAvailable(xWriteMessageBuffer);
        if (streamFreeSpace < size)
        {
            status = kStatus_HAL_MulticoreError;
            LOGE("Not enough space, free %x needed %x", streamFreeSpace, size);
        }

        if (status == kStatus_HAL_MulticoreSuccess)
        {
            (void)xMessageBufferSend(xWriteMessageBuffer, data, size, 0);
            LOGI("MulticoreDev_send: Send %d bytes", size);
        }
    }
    else
    {
        LOGD("MulticoreDev_send: Nothing to send");
    }

    return status;
}

static hal_multicore_status_t HAL_MulticoreDev_MessageBuffer_InputNotify(const multicore_dev_t *dev, void *data)
{
    hal_multicore_status_t status = kStatus_HAL_MulticoreSuccess;

    return status;
}

static hal_multicore_status_t HAL_MulticoreDev_MessageBuffer_Start(const multicore_dev_t *dev)
{
    hal_multicore_status_t status = kStatus_HAL_MulticoreSuccess;

    /* Wait until the secondary core application signals it is ready to communicate. */
    while (true != s_SecondCoreReady)
    {
        (void)MCMGR_TriggerEvent(kMCMGR_RemoteApplicationEvent, true);
        vTaskDelay(pdMS_TO_TICKS(10));
    };

    /* Send one more event to be sure the other core got it */
    (void)MCMGR_TriggerEvent(kMCMGR_RemoteApplicationEvent, true);

#if FWK_SUPPORT_STATIC_ALLOCATION
    if (xTaskCreateStatic(_HAL_MulticoreDev_MessageBuffer_RcvMsgHandler, MULTICORE_RCV_TASK_NAME,
                          MULTICORE_RCV_TASK_STACK, NULL, uxTaskPriorityGet(NULL), s_MulticoreRcvTaskStack,
                          s_MulticoreRcvTaskTCBReference) == NULL)
#else
    if (xTaskCreate(_HAL_MulticoreDev_MessageBuffer_RcvMsgHandler, MULTICORE_RCV_TASK_NAME, MULTICORE_RCV_TASK_STACK,
                    NULL, uxTaskPriorityGet(NULL), NULL) != pdPASS)
#endif
    {
        LOGE("[MessageBuffer] Task creation failed!.");
        while (1)
            ;
    }

    return status;
}

static hal_multicore_status_t HAL_MulticoreDev_MessageBuffer_Init(multicore_dev_t *dev,
                                                                  multicore_dev_callback_t callback,
                                                                  void *param)
{
    hal_multicore_status_t status = kStatus_HAL_MulticoreSuccess;
    LOGD("Start Multicore MessageBuffer INIT");

    s_MulticoreDev_MessageBuffer.cap.callback = callback;

    xWriteMessageBuffer = xMessageBufferCreateStatic(
        /* The buffer size in bytes. */
        MB_STORAGE_BUFFER_SIZE,
        /* Statically allocated buffer storage area. */
        &ucWriteMessageBufferStorage,
        /* Message buffer handle. */
        &xWriteMessageBufferStruct);

    (void)MCMGR_RegisterEvent(kMCMGR_FreeRtosMessageBuffersEvent, FreeRtosMessageBuffersEventHandler, ((void *)0));
    (void)MCMGR_RegisterEvent(kMCMGR_RemoteApplicationEvent, RemoteAppReadyEventHandler, (void *)&s_SecondCoreReady);

    /* We initied we are ready to rcv messages */
    LOGD("Exit Multicore MessageBuffer INIT");
    return status;
}

HAL_MULTICORE_DEV_DECLARE(MessageBuffer)
{
    int status = 0;
    LOGD("multicore_dev_MessageBuffer_register");
    status = FWK_MulticoreManager_DeviceRegister(&s_MulticoreDev_MessageBuffer);
    return status;
}
#endif /* ENABLE_FLASH_DEV_Littlefs */
