/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief standby mode implementation.
 */

#include "board_define.h"
#ifdef ENABLE_LPM_DEV_Standby
#include "fwk_log.h"
#include "fwk_lpm_manager.h"
#include "hal_lpm_dev.h"

#ifdef __cplusplus
extern "C" {
#endif
void lv_enable_ui_preview(bool enable);
void BOARD_BacklightControl(bool on);
#if defined(__cplusplus)
}
#endif

#define LPM_CHECK_TIMER_IN_MS      2000
#define LPM_PRESTANDBY_TIMER_IN_MS 2000

static void _EnterStandbyMode(void)
{
    LOGD("[Standby] Enter standby mode");
    BOARD_BacklightControl(0);
    lv_enable_ui_preview(0);
}

static void HAL_LpmDev_TimerCallback(TimerHandle_t handle)
{
    if (handle == NULL)
    {
        LOGE("Lpm dev is NULL");
        return;
    }

    lpm_dev_t *pDev = (lpm_dev_t *)pvTimerGetTimerID(handle);
    if (pDev->callback != NULL)
    {
        pDev->callback(pDev);
    }
}

static void HAL_LpmDev_PreEnterSleepTimerCallback(TimerHandle_t handle)
{
    if (handle == NULL)
    {
        LOGE("Lpm dev is NULL");
        return;
    }

    lpm_dev_t *pDev = (lpm_dev_t *)pvTimerGetTimerID(handle);
    if (pDev->preEnterSleepCallback != NULL)
    {
        pDev->preEnterSleepCallback(pDev);
    }
}

hal_lpm_status_t HAL_LpmDev_Init(lpm_dev_t *dev,
                                 lpm_manager_timer_callback_t callback,
                                 lpm_manager_timer_callback_t preEnterSleepCallback)
{
    int ret = kStatus_HAL_LpmSuccess;

    dev->callback              = callback;
    dev->preEnterSleepCallback = preEnterSleepCallback;

    dev->timer =
        xTimerCreate("LpmTimer", pdMS_TO_TICKS(LPM_CHECK_TIMER_IN_MS), pdTRUE, (void *)dev, HAL_LpmDev_TimerCallback);
    if (dev->timer == NULL)
    {
        LOGE("Lpm Timer create failed");
        return kStatus_HAL_LpmTimerNull;
    }

    dev->preEnterSleepTimer = xTimerCreate("LpmPreEnterSleepTimer", pdMS_TO_TICKS(LPM_PRESTANDBY_TIMER_IN_MS), pdTRUE,
                                           (void *)dev, HAL_LpmDev_PreEnterSleepTimerCallback);
    if (dev->preEnterSleepTimer == NULL)
    {
        LOGE("Lpm Pre-Enter Sleep Timer create failed");
        return kStatus_HAL_LpmTimerNull;
    }

    dev->lock = xSemaphoreCreateMutex();
    if (dev->lock == NULL)
    {
        LOGE("Create Lpm lock fail");
        return kStatus_HAL_LpmLockNull;
    }

    FWK_LpmManager_SetSleepMode(kLPMMode_STANDBY);

    return ret;
}

hal_lpm_status_t HAL_LpmDev_Deinit(const lpm_dev_t *dev)
{
    int ret = kStatus_HAL_LpmSuccess;

    return ret;
}

hal_lpm_status_t HAL_LpmDev_OpenTimer(const lpm_dev_t *dev)
{
    int ret = kStatus_HAL_LpmSuccess;

    if (dev->timer == NULL)
    {
        LOGE("Lpm Timer is NULL");
        return kStatus_HAL_LpmTimerNull;
    }

    if (xTimerStart(dev->timer, 0) != pdPASS)
    {
        LOGE("Lpm Timer start fail");
        ret = kStatus_HAL_LpmTimerFail;
    }

    return ret;
}

hal_lpm_status_t HAL_LpmDev_StopTimer(const lpm_dev_t *dev)
{
    int ret = kStatus_HAL_LpmSuccess;

    if (dev->timer == NULL)
    {
        LOGE("Lpm Timer is NULL");
        return kStatus_HAL_LpmTimerNull;
    }

    if (xTimerStop(dev->timer, 0) != pdPASS)
    {
        LOGD("Lpm Timer stop fail");
        ret = kStatus_HAL_LpmTimerFail;
    }

    return ret;
}

