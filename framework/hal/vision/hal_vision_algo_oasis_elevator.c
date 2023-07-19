/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the license terms that accompany it. By expressly accepting
 * such terms or by downloading, installing, activating and/or otherwise using
 * the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software.
 */

/*
 * @brief SLN Oasis-Lite Vision Algorithm HAL driver implementation for the elevator application.
 */

#include "board_define.h"
#ifdef ENABLE_VISIONALGO_DEV_Oasis_Elevator
#include <FreeRTOS.h>
#include <stdlib.h>
#include <stdio.h>
#include <task.h>
#include "fwk_log.h"
#include "fwk_platform.h"
#include "fwk_vision_algo_manager.h"
#include "fwk_profiler.h"
#include "fwk_lpm_manager.h"
#include "fwk_timer.h"

#include "hal_lpm_dev.h"
#include "hal_vision_algo.h"
#include "smart_tlhmi_event_descriptor.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define OASISLOG_ENABLE 1
#if OASISLOG_ENABLE
#define OASIS_LOGI LOGI
#define OASIS_LOGD LOGD
#define OASIS_LOGE LOGE
#define OASIS_LOGV LOGV
#else
#define OASIS_LOGI(...)
#define OASIS_LOGD(...)
#define OASIS_LOGE(...)
#define OASIS_LOGV(...)
#endif

typedef struct _oasis_param
{
    OASISLTInitPara_t config;
    vision_algo_result_t result;
    ImageFrame_t frames[OASISLT_INT_FRAME_IDX_LAST];
    ImageFrame_t *pframes[OASISLT_INT_FRAME_IDX_LAST];
    OASISRunFlag_t currRunFlag;
    OASISRunFlag_t prevRunFlag;
    vision_algo_dev_t *dev;
    oasis_lite_mode_t mode;
} oasis_param_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
static oasis_param_t s_OasisElevator;
static const facedb_ops_t *s_pFacedbOps         = NULL;
static const elevatordb_ops_t *s_pElevatordbOps = NULL;
static uint8_t *s_pFaceFeature                  = NULL;
static int16_t s_faceId                         = -1;
static elevator_attr_t s_elevatorAttr;
static char s_userName[FACE_NAME_MAX_LEN];
static uint16_t s_blockingList    = 0;
static unsigned int s_debugOption = false;

#if OASIS_STATIC_MEM_BUFFER
__attribute__((section(".bss.$SRAM_OCRAM_CACHED"), aligned(64))) uint8_t g_OasisMemPool[OASIS_STATIC_MEM_POOL];
#endif

AT_FB_SHMEM_SECTION_ALIGN(
    uint8_t s_OasisRgbFrameBuffer[OASIS_RGB_FRAME_HEIGHT * OASIS_RGB_FRAME_WIDTH * OASIS_RGB_FRAME_BYTE_PER_PIXEL], 64);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static oasis_status_t _oasis_start(const vision_algo_dev_t *receiver);
static void _oasis_stop();
static void _set_blocker_bit(oasis_blocking_event_id_t blockerId);
static void _clear_blocker_bit(oasis_blocking_event_id_t blockerId);
static uint8_t _check_blocker_bit(oasis_blocking_event_id_t blockerId);
static void _process_inference_result(oasis_param_t *pParam);
/*******************************************************************************
 * Code
 ******************************************************************************/
static int _oasis_log(const char *formatString)
{
    // todo
    // OASIS_LOGE("%s", formatString);
    return 0;
}

static int _oasis_dev_eventsHandler(uint32_t event_id, void *response, event_status_t status, unsigned char isFinished)
{
    if (response == NULL)
    {
        return -1;
    }

    switch (event_id)
    {
        case kEventFaceRecID_DelUserAll:
        case kEventFaceRecID_DelUser:
        {
            if (status == kEventStatus_Ok)
            {
                LOGD("[OASIS] Delete success");
            }
            else
            {
                LOGD("[OASIS] Delete with error:%d", status);
            }
        }
        break;
        case kEventFaceRecID_GetUserList:
        {
            user_info_event_t *usersInfo = (user_info_event_t *)response;
            for (int index = 0; index < usersInfo->count; index++)
            {
                char savedStatus[10];
                face_user_info_t *userInfo = &(usersInfo->userInfo[index]);
                elevator_attr_t attr       = {0};
                strcpy(savedStatus, userInfo->isSaved ? "Saved" : "Not saved");

                if (s_pElevatordbOps != NULL)
                {
                    s_pElevatordbOps->getWithId(userInfo->id, &attr);
                }

                LOGD("[OASIS] %-10s - ID:%3d Name:%s Floor:%d Language:%d", savedStatus, userInfo->id, userInfo->name,
                     attr.floor, attr.language);
            }
        }
        break;
        case kEventFaceRecID_GetUserCount:
        {
            uint32_t count = *(uint32_t *)response;
            LOGD("[OASIS] Registered user count %d", count);
        }
        break;

        default:
            break;
    }

    return 0;
}

static inline void _oasis_dev_response(event_base_t eventBase,
                                       void *response,
                                       event_status_t status,
                                       unsigned char isFinished)
{
    _oasis_dev_eventsHandler(eventBase.eventId, response, status, isFinished);
}

static void _oasis_dev_requestFrame(const vision_algo_dev_t *dev)
{
    /* Build Valgo event */
    valgo_event_t valgo_event = {
        .eventId = kVAlgoEvent_RequestFrame, .eventInfo = kEventInfo_Remote, .data = NULL, .size = 0, .copy = 0};

    if ((dev != NULL) && (dev->cap.callback != NULL))
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev->id, valgo_event, fromISR);
    }
}

