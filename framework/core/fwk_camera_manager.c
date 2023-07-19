/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief camera manager framework implementation.
 */

#include "fwk_platform.h"
#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_task.h"
#include "fwk_perf.h"
#include "fwk_graphics.h"
#include "fwk_camera_manager.h"

typedef struct
{
    fwk_task_data_t commonData;
    /* registered camera devices */
    camera_dev_t *devs[MAXIMUM_CAMERA_DEV];
    /* camera dequeue messages */
    fwk_message_t cameraDequeueMsg[MAXIMUM_CAMERA_DEV];
#if FWK_SUPPORT_MULTICORE
    /* Used for remote response and deque */
    fwk_message_t cameraRemoteDequeueMsg;
#endif /* FWK_SUPPORT_MULTICORE */
    /* display frame response messages */
    fwk_message_t displayResponseMsg[MAXIMUM_DISPLAY_DEV];
    /* vision algorithm frame response messages */
    fwk_message_t vAlgoResponseMsg[MAXIMUM_VISION_ALGO_DEV * kVAlgoFrameID_Count];
    /* display request frame information */
    msg_payload_t displayRequestFrameInfo[MAXIMUM_DISPLAY_DEV];
    /* vision algorithm request frame information */
    msg_payload_t vAlgoRequestFrameInfo[MAXIMUM_VISION_ALGO_DEV * kVAlgoFrameID_Count];
    /* the overlay surface */
    gfx_surface_t *pOverlaySurface;
} camera_task_data_t;

typedef struct
{
    fwk_task_t task;
    camera_task_data_t cameraData;
} camera_task_t;

/*
 * camera manager task
 */
static camera_task_t s_CameraTask;

#if FWK_SUPPORT_STATIC_ALLOCATION
FWKDATA static StackType_t s_CameraTaskStack[CAMERA_MANAGER_TASK_STACK];
FWKDATA static StaticTask_t s_CameraTaskTCB;
static void *s_CameraTaskTCBReference = (void *)&s_CameraTaskTCB;
#else
static void *s_CameraTaskStack        = NULL;
static void *s_CameraTaskTCBReference = NULL;
#endif

/*
 * camera dev callback
 */
static int _FWK_CameraManager_DeviceCallback(const camera_dev_t *dev,
                                             camera_event_t event,
                                             void *param,
                                             uint8_t fromISR)
{
    switch (event.eventId)
    {
        case kCameraEvent_SendFrame:
        {
            fwk_message_t *pMsg = (fwk_message_t *)param;
            pMsg->id            = kFWKMessageID_CameraDequeue;
            pMsg->msgInfo       = event.eventInfo;
#if FWK_SUPPORT_MULTICORE
            pMsg->multicore.isMulticoreMessage  = 0;
            pMsg->multicore.wasMulticoreMessage = 0;
#endif
            if (fromISR)
            {
                FWK_Message_PutFromIsr(kFWKTaskID_Camera, &pMsg);
            }
            else
            {
                FWK_Message_Put(kFWKTaskID_Camera, &pMsg);
            }
        }
        break;
        case kCameraEvent_CameraDeviceInit:
        {
            hal_camera_status_t init_status = *(hal_camera_status_t *)param;
            if (init_status != kStatus_HAL_CameraSuccess)
            {
                LOGE("INIT camera %s failed", dev->name);
                break;
            }

            for (int i = 0; i < MAXIMUM_CAMERA_DEV; i++)
            {
                camera_dev_t *pDev = s_CameraTask.cameraData.devs[i];
                if ((pDev != NULL) && (pDev->id == dev->id))
                {
                    pDev->state = kState_HAL_Initialized;
                    break;
                }
            }
        }
        break;
        default:
            LOGE("Invalid event received");
            break;
    }

    return 0;
}

