/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief output device manager framework implementation.
 */

#include "fwk_platform.h"
#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_task.h"
#include "fwk_graphics.h"
#include "fwk_output_manager.h"

typedef struct
{
    fwk_task_data_t commonData;
    output_dev_t *devs[MAXIMUM_OUTPUT_DEV]; /* registered output devices */
    List_t outEventReceiverList;            /* registered output event receiver */
    int uiReceiverCount;
} output_task_data_t;

typedef struct
{
    fwk_task_t task;
    output_task_data_t outputData;
} output_task_t;

typedef struct
{
    const output_dev_t *pDev;

    const output_dev_event_handler_t *handler;

    ListItem_t receiverListItem;
} output_event_receiver_t;

static output_task_t s_OutputTask;
#if FWK_SUPPORT_STATIC_ALLOCATION
FWKDATA static StackType_t s_OutputTaskStack[OUTPUT_MANAGER_TASK_STACK];
FWKDATA static StaticTask_t s_OutputTaskTCB;
static void *s_OutputTaskTCBReference = (void *)&s_OutputTaskTCB;
#else
static void *s_OutputTaskStack        = NULL;
static void *s_OutputTaskTCBReference = NULL;
#endif

static int _FWK_OutputManager_DeviceCallback(int devId, output_event_t event, uint8_t fromISR);

static int _FWK_OutputManager_task_init(fwk_task_data_t *pTaskData)
{
    if (pTaskData == NULL)
        return -1;

    int error                           = 0;
    output_task_data_t *pOutputTaskData = (output_task_data_t *)pTaskData;

    /* init the output dev */
    for (int i = 0; i < MAXIMUM_OUTPUT_DEV; i++)
    {
        output_dev_t *pDev = pOutputTaskData->devs[i];
        if (pDev != NULL && pDev->ops->init != NULL)
        {
            LOGD("INIT output dev \"%s\"", pDev->name);
            error = pDev->ops->init(pDev, _FWK_OutputManager_DeviceCallback);

            if (error)
            {
                LOGE("INIT output dev \"%s\" error: %d", pDev->name, error);
            }
        }
    }

    for (int i = 0; i < MAXIMUM_OUTPUT_DEV; i++)
    {
        output_dev_t *pDev = pOutputTaskData->devs[i];
        if (pDev != NULL && pDev->ops->start != NULL)
        {
            LOGD("START output dev \"%s\"", pDev->name);
            error = pDev->ops->start(pDev);

            if (error)
            {
                LOGE("START output dev \"%s\" error: %d", pDev->name, error);
                return error;
            }
        }
    }

    return error;
}

/*
 * output dev callback
 */
static int _FWK_OutputManager_DeviceCallback(int devId, output_event_t event, uint8_t fromISR)
{
    bool msgReady            = true;
    fwk_task_id_t receiverId = kFWKTaskID_COUNT;

    if (event.eventId == kOutputEvent_VisionAlgoInputNotify)
    {
        receiverId = kFWKTaskID_VisionAlgo;
    }
    else if (event.eventId == kOutputEvent_VoiceAlgoInputNotify)
    {
        receiverId = kFWKTaskID_VoiceAlgo;
    }
    else if (event.eventId == kOutputEvent_OutputInputNotify)
    {
        receiverId = kFWKTaskID_Output;
    }
    else if (event.eventId == kOutputEvent_SpeakerToAfeFeedback)
    {
        receiverId = kFWKTaskID_Audio;
    }
    else if (event.eventId == kOutputEvent_DisplayInputNotify)
    {
        receiverId = kFWKTaskID_Display;
    }

    switch (event.eventId)
    {
        case kOutputEvent_VisionAlgoInputNotify:
        case kOutputEvent_VoiceAlgoInputNotify:
        case kOutputEvent_OutputInputNotify:
        case kOutputEvent_SpeakerToAfeFeedback:
        case kOutputEvent_DisplayInputNotify:
        {
            fwk_message_t *pMsg = (fwk_message_t *)FWK_MALLOC(sizeof(fwk_message_t));
            if (pMsg)
            {
                memset(pMsg, 0, sizeof(fwk_message_t));
                pMsg->freeAfterConsumed = 1;
                pMsg->id                = kFWKMessageID_InputNotify;
                pMsg->payload.devId     = devId;
                if (event.eventInfo < kEventInfo_Invalid)
                {
                    pMsg->msgInfo = event.eventInfo;
                }
#if FWK_SUPPORT_MULTICORE
                /* Set this a multicore Message to broadcast results in all the system */
                if ((event.eventInfo != kEventInfo_Local) && (event.eventInfo < kEventInfo_Invalid))
                {
                    pMsg->multicore.isMulticoreMessage = 1;
                    pMsg->multicore.taskId             = receiverId;
                }
#endif /* FWK_SUPPORT_MULTICORE */

                if (event.copy == 0)
                {
                    pMsg->payload.data              = event.data;
                    pMsg->payload.freeAfterConsumed = 0;
                }
                else
                {
                    pMsg->payload.data = FWK_MALLOC(event.size);
                    if (pMsg->payload.data != NULL)
                    {
                        memcpy(pMsg->payload.data, event.data, event.size);
                        pMsg->payload.freeAfterConsumed = 1;
                        pMsg->payload.size              = event.size;
                    }
                    else
                    {
                        LOGE("Can't allocate raw.data memory for Message kFWKMessageID_InputNotify");
                        msgReady = false;
                    }
                }

                if (msgReady)
                {
                    /* Send Speaker feedback (about streaming audio) to AFE */
                    if (fromISR)
                    {
                        FWK_Message_PutFromIsr(receiverId, &pMsg);
                    }
                    else
                    {
                        FWK_Message_Put(receiverId, &pMsg);
                    }
                }
                else
                {
                    FWK_FREE(pMsg);
                }
            }
            else
            {
                LOGE("Can't allocate memory for Message kFWKMessageID_InputNotify");
            }
        }
        break;

        default:
            break;
    }

    return 0;
}