static void _oasis_dev_notifyResult(const vision_algo_dev_t *dev, vision_algo_result_t *result)
{
    static vision_algo_result_t oldResults = {0};
    /* Build Valgo event */
    valgo_event_t valgo_event = {.eventId   = kVAlgoEvent_VisionResultUpdate,
                                 .eventInfo = kEventInfo_Remote,
                                 .data      = result,
                                 .size      = sizeof(vision_algo_result_t),
                                 .copy      = 1};

    if (dev != NULL && result != NULL && dev->cap.callback != NULL)
    {
        if (memcmp(result, &oldResults, sizeof(vision_algo_result_t)))
        {
            LOGD("[OASIS] Result rec:%d face_id:%d", result->oasisLite.face_recognized, result->oasisLite.face_id);
            uint8_t fromISR = __get_IPSR();
            dev->cap.callback(dev->id, valgo_event, fromISR);
            memcpy(&oldResults, result, sizeof(vision_algo_result_t));
        }
    }
}

static void _oasis_evtCb(ImageFrame_t *frames[], OASISLTEvt_t evt, OASISLTCbPara_t *para, void *userData)
{
    OASIS_LOGV("  OASIS_EVT:[%d]", evt);
    oasis_param_t *pOasisLite     = (oasis_param_t *)userData;
    oasis_lite_result_t *result   = &pOasisLite->result.oasisLite;
    oasis_lite_debug_t *debugInfo = &pOasisLite->result.oasisLite.debug_info;

    switch (evt)
    {
        case OASISLT_EVT_DET_START:
        {
            FWK_Profiler_StartEvent(OASISLT_EVT_DET_START);
            memset(debugInfo, 0, sizeof(oasis_lite_debug_t));

            result->face_id   = INVALID_FACE_ID;
            debugInfo->faceID = INVALID_FACE_ID;
        }
        break;

        case OASISLT_EVT_DET_COMPLETE:
        {
            FWK_Profiler_EndEvent(OASISLT_EVT_DET_START);

            if (para->faceBoxRGB == NULL)
            {
                OASIS_LOGI("[OASIS] DET:No face detected");
                result->face_count = 0;
            }
            else
            {
                if (s_debugOption)
                {
                    OASIS_LOGD("[OASIS] DET:[Left: %d, Top: %d, Right: %d, Bottom: %d]", para->faceBoxRGB->rect[0],
                               para->faceBoxRGB->rect[1], para->faceBoxRGB->rect[2], para->faceBoxRGB->rect[3]);
                }
                result->face_count = 1;
                result->face_box   = (*(para->faceBoxRGB));
            }
        }
        break;

        case OASISLT_EVT_QUALITY_CHK_START:
        {
            FWK_Profiler_StartEvent(OASISLT_EVT_QUALITY_CHK_START);
        }
        break;

        case OASISLT_EVT_QUALITY_CHK_COMPLETE:
        {
            FWK_Profiler_EndEvent(OASISLT_EVT_QUALITY_CHK_START);

            switch (para->qualityResult)
            {
                case OASIS_QUALITY_RESULT_FACE_OK:
                {
                    result->qualityCheck = kOasisLiteQualityCheck_Ok;
                    debugInfo->isOk      = 1;
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] Quality:ok");
                    }
                }
                break;

                case OASIS_QUALITY_RESULT_FACE_TOO_SMALL:
                {
                    result->qualityCheck   = kOasisLiteQualityCheck_SmallFace;
                    debugInfo->isSmallFace = 1;
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] Quality:Small Face");
                    }
                }
                break;

                case OASIS_QUALITY_RESULT_FACE_BLUR:
                {
                    result->qualityCheck = kOasisLiteQualityCheck_Blurry;
                    debugInfo->isBlurry  = 1;
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] Quality:Blurry Face");
                    }
                }
                break;

                case OASIS_QUALITY_RESULT_FACE_ORIENTATION_UNMATCH:
                {
                    result->qualityCheck  = kOasisLiteQualityCheck_SideFace;
                    debugInfo->isSideFace = 1;
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] Quality:Side face[%d]", para->reserved[2]);
                    }
                }
                break;

                case OASIS_QUALITY_RESULT_FAIL_BRIGHTNESS_DARK:
                case OASIS_QUALITY_RESULT_FAIL_BRIGHTNESS_OVEREXPOSURE:
                {
                    result->qualityCheck     = kOasisLiteQualityCheck_Brightness;
                    debugInfo->rgbBrightness = para->reserved[12];
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] Quality:Face Brightness unfit[%d]", para->reserved[12]);
                    }
                }
                break;

                case OASIS_QUALITY_RESULT_PARTIAL_BRIGHTNESS_FAIL:
                {
                    result->qualityCheck = kOasisLiteQualityCheck_Brightness;
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] Quality:Face Partial Brightness unfit lrsim[%d], uddiff[%d]",
                                   para->reserved[18], para->reserved[19]);
                    }
                }
                break;

                default:
                    break;
            }
        }
        break;

        case OASISLT_EVT_REC_START:
        {
            FWK_Profiler_StartEvent(OASISLT_EVT_REC_START);
        }
        break;

        case OASISLT_EVT_REC_COMPLETE:
        {
            FWK_Profiler_EndEvent(OASISLT_EVT_REC_START);
            // Recognition complete
            OASISLTRecognizeRes_t recResult = para->recResult;

            if (recResult == OASIS_REC_RESULT_KNOWN_FACE)
            {
                /* Recognized */
                result->rec_result      = kOASISLiteRecognitionResult_Success;
                result->face_recognized = 1;
                result->face_id         = para->faceID;
                debugInfo->sim          = para->reserved[0];
                debugInfo->faceID       = para->faceID;

                if (s_pFacedbOps != NULL)
                {
                    char *faceName = s_pFacedbOps->getNameWithId(para->faceID);
                    strcpy(result->name, faceName);
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] KNOWN_FACE:[%s][%d]", faceName, para->reserved[0]);
                    }
                }
                else
                {
                    sprintf(result->name, "user_%03d", para->faceID);
                    OASIS_LOGE("[OASIS] ERROR:failed to get face name %d", para->faceID);
                }
            }
            else if (recResult == OASIS_REC_RESULT_UNKNOWN_FACE)
            {
                result->rec_result      = kOASISLiteRecognitionResult_Unknown;
                result->face_recognized = 1;
                result->face_id         = INVALID_FACE_ID;
                debugInfo->sim          = para->reserved[0];
                debugInfo->faceID       = para->faceID;
                // unknown face
                if (s_debugOption)
                {
                    OASIS_LOGD("[OASIS] UNKNOWN_FACE:Sim:[%d.%d%%]:[%d]", (int)(para->reserved[0] / 100),
                               (int)(para->reserved[0] % 100), para->faceID);
                }
            }
            else
            {
                result->rec_result      = kOASISLiteRecognitionResult_Invalid;
                result->face_recognized = 0;
                result->face_id         = INVALID_FACE_ID;
                OASIS_LOGI("[OASIS] INVALID_FACE");
            }
        }
        break;

        case OASISLT_EVT_REG_START:
        {
        }
        break;

        case OASISLT_EVT_REG_IN_PROGRESS:
        {
        }
        break;

        case OASISLT_EVT_REG_COMPLETE:
        {
            uint16_t id              = para->faceID;
            OASISLTRegisterRes_t res = para->regResult;

            if (res == OASIS_REG_RESULT_OK || res == OASIS_REG_RESULT_DUP)
            {
                if (res == OASIS_REG_RESULT_OK)
                {
                    result->reg_result = kOASISLiteRegistrationResult_Success;
                    LOGI("[OASIS] kOASISLiteRegistrationResult_Success");
                    result->face_id = INVALID_FACE_ID;
                }
                else if (res == OASIS_REG_RESULT_DUP)
                {
                    result->reg_result = kOASISLiteRegistrationResult_Duplicated;
                    LOGI("[OASIS] kOASISLiteRegistrationResult_Duplicated");
                    result->face_id = id;
                    if (id != (uint16_t)-1)
                    {
                        if (s_pFacedbOps != NULL)
                        {
                            char *faceName = s_pFacedbOps->getNameWithId(id);
                            strcpy(result->name, faceName);
                        }

                        if (s_pElevatordbOps != NULL)
                        {
                            elevator_attr_t attr;
                            elevator_info_t *pElevatorInfo = (elevator_info_t *)result->userData;
                            s_pElevatordbOps->getWithId(id, &attr);
                            pElevatorInfo->floorNum = attr.floor;
                            pElevatorInfo->language = attr.language;
                            if (s_debugOption)
                            {
                                LOGD("[OASIS] Known user:[%d][%s] Floor:[%d] Language:[%d]", id, result->name,
                                     pElevatorInfo->floorNum, pElevatorInfo->language);
                            }
                        }

                        s_faceId = id;
                    }
                }
            }
            else
            {
                result->reg_result = kOASISLiteRegistrationResult_Invalid;
                result->face_id    = INVALID_FACE_ID;
            }
        }
        break;

        case OASISLT_EVT_DEREG_START:
        {
            result->face_id      = INVALID_FACE_ID;
            result->dereg_result = kOASISLiteDeregistrationResult_Invalid;
        }
        break;
        case OASISLT_EVT_DEREG_COMPLETE:
        {
            if (para->deregResult == OASIS_DEREG_RESULT_OK)
            {
                OASIS_LOGD("[OASIS] FACE_REMOVED:[%d]", para->faceID);
                result->dereg_result = kOASISLiteDeregistrationResult_Success;
            }
        }
        break;
        default:
        {
        }
        break;
    }
}