static int _FWK_CameraManager_TaskInit(fwk_task_data_t *pTaskData)
{
    if (pTaskData == NULL)
        return -1;

    int error                           = 0;
    camera_task_data_t *pCameraTaskData = (camera_task_data_t *)pTaskData;

    /* init the camera dev */
    for (int i = 0; i < MAXIMUM_CAMERA_DEV; i++)
    {
        camera_dev_t *pDev = pCameraTaskData->devs[i];
        if (pDev != NULL && pDev->ops->init != NULL)
        {
            LOGD("INIT camera dev[%d]", i);
            error = pDev->ops->init(pDev, pDev->config.width, pDev->config.height, _FWK_CameraManager_DeviceCallback,
                                    (void *)&pCameraTaskData->cameraDequeueMsg[i]);
            if (error == kStatus_HAL_CameraSuccess)
            {
                pDev->state = kState_HAL_Initialized;
            }
            else if (error != kStatus_HAL_CameraNonBlocking)
            {
                LOGE("INIT camera dev %d error %d", i, error);
                return error;
            }
        }
    }

    for (int i = 0; i < MAXIMUM_CAMERA_DEV; i++)
    {
        camera_dev_t *pDev = pCameraTaskData->devs[i];

        if (pDev != NULL && pDev->ops->start != NULL)
        {
            while (pDev->state != kState_HAL_Initialized)
            {
                /* Wait for the camera to be initialized before starting it*/
                vTaskDelay(1);
            }

            LOGD("START camera dev[%d]", i);
            error = pDev->ops->start(pDev);

            if (error == kStatus_HAL_CameraSuccess)
            {
                pDev->state = kState_HAL_Started;
            }
            else if (error)
            {
                LOGE("START camera dev %d error %d", i, error);
                return error;
            }
        }
    }

    return error;
}

