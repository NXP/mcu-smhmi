# Overview

Events are a means by which information is communicated between different devices via their managers.

## Event triggers

Events can correspond to many different happenings during the runtime of the application,
and can include things like:

- Button pressed
- Face detected
- Shell command received

When an event is triggered, the device that first received the event communicates that event to its manager,
which in turn notifys other managers designated to receive the event.

For example,
when a button is pressed,
a flow similar to the following takes place:

1. The "Push Button" HAL device receives an interrupt corresponding to the button that was pressed.
2. Inside the HAL device's interrupt handler,
   the device associates an event with the button that was pressed.
3. The HAL device specifies which managers should receive the event.
4. The HAL device forwards the event to its manager.

The code that corresponds to this scenario can be seen in the below excerpts from "framework/hal/input/hal_input_push_buttons.c" and app source located in "source/event_handlers" folder, respectively.

```c title="" {10,11}
void _HAL_InputDev_IrqHandler(button_data_t *button, switch_press_type_t pressType)
{
    if (s_InputDev_PushButtons.cap.callback != NULL)
    {
        uint32_t receiverList;
        if (APP_InputDev_PushButtons_SetEvent(button->buttonId, pressType, &s_pEvent, &receiverList) == kStatus_Success)
        {
            s_inputEvent.inputData = s_pEvent;
            uint8_t fromISR        = __get_IPSR();
            s_InputDev_PushButtons.cap.callback(&s_InputDev_PushButtons, kInputEventID_Recv, receiverList,
                                                &s_inputEvent, 0, fromISR);
        }
        else
        {
            LOGE("No valid event associated with SW%d button %s press", button->buttonId,
                 pressType == kSwitchPressType_Short ? "short" : "long");
        }
    }
}
```

```{note}
The "callback" function in the above code refers to an internal callback function inside the [Input Manager](../device_managers/input_manager.md)
which relays input events to each of the managers specified in an event's `receiverList`.
```

```c
    switch (button)
    {
        case kSwitchID_1:
            if (pressType == kSwitchPressType_Long)
            {
                LOGD("Long PRESS Detected.");
                unsigned int totalUsageCount;
                FWK_LpmManager_RequestStatus(&totalUsageCount);
                FWK_LpmManager_EnableSleepMode(kLPMManagerStatus_SleepEnable);
            }
            break;

        case kSwitchID_2:
            if ((pressType == kSwitchPressType_Short) || (pressType == kSwitchPressType_Long))
            {
                *receiverList                    = 1 << kFWKTaskID_VisionAlgo;
                s_FaceRecEvent.eventBase.eventId = kEventFaceRecID_DelUser;
                s_FaceRecEvent.delFace.hasName   = false;
                s_FaceRecEvent.delFace.hasID     = false;
                *event                           = &s_FaceRecEvent;
            }
            break;

        case kSwitchID_3:
            if ((pressType == kSwitchPressType_Short) || (pressType == kSwitchPressType_Long))
            {
                *receiverList                    = 1 << kFWKTaskID_VisionAlgo;
                s_FaceRecEvent.eventBase.eventId = kEventFaceRecID_AddUser;
                s_FaceRecEvent.addFace.hasName   = false;
                *event                           = &s_FaceRecEvent;
            }
            break;

        default:
            ret = kStatus_Fail;
            break;
    }

    return ret;
```

## Types of events

Events can be used to communicate all sorts of information,
but the two types of events defined by default are
`InferComplete` events and `InputNotify` events.

Both types of events represent different information being communicated to and by the HAL devices.

### InferComplete events

Inference events are used to indicate that a vision/voice algorithm HAL device has completed a stage in its inference pipeline.

```{note}
Currently, only output HAL devices can respond to `InferComplete` events.
This is not true of `InputNotify` events.
```

In the current application, it can refer to several things, including:

- Face detected
- Face recognized
- Fake face detected

Output HAL devices can respond to inference events by implementing an `inferComplete` method.
When an "InferComplete" event is triggered,
the output manager attempts to call the `inferComplete` event handler of each of its devices,
(assuming the device has implemented an `inferComplete` function).

As part of the `inferComplete` function call,
the output manager also communicates the HAL device from which the event originated, the ID of the event received,
as well as any additional information related to the event that was generated.