static int _oasis_getFace(uint16_t *faceIds, void **pFaces, uint32_t *faceNum, void *userData)
{
    int ret = 0;
    OASIS_LOGI("++_oasis_getFace");

    if (s_pFacedbOps != NULL)
    {
        int dbCount = s_pFacedbOps->getFaceCount();
        // query the face db count
        if (*faceNum == 0)
        {
            *faceNum = dbCount;
            return ret;
        }

        s_pFacedbOps->getIdsAndFaces(faceIds, pFaces);
    }

    OASIS_LOGI("--_oasis_getFace");
    return ret;
}

static int _oasis_addFace(
    uint16_t *faceId, void *faceData, SnapshotItem_t *snapshotData, int snapshotNum, void *userData)
{
    int ret = 0;
    OASIS_LOGI("++_oasis_addFace");

    if (s_pFacedbOps != NULL)
    {
        facedb_status_t status = s_pFacedbOps->genId(&s_faceId);
        if ((status == kFaceDBStatus_Success) && (s_faceId != INVALID_ID))
        {
            *faceId = s_faceId;
            if (s_pFaceFeature != NULL)
            {
                memcpy(s_pFaceFeature, faceData, OASISLT_getFaceItemSize());
            }
        }
        else if (status == kFaceDBStatus_Full)
        {
            OASIS_LOGE("OASIS: Database is full.");
        }
    }

    OASIS_LOGI("--_oasis_addFace");
    return ret;
}

static int _oasis_updFace(
    uint16_t faceId, void *faceData, SnapshotItem_t *snapshotData, int snapshotNum, void *userData)
{
    int ret = 0;
    OASIS_LOGI("++_oasis_updFace");

    if (s_pFacedbOps != NULL)
    {
        int faceItemSize = OASISLT_getFaceItemSize();
        ret = s_pFacedbOps->updFaceWithId(faceId, s_pFacedbOps->getNameWithId(faceId), faceData, faceItemSize);
    }

    OASIS_LOGI("--_oasis_updFace");
    return ret;
}

static int _oasis_delFace(uint16_t faceId, void *userData)
{
    int ret = 0;
    OASIS_LOGI("++_oasis_delFace");

    if (faceId != INVALID_ID)
    {
        if (s_pFacedbOps != NULL)
        {
            s_pFacedbOps->delFaceWithId(faceId);
        }

        if (s_pElevatordbOps != NULL)
        {
            s_pElevatordbOps->delWithId(faceId);
        }
    }

    OASIS_LOGI("--_oasis_delFace");
    return ret;
}

static void _set_blocker_bit(oasis_blocking_event_id_t blockerId)
{
    s_blockingList |= 1UL << blockerId;
}

static void _clear_blocker_bit(oasis_blocking_event_id_t blockerId)
{
    s_blockingList &= ~(1UL << blockerId);
}

static uint8_t _check_blocker_bit(oasis_blocking_event_id_t blockerId)
{
    return (s_blockingList >> blockerId) & 1U;
}

