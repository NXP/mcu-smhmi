/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief multicore manager implementation.
 */

#include "fwk_common.h"
#if FWK_SUPPORT_MULTICORE
#include "fwk_platform.h"
#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_task.h"
#include "fwk_multicore_manager.h"
#include "hal_multicore_dev.h"

typedef struct _multicore_task_data
{
    fwk_task_data_t commonData;
    multicore_dev_t *dev; /* Only one device should be register */
} multicore_task_data_t;

typedef struct _multicore_task
{
    fwk_task_t task;
    multicore_task_data_t multicoreData;
} multicore_task_t;

/*
 * multicore manager task
 */
static multicore_task_t s_MulticoreTask;
#if FWK_SUPPORT_STATIC_ALLOCATION
FWKDATA static StackType_t s_MulticoreTaskStack[MULTICORE_MANAGER_TASK_STACK];
FWKDATA static StaticTask_t s_MulticoreTaskTCB;
static void *s_MulticoreTaskTCBBReference = (void *)&s_MulticoreTaskTCB;
#else
static void *s_MulticoreTaskStack         = NULL;
static void *s_MulticoreTaskTCBBReference = NULL;
#endif /* FWK_SUPPORT_STATIC_ALLOCATION */

static int _FWK_MulticoreManager_RecomposeMessage(fwk_message_t *pMsg, void *data, uint32_t dataSize)
{
    int ret = 0;

    switch (pMsg->id)
    {
        case kFWKMessageID_InputReceive:
        case kFWKMessageID_VAlgoResultUpdate:
        case kFWKMessageID_VAlgoASRResultUpdate:
        case kFWKMessageID_InputNotify:
        case kFWKMessageID_CameraRemoteDequeue:
        case kFWKMessageID_CameraRemoteDequeueResponse:
        {
            if (pMsg->payload.size != dataSize)
            {
                LOGE("Data size doesn't match structure raw data size.")
                ret = -1;
            }
            else
            {
                pMsg->payload.data = FWK_MALLOC(pMsg->payload.size);
                if (pMsg->payload.data != NULL)
                {
                    memcpy(pMsg->payload.data, data, dataSize);
                    pMsg->payload.freeAfterConsumed = 1;
                }
                else
                {
                    LOGE("Couldn't allocate memory for raw data.")
                    ret = -1;
                }
            }
        }
        break;

        default:
        {
            LOGE("Unknown message id. Can't recompose the message.");
            ret = -1;
        }
        break;
    }

    return ret;
}

/*
 * multicore dev callback
 */
static int _FWK_MulticoreManager_DeviceCallback(const multicore_dev_t *dev, multicore_event_t event, uint8_t fromISR)
{
    int ret             = 0;
    fwk_message_t *pMsg = NULL;

    switch (event.eventId)
    {
        case kMulticoreEvent_MsgReceive:
        {
            pMsg = FWK_MALLOC(sizeof(fwk_message_t));
            if (pMsg)
            {
                memcpy(pMsg, event.data, sizeof(fwk_message_t));
                pMsg->freeAfterConsumed = 1;

                /* if the receiver is not register, drop the message */
                if (FWK_Task_IsRegistered(pMsg->multicore.taskId) == false)
                {
                    LOGE("Manager is not register on this core.");
                    ret = -1;
                }
                else
                {
                    pMsg->multicore.isMulticoreMessage  = 0;
                    pMsg->multicore.wasMulticoreMessage = 1;
                    pMsg->msgInfo                       = kMsgInfo_Local;
                    if (event.size > sizeof(fwk_message_t))
                    {
                        /* More data. compose message based on event id */
                        uint32_t dataSize = event.size - sizeof(fwk_message_t);
                        ret = _FWK_MulticoreManager_RecomposeMessage(pMsg, (void *)(event.data + sizeof(fwk_message_t)),
                                                                     dataSize);
                    }
                }
            }
            else
            {
                LOGE("Failed to allocate memory for pMsg.");
                ret = -1;
            }
        }
        break;

        default:
            ret = -1;
            break;
    }

    if (ret == 0)
    {
        FWK_Message_Put(pMsg->multicore.taskId, &pMsg);
    }
    else if (pMsg)
    {
        FWK_FREE(pMsg);
    }

    return ret;
}

static int _FWK_MulticoreManager_TaskInit(fwk_task_data_t *pTaskData)
{
    if (pTaskData == NULL)
    {
        return -1;
    }
    int error                                 = 0;
    multicore_task_data_t *pMulticoreTaskData = (multicore_task_data_t *)pTaskData;
    multicore_dev_t *pDev                     = pMulticoreTaskData->dev;

    if (pDev == NULL)
    {
        return 0;
    }

    /* init the multicore device */
    if (pDev->ops->init != NULL)
    {
        error = pDev->ops->init(pDev, _FWK_MulticoreManager_DeviceCallback, NULL);

        if (error)
        {
            LOGE("[MulticoreManager]:INIT error: %d", error);
            return error;
        }
    }
    if (pDev->ops->start != NULL)
    {
        error = pDev->ops->start(pDev);

        if (error)
        {
            LOGE("[MulticoreManager]:START multicore dev error: %d", error);
            return error;
        }
    }

    return error;
}

