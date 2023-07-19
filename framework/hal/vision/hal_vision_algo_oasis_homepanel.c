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
#include "hal_sln_thermostatdb.h"
#include "hal_sln_security_facedb.h"
#include "smart_tlhmi_event_descriptor.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

#if VISION_SHARED_FRAME_BUFFER
#if (((OASIS_RGB_FRAME_WIDTH * OASIS_RGB_FRAME_HEIGHT * OASIS_RGB_FRAME_BYTE_PER_PIXEL) + \
      (OASIS_IR_FRAME_WIDTH * OASIS_IR_FRAME_HEIGHT * OASIS_IR_FRAME_BYTE_PER_PIXEL)) > VISION_ALGO_BUFFER_SIZE)
#error "Not enough memory for oasis home panel"
#endif
#endif /* VISION_SHARED_FRAME_BUFFER */
/* configurable parameters */
#ifndef OASIS_HOMEPANEL_AUTO_START
#define OASIS_HOMEPANEL_AUTO_START 0
#endif

#ifndef OASIS_HOMEPANEL_DEBUG_OPTION
#define OASIS_HOMEPANEL_DEBUG_OPTION false
#endif

#ifndef OASIS_HOMEPANEL_RESULT_EVENT_INFO
#define OASIS_HOMEPANEL_RESULT_EVENT_INFO kEventInfo_Remote
#endif

#ifndef OASIS_HOMEPANEL_REQUESTFRAME_EVENT_INFO
#define OASIS_HOMEPANEL_REQUESTFRAME_EVENT_INFO kEventInfo_Remote
#endif

#define OASIS_HOMEPANEL_IR_PWM_VAL 75

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
} oasis_param_t;

typedef enum
{
    kOasisScenario_Thermostat,
    kOasisScenario_Security,
} oasis_scenario_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
static oasis_param_t s_OasisHomePanel;
static oasis_scenario_t s_OasisScenario             = kOasisScenario_Thermostat;
static const facedb_ops_t *s_pFacedbOps             = NULL;
static const thermostatdb_ops_t *s_pThermostatdbOps = NULL;

static uint8_t *s_pFaceFeature    = NULL;
static uint16_t s_faceId          = INVALID_FACE_ID;
static unsigned int s_debugOption = OASIS_HOMEPANEL_DEBUG_OPTION;

#if OASIS_STATIC_MEM_BUFFER
__attribute__((section(".bss.$SRAM_OCRAM_CACHED"), aligned(64))) uint8_t g_OasisMemPool[OASIS_STATIC_MEM_POOL];
#endif

#if VISION_SHARED_FRAME_BUFFER
static uint8_t *s_OasisRgbFrameBuffer = g_VisionAlgoBuffer;
static uint8_t *s_OasisIrFrameBuffer =
    g_VisionAlgoBuffer + (OASIS_RGB_FRAME_WIDTH * OASIS_RGB_FRAME_HEIGHT * OASIS_RGB_FRAME_BYTE_PER_PIXEL);
#else
SDK_ALIGN(
    uint8_t s_OasisRgbFrameBuffer[OASIS_RGB_FRAME_HEIGHT * OASIS_RGB_FRAME_WIDTH * OASIS_RGB_FRAME_BYTE_PER_PIXEL], 64);
SDK_ALIGN(uint8_t s_OasisIrFrameBuffer[OASIS_IR_FRAME_HEIGHT * OASIS_IR_FRAME_WIDTH * OASIS_IR_FRAME_BYTE_PER_PIXEL],
          64);

#endif /* VISION_SHARED_FRAME_BUFFER */

AT_USR_SHMEM_SECTION_ALIGN(users_share_db_t g_UserList, 64);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void _process_inference_result(oasis_param_t *pParam);
static void _oasis_set_ir_led_brightness(const vision_algo_dev_t *dev, int brightness);
static void _oasis_adjustBrightness(uint8_t frameIdx, uint8_t direction, void *userData);
/*******************************************************************************
 * Code
 ******************************************************************************/
static int _oasis_log(const char *formatString)
{
    // OASIS_LOGD("%s", formatString);
    return 0;
}

static void _oasis_dev_requestFrame(const vision_algo_dev_t *dev)
{
    /* Build Valgo event */
    valgo_event_t valgoEvent = {.eventId   = kVAlgoEvent_RequestFrame,
                                .eventInfo = OASIS_HOMEPANEL_REQUESTFRAME_EVENT_INFO,
                                .data      = NULL,
                                .size      = 0,
                                .copy      = 0};

    if ((dev != NULL) && (dev->cap.callback != NULL))
    {
        dev->cap.callback(dev->id, valgoEvent, 0);
    }
}

static void _oasis_dev_notifyResult(const vision_algo_dev_t *dev, vision_algo_result_t *result)
{
    /* Build Valgo event */
    valgo_event_t valgoEvent = {.eventId   = kVAlgoEvent_VisionResultUpdate,
                                .eventInfo = OASIS_HOMEPANEL_RESULT_EVENT_INFO,
                                .data      = result,
                                .size      = sizeof(vision_algo_result_t),
                                .copy      = 1};

    if (dev != NULL && result != NULL && dev->cap.callback != NULL)
    {
        dev->cap.callback(dev->id, valgoEvent, 0);

        switch (result->oasisLite.state)
        {
            case kOASISLiteState_Recognition:
            {
                if (result->oasisLite.face_recognized)
                {
                    LOGD("FaceRec Known user: id[%d], name[%s]", result->oasisLite.face_id, result->oasisLite.name);
                }
            }
            break;

            case kOASISLiteState_Registration:
            {
                if (result->oasisLite.reg_result == kOASISLiteRegistrationResult_Duplicated)
                {
                    LOGD("FaceReg Duplicated: id[%d], name[%s]", result->oasisLite.face_id, result->oasisLite.name);
                }
                else if (result->oasisLite.reg_result == kOASISLiteRegistrationResult_Success)
                {
                    LOGD("FaceReg Success: id[%d]", result->oasisLite.face_id);
                }
            }
            break;

            default:
                break;
        }
    }
}

