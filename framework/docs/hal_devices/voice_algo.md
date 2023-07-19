# Voice Algorithm Devices

The Voice Algorithm HAL device type represents an abstraction to do voice recognition based on clean stream AFE generated.

After Voice Algorithm manager receives clean stream, Voice Algorithm Hal device `run` method will be called. If voice command is detected, device will output inference result and transfer result to Output HAL device through `valgo_dev_callback_t callback`.
For more information about events and event handling, see "events/overview.md" file.

Currently, only one voice algorithm device can be registered to the Voice Manager at a time per the design of the framework.

## Device Definition

The HAL device definition for voice algorithm devices can be found under "framework/hal_api/hal_valgo_dev.h" and is reproduced below:

```c title="voice_algo_dev_t"
/*! @brief Attributes of a voice algo device */
struct _voice_algo_dev
{
    /* unique id which is assigned by algorithm manager during the registration */
    int id;
    /* name to identify */
    char name[DEVICE_NAME_MAX_LENGTH];
    /* private capability */
    valgo_dev_private_capability_t cap;
    /* operations */
    voice_algo_dev_operator_t *ops;
    /* private data */
    voice_algo_private_data_t data;
};
```

The `operators` associated with the voice algo HAL device are as shown below:

```c
/*! @brief Operation that needs to be implemented by a voice algorithm device */
typedef struct voice_algo_dev_operator_t
{
    /* initialize the dev */
    hal_valgo_status_t (*init)(voice_algo_dev_t *dev, valgo_dev_callback_t callback, void *param);
    /* deinitialize the dev */
    hal_valgo_status_t (*deinit)(voice_algo_dev_t *dev);
    /* start the dev */
    hal_valgo_status_t (*run)(const voice_algo_dev_t *dev, void *data);
    /* recv events */
    hal_valgo_status_t (*inputNotify)(const voice_algo_dev_t *receiver, void *data);

} voice_algo_dev_operator_t;
```

The `capabilities` associated with the voice algo HAL device are as shown below:

```c
typedef struct _valgo_dev_private_capability
{
    /* callback */
    valgo_dev_callback_t callback;
    /* param for the callback */
    void *param;
} valgo_dev_private_capability_t;
```

The `private data` fields associated with the voice algo HAL device is as shown below:

```c
typedef struct _voice_algo_private_data
{
} voice_algo_private_data_t;
```

## Operators

Operators are functions which "operate" on a HAL device itself.
Operators are akin to "public methods" in object oriented-languages,
and are used by the Voice Algorithm Manager to init, run, etc. its registered voice algo device.

For more information about operators, see `Operators`.

### Init

```c
hal_valgo_status_t (*init)(voice_algo_dev_t *dev, valgo_dev_callback_t callback, void *param);
```

Init the voice algo HAL device.

`Init` performs all setups the device requires, such as preparing memory for voice algorithm runtime consumption,
loading AI models, running library initialization API and so on.

