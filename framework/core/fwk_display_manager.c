/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief display manager implementation.
 */

#include "fwk_platform.h"
#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_task.h"
#include "fwk_perf.h"
#include "fwk_graphics.h"
#include "fwk_display_manager.h"

typedef struct
{
    fwk_task_data_t commonData;
    display_dev_t *devs[MAXIMUM_DISPLAY_DEV];              /* registered display devices */
    fwk_message_t displayRequestMsgs[MAXIMUM_DISPLAY_DEV]; /* display request frame message */
} display_task_data_t;

typedef struct
{
    fwk_task_t task;
    display_task_data_t displayData;
} display_task_t;

/*
 * display manager task
 */
static display_task_t s_DisplayTask;
#if FWK_SUPPORT_STATIC_ALLOCATION
FWKDATA static StackType_t s_DisplayTaskStack[DISPLAY_MANAGER_TASK_STACK];
FWKDATA static StaticTask_t s_DisplayTaskTCB;
static void *s_DisplayTaskTCBBReference = (void *)&s_DisplayTaskTCB;
#else
static void *s_DisplayTaskStack         = NULL;
static void *s_DisplayTaskTCBBReference = NULL;
#endif

/*
 * display dev callback
 */
static int _FWK_DisplayManager_DeviceCallback(const display_dev_t *dev,
                                              display_event_t event,
                                              void *param,
                                              uint8_t fromISR)
{
    switch (event)
    {
        case kDisplayEvent_RequestFrame:
        {
            for (int i = 0; i < MAXIMUM_DISPLAY_DEV; i++)
            {
                if ((s_DisplayTask.displayData.devs[i] != NULL) && (s_DisplayTask.displayData.devs[i]->id == dev->id))
                {
                    fwk_message_t *pDisplayReqMsg;
                    pDisplayReqMsg                          = &s_DisplayTask.displayData.displayRequestMsgs[i];
                    pDisplayReqMsg->id                      = kFWKMessageID_DisplayRequestFrame;
                    pDisplayReqMsg->payload.devId           = dev->id;
                    pDisplayReqMsg->payload.frame.srcFormat = dev->cap.srcFormat;
                    if (param != NULL)
                    {
                        pDisplayReqMsg->payload.data = param;
                    }

                    if (fromISR)
                    {
                        FWK_Message_PutFromIsr(kFWKTaskID_Camera, &pDisplayReqMsg);
                    }
                    else
                    {
                        FWK_Message_Put(kFWKTaskID_Camera, &pDisplayReqMsg);
                    }
                }
            }
        }
        break;

        default:
            break;
    }

    return 0;
}

static int _FWK_DisplayManager_TaskInit(fwk_task_data_t *pTaskData)
{
    if (pTaskData == NULL)
        return -1;

    int error                             = 0;
    display_task_data_t *pDisplayTaskData = (display_task_data_t *)pTaskData;

    /* init the display devices */
    for (int i = 0; i < MAXIMUM_DISPLAY_DEV; i++)
    {
        display_dev_t *pDev = pDisplayTaskData->devs[i];
        if ((pDev != NULL) && (pDev->ops->init != NULL))
        {
            LOGD("[DisplayManager]:INIT dev[%d]", i);
            error = pDev->ops->init(pDev, pDev->cap.width, pDev->cap.height, _FWK_DisplayManager_DeviceCallback, NULL);

            if (error)
            {
                LOGE("[DisplayManager]:INIT dev [%d] error: %d", i, error);
                return error;
            }
        }
    }

    /* start the display devices */
    for (int i = 0; i < MAXIMUM_DISPLAY_DEV; i++)
    {
        display_dev_t *pDev = pDisplayTaskData->devs[i];
        if (pDev != NULL && pDev->ops->start != NULL)
        {
            LOGD("[DisplayManager]:START dev[%d]", i);
            error = pDev->ops->start(pDev);

            if (error)
            {
                LOGE("[DisplayManager]:START display dev [%d] error: %d", i, error);
                return error;
            }
        }
    }

    /* put the display frame request message */
    for (int i = 0; i < MAXIMUM_DISPLAY_DEV; i++)
    {
        display_dev_t *pDev = pDisplayTaskData->devs[i];
        if (pDev != NULL)
        {
            fwk_message_t *pMsg;
            pMsg                          = &pDisplayTaskData->displayRequestMsgs[i];
            pMsg->id                      = kFWKMessageID_DisplayRequestFrame;
            pMsg->payload.devId           = pDev->id;
            pMsg->payload.frame.height    = pDev->cap.height;
            pMsg->payload.frame.width     = pDev->cap.width;
            pMsg->payload.frame.pitch     = pDev->cap.pitch;
            pMsg->payload.frame.left      = pDev->cap.left;
            pMsg->payload.frame.top       = pDev->cap.top;
            pMsg->payload.frame.right     = pDev->cap.right;
            pMsg->payload.frame.bottom    = pDev->cap.bottom;
            pMsg->payload.frame.rotate    = pDev->cap.rotate;
            pMsg->payload.frame.format    = pDev->cap.format;
            pMsg->payload.frame.srcFormat = pDev->cap.srcFormat;
            pMsg->payload.data            = pDev->cap.frameBuffer;
            FWK_Message_Put(kFWKTaskID_Camera, &pMsg);
        }
    }

    return error;
}