For example, a "Face Recognized" event also includes the ID of the face being recognized. Below is an example of how the RGB LED HAL device responds to several different events.

```c title="RGB Output Device InferComplete Handler" {12,15}
static hal_output_status_t HAL_OutputDev_RgbLed_InferComplete(const output_dev_t *dev,
                                                              output_algo_source_t source,
                                                              void *inferResult)
{
    vision_algo_result_t *visionAlgoResult = (vision_algo_result_t *)inferResult;
    hal_output_status_t error              = kStatus_HAL_OutputSuccess;

    if (visionAlgoResult != NULL)
    {
        if (visionAlgoResult->id == kVisionAlgoID_OasisLite)
        {
            oasis_lite_result_t *result = &(visionAlgoResult->oasisLite);
            if (source == kOutputAlgoSource_Vision)
            {
                if ((result->face_recognized) && (result->face_id >= 0))
                {
                    RGB_LED_SET_COLOR(kRGBLedColor_Green);
                }
                else if (result->face_count)
                {
                    RGB_LED_SET_COLOR(kRGBLedColor_Red);
                }
                else
                {
                    RGB_LED_SET_COLOR(kRGBLedColor_Off);
                }
            }
        }
```

For more information about handling events, see "event handlers" section.

### InputNotify events

Input events are events that indicate that input has been received by an input HAL device.

```{note}
Only input HAL devices can generate an "InputNotify" event.
However, all HAL devices
(with the exception of [LPM](../hal_devices/low_power.md), Flash, and Graphics devices)
are able to respond to an "InputNotify" event.
```

Examples of input events include:

- Button pressed
- Shell command received
- Wi-Fi/BLE input received

The event to generate for a given input is decided by the device which receives the input.

For example, the Push-Button device associates different events based on the different button presses and the duration of those button presses, either long or short presses.

```c title="Push Button Event Generator"
    switch (button)
    {
        case kSwitchID_1:
            if (pressType == kSwitchPressType_Long)
            {
                LOGD("Long PRESS Detected.");
                unsigned int totalUsageCount;
                FWK_LpmManager_RequestStatus(&totalUsageCount);
                FWK_LpmManager_EnableSleepMode(kLPMManagerStatus_SleepEnable);
            }
            break;

        case kSwitchID_2:
            if ((pressType == kSwitchPressType_Short) || (pressType == kSwitchPressType_Long))
            {
                *receiverList                    = 1 << kFWKTaskID_VisionAlgo;
                s_FaceRecEvent.eventBase.eventId = kEventFaceRecID_DelUser;
                s_FaceRecEvent.delFace.hasName   = false;
                s_FaceRecEvent.delFace.hasID     = false;
                *event                           = &s_FaceRecEvent;
            }
            break;

        case kSwitchID_3:
            if ((pressType == kSwitchPressType_Short) || (pressType == kSwitchPressType_Long))
            {
                *receiverList                    = 1 << kFWKTaskID_VisionAlgo;
                s_FaceRecEvent.eventBase.eventId = kEventFaceRecID_AddUser;
                s_FaceRecEvent.addFace.hasName   = false;
                *event                           = &s_FaceRecEvent;
            }
            break;

        default:
            ret = kStatus_Fail;
            break;
    }
```

Alongside an input event,
the HAL device from which the event originated may also relay additional information.
Depending on the event,
this may correspond to the button that was pressed,
the shell command and args that were received, and so on.

In the above example,
we can see that pressing the SW3 push button generates a `kEventFaceRecID_AddUser` event, specifying that there is no name for the face to add.

<!-- TODO: Define difference between event_descriptor common and face_rec -->

```{note}
A list of general events can be found in `hal_event_descriptor_common.h`, while a list of face recognition-specific events can be found in `hal_event_descriptor_face_rec.h`.
It is recommended that new events be added to the `hal_event_descriptor_common.h` file.
```

To respond to an "InputNotify" event, a HAL device must implement an `inputNotify` handler function.
When an "InputNotify" event is triggered,
each manager which receives the event attempts to call the `inputNotify` method of every one of its devices,
(assuming the device has implemented an `inputNotify` method).

For more information regarding event handlers, see "event handlers" section.
