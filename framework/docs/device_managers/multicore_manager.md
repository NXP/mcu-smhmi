---
sidebar_position: 8
---

# Multicore Manager

Multicore manager manages the multicore HAL device which can be registered into the system.
In the current framework implementation, there are two ways in which a message becomes a multicore message.

1. isMulticoreMessage flag set to 1

    A message constructed with isMulticoreMessage set to 1, becomes automatically a multicast message that is going to be send to both cores. TaskId field specify the task that should handle the message from the other core.
    The below code snip shows how the message is send to both CM4/CM7 with Multicore manager as `man in the middle`.

    ```c
    pVAlgoResMsg->multicore.isMulticoreMessage = 1;
    pVAlgoResMsg->multicore.taskId             = kFWKTaskID_Output;
    FWK_Message_Put(kFWKTaskID_VisionAlgo, &pVAlgoResMsg);
    ```
    If the message has been sent by the CM7/Camera_Manager,
    the message will be send to CM7/VisionAlgo and to CM4/Output via Multicore Manager

    ```bash title="Multicore message flow isMulticoreMessage = 1"
    FWK_Message_Put(kFWKTaskID_VisionAlgo, &pVAlgoResMsg);
   ├── Message send to CM7/kFWKTaskID_VisionAlgo
   └── Message send to CM7/Multicore Manager -> Deep Copy -> Message send to CM4/Multicore Manager -> Message send to CM4/pVAlgoResMsg.taskId
    ```

2. isMulticoreMessage field set to 0

    A message constructed with isMulticoreMessage set to 0, is a unicast message that is going to be send only to the task specified in the FWK_Message_Put.
    If the task specify is the Multicore task, an additional taskId must pe specify, in order for the multicore counterpart to be able to properly consume it.
    ```c
    pAudioReqMsg->multicore.isMulticoreMessage = 0;
    pAudioReqMsg->multicore.taskId             = kFWKTaskID_Output;
    FWK_Message_Put(kFWKTaskID_Multicore, &pAudioReqMsg);
    ```
    If the message has been sent by the CM7/Camera_Manager,
    the message will be send only to CM4/Output via Multicore Manager

    ```bash title="Multicore message flow isMulticoreMessage = 0"
    FWK_Message_Put(kFWKTaskID_Multicore, &pAudioReqMsg);
   └── Message send to CM7/Multicore Manager -> Deep Copy -> Message send to CM4/Multicore Manager -> Message send to CM4/pAudioReqMsg.taskId
    ```

When sending a message, a deep copy of the message is done by the Multicore Manager. The purpose of the deep copy is to avoid sending references from untouchable regions.
Like CM7 sending a reference that points to internal TCM memory which can not be seen by the CM4.
Deep copy ensures that the message are stored in a shared buffer, so because of that, messages should be small in size.

In case there are bigger buffers that need to be send, they have to be in a shared memory area and passed by reference.(camera buffers)

## APIs

### FWK_MulticoreManager_Init

```c
/**
 * @brief Init internal structures for Multicore Manager
 * @return int Return 0 if the init process was successful
 */
int FWK_MulticoreManager_Init();
```

### FWK_MulticoreManager_DeviceRegister

```c
/**
 * @brief Register a Multicore device. Only one multicore device is supported. The dev needs to be registered before
 * FWK_MulticoreManager_Start is called
 * @param dev Pointer to a camera device structure
 * @return int Return 0 if registration was successful
 */
int FWK_MulticoreManager_DeviceRegister(multicore_dev_t *dev);
```

### FWK_MulticoreManager_Start

```c
/**
 * @brief Spawn Multicore manager task which will call init/start for all registered multicore devices
 * @param taskPriority the priority of the Multicore manager task
 * @return int Return 0 if the starting process was successful
 */
int FWK_MulticoreManager_Start(int taskPriority);
```

### FWK_MulticoreManager_Deinit

```c
/**
 * @brief Deinit MulticoreManager
 * @return int Return 0 if the deinit process was successful
 */
int FWK_MulticoreManager_Deinit();
```