void _loadUserListIntoSharedMemory(void)
{
    BOARD_ShareDb_Lock();

    s_pFacedbOps                     = &g_facedb_ops;
    int count                        = s_pFacedbOps->getFaceCount();
    g_UserList.thermostat_list.count = count;
    if (count > 0)
    {
        uint16_t *faceIds = (uint16_t *)pvPortMalloc(count * sizeof(uint16_t));
        s_pFacedbOps->getIds(faceIds);
        for (int i = 0; i < count; i++)
        {
            thermostat_attribute_t attr;
            memset(&attr, 0x0, sizeof(attr));

            g_UserList.thermostat_list.objs[i].face_id = faceIds[i];
            strcpy((char *)g_UserList.thermostat_list.objs[i].user_name, s_pFacedbOps->getNameWithId(faceIds[i]));

            s_pThermostatdbOps->getWithId(faceIds[i], &attr);
            g_UserList.thermostat_list.objs[i].mode      = attr.mode;
            g_UserList.thermostat_list.objs[i].fan_speed = attr.fan_speed;

            memcpy(g_UserList.thermostat_list.objs[i].temperature, attr.temperature, sizeof(attr.temperature));
        }
        vPortFree(faceIds);
    }

    s_pFacedbOps                   = &g_security_facedb_ops;
    count                          = s_pFacedbOps->getFaceCount();
    g_UserList.security_list.count = count;
    if (count > 0)
    {
        uint16_t *faceIds = (uint16_t *)pvPortMalloc(count * sizeof(uint16_t));
        s_pFacedbOps->getIds(faceIds);
        for (int i = 0; i < count; i++)
        {
            g_UserList.security_list.objs[i].face_id = faceIds[i];
            strcpy((char *)g_UserList.security_list.objs[i].user_name, s_pFacedbOps->getNameWithId(faceIds[i]));
        }
        vPortFree(faceIds);
    }

    BOARD_ShareDb_Unlock();
}

