/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief input device manager framework implementation.
 */

#include "fwk_platform.h"
#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_task.h"
#include "fwk_input_manager.h"
#include "fwk_graphics.h"
#include "hal_input_dev.h"

typedef struct
{
    fwk_task_data_t commonData;
    input_dev_t *devs[MAXIMUM_INPUT_DEV];  /* registered input devices */
    fwk_message_t msgs[MAXIMUM_INPUT_DEV]; /* input messages */
} input_task_data_t;

typedef struct
{
    fwk_task_t task;
    input_task_data_t inputData;
} input_task_t;

static int _FWK_InputManager_TaskInit(fwk_task_data_t *pTaskData);
static int _FWK_InputManager_DeviceCallback(const input_dev_t *dev, input_event_t *event, uint8_t fromISR);
static void _FWK_InputManager_MessageHandle(fwk_message_t *pMsg, fwk_task_data_t *pTaskData);

/*
 * input manager task
 */
static input_task_t s_InputTask;

#if FWK_SUPPORT_STATIC_ALLOCATION
FWKDATA static StackType_t s_InputTaskStack[INPUT_MANAGER_TASK_STACK];
FWKDATA static StaticTask_t s_InputTaskTCB;
static void *s_InputTaskTCBReference = (void *)&s_InputTaskTCB;
#else
static void *s_InputTaskStack        = NULL;
static void *s_InputTaskTCBReference = NULL;
#endif

static void _FWK_InputManager_FrameworkMessageHandler(framework_request_t frameworkRequest)
{
    switch (frameworkRequest.frameworkEventId)
    {
        case kFrameworkEvents_GetManagerInfo:
        {
            uint8_t count;
            FWK_Task_GetCount(&count);

            for (int id = 0; id < kFWKTaskID_COUNT && count > 0; id++)
            {
                char *name;
                uint32_t priority;

                if (FWK_Task_GetInfo(id, &name, &priority) == 0)
                {
                    fwk_task_info_t fwkTaskInfo             = {.name = name, .managerId = id, .priority = priority};
                    framework_response_t framework_response = {.fwkTaskInfo = fwkTaskInfo};
                    count--;
                    if (frameworkRequest.respond != NULL)
                    {
                        frameworkRequest.respond(kFrameworkEvents_GetManagerInfo, &framework_response, 0);
                    }
                }
            }
            if (frameworkRequest.respond != NULL)
            {
                frameworkRequest.respond(kFrameworkEvents_GetManagerInfo, NULL, 1);
            }
        }
        break;
        case kFrameworkEvents_GetManagerComponents:
        {
            if (FWK_Task_IsRegistered(frameworkRequest.managerId))
            {
                fwk_message_t *pMsg = (fwk_message_t *)FWK_MALLOC(sizeof(fwk_message_t));
                if (pMsg != NULL)
                {
                    memset(pMsg, 0, sizeof(fwk_message_t));
                    pMsg->freeAfterConsumed        = 1;
                    pMsg->id                       = kFWKMessageID_InputFrameworkGetComponents;
                    pMsg->payload.frameworkRequest = frameworkRequest;
                    FWK_Message_Put(frameworkRequest.managerId, &pMsg);
                }
                else
                {
                    LOGE("Can't allocate memory for msg in kFrameworkEvents_GetManagerComponents.")
                }
            }
            else if (frameworkRequest.respond != NULL)
            {
                frameworkRequest.respond(kFrameworkEvents_GetManagerComponents, NULL, true);
            }
        }
        break;
        default:
            break;
    }
}

static int _FWK_InputManager_TaskInit(fwk_task_data_t *pTaskData)
{
    if (pTaskData == NULL)
        return -1;
    int error                         = 0;
    input_task_data_t *pInputTaskData = (input_task_data_t *)pTaskData;

    /* init the input dev */
    for (int i = 0; i < MAXIMUM_INPUT_DEV; i++)
    {
        input_dev_t *pDev = pInputTaskData->devs[i];
        if (pDev != NULL && pDev->ops->init != NULL)
        {
            LOGD("INIT input dev[%d]", i);
            error = pDev->ops->init(pDev, _FWK_InputManager_DeviceCallback);

            if (error)
            {
                LOGE("INIT input dev [%d] error: %d", i, error);
            }
        }
    }

    for (int i = 0; i < MAXIMUM_INPUT_DEV; i++)
    {
        input_dev_t *pDev = pInputTaskData->devs[i];
        if (pDev != NULL && pDev->ops->start != NULL)
        {
            LOGD("START input dev [%d]", i);
            error = pDev->ops->start(pDev);

            if (error)
            {
                LOGE("START input dev [%d] error: %d", i, error);
                return error;
            }
        }
    }

    return error;
}