static void _FWK_CameraManager_DisplayResponse(camera_dev_t *pDev,
                                               fwk_message_t *pMsg,
                                               camera_task_data_t *pCameraTaskData)
{
    if ((pMsg == NULL) || (pCameraTaskData == NULL))
        return;

    for (int i = 0; i < MAXIMUM_DISPLAY_DEV; i++)
    {
        if ((pCameraTaskData->displayRequestFrameInfo[i].data != NULL) &&
            (pMsg->payload.frame.format == pCameraTaskData->displayRequestFrameInfo[i].frame.srcFormat))
        {
            if (pDev != NULL && pDev->ops->postProcess != NULL)
            {
                pDev->ops->postProcess(pDev, &(pMsg->payload.data), &(pMsg->payload.frame.format));
            }

            /* send the display response message to display */
            gfx_surface_t cameraSurface;
            gfx_surface_t displaySurface;
            gfx_rotate_config_t cameraToDisplayRotate;
            flip_mode_t cameraFlip;
            gfx_rotate_config_t *pRotate = NULL;
            cw_rotate_degree_t srcRotate = pMsg->payload.frame.rotate;
            cw_rotate_degree_t dstRotate = pCameraTaskData->displayRequestFrameInfo[i].frame.rotate;

            if ((srcRotate == kCWRotateDegree_90) && (dstRotate == kCWRotateDegree_270))
            {
                srcRotate = kCWRotateDegree_0;
                dstRotate = kCWRotateDegree_0;
            }

            /* source surface */
            cameraSurface.pitch    = pMsg->payload.frame.pitch;
            cameraSurface.format   = pMsg->payload.frame.format;
            cameraSurface.swapByte = pMsg->payload.frame.swapByte;
            cameraSurface.lock     = NULL;

            if ((srcRotate == kCWRotateDegree_0) || (srcRotate == kCWRotateDegree_180))
            {
                cameraSurface.height = pMsg->payload.frame.height;
                cameraSurface.width  = pMsg->payload.frame.width;
                cameraSurface.left   = pMsg->payload.frame.left;
                cameraSurface.top    = pMsg->payload.frame.top;
                cameraSurface.right  = pMsg->payload.frame.right;
                cameraSurface.bottom = pMsg->payload.frame.bottom;
            }
            else if ((srcRotate == kCWRotateDegree_90) || (srcRotate == kCWRotateDegree_270))
            {
                cameraSurface.height = pMsg->payload.frame.width;
                cameraSurface.width  = pMsg->payload.frame.height;
                cameraSurface.left   = pMsg->payload.frame.top;
                cameraSurface.top    = pMsg->payload.frame.left;
                cameraSurface.right  = pMsg->payload.frame.bottom;
                cameraSurface.bottom = pMsg->payload.frame.right;
            }

            if ((srcRotate != kCWRotateDegree_0))
            {
                cameraToDisplayRotate.target = kGFXRotate_SRCSurface;
                cameraToDisplayRotate.degree = srcRotate;
                pRotate                      = &cameraToDisplayRotate;
            }

            /* flip */
            cameraFlip        = pMsg->payload.frame.flip;
            cameraSurface.buf = pMsg->payload.data;

            /* dst surface */
            displaySurface.height = pCameraTaskData->displayRequestFrameInfo[i].frame.height;
            displaySurface.width  = pCameraTaskData->displayRequestFrameInfo[i].frame.width;
            displaySurface.left   = pCameraTaskData->displayRequestFrameInfo[i].frame.left;
            displaySurface.top    = pCameraTaskData->displayRequestFrameInfo[i].frame.top;
            displaySurface.right  = pCameraTaskData->displayRequestFrameInfo[i].frame.right;
            displaySurface.bottom = pCameraTaskData->displayRequestFrameInfo[i].frame.bottom;

            /* rotate */
            if (dstRotate != kCWRotateDegree_0)
            {
                if (srcRotate != kCWRotateDegree_0)
                {
                    LOGE("Cannot rotate both source and output");
                    return;
                }

                cameraToDisplayRotate.target = kGFXRotate_DSTSurface;
                cameraToDisplayRotate.degree = dstRotate;
                pRotate                      = &cameraToDisplayRotate;

                if (dstRotate == kCWRotateDegree_90 || dstRotate == kCWRotateDegree_270)
                {
                    displaySurface.left   = pCameraTaskData->displayRequestFrameInfo[i].frame.top;
                    displaySurface.top    = pCameraTaskData->displayRequestFrameInfo[i].frame.left;
                    displaySurface.right  = pCameraTaskData->displayRequestFrameInfo[i].frame.bottom;
                    displaySurface.bottom = pCameraTaskData->displayRequestFrameInfo[i].frame.right;
                    displaySurface.height = pCameraTaskData->displayRequestFrameInfo[i].frame.width;
                    displaySurface.width  = pCameraTaskData->displayRequestFrameInfo[i].frame.height;
                }
            }

            displaySurface.pitch  = pCameraTaskData->displayRequestFrameInfo[i].frame.pitch;
            displaySurface.format = pCameraTaskData->displayRequestFrameInfo[i].frame.format;
            displaySurface.buf    = pCameraTaskData->displayRequestFrameInfo[i].data;
            displaySurface.lock   = NULL;

            if (pCameraTaskData->pOverlaySurface == NULL)
            {
                gfx_blit(&cameraSurface, &displaySurface, pRotate, cameraFlip);
            }
            else
            {
                gfx_compose(&cameraSurface, pCameraTaskData->pOverlaySurface, &displaySurface, pRotate, cameraFlip);
            }

            fwk_message_t *pDisplayResMsg = &pCameraTaskData->displayResponseMsg[i];
            pDisplayResMsg->payload.data  = pCameraTaskData->displayRequestFrameInfo[i].data;
            pDisplayResMsg->payload.devId = pCameraTaskData->displayRequestFrameInfo[i].devId;
            LOGI("Sending camera frame to display id #%d", pCameraTaskData->displayRequestFrameInfo[i].devId);
            FWK_Message_Put(kFWKTaskID_Display, &pDisplayResMsg);

            /* indicate the request display buffer is filled */
            pCameraTaskData->displayRequestFrameInfo[i].data = NULL;
        }
    }
}