The `callback` function](#callback) to the device's manager is typically installed as part of the `Init` function as well.

This operator will be called by the voice algorithm manager when the voice manager task first starts.

### Deinit

```c
hal_valgo_status_t (*deinit)(voice_algo_dev_t *dev);
```

The `DeInit` function is used to "deinitialize" the algorithm device.
`DeInit` should release any hardware resources the device uses (heap memory, handles created by device etc.), turn off the hardware, and perform any other shutdown required by the device.

This method is not called in AFE Manager based on current framework version **[1]**.

```{note}
**[1]** The `DeInit` function generally will not be called under normal operation.
```

### Run

```c
hal_valgo_status_t (*run)(const voice_algo_dev_t *dev, void *data);
```

Begin running the voice algorithm.

The `run` operator is used to start running algorithm inference and processing voice frame data.

This operator is called by the Voice Algorithm manager when
 kFWKMessageID_VAlgoASRInputProcess message is received from the AFE Manager and forwarded to the algorithm device via the Voice Algorithm Manager.

Once the Voice Algorithm device finishes processing the voice frame data, its manager will forward inference result to the Output Manager. If Wake Word is detected, Voice manager will forward a message indicating length of wake word to AFE manager.

### InputNotify

```c
hal_valgo_status_t (*inputNotify)(const voice_algo_dev_t *receiver, void *data);
```

Handle input events.

The `InputNotify` operator is called by the Voice Algorithm Manager whenever a `kFWKMessageID_InputNotify` message is received and forwarded from the Voice Algorithm Manager's message queue.

For more information regarding events and event handling, see "events/overview.md" file.

## Capabilities

The `capabilities` struct is primarily used for storing a callback to communicate information from the device back to the Voice Algorithm Manager.
This callback function is typically installed via a device's `init` operator.

### callback

```c title="valgo_dev_callback_t"
/*!
 * @brief Callback function to notify managers the results of inference
 * valgo_dev* dev Pointer to an algorithm device
 * valgo_event_t event Event which took place
 * persistent memory area.
 */

typedef int (*valgo_dev_callback_t)(int devId, valgo_event_t event,uint8_t fromISR);
```

```c
valgo_dev_callback_t callback;
```

Callback to the Voice Algorithm Manager.

The Voice Algorithm manager will provide the callback to the device when the `init` operator is called.
As a result, the HAL device should make sure to store the callback in the `init` operator's implementation.

The HAL device invokes this callback to notify the Voice Algorithm manager of specific events.

The event structure is the following.

```c title="valgo_event_t"
/*! @brief Structure used to define an event.*/
typedef struct _valgo_event
{
    /* Eventid from the list above.*/
    valgo_event_id_t eventId;
    event_info_t     eventInfo;
    /* Pointer to a struct of data that needs to be forwarded. */
    void *data;
    /* Size of the struct that needs to be forwarded. */
    unsigned int size;
    /* If copy is set to 1, the framework will forward a copy of the data. */
    unsigned char copy;
} valgo_event_t;
```

All the events, which are identifiable by the eventId, can be send to:
* both core in a broadcast manner by setting the eventInfo flag to kEventInfo_DualCore
* remote core by setting the eventInfo flag to kEventInfo_Remote
* local core by the eventInfo flag to kEventInfo_Local

In general, all supported message type can be used in conjunction with the copy field set to 1 in order to deep copy the message. One exception is kVAlgoEvent_AsrToAudioDump event, which we encourage to be send with the flag set to 0 in order to avoid copy on large buffers.

### param

```c
void *param;
```
The param for the callback (optional).

## Example

Because only one Voice Algorithm device can be registered at a time per the design of the framework,
the SLN-TLHMI-IOT project has two Voice Algorithm devices(DSMT/VIT) implemented **[2]**.

```{note}
**[2]** This example is implemented using DSMT algorithm
```

This example is reproduced below:

```c
hal_valgo_status_t voice_algo_dev_asr_init(voice_algo_dev_t *dev, valgo_dev_callback_t callback, void *param)
static hal_valgo_status_t HAL_VisionAlgoDev_OasisLite_Deinit(vision_algo_dev_t *dev);
hal_valgo_status_t voice_algo_dev_asr_run(const voice_algo_dev_t *dev, void *data)
hal_valgo_status_t voice_algo_dev_input_notify(const voice_algo_dev_t *dev, void *data)

const static voice_algo_dev_operator_t voice_algo_dev_asr_ops = {
    .init        = voice_algo_dev_asr_init,
    .deinit      = NULL,
    .run         = voice_algo_dev_asr_run,
    .inputNotify = voice_algo_dev_input_notify
};

static voice_algo_dev_t voice_algo_dev_asr = {
    .id  = 0,
    .ops = (voice_algo_dev_operator_t *)&voice_algo_dev_asr_ops,
    .cap = {.param = NULL},
};

hal_valgo_status_t voice_algo_dev_asr_init(voice_algo_dev_t *dev, valgo_dev_callback_t callback, void *param)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    uint32_t timerId       = 0;

    /* Set callback function */
    dev->cap.callback = callback;

    ...

    /* Initialize the ASR engine */
    initialize_asr();

    ...

    return ret;
}

/* voice algorithm device inference run function*/
hal_valgo_status_t voice_algo_dev_asr_run(const voice_algo_dev_t *dev, void *data)
{
    hal_valgo_status_t status    = kStatus_HAL_ValgoSuccess;
    static asr_events_t asrEvent = ASR_SESSION_ENDED;
    struct asr_inference_engine *pInfWW;
    struct asr_inference_engine *pInfCMD;
    char **cmdString;
    int16_t *pi16Sample;

    msg_payload_t *audioIn = (msg_payload_t *)data;

    ...

    /* Wake Word detection. Check all enabled languages, but stop on first match. */
    for (pInfWW = s_AsrEngine.voiceControl.infEngineWW; pInfWW != NULL; pInfWW = pInfWW->next)
    {
        if (asr_process_audio_buffer(pInfWW->handler, pi16Sample, NUM_SAMPLES_AFE_OUTPUT, pInfWW->iWhoAmI_inf) == kAsrLocalDetected)

        {
            LOGI("Trust: %d, SGDiff: %d\r\n", s_AsrEngine.voiceControl.result.trustScore,
                    s_AsrEngine.voiceControl.result.SGDiffScore);
        }
    }

    ...

    return status;
}

hal_valgo_status_t voice_algo_dev_input_notify(const voice_algo_dev_t *dev, void *data)
{
    hal_valgo_status_t error = kStatus_HAL_ValgoSuccess;
    event_voice_t event      = *(event_voice_t *)data;
    const char *language_str = NULL;

    ...

    return error;
}

int HAL_VoiceAlgoDev_Asr_Register()
{
    int error = 0;
    LOGD("HAL_VoiceAlgoDev_Asr_Register");
    error = FWK_VoiceAlgoManager_DeviceRegister(&voice_algo_dev_asr);
    return error;
}
```