static void _FWK_MulticoreManager_MessageHandle(fwk_message_t *pMsg, fwk_task_data_t *pTaskData)
{
    if ((pMsg == NULL) || (pTaskData == NULL))
    {
        return;
    }

    multicore_task_data_t *pMulticoreTaskData = (multicore_task_data_t *)pTaskData;
    LOGI("MulticoreManage MsgHandler receive msg with id %d for task: %d", pMsg->id, pMsg->multicore.taskId);
    switch (pMsg->id)
    {
        case kFWKMessageID_InputReceive:
        {
            uint32_t totalSize = pMsg->payload.size + sizeof(fwk_message_t);
            uint8_t *tmpBuffer = FWK_MALLOC(totalSize);
            if (tmpBuffer != NULL)
            {
                /* Make a deep Copy */
                memcpy(tmpBuffer, pMsg, sizeof(fwk_message_t));
                memcpy(tmpBuffer + sizeof(fwk_message_t), pMsg->payload.data, pMsg->payload.size);
                pMulticoreTaskData->dev->ops->send(pMulticoreTaskData->dev, tmpBuffer, totalSize);
                FWK_FREE(tmpBuffer);
            }
            else
            {
                LOGE("Failed to allocate memory for tmpBuffer.");
            }
        }
        break;
        case kFWKMessageID_VAlgoResultUpdate:
        case kFWKMessageID_VAlgoASRResultUpdate:
        case kFWKMessageID_InputNotify:
        {
            if ((pMulticoreTaskData->dev) && (pMulticoreTaskData->dev->ops->send != NULL))
            {
                uint32_t totalSize = pMsg->payload.size + sizeof(fwk_message_t);
                uint8_t *tmpBuffer = FWK_MALLOC(totalSize);
                if (tmpBuffer != NULL)
                {
                    /* Make a deep Copy */
                    memcpy(tmpBuffer, pMsg, sizeof(fwk_message_t));
                    memcpy(tmpBuffer + sizeof(fwk_message_t), pMsg->payload.data, pMsg->payload.size);
                    ((fwk_message_t *)tmpBuffer)->msgInfo = kMsgInfo_Local;
                    pMulticoreTaskData->dev->ops->send(pMulticoreTaskData->dev, tmpBuffer, totalSize);
                    FWK_FREE(tmpBuffer);
                }
                else
                {
                    LOGE("Failed to allocate memory for tmpBuffer.");
                }
            }
        }
        break;
        case kFWKMessageID_DisplayRequestFrame:
        case kFWKMessageID_DisplayResponseFrame:
        case kFWKMessageID_VAlgoRequestFrame:
        case kFWKMessageID_VAlgoResponseFrame:
        case kFWKMessageID_AudioDump:
        case kFWKMessageID_CameraRemoteDequeue:
        case kFWKMessageID_CameraRemoteDequeueResponse:
        {
            if ((pMulticoreTaskData->dev) && (pMulticoreTaskData->dev->ops->send != NULL))
            {
                pMulticoreTaskData->dev->ops->send(pMulticoreTaskData->dev, pMsg, sizeof(fwk_message_t));
            }
        }
        break;
        default:
            break;
    }
}

int FWK_MulticoreManager_Init()
{
    s_MulticoreTask.multicoreData.dev = NULL;
    return 0;
}

int FWK_MulticoreManager_Start(int taskPriority)
{
    int error = 0;
    LOGD("[MulticoreManager]:Starting...");
    s_MulticoreTask.task.msgHandle  = _FWK_MulticoreManager_MessageHandle;
    s_MulticoreTask.task.taskInit   = _FWK_MulticoreManager_TaskInit;
    s_MulticoreTask.task.data       = (fwk_task_data_t *)&(s_MulticoreTask.multicoreData);
    s_MulticoreTask.task.taskId     = kFWKTaskID_Multicore;
    s_MulticoreTask.task.delayMs    = 1;
    s_MulticoreTask.task.taskStack  = s_MulticoreTaskStack;
    s_MulticoreTask.task.taskBuffer = s_MulticoreTaskTCBBReference;
    FWK_Task_Start((fwk_task_t *)&s_MulticoreTask.task, MULTICORE_MANAGER_TASK_NAME, MULTICORE_MANAGER_TASK_STACK,
                   taskPriority);

    LOGD("[MulticoreManager]:Started");

    return error;
}

int FWK_MulticoreManager_Deinit()
{
    if ((s_MulticoreTask.multicoreData.dev != NULL) && (s_MulticoreTask.multicoreData.dev->ops->deinit != NULL))
    {
        s_MulticoreTask.multicoreData.dev->ops->deinit(s_MulticoreTask.multicoreData.dev);
    }

    return 0;
}

int FWK_MulticoreManager_DeviceRegister(multicore_dev_t *dev)
{
    int error = -1;

    if (s_MulticoreTask.multicoreData.dev == NULL)
    {
        dev->id                           = 1;
        s_MulticoreTask.multicoreData.dev = dev;
        return 0;
    }

    return error;
}
#endif /* FWK_SUPPORT_MULTICORE */