static void _FWK_InputManager_MessageHandle(fwk_message_t *pMsg, fwk_task_data_t *pTaskData)
{
    LOGI("[InputManager]:Received message:[%s]", FWK_Message_Name(pMsg->id));

    if ((pMsg == NULL) || (pTaskData == NULL))
        return;

    input_task_data_t *pInputTaskData = (input_task_data_t *)pTaskData;

    switch (pMsg->id)
    {
        case kFWKMessageID_InputFrameworkReceived:
        {
            framework_request_t frameworkRequest = pMsg->payload.frameworkRequest;
            _FWK_InputManager_FrameworkMessageHandler(frameworkRequest);
        }
        break;

        case kFWKMessageID_InputNotify:
        {
            for (int i = 0; i < MAXIMUM_INPUT_DEV; i++)
            {
                input_dev_t *pDev = pInputTaskData->devs[i];
                if ((pDev != NULL) && (pDev->ops->inputNotify != NULL))
                {
                    hal_input_status_t error = pDev->ops->inputNotify(pDev, pMsg->payload.data);

                    if (error)
                    {
                        LOGE("inputNotify input dev id:%d name:%s error %d", pDev->id, pDev->name, error);
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

        case kFWKMessageID_InputReceive:
        {
            input_dev_t *pDev = NULL;

            for (int i = 0; i < MAXIMUM_INPUT_DEV; i++)
            {
                pDev = pInputTaskData->devs[i];
                if (pDev && (pDev->id == pMsg->payload.devId))
                {
                    break;
                }
            }

            if ((pDev != NULL)
#if FWK_SUPPORT_MULTICORE
                || (pMsg->multicore.wasMulticoreMessage == true)
#endif /* FWK_SUPPORT_MULTICORE */
            )
            {
                uint32_t receiverList = pMsg->payload.input.receiverList;

                for (int i = kFWKTaskID_Camera; i < kFWKTaskID_COUNT; i++)
                {
                    if (receiverList & 1 << i)
                    {
                        if (FWK_Task_IsRegistered(i) == false)
                        {
                            continue;
                        }

                        fwk_message_t *Msg = (fwk_message_t *)FWK_MALLOC(sizeof(fwk_message_t));
                        if (Msg != NULL)
                        {
                            memset(Msg, 0, sizeof(fwk_message_t));
                            Msg->freeAfterConsumed = 1;
                            Msg->id                = kFWKMessageID_InputNotify;
                            Msg->payload.devId     = pMsg->payload.devId;
                            if (pMsg->payload.input.copy)
                            {
                                Msg->payload.data = FWK_MALLOC(pMsg->payload.size);

                                if (Msg->payload.data != NULL)
                                {
                                    memcpy(Msg->payload.data, pMsg->payload.data, pMsg->payload.size);
                                    Msg->payload.freeAfterConsumed = 1;
                                }
                                else
                                {
                                    LOGE("Can't allocate memory for msg raw data in kFWKMessageID_InputReceive.");
                                    FWK_FREE(Msg);
                                    continue;
                                }
                            }
                            else
                            {
                                Msg->payload.data              = pMsg->payload.data;
                                Msg->payload.freeAfterConsumed = 0;
                            }
                            Msg->payload.size = pMsg->payload.size;
                            FWK_Message_Put(i, &Msg);
                        }
                        else
                        {
                            LOGE("Can't allocate memory for msg in kFWKMessageID_InputReceive.");
                        }
                    }
                }
            }

            if (pMsg->payload.freeAfterConsumed)
            {
                pMsg->payload.freeAfterConsumed = 0;
                FWK_FREE(pMsg->payload.data);
                pMsg->payload.data = NULL;
            }
        }
        break;

        case kFWKMessageID_InputFrameworkGetComponents:
        {
            framework_request_t frameworkRequest    = pMsg->payload.frameworkRequest;
            fwk_task_component_t fwkTaskComponent   = {0};
            framework_response_t framework_response = {0};
            fwkTaskComponent.managerId              = kFWKTaskID_Input;

            for (int i = 0; i < MAXIMUM_INPUT_DEV; i++)
            {
                input_dev_t *pDev = pInputTaskData->devs[i];
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

/*
 * input dev callback
 */
static int _FWK_InputManager_DeviceCallback(const input_dev_t *dev, input_event_t *event, uint8_t fromISR)
{
    fwk_message_t *pMsg  = NULL;
    fwk_task_id_t taskId = kFWKTaskID_Camera;

    for (int i = 0; i < MAXIMUM_INPUT_DEV; i++)
    {
        if (s_InputTask.inputData.devs[i]->id == dev->id)
        {
            pMsg = &s_InputTask.inputData.msgs[i];
            break;
        }
    }

    if (pMsg == NULL)
    {
        return -1;
    }

    switch (event->eventId)
    {
        case kInputEventID_FrameworkRecv:
        {
            pMsg->id                       = kFWKMessageID_InputFrameworkReceived;
            pMsg->payload.frameworkRequest = *(event->u.frameworkRequest);
            taskId                         = kFWKTaskID_Input;
        }
        break;

        case kInputEventID_Recv:
        {
            pMsg->id                         = kFWKMessageID_InputReceive;
            pMsg->payload.devId              = dev->id;
            pMsg->payload.input.receiverList = event->u.inputData.receiverList;
            pMsg->payload.data               = event->u.inputData.data;
            pMsg->payload.input.copy         = event->u.inputData.copy;
            pMsg->payload.size               = event->size;
            pMsg->payload.freeAfterConsumed  = 0;
#if FWK_SUPPORT_MULTICORE
            if (event->eventInfo != kEventInfo_Local && event->eventInfo < kEventInfo_Invalid)
            {
                pMsg->payload.input.copy           = 1;
                pMsg->multicore.isMulticoreMessage = 1;
                pMsg->multicore.taskId             = kFWKTaskID_Input;
            }
#endif /* FWK_SUPPORT_MULTICORE */
            taskId = kFWKTaskID_Input;
        }
        break;

        case kInputEventID_AudioRecv:
        {
            pMsg->id                        = kFWKMessageID_InputAudioReceived;
            pMsg->payload.devId             = dev->id;
            pMsg->payload.data              = event->u.audioData;
            pMsg->payload.freeAfterConsumed = 0;
            taskId                          = kFWKTaskID_Audio;
        }
        break;
        default:
            break;
    }

    if (fromISR)
    {
        FWK_Message_PutFromIsr(taskId, &pMsg);
    }
    else
    {
        FWK_Message_Put(taskId, &pMsg);
    }

    return 0;
}

int FWK_InputManager_Init()
{
    for (int i = 0; i < MAXIMUM_INPUT_DEV; i++)
    {
        s_InputTask.inputData.devs[i] = NULL;
    }

    for (int i = 0; i < MAXIMUM_INPUT_DEV; i++)
    {
        fwk_message_t *pMsg = &s_InputTask.inputData.msgs[i];
        pMsg->id            = kFWKMessageID_InputReceive;
        pMsg->payload.devId = i;
    }

    return 0;
}

int FWK_InputManager_Start(int taskPriority)
{
    /* create the input manager task */
    LOGD("[InputManager]:Starting...");
    int error = 0;

    s_InputTask.task.msgHandle  = _FWK_InputManager_MessageHandle;
    s_InputTask.task.taskInit   = _FWK_InputManager_TaskInit;
    s_InputTask.task.data       = (fwk_task_data_t *)&(s_InputTask.inputData);
    s_InputTask.task.taskId     = kFWKTaskID_Input;
    s_InputTask.task.delayMs    = 1;
    s_InputTask.task.taskStack  = s_InputTaskStack;
    s_InputTask.task.taskBuffer = s_InputTaskTCBReference;
    FWK_Task_Start((fwk_task_t *)&s_InputTask.task, INPUT_MANAGER_TASK_NAME, INPUT_MANAGER_TASK_STACK, taskPriority);

    LOGD("[InputManager]:Started");
    return error;
}

int FWK_InputManager_Deinit()
{
    return 0;
}

int FWK_InputManager_DeviceRegister(input_dev_t *dev)
{
    int error = -1;

    for (int i = 0; i < MAXIMUM_INPUT_DEV; i++)
    {
        if (s_InputTask.inputData.devs[i] == NULL)
        {
            dev->id                       = i;
            s_InputTask.inputData.devs[i] = dev;
            return 0;
        }
    }

    return error;
}
