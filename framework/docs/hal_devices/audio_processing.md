# Audio Processing Device

Audio Processing Device mainly is used for Audio Front End(AFE)processing. In following sections, we abridge `Audio Processing Device` as `AFE device`. And also use 'AFE manager' instead of 'audio_processing manager'

The `AFE` HAL device provides an abstraction to represent audio front end(AFE)handling.

AFE provides several sub-algorithm modules, finally outputting clean stream for ASR engine. AFE supports Beamformer, AEC, NS and DOA.
Beamformer eliminates reverberation and background noise.
AEC(Acoustic Echo Cancellation) can support multi-channel systems, which is used for suppressing local speaker stream.
DOA(Direction Of Arrival) tracking with 1 degree resolution.

The AFE device is fed with microphone streams and reference streams(speaker streams) and output clean stream for ASR engine.

As with other device types, AFE device is controlled via AFE manager.
The AFE manager is responsible for managing all registered AFE HAL devices,
and invoking AFE device operators (`init`, `start`, `run`, `stop`, etc.) as necessary.
Additionally, the AFE Manager allows for multiple AFE devices to be registered and operate at once, but based on real project requirements, in most cases, only one AFE device is needed.

## Device Definition

The HAL device definition for `AFE` devices can be found under "framework/hal_api/hal_audio_processing_dev.h" and is reproduced below:


```c title="audio_processing_dev_t"
typedef struct _audio_processing_dev audio_processing_dev_t;
/*! @brief Attributes of an audio processing device. */
struct _audio_processing_dev
{
    /* unique id which is assigned by audio processing manager during registration */
    int id;
    /* name of the device */
    char name[DEVICE_NAME_MAX_LENGTH];
    /* operations */
    const audio_processing_dev_operator_t *ops;
    /* private capability */
    audio_processing_dev_private_capability_t cap;
};
```

The device `operators` associated with AFE HAL devices are as shown below:

```c title="Operators"
/*! @brief Operation that needs to be implemented by a audio processing device */
typedef struct _audio_processing_dev_operator
{
    /* initialize the dev */
    hal_audio_processing_status_t (*init)(audio_processing_dev_t *dev, audio_processing_dev_callback_t callback);
    /* deinitialize the dev */
    hal_audio_processing_status_t (*deinit)(const audio_processing_dev_t *dev);
    /* start the dev */
    hal_audio_processing_status_t (*start)(const audio_processing_dev_t *dev);
    /* start the dev */
    hal_audio_processing_status_t (*stop)(const audio_processing_dev_t *dev);
    /* notify the audio_processing_dev_t */
    hal_audio_processing_status_t (*run)(const audio_processing_dev_t *dev, void *param);
    /* notify the audio_processing_dev_t */
    hal_audio_processing_status_t (*inputNotify)(const audio_processing_dev_t *dev, void *param);
} audio_processing_dev_operator_t;
```

The device `capabilities` associated with AFE HAL devices are as shown below:

```c title="config"
/*! @brief Structure that capability of the AFE device. */
typedef struct _audio_processing_dev_private_capability
{
    /* callback */
    audio_processing_dev_callback_t callback;
} audio_processing_dev_private_capability_t;
```

## Operators

Operators are functions which "operate" on a HAL device itself. Operators are akin to "public methods" in object oriented-languages, and are used by the AFE Manager to setup, start, etc. each of its registered AFE devices.

### Init

```c
hal_audio_processing_status_t (*init)(audio_processing_dev_t *dev, audio_processing_dev_callback_t callback);
```

Initialize the AFE device.

`Init` performs all setups the device requires, such as preparing memory for AFE runtime consumption,
microphone number and position and so on.

This operator will be called by the AFE Manager when the AFE Manager task first starts.

### Deinit

```c
hal_audio_processing_status_t (*deinit)(const audio_processing_dev_t *dev);
```

De-initialize the AFE device.

`DeInit` releases all memory resources allocated in initialization stage.
Set all handles created to NULL.

This operator is not called in AFE Manager based on current framework version **[1]**.

```{note}
**[1]** The `DeInit` function generally will not be called under normal operation.
```

### Start

```c
hal_audio_processing_status_t (*start)(const audio_processing_dev_t *dev);
```

Start the AFE device.

The `Start` operator will be called in the initialization stage of the AFE Manager's task after the call to the `Init` operator.
Since AFE device is a pure software device, there is not Clock/GPIO or any peripheral bus depended.
In most cases, the `Start` method can return kStatus_HAL_AudioProcessingSuccess directly.

### Stop

```c
hal_audio_processing_status_t (*stop)(const audio_processing_dev_t *dev);
```

`Stop` is reverted operation compared to `Start`.  Just return kStatus_HAL_AudioProcessingSuccess if there is nothing needed to be done to device.

For the AFE device SDK implemented, this method returns kStatus_HAL_AudioProcessingSuccess  directly. And it is not called  in AFE Manager based on current framework version.

### Run

```c
hal_audio_processing_status_t (*run)(const audio_processing_dev_t *dev, void *param);
```

Execute AFE engine for handling microphone stream and outputing clean stream.

The `Run` operator will be called by the AFE Manager to handle audio frame with 160 samples.

### InputNotify

```c
 hal_audio_processing_status_t (*inputNotify)(const audio_processing_dev_t *dev, void *param);
```

Handle input events.