static void _oasis_start_recognition(oasis_param_t *pParam)
{
    if (pParam == NULL)
        return;

    if (pParam->currRunFlag == OASIS_RUN_FLAG_STOP)
    {
        LOGI("Skip the start of recognition as oasis is stopped.");
        return;
    }

    memset(&pParam->result, 0, sizeof(pParam->result));
    pParam->currRunFlag            = OASIS_DET_REC;
    pParam->result.id              = kVisionAlgoID_OasisLite;
    pParam->result.oasisLite.state = kOASISLiteState_Recognition;

    _process_inference_result(pParam);
}

static void _oasis_start_registration(oasis_param_t *pParam)
{
    if (pParam == NULL)
        return;

    if (pParam->currRunFlag == OASIS_RUN_FLAG_STOP)
    {
        LOGI("Skip the start of registration as oasis is stopped.");
        return;
    }

    memset(&pParam->result, 0, sizeof(pParam->result));
    pParam->currRunFlag              = OASIS_DET_REC_REG;
    pParam->result.id                = kVisionAlgoID_OasisLite;
    pParam->result.oasisLite.state   = kOASISLiteState_Registration;
    pParam->result.oasisLite.face_id = INVALID_FACE_ID;

    _process_inference_result(pParam);
}

static void _oasis_start_deregistration(oasis_param_t *pParam)
{
    if (pParam == NULL)
        return;

    if (pParam->currRunFlag == OASIS_RUN_FLAG_STOP)
    {
        LOGI("Skip the start of deregistration as oasis is stopped.");
        return;
    }

    memset(&pParam->result, 0, sizeof(pParam->result));
    pParam->currRunFlag            = OASIS_DET_REC_DEREG;
    pParam->result.id              = kVisionAlgoID_OasisLite;
    pParam->result.oasisLite.state = kOASISLiteState_DeRegistration;

    _process_inference_result(pParam);
}

static oasis_status_t _oasis_start(const vision_algo_dev_t *receiver)
{
    LOGD("[OASIS] Start %d", s_blockingList);
    if (s_blockingList == 0)
    {
        s_faceId = INVALID_FACE_ID;
        _oasis_dev_requestFrame(receiver);
        s_OasisElevator.currRunFlag = s_OasisElevator.prevRunFlag;
        s_OasisElevator.prevRunFlag = OASIS_RUN_FLAG_STOP;
        _oasis_start_registration(&s_OasisElevator);

        return kOasis_Success;
    }
    else
    {
        return kOasis_Failed;
    }
}

static void _oasis_stop()
{
    LOGD("[OASIS] Stop");

    s_OasisElevator.prevRunFlag = s_OasisElevator.currRunFlag;
    s_OasisElevator.currRunFlag = OASIS_RUN_FLAG_STOP;

    memset(&s_OasisElevator.result, 0, sizeof(s_OasisElevator.result));
    s_OasisElevator.result.id = kVisionAlgoID_OasisLite;
    //_oasis_dev_notifyResult(s_OasisElevator.dev, &(s_OasisElevator.result));
}

static void _oasis_dev_led_pwm_control(const vision_algo_dev_t *dev, event_common_t *event)
{
    /* Build Valgo event */
    valgo_event_t valgo_event = {.eventId   = kVAlgoEvent_VisionLedPwmControl,
                                 .eventInfo = kEventInfo_Remote,
                                 .data      = event,
                                 .size      = sizeof(event_common_t),
                                 .copy      = 1};

    if (dev != NULL && event != NULL && dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev->id, valgo_event, fromISR);
    }
}

static void _oasis_dev_camera_exposure_control(const vision_algo_dev_t *dev, event_common_t *event)
{
    /* Build Valgo event */
    valgo_event_t valgo_event = {.eventId   = kVAlgoEvent_VisionCamExpControl,
                                 .eventInfo = kEventInfo_Remote,
                                 .data      = event,
                                 .size      = sizeof(event_common_t),
                                 .copy      = 1};

    if (dev != NULL && event != NULL && dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev->id, valgo_event, fromISR);
    }
}

static void _oasis_resetBrightness(void)
{
#if 0
    event_common_t eventIRLed;
    eventIRLed.brightnessControl.enable = false;
    eventIRLed.eventBase.eventId        = kEventID_ControlIRLedBrightness;
    _oasis_dev_led_pwm_control(s_OasisElevator.dev, &eventIRLed);

    event_common_t eventWhiteLed;
    eventWhiteLed.brightnessControl.enable = false;
    eventWhiteLed.eventBase.eventId        = kEventID_ControlWhiteLedBrightness;
    _oasis_dev_led_pwm_control(s_OasisElevator.dev, &eventWhiteLed);

    event_common_t eventRGBCam;
    eventRGBCam.brightnessControl.enable = false;
    eventRGBCam.eventBase.eventId        = kEventID_ControlRGBCamExposure;
    _oasis_dev_camera_exposure_control(s_OasisElevator.dev, &eventRGBCam);

    LOGD("Set to default pwm and/or exposure when processing is finished or timeout.");
#endif
}

/* Used to dynamically adjust face brightness, user can adjust brightness by modifing LED's light intensity or camera
 * exposure level. frame_idx: which frame needs to be adjusted on, OASISLT_INT_FRAME_IDX_RGB or OASISLT_INT_FRAME_IDX_IR
 * ? direction: 1: up, need to increase brightness;  0: down, need to reduce brightness. For GC0308, we can combine
 * camera exposure and led pwm to adjust rgb face brightness. For MT9M114, only use pwm to adjust rgb face brightness.
 */
static void _oasis_adjustBrightness(uint8_t frameIdx, uint8_t direction, void *userData)
{
    OASIS_LOGV("++_oasis_adjustBrightness");

    event_common_t eventCamExposure;
    eventCamExposure.brightnessControl.enable    = true;
    eventCamExposure.brightnessControl.direction = direction;
    eventCamExposure.eventBase.eventId           = kEventID_ControlRGBCamExposure;
    _oasis_dev_camera_exposure_control(s_OasisElevator.dev, &eventCamExposure);

    OASIS_LOGV("--_oasis_adjustBrightness");
}

