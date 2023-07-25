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
 * @brief SLN Oasis-Lite Vision Algorithm HAL driver implementation for the smart lock application.
 */

#include "board_define.h"
#ifdef ENABLE_VISIONALGO_DEV_Oasis_HomePanel
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
#include "hal_sln_coffeedb.h"
#include "smart_tlhmi_event_descriptor.h"

// TODO: temporary fake the IR frame as the real ir camera is not up
#include "hal_vision_algo_ir_480_640_frame.h"

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
    oasis_lite_mode_t mode; // 0:smart-lock   1:ffi
} oasis_param_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
static oasis_param_t s_OasisHomePanel;
static const facedb_ops_t *s_pFacedbOps     = NULL;
static uint8_t *s_pFaceFeature              = NULL;
static uint16_t s_faceId                    = -1;
static uint16_t s_blockingList              = 0;

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
/*******************************************************************************
 * Code
 ******************************************************************************/
static int _oasis_log(const char *formatString)
{
    // OASIS_LOGE("%s", formatString);
    return 0;
}

static void _oasis_dev_requestFrame(const vision_algo_dev_t *dev)
{
    /* Build Valgo event */
    valgo_event_t valgo_event = {.eventId = kVAlgoEvent_RequestFrame, .data = NULL, .size = 0, .copy = 0};

    if ((dev != NULL) && (dev->cap.callback != NULL))
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev->id, valgo_event, fromISR);
    }
}

