# Naming Conventions

The framework code adheres to a set of naming conventions
for making the code easily readable and searchable using modern code completion tools.

```{note}
The naming conventions described below apply *only* to framework-related code that is primarily located in the`framework` folder, and `source`  folder of the application.
```

## Functions

Functions names follow the format of `{APP/FWK/HAL}\_{DevType}\_{DevName}_{Action}`.

**For example:**

```c title="HAL/common/hal_input_push_buttons.c"
hal_input_status_t HAL_InputDev_PushButtons_Start(const input_dev_t *dev);
```

To increase searchability using code completion tools
functions for each framework component have their own prefix denoting the component they relate to:

- `APP` - app-specific function. Usually device registration or event handler-related.
- `FWK` - framework-specific function. Usually framework API function.
- `HAL` - HAL-specific function. Usually HAL device operators.

Additionally,
an underscore `_` may be placed in front of a function name to indicate that the function is `static`/`private`.

```{note}
Static functions oftentimes exclude all but the underscore and the `Action` as the component, devType, and devName are implicit.
```

**For example:**

```c title="Static Function Example"
static shell_status_t _VersionCommand(shell_handle_t shellContextHandle, int32_t argc, char **argv);
static shell_status_t _ResetCommand(shell_handle_t shellContextHandle, int32_t argc, char **argv);
static shell_status_t _SaveCommand(shell_handle_t shellContextHandle, int32_t argc, char **argv);
static shell_status_t _AddCommand(shell_handle_t shellContextHandle, int32_t argc, char **argv);
static shell_status_t _DelCommand(shell_handle_t shellContextHandle, int32_t argc, char **argv);
```

Following one of the above prefixes is the device type of the device defining the function.

- `InputDev`
- `OutputDev`
- `CameraDev`
- `DisplayDev`
- and so forth.

As the device type is the name of the device,
the name must match the name of the device specified in the filename.

**For example:**

```c title="HAL/common/hal_input_push_buttons.c"
hal_input_status_t HAL_InputDev_PushButtons_Start(const input_dev_t *dev);
```

The name of the device ends with the "action" performed on/by the device. It could be anything including `Start`, `Stop`, `Register`, etc.

Below are several examples of different function names.

```c title="APP Function Example" {0}
void APP_InputDev_Shell_RegisterShellCommands(shell_handle_t shellContextHandle,
                                              input_dev_t *shellDev,
                                              input_dev_callback_t callback)
{
    s_InputCallback            = callback;
    s_SourceShell              = shellDev;
    s_ShellHandle              = shellContextHandle;
    s_FrameworkRequest.respond = _FrameworkEventsHandler;
    SHELL_RegisterCommand(shellContextHandle, SHELL_COMMAND(version));
    SHELL_RegisterCommand(shellContextHandle, SHELL_COMMAND(reset));
    SHELL_RegisterCommand(shellContextHandle, SHELL_COMMAND(save));
    SHELL_RegisterCommand(shellContextHandle, SHELL_COMMAND(add));
```

```c title="FWK Function Example" {5}
int HAL_InputDev_PushButtons_Register()
{
    int error = 0;
    LOGD("input_dev_push_buttons_register");
    error = FWK_InputManager_DeviceRegister(&s_InputDev_PushButtons);
    return error;
}
```

```c title="HAL Function Example"
hal_input_status_t HAL_InputDev_PushButtons_Init(input_dev_t *dev, input_dev_callback_t callback);
hal_input_status_t HAL_InputDev_PushButtons_Deinit(const input_dev_t *dev);
hal_input_status_t HAL_InputDev_PushButtons_Start(const input_dev_t *dev);
hal_input_status_t HAL_InputDev_PushButtons_Stop(const input_dev_t *dev);
hal_input_status_t HAL_InputDev_PushButtons_InputNotify(const input_dev_t *dev, void *param);
```

## Variables

Local and global variables use `camelCase`.

```c title="Local Variable Example" {5,6}
static hal_output_status_t HAL_OutputDev_RgbLed_InferComplete(const output_dev_t *dev,
                                                              output_algo_source_t source,
                                                              void *inferResult)
{
    vision_algo_result_t *visionAlgoResult = (vision_algo_result_t *)inferResult;
    hal_output_status_t error              = kStatus_HAL_OutputSuccess;
```

Static variables are prefixed with `s_PascalCase`

**For example:**

```c title="Static Variable Example"
static event_common_t s_CommonEvent;
static event_face_rec_t s_FaceRecEvent;
static event_recording_t s_RecordingEvent;
static input_event_t s_InputEvent;
static framework_request_t s_FrameworkRequest;
static input_dev_callback_t s_InputCallback;
static input_dev_t *s_SourceShell; /* Shell device that commands are sent over */
static shell_handle_t s_ShellHandle;
```

## Typedefs

Type definitions are written in `snake_case` and end in `_t`.

**For example:**

```c title="Typedef Example"
typedef struct
{
    fwk_task_t task;
    input_task_data_t inputData;
} input_task_t;
```

## Enums

Enumerations are written in the the form `kEventType_State`.

**For example:**

```c title="Enumeration Example"
typedef enum _rgb_led_color
{
    kRGBLedColor_Red,    /*!< LED Red Color */
    kRGBLedColor_Orange, /*!< LED Orange Color */
    kRGBLedColor_Yellow, /*!< LED Yellow Color */
    kRGBLedColor_Green,  /*!< LED Green Color */
    kRGBLedColor_Blue,   /*!< LED Blue Color */
    kRGBLedColor_Purple, /*!< LED Purple Color */
    kRGBLedColor_Cyan,   /*!< LED Cyan Color */
    kRGBLedColor_White,  /*!< LED White Color */
    kRGBLedColor_Off,    /*!< LED Off */
} rgbLedColor_t;
```

Enumerations for a status specifically must be written in the form `kStatus_{Component}_{State}`.

**For example:**

```c Enumeration title="Status Example"
/*! @brief Error codes for input hal devices */
typedef enum _hal_input_status
{
    kStatus_HAL_InputSuccess = 0,                                                      /*!< Successfully */
    kStatus_HAL_InputError   = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Input, 1), /*!< Error occurs */
} hal_input_status_t;
```

## Macros and Defines

Defines are written in all caps.

**For example:**

```c title="HAL/common/hal_input_push_buttons.c"
#define INPUT_DEV_PB_WAKE_GPIO        BOARD_USER_BUTTON_GPIO
#define INPUT_DEV_PB_WAKE_GPIO_PIN    BOARD_USER_BUTTON_GPIO_PIN
#define INPUT_DEV_SW1_GPIO            BOARD_BUTTON_SW1_GPIO
#define INPUT_DEV_SW1_GPIO_PIN        BOARD_BUTTON_SW1_PIN
#define INPUT_DEV_SW2_GPIO            BOARD_BUTTON_SW2_GPIO
#define INPUT_DEV_SW2_GPIO_PIN        BOARD_BUTTON_SW2_PIN
#define INPUT_DEV_SW3_GPIO            BOARD_BUTTON_SW3_GPIO
#define INPUT_DEV_SW3_GPIO_PIN        BOARD_BUTTON_SW3_PIN
#define INPUT_DEV_PUSH_BUTTONS_IRQ    GPIO13_Combined_0_31_IRQn
#define INPUT_DEV_PUSH_BUTTON_SW1_IRQ BOARD_BUTTON_SW1_IRQ
#define INPUT_DEV_PUSH_BUTTON_SW2_IRQ BOARD_BUTTON_SW2_IRQ
#define INPUT_DEV_PUSH_BUTTON_SW3_IRQ BOARD_BUTTON_SW3_IRQ
```