static void _oasis_evtCb(ImageFrame_t *frames[], OASISLTEvt_t evt, OASISLTCbPara_t *para, void *userData)
{
    OASIS_LOGV("  OASIS_EVT:[%d]", evt);
    oasis_param_t *pOasisLite      = (oasis_param_t *)userData;
    oasis_lite_result_t *pResult   = &pOasisLite->result.oasisLite;
    oasis_lite_debug_t *pDebugInfo = &pOasisLite->result.oasisLite.debug_info;

    switch (evt)
    {
        case OASISLT_EVT_DET_START:
        {
            FWK_Profiler_StartEvent(OASISLT_EVT_DET_START);
            pResult->face_id         = INVALID_FACE_ID;
            pResult->face_count      = 0;
            pResult->face_recognized = 0;
            pResult->result          = 0;
            pResult->qualityCheck    = kOasisLiteQualityCheck_Count;
            memset(pResult->name, 0x0, FACE_NAME_MAX_LEN);

            memset(pDebugInfo, 0, sizeof(oasis_lite_debug_t));
            pDebugInfo->faceID = INVALID_FACE_ID;
        }
        break;

        case OASISLT_EVT_DET_COMPLETE:
        {
            FWK_Profiler_EndEvent(OASISLT_EVT_DET_START);

            if (para->faceBoxRGB == NULL && para->faceBoxIR == NULL)
            {
                if (s_debugOption)
                {
                    OASIS_LOGD("[OASIS] DET:No face detected");
                }
                pResult->face_count = 0;
            }
            else
            {
                if (s_debugOption)
                {
                    if (para->faceBoxRGB != NULL)
                    {
                        OASIS_LOGD("[OASIS] RGB DET:[Left: %d, Top: %d, Right: %d, Bottom: %d]", para->faceBoxRGB->rect[0],
                                   para->faceBoxRGB->rect[1], para->faceBoxRGB->rect[2], para->faceBoxRGB->rect[3]);
                    }

                    if (para->faceBoxIR != NULL)
                    {
                        OASIS_LOGD("[OASIS] IR  DET:[Left: %d, Top: %d, Right: %d, Bottom: %d]", para->faceBoxIR->rect[0],
                                   para->faceBoxIR->rect[1], para->faceBoxIR->rect[2], para->faceBoxIR->rect[3]);
                    }

                }

                pResult->face_count = 1;
                pResult->face_box   = (*(para->faceBoxIR));
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
                    pResult->qualityCheck = kOasisLiteQualityCheck_Ok;
                    pDebugInfo->isOk      = 1;
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] Quality:ok");
                    }
                }
                break;

                case OASIS_QUALITY_RESULT_FACE_TOO_SMALL:
                {
                    pResult->qualityCheck   = kOasisLiteQualityCheck_SmallFace;
                    pDebugInfo->isSmallFace = 1;
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] Quality:Small Face");
                    }
                }
                break;

                case OASIS_QUALITY_RESULT_FAIL_BRIGHTNESS_DARK:
                case OASIS_QUALITY_RESULT_FAIL_BRIGHTNESS_OVEREXPOSURE:
                {
                    pResult->qualityCheck     = kOasisLiteQualityCheck_Brightness;
                    pDebugInfo->rgbBrightness = para->reserved[12];
                    pDebugInfo->irBrightness  = para->reserved[10];
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] Quality:Brightness rgb[%d], ir[%d]", para->reserved[12], para->reserved[10]);
                    }

                    if (para->qualityResult == OASIS_QUALITY_RESULT_FAIL_BRIGHTNESS_DARK)
                    {
                        _oasis_adjustBrightness(OASISLT_INT_FRAME_IDX_IR, 1, NULL);
                    }
                    else if (para->qualityResult == OASIS_QUALITY_RESULT_FAIL_BRIGHTNESS_OVEREXPOSURE)
                    {
                        _oasis_adjustBrightness(OASISLT_INT_FRAME_IDX_IR, 0, NULL);
                    }
                }
                break;

                case OASIS_QUALITY_RESULT_FACE_BLUR:
                {
                    pResult->qualityCheck = kOasisLiteQualityCheck_Blurry;
                    pDebugInfo->isBlurry  = 1;
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] Quality:Blurry Face [%d]", para->reserved[4]);
                    }
                }
                break;

                case OASIS_QUALITY_RESULT_FACE_ORIENTATION_UNMATCH:
                {
                    pResult->qualityCheck  = kOasisLiteQualityCheck_SideFace;
                    pDebugInfo->isSideFace = 1;
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] Quality:Side face[%d]", para->reserved[2]);
                    }
                }
                break;


                case OASIS_QUALITY_RESULT_RGB_FAKE:
                {
                    pResult->qualityCheck  = kOasisLiteQualityCheck_FakeFace;
                    pDebugInfo->is2dFake = 1;
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] Quality:RGB Fake face[%d]", para->reserved[8]);
                    }
                }
                break;

                case OASIS_QUALITY_RESULT_IR_FAKE:
                {
                    pResult->qualityCheck  = kOasisLiteQualityCheck_FakeFace;
                    pDebugInfo->is2dFake = 1;
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] Quality:IR Fake face[%d]", para->reserved[6]);
                    }
                }
                break;

                case OASIS_QUALITY_RESULT_PARTIAL_BRIGHTNESS_FAIL:
                {
                    pResult->qualityCheck = kOasisLiteQualityCheck_PartialBrightness;
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] Quality:Face Partial Brightness irlrsim[%d], iruddiff[%d]", para->reserved[21], para->reserved[22]);
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

            if (para->recResult == OASIS_REC_RESULT_KNOWN_FACE)
            {
                /* Recognized */
                pResult->rec_result      = kOASISLiteRecognitionResult_Success;
                pResult->face_recognized = 1;
                pResult->face_id         = para->faceID;
                pDebugInfo->sim          = para->reserved[0];
                pDebugInfo->faceID       = para->faceID;

                if (s_OasisScenario == kOasisScenario_Thermostat)
                {
                    s_pFacedbOps = &g_facedb_ops;
                }
                else if (s_OasisScenario == kOasisScenario_Security)
                {
                    s_pFacedbOps = &g_security_facedb_ops;
                }

                if (s_pFacedbOps != NULL)
                {
                    char *faceName = s_pFacedbOps->getNameWithId(para->faceID);
                    strcpy(pResult->name, faceName);
                    if (s_debugOption)
                    {
                        OASIS_LOGD("[OASIS] KNOWN_FACE:[%s][%d]", faceName, para->reserved[0]);

                        if (s_OasisScenario == kOasisScenario_Thermostat)
                        {
                            thermostat_attribute_t attr;
                            memset(&attr, 0x0, sizeof(attr));

                            s_pThermostatdbOps->getWithId(para->faceID, &attr);
                            OASIS_LOGD("[OASIS] THERMOSTAT id[%d]: [%d][%d][%s]", attr.id, attr.mode, attr.fan_speed,
                                       attr.temperature);
                        }
                    }
                }
            }
            else if (para->recResult == OASIS_REC_RESULT_UNKNOWN_FACE)
            {
                pResult->rec_result      = kOASISLiteRecognitionResult_Unknown;
                pResult->face_recognized = 0;
                pResult->face_id         = INVALID_FACE_ID;
                pDebugInfo->sim          = para->reserved[0];
                pDebugInfo->faceID       = para->faceID;

                // unknown face
                if (s_debugOption)
                {
                    OASIS_LOGD("[OASIS] UNKNOWN_FACE:Sim:[%d.%d%%]:[%d]", (int)(para->reserved[0] / 100),
                               (int)(para->reserved[0] % 100), para->faceID);
                }
            }
            else
            {
                pResult->rec_result      = kOASISLiteRecognitionResult_Invalid;
                pResult->face_recognized = 0;
                pResult->face_id         = INVALID_FACE_ID;
                pDebugInfo->sim          = para->reserved[0];
                if (s_debugOption)
                {
                    OASIS_LOGD("[OASIS] INVALID_FACE [%d.%d%%]:[%d]", (int)(para->reserved[0] / 100), (int)(para->reserved[0] % 100), para->faceID);
                }

                // here multi-frame is different, We consider it as blur face.
                pResult->qualityCheck = kOasisLiteQualityCheck_Blurry;
                pDebugInfo->isBlurry  = 1;
            }
        }
        break;

        case OASISLT_EVT_REG_START:
        {
            FWK_Profiler_StartEvent(OASISLT_EVT_REG_START);
        }
        break;

        case OASISLT_EVT_REG_COMPLETE:
        {
            FWK_Profiler_EndEvent(OASISLT_EVT_REG_COMPLETE);

            if (para->regResult == OASIS_REG_RESULT_OK)
            {
                if (s_debugOption)
                {
                    LOGD("[OASIS] kOASISLiteRegistrationResult_Success [%d]", para->faceID);
                }

                pResult->reg_result = kOASISLiteRegistrationResult_Success;
                pResult->face_id    = para->faceID;
            }
            else if (para->regResult == OASIS_REG_RESULT_DUP)
            {
                if (s_debugOption)
                {
                    LOGD("[OASIS] kOASISLiteRegistrationResult_Duplicated [%d]", para->faceID);
                }

                pResult->reg_result = kOASISLiteRegistrationResult_Duplicated;
                pResult->face_id    = para->faceID;
                if (para->faceID != INVALID_FACE_ID)
                {
                    if (s_OasisScenario == kOasisScenario_Thermostat)
                    {
                        s_pFacedbOps = &g_facedb_ops;
                    }
                    else if (s_OasisScenario == kOasisScenario_Security)
                    {
                        s_pFacedbOps = &g_security_facedb_ops;
                    }

                    if (s_pFacedbOps != NULL)
                    {
                        char *faceName = s_pFacedbOps->getNameWithId(para->faceID);
                        strcpy(pResult->name, faceName);
                    }
                }
            }
            else
            {
                pResult->reg_result = kOASISLiteRegistrationResult_Invalid;
                pResult->face_id    = INVALID_FACE_ID;
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

    if (s_OasisScenario == kOasisScenario_Thermostat)
    {
        s_pFacedbOps = &g_facedb_ops;
    }
    else if (s_OasisScenario == kOasisScenario_Security)
    {
        s_pFacedbOps = &g_security_facedb_ops;
    }

    if (s_pFacedbOps != NULL)
    {
        int dbCount = s_pFacedbOps->getFaceCount();
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

    if (s_OasisScenario == kOasisScenario_Thermostat)
    {
        s_pFacedbOps = &g_facedb_ops;
    }
    else if (s_OasisScenario == kOasisScenario_Security)
    {
        s_pFacedbOps = &g_security_facedb_ops;
    }

    if (s_pFacedbOps != NULL)
    {
        facedb_status_t status = s_pFacedbOps->genId(faceId);
        if ((status == kFaceDBStatus_Success) && (*faceId != INVALID_FACE_ID))
        {
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

    if (s_OasisScenario == kOasisScenario_Thermostat)
    {
        s_pFacedbOps = &g_facedb_ops;
    }
    else if (s_OasisScenario == kOasisScenario_Security)
    {
        s_pFacedbOps = &g_security_facedb_ops;
    }

    if (s_pFacedbOps != NULL)
    {
        int faceItemSize = OASISLT_getFaceItemSize();
        ret = s_pFacedbOps->updFaceWithId(faceId, s_pFacedbOps->getNameWithId(faceId), faceData, faceItemSize);

        _loadUserListIntoSharedMemory();
    }

    OASIS_LOGI("--_oasis_updFace");
    return ret;
}

static int _oasis_delFace(uint16_t faceId, void *userData)
{
    int ret = 0;
    OASIS_LOGI("++_oasis_delFace");

    if (faceId != INVALID_FACE_ID)
    {
        if (s_OasisScenario == kOasisScenario_Thermostat)
        {
            s_pFacedbOps = &g_facedb_ops;
        }
        else if (s_OasisScenario == kOasisScenario_Security)
        {
            s_pFacedbOps = &g_security_facedb_ops;
        }

        if (s_pFacedbOps != NULL)
        {
            facedb_status_t status = s_pFacedbOps->delFaceWithId(faceId);
            if (status == kFaceDBStatus_Success)
            {
                ret = 0;
            }

            _loadUserListIntoSharedMemory();
        }
    }

    OASIS_LOGI("--_oasis_delFace");
    return ret;
}

static void _oasis_start_recognition(void)
{
    LOGD("[OASIS] _oasis_start_recognition");
    memset(&s_OasisHomePanel.result, 0, sizeof(s_OasisHomePanel.result));

    s_OasisHomePanel.prevRunFlag            = s_OasisHomePanel.currRunFlag;
    s_OasisHomePanel.currRunFlag            = OASIS_DET_REC;
    s_OasisHomePanel.result.id              = kVisionAlgoID_OasisLite;
    s_OasisHomePanel.result.oasisLite.state = kOASISLiteState_Recognition;

    _oasis_set_ir_led_brightness(s_OasisHomePanel.dev, OASIS_HOMEPANEL_IR_PWM_VAL);
    _oasis_dev_notifyResult(s_OasisHomePanel.dev, &(s_OasisHomePanel.result));
}

static void _oasis_start_registration(void)
{
    LOGD("[OASIS] _oasis_start_registration");
    memset(&s_OasisHomePanel.result, 0, sizeof(s_OasisHomePanel.result));

    s_OasisHomePanel.prevRunFlag            = s_OasisHomePanel.currRunFlag;
    s_OasisHomePanel.currRunFlag            = OASIS_DET_REC_REG;
    s_OasisHomePanel.result.id              = kVisionAlgoID_OasisLite;
    s_OasisHomePanel.result.oasisLite.state = kOASISLiteState_Registration;

    _oasis_set_ir_led_brightness(s_OasisHomePanel.dev, OASIS_HOMEPANEL_IR_PWM_VAL);
    _oasis_dev_notifyResult(s_OasisHomePanel.dev, &(s_OasisHomePanel.result));
}

static void _oasis_stop(void)
{
    LOGD("[OASIS] _oasis_stop");
    memset(&s_OasisHomePanel.result, 0, sizeof(s_OasisHomePanel.result));

    s_OasisHomePanel.prevRunFlag            = s_OasisHomePanel.currRunFlag;
    s_OasisHomePanel.currRunFlag            = OASIS_RUN_FLAG_STOP;
    s_OasisHomePanel.result.id              = kVisionAlgoID_OasisLite;
    s_OasisHomePanel.result.oasisLite.state = kOASISLiteState_Stopped;

    _oasis_set_ir_led_brightness(s_OasisHomePanel.dev, 0);
    _oasis_dev_notifyResult(s_OasisHomePanel.dev, &(s_OasisHomePanel.result));
}

static void _oasis_set_ir_led_brightness(const vision_algo_dev_t *dev, int brightness)
{
    if ((brightness < 0) || (100 < brightness))
    {
        return;
    }

    event_common_t event = {0};

    event.eventBase.eventId = kEventID_SetIRLedBrightness;
    event.eventBase.respond = NULL;
    event.irLed.brightness  = (uint8_t)brightness;

    valgo_event_t valgoEvent = {.eventId   = kVAlgoEvent_VisionLedPwmControl,
                                 .eventInfo = kEventInfo_Remote,
                                 .data      = &event,
                                 .size      = sizeof(event_common_t),
                                 .copy      = 1};

    if (dev != NULL && dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev->id, valgoEvent, fromISR);
    }
}

static void _oasis_set_camera_exposure(const vision_algo_dev_t *dev, event_common_t *event)
{
    /* Build Valgo event */
    valgo_event_t valgoEvent = {.eventId   = kVAlgoEvent_VisionCamExpControl,
                                .eventInfo = kEventInfo_Local,
                                .data      = event,
                                .size      = sizeof(event_common_t),
                                .copy      = 1};

    if (dev != NULL && event != NULL && dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev->id, valgoEvent, fromISR);
    }
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
    _oasis_set_camera_exposure(s_OasisHomePanel.dev, &eventCamExposure);

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

    switch (pResult->state)
    {
        case kOASISLiteState_Recognition:
        {
            if (pResult->face_recognized)
            {
                _oasis_dev_notifyResult(dev, &(pParam->result));
                _oasis_stop();
            }
            else if (pResult->face_count > 0)
            {
                _oasis_dev_notifyResult(dev, &(pParam->result));
            }
        }
        break;

        case kOASISLiteState_Registration:
        {
            // notify the result
            if ((pResult->reg_result == kOASISLiteRegistrationResult_Success) ||
                (pResult->reg_result == kOASISLiteRegistrationResult_Duplicated))
            {
                if (pResult->reg_result == kOASISLiteRegistrationResult_Success)
                {
                    s_faceId = pResult->face_id;
                }
                _oasis_dev_notifyResult(dev, &(pParam->result));
                _oasis_stop();
            }
            else if (pResult->face_count > 0)
            {
                _oasis_dev_notifyResult(dev, &(pParam->result));
            }
        }
        break;

        default:
        {
            if (s_debugOption)
            {
                _oasis_dev_notifyResult(dev, &(pParam->result));
            }
        }
        break;
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

    dev->data.autoStart = OASIS_HOMEPANEL_AUTO_START;

    dev->data.frames[kVAlgoFrameID_RGB].height       = OASIS_RGB_FRAME_HEIGHT;
    dev->data.frames[kVAlgoFrameID_RGB].width        = OASIS_RGB_FRAME_WIDTH;
    dev->data.frames[kVAlgoFrameID_RGB].pitch        = OASIS_RGB_FRAME_WIDTH * OASIS_RGB_FRAME_BYTE_PER_PIXEL;
    dev->data.frames[kVAlgoFrameID_RGB].is_supported = 1;
    dev->data.frames[kVAlgoFrameID_RGB].rotate       = kCWRotateDegree_0;
    dev->data.frames[kVAlgoFrameID_RGB].flip         = kFlipMode_None;
    dev->data.frames[kVAlgoFrameID_RGB].format       = kPixelFormat_BGR;
    dev->data.frames[kVAlgoFrameID_RGB].srcFormat    = OASIS_RGB_FRAME_SRC_FORMAT;
    dev->data.frames[kVAlgoFrameID_RGB].data         = s_OasisRgbFrameBuffer;

    dev->data.frames[kVAlgoFrameID_IR].height       = OASIS_IR_FRAME_HEIGHT;
    dev->data.frames[kVAlgoFrameID_IR].width        = OASIS_IR_FRAME_WIDTH;
    dev->data.frames[kVAlgoFrameID_IR].pitch        = OASIS_IR_FRAME_WIDTH * OASIS_IR_FRAME_BYTE_PER_PIXEL;
    dev->data.frames[kVAlgoFrameID_IR].is_supported = 1;
    dev->data.frames[kVAlgoFrameID_IR].rotate       = kCWRotateDegree_0;
    dev->data.frames[kVAlgoFrameID_IR].flip         = kFlipMode_None;
    dev->data.frames[kVAlgoFrameID_IR].format       = kPixelFormat_BGR;
    dev->data.frames[kVAlgoFrameID_IR].srcFormat    = OASIS_IR_FRAME_SRC_FORMAT;
    dev->data.frames[kVAlgoFrameID_IR].data         = s_OasisIrFrameBuffer;

    // init the RGB frame
    s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_RGB].height = OASIS_RGB_FRAME_HEIGHT;
    s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_RGB].width  = OASIS_RGB_FRAME_WIDTH;
    s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_RGB].fmt    = OASIS_IMG_FORMAT_BGR888;
    s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_RGB].data   = dev->data.frames[kVAlgoFrameID_RGB].data;
    s_OasisHomePanel.pframes[OASISLT_INT_FRAME_IDX_RGB]       = &s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_RGB];

    // init the IR frame
    s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_IR].height = OASIS_IR_FRAME_HEIGHT;
    s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_IR].width  = OASIS_IR_FRAME_WIDTH;
    s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_IR].fmt    = OASIS_IMG_FORMAT_BGR888;
    s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_IR].data   = dev->data.frames[kVAlgoFrameID_IR].data;
    s_OasisHomePanel.pframes[OASISLT_INT_FRAME_IDX_IR]       = &s_OasisHomePanel.frames[OASISLT_INT_FRAME_IDX_IR];

    // init the oasis config
    s_OasisHomePanel.config.imgType              = OASIS_IMG_TYPE_IR_RGB_DUAL;
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
        if (s_OasisHomePanel.config.memPool == NULL)
        {
            OASIS_LOGE("Unable to allocate memory for OASIS mem pool of size %d.", s_OasisHomePanel.config.size);
            while (1)
                ;
        }

        oasisRet = OASISLT_init(&s_OasisHomePanel.config);
    }

    if (oasisRet != OASISLT_OK)
    {
        OASIS_LOGE("[OASIS] OASISLT_init ret %d", ret);
        ret = kStatus_HAL_ValgoInitError;
        return ret;
    }

    /* Initial Oasis status */
    s_OasisHomePanel.prevRunFlag = OASIS_RUN_FLAG_STOP;
    s_OasisHomePanel.currRunFlag = OASIS_RUN_FLAG_STOP;