static void _oasis_dev_notifyResult(const vision_algo_dev_t *dev, vision_algo_result_t *result)
{
    /* Build Valgo event */
    valgo_event_t valgo_event = {
        .eventId = kVAlgoEvent_VisionResultUpdate, .data = result, .size = sizeof(vision_algo_result_t), .copy = 1};

    if (dev != NULL && result != NULL && dev->cap.callback != NULL)
    {
        LOGD("[OASIS] Result rec:%d face_id:%d\r\n", result->oasisLite.face_recognized, result->oasisLite.face_id);
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev->id, valgo_event, fromISR);
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
            debugInfo->faceID = INVALID_FACE_ID;
        }
        break;

        case OASISLT_EVT_DET_COMPLETE:
        {
            FWK_Profiler_EndEvent(OASISLT_EVT_DET_START);
            result->face_id = -1;
            if (para->faceBoxRGB == NULL)
            {
                OASIS_LOGI("[OASIS] DET:No face detected");
                result->face_count = 0;
            }
            else
            {
                OASIS_LOGD("[OASIS] DET:[Left: %d, Top: %d, Right: %d, Bottom: %d]", para->faceBoxRGB->rect[0],
                           para->faceBoxRGB->rect[1], para->faceBoxRGB->rect[2], para->faceBoxRGB->rect[3]);
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
            debugInfo->irBrightness  = para->reserved[10];
            debugInfo->rgbBrightness = para->reserved[12];
            OASIS_LOGI("[OASIS] irBrightness:%d", debugInfo->irBrightness);
            OASIS_LOGI("[OASIS] rgbBrightness:%d", debugInfo->rgbBrightness);

            switch (para->qualityResult)
            {
                case OASIS_QUALITY_RESULT_FACE_OK:
                {
                    result->qualityCheck = kOasisLiteQualityCheck_Ok;
                    debugInfo->isOk      = 1;
                    OASIS_LOGD("[OASIS] Quality:ok");
                }
                break;

                case OASIS_QUALITY_RESULT_FACE_TOO_SMALL:
                {
                    OASIS_LOGD("[OASIS] Quality:Small Face");
                }
                break;

                case OASIS_QUALITY_RESULT_FACE_ORIENTATION_UNMATCH:
                {
                    result->qualityCheck  = kOasisLiteQualityCheck_SideFace;
                    debugInfo->isSideFace = 1;
                    OASIS_LOGD("[OASIS] Quality:Side face[%d]", para->reserved[2]);
                }
                break;

                case OASIS_QUALITY_RESULT_FACE_BLUR:
                {
                    result->qualityCheck = kOasisLiteQualityCheck_Blurry;
                    debugInfo->isBlurry  = 1;
                    OASIS_LOGD("[OASIS] Quality:Blurry Face");
                }
                break;

                case OASIS_QUALITY_RESULT_FAIL_BRIGHTNESS_DARK:
                case OASIS_QUALITY_RESULT_FAIL_BRIGHTNESS_OVEREXPOSURE:
                {
                    OASIS_LOGD("[OASIS] Quality:Face Brightness unfit[%d]", para->reserved[12]);
                }
                break;
				
				case OASIS_QUALITY_RESULT_PARTIAL_BRIGHTNESS_FAIL:
                {
                    result->qualityCheck     = kOasisLiteQualityCheck_Brightness;
                    OASIS_LOGD("[OASIS] Quality:Face Partial Brightness unfit lrsim[%d], uddiff[%d]", para->reserved[18], para->reserved[19]);
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
                    OASIS_LOGD("[OASIS] KNOWN_FACE:[%s][%d]", faceName, para->reserved[0]);
                    strcpy(result->name, faceName);
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
                result->face_id         = -1;
                debugInfo->sim          = para->reserved[0];
                debugInfo->faceID       = para->faceID;
                // unknown face
                OASIS_LOGD("[OASIS] UNKNOWN_FACE:Sim:[%d.%d%]:[%d]", (int)(para->reserved[0] / 100),
                           (int)(para->reserved[0] % 100), para->faceID);
            }
            else
            {
                result->rec_result      = kOASISLiteRecognitionResult_Invalid;
                result->face_recognized = 0;
                result->face_id         = -1;
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
                    result->face_id = -1;
                }
                else if (res == OASIS_REG_RESULT_DUP)
                {
                    result->reg_result = kOASISLiteRegistrationResult_Duplicated;
                    LOGI("[OASIS] kOASISLiteRegistrationResult_Duplicated");
                    result->face_id = id;
                    if (id != -1)
                    {
                        if (s_pFacedbOps != NULL)
                        {
                            char *faceName = s_pFacedbOps->getNameWithId(id);
                            strcpy(result->name, faceName);
                        }

                        s_faceId = id;
                    }
                }
            }
            else
            {
                result->reg_result = kOASISLiteRegistrationResult_Invalid;
                result->face_id    = -1;
            }
        }
        break;

        case OASISLT_EVT_DEREG_START:
        {
            result->face_id      = -1;
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
            // TODO: Temporary workaround. Remove this once name is returned in oasisLite library as part of dereg event
            if (s_OasisHomePanel.result.oasisLite.state == kOASISLiteState_DeRegistration)
            {
                strcpy(s_OasisHomePanel.result.oasisLite.name, s_pFacedbOps->getNameWithId(faceId));
            }

            facedb_status_t status = s_pFacedbOps->delFaceWithId(faceId);
            if (status == kFaceDBStatus_Success)
            {
                ret = 0;
            }
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

    _oasis_dev_notifyResult(pParam->dev, &(pParam->result));
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
    pParam->result.oasisLite.face_id = -1;

    _oasis_dev_notifyResult(s_OasisHomePanel.dev, &(pParam->result));
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

    _oasis_dev_notifyResult(pParam->dev, &(pParam->result));
}

static oasis_status_t _oasis_start(const vision_algo_dev_t *receiver)
{
    LOGD("[OASIS] Start %d", s_blockingList);
    if (s_blockingList == 0)
    {
        _oasis_dev_requestFrame(receiver);
        s_OasisHomePanel.currRunFlag = s_OasisHomePanel.prevRunFlag;
        s_OasisHomePanel.prevRunFlag = OASIS_RUN_FLAG_STOP;
        _oasis_start_registration(&s_OasisHomePanel);

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

    s_OasisHomePanel.prevRunFlag = s_OasisHomePanel.currRunFlag;
    s_OasisHomePanel.currRunFlag = OASIS_RUN_FLAG_STOP;

    memset(&s_OasisHomePanel.result, 0, sizeof(s_OasisHomePanel.result));
    s_OasisHomePanel.result.id = kVisionAlgoID_OasisLite;
    //_oasis_dev_notifyResult(s_OasisHomePanel.dev, &(s_OasisHomePanel.result));
}

static void _oasis_dev_led_pwm_control(const vision_algo_dev_t *dev, event_common_t *event)
{
    /* Build Valgo event */
    valgo_event_t valgo_event = {
        .eventId = kVAlgoEvent_VisionLedPwmControl, .data = event, .size = sizeof(event_common_t), .copy = 1};

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
    _oasis_dev_led_pwm_control(s_OasisHomePanel.dev, &eventIRLed);

    event_common_t eventWhiteLed;
    eventWhiteLed.brightnessControl.enable = false;
    eventWhiteLed.eventBase.eventId        = kEventID_ControlWhiteLedBrightness;
    _oasis_dev_led_pwm_control(s_OasisHomePanel.dev, &eventWhiteLed);

    event_common_t eventRGBCam;
    eventRGBCam.brightnessControl.enable = false;
    eventRGBCam.eventBase.eventId        = kEventID_ControlRGBCamExposure;
    _oasis_dev_camera_exposure_control(s_OasisHomePanel.dev, &eventRGBCam);

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
    _oasis_dev_camera_exposure_control(s_OasisHomePanel.dev, &eventCamExposure);

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
}

static hal_valgo_status_t HAL_VisionAlgoDev_OasisHomePanel_Init(vision_algo_dev_t *dev,
                                                                    valgo_dev_callback_t callback,
                                                                    void *param)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    OASIS_LOGI("++HAL_VisionAlgoDev_OasisHomePanel_Init");
    OASISLTResult_t oasisRet = OASISLT_OK;

    s_OasisHomePanel.dev = dev;

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
    s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_RGB].height = OASIS_RGB_FRAME_HEIGHT;
    s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_RGB].width  = OASIS_RGB_FRAME_WIDTH;
    s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_RGB].fmt    = OASIS_IMG_FORMAT_BGR888;
    s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_RGB].data   = dev->data.frames[kVAlgoFrameID_RGB].data;
    s_OasisHomePanel.pframes[OASISLT_INT_FRAME_IDX_RGB] = &s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_RGB];

    // init the oasis config
    s_OasisHomePanel.config.imgType              = OASIS_IMG_TYPE_RGB_SINGLE;
    s_OasisHomePanel.config.minFace              = OASIS_DETECT_MIN_FACE;
    s_OasisHomePanel.config.cbs.EvtCb            = _oasis_evtCb;
    s_OasisHomePanel.config.cbs.GetFaces         = _oasis_getFace;
    s_OasisHomePanel.config.cbs.AddFace          = _oasis_addFace;
    s_OasisHomePanel.config.cbs.UpdateFace       = _oasis_updFace;
    s_OasisHomePanel.config.cbs.DeleteFace       = _oasis_delFace;
    s_OasisHomePanel.config.cbs.AdjustBrightness = _oasis_adjustBrightness;
    s_OasisHomePanel.config.cbs.reserved         = _oasis_log;

    s_OasisHomePanel.config.enableFlags                 = OASIS_ENABLE_LIVENESS;
    s_OasisHomePanel.config.falseAcceptRate             = OASIS_FAR_1_1000000;
    s_OasisHomePanel.config.height                      = OASIS_RGB_FRAME_HEIGHT;
    s_OasisHomePanel.config.width                       = OASIS_RGB_FRAME_WIDTH;
    s_OasisHomePanel.config.size                        = 0;
    s_OasisHomePanel.config.memPool                     = NULL;
    s_OasisHomePanel.config.fastMemSize                 = DTC_OPTIMIZE_BUFFER_SIZE;
    s_OasisHomePanel.config.fastMemBuf                  = (char *)g_DTCOPBuf;
    s_OasisHomePanel.config.runtimePara.brightnessTH[0] = 50;
    s_OasisHomePanel.config.runtimePara.brightnessTH[1] = 180;
    s_OasisHomePanel.config.runtimePara.frontTH         = 0.5;

    oasisRet = OASISLT_init(&s_OasisHomePanel.config);
    if (oasisRet == OASIS_INIT_INVALID_MEMORYPOOL)
    {
#if OASIS_STATIC_MEM_BUFFER
        if (OASIS_STATIC_MEM_POOL >= s_OasisHomePanel.config.size)
        {
            s_OasisHomePanel.config.memPool = (char *)g_OasisMemPool;
        }
        else
        {
            s_OasisHomePanel.config.memPool = NULL;
        }
#else
        s_OasisHomePanel.config.memPool = (char *)pvPortMalloc(s_OasisHomePanel.config.size);
#endif
        OASIS_LOGD("[OASIS] OASIS LITE MEM POOL %d", s_OasisHomePanel.config.size);
        oasisRet = OASISLT_init(&s_OasisHomePanel.config);
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
        s_OasisHomePanel.currRunFlag = OASIS_DET_REC_REG;
    }
    else
    {
        s_OasisHomePanel.currRunFlag = OASIS_RUN_FLAG_STOP;
    }
    s_OasisHomePanel.prevRunFlag = OASIS_DET_REC_REG;