The `InputNotify` operator is called by the AFE Manager whenever a `kFWKMessageID_InputNotify` message is received by and forwarded from the AFE Manager's message queue.

For more information regarding events and event handling, see [Events](../events/overview.md).

## Capabilities

```c title="AFE 'capabilities' Struct"
typedef struct _audio_processing_dev_private_capability
{
    /* callback */
    audio_processing_dev_callback_t callback;
} audio_processing_dev_private_capability_t;
```

The `capabilities` struct is primarily used for storing a callback to communicate information from the device back to the AFE Manager.
This callback function is typically installed via a device's `init` operator.

### callback

```c title="audio_processing_dev_callback_t"
/**
 * @brief Callback function to notify audio processing manager that an async event took place
 * @param dev Device structure of the audio processing device calling this function
 * @param event id of the event that took place
 * @return 0 if the operation was successfully
 */
typedef int (*audio_processing_dev_callback_t)(
    const audio_processing_dev_t *dev, audio_processing_event_t event, uint8_t fromISR);
```

Callback to the AFE Manager.

The HAL device invokes this callback to notify the AFE Manager of specific events like "audio processing done or audio dumping event."

The AFE Manager provides this callback to the device when the `init` operator is called.
As a result, the HAL device should make sure to store the callback in the `init` operator's implementation.

The event structure is as follow:
```c title="audio_processing_event_t"
/*! @brief Structure used to define an event.*/
typedef struct _audio_processing_event
{
    /* Eventid from the list above.*/
	audio_processing_event_id_t eventId;
    event_info_t     eventInfo;
    /* Pointer to a struct of data that needs to be forwarded. */
    void *data;
    /* Size of the struct that needs to be forwarded. */
    unsigned int size;
    /* If copy is set to 1, the framework will forward a copy of the data. */
    unsigned char copy;
} audio_processing_event_t;
```
As mentioned before, the eventId supported right now are Audio Processing Done and Audio Processing Dump.

* kAudioProcessingEvent_Done is an event used to signal that the processing done over the last chunk has been finalized. Depending where the ASR is initiated, this message can be forward to:
    * both core by setting the eventInfo flag to kEventInfo_DualCore
    * remote core only by setting the eventInfo flag to kEventInfo_Remote
    * local, by setting the eventInfo flag to kEventInfo_Local.

```{note}
We strongly encourage to design the architecture of the system to have both the AFE and ASR on the same core, to avoid high data traffic between cores. Also the `copy` flag should be set to 0 in order to have achieve best performance.
```
* kAudioProcessingEvent_Dump is an event which is send to an output device that can log the audio stream on an output interface `UART/USB/Wi-Fi/BLE`. As mentioned before this message can also be DualCore/Remote/Local, but we encourage to have it as a local message due to high data transfer. If the design doesn't support this, use reference and shared memory buffers, instead of deep copy the data .

```c title="Example AFE Dev Init" {10}
hal_audio_processing_status_t audio_processing_afe_init(audio_processing_dev_t *dev,
                                                        audio_processing_dev_callback_t callback)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;

    sln_afe_status_t afeStatus = kAfeSuccess;
    sln_afe_config_t afeConfig = {0};

    dev->cap.callback = callback;

    afeConfig.numberOfMics    = AUDIO_PDM_MIC_COUNT;
    afeConfig.afeMemBlock     = s_afeExternalMemory;
    ....
    return error;
}
```

### param

```c
void *param;
```

The parameter of the callback points to audio data AFE outputing.


## Example

The SLN-TLHMI-IOT project implements one AFE device for use as-is or for use as reference for implementing new AFE devices.
Source file for these AFE HAL device can be found under `hal/voice/hal_audio_processing_afe.c`.

```c title="hal/voice/hal_audio_processing_afe.c"

const static audio_processing_dev_operator_t audio_processing_afe_ops = {
    .init        = audio_processing_afe_init,
    .deinit      = audio_processing_afe_deinit,
    .start       = audio_processing_afe_start,
    .stop        = audio_processing_afe_stop,
    .run         = audio_processing_afe_run,
    .inputNotify = audio_processing_afe_notify,
};

static audio_processing_dev_t audio_processing_afe = {
    .id = 1, .name = "AFE", .ops = &audio_processing_afe_ops, .cap = {.callback = NULL}};

hal_audio_processing_status_t audio_processing_afe_init(audio_processing_dev_t *dev,
                                                        audio_processing_dev_callback_t callback)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
	/*
     * Prepare AFE memory and configuration parameters needed,
     * and then initialize AFE library.
	*/

    return error;
}

hal_audio_processing_status_t audio_processing_afe_deinit(const audio_processing_dev_t *dev)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    return error;
}

hal_audio_processing_status_t audio_processing_afe_start(const audio_processing_dev_t *dev)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    return error;
}

hal_audio_processing_status_t audio_processing_afe_stop(const audio_processing_dev_t *dev)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    return error;
}

hal_audio_processing_status_t audio_processing_afe_notify(const audio_processing_dev_t *dev, void *param)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    event_voice_t event                 = *(event_voice_t *)param;

	/* Parse event structure and do further handling */

    return error;
}

hal_audio_processing_status_t audio_processing_afe_run(const audio_processing_dev_t *dev, void *param)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    event_voice_t event                 = *(event_voice_t *)param;

    /* Parse event structure and execute AFE engine for handling microphone streams */

    return error;
}

```