static void _FWK_CameraManager_VisionAlgoResponse(camera_dev_t *pDev,
                                                  fwk_message_t *pMsg,
                                                  camera_task_data_t *pCameraTaskData)
{
    if ((pMsg == NULL) || (pCameraTaskData == NULL))
        return;

    /* response the vision algorithm requests */
    for (int i = 0; i < MAXIMUM_VISION_ALGO_DEV * kVAlgoFrameID_Count; i++)
    {
        if ((pCameraTaskData->vAlgoRequestFrameInfo[i].data != NULL) &&
            (pMsg->payload.frame.format == pCameraTaskData->vAlgoRequestFrameInfo[i].frame.srcFormat))
        {
            if (pDev != NULL && pDev->ops->postProcess != NULL)
            {
                pDev->ops->postProcess(pDev, &(pMsg->payload.data), &(pMsg->payload.frame.format));
            }

            gfx_surface_t cameraSurface;
            gfx_rotate_config_t cameraToValgoRotate;
            gfx_rotate_config_t *pRotate = NULL;

            cw_rotate_degree_t srcRotate = pMsg->payload.frame.rotate;
            cw_rotate_degree_t dstRotate = pCameraTaskData->vAlgoRequestFrameInfo[i].frame.rotate;

            if ((srcRotate == kCWRotateDegree_90) && (dstRotate == kCWRotateDegree_270))
            {
                srcRotate = kCWRotateDegree_0;
                dstRotate = kCWRotateDegree_0;
            }

            /* source surface */
            cameraSurface.pitch    = pMsg->payload.frame.pitch;
            cameraSurface.format   = pMsg->payload.frame.format;
            cameraSurface.swapByte = pMsg->payload.frame.swapByte;
            cameraSurface.lock     = NULL;

            if ((srcRotate == kCWRotateDegree_0) || (srcRotate == kCWRotateDegree_180))
            {
                cameraSurface.height = pMsg->payload.frame.height;
                cameraSurface.width  = pMsg->payload.frame.width;
                cameraSurface.left   = pMsg->payload.frame.left;
                cameraSurface.top    = pMsg->payload.frame.top;
                cameraSurface.right  = pMsg->payload.frame.right;
                cameraSurface.bottom = pMsg->payload.frame.bottom;
            }
            else if ((srcRotate == kCWRotateDegree_90) || (srcRotate == kCWRotateDegree_270))
            {
                cameraSurface.height = pMsg->payload.frame.width;
                cameraSurface.width  = pMsg->payload.frame.height;
                cameraSurface.left   = pMsg->payload.frame.top;
                cameraSurface.top    = pMsg->payload.frame.left;
                cameraSurface.right  = pMsg->payload.frame.bottom;
                cameraSurface.bottom = pMsg->payload.frame.right;
            }

            if (srcRotate != kCWRotateDegree_0)
            {
                cameraToValgoRotate.target = kGFXRotate_SRCSurface;
                cameraToValgoRotate.degree = pMsg->payload.frame.rotate;
                pRotate                    = &cameraToValgoRotate;
            }

            /* flip_mode_t cameraFlip = pMsg->frame.flip; */
            cameraSurface.buf = pMsg->payload.data;

            gfx_surface_t algorithmSurface;

            algorithmSurface.height = pCameraTaskData->vAlgoRequestFrameInfo[i].frame.height;
            algorithmSurface.width  = pCameraTaskData->vAlgoRequestFrameInfo[i].frame.width;
            algorithmSurface.left   = pCameraTaskData->vAlgoRequestFrameInfo[i].frame.left;
            algorithmSurface.top    = pCameraTaskData->vAlgoRequestFrameInfo[i].frame.top;
            algorithmSurface.right  = pCameraTaskData->vAlgoRequestFrameInfo[i].frame.right;
            algorithmSurface.bottom = pCameraTaskData->vAlgoRequestFrameInfo[i].frame.bottom;

            /* rotate */
            if (dstRotate != kCWRotateDegree_0)
            {
                if (srcRotate != kCWRotateDegree_0)
                {
                    LOGE("Cannot rotate both source and output");
                    return;
                }

                cameraToValgoRotate.target = kGFXRotate_DSTSurface;
                cameraToValgoRotate.degree = pCameraTaskData->vAlgoRequestFrameInfo[i].frame.rotate;
                pRotate                    = &cameraToValgoRotate;
            }

            algorithmSurface.pitch  = pCameraTaskData->vAlgoRequestFrameInfo[i].frame.pitch;
            algorithmSurface.format = pCameraTaskData->vAlgoRequestFrameInfo[i].frame.format;
            algorithmSurface.buf    = pCameraTaskData->vAlgoRequestFrameInfo[i].data;
            algorithmSurface.lock   = NULL;

            gfx_blit(&cameraSurface, &algorithmSurface, pRotate, kFlipMode_None);

            fwk_message_t *pVAlgoResMsg = &pCameraTaskData->vAlgoResponseMsg[i];
            pVAlgoResMsg->payload.data  = pCameraTaskData->vAlgoRequestFrameInfo[i].data;
            pVAlgoResMsg->payload.devId = pCameraTaskData->vAlgoRequestFrameInfo[i].devId;

#if FWK_SUPPORT_MULTICORE
            pVAlgoResMsg->multicore.isMulticoreMessage = 1;
            pVAlgoResMsg->multicore.taskId             = kFWKTaskID_VisionAlgo;
#endif /* FWK_SUPPORT_MULTICORE */

            FWK_Message_Put(kFWKTaskID_VisionAlgo, &pVAlgoResMsg);

            /* indicate the request algorithm buffer is filled */
            pCameraTaskData->vAlgoRequestFrameInfo[i].data = NULL;
        }
    }
}

