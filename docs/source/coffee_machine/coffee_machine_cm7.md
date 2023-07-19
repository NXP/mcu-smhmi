# Coffee machine CM7

This Coffee Machine CM7 host project runs on the CM7 core.

It is linked to flash with the combination of the CM4 project.

The CM7 was designed to focus on the vision and voice algorithms' processing to get the best performance.

# Main functionalities
- Vision algorithm
- Voice algorithm
- Audio playback
- Microphone stream input
- Multicore communication
- Littlefs format filesystem

# Boot sequence
The "main" entry of this project is located in the "coffee_machine/cm7/source/sln_smart_tlhmi_cm7.cpp" file.
The basic boot up flow is:
- Initialize board level
- Initialize framework
- Register HAL devices
- Start the framework
- Start the freeRTOS scheduler

```c
int main(void)
{
    /* init the board */
    APP_BoardInit();

    ...

    /* init the framework*/
    APP_InitFramework();

    /* register the hal devices*/
    APP_RegisterHalDevices();

    /* start the framework*/
    APP_StartFramework();

    vTaskStartScheduler();

    for (;;)
    {
    }
}
```

# Board level initialization
The board-level initialization is implemented in the "APP_BoardInit()" entry which is located in "coffee_machine/cm7/source/sln_smart_tlhmi_cm7.cpp".
Below is the main flow:
- Relocate vector table into RAM
- Configure MPU, Clock, and Pins
- Debug console with hardware semaphore initialization
- System time stamp start
- Load resource from flash into share memory region
- Multicore manager init and boot slave core

```c
void APP_BoardInit(void)
{
    BOARD_RelocateVectorTableToRam();

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    BOARD_InitDebugConsole();
    Time_Init(1);

    APP_LoadResource();

    /* Initialize the HW Semaphore */
    SEMA4_Init(BOARD_SEM4_BASE);

#if defined(ENABLE_MASTER) && ENABLE_MASTER
    /* Initialize MCMGR before calling its API */
    (void)MCMGR_Init();

    /* Boot Secondary core application */
    (void)MCMGR_StartCore(kMCMGR_Core1, (void *)(char *)CORE1_BOOT_ADDRESS, 0, kMCMGR_Start_Synchronous);
#endif /* defined(ENABLE_MASTER) && ENABLE_MASTER */
}
```
# Framework managers
The below framework managers are enabled in the cm7 side with the following priorities:
- Vision algorithm manager - P3
- Voice algorithm manager - P3
- Audio processing manager - P2
- Input manager - P1
- Output manager - P4
- Multicore manager - P0
- Flash device manager

Where P0 is the highest priority and P4 is the least prioritized.
```{note}
Choosing the right priority for the manager is something that must be addressed based on the requirements. 
Our recommendation is to keep Vision manager equal to or less than Voice manager, or the audio sample can be lost.
```
Refer to the framework documentation located in "framework/doc" for a detailed description of these framework managers.

```{note}
To prepare the environment for other framework managers, initialize the file system and application configuration first.
```

```c
int APP_InitFramework(void)
{
    int ret = 0;

    HAL_FLASH_DEV_REGISTER(Littlefs, ret);
    HAL_OutputDev_SmartTlhmiConfig_Init();

    FWK_MANAGER_INIT(VisionAlgoManager, ret);
    FWK_MANAGER_INIT(VoiceAlgoManager, ret);
    FWK_MANAGER_INIT(AudioProcessing, ret);
    FWK_MANAGER_INIT(OutputManager, ret);
    FWK_MANAGER_INIT(InputManager, ret);
#if defined(ENABLE_MASTER) && ENABLE_MASTER
    FWK_MANAGER_INIT(MulticoreManager, ret);
#endif /* defined(ENABLE_MASTER) && ENABLE_MASTER */

    return ret;
}

int APP_StartFramework(void)
{
    int ret = 0;

    FWK_MANAGER_START(VisionAlgoManager, VISION_ALGO_MANAGER_TASK_PRIORITY, ret);
    FWK_MANAGER_START(OutputManager, OUTPUT_MANAGER_TASK_PRIORITY, ret);
    FWK_MANAGER_START(AudioProcessing, AUDIO_PROCESSING_TASK_PRIORITY, ret);
    FWK_MANAGER_START(InputManager, INPUT_MANAGER_TASK_PRIORITY, ret);
    FWK_MANAGER_START(VoiceAlgoManager, VOICE_ALGO_MANAGER_TASK_PRIORITY, ret);
#if defined(ENABLE_MASTER) && ENABLE_MASTER
    FWK_MANAGER_START(MulticoreManager, MULTICORE_MANAGER_TASK_PRIORITY, ret);
#endif /* defined(ENABLE_MASTER) && ENABLE_MASTER */

    return ret;
}
```

# Framework HAL devices
The enabled HAL devices are configured in the "coffee_machine/cm7/board/board_define.h" file as shown below:

```c
#define ENABLE_INPUT_DEV_PdmMic
#define ENABLE_AUDIO_PROCESSING_DEV_Afe
#define ENABLE_DSMT_ASR
#define ENABLE_OUTPUT_DEV_MqsAudio
#define ENABLE_OUTPUT_DEV_SmartTlhmiConfig
#define ENABLE_VISIONALGO_DEV_Oasis_CoffeeMachine
#define ENABLE_FLASH_DEV_Littlefs
#define ENABLE_FACEDB
#define USE_CAMERA_MipiGc2145
#if defined(ENABLE_MASTER) && ENABLE_MASTER
#define ENABLE_MULTICORE_DEV_MessageBuffer
#endif /* defined(ENABLE_MASTER) && ENABLE_MASTER */
```