#ifdef ENABLE_FACEDB
    facedb_status_t status_facedb = kFaceDBStatus_Success;

    /* Initial Face Database */
    s_pFacedbOps  = &g_facedb_ops;
    status_facedb = s_pFacedbOps->init(OASISLT_getFaceItemSize());
    if ((kFaceDBStatus_Success != status_facedb) && (kFaceDBStatus_VersionMismatch != status_facedb))
    {
        OASIS_LOGE("[OASIS] FaceDb initial failed");
        ret = kStatus_HAL_ValgoInitError;
        return ret;
    }

    /* Initial Security Face Database */
    s_pFacedbOps  = &g_security_facedb_ops;
    status_facedb = s_pFacedbOps->init(OASISLT_getFaceItemSize());
    if ((kFaceDBStatus_Success != status_facedb) && (kFaceDBStatus_VersionMismatch != status_facedb))
    {
        OASIS_LOGE("[OASIS] Security FaceDb initial failed");
        ret = kStatus_HAL_ValgoInitError;
        return ret;
    }

    thermostatdb_status_t status_thermostatdb = kThermostatDBStatus_Success;
    if (s_pThermostatdbOps == NULL)
    {
        s_pThermostatdbOps  = &g_thermostatdb_ops;
        status_thermostatdb = s_pThermostatdbOps->init();
        if (status_thermostatdb != kThermostatDBStatus_Success)
        {
            OASIS_LOGE("[OASIS] Thermostatdb initial failed");
            ret = kStatus_HAL_ValgoInitError;
            return ret;
        }
    }

    _loadUserListIntoSharedMemory();
