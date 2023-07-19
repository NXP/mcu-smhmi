/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief voice algorithm manager implementation.
 */

#include "fwk_platform.h"
#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_task.h"
#include "fwk_perf.h"
#include "fwk_voice_algo_manager.h"

typedef struct _voice_algo_task_data
{
    fwk_task_data_t commonData;
    /* registered voice algorithm devices */
    voice_algo_dev_t *devs[MAXIMUM_VOICE_ALGO_DEV];

} voice_algo_task_data_t;

typedef struct _voice_algo_task_t
{
    fwk_task_t task;
    voice_algo_task_data_t algoData;
} voice_algo_task_t;

/*
 * voice algorithm manager task
 */
static voice_algo_task_t s_VoiceAlgoTask;

#if FWK_SUPPORT_STATIC_ALLOCATION
FWKDATA static StackType_t s_VoiceAlgoTaskStack[VOICE_ALGO_MANAGER_TASK_STACK];
FWKDATA static StaticTask_t s_VoiceAlgoTaskTCB;
static void *s_VoiceAlgoTaskTCBReference = (void *)&s_VoiceAlgoTaskTCB;
#else
void *s_VoiceAlgoTaskStack        = NULL;
void *s_VoiceAlgoTaskTCBReference = NULL;
#endif

/*
 * voice algorithm dev callback
 */
static int _FWK_VoiceAlgoManager_DeviceCallback(int devId, valgo_event_t event, uint8_t fromISR)
{
    bool msgReady = true;

    switch (event.eventId)
    {
        case kVAlgoEvent_VoiceResultUpdate:
        {
            fwk_message_t *pMsg = (fwk_message_t *)FWK_MALLOC(sizeof(fwk_message_t));
            if (pMsg)
            {
                memset(pMsg, 0, sizeof(fwk_message_t));
                pMsg->freeAfterConsumed = 1;
                pMsg->id                = kFWKMessageID_VAlgoASRResultUpdate;
                pMsg->payload.devId     = devId;
                pMsg->payload.size      = event.size;
                if (event.eventInfo < kEventInfo_Invalid)
                {
                    pMsg->msgInfo = event.eventInfo;
                }
#if FWK_SUPPORT_MULTICORE
                if ((event.eventInfo != kEventInfo_Local) && (event.eventInfo < kEventInfo_Invalid))
                {
                    /* Set this a multicore Message to broadcast results in all the system */
                    pMsg->multicore.isMulticoreMessage = 1;
                    pMsg->multicore.taskId             = kFWKTaskID_Output;
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
                    }
                    else
                    {
                        LOGE("Can't allocate raw.data memory for Message kFWKMessageID_VAlgoASRResultUpdate");
                        msgReady = false;
                    }
                }

                if (msgReady)
                {
                    /* Send the voice algorithm result update to dispatcher */
                    if (fromISR)
                    {
                        FWK_Message_PutFromIsr(kFWKTaskID_Output, &pMsg);
                    }
                    else
                    {
                        FWK_Message_Put(kFWKTaskID_Output, &pMsg);
                    }
                }
                else
                {
                    FWK_FREE(pMsg);
                }
            }
            else
            {
                LOGE("Can't allocate memory for Message kFWKMessageID_VAlgoASRResultUpdate");
            }
        }
        break;

        case kVAlgoEvent_AsrToAfeFeedback:
        case kVAlgoEvent_AsrToAfeProcessed:
        {
            fwk_message_t *pMsg = (fwk_message_t *)FWK_MALLOC(sizeof(fwk_message_t));
            if (pMsg)
            {
                memset(pMsg, 0, sizeof(fwk_message_t));
                pMsg->freeAfterConsumed = 1;
                pMsg->id                = kFWKMessageID_InputNotify;
                pMsg->payload.devId     = devId;
                pMsg->payload.size      = event.size;
                if (event.eventInfo < kEventInfo_Invalid)
                {
                    pMsg->msgInfo = event.eventInfo;
                }

#if FWK_SUPPORT_MULTICORE
                if ((event.eventInfo != kEventInfo_Local) && (event.eventInfo < kEventInfo_Invalid))
                {
                    /* Set this a multicore Message to broadcast results in all the system */
                    pMsg->multicore.isMulticoreMessage = 1;
                    pMsg->multicore.taskId             = kFWKTaskID_Audio;
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
                    }
                    else
                    {
                        LOGE("Can't allocate raw.data memory for Message kVAlgoEvent_AsrToAfeFeedback");
                        msgReady = false;
                    }
                }

                if (msgReady)
                {
                    /* Send ASR feedback (about wake word length) to AFE */
                    if (fromISR)
                    {
                        FWK_Message_PutFromIsr(kFWKTaskID_Audio, &pMsg);
                    }
                    else
                    {
                        FWK_Message_Put(kFWKTaskID_Audio, &pMsg);
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

        case kVAlgoEvent_AsrToAudioDump:
        {
            /* Audio_Dump has low priority, so it can happen that kAudioProcessingEvent_Dump
             * messages are sent multiple times before Audio_Dump can actually process them.
             * Make copies for kAudioProcessingEvent_Dump messages so they do not overlap. */
            fwk_message_t *pMsg = (fwk_message_t *)FWK_MALLOC(sizeof(fwk_message_t));
            if (pMsg)
            {
                memset(pMsg, 0, sizeof(fwk_message_t));

                pMsg->freeAfterConsumed = 1;
                pMsg->id                = kFWKMessageID_AudioDump;
                pMsg->payload.devId     = devId;
                pMsg->payload.size      = event.size;
                pMsg->msgInfo           = event.eventInfo;

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
                    }
                    else
                    {
                        LOGE("Can't allocate raw.data memory for Message kVAlgoEvent_AsrToAudioDump");
                        msgReady = false;
                    }
                }

#if FWK_SUPPORT_MULTICORE
                if ((event.eventInfo != kEventInfo_Local) && (event.eventInfo < kEventInfo_Invalid))
                {
                    /* Set this a multicore Message to broadcast results in all the system */
                    pMsg->multicore.isMulticoreMessage = 1;
                    pMsg->multicore.taskId             = kFWKTaskID_Output;
                }
#endif /* FWK_SUPPORT_MULTICORE */

                if (msgReady)
                {
                    if (fromISR)
                    {
                        FWK_Message_PutFromIsr(kFWKTaskID_Output, &pMsg);
                    }
                    else
                    {
                        FWK_Message_Put(kFWKTaskID_Output, &pMsg);
                    }
                }
                else
                {
                    FWK_FREE(pMsg);
                }
            }
            else
            {
                LOGE("Can't allocate memory for Message kVAlgoEvent_AsrToAudioDump");
            }
        }
        break;

        default:
            break;
    }

    return 0;
}

