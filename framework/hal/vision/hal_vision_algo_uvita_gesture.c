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

#include "fsl_cache.h"
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

/*******************************************************************************
 * Defines
 ******************************************************************************/

#if VISION_SHARED_FRAME_BUFFER
#if (((UVITA_GESTURE_RGB_FRAME_WIDTH * UVITA_GESTURE_RGB_FRAME_HEIGHT * UVITA_GESTURE_RGB_FRAME_BPP) +        \
      (UVITA_GESTURE_RGB_DETFRAME_WIDTH * UVITA_GESTURE_RGB_DETFRAME_HEIGHT * UVITA_GESTURE_RGB_FRAME_BPP)) > \
     VISION_ALGO_BUFFER_SIZE)
#error "Not enough memory for uvita gesture"
#endif
#endif /* VISION_SHARED_FRAME_BUFFER */
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
    uvita_image_frame detFrame;
    uvita_state_t state;
    vision_algo_result_t result;
} uvita_gesture_param_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uvita_gesture_param_t s_UvitaGestureRecognition;

/*
 * Gesture type name string according to the gesture_type definition
 */
static char *s_GestureTypeName[] = {
    "Ok",       "Fist",     "One",       "Victory", "Three",    "Four", "Palm",
    "Thumb up", "Pinky up", "Rock roll", "Call me", "Previous", "Next", "None",
};
static char *s_HandName[] = {"Left", "Right", "None"};

#if VISION_SHARED_FRAME_BUFFER
static uint8_t *s_UvitaRgbFrameBuffer = g_VisionAlgoBuffer;
static uint8_t *s_UvitaRgbDetBuffer =
    g_VisionAlgoBuffer + (UVITA_GESTURE_RGB_FRAME_WIDTH * UVITA_GESTURE_RGB_FRAME_HEIGHT * UVITA_GESTURE_RGB_FRAME_BPP);
#else
SDK_ALIGN(uint8_t s_UvitaRgbFrameBuffer[UVITA_GESTURE_RGB_FRAME_HEIGHT * UVITA_GESTURE_RGB_FRAME_WIDTH *
                                        UVITA_GESTURE_RGB_FRAME_BPP],
          64);
SDK_ALIGN(uint8_t s_UvitaRgbDetBuffer[UVITA_GESTURE_RGB_DETFRAME_HEIGHT * UVITA_GESTURE_RGB_DETFRAME_WIDTH *
                                      UVITA_GESTURE_RGB_FRAME_BPP],
          64);

#endif /* VISION_SHARED_FRAME_BUFFER */

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
    /* Build Valgo event */
    valgo_event_t valgo_event = {.eventId   = kVAlgoEvent_VisionResultUpdate,
                                 .eventInfo = UVITA_GESTUREREC_RESULT_EVENT_INFO,
                                 .data      = result,
                                 .size      = sizeof(vision_algo_result_t),
                                 .copy      = 1};

    if ((dev != NULL) && (result != NULL) && (dev->cap.callback != NULL))
    {
        static bool oldHasHand = false;
        uint8_t advertise      = false;

        if (result->uvitaHandGesture.hand.has_hand)
        {
            /* Advertise only if we are sure */

            uint8_t decimal[3];
            uint16_t conf = result->uvitaHandGesture.hand.conf * 1000;
            uint16_t pow  = 1;
            for (int i = 0; i < 3; i++)
            {
                decimal[i] = conf / pow % 10;
                pow        = pow * 10;
            }
            LOGI("[Uvita] Hand detected. conf:[0.%d%d%d]", decimal[2], decimal[1], decimal[0]);

            if (result->uvitaHandGesture.hand.conf > UVITA_GESTURE_THRESHOLD)
            {
                LOGD("[Uvita] Gesture:[%s][%d]:[%s] conf:[0.%d%d%d]",
                     s_HandName[result->uvitaHandGesture.hand.left_right], result->uvitaHandGesture.hand.gtype,
                     s_GestureTypeName[result->uvitaHandGesture.hand.gtype], decimal[2], decimal[1], decimal[0]);

                advertise = true;
            }
        }
        else
        {
            if (oldHasHand)
            {
                /* this is the first NoHand detected advertise */
                LOGD("[Uvita] No hand detected");
                result->uvitaHandGesture.hand.left_right = NONE_HAND;
                result->uvitaHandGesture.hand.gtype      = GESTURE_NONE;
                advertise                                = true;
            }
        }

        if ((advertise == true) && (dev->cap.callback != NULL))
        {
            uint8_t fromISR = __get_IPSR();
            dev->cap.callback(dev->id, valgo_event, fromISR);
        }

        oldHasHand = result->uvitaHandGesture.hand.has_hand;
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
    dev->data.frames[kVAlgoFrameID_RGB].flip         = kFlipMode_Horizontal;
    dev->data.frames[kVAlgoFrameID_RGB].format       = kPixelFormat_BGR;
    dev->data.frames[kVAlgoFrameID_RGB].srcFormat    = UVITA_GESTURE_RGB_FRAME_SRC_FORMAT;
    dev->data.frames[kVAlgoFrameID_RGB].data         = s_UvitaRgbFrameBuffer;

    dev->data.frames[kVAlgoFrameID_IR].height       = UVITA_GESTURE_RGB_DETFRAME_HEIGHT;
    dev->data.frames[kVAlgoFrameID_IR].width        = UVITA_GESTURE_RGB_DETFRAME_WIDTH;
    dev->data.frames[kVAlgoFrameID_IR].pitch        = UVITA_GESTURE_RGB_DETFRAME_WIDTH * UVITA_GESTURE_RGB_FRAME_BPP;
    dev->data.frames[kVAlgoFrameID_IR].is_supported = 1;
    dev->data.frames[kVAlgoFrameID_IR].rotate       = kCWRotateDegree_0;
    dev->data.frames[kVAlgoFrameID_IR].flip         = kFlipMode_Horizontal;
    dev->data.frames[kVAlgoFrameID_IR].format       = kPixelFormat_BGR;
    dev->data.frames[kVAlgoFrameID_IR].srcFormat    = UVITA_GESTURE_RGB_FRAME_SRC_FORMAT;
    dev->data.frames[kVAlgoFrameID_IR].data         = s_UvitaRgbDetBuffer;

    // init the RGB frame
    s_UvitaGestureRecognition.frame.height = UVITA_GESTURE_RGB_FRAME_HEIGHT;
    s_UvitaGestureRecognition.frame.width  = UVITA_GESTURE_RGB_FRAME_WIDTH;
    s_UvitaGestureRecognition.frame.fmt    = UVITA_IMG_INT8_BGR888;
    s_UvitaGestureRecognition.frame.data   = s_UvitaRgbFrameBuffer;

    // init the RGB det frame

    s_UvitaGestureRecognition.detFrame.height = UVITA_GESTURE_RGB_DETFRAME_HEIGHT;
    s_UvitaGestureRecognition.detFrame.width  = UVITA_GESTURE_RGB_DETFRAME_WIDTH;
    s_UvitaGestureRecognition.detFrame.fmt    = UVITA_IMG_INT8_BGR888;
    s_UvitaGestureRecognition.detFrame.data   = s_UvitaRgbDetBuffer;

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

        uvita_gesture_out *handOut = &s_UvitaGestureRecognition.result.uvitaHandGesture.hand;
        uvita_gesture_run(&s_UvitaGestureRecognition.frame, &s_UvitaGestureRecognition.detFrame, handOut);
        _uvita_dev_notifyResult(dev, &s_UvitaGestureRecognition.result);
        vTaskDelay(pdMS_TO_TICKS(100));
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