#endif

    s_pFaceFeature = pvPortMalloc(OASISLT_getFaceItemSize());
    if (s_pFaceFeature == NULL)
    {
        OASIS_LOGE("[OASIS] Unable to allocate memory for face feature.");
    }

    if (dev->data.autoStart)
    {
        _oasis_start_recognition();
    }

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

    if (s_OasisHomePanel.currRunFlag == OASIS_DET_REC || s_OasisHomePanel.currRunFlag == OASIS_DET_REC_REG)
    {
        FWK_Profiler_ClearEvents();

        int oasis_ret = OASISLT_run_extend(s_OasisHomePanel.pframes, s_OasisHomePanel.currRunFlag,
                                           s_OasisHomePanel.config.minFace, &s_OasisHomePanel);
        if (oasis_ret)
        {
            OASIS_LOGE("OASISLT_run_extend failed with error: %d", oasis_ret);
        }

        FWK_Profiler_Log();

        _process_inference_result(&s_OasisHomePanel);
    }

    if (s_OasisHomePanel.currRunFlag == OASIS_RUN_FLAG_STOP)
    {
        return kStatus_HAL_ValgoStop;
    }

    OASIS_LOGI("--HAL_VisionAlgoDev_OasisHomePanel_Run");
    return ret;
}

static facedb_status_t _thermostatdb_add(respond_add_thermostat_user_event_t *userinfo)
{
    facedb_status_t status = kFaceDBStatus_Success;
    s_pFacedbOps           = &g_facedb_ops;

    LOGD("[OASIS]: thermostatdb add user id:%d", userinfo->id);

    if (userinfo->id == INVALID_FACE_ID || userinfo->id != s_faceId)
    {
        LOGE("[OASIS]: thermostatdb request id: %u, current id: %u", userinfo->id, s_faceId);
        return kFaceDBStatus_WrongID;
    }

    if (userinfo->name[0] == '\0')
    {
        status = s_pFacedbOps->addFace(s_faceId, NULL, s_pFaceFeature, OASISLT_getFaceItemSize());
    }
    else
    {
        status = s_pFacedbOps->addFace(s_faceId, (char *)userinfo->name, s_pFaceFeature, OASISLT_getFaceItemSize());
    }

    if (status != kFaceDBStatus_Success)
    {
        LOGD("[OASIS]: failed to add id:%d to thermostat facedb", s_faceId);
        return status;
    }

    thermostat_attribute_t attr = {0};
    attr.id                     = userinfo->id;
    attr.mode                   = userinfo->mode;
    attr.fan_speed              = userinfo->fan_speed;
    memcpy(attr.temperature, userinfo->temperature, sizeof(attr.temperature));

    if (s_pThermostatdbOps->addWithId(attr.id, &attr) != kThermostatDBStatus_Success)
    {
        LOGD("[OASIS]: Thermostat add user fail[%d][%d][%d][%s]", attr.id, attr.mode, attr.fan_speed, attr.temperature);

        s_pFacedbOps->delFaceWithId(userinfo->id);
        return kFaceDBStatus_Failed;
    }

    _loadUserListIntoSharedMemory();

    memset(s_pFaceFeature, 0x0, OASISLT_getFaceItemSize());
    s_faceId = INVALID_FACE_ID;

    return status;
}