The registration of the enabled HAL devices is implemented in the `APP_RegisterHalDevices(...)` function which is located in ""coffee_machine/cm7/source/sln_smart_tlhmi_cm7.cpp":

```{note}
The `APP_RegisterHalDevices(...)` must be called after the framework initialization `APP_InitFramework(...)` and before framework startup `APP_StartFramework(...)`.
```

```c
int APP_RegisterHalDevices(void)
{
    int ret = 0;

    HAL_OUTPUT_DEV_REGISTER(MqsAudio, ret);
    HAL_AUDIO_PROCESSING_DEV_REGISTER(Afe, ret);
    HAL_INPUT_DEV_REGISTER(PdmMic, ret);
    HAL_VOICEALGO_DEV_REGISTER(Asr, ret);
    HAL_VALGO_DEV_REGISTER(OasisCoffeeMachine, ret);
#if defined(ENABLE_MASTER) && ENABLE_MASTER
    HAL_MULTICORE_DEV_REGISTER(MessageBuffer, ret);
#endif /* defined(ENABLE_MASTER) && ENABLE_MASTER */
    HAL_INPUT_DEV_REGISTER(WiFiAWAM510, ret);

    return ret;
}
```

# Logging
Both the CM7 and CM4 projects are leveraging the [FreeRTOS logging library](https://www.freertos.org/logging.html).

The FreeRTOS logging library code is located in the logging folder where you can find the detailed document located in "coffee_machine/cm7/freertos/libraries/logging/README.md".

The CM7 and CM4 share the low-level LPUART12 peripheral for the logging output. The hardware semaphore mechanism is used to guarantee the concurrence access of the LPUART12 peripheral. They share a low-level timer to get the unified timestamp of the logging information.

## Log Task Init
The application calls the `xLoggingTaskInitialize(...)` API to create the logging task in the `main()` entry of this project and is located in "coffee_machine/cm7/source/sln_smart_tlhmi_cm7.cpp":

```c
xLoggingTaskInitialize(LOGGING_TASK_STACK_SIZE, LOGGING_TASK_PRIORITY, LOGGING_QUEUE_LENGTH);
```

## Log Macros
There are four kinds of logging that can be used in both cm7 and cm4, which you can find in "framework/inc/fwk_log.h".

```c
#ifndef LOGV
#define LOGV(fmt, args...) {implement...}
...
#endif

#ifndef LOGD
#define LOGD(fmt, args...) {implement...}
#endif

#ifndef LOGI
#define LOGI(fmt, args...) {implement...}
#endif

#ifndef LOGE
#define LOGE(fmt, args...) {implement...}
#endif
```
# Coffee Machine database
The Coffee Machine application uses framework flash operations with low-level littlefs file system to store the recognized user-faces database and user coffee information. The detailed usage API is located in files "framework/hal/vision/hal_sln_facedb.h" and "coffee_machine/cm7/source/hal_sln_coffeedb.h". The face database and user coffee information database entry are bound together using user id. The user id is a unique identifier on one device.

To make it easier for users to add their database with personal attributes, we split the face database from user database. The user should create something similar with hal_sln_coffeedb.h and add attributes like in the coffee_attribute_t structure.

## Face recognize databse usage
***g_facedb_ops*** handles all kinds of face database operation.
```c
typedef struct _facedb_ops
{
    facedb_status_t (*init)(uint16_t featureSize);
    facedb_status_t (*saveFace)(void);
    facedb_status_t (*addFace)(uint16_t id, char *name, void *face, int size);
    facedb_status_t (*delFaceWithId)(uint16_t id);
    facedb_status_t (*delFaceWithName)(char *name);
    facedb_status_t (*updNameWithId)(uint16_t id, char *name);
    facedb_status_t (*updFaceWithId)(uint16_t id, char *name, void *face, int size);
    facedb_status_t (*getFaceWithId)(uint16_t id, void **pFace);
    facedb_status_t (*getIdsAndFaces)(uint16_t *face_ids, void **pFace);
    facedb_status_t (*getIdWithName)(char *name, uint16_t *id);
    facedb_status_t (*genId)(uint16_t *new_id);
    facedb_status_t (*getIds)(uint16_t *face_ids);
    bool (*getSaveStatus)(uint16_t id);
    int (*getFaceCount)(void);
    char *(*getNameWithId)(uint16_t id);
} facedb_ops_t;

extern const facedb_ops_t g_facedb_ops;
```

## User coffee information database usage
***g_coffedb_ops*** handles all kinds of user information database operation.

```c
ypedef enum _coffee_type
{
    Coffee_Espresso,
    Coffee_Americano,
    Coffee_Cappuccino,
    Caffee_Latte,
} coffee_type_t;

typedef enum _coffee_size
{
    Coffee_Small,
    Coffee_Medium,
    Coffee_Large,
} coffee_size_t;

typedef enum _coffee_strength
{
    Coffee_Soft,
    Coffee_Mild,
    Coffee_Strong,
} coffee_strength_t;

typedef struct _coffee_attribute
{
    uint16_t id;
    uint8_t type;
    uint8_t size;
    uint8_t strength;
    uint8_t reserved[16];
} coffee_attribute_t;

typedef struct _coffeedb_ops
{
    coffeedb_status_t (*init)(void);
    coffeedb_status_t (*deinit)(void);
    coffeedb_status_t (*addWithId)(uint16_t id, coffee_attribute_t *attr);
    coffeedb_status_t (*delWithId)(uint16_t id);
    coffeedb_status_t (*updWithId)(uint16_t id, coffee_attribute_t *attr);
    coffeedb_status_t (*getWithId)(uint16_t id, coffee_attribute_t *attr);
} coffeedb_ops_t;

extern const coffeedb_ops_t g_coffedb_ops;
```