static void _process_inference_result(oasis_param_t *pParam)
{
    if (pParam == NULL)
        return;

    oasis_lite_result_t *pResult = &pParam->result.oasisLite;
    vision_algo_dev_t *dev       = pParam->dev;
    if ((pResult == NULL) || (dev == NULL))
        return;

    // notify the result
    if ((pResult->reg_result == kOASISLiteRegistrationResult_Success) ||
        (pResult->reg_result == kOASISLiteRegistrationResult_Duplicated))
    {
        _oasis_dev_notifyResult(dev, &(pParam->result));
        _oasis_stop();
    }
    else
    {
        if (s_debugOption)
        {
            _oasis_dev_notifyResult(dev, &(pParam->result));
        }
    }
}

static hal_valgo_status_t HAL_VisionAlgoDev_OasisElevator_Init(vision_algo_dev_t *dev,
                                                               valgo_dev_callback_t callback,
                                                               void *param)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    OASIS_LOGI("++HAL_VisionAlgoDev_OasisElevator_Init");
    OASISLTResult_t oasisRet = OASISLT_OK;

    s_OasisElevator.dev = dev;

    // init the device
    memset(&dev->cap, 0, sizeof(dev->cap));
    dev->cap.callback = callback;

    dev->data.autoStart                              = 0;
    dev->data.frames[kVAlgoFrameID_RGB].height       = OASIS_RGB_FRAME_HEIGHT;
    dev->data.frames[kVAlgoFrameID_RGB].width        = OASIS_RGB_FRAME_WIDTH;
    dev->data.frames[kVAlgoFrameID_RGB].pitch        = OASIS_RGB_FRAME_WIDTH * OASIS_RGB_FRAME_BYTE_PER_PIXEL;
    dev->data.frames[kVAlgoFrameID_RGB].is_supported = 1;
    dev->data.frames[kVAlgoFrameID_RGB].rotate       = kCWRotateDegree_0;
    dev->data.frames[kVAlgoFrameID_RGB].flip         = kFlipMode_None;
    dev->data.frames[kVAlgoFrameID_RGB].format       = kPixelFormat_BGR;
    dev->data.frames[kVAlgoFrameID_RGB].srcFormat    = OASIS_RGB_FRAME_SRC_FORMAT;
    dev->data.frames[kVAlgoFrameID_RGB].data         = s_OasisRgbFrameBuffer;

    // init the RGB frame
    s_OasisElevator.frames[OASISLT_INT_FRAME_IDX_RGB].height = OASIS_RGB_FRAME_HEIGHT;
    s_OasisElevator.frames[OASISLT_INT_FRAME_IDX_RGB].width  = OASIS_RGB_FRAME_WIDTH;
    s_OasisElevator.frames[OASISLT_INT_FRAME_IDX_RGB].fmt    = OASIS_IMG_FORMAT_BGR888;
    s_OasisElevator.frames[OASISLT_INT_FRAME_IDX_RGB].data   = dev->data.frames[kVAlgoFrameID_RGB].data;
    s_OasisElevator.pframes[OASISLT_INT_FRAME_IDX_RGB]       = &s_OasisElevator.frames[OASISLT_INT_FRAME_IDX_RGB];

    // init the oasis config
    s_OasisElevator.config.imgType              = OASIS_IMG_TYPE_RGB_SINGLE;
    s_OasisElevator.config.minFace              = OASIS_DETECT_MIN_FACE;
    s_OasisElevator.config.cbs.EvtCb            = _oasis_evtCb;
    s_OasisElevator.config.cbs.GetFaces         = _oasis_getFace;
    s_OasisElevator.config.cbs.AddFace          = _oasis_addFace;
    s_OasisElevator.config.cbs.UpdateFace       = _oasis_updFace;
    s_OasisElevator.config.cbs.DeleteFace       = _oasis_delFace;
    s_OasisElevator.config.cbs.AdjustBrightness = _oasis_adjustBrightness;
    s_OasisElevator.config.cbs.reserved         = _oasis_log;

    s_OasisElevator.config.enableFlags                 = OASIS_ENABLE_LIVENESS;
    s_OasisElevator.config.falseAcceptRate             = OASIS_FAR_1_1000000;
    s_OasisElevator.config.height                      = OASIS_RGB_FRAME_HEIGHT;
    s_OasisElevator.config.width                       = OASIS_RGB_FRAME_WIDTH;
    s_OasisElevator.config.size                        = 0;
    s_OasisElevator.config.memPool                     = NULL;
    s_OasisElevator.config.fastMemSize                 = DTC_OPTIMIZE_BUFFER_SIZE;
    s_OasisElevator.config.fastMemBuf                  = (char *)g_DTCOPBuf;
    s_OasisElevator.config.runtimePara.brightnessTH[0] = 50;
    s_OasisElevator.config.runtimePara.brightnessTH[1] = 180;
    s_OasisElevator.config.runtimePara.frontTH         = 0.5;

    oasisRet = OASISLT_init(&s_OasisElevator.config);
    if (oasisRet == OASIS_INIT_INVALID_MEMORYPOOL)
    {
#if OASIS_STATIC_MEM_BUFFER
        if (OASIS_STATIC_MEM_POOL >= s_OasisElevator.config.size)
        {
            s_OasisElevator.config.memPool = (char *)g_OasisMemPool;
        }
        else
        {
            s_OasisElevator.config.memPool = NULL;
        }
#else
        s_OasisElevator.config.memPool = (char *)pvPortMalloc(s_OasisElevator.config.size);
#endif
        OASIS_LOGD("[OASIS] OASIS LITE MEM POOL %d", s_OasisElevator.config.size);

        if (s_OasisElevator.config.memPool == NULL)
        {
            OASIS_LOGE("Unable to allocate memory for OASIS mem pool of size %d.", s_OasisElevator.config.size);
            while (1)
                ;
        }

        oasisRet = OASISLT_init(&s_OasisElevator.config);
    }

    if (oasisRet != OASISLT_OK)
    {
        OASIS_LOGE("[OASIS] OASISLT_init ret %d", ret);
        ret = kStatus_HAL_ValgoInitError;
        return ret;
    }

    /* Initial Oasis status */
    if (dev->data.autoStart)
    {
        s_OasisElevator.currRunFlag = OASIS_DET_REC_REG;
    }
    else
    {
        s_OasisElevator.currRunFlag = OASIS_RUN_FLAG_STOP;
    }
    s_OasisElevator.prevRunFlag = OASIS_DET_REC_REG;