#ifdef ENABLE_FACEDB
    /* Initial Face Database */
    if (s_pFacedbOps == NULL)
    {
        s_pFacedbOps           = &g_facedb_ops;
        facedb_status_t status = s_pFacedbOps->init(OASISLT_getFaceItemSize());
        if (kFaceDBStatus_Success != status)
        {
            OASIS_LOGE("[OASIS] FaceDb initial failed");
            ret = kStatus_HAL_ValgoInitError;
            return ret;
        }
    }
#endif

    s_pFaceFeature = pvPortMalloc(OASISLT_getFaceItemSize());
    if (s_pFaceFeature == NULL)
    {
        OASIS_LOGE("[OASIS] Unable to allocate memory for face feature.");
    }

    _oasis_start_registration(&s_OasisHomePanel);

    OASIS_LOGD("[OASIS]:Init ok");
    OASIS_LOGI("--HAL_VisionAlgoDev_OasisHomePanel_Init");
    return ret;
}

// deinitialize the dev
static hal_valgo_status_t HAL_VisionAlgoDev_OasisHomePanel_Deinit(vision_algo_dev_t *dev)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    LOGI("++HAL_VisionAlgoDev_OasisHomePanel_Deinit");

    LOGI("--HAL_VisionAlgoDev_OasisHomePanel_Deinit");
    return ret;
}