hal_lpm_status_t HAL_LpmDev_OpenPreEnterSleepTimer(const lpm_dev_t *dev)
{
    int ret = kStatus_HAL_LpmSuccess;

    if (dev->preEnterSleepTimer == NULL)
    {
        LOGE("Lpm Pre-Enter Sleep Timer is NULL");
        return kStatus_HAL_LpmTimerNull;
    }

    if (xTimerStart(dev->preEnterSleepTimer, 0) != pdPASS)
    {
        LOGE("Lpm Pre-Enter Sleep Timer start fail");
        ret = kStatus_HAL_LpmTimerFail;
    }

    return ret;
}

hal_lpm_status_t HAL_LpmDev_StopPreEnterSleepTimer(const lpm_dev_t *dev)
{
    int ret = kStatus_HAL_LpmSuccess;

    if (dev->preEnterSleepTimer == NULL)
    {
        LOGE("Lpm Pre-Enter Sleep Timer is NULL");
        return kStatus_HAL_LpmTimerNull;
    }

    if (xTimerStop(dev->preEnterSleepTimer, 0) != pdPASS)
    {
        LOGE("Lpm Pre-Enter Sleep Timer stop fail");
        ret = kStatus_HAL_LpmTimerFail;
    }

    return ret;
}

hal_lpm_status_t HAL_LpmDev_EnterSleep(const lpm_dev_t *dev, hal_lpm_mode_t mode)
{
    int ret = kStatus_HAL_LpmSuccess;
    LOGD("[Standby] Enter mode %d", mode);
    switch (mode)
    {
        case kLPMMode_STANDBY:
        {
            _EnterStandbyMode();
        }
        break;

        default:
            break;
    }

    return ret;
}

hal_lpm_status_t HAL_LpmDev_Lock(const lpm_dev_t *dev)
{
    uint8_t fromISR = __get_IPSR();

    if (dev->lock == NULL)
    {
        LOGE("Lpm lock is null");
        return kStatus_HAL_LpmLockNull;
    }

    if (fromISR)
    {
        BaseType_t HigherPriorityTaskWoken = pdFALSE;
        if (xSemaphoreTakeFromISR(dev->lock, &HigherPriorityTaskWoken) != pdPASS)
        {
            LOGE("Lpm lock take error");
            return kStatus_HAL_LpmLockError;
        }
    }
    else
    {
        if (xSemaphoreTake(dev->lock, portMAX_DELAY) != pdPASS)
        {
            LOGE("Lpm lock take error");
            return kStatus_HAL_LpmLockError;
        }
    }

    return kStatus_HAL_LpmSuccess;
}

hal_lpm_status_t HAL_LpmDev_Unlock(const lpm_dev_t *dev)
{
    uint8_t fromISR = __get_IPSR();

    if (dev->lock == NULL)
    {
        LOGE("Lpm lock is null");
        return kStatus_HAL_LpmLockNull;
    }

    if (fromISR)
    {
        BaseType_t HigherPriorityTaskWoken = pdFALSE;
        if (xSemaphoreGiveFromISR(dev->lock, &HigherPriorityTaskWoken) != pdPASS)
        {
            LOGE("Lpm lock give error");
            return kStatus_HAL_LpmLockError;
        }
    }
    else
    {
        if (xSemaphoreGive(dev->lock) != pdPASS)
        {
            LOGE("Lpm lock give error");
            return kStatus_HAL_LpmLockError;
        }
    }

    return kStatus_HAL_LpmSuccess;
}

static lpm_dev_operator_t s_LpmDevOperators = {
    .init              = HAL_LpmDev_Init,
    .deinit            = HAL_LpmDev_Deinit,
    .openTimer         = HAL_LpmDev_OpenTimer,
    .stopTimer         = HAL_LpmDev_StopTimer,
    .openPreEnterTimer = HAL_LpmDev_OpenPreEnterSleepTimer,
    .stopPreEnterTimer = HAL_LpmDev_StopPreEnterSleepTimer,
    .enterSleep        = HAL_LpmDev_EnterSleep,
    .lock              = HAL_LpmDev_Lock,
    .unlock            = HAL_LpmDev_Unlock,
};

static lpm_dev_t s_LpmDev = {
    .id  = 0,
    .ops = &s_LpmDevOperators,
};

int HAL_LpmDev_Standby_Register()
{
    int ret = 0;

    FWK_LpmManager_DeviceRegister(&s_LpmDev);

    return ret;
}
#endif /* ENABLE_LPM_DEV_STANDBY */