static int _FWK_VoiceAlgoManager_TaskInit(fwk_task_data_t *pTaskData)
{
    if (pTaskData == NULL)
        return -1;

    int error                             = 0;
    hal_valgo_status_t hal_ret            = kStatus_HAL_ValgoError;
    voice_algo_task_data_t *pAlgoTaskData = (voice_algo_task_data_t *)pTaskData;

    /* Init ASR voice dev */
    for (int i = 0; i < MAXIMUM_VOICE_ALGO_DEV; i++)
    {
        voice_algo_dev_t *pDev = pAlgoTaskData->devs[i];
        if (pDev != NULL && pDev->ops->init != NULL)
        {
            LOGD("INIT voice algo dev[%d]", i);
            hal_ret = pDev->ops->init(pDev, _FWK_VoiceAlgoManager_DeviceCallback, NULL);

            if (hal_ret)
            {
                LOGE("INIT voice algo dev [%d] error: %d", i, hal_ret);
                error = -1;
                continue;
            }
        }
    }

    return error;
}

static void _FWK_VoiceAlgoManager_MessageHandle(fwk_message_t *pMsg, fwk_task_data_t *pTaskData)
{
    if ((pMsg == NULL) || (pTaskData == NULL))
        return;

    voice_algo_task_data_t *pAlgoTaskData = (voice_algo_task_data_t *)pTaskData;

    switch (pMsg->id)
    {
        case kFWKMessageID_VAlgoASRInputProcess:
        {
            voice_algo_dev_t *pDev = pAlgoTaskData->devs[pMsg->payload.devId];
            if ((pDev != NULL) && (pDev->ops->run != NULL))
            {
                hal_valgo_status_t status;
                status = pDev->ops->run(pDev, &pMsg->payload);

                if (status == kStatus_HAL_ValgoSuccess)
                {
                    /* TODO anything */
                }
            }
        }
        break;

        case kFWKMessageID_InputNotify:
        {
            for (int i = 0; i < MAXIMUM_VOICE_ALGO_DEV; i++)
            {
                voice_algo_dev_t *pDev = pAlgoTaskData->devs[i];
                if ((pDev != NULL) && (pDev->ops->inputNotify != NULL))
                {
                    hal_valgo_status_t status = pDev->ops->inputNotify(pDev, pMsg->payload.data);

                    if (status != kStatus_HAL_ValgoSuccess)
                    {
                        LOGE("inputNotify valgo dev %s failed with error: %d", pDev->name, status);
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
            fwkTaskComponent.managerId              = kFWKTaskID_VoiceAlgo;

            for (int i = 0; i < MAXIMUM_VOICE_ALGO_DEV; i++)
            {
                voice_algo_dev_t *pDev = pAlgoTaskData->devs[i];
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

int FWK_VoiceAlgoManager_Init()
{
    memset(s_VoiceAlgoTask.algoData.devs, 0, sizeof(s_VoiceAlgoTask.algoData.devs));

    return 0;
}

int FWK_VoiceAlgoManager_Start(int taskPriority)
{
    /* create the voice algorithm manager task */
    LOGD("[voice_algo_manager]:Starting...");
    int error = 0;

    s_VoiceAlgoTask.task.msgHandle  = _FWK_VoiceAlgoManager_MessageHandle;
    s_VoiceAlgoTask.task.taskInit   = _FWK_VoiceAlgoManager_TaskInit;
    s_VoiceAlgoTask.task.data       = (fwk_task_data_t *)&(s_VoiceAlgoTask.algoData);
    s_VoiceAlgoTask.task.taskId     = kFWKTaskID_VoiceAlgo;
    s_VoiceAlgoTask.task.delayMs    = 1;
    s_VoiceAlgoTask.task.taskStack  = s_VoiceAlgoTaskStack;
    s_VoiceAlgoTask.task.taskBuffer = s_VoiceAlgoTaskTCBReference;
    FWK_Task_Start((fwk_task_t *)&s_VoiceAlgoTask.task, VOICE_ALGO_MANAGER_TASK_NAME, VOICE_ALGO_MANAGER_TASK_STACK,
                   taskPriority);

    LOGD("[voice_algo_manager]:Started");
    return error;
}

int FWK_VoiceAlgoManager_Deinit()
{
    return 0;
}

int FWK_VoiceAlgoManager_DeviceRegister(voice_algo_dev_t *dev)
{
    int error = -1;

    for (int i = 0; i < MAXIMUM_VOICE_ALGO_DEV; i++)
    {
        if (s_VoiceAlgoTask.algoData.devs[i] == NULL)
        {
            dev->id                          = i;
            s_VoiceAlgoTask.algoData.devs[i] = dev;
            return 0;
        }
    }

    return error;
}