#ifdef ENABLE_FACEDB
    facedb_status_t status_facedb         = kFaceDBStatus_Success;
    elevatordb_status_t status_elevatordb = kElevatorDBStatus_Success;

    /* Initial Face Database */
    if (s_pFacedbOps == NULL)
    {
        s_pFacedbOps  = &g_facedb_ops;
        status_facedb = s_pFacedbOps->init(OASISLT_getFaceItemSize());
        if ((kFaceDBStatus_Success != status_facedb) && (kFaceDBStatus_VersionMismatch != status_facedb))
        {
            OASIS_LOGE("[OASIS] FaceDb initial failed");
            ret = kStatus_HAL_ValgoInitError;
            return ret;
        }
    }

    if (s_pElevatordbOps == NULL)
    {
        s_pElevatordbOps  = &g_elevatordb_ops;
        status_elevatordb = s_pElevatordbOps->init();
        if ((kElevatorDBStatus_VersionMismatch != status_elevatordb) &&
            (kElevatorDBStatus_Success != status_elevatordb))
        {
            OASIS_LOGE("[OASIS] ElevatorDb initial failed");
            ret = kStatus_HAL_ValgoInitError;
            return ret;
        }
    }

    if (kElevatorDBStatus_VersionMismatch == status_elevatordb)
    {
        s_pFacedbOps->delFaceWithId(INVALID_FACE_ID);
        status_elevatordb = s_pElevatordbOps->init();
        if (kElevatorDBStatus_Success != status_elevatordb)
        {
            OASIS_LOGE("[OASIS] ElevatorDb initial failed");
            ret = kStatus_HAL_ValgoInitError;
            return ret;
        }
    }

    if (kFaceDBStatus_VersionMismatch == status_facedb)
    {
        s_pElevatordbOps->delWithId(INVALID_FACE_ID);
        status_facedb = s_pFacedbOps->init(OASISLT_getFaceItemSize());
        if (kFaceDBStatus_Success != status_facedb)
        {
            OASIS_LOGE("[OASIS] FaceDb initial failed");
            ret = kStatus_HAL_ValgoInitError;
            return ret;
        }
    }

    memset(&s_elevatorAttr, 0x00, sizeof(elevator_attr_t));
    s_elevatorAttr.id    = INVALID_ID;
    s_elevatorAttr.floor = INVALID_FLOOR;

#endif /* ENABLE_FACEDB */

    s_pFaceFeature = pvPortMalloc(OASISLT_getFaceItemSize());
    if (s_pFaceFeature == NULL)
    {
        OASIS_LOGE("[OASIS] Unable to allocate memory for face feature.");
    }

    _oasis_start_registration(&s_OasisElevator);

    OASIS_LOGD("[OASIS] Init ok");
    OASIS_LOGI("--HAL_VisionAlgoDev_OasisElevator_Init");
    return ret;
}

// deinitialize the dev
static hal_valgo_status_t HAL_VisionAlgoDev_OasisElevator_Deinit(vision_algo_dev_t *dev)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    LOGI("++HAL_VisionAlgoDev_OasisElevator_Deinit");

    LOGI("--HAL_VisionAlgoDev_OasisElevator_Deinit");
    return ret;
}

// start the dev
static hal_valgo_status_t HAL_VisionAlgoDev_OasisElevator_Run(const vision_algo_dev_t *dev, void *data)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    OASIS_LOGV("++HAL_VisionAlgoDev_OasisElevator_Run: %d", s_OasisElevator.currRunFlag);

    if (s_OasisElevator.currRunFlag != OASIS_RUN_FLAG_NUM && s_OasisElevator.currRunFlag != OASIS_RUN_FLAG_STOP)
    {
        // clear the result
        memset(&s_OasisElevator.result, 0, sizeof(s_OasisElevator.result));
        s_OasisElevator.result.id = kVisionAlgoID_OasisLite;

        FWK_Profiler_ClearEvents();

        int oasis_ret = OASISLT_run_extend(s_OasisElevator.pframes, s_OasisElevator.currRunFlag,
                                           s_OasisElevator.config.minFace, &s_OasisElevator);
        if (oasis_ret)
        {
            OASIS_LOGE("OASISLT_run_extend failed with error: %d", oasis_ret);
        }

        FWK_Profiler_Log();

        /* Take decision regarding the inference results */
        _process_inference_result(&s_OasisElevator);
    }

    if (s_OasisElevator.currRunFlag == OASIS_RUN_FLAG_STOP)
    {
        return kStatus_HAL_ValgoStop;
    }
    OASIS_LOGI("--HAL_VisionAlgoDev_OasisElevator_Run");
    return ret;
}

