/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief Framework task implementation.
 */

#include "fwk_platform.h"
#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_task.h"

#define mainQUEUE_LENGTH (10)

static TaskHandle_t s_TaskList[kFWKTaskID_COUNT];

static void _fwk_task_proc(void *pvParameters)
{
    fwk_message_t *pMsg;
    fwk_task_t *slnTask = (fwk_task_t *)pvParameters;

    LOGD("Task:[%p]:[%d]:[%p] Started", slnTask, slnTask->taskId, slnTask->data->queueHandle);

    if (slnTask->taskInit)
    {
        if (slnTask->taskInit(slnTask->data) != 0)
        {
            LOGE("Task:[%p]:[%d]:Task init failed", slnTask, slnTask->taskId);
            while (1)
                ;
        }
    }

    while (1)
    {
        LOGV("Task:[%p]:[%d]:[%p] Waiting to receive message", slnTask, slnTask->taskId, slnTask->data->queueHandle);

        if (slnTask->data == NULL)
        {
            LOGE("Task data is empty");
            while (1)
                ;
        }
        BaseType_t ret = xQueueReceive(slnTask->data->queueHandle, (void *)&pMsg, portMAX_DELAY);

        if (ret == pdTRUE)
        {
            LOGV("Task:[%p]:[%d]:[%p] Received message:[%p]", slnTask, slnTask->taskId, slnTask->data->queueHandle,
                 pMsg);
            slnTask->msgHandle(pMsg, slnTask->data);
        }
        else
        {
            LOGE("Task:[%p]:[%d]:[%p] Received error message:[%d]", slnTask, slnTask->taskId,
                 slnTask->data->queueHandle, (int)ret);
        }

        /* Multicore task shouldn't free the message */
        if (pMsg && (pMsg->freeAfterConsumed))
        {
#if FWK_SUPPORT_MULTICORE
            /* Don't free if the message is multicore and the task is the multicore task */
            if ((pMsg->multicore.isMulticoreMessage == 0) || (kFWKTaskID_Multicore != slnTask->taskId))
            {
                pMsg->freeAfterConsumed = 0;
                FWK_FREE(pMsg);
            }
            /* free the multicore message if it is only for remote */
            else if ((kFWKTaskID_Multicore == slnTask->taskId) && (pMsg->multicore.isMulticoreMessage == 1) &&
                (pMsg->msgInfo == kMsgInfo_Remote))
            {
                /* free the payload */
                if (pMsg->payload.freeAfterConsumed)
                {
                    pMsg->payload.freeAfterConsumed = 0;
                    FWK_FREE(pMsg->payload.data);
                }
                pMsg->freeAfterConsumed = 0;
                FWK_FREE(pMsg);
            }
#else
            /* free the payload */
            if (pMsg->payload.freeAfterConsumed)
            {
                pMsg->payload.freeAfterConsumed = 0;
                FWK_FREE(pMsg->payload.data);
            }

            pMsg->freeAfterConsumed = 0;
            FWK_FREE(pMsg);
#endif /*FWK_SUPPORT_MULTICORE*/
        }

        if (slnTask->delayMs > 0)
        {
            vTaskDelay(pdMS_TO_TICKS(slnTask->delayMs));
        }
    }
}
static uint32_t _fwk_task_get_prio(TaskHandle_t task)
{
    return uxTaskPriorityGet(task);
}

static char *_fwk_task_get_name(TaskHandle_t task)
{
    return pcTaskGetName(task);
}

int FWK_Task_GetInfo(fwk_task_id_t taskId, char **name, uint32_t *priority)
{
    if ((name == NULL) || (priority == NULL) || (s_TaskList[taskId] == NULL))
    {
        return -1;
    }

    *name     = _fwk_task_get_name(s_TaskList[taskId]);
    *priority = _fwk_task_get_prio(s_TaskList[taskId]);

    return 0;
}

int FWK_Task_GetCount(uint8_t *count)
{
    if (count == NULL)
    {
        return -1;
    }

    *count = 0;
    for (int id = 0; id < kFWKTaskID_APPStart; id++)
    {
        if (s_TaskList[id] != NULL)
        {
            (*count)++;
        }
    }
    return 0;
}

bool FWK_Task_IsRegistered(fwk_task_id_t taskId)
{
    return s_TaskList[taskId] != NULL ? true : false;
}

void FWK_Task_Start(fwk_task_t *pTask, const char *taskName, int taskStackSize, int taskPriority)
{
    if (pTask == NULL)
    {
        LOGE("\"%s\" Task is empty", taskName);
        while (1)
            ;
    }
    if (pTask->data == NULL)
    {
        LOGE("\"%s\" Task data is empty, please allocate memory for the task data ", taskName);
        while (1)
            ;
    }
    unsigned payloadSize     = sizeof(fwk_message_t *);
    pTask->data->queueHandle = xQueueCreate(mainQUEUE_LENGTH, payloadSize);

    LOGD("Task:[%p]:[%d]:[%p]:[%s] Start", pTask, pTask->taskId, pTask->data->queueHandle, taskName);

    if ((taskPriority >= 0) && (taskPriority <= configMAX_PRIORITIES - 1))
    {
        taskPriority = configMAX_PRIORITIES - 1 - taskPriority;
    }
    else
    {
        LOGE("\"%s\" Invalid task priority", taskName);
        taskPriority = 0;
    }

    if (NULL != pTask->data->queueHandle)
    {
        FWK_Message_RegisterQueue(pTask->taskId, pTask->data->queueHandle);

#if FWK_SUPPORT_STATIC_ALLOCATION
        if ((pTask->taskStack != NULL) && (pTask->taskBuffer != NULL))
        {
            s_TaskList[pTask->taskId] = xTaskCreateStatic(_fwk_task_proc, taskName, taskStackSize, pTask, taskPriority,
                                                          pTask->taskStack, pTask->taskBuffer);
            if (s_TaskList[pTask->taskId] == NULL)
            {
                LOGE("Task \"%s\" creation failed", taskName);

                while (1)
                    ;
            }
        }
        else
#endif
        {
            if (xTaskCreate(_fwk_task_proc, taskName, taskStackSize, pTask, taskPriority, &s_TaskList[pTask->taskId]) !=
                pdPASS)
            {
                LOGE("Task \"%s\" creation failed", taskName);

                while (1)
                    ;
            }
        }
    }
}