static void _FWK_CameraManager_MessageHandle(fwk_message_t *pMsg, fwk_task_data_t *pTaskData)
{
    int error = 0;
    if ((pMsg == NULL) || (pTaskData == NULL))
        return;

    camera_task_data_t *pCameraTaskData = (camera_task_data_t *)pTaskData;

    LOGI("[CameraManager]:Received message:[%s]", FWK_Message_Name(pMsg->id));

    switch (pMsg->id)
    {
        case kFWKMessageID_DisplayRequestFrame:
        {
            int displayDevId = pMsg->payload.devId;
            LOGI("Request frame dev = %d", displayDevId);

            if (pCameraTaskData->displayRequestFrameInfo[displayDevId].data == NULL)
            {
                memcpy((void *)&pCameraTaskData->displayRequestFrameInfo[displayDevId], (void *)&pMsg->payload,
                       sizeof(msg_payload_t));
            }
        }
        break;

        case kFWKMessageID_VAlgoRequestFrame:
        {
            int valgoReqId = pMsg->payload.devId;

            if (pCameraTaskData->vAlgoRequestFrameInfo[valgoReqId].data == NULL)
            {
                memcpy((void *)&pCameraTaskData->vAlgoRequestFrameInfo[valgoReqId], (void *)&pMsg->payload,
                       sizeof(msg_payload_t));
            }
        }
        break;

        case kFWKMessageID_DispatcherRequestShowOverlay:
        {
            pCameraTaskData->pOverlaySurface = pMsg->payload.overlay.pSurface;
        }
        break;

        case kFWKMessageID_CameraDequeue:
        {
            int devId          = pMsg->payload.devId;
            camera_dev_t *pDev = pCameraTaskData->devs[devId];

            /* dequeue the frame buffer */
            if (pDev != NULL)
            {
                pDev->ops->dequeue(pDev, &(pMsg->payload.data), &(pMsg->payload.frame.format));
                pMsg->payload.devId          = pDev->id;
                pMsg->payload.frame.height   = pDev->config.height;
                pMsg->payload.frame.width    = pDev->config.width;
                pMsg->payload.frame.pitch    = pDev->config.pitch;
                pMsg->payload.frame.left     = pDev->config.left;
                pMsg->payload.frame.top      = pDev->config.top;
                pMsg->payload.frame.right    = pDev->config.right;
                pMsg->payload.frame.bottom   = pDev->config.bottom;
                pMsg->payload.frame.rotate   = pDev->config.rotate;
                pMsg->payload.frame.flip     = pDev->config.flip;
                pMsg->payload.frame.swapByte = pDev->config.swapByte;
            }

            /* consume the dequeued valid frame */
            if (pMsg->payload.data != NULL)
            {
                if (pMsg->msgInfo == kMsgInfo_Local)
                {
                    /* handle the display response */
                    _FWK_CameraManager_DisplayResponse(pDev, pMsg, pCameraTaskData);

                    /* handle the vision algorithm response */
                    _FWK_CameraManager_VisionAlgoResponse(pDev, pMsg, pCameraTaskData);

                    /* enqueue a new camera buffer request */
                    if (pDev != NULL && pDev->ops->enqueue != NULL)
                    {
                        error = pDev->ops->enqueue(pDev, NULL);

                        if (error)
                        {
                            LOGE("Camera dev %d enqueue error %d", devId, error);
                        }

                        /* calculate the fps */
                        fwk_fps(kFWKFPSType_Camera, devId);
                    }
                }
#if FWK_SUPPORT_MULTICORE
                else
                {
                    fwk_message_t *Msg = &s_CameraTask.cameraData.cameraRemoteDequeueMsg;
                    memcpy(Msg, pMsg, sizeof(fwk_message_t));
                    Msg->freeAfterConsumed             = 0;
                    Msg->multicore.isMulticoreMessage  = 1;
                    Msg->multicore.wasMulticoreMessage = 0;
                    Msg->multicore.taskId              = kFWKTaskID_Camera;
                    Msg->id                            = kFWKMessageID_CameraRemoteDequeue;
                    Msg->msgInfo                       = kMsgInfo_Remote;
                    FWK_Message_Put(kFWKTaskID_Camera, &Msg);
                }
#endif
            }
        }
        break;
#if FWK_SUPPORT_MULTICORE
        case kFWKMessageID_CameraRemoteDequeue:
        {
            camera_dev_t *pDev = NULL;

            /* consume the dequeued valid frame */
            if (pMsg->payload.data != NULL)
            {
                /* handle the display response */
                _FWK_CameraManager_DisplayResponse(pDev, pMsg, pCameraTaskData);

                /* handle the vision algorithm response */
                _FWK_CameraManager_VisionAlgoResponse(pDev, pMsg, pCameraTaskData);

                /* response the kFWKMessageID_CameraRemoteDequeueResponse to the sender */
                fwk_message_t *Msg = &s_CameraTask.cameraData.cameraRemoteDequeueMsg;
                memcpy(Msg, pMsg, sizeof(fwk_message_t));
                Msg->freeAfterConsumed             = 0;
                Msg->multicore.isMulticoreMessage  = 1;
                Msg->multicore.wasMulticoreMessage = 0;
                Msg->multicore.taskId              = kFWKTaskID_Camera;
                Msg->id                            = kFWKMessageID_CameraRemoteDequeueResponse;
                Msg->msgInfo                       = kMsgInfo_Remote;
                FWK_Message_Put(kFWKTaskID_Camera, &Msg);
            }
        }
        break;

        case kFWKMessageID_CameraRemoteDequeueResponse:
        {
            int devId          = pMsg->payload.devId;
            camera_dev_t *pDev = pCameraTaskData->devs[devId];

            /* enqueue a new camera buffer request */
            if (pDev != NULL && pDev->ops->enqueue != NULL)
            {
                error = pDev->ops->enqueue(pDev, NULL);

                if (error)
                {
                    LOGE("Camera dev %d enqueue error %d", devId, error);
                }

                /* calculate the fps */
                fwk_fps(kFWKFPSType_Camera, devId);
            }
        }
        break;
#endif /* FWK_SUPPORT_MULTICORE */
        case kFWKMessageID_InputNotify:
        {
            for (int i = 0; i < MAXIMUM_CAMERA_DEV; i++)
            {
                camera_dev_t *pDev = pCameraTaskData->devs[i];
                if ((pDev != NULL) && (pDev->ops->inputNotify != NULL))
                {
                    error = pDev->ops->inputNotify(pDev, pMsg->payload.data);

                    if (error)
                    {
                        LOGE("inputNotify camera dev id:%d name:%s error %d", pDev->id, pDev->name, error);
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

        case kFWKMessageID_LpmPreEnterSleep:
        {
            for (int i = 0; i < MAXIMUM_CAMERA_DEV; i++)
            {
                camera_dev_t *pDev = pCameraTaskData->devs[i];
                if ((pDev != NULL) && (pDev->ops->deinit != NULL))
                {
                    pDev->ops->deinit(pDev);
                }
            }
        }
        break;

        case kFWKMessageID_InputFrameworkGetComponents:
        {
            framework_request_t frameworkRequest   = pMsg->payload.frameworkRequest;
            fwk_task_component_t fwkTaskComponent  = {0};
            framework_response_t frameworkResponse = {0};
            fwkTaskComponent.managerId             = kFWKTaskID_Camera;

            for (int i = 0; i < MAXIMUM_CAMERA_DEV; i++)
            {
                camera_dev_t *pDev = pCameraTaskData->devs[i];
                if (pDev != NULL)
                {
                    fwkTaskComponent.deviceId          = pDev->id;
                    fwkTaskComponent.deviceName        = pDev->name;
                    frameworkResponse.fwkTaskComponent = fwkTaskComponent;
                    if (frameworkRequest.respond != NULL)
                    {
                        frameworkRequest.respond(kFrameworkEvents_GetManagerComponents, &frameworkResponse, false);
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

int FWK_CameraManager_Init()
{
    for (int i = 0; i < MAXIMUM_CAMERA_DEV; i++)
    {
        s_CameraTask.cameraData.devs[i] = NULL;
    }
    s_CameraTask.cameraData.pOverlaySurface = NULL;

    return 0;
}

int FWK_CameraManager_Start(int taskPriority)
{
    /* create the camera task */
    LOGD("[CameraManager]:Starting...");
    int error = 0;

    /* init all messages which camera task will send to other tasks */
    /* init the camera dequeue message */
    for (int i = 0; i < MAXIMUM_CAMERA_DEV; i++)
    {
        camera_dev_t *pDev  = s_CameraTask.cameraData.devs[i];
        fwk_message_t *pMsg = &s_CameraTask.cameraData.cameraDequeueMsg[i];
        if (pDev && pMsg)
        {
            pMsg->id                   = kFWKMessageID_CameraDequeue;
            pMsg->payload.devId        = i;
            pMsg->payload.frame.width  = pDev->config.width;
            pMsg->payload.frame.height = pDev->config.height;
            pMsg->payload.data         = NULL;
        }
    }

    for (int i = 0; i < MAXIMUM_DISPLAY_DEV; i++)
    {
        fwk_message_t *pMsg        = &s_CameraTask.cameraData.displayResponseMsg[i];
        msg_payload_t *pMsgPayload = &s_CameraTask.cameraData.displayRequestFrameInfo[i];
        if (pMsg && pMsgPayload)
        {
            pMsg->id                  = kFWKMessageID_DisplayResponseFrame;
            pMsgPayload->devId        = MAXIMUM_DISPLAY_DEV;
            pMsgPayload->frame.format = kPixelFormat_Invalid;
            pMsgPayload->frame.height = 0;
            pMsgPayload->frame.width  = 0;
            pMsgPayload->data         = 0;
        }
    }

    for (int i = 0; i < MAXIMUM_VISION_ALGO_DEV * kVAlgoFrameID_Count; i++)
    {
        fwk_message_t *pMsg        = &s_CameraTask.cameraData.vAlgoResponseMsg[i];
        msg_payload_t *pMsgPayload = &s_CameraTask.cameraData.vAlgoRequestFrameInfo[i];
        if (pMsg && pMsgPayload)
        {
            pMsg->id                  = kFWKMessageID_VAlgoResponseFrame;
            pMsgPayload->devId        = i / (kVAlgoFrameID_Count);
            pMsgPayload->frame.format = kPixelFormat_Invalid;
            pMsgPayload->frame.height = 0;
            pMsgPayload->frame.width  = 0;
            pMsgPayload->data         = 0;
        }
    }

    s_CameraTask.task.msgHandle  = _FWK_CameraManager_MessageHandle;
    s_CameraTask.task.taskInit   = _FWK_CameraManager_TaskInit;
    s_CameraTask.task.data       = (fwk_task_data_t *)&(s_CameraTask.cameraData);
    s_CameraTask.task.taskId     = kFWKTaskID_Camera;
    s_CameraTask.task.delayMs    = 1;
    s_CameraTask.task.taskStack  = s_CameraTaskStack;
    s_CameraTask.task.taskBuffer = s_CameraTaskTCBReference;
    FWK_Task_Start((fwk_task_t *)&s_CameraTask.task, CAMERA_MANAGER_TASK_NAME, CAMERA_MANAGER_TASK_STACK, taskPriority);

    LOGD("[CameraManager]:Started");
    return error;
}

int FWK_CameraManager_Deinit()
{
    return 0;
}

int FWK_CameraManager_DeviceRegister(camera_dev_t *dev)
{
    int error = -1;

    for (int i = 0; i < MAXIMUM_CAMERA_DEV; i++)
    {
        if (s_CameraTask.cameraData.devs[i] == NULL)
        {
            dev->id                         = i;
            dev->state                      = kState_HAL_Registered;
            s_CameraTask.cameraData.devs[i] = dev;
            return 0;
        }
    }

    return error;
}
