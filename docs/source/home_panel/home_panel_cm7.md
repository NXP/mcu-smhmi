# Smart panel CM7

This Smart panel CM7 host project runs on the CM7 core.

It is linked to flash with the combination of the CM4 project.

The CM7 was designed to focus on the vision, gesture and voice algorithms' processing to get the best performance.

## Main functionalities
- Vision algorithm
- Gesture algorithm
- Voice algorithm
- Audio playback
- Microphone stream input
- Multicore communication
- Littlefs format filesystem

## Boot sequence
The "main" entry of this project is located in the "home_panel/cm7/source/sln_smart_tlhmi_cm7.cpp" file.
The basic boot up flow is:
- Initialize board level hardware
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

## Board level initialization
The board-level initialization is implemented in the "APP_BoardInit()" entry which is located in "home_panel/cm7/source/sln_smart_tlhmi_cm7.cpp".
Below is the main flow:
- Relocate vector table into RAM
- Configure MPU, Clock, and Pins
- Debug console with hardware semaphore initialization
- System time stamp start
- Config Flexio camera DMA
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

    BOARD_FlexioCameraConfigDMA();

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
## Framework managers
The below framework managers are enabled in the cm7 side with the following priorities:
- Vision algorithm manager - P3
- Voice algorithm manager - P3
- Audio processing manager - P2
- Input manager - P1
- Output manager - P3
- Camera manager - P2
- Multicore manager - P0
- Flash device manager

Where P0 is the highest priority and P4 is the least prioritized.
```{note}
Choosing the right priority for the manager is something that must be addressed based on the requirements. 
Our recommendation is to keep Vision manager equal to or less than Voice manager, or the audio sample can be lost.
```
Refer to the framework chapter for a detailed description of these framework managers.

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
    FWK_MANAGER_INIT(OutputManager, ret);
    FWK_MANAGER_INIT(AudioProcessing, ret);
    FWK_MANAGER_INIT(InputManager, ret);
    FWK_MANAGER_INIT(VoiceAlgoManager, ret);
    FWK_MANAGER_INIT(CameraManager, ret);

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
    FWK_MANAGER_START(CameraManager, CAMERA_MANAGER_TASK_PRIORITY, ret);
#if defined(ENABLE_MASTER) && ENABLE_MASTER
    FWK_MANAGER_START(MulticoreManager, MULTICORE_MANAGER_TASK_PRIORITY, ret);