static facedb_status_t _thermostatdb_update(respond_add_thermostat_user_event_t *userinfo)
{
    facedb_status_t status = kFaceDBStatus_Success;
    s_pFacedbOps           = &g_facedb_ops;

    LOGD("[OASIS]: thermostatdb update user id:%d", userinfo->id);

    if (userinfo->id == INVALID_FACE_ID || userinfo->name[0] == '\0')
    {
        LOGE("[OASIS]: thermostatdb request id: %u, name: %s", userinfo->id, userinfo->name);
        return kFaceDBStatus_WrongID;
    }

    status = s_pFacedbOps->updNameWithId(userinfo->id, (char *)userinfo->name);
    if (status != kFaceDBStatus_Success)
    {
        LOGE("[OASIS]: update thermostatdb failed id:%d name:%s", userinfo->id, userinfo->name);
        return status;
    }

    thermostat_attribute_t attr = {0};
    attr.id                     = userinfo->id;
    attr.mode                   = userinfo->mode;
    attr.fan_speed              = userinfo->fan_speed;
    memcpy(attr.temperature, userinfo->temperature, sizeof(attr.temperature));

    if (s_pThermostatdbOps->updWithId(attr.id, &attr) != kThermostatDBStatus_Success)
    {
        LOGE("[OASIS]: Thermostat update user fail[%d][%d][%d][%s]", attr.id, attr.mode, attr.fan_speed,
             attr.temperature);
        return kFaceDBStatus_Failed;
    }

    _loadUserListIntoSharedMemory();
    return status;
}

static facedb_status_t _securitydb_add(respond_add_thermostat_user_event_t *userinfo)
{
    facedb_status_t status    = kFaceDBStatus_Success;
    const facedb_ops_t *dbOps = &g_security_facedb_ops;

    LOGD("[OASIS]: securitydb add user id:%d", userinfo->id);

    if (userinfo->id == INVALID_FACE_ID || userinfo->id != s_faceId)
    {
        LOGE("[OASIS]: security db request id: %u, current id: %u", userinfo->id, s_faceId);
        return kFaceDBStatus_WrongID;
    }

    if (userinfo->name[0] == '\0')
    {
        status = dbOps->addFace(s_faceId, NULL, s_pFaceFeature, OASISLT_getFaceItemSize());
    }
    else
    {
        status = dbOps->addFace(s_faceId, (char *)userinfo->name, s_pFaceFeature, OASISLT_getFaceItemSize());
    }

    if (status != kFaceDBStatus_Success)
    {
        LOGD("[OASIS]: securitydb add user failed [%d]", s_faceId);
    }

    _loadUserListIntoSharedMemory();

    memset(s_pFaceFeature, 0x0, OASISLT_getFaceItemSize());
    s_faceId = INVALID_FACE_ID;

    return status;
}

static facedb_status_t _securitydb_update(respond_add_thermostat_user_event_t *userinfo)
{
    facedb_status_t status    = kFaceDBStatus_Success;
    const facedb_ops_t *dbOps = &g_security_facedb_ops;

    LOGD("[OASIS]: securitydb update user id:%d", userinfo->id);

    if (userinfo->id == INVALID_FACE_ID || userinfo->name[0] == '\0')
    {
        LOGE("[OASIS]: securitydb request id: %u, name: %s", userinfo->id, userinfo->name);
        return kFaceDBStatus_WrongID;
    }

    status = dbOps->updNameWithId(userinfo->id, userinfo->name);
    if (status != kFaceDBStatus_Success)
    {
        LOGD("[OASIS]: update securitydb failed id:%d name:%s", userinfo->id, userinfo->name);
        return status;
    }

    _loadUserListIntoSharedMemory();
    return status;
}