static hal_valgo_status_t HAL_VisionAlgoDev_OasisElevator_InputNotify(const vision_algo_dev_t *receiver, void *data)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    LOGI("++HAL_VisionAlgoDev_OasisElevator_InputNotify");

    event_base_t eventBase = *(event_base_t *)data;
    switch (eventBase.eventId)
    {
        case kEventElevatorId_RegisterFloor:
        {
            event_smart_tlhmi_t *pEvent = (event_smart_tlhmi_t *)data;

            if (s_faceId != (unsigned int)INVALID_FACE_ID)
            {
                if (s_pFacedbOps != NULL)
                {
                    facedb_status_t status =
                        s_pFacedbOps->addFace(s_faceId, NULL, s_pFaceFeature, OASISLT_getFaceItemSize());
                    if (status == kFaceDBStatus_Success)
                    {
                        memset(s_pFaceFeature, 0x0, OASISLT_getFaceItemSize());
                    }
                }

                if (s_pElevatordbOps != NULL)
                {
                    elevator_info_t elevatorInfo = pEvent->elevatorInfo;
                    elevator_attr_t attr         = {
                        .id = s_faceId, .floor = elevatorInfo.floorNum, .language = elevatorInfo.language};

                    if (elevatorInfo.id == (unsigned int)INVALID_FACE_ID)
                    {
                        /* Add new entry */
                        LOGD("[OASIS] Add user[%d]'s elevator:[%d, %d]", s_faceId, elevatorInfo.floorNum,
                             elevatorInfo.language);

                        elevatordb_status_t status = s_pElevatordbOps->addWithId(s_faceId, &attr);
                        if (kElevatorDBStatus_Success != status)
                        {
                            LOGD("[OASIS] add user[%d]'s floor failed", s_faceId);
                        }
                    }
                    else
                    {
                        /* Update existing entry */
                        LOGD("[OASIS] Update user[%d]'s elevator:[%d, %d]", elevatorInfo.id, elevatorInfo.floorNum,
                             elevatorInfo.language);

                        attr.id                    = elevatorInfo.id;
                        elevatordb_status_t status = s_pElevatordbOps->updWithId(elevatorInfo.id, &attr);
                        if (kElevatorDBStatus_Success != status)
                        {
                            LOGD("[OASIS] Update user[%d]'s failed", s_faceId);
                        }
                    }
                }
            }
        }
        break;

        case kEventFaceRecID_DelUserAll:
        {
            event_face_rec_t event              = *(event_face_rec_t *)data;
            event_status_t event_respond_status = kEventStatus_Ok;

            if (s_pFacedbOps != NULL)
            {
                if (s_pFacedbOps->delFaceWithId(INVALID_FACE_ID) != kFaceDBStatus_Success)
                {
                    event_respond_status = kEventStatus_Error;
                }
            }

            if (s_pElevatordbOps != NULL)
            {
                if (s_pElevatordbOps->delWithId(INVALID_FACE_ID) != kElevatorDBStatus_Success)
                {
                    event_respond_status = kEventStatus_Error;
                }
            }

            _oasis_dev_response(eventBase, &event.delFace, event_respond_status, true);
        }
        break;

        case kEventFaceRecID_DelUser:
        {
            event_face_rec_t event              = *(event_face_rec_t *)data;
            event_status_t event_respond_status = kEventStatus_Ok;
            if (event.delFace.hasID)
            {
                LOGD("[OASIS] Del user [%d]", event.delFace.id);
                if (s_pFacedbOps != NULL)
                {
                    if (s_pFacedbOps->delFaceWithId(event.delFace.id) != kFaceDBStatus_Success)
                    {
                        event_respond_status = kEventStatus_Error;
                    }
                }

                if (s_pElevatordbOps != NULL)
                {
                    if (s_pElevatordbOps->delWithId(event.delFace.id) != kElevatorDBStatus_Success)
                    {
                        event_respond_status = kEventStatus_Error;
                    }
                }

                _oasis_dev_response(eventBase, &event.delFace, event_respond_status, true);
            }
            else if (event.delFace.hasName)
            {
                LOGD("[OASIS] Del user [%s]", event.delFace.name);
                uint16_t id = INVALID_ID;
                if (s_pFacedbOps != NULL)
                {
                    if (s_pFacedbOps->getIdWithName(event.delFace.name, &id) == kFaceDBStatus_Success)
                    {
                        if (s_pFacedbOps->delFaceWithId(id) != kFaceDBStatus_Success)
                        {
                            event_respond_status = kEventStatus_Error;
                        }
                    }
                    else
                    {
                        event_respond_status = kEventStatus_Error;
                    }
                }

                if (s_pElevatordbOps != NULL && id != INVALID_ID)
                {
                    if (s_pElevatordbOps->delWithId(id) != kElevatorDBStatus_Success)
                    {
                        event_respond_status = kEventStatus_Error;
                    }
                }

                _oasis_dev_response(eventBase, &event.delFace, event_respond_status, true);
            }
            else
            {
                LOGD("[OASIS] Del user [%d]", s_faceId);
                /* No name and no id. Delete previously recognized face */
                if (s_faceId != -1)
                {
                    if (s_pFacedbOps != NULL)
                    {
                        facedb_status_t status = s_pFacedbOps->delFaceWithId(s_faceId);
                        if (status != kFaceDBStatus_Success)
                        {
                            LOGD("[OASIS] Del user %d failed", s_faceId);
                        }
                    }

                    if (s_pElevatordbOps != NULL)
                    {
                        elevatordb_status_t status = s_pElevatordbOps->delWithId(s_faceId);
                        if (kElevatorDBStatus_Success != status)
                        {
                            LOGD("[OASIS] Del user[%d]'s  floor failed", s_faceId);
                        }
                    }

                    s_faceId = -1;
                }
            }
        }
        break;

        case kEventFaceRecID_SaveUserList:
        {
            event_face_rec_t event = *(event_face_rec_t *)data;
            uint8_t isSaved        = s_pFacedbOps->getSaveStatus(INVALID_FACE_ID);

            if (isSaved == true)
            {
                _oasis_dev_response(eventBase, &event, kEventStatus_Ok, true);
                return ret;
            }

            facedb_status_t ret = s_pFacedbOps->saveFace();
            if (ret == kFaceDBStatus_Success)
            {
                _oasis_dev_response(eventBase, &event, kEventStatus_Ok, true);
            }
            else
            {
                _oasis_dev_response(eventBase, &event, kEventStatus_Error, true);
            }
        }
        break;

        case kEventFaceRecID_GetUserList:
        {
            LOGD("[OASIS] Get user list");
            user_info_event_t userEvent;
            int count = s_pFacedbOps->getFaceCount();
            if (count > 0)
            {
                uint16_t *faceIds = (uint16_t *)pvPortMalloc(count * sizeof(uint16_t));
                if (faceIds != NULL)
                {
                    face_user_info_t *userInfos = (face_user_info_t *)pvPortMalloc(count * sizeof(face_user_info_t));
                    if (userInfos != NULL)
                    {
                        userEvent.userInfo = userInfos;
                        userEvent.count    = count;

                        s_pFacedbOps->getIds(faceIds);

                        for (int i = 0; i < count; i++)
                        {
                            userInfos[i].id      = faceIds[i];
                            userInfos[i].isSaved = s_pFacedbOps->getSaveStatus(faceIds[i]);
                            memcpy(userInfos[i].name, s_pFacedbOps->getNameWithId(faceIds[i]),
                                   sizeof(userInfos[i].name));
                        }

                        _oasis_dev_response(eventBase, &userEvent, kEventStatus_Ok, true);
                        vPortFree(userInfos);
                    }
                    vPortFree(faceIds);
                }
                else
                {
                    userEvent.userInfo = NULL;
                    userEvent.count    = 0;
                    _oasis_dev_response(eventBase, &userEvent, kEventStatus_Ok, true);
                }
            }
        }
        break;

        case kEventFaceRecID_GetUserCount:
        {
            int count = s_pFacedbOps->getFaceCount();
            _oasis_dev_response(eventBase, &count, kEventStatus_Ok, true);
        }
        break;

        case kEventFaceRecID_UpdateUserInfo:
        {
            event_face_rec_t event            = *(event_face_rec_t *)data;
            update_user_event_t userInfo      = event.updateFace;
            event_status_t eventRespondStatus = kEventStatus_Ok;
            if (s_pFacedbOps->updNameWithId(userInfo.id, userInfo.name) != kFaceDBStatus_Success)
            {
                eventRespondStatus = kEventStatus_Error;
                LOGE("[OASIS] Update user info failed");
            }
            LOGI("[OASIS] Updating user info for id %d", userInfo.id);
            _oasis_dev_response(eventBase, &event.updateFace, eventRespondStatus, true);
        }
        break;

        case kEventFaceRecID_OasisGetState:
        {
            event_face_rec_t oasisEvent;
            if (s_OasisElevator.currRunFlag == OASIS_RUN_FLAG_STOP)
            {
                oasisEvent.oasisState.state = kOASISLiteState_Stopped;
            }
            else
            {
                oasisEvent.oasisState.state = kOASISLiteState_Running;
            }

            LOGI("[OASIS] get oasis state %d", oasisEvent.oasisState.state);
            _oasis_dev_response(eventBase, &oasisEvent, kEventStatus_Ok, true);
        }
        break;

        case kEventFaceRecID_OasisSetState:
        {
            event_face_rec_t eventOasis, oasisResponse;
            eventOasis = *(event_face_rec_t *)data;

            LOGD("[OASIS] Set state [%d]", eventOasis.oasisState.state);
            if (eventOasis.oasisState.state == kOASISLiteState_Stopped)
            {
                oasisResponse.oasisState.state = kOASISLiteState_Stopped;

                _set_blocker_bit(kOasisBlockingList_UserInput);

                /* ignore if already stopped */
                if (s_OasisElevator.currRunFlag != OASIS_RUN_FLAG_STOP)
                {
                    /* stop the oasis */
                    _oasis_stop();
                    _oasis_dev_response(eventBase, &oasisResponse, kEventStatus_Ok, true);
                }
                else
                {
                    /* send a message that it's already stopped */
                    _oasis_dev_response(eventBase, &oasisResponse, kEventStatus_Error, true);
                }
            }
            else if (eventOasis.oasisState.state == kOASISLiteState_Running)
            {
                oasisResponse.oasisState.state = kOASISLiteState_Running;

                /* ignore if already started */
                if (s_OasisElevator.currRunFlag == OASIS_RUN_FLAG_STOP)
                {
                    /* start the oasis */
                    _clear_blocker_bit(kOasisBlockingList_UserInput);
                    oasis_status_t status = _oasis_start(receiver);
                    if (status == kOasis_Failed)
                    {
                        oasisResponse.oasisState.state = kOASISLiteState_Stopped;
                        _oasis_dev_response(eventBase, &oasisResponse, kEventStatus_Error, true);
                    }
                    else
                    {
                        /* Toggle bit only on success */
                        _oasis_dev_response(eventBase, &oasisResponse, kEventStatus_Ok, true);
                    }
                }
                else
                {
                    /* send a message that it's already running */
                    _oasis_dev_response(eventBase, &oasisResponse, kEventStatus_Error, true);
                }
            }
        }
        break;

        case kEventFaceRecID_OasisDebugOption:
        {
            event_face_rec_t eventOasis;
            eventOasis               = *(event_face_rec_t *)data;
            unsigned int optionValue = (unsigned int)eventOasis.data;
            LOGD("[OASIS] Debug option [%d]:[%d]", s_debugOption, optionValue);
            s_debugOption = optionValue;
        }
        break;

        default:
            break;
    }

    OASIS_LOGI("--HAL_VisionAlgoDev_OasisElevator_InputNotify");
    return ret;
}