static void _FWK_OutputManager_MessageHandle(fwk_message_t *pMsg, fwk_task_data_t *pTaskData)
{
    ListItem_t *pxListItem, *pxNext;
    ListItem_t const *pxListEnd;

    if ((pMsg == NULL) || (pTaskData == NULL))
        return;

    static bool s_SleepModeIsOn         = false;
    output_task_data_t *pOutputTaskData = (output_task_data_t *)pTaskData;
    List_t *pReceiverList               = &(pOutputTaskData->outEventReceiverList);

    pxListItem                    = listGET_HEAD_ENTRY(pReceiverList);
    pxListEnd                     = listGET_END_MARKER(pReceiverList);
    output_event_receiver_t *pRec = NULL;

    switch (pMsg->id)
    {
        case kFWKMessageID_VAlgoASRResultUpdate:
        case kFWKMessageID_VAlgoResultUpdate:
        case kFWKMessageID_LpmPreEnterSleep:
        {
            if (s_SleepModeIsOn)
            {
                /* Don't forward any inference results */
                break;
            }

            while (pxListItem != pxListEnd)
            {
                pRec = (output_event_receiver_t *)listGET_LIST_ITEM_OWNER(pxListItem);

                if ((pRec != NULL) && (pRec->pDev != NULL) && (pRec->handler != NULL) &&
                    (pRec->handler->inferenceComplete != NULL))
                {
                    int error           = 0;
                    int updateOverlayUI = 0;
                    if (pMsg->id == kFWKMessageID_VAlgoASRResultUpdate)
                    {
                        error =
                            pRec->handler->inferenceComplete(pRec->pDev, kOutputAlgoSource_Voice, pMsg->payload.data);
                        if ((error == kStatus_HAL_OutputSuccess) && (pRec->pDev->attr.type == kOutputDevType_UI))
                        {
                            updateOverlayUI = 1;
                        }
                    }
                    else if (pMsg->id == kFWKMessageID_VAlgoResultUpdate)
                    {
                        error =
                            pRec->handler->inferenceComplete(pRec->pDev, kOutputAlgoSource_Vision, pMsg->payload.data);
                        if ((error == kStatus_HAL_OutputSuccess) && (pRec->pDev->attr.type == kOutputDevType_UI))
                        {
                            updateOverlayUI = 1;
                        }
                    }
                    else if (pMsg->id == kFWKMessageID_LpmPreEnterSleep)
                    {
                        error = pRec->handler->inferenceComplete(pRec->pDev, kOutputAlgoSource_LPM, pMsg->payload.data);
                        s_SleepModeIsOn = true;
                    }

                    if (updateOverlayUI)
                    {
                        /* only support one UI receiver currently */
                        fwk_message_t *pMsg = (fwk_message_t *)FWK_MALLOC(sizeof(fwk_message_t));
                        if (pMsg != NULL)
                        {
                            memset(pMsg, 0, sizeof(fwk_message_t));
                            pMsg->id                       = kFWKMessageID_DispatcherRequestShowOverlay;
                            pMsg->freeAfterConsumed        = 1;
                            pMsg->payload.overlay.pSurface = pRec->pDev->attr.pSurface;
                            FWK_Message_Put(kFWKTaskID_Camera, &pMsg);
                        }
                        else
                        {
                            LOGE("Can't allocate memory for pMsg.");
                        }
                    }

                    if (error)
                    {
                        LOGE("Output device \"%s\":inference result event handler resulted in an error: %d",
                             pRec->pDev->name, error);
                    }
                }

                pxNext     = listGET_NEXT(pxListItem);
                pxListItem = pxNext;
            }

            if (pMsg->payload.freeAfterConsumed)
            {
                pMsg->payload.freeAfterConsumed = 0;
                FWK_FREE(pMsg->payload.data);
            }
        }
        break;

        case kFWKMessageID_InputNotify:
        {
            while (pxListItem != pxListEnd)
            {
                pRec = (output_event_receiver_t *)listGET_LIST_ITEM_OWNER(pxListItem);

                if ((pRec != NULL) && (pRec->pDev != NULL) && (pRec->handler != NULL) &&
                    (pRec->handler->inputNotify != NULL))
                {
                    int error = pRec->handler->inputNotify(pRec->pDev, pMsg->payload.data);

                    if (error)
                    {
                        LOGE("Output device \"%s\":input notify event handler resulted in an error: %d",
                             pRec->pDev->name, error);
                    }
                }

                pxNext     = listGET_NEXT(pxListItem);
                pxListItem = pxNext;
            }

            if (pMsg->payload.freeAfterConsumed)
            {
                pMsg->payload.freeAfterConsumed = 0;
                FWK_FREE(pMsg->payload.data);
            }
        }
        break;

        case kFWKMessageID_AudioDump:
        {
            while (pxListItem != pxListEnd)
            {
                pRec = (output_event_receiver_t *)listGET_LIST_ITEM_OWNER(pxListItem);

                if ((pRec != NULL) && (pRec->pDev != NULL) && (pRec->handler != NULL) && (pRec->handler->dump != NULL))
                {
                    pRec->handler->dump(pRec->pDev, &pMsg->payload);
                }
                pxNext     = listGET_NEXT(pxListItem);
                pxListItem = pxNext;
            }
            if (pMsg->payload.freeAfterConsumed)
            {
                pMsg->payload.freeAfterConsumed = 0;
                FWK_FREE(pMsg->payload.data);
            }
        }
        break;

        case kFWKMessageID_InputFrameworkGetComponents:
        {
            framework_request_t frameworkRequest   = pMsg->payload.frameworkRequest;
            fwk_task_component_t fwkTaskComponent  = {0};
            framework_response_t frameworkResponse = {0};
            fwkTaskComponent.managerId             = kFWKTaskID_Output;
            while (pxListItem != pxListEnd)
            {
                const output_dev_t *pDev = NULL;
                pRec                     = (output_event_receiver_t *)listGET_LIST_ITEM_OWNER(pxListItem);

                if (pRec != NULL)
                {
                    pDev = pRec->pDev;
                }

                if (pDev != NULL)
                {
                    fwkTaskComponent.deviceId          = pDev->id;
                    fwkTaskComponent.deviceName        = pDev->name;
                    fwkTaskComponent.configs           = pDev->configs;
                    frameworkResponse.fwkTaskComponent = fwkTaskComponent;
                    if (frameworkRequest.respond != NULL)
                    {
                        frameworkRequest.respond(kFrameworkEvents_GetManagerComponents, &frameworkResponse, false);
                    }
                }
                pxNext     = listGET_NEXT(pxListItem);
                pxListItem = pxNext;
            }

            if (frameworkRequest.respond != NULL)
            {
                frameworkRequest.respond(kFrameworkEvents_GetManagerComponents, NULL, true);
            }
        }
        break;

        case kFWKMessageID_InputFrameworkGetDeviceConfigs:
        {
            framework_request_t frameworkRequest = pMsg->payload.frameworkRequest;
            fwk_task_component_t fwkTaskComponent;
            framework_response_t frameworkResponse;
            fwkTaskComponent.managerId = kFWKTaskID_Output;
            while (pxListItem != pxListEnd)
            {
                const output_dev_t *pDev = NULL;
                pRec                     = (output_event_receiver_t *)listGET_LIST_ITEM_OWNER(pxListItem);

                if (pRec != NULL)
                {
                    pDev = pRec->pDev;
                }

                /* Found device with matching name and/or ID, so print configs associated with it */
                /* TODO: Extend to use manager id + dev_id as well as device name*/
                if (pDev != NULL && ((strcmp(pDev->name, frameworkRequest.deviceName) == 0)))
                {
                    fwkTaskComponent.deviceId          = pDev->id;
                    fwkTaskComponent.deviceName        = pDev->name;
                    fwkTaskComponent.configs           = pDev->configs;
                    frameworkResponse.fwkTaskComponent = fwkTaskComponent;

                    if (frameworkRequest.respond != NULL)
                    {
                        frameworkRequest.respond(kFrameworkEvents_GetDeviceConfigs, &frameworkResponse, false);
                    }
                    return;
                }
                pxNext     = listGET_NEXT(pxListItem);
                pxListItem = pxNext;
            }
            /* Signifies that a matching device was never found */
            if (frameworkRequest.respond != NULL)
            {
                frameworkRequest.respond(kFrameworkEvents_GetDeviceConfigs, NULL, true);
            }
        }
        break;

        default:
            break;
    }
}