// start the dev
static hal_valgo_status_t HAL_VisionAlgoDev_OasisHomePanel_Run(const vision_algo_dev_t *dev, void *data)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    OASIS_LOGV("++HAL_VisionAlgoDev_OasisHomePanel_Run: %d", s_OasisHomePanel.currRunFlag);

    if (s_OasisHomePanel.currRunFlag != OASIS_RUN_FLAG_NUM &&
        s_OasisHomePanel.currRunFlag != OASIS_RUN_FLAG_STOP)
    {
        // clear the result
        memset(&s_OasisHomePanel.result, 0, sizeof(s_OasisHomePanel.result));
        s_OasisHomePanel.result.id = kVisionAlgoID_OasisLite;

        FWK_Profiler_ClearEvents();

        int oasis_ret = OASISLT_run_extend(s_OasisHomePanel.pframes, s_OasisHomePanel.currRunFlag,
                                           s_OasisHomePanel.config.minFace, &s_OasisHomePanel);
        if (oasis_ret)
        {
            OASIS_LOGE("OASISLT_run_extend failed with error: %d", oasis_ret);
        }

        FWK_Profiler_Log();

        /* Take decision regarding the inference results */
        _process_inference_result(&s_OasisHomePanel);
    }

    if (s_OasisHomePanel.currRunFlag == OASIS_RUN_FLAG_STOP)
    {
        return kStatus_HAL_ValgoStop;
    }
    OASIS_LOGI("--HAL_VisionAlgoDev_OasisHomePanel_Run");
    return ret;
}

static hal_valgo_status_t HAL_VisionAlgoDev_OasisHomePanel_InputNotify(const vision_algo_dev_t *receiver,
                                                                           void *data)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    OASIS_LOGI("++HAL_VisionAlgoDev_OasisHomePanel_InputNotify");


    if (s_pFacedbOps == NULL)
    {
        OASIS_LOGE("[OASIS] Face Database is null");
        return kStatus_HAL_ValgoError;
    }

    event_base_t eventBase = *(event_base_t *)data;
    switch (eventBase.eventId)
    {
        case kEventFaceRecID_OasisGetState:
        {
            if (s_OasisHomePanel.currRunFlag == OASIS_RUN_FLAG_STOP)
            {
                LOGD("[OASIS] stop.");
            }
            else
            {
                LOGD("[OASIS] running.");
            }
        }
        break;

        case kEventFaceRecID_OasisSetState:
        {
            event_face_rec_t eventOasis = *(event_face_rec_t *)data;

            LOGD("[OASIS] set state [%d]", eventOasis.oasisState.state);
            if (eventOasis.oasisState.state == kOasisState_Stopped)
            {
                /* ignore if already stopped */
                if (s_OasisHomePanel.currRunFlag != OASIS_RUN_FLAG_STOP)
                {
                    /* stop the oasis */
                    _oasis_stop();
                }
            }
            else if (eventOasis.oasisState.state == kOasisState_Running)
            {
                /* ignore if already started */
                if (s_OasisHomePanel.currRunFlag == OASIS_RUN_FLAG_STOP)
                {
                    _oasis_start(receiver);
                }
            }
        }
        break;
#if 0
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

                    s_faceId = -1;
                }
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
#endif

        default:
            break;
    }

    OASIS_LOGI("--HAL_VisionAlgoDev_OasisHomePanel_InputNotify");
    return ret;
}

const static vision_algo_dev_operator_t s_VisionAlgoDev_OasisHomePanelOps = {
    .init        = HAL_VisionAlgoDev_OasisHomePanel_Init,
    .deinit      = HAL_VisionAlgoDev_OasisHomePanel_Deinit,
    .run         = HAL_VisionAlgoDev_OasisHomePanel_Run,
    .inputNotify = HAL_VisionAlgoDev_OasisHomePanel_InputNotify,
};

static vision_algo_dev_t s_VisionAlgoDev_OasisHomePanel = {
    .id   = 0,
    .name = "CoffeeMachine",
    .ops  = (vision_algo_dev_operator_t *)&s_VisionAlgoDev_OasisHomePanelOps,
    .cap  = {.param = NULL},
};

int HAL_VisionAlgoDev_OasisHomePanel_Register()
{
    int error = 0;
    LOGD("HAL_VisionAlgoDev_OasisHomePanel_Register");
    error = FWK_VisionAlgoManager_DeviceRegister(&s_VisionAlgoDev_OasisHomePanel);
    return error;
}
#endif /* ENABLE_VISIONALGO_DEV_OasisHomePanel */