const static vision_algo_dev_operator_t s_VisionAlgoDev_OasisElevatorOps = {
    .init        = HAL_VisionAlgoDev_OasisElevator_Init,
    .deinit      = HAL_VisionAlgoDev_OasisElevator_Deinit,
    .run         = HAL_VisionAlgoDev_OasisElevator_Run,
    .inputNotify = HAL_VisionAlgoDev_OasisElevator_InputNotify,
};

static vision_algo_dev_t s_VisionAlgoDev_OasisElevator = {
    .id   = 0,
    .name = "Elevator",
    .ops  = (vision_algo_dev_operator_t *)&s_VisionAlgoDev_OasisElevatorOps,
    .cap  = {.param = NULL},
};

// for vision_algo_oasis_elevator device, please link oasis/liboasis_lite2D_DEFAULT_117f_ae.a
int HAL_VisionAlgoDev_OasisElevator_Register(int mode)
{
    int error = 0;
    LOGD("HAL_VisionAlgoDev_OasisElevator_Register");
    error = FWK_VisionAlgoManager_DeviceRegister(&s_VisionAlgoDev_OasisElevator);
    memset(&s_OasisElevator, 0, sizeof(s_OasisElevator));
    s_OasisElevator.mode = mode;
    return error;
}
#endif /* ENABLE_VISIONALGO_DEV_OasisElevator */
