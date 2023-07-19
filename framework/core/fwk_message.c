/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief framework message implementation.
 */

#include "fwk_platform.h"
#include "fwk_log.h"
#include "fwk_message.h"

static QueueHandle_t s_MessageQueue[kFWKTaskID_COUNT];

static const char *s_MessageNameStr[kFWKMessageID_Invalid + 1] = {
    "camera_dq", "camera_rdq", "camera_rdq_res", "camera_set", "display_req", "display_res",
    /* vision algorithm manager message*/
    "alg_req_frame", "alg_respond_frame", "alg_result_update",

    /* event dispatcher task send message*/
    "dispatch_overlay",
    /* input task input triggered*/
    "input_recv", "inputNotify", "raw_msg", "invalid"};

const char *FWK_Message_Name(fwk_message_id_t id)
{
    if (id >= 0 && id < kFWKMessageID_Invalid)
    {
        return s_MessageNameStr[id];
    }

    return s_MessageNameStr[kFWKMessageID_Invalid - 1];
}

BaseType_t FWK_Message_RegisterQueue(fwk_task_id_t taskId, QueueHandle_t queueHandle)
{
    BaseType_t ret = pdTRUE;

    if (taskId < kFWKTaskID_COUNT)
    {
        s_MessageQueue[taskId] = queueHandle;
    }
    else
    {
        ret = pdFALSE;
    }

    LOGI("[Message]: Queue %d register", taskId);
    return ret;
}

BaseType_t FWK_Message_PutFromIsr(fwk_task_id_t taskId, fwk_message_t **ppMsg)
{
    BaseType_t ret                     = pdTRUE;
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    msg_info_t msg_info                = (*ppMsg)->msgInfo;

#if FWK_SUPPORT_MULTICORE
    if ((*ppMsg)->multicore.isMulticoreMessage == 1)
    {
        /* send the same message to multicore manager */
        LOGISRV("Task:[%d] put from isr message:[0x%p]:[%d]", kFWKTaskID_Multicore, (*ppMsg), (*ppMsg)->id);
        if (s_MessageQueue[kFWKTaskID_Multicore] == 0)
        {
            LOGISRD("Task:[%d] is invalid", kFWKTaskID_Multicore);
        }
        else
        {
            ret =
                xQueueSendToBackFromISR(s_MessageQueue[kFWKTaskID_Multicore], (void *)ppMsg, &higherPriorityTaskWoken);

            if (ret != pdTRUE)
            {
                LOGISRE("Task:[%d] put from isr message:[0x%p]:[%d] error %d", kFWKTaskID_Multicore, (*ppMsg),
                        (*ppMsg)->id, (int)ret);
            }
            else
            {
                LOGISRV("Task:[%d] put from isr message:[0x%p]:[%d] done", kFWKTaskID_Multicore, (*ppMsg),
                        (*ppMsg)->id);
            }
        }
    }
#endif /* FWK_SUPPORT_MULTICORE */

    LOGISRV("Task:[%d] put from isr message:[0x%p]:[%d]", taskId, (*ppMsg), (*ppMsg)->id);

    if ((msg_info == kMsgInfo_Remote) || (s_MessageQueue[taskId] == 0))
    {
        /* Multicore has already freed the message, return */
        return ret;
    }

    ret = xQueueSendToBackFromISR(s_MessageQueue[taskId], (void *)ppMsg, &higherPriorityTaskWoken);

    if (ret != pdTRUE)
    {
        LOGISRE("Task:[%d] put from isr message:[0x%p]:[%d] error %d", taskId, (*ppMsg), (*ppMsg)->id, (int)ret);
    }
    else
    {
        LOGISRV("Task:[%d] put from isr message:[0x%p]:[%d] done", taskId, (*ppMsg), (*ppMsg)->id);
    }

#if RT_PLATFORM
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
#endif

    return ret;
}

BaseType_t FWK_Message_Put(fwk_task_id_t taskId, fwk_message_t **ppMsg)
{
    BaseType_t ret      = pdTRUE;
    msg_info_t msg_info = (*ppMsg)->msgInfo;

#if FWK_SUPPORT_MULTICORE
    if ((*ppMsg)->multicore.isMulticoreMessage == 1)
    {
        /* send the same message to multicore manager */
        LOGV("Task:[%d] put message:[0x%p]:[%d]", kFWKTaskID_Multicore, (*ppMsg), (*ppMsg)->id);
        if (s_MessageQueue[kFWKTaskID_Multicore] == 0)
        {
            LOGD("Task:[%d] is invalid", kFWKTaskID_Multicore);
        }
        else
        {
            ret = xQueueSend(s_MessageQueue[kFWKTaskID_Multicore], (void *)ppMsg, (TickType_t)pdMS_TO_TICKS(0));

            if (ret != pdTRUE)
            {
                LOGE("Task:[%d] put message:[0x%p]:[%d] error %d", kFWKTaskID_Multicore, (*ppMsg), (*ppMsg)->id,
                     (int)ret);
            }
            else
            {
                LOGV("Task:[%d] put message:[0x%p]:[%d] done", kFWKTaskID_Multicore, (*ppMsg), (*ppMsg)->id);
            }
        }
    }
#endif /* FWK_SUPPORT_MULTICORE */

    LOGV("Task:[%d] put message:[0x%p]:[%d]", taskId, (*ppMsg), (*ppMsg)->id);

    if ((msg_info == kMsgInfo_Remote) || (s_MessageQueue[taskId] == 0))
    {
        /* Multicore has already freed the message, return */
        return ret;
    }

    ret = xQueueSend(s_MessageQueue[taskId], (void *)ppMsg, (TickType_t)pdMS_TO_TICKS(0));

    if (ret != pdTRUE)
    {
        LOGE("Task:[%d] put message:[0x%p]:[%d] error %d", taskId, (*ppMsg), (*ppMsg)->id, (int)ret);
    }
    else
    {
        LOGV("Task:[%d] put message:[0x%p]:[%d] done", taskId, (*ppMsg), (*ppMsg)->id);
    }

    return ret;
}

BaseType_t FWK_Message_Get(fwk_task_id_t taskId, fwk_message_t **ppMsg)
{
    BaseType_t ret = pdTRUE;
    LOGV("Task:[%d] get message", taskId);

    if (s_MessageQueue[taskId] == 0)
    {
        return ret;
    }

    ret = xQueueReceive(s_MessageQueue[taskId], (void *)(ppMsg), portMAX_DELAY);

    if (ret != pdTRUE)
    {
        LOGE("Task:[%d] get message error %d", taskId, (int)ret);
    }
    else
    {
        LOGV("Task:[%d] get message:[0x%p]:[%d] done", taskId, (*ppMsg), (*ppMsg)->id);
    }

    return ret;
}