static void _FWK_DisplayManager_MessageHandle(fwk_message_t *pMsg, fwk_task_data_t *pTaskData)
{
    if ((pMsg == NULL) || (pTaskData == NULL))
        return;

    display_task_data_t *pDisplayTaskData = (display_task_data_t *)pTaskData;

    switch (pMsg->id)
    {
        case kFWKMessageID_DisplayResponseFrame:
        {
            /* got one display response frame */
            for (int i = 0; i < MAXIMUM_DISPLAY_DEV; i++)
            {
                display_dev_t *pDev = pDisplayTaskData->devs[i];
                if ((pDev != NULL) && (pDev->ops->blit != NULL) && (pDev->id == pMsg->payload.devId))
                {
                    hal_display_status_t status;
                    LOGI("Frame received for display w/ id #%d", pMsg->payload.devId);
                    status = pDev->ops->blit(pDev, pMsg->payload.data, pDev->cap.width, pDev->cap.height);
                    if (status == kStatus_HAL_DisplayRequestFrame)
                    {
                        /* send the display request frame */
                        fwk_message_t *pDisplayReqMsg;
                        pDisplayReqMsg                = &pDisplayTaskData->displayRequestMsgs[i];
                        pDisplayReqMsg->id            = kFWKMessageID_DisplayRequestFrame;
                        pDisplayReqMsg->payload.devId = pMsg->payload.devId;
                        FWK_Message_Put(kFWKTaskID_Camera, &pDisplayReqMsg);
                    }

                    if (status != kStatus_HAL_DisplayTxBusy && status != kStatus_HAL_DisplayError)
                    {
                        /* calculate the fps */
                        fwk_fps(kFWKFPSType_Display, pMsg->frame.devId);
                    }
                }
            }
        }
        break;

        case kFWKMessageID_InputNotify:
        {
            for (int i = 0; i < MAXIMUM_DISPLAY_DEV; i++)
            {
                display_dev_t *pDev = pDisplayTaskData->devs[i];
                if (pDev != NULL && pDev->ops->inputNotify != NULL)
                {
                    hal_display_status_t error = pDev->ops->inputNotify(pDev, pMsg->payload.data);

                    if (error)
                    {
                        LOGE("inputNotify display dev id:%d name:%s error %d", pDev->id, pDev->name, error);
                    }
                }
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
            framework_request_t frameworkRequest    = pMsg->payload.frameworkRequest;
            fwk_task_component_t fwkTaskComponent   = {0};
            framework_response_t framework_response = {0};
            fwkTaskComponent.managerId              = kFWKTaskID_Display;

            for (int i = 0; i < MAXIMUM_DISPLAY_DEV; i++)
            {
                display_dev_t *pDev = pDisplayTaskData->devs[i];
                if (pDev != NULL)
                {
                    fwkTaskComponent.deviceId           = pDev->id;
                    fwkTaskComponent.deviceName         = pDev->name;
                    fwkTaskComponent.configs            = NULL;
                    framework_response.fwkTaskComponent = fwkTaskComponent;
                    if (frameworkRequest.respond != NULL)
                    {
                        frameworkRequest.respond(kFrameworkEvents_GetManagerComponents, &framework_response, false);
                    }
                }
            }
            if (frameworkRequest.respond != NULL)
            {
                frameworkRequest.respond(kFrameworkEvents_GetManagerComponents, NULL, true);
            }
        }
        break;

        default:
            break;
    }
}

int FWK_DisplayManager_Init()
{
    for (int i = 0; i < MAXIMUM_DISPLAY_DEV; i++)
    {
        s_DisplayTask.displayData.devs[i] = NULL;
    }

    return 0;
}

int FWK_DisplayManager_Start(int taskPriority)
{
    LOGD("[DisplayManager]:Starting...");
    int error = 0;

    s_DisplayTask.task.msgHandle  = _FWK_DisplayManager_MessageHandle;
    s_DisplayTask.task.taskInit   = _FWK_DisplayManager_TaskInit;
    s_DisplayTask.task.data       = (fwk_task_data_t *)&(s_DisplayTask.displayData);
    s_DisplayTask.task.taskId     = kFWKTaskID_Display;
    s_DisplayTask.task.delayMs    = 1;
    s_DisplayTask.task.taskStack  = s_DisplayTaskStack;
    s_DisplayTask.task.taskBuffer = s_DisplayTaskTCBBReference;
    FWK_Task_Start((fwk_task_t *)&s_DisplayTask.task, DISPLAY_MANAGER_TASK_NAME, DISPLAY_MANAGER_TASK_STACK,
                   taskPriority);

    LOGD("[DisplayManager]:Started");

    return error;
}

int FWK_DisplayManager_Deinit()
{
    for (int i = 0; i < MAXIMUM_DISPLAY_DEV; i++)
    {
        if ((s_DisplayTask.displayData.devs[i] != NULL) && (s_DisplayTask.displayData.devs[i]->ops->deinit != NULL))
        {
            s_DisplayTask.displayData.devs[i]->ops->deinit(s_DisplayTask.displayData.devs[i]);
        }
    }

    return 0;
}

int FWK_DisplayManager_DeviceRegister(display_dev_t *dev)
{
    int error = -1;

    for (int i = 0; i < MAXIMUM_DISPLAY_DEV; i++)
    {
        if (s_DisplayTask.displayData.devs[i] == NULL)
        {
            dev->id                           = i;
            s_DisplayTask.displayData.devs[i] = dev;
            return 0;
        }
    }

    return error;
}
