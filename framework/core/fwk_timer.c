/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief Framework timer reference implementation.
 */

#include <FreeRTOS.h>
#include "fwk_timer.h"
#include <stdio.h>
#include "fwk_log.h"
#include "fwk_platform.h"

static void _FWK_Timer_Callback(TimerHandle_t handle)
{
    if (handle == NULL)
    {
        LOGE("Timer handle is NULL");
        return;
    }

    fwk_timer_t **pPTimer = (fwk_timer_t **)pvTimerGetTimerID(handle);

    if (pPTimer == NULL)
    {
        LOGE("Cannot get timer ID from timer handle");
        return;
    }

    fwk_timer_t *pTimer = *pPTimer;

    if (pTimer == NULL)
    {
        LOGE("Cannot get timer handle");
        return;
    }

    if (pTimer->function)
    {
        pTimer->function(pTimer->arg);
    }

    /* user might have stopped the timer manual in the callback function */
    pTimer = *pPTimer;
    if (pTimer == NULL)
    {
        LOGI("Timer was stopped in the callback function");
        return;
    }

    /* stop the timer for the non auto reload mode */
    if (!pTimer->autoReload)
    {
        if (FWK_Timer_Stop(pPTimer) != 0)
        {
            LOGE("Failed to stop timer \"%s\"", pTimer->name);
        }
    }
}

int FWK_Timer_Start(
    const char *name, int ms, int autoReload, fwk_timer_callback_t func, void *arg, fwk_timer_t **pPTimer)
{
    *pPTimer = NULL;

    fwk_timer_t *pTimer = (fwk_timer_t *)FWK_MALLOC(sizeof(fwk_timer_t));

    if (pTimer == NULL)
    {
        LOGE("Failed to create timer \"%s\"", name);
        return -1;
    }

    memset(pTimer->name, 0x0, TIMER_NAME_LENGTH);
    sprintf(pTimer->name, name, strlen(name) < TIMER_NAME_LENGTH ? strlen(name) : TIMER_NAME_LENGTH);
    pTimer->function   = func;
    pTimer->arg        = arg;
    pTimer->autoReload = autoReload;
    pTimer->lock       = xSemaphoreCreateMutex();

    if (pTimer->lock == NULL)
    {
        LOGE("Failed to create timer lock");
    }

    UBaseType_t uxAutoReload = pdFALSE;
    if (autoReload)
    {
        uxAutoReload = pdTRUE;
    }

    pTimer->handle = xTimerCreate(name, (TickType_t)pdMS_TO_TICKS(ms), uxAutoReload, pPTimer,
                                  (TimerCallbackFunction_t)_FWK_Timer_Callback);

    if (pTimer->handle == NULL)
    {
        LOGE("Failed to create timer \"%s\"", name);
        FWK_FREE(pTimer);
        pTimer = NULL;
        return -1;
    }

    if (xTimerStart(pTimer->handle, 0) == pdPASS)
    {
        *pPTimer = pTimer;
        LOGI("Timer %s started successfully", name);
    }
    else
    {
        LOGE("Failed to start timer \"%s\"", name);
        xTimerDelete(pTimer->handle, 0);
        FWK_FREE(pTimer);
        pTimer = NULL;
        return -1;
    }

    return 0;
}

int FWK_Timer_Reset(fwk_timer_t **pPTimer)
{
    if (pPTimer == NULL)
    {
        LOGE("pPTimer does not exist");
        return -1;
    }

    fwk_timer_t *pTimer = *pPTimer;

    if ((pTimer != NULL) && (pTimer->handle != NULL) && (xTimerIsTimerActive(pTimer->handle)))
    {
        if (xTimerReset(pTimer->handle, 0) == pdPASS)
        {
            LOGI("Timer \"%s\" reset successfully", pTimer->name);
            return 0;
        }
        else
        {
            LOGE("Failed to reset timer \"%s\"", pTimer->name);
        }
    }

    if (pTimer != NULL)
    {
        LOGE("Timer \"%s\" is not active", pTimer->name);
    }
    else
    {
        LOGE("pTimer does not exist");
    }

    return -1;
}

int FWK_Timer_Stop(fwk_timer_t **pPTimer)
{
    if (pPTimer == NULL)
    {
        LOGE("pPTimer does not exist");
        return -1;
    }

    fwk_timer_t *pTimer = *pPTimer;

    if (pTimer != NULL)
    {
        if (pdTRUE != xSemaphoreTake(pTimer->lock, portMAX_DELAY))
        {
            LOGE("timer stop lock take failed");
        }

        if (pTimer->handle != NULL)
        {
            if (xTimerIsTimerActive(pTimer->handle))
            {
                if (xTimerStop(pTimer->handle, 0) != pdPASS)
                {
                    LOGE("timer %s stopped failed", pTimer->name);
                }
            }

            if (xTimerDelete(pTimer->handle, 0) == pdPASS)
            {
                LOGI("timer %s stopped successfully", pTimer->name);
            }
            else
            {
                LOGE("timer %s stopped failed", pTimer->name);
            }
        }
        else
        {
            LOGE("timer had been stopped");
        }

        LOGI("FWK_Timer_Stop free 0x%x/0x%x", (unsigned int *)pPTimer, (unsigned int *)pTimer);

        SemaphoreHandle_t lock = pTimer->lock;

        FWK_FREE(pTimer);

        if (pdTRUE != xSemaphoreGive(lock))
        {
            LOGE("timer stop lock give failed");
        }

        vSemaphoreDelete(lock);

        pTimer   = NULL;
        *pPTimer = NULL;
        return 0;
    }
    else
    {
        LOGE("pTimer does not exist");
    }

    return -1;
}

uint32_t FWK_Timer_GetRemainingTime(fwk_timer_t **pPTimer)
{
    if (pPTimer == NULL)
    {
        LOGE("[getRemainingTime]: pPTimer does not exist");
        return -1;
    }

    fwk_timer_t *pTimer = *pPTimer;

    if ((pTimer != NULL) && (pTimer->handle != NULL) && (xTimerIsTimerActive(pTimer->handle)))
    {
        return xTimerGetExpiryTime(pTimer->handle) - xTaskGetTickCount();
    }

    if (pTimer != NULL)
    {
        LOGE("Timer \"%s\" is not active", pTimer->name);
    }
    else
    {
        LOGE("pTimer does not exist");
    }

    return -1;
}
