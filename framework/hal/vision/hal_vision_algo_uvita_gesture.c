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
 * @brief SLN UVITA Gesture recognition Algorithm
 */

#include "board_define.h"
#ifdef ENABLE_VISIONALGO_DEV_Uvita_Gesture
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

#include "uvita_gesture.h"

/* configurable parameters */
#ifndef UVITA_GESTUREREC_ALGOTYPE
#define UVITA_GESTUREREC_ALGOTYPE kVisionAlgoID_UvitaGesture
#endif /* UVITA_GESTUREREC_ALGOTYPE */

#ifndef UVITA_GESTUREREC_AUTO_START
#define UVITA_GESTUREREC_AUTO_START 0
#endif /* UVITA_GESTUREREC_AUTO_START */

#ifndef UVITA_GESTUREREC_RESULT_EVENT_INFO
#define UVITA_GESTUREREC_RESULT_EVENT_INFO kEventInfo_Remote
#endif /* UVITA_GESTUREREC_RESULT_EVENT_INFO */

#ifndef UVITA_GESTUREREC_REQUESTFRAME_EVENT_INFO
#define UVITA_GESTUREREC_REQUESTFRAME_EVENT_INFO kEventInfo_Remote
#endif /* UVITA_GESTUREREC_REQUESTFRAME_EVENT_INFO */

#ifndef UVITA_GESTUREREC_DEBUG_OPTION
#define UVITA_GESTUREREC_DEBUG_OPTION false
#endif /* UVITA_GESTUREREC_DEBUG_OPTION */

/*******************************************************************************
 * Defines
 ******************************************************************************/

#define UVITA_GESTURE_THRESHOLD 0.5
typedef struct _uvita_gesture_param
{
    vision_algo_dev_t *dev;
    uvita_image_frame frame;
    uvita_state_t state;
    vision_algo_result_t result;
} uvita_gesture_param_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uvita_gesture_param_t s_UvitaGestureRecognition;
static uint32_t s_blockingList;

/*
 * Gesture type name string according to the gesture_type definition
 */
static char *s_GestureTypeName[] = {
    "Ok", "Fist", "One", "Victory", "Three", "Four", "Palm", "Thum up", "Pinky up", "Rock roll", "Call me", "None",
};

AT_FB_SHMEM_SECTION_ALIGN(uint8_t s_UvitaRgbFrameBuffer[UVITA_GESTURE_RGB_FRAME_HEIGHT * UVITA_GESTURE_RGB_FRAME_WIDTH *
                                                        UVITA_GESTURE_RGB_FRAME_BPP],
                          64);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static inline void _uvita_dev_response(event_base_t eventBase,
                                       void *response,
                                       event_status_t status,
                                       unsigned char isFinished)
{
}

static void _uvita_dev_requestFrame(const vision_algo_dev_t *dev)
{
    /* Build Valgo event */
    valgo_event_t valgo_event = {.eventId   = kVAlgoEvent_RequestFrame,
                                 .eventInfo = UVITA_GESTUREREC_REQUESTFRAME_EVENT_INFO,
                                 .data      = NULL,
                                 .size      = 0,
                                 .copy      = 0};

    if ((dev != NULL) && (dev->cap.callback != NULL))
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev->id, valgo_event, fromISR);
    }
}

static void _uvita_dev_notifyResult(const vision_algo_dev_t *dev, vision_algo_result_t *result)
{
    static vision_algo_result_t oldResults = {0};
    /* Build Valgo event */
    valgo_event_t valgo_event = {.eventId   = kVAlgoEvent_VisionResultUpdate,
                                 .eventInfo = UVITA_GESTUREREC_RESULT_EVENT_INFO,
                                 .data      = result,
                                 .size      = sizeof(vision_algo_result_t),
                                 .copy      = 1};

    if ((dev != NULL) && (result != NULL) && (dev->cap.callback != NULL))
    {
        if (result->uvitaHandGesture.hand.conf > UVITA_GESTURE_THRESHOLD)
        {
            /* Advertise only if we are sure */
            if (memcmp(result, &oldResults, sizeof(vision_algo_result_t)))
            {
                uint8_t decimal[3];
                uint16_t conf = result->uvitaHandGesture.hand.conf * 1000;
                uint16_t pow  = 1;
                for (int i = 0; i < 3; i++)
                {
                    decimal[i] = conf / pow % 10;
                    pow        = pow * 10;
                }

                LOGD("[Uvita] Gesture:[%d]:[%s] conf:[0.%d%d%d]", result->uvitaHandGesture.hand.gtype,
                     s_GestureTypeName[result->uvitaHandGesture.hand.gtype], decimal[2], decimal[1], decimal[0]);

                uint8_t fromISR = __get_IPSR();
                dev->cap.callback(dev->id, valgo_event, fromISR);
                memcpy(&oldResults, result, sizeof(vision_algo_result_t));
            }
        }
    }
}