#endif /* defined(ENABLE_MASTER) && ENABLE_MASTER */

    return ret;
}
```

## Framework HAL devices
The enabled HAL devices are configured in the "home_panel/cm7/board/board_define.h" file as shown below:

```c
#define ENABLE_INPUT_DEV_PdmMic
#define ENABLE_AUDIO_PROCESSING_DEV_Afe
#define ENABLE_VIT_ASR
#define ENABLE_OUTPUT_DEV_MqsStreamerAudio
#define ENABLE_OUTPUT_DEV_SmartTlhmiConfig
#define ENABLE_VISIONALGO_DEV_Oasis_HomePanel
#define ENABLE_VISIONALGO_DEV_Uvita_Gesture
#define ENABLE_FLASH_DEV_Littlefs
#define ENABLE_FACEDB
#define ENABLE_CAMERA_DEV_FlexioGc0308
#if defined(ENABLE_MASTER) && ENABLE_MASTER
#define ENABLE_MULTICORE_DEV_MessageBuffer
#endif /* defined(ENABLE_MASTER) && ENABLE_MASTER */
```

The registration of the enabled HAL devices is implemented in the `APP_RegisterHalDevices(...)` function which is located in ""home_panel/cm7/source/sln_smart_tlhmi_cm7.cpp":

```{note}
The `APP_RegisterHalDevices(...)` must be called after the framework initialization `APP_InitFramework(...)` and before framework startup `APP_StartFramework(...)`.
```

```c
int APP_RegisterHalDevices(void)
{
    int ret = 0;

    HAL_OUTPUT_DEV_REGISTER(MqsAudio_Streamer, ret);
    HAL_AUDIO_PROCESSING_DEV_REGISTER(Afe, ret);
    HAL_INPUT_DEV_REGISTER(PdmMic, ret);

    HAL_VOICEALGO_DEV_REGISTER(Asr_VIT, ret);
    HAL_CAMERA_DEV_REGISTER(FlexioGc0308, ret);
    HAL_VALGO_DEV_REGISTER(UvitaGestureRecognition, ret);
    HAL_VALGO_DEV_REGISTER(OasisHomePanel, ret);
#if defined(ENABLE_MASTER) && ENABLE_MASTER
    HAL_MULTICORE_DEV_REGISTER(MessageBuffer, ret);
#endif /* defined(ENABLE_MASTER) && ENABLE_MASTER */
    HAL_INPUT_DEV_REGISTER(WiFiAWAM510, ret);

    return ret;
}
```

## Logging
Both the CM7 and CM4 projects are leveraging the [FreeRTOS logging library](https://www.freertos.org/logging.html).

The FreeRTOS logging library code is located in the logging folder where you can find the detailed document located in "home_panel/cm7/aws_iot/amazon-freertos/libraries/logging/README.md".

The CM7 and CM4 share the low-level LPUART12 peripheral for the logging output. The hardware semaphore mechanism is used to guarantee the concurrence access of the LPUART12 peripheral. They share a low-level timer to get the unified timestamp of the logging information.

### Log Task Init
The application calls the `xLoggingTaskInitialize(...)` API to create the logging task in the `main()` entry of this project and is located in "home_panel/cm7/source/sln_smart_tlhmi_cm7.cpp":

```c
xLoggingTaskInitialize(LOGGING_TASK_STACK_SIZE, LOGGING_TASK_PRIORITY, LOGGING_QUEUE_LENGTH);
```

### Log Macros
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
### UART Hardware connection
The Smart panel application prints the log to the LPUART12 port. The tx/rx pins of LPUART12 are connected to J202, at the back of the board (pin3-RX and pin4-TX).
Connect the LPUART12 tx/rx pins to a TTL USB serial adapter (LPUART12 rx pin to usb adapter tx pin, LPUART12 tx pin to usb adapter rx pin), and connect the TTL USB serial adapter to a USB port on the host machine (Windows or Linux based).

### Get UART log from Windows Host
For a Windows host system, find the corresponding COM port number from the Windows "Device Manager" for the newly connected TTL USB serial adapter. Using terminal emulator program (f.e putty) to connect to the COM port, setting the speed to 115200Hz.

### Get UART log from Linux host
For a Linux host system, Connect to the serial port (usually /dev/ttyUSBx) using a Linux terminal emulator program (f.e minicom), use 115200 8N1 as the serial port setting for the serial communication.

## Smart panel database
The Smart panel application uses framework flash operations wih low-level littlefs file system to store persistent user information. There are two kinds of users in the Smart panel application, the thermostat users (including the recognized user-faces database and user preferred thermostat setting database) and the security users (only includign the user-faces database) . The detailed usage API is located in files "framework/hal/vision/hal_sln_facedb.h" and "home_panel/cm7/source/hal_sln_thermostatdb.h". The face database and user thermostat setting information database entry are bound together using user id. The user id is a unique identifier on one device. The security user database API is in "home_panel/cm7/source/hal_sln_security_facedb.h",  which only stores the user face data.

To make it easier for users to add their database with personal attributes, we split the face database from user database. The user should create something similar with hal_sln_thermostatdb.h and add attributes like in the thermostat_attribute_t structure.

### Face recognize databse usage
***g_facedb_ops*** handles all kinds of thermostat user face database operation.
***g_security_facedb_ops*** handles all kinds of thermostat user face database operation.
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
extern const facedb_ops_t g_security_facedb_ops;
```

### User Thermostat setting database usage
***g_thermostatdb_ops*** handles all kinds of thermostat user information database operation.

```c
typedef enum _thermostatdb_status
{
    kThermostatDBStatus_Success,
    kThermostatDBStatus_Failed,
    kThermostatDBStatus_MallocFail,
    kThermostatDBStatus_MetaDataFail,
    kThermostatDBStatus_DbLoadFail,
    kThermostatDBStatus_LockFail
} thermostatdb_status_t;

typedef enum _thermostat_mode
{
    kThermostat_Auto,
    kThermostat_Cold,
    kThermostat_Heat,
} thermostat_mode_t;

typedef enum _thermostat_speed
{
    kThermostatFan_On,
    kThermostatFan_Off,
    kThermostatFan_Low,
    kThermostatFan_Med,
    kThermostatFan_High,
} thermostat_fan_speed_t;
typedef struct _thermostat_attribute
{
    uint16_t id;
    uint8_t mode;
    uint8_t fan_speed;
    uint8_t temperature[TEMPERATURE_VALUE_SIZE];
    uint8_t reserved[16];
} thermostat_attribute_t;

typedef struct _thermostatdb_ops
{
    thermostatdb_status_t (*init)(void);
    thermostatdb_status_t (*deinit)(void);
    thermostatdb_status_t (*addWithId)(uint16_t id, thermostat_attribute_t *attr);
    thermostatdb_status_t (*delWithId)(uint16_t id);
    thermostatdb_status_t (*updWithId)(uint16_t id, thermostat_attribute_t *attr);
    thermostatdb_status_t (*getWithId)(uint16_t id, thermostat_attribute_t *attr);
} thermostatdb_ops_t;

extern const thermostatdb_ops_t g_thermostatdb_ops;
```