int FWK_OutputManager_Init()
{
    for (int i = 0; i < MAXIMUM_OUTPUT_DEV; i++)
    {
        s_OutputTask.outputData.devs[i] = NULL;
    }
    List_t *pReceiverList                   = &s_OutputTask.outputData.outEventReceiverList;
    s_OutputTask.outputData.uiReceiverCount = 0;
    vListInitialise(pReceiverList);

    return 0;
}

int FWK_OutputManager_Start(int taskPriority)
{
    LOGD("[OutputManager]:Starting...");
    int error = 0;

    s_OutputTask.task.msgHandle  = _FWK_OutputManager_MessageHandle;
    s_OutputTask.task.taskInit   = _FWK_OutputManager_task_init;
    s_OutputTask.task.data       = (fwk_task_data_t *)&(s_OutputTask.outputData);
    s_OutputTask.task.taskId     = kFWKTaskID_Output;
    s_OutputTask.task.delayMs    = 1;
    s_OutputTask.task.taskStack  = s_OutputTaskStack;
    s_OutputTask.task.taskBuffer = s_OutputTaskTCBReference;

    FWK_Task_Start((fwk_task_t *)&s_OutputTask.task, OUTPUT_MANAGER_TASK_NAME, OUTPUT_MANAGER_TASK_STACK, taskPriority);

    LOGD("[OutputManager]:Started");

    return error;
}