static hal_valgo_status_t HAL_VisionAlgoDev_UvitaGestureRecognition_Init(vision_algo_dev_t *dev,
                                                                         valgo_dev_callback_t callback,
                                                                         void *param)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    LOGI("++HAL_VisionAlgoDev_UvitaGestureRecognition_Init");
    uvita_status uvitaRet = UVITA_SUCCESS;
    uvita_param init_para = {0};

    s_UvitaGestureRecognition.dev = dev;

    // init the device
    memset(&dev->cap, 0, sizeof(dev->cap));
    dev->cap.callback = callback;

    dev->data.autoStart                              = UVITA_GESTUREREC_AUTO_START;
    dev->data.frames[kVAlgoFrameID_RGB].height       = UVITA_GESTURE_RGB_FRAME_HEIGHT;
    dev->data.frames[kVAlgoFrameID_RGB].width        = UVITA_GESTURE_RGB_FRAME_WIDTH;
    dev->data.frames[kVAlgoFrameID_RGB].pitch        = UVITA_GESTURE_RGB_FRAME_WIDTH * UVITA_GESTURE_RGB_FRAME_BPP;
    dev->data.frames[kVAlgoFrameID_RGB].is_supported = 1;
    dev->data.frames[kVAlgoFrameID_RGB].rotate       = kCWRotateDegree_0;
    dev->data.frames[kVAlgoFrameID_RGB].flip         = kFlipMode_None;
    dev->data.frames[kVAlgoFrameID_RGB].format       = kPixelFormat_BGR;
    dev->data.frames[kVAlgoFrameID_RGB].srcFormat    = UVITA_GESTURE_RGB_FRAME_SRC_FORMAT;
    dev->data.frames[kVAlgoFrameID_RGB].data         = s_UvitaRgbFrameBuffer;

    // init the RGB frame
    s_UvitaGestureRecognition.frame.height = UVITA_GESTURE_RGB_FRAME_HEIGHT;
    s_UvitaGestureRecognition.frame.width  = UVITA_GESTURE_RGB_FRAME_WIDTH;
    s_UvitaGestureRecognition.frame.fmt    = UVITA_IMG_INT8_BGR888;
    s_UvitaGestureRecognition.frame.data   = dev->data.frames[kVAlgoFrameID_RGB].data;

    init_para.mem_size = uvita_gesture_get_buf_size(UVITA_GESTURE_RGB_FRAME_HEIGHT, UVITA_GESTURE_RGB_FRAME_WIDTH);

    LOGD("uvita requires memory size: %d\r\n", init_para.mem_size);

    char *mempool = (char *)pvPortMalloc(init_para.mem_size);

    if (mempool == NULL)
    {
        LOGE("uvita malloc error. Need %d free %d.\r\n", init_para.mem_size, xPortGetFreeHeapSize());
        while (1)
            ;
    }

    init_para.mem_pool      = mempool;
    init_para.fast_mem_size = DTC_OPTIMIZE_BUFFER_SIZE;
    init_para.fast_mem_pool = (char *)g_DTCOPBuf;

    /* uvita initialization */
    uvitaRet = uvita_gesture_init(&init_para);

    if (uvitaRet != UVITA_SUCCESS)
    {
        LOGE("uvita initialization error: %d\r\n", ret);
        while (1)
            ;
    }

    s_UvitaGestureRecognition.state     = kUvitaState_Stopped;
    s_UvitaGestureRecognition.result.id = UVITA_GESTUREREC_ALGOTYPE;

    LOGD("[UVITA_GESTURE]:Init ok");
    LOGI("--HAL_VisionAlgoDev_UvitaGestureRecognition_Init");
    return ret;
}