static hal_valgo_status_t HAL_VisionAlgoDev_OasisHomePanel_InputNotify(const vision_algo_dev_t *receiver, void *data)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    OASIS_LOGI("++HAL_VisionAlgoDev_OasisHomePanel_InputNotify");

    event_base_t eventBase = *(event_base_t *)data;
    switch (eventBase.eventId)
    {
        case kEventFaceRecID_OasisGetState:
        {
            if (s_OasisHomePanel.currRunFlag == OASIS_RUN_FLAG_STOP)
            {
                LOGD("[OASIS] stop.");
            }
            else if (s_OasisHomePanel.currRunFlag == OASIS_DET_REC)
            {
                if (s_OasisScenario == kOasisScenario_Thermostat)
                {
                    LOGD("[OASIS] face recognition.");
                }
                else if (s_OasisScenario == kOasisScenario_Security)
                {
                    LOGD("[OASIS] face security recognition.");
                }
            }
            else if (s_OasisHomePanel.currRunFlag == OASIS_DET_REC_REG)
            {
                if (s_OasisScenario == kOasisScenario_Thermostat)
                {
                    LOGD("[OASIS] face registration.");
                }
                else if (s_OasisScenario == kOasisScenario_Security)
                {
                    LOGD("[OASIS] face security registration.");
                }
            }
        }
        break;

        case kEventFaceRecID_OasisSetState:
        case kEventFaceRecID_OasisSetSecurityState:
        {
            event_face_rec_t eventOasis = *(event_face_rec_t *)data;

            if (eventBase.eventId == kEventFaceRecID_OasisSetState)
            {
                s_OasisScenario = kOasisScenario_Thermostat;
                LOGD("[OASIS] Set Thermostat Scenario");
            }
            else if (eventBase.eventId == kEventFaceRecID_OasisSetSecurityState)
            {
                s_OasisScenario = kOasisScenario_Security;
                LOGD("[OASIS] Set Security Scenario");
            }

            LOGD("[OASIS] set state [%d][%d]", s_OasisScenario, eventOasis.oasisState.state);

            s_faceId = INVALID_FACE_ID;
            memset(s_pFaceFeature, 0x0, OASISLT_getFaceItemSize());

            switch (eventOasis.oasisState.state)
            {
                case kOASISLiteState_Stopped:
                {
                    _oasis_stop();
                }
                break;

                case kOASISLiteState_Recognition:
                {
                    _oasis_start_recognition();
                    _oasis_dev_requestFrame(s_OasisHomePanel.dev);
                }
                break;

                case kOASISLiteState_Registration:
                {
                    _oasis_start_registration();
                    _oasis_dev_requestFrame(s_OasisHomePanel.dev);
                }
                break;

                default:
                    break;
            }
        }
        break;

        case kEventFaceRecId_RespondAddUser:
        {
            if (s_OasisScenario == kOasisScenario_Thermostat)
            {
                event_smart_tlhmi_t *pEvent = (event_smart_tlhmi_t *)data;
                _thermostatdb_add(&pEvent->thermostatUserInfo);
            }
            else
            {
                LOGD("[OASIS]: wrong scenario [%d][%d]", eventBase.eventId, s_OasisScenario);
            }
        }
        break;

        case kEventFaceRecId_RespondUpdateUser:
        {
            event_smart_tlhmi_t *pEvent = (event_smart_tlhmi_t *)data;
            _thermostatdb_update(&pEvent->thermostatUserInfo);
        }
        break;

        case kEventFaceRecId_RespondAddSecurityUser:
        {
            if (s_OasisScenario == kOasisScenario_Security)
            {
                event_smart_tlhmi_t *pEvent = (event_smart_tlhmi_t *)data;
                _securitydb_add(&pEvent->thermostatUserInfo);
            }
            else
            {
                LOGD("[OASIS]: wrong scenario[%d][%d]", eventBase.eventId, s_OasisScenario);
            }
        }
        break;

        case kEventFaceRecId_RespondUpdateSecurityUser:
        {
            if (s_OasisScenario == kOasisScenario_Security)
            {
                event_smart_tlhmi_t *pEvent = (event_smart_tlhmi_t *)data;
                _securitydb_update(&pEvent->thermostatUserInfo);
            }
            else
            {
                LOGD("[OASIS]: wrong scenario [%d][%d]", eventBase.eventId, s_OasisScenario);
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

        case kEventFaceRecID_DelUser:
        case kEventFaceRecId_DelSecurityUser:
        {
            event_face_rec_t event = *(event_face_rec_t *)data;

            if (eventBase.eventId == kEventFaceRecID_DelUser)
            {
                s_pFacedbOps = &g_facedb_ops;
            }
            else if (eventBase.eventId == kEventFaceRecId_DelSecurityUser)
            {
                s_pFacedbOps = &g_security_facedb_ops;
            }

            if (event.delFace.hasID)
            {
                LOGD("[OASIS] Del user [%d]", event.delFace.id);
                if (s_pFacedbOps != NULL)
                {
                    if (s_pFacedbOps->delFaceWithId(event.delFace.id) != kFaceDBStatus_Success)
                    {
                        LOGE("[OASIS] Del user [%d] failed", event.delFace.id);
                    }
                }

                if (eventBase.eventId == kEventFaceRecID_DelUser)
                {
                    if (s_pThermostatdbOps->delWithId(event.delFace.id) != kThermostatDBStatus_Success)
                    {
                        LOGE("[OASIS] Del thermostat [%d] failed", event.delFace.id);
                    }
                }
            }
            else if (event.delFace.hasName)
            {
                LOGD("[OASIS] Del user [%s]", event.delFace.name);
                uint16_t id = INVALID_FACE_ID;
                if (s_pFacedbOps != NULL)
                {
                    if (s_pFacedbOps->getIdWithName(event.delFace.name, &id) == kFaceDBStatus_Success)
                    {
                        if (s_pFacedbOps->delFaceWithId(id) != kFaceDBStatus_Success)
                        {
                            LOGE("[OASIS] Del user [%s] failed", event.delFace.name);
                        }

                        if (eventBase.eventId == kEventFaceRecID_DelUser)
                        {
                            if (s_pThermostatdbOps->delWithId(id) != kThermostatDBStatus_Success)
                            {
                                LOGE("[OASIS] Del thermostat [%d] failed", event.delFace.id);
                            }
                        }
                    }
                    else
                    {
                        LOGE("[OASIS] Del user [%s] failed", event.delFace.name);
                    }
                }
            }

            _loadUserListIntoSharedMemory();
        }
        break;

        case kEventFaceRecID_DelUserAll:
        {
            OASIS_LOGD("[OASIS] Del all user");

            s_pFacedbOps = &g_facedb_ops;
            if (s_pFacedbOps != NULL)
            {
                if (s_pFacedbOps->delFaceWithId(INVALID_FACE_ID) != kFaceDBStatus_Success)
                {
                    OASIS_LOGE("[OASIS] Del all thermostat user failed");
                }
            }

            s_pFacedbOps = &g_security_facedb_ops;
            if (s_pFacedbOps != NULL)
            {
                if (s_pFacedbOps->delFaceWithId(INVALID_FACE_ID) != kFaceDBStatus_Success)
                {
                    OASIS_LOGE("[OASIS] Del all security user failed");
                }
            }

            if (s_pThermostatdbOps != NULL)
            {
                if (s_pThermostatdbOps->delWithId(INVALID_FACE_ID) != kThermostatDBStatus_Success)
                {
                    OASIS_LOGE("[OASIS] Del all thermostat information failed");
                }
            }

            _loadUserListIntoSharedMemory();
        }
        break;

        case kEventFaceRecID_GetUserList:
        {
            LOGD("[OASIS] Get thermostat user list");
            s_pFacedbOps = &g_facedb_ops;
            int count    = s_pFacedbOps->getFaceCount();
            if (count > 0)
            {
                uint16_t *faceIds = (uint16_t *)pvPortMalloc(count * sizeof(uint16_t));
                if (faceIds != NULL)
                {
                    respond_add_thermostat_user_event_t *userInfos =
                        (respond_add_thermostat_user_event_t *)pvPortMalloc(
                            count * sizeof(respond_add_thermostat_user_event_t));
                    if (userInfos != NULL)
                    {
                        s_pFacedbOps->getIds(faceIds);

                        for (int i = 0; i < count; i++)
                        {
                            thermostat_attribute_t attr;
                            memset(&attr, 0x0, sizeof(attr));

                            userInfos[i].id = faceIds[i];
                            memcpy(userInfos[i].name, s_pFacedbOps->getNameWithId(faceIds[i]),
                                   sizeof(userInfos[i].name));

                            s_pThermostatdbOps->getWithId(faceIds[i], &attr);
                            userInfos[i].mode      = attr.mode;
                            userInfos[i].fan_speed = attr.fan_speed;
                            memcpy(userInfos[i].temperature, attr.temperature, sizeof(attr.temperature));
                        }

                        for (int i = 0; i < count; i++)
                        {
                            LOGD("[%d][%s][%d][%d][%s]", userInfos[i].id, userInfos[i].name, userInfos[i].mode,
                                 userInfos[i].fan_speed, userInfos[i].temperature);
                        }
                        vPortFree(userInfos);
                    }
                    vPortFree(faceIds);
                }
            }

            LOGD("[OASIS] Get security user list");
            s_pFacedbOps = &g_security_facedb_ops;
            count        = s_pFacedbOps->getFaceCount();
            if (count > 0)
            {
                uint16_t *faceIds = (uint16_t *)pvPortMalloc(count * sizeof(uint16_t));
                if (faceIds != NULL)
                {
                    face_user_info_t *userInfos = (face_user_info_t *)pvPortMalloc(count * sizeof(face_user_info_t));
                    if (userInfos != NULL)
                    {
                        s_pFacedbOps->getIds(faceIds);

                        for (int i = 0; i < count; i++)
                        {
                            userInfos[i].id = faceIds[i];
                            memcpy(userInfos[i].name, s_pFacedbOps->getNameWithId(faceIds[i]),
                                   sizeof(userInfos[i].name));
                        }

                        for (int i = 0; i < count; i++)
                        {
                            LOGD("[%d][%s]", userInfos[i].id, userInfos[i].name);
                        }
                        vPortFree(userInfos);
                    }
                    vPortFree(faceIds);
                }
            }
        }
        break;

        case kEventFaceRecID_GetUserCount:
        {
            s_pFacedbOps = &g_facedb_ops;
            int count    = s_pFacedbOps->getFaceCount();
            LOGD("[OASIS] Get thermostat user count [%d]", count);

            s_pFacedbOps = &g_security_facedb_ops;
            count        = s_pFacedbOps->getFaceCount();
            LOGD("[OASIS] Get security user count [%d]", count);
        }
        break;

        case kEventFaceRecId_RegisterSecuirtyUser:
        {
            event_smart_tlhmi_t *pEvent = (event_smart_tlhmi_t *)data;

            if (s_faceId != INVALID_FACE_ID)
            {
                if (s_pFacedbOps != NULL)
                {
                    if (pEvent->securityUser.id >= 0)
                    {
                        // add new user face data to db
                        facedb_status_t status =
                            s_pFacedbOps->addFace(s_faceId, NULL, s_pFaceFeature, OASISLT_getFaceItemSize());
                        if (status == kFaceDBStatus_Success)
                        {
                            memset(s_pFaceFeature, 0x0, OASISLT_getFaceItemSize());
                        }

                        s_faceId = INVALID_FACE_ID;
                    }
                }
            }
        }
        break;

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
    .name = "HomePanel",
    .ops  = (vision_algo_dev_operator_t *)&s_VisionAlgoDev_OasisHomePanelOps,
    .cap  = {.param = NULL},
};

int HAL_VisionAlgoDev_OasisHomePanel_Register(void)
{
    int error = 0;
    LOGD("HAL_VisionAlgoDev_OasisHomePanel_Register");
    error = FWK_VisionAlgoManager_DeviceRegister(&s_VisionAlgoDev_OasisHomePanel);
    return error;
}
#endif /* ENABLE_VISIONALGO_DEV_OasisHomePanel */