int FWK_OutputManager_Deinit()
{
    return 0;
}

int FWK_OutputManager_DeviceRegister(output_dev_t *dev)
{
    int error = -1;

    for (int i = 0; i < MAXIMUM_OUTPUT_DEV; i++)
    {
        if (s_OutputTask.outputData.devs[i] == NULL)
        {
            dev->id                         = i;
            s_OutputTask.outputData.devs[i] = dev;
            return 0;
        }
    }

    return error;
}

int FWK_OutputManager_RegisterEventHandler(const output_dev_t *dev, const output_dev_event_handler_t *handler)
{
    int error = 0;

    if (dev->attr.type == kOutputDevType_UI)
    {
        if (s_OutputTask.outputData.uiReceiverCount == 1)
        {
            LOGE(
                "[ERROR] A UI event receiver device has already been registered. Currently only one device can be "
                "registered as a UI event receiver.\r\n");
            return -1;
        }

        s_OutputTask.outputData.uiReceiverCount++;
    }

    output_event_receiver_t *pRec = FWK_MALLOC(sizeof(output_event_receiver_t));
    if (pRec == NULL)
    {
        LOGE("Cannot allocate memory for outevent receiver device \"%s\"!", dev->name);
        return error;
    }
    pRec->pDev    = dev;
    pRec->handler = handler;
    vListInitialiseItem(&(pRec->receiverListItem));

    listSET_LIST_ITEM_OWNER(&(pRec->receiverListItem), pRec);

    List_t *pReceiverList = &s_OutputTask.outputData.outEventReceiverList;
    vListInsertEnd(pReceiverList, &(pRec->receiverListItem));

    return error;
}

int FWK_OutputManager_UnregisterEventHandler(const output_dev_t *dev)
{
    int error = 0;

    ListItem_t *pxListItem, *pxNext;
    ListItem_t const *pxListEnd;
    List_t *pReceiverList         = &s_OutputTask.outputData.outEventReceiverList;
    pxListItem                    = listGET_HEAD_ENTRY(pReceiverList);
    pxListEnd                     = listGET_END_MARKER(pReceiverList);
    output_event_receiver_t *pRec = NULL;

    while (pxListItem != pxListEnd)
    {
        pRec = (output_event_receiver_t *)listGET_LIST_ITEM_OWNER(pxListItem);

        if ((pRec != NULL) && (pRec->pDev != NULL) && (pRec->handler != NULL) && (pRec->pDev == dev))
        {
            break;
        }

        pxNext     = listGET_NEXT(pxListItem);
        pxListItem = pxNext;
    }
    if ((pRec == NULL) || (pxListItem == pxListEnd))
    {
        LOGE("Failed to unregister handler for dev \"%s\"", dev->name);
        return error;
    }

    uxListRemove(&(pRec->receiverListItem));

    pRec->pDev    = NULL;
    pRec->handler = NULL;

    FWK_FREE(pRec);

    if (dev->attr.type == kOutputDevType_UI)
    {
        s_OutputTask.outputData.uiReceiverCount--;
    }

    return error;
}