// deinitialize the dev
static hal_valgo_status_t HAL_VisionAlgoDev_UvitaGestureRecognition_Deinit(vision_algo_dev_t *dev)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    LOGI("++HAL_VisionAlgoDev_UvitaGestureRecognition_Deinit");

    LOGI("--HAL_VisionAlgoDev_UvitaGestureRecognition_Deinit");
    return ret;
}

// start the dev
static hal_valgo_status_t HAL_VisionAlgoDev_UvitaGestureRecognition_Run(const vision_algo_dev_t *dev, void *data)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    LOGV("++HAL_VisionAlgoDev_UvitaGestureRecognition_Run: %d", s_UvitaGestureRecognition.state);

    if (s_UvitaGestureRecognition.state == kUvitaState_Running)
    {
        /* Let this print for test benchmark */
        LOGI("START UVITA GESTURE");
        uvita_hand_out *handOut = &s_UvitaGestureRecognition.result.uvitaHandGesture.hand;
        uvita_gesture_run(&s_UvitaGestureRecognition.frame, &s_UvitaGestureRecognition.frame, handOut);
        _uvita_dev_notifyResult(dev, &s_UvitaGestureRecognition.result);
        vTaskDelay(100);
    }
    else if (kUvitaState_Stopped == s_UvitaGestureRecognition.state)
    {
        ret = kStatus_HAL_ValgoStop;
    }

    LOGI("--HAL_VisionAlgoDev_UvitaGestureRecognition_Run");
    return ret;
}

static hal_valgo_status_t HAL_VisionAlgoDev_UvitaGestureRecognition_InputNotify(const vision_algo_dev_t *receiver,
                                                                                void *data)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    LOGD("++HAL_VisionAlgoDev_UvitaGestureRecognition_InputNotify");

    event_base_t eventBase = *(event_base_t *)data;
    switch (eventBase.eventId)
    {
        case kEventFaceRecID_UvitaGestureSetState:
        {
            event_face_rec_t *pEvent = (event_face_rec_t *)data;
            uvita_state_t newState   = pEvent->uvitaState.state;
            if (newState != s_UvitaGestureRecognition.state)
            {
                s_UvitaGestureRecognition.state = newState;
                if (newState == kUvitaState_Running)
                {
                    _uvita_dev_requestFrame(receiver);
                }
                LOGD("UvitaGestureRec:[NewState]:%d", s_UvitaGestureRecognition.state);
            }
        }
        break;
        case kEventFaceRecID_UvitaGestureGetState:
        {
            LOGD("Uvita Gesture state:%d", s_UvitaGestureRecognition.state);
        }
        break;
        default:
            break;
    }

    LOGI("--HAL_VisionAlgoDev_UvitaGestureRecognition_InputNotify");
    return ret;
}

const static vision_algo_dev_operator_t s_VisionAlgoDev_UvitaGestureRecognitionOps = {
    .init        = HAL_VisionAlgoDev_UvitaGestureRecognition_Init,
    .deinit      = HAL_VisionAlgoDev_UvitaGestureRecognition_Deinit,
    .run         = HAL_VisionAlgoDev_UvitaGestureRecognition_Run,
    .inputNotify = HAL_VisionAlgoDev_UvitaGestureRecognition_InputNotify,
};

static vision_algo_dev_t s_VisionAlgoDev_UvitaGestureRecognition = {
    .id   = 0,
    .name = "GestureRec",
    .ops  = (vision_algo_dev_operator_t *)&s_VisionAlgoDev_UvitaGestureRecognitionOps,
    .cap  = {.param = NULL},
};

// for vision_algo_uvita_gesture device, please link uvita_gesture/libuvita_gesture_cm7.a
int HAL_VisionAlgoDev_UvitaGestureRecognition_Register()
{
    int error = 0;
    LOGD("HAL_VisionAlgoDev_UvitaGestureRecognition_Register");
    error = FWK_VisionAlgoManager_DeviceRegister(&s_VisionAlgoDev_UvitaGestureRecognition);
    memset(&s_UvitaGestureRecognition, 0, sizeof(s_UvitaGestureRecognition));

    return error;
}
#endif /* ENABLE_VISIONALGO_DEV_Uvita_Gesture */
