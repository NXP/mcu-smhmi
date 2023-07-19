# Introduction

We created the template to demonstrate the Smart HMI application with multi-core, LVGL GUI, Face Recognition, Gesture Recognition and Far-Field Voice Recognition AI/ML algorithms integrated.

You can leverage this template to quickly build your own applications:

-- Implement multi-core cooperation for higher performance

-- Create your fancy GUI with open-source LVGL library

-- Use the Face Recognition as the user identity

-- Use the Gesture Recognition as the touchless interface

-- Use the Far-Field Voice Recognition as the touchless interface

# Build the LVGL GUI

LVGL is a free and open-source embedded graphic library with features that enable you to create embedded GUIs with intuitive graphical elements, beautiful visual effects and a low memory footprint.
The complete graphic framework includes a variety of widgets for you to use in the creation of your GUI, and supports more advanced functions such as animations and anti-aliasing.

To learn more about LVGL visit https://lvgl.io/

## Design and create the GUI with NXP's free GUI Guider tool

GUI Guider is a user-friendly graphical user interface development tool from NXP that enables the rapid development of high quality displays with the open-source LVGL graphics library. GUI Guider's drag-and-drop editor makes it easy to utilize the many features of LVGL such as widgets, animations and styles to create a GUI with minimal or no coding at all.

To learn more about GUI Guider visit <https://www.nxp.com/design/software/development-software/gui-guider:GUI-GUIDER>

You can also refer our full GUI Guider project for Coffee Machine and Elevator demo as below:

-- Coffee Machine [TLHMI_Coffee_Demo_LGVLv8_Landscape.guiguider](../../coffee_machine/gui_guider/TLHMI_Coffee_Demo_LGVLv8_Landscape.guiguider)

-- Elevator [TLHMI_Elevator_Demo.guiguider] (../../elevator/gui_guider/TLHMI_Elevator_Demo.guiguider)

## Integrate your generated LVGL GUI code

The whole GUI code is running in the CM4 core and built into the CM4 project.

By default below function is the main entry of the whole LVGL GUI which is located in your generated GUI code [gui_guider.c](coffee_machine/cm4/generated/gui_guider.c)
```c
void setup_ui(lv_ui *ui)
{
    setup_scr_standby(ui);
    lv_scr_load(ui->standby);
}
```

We created the LVGL Display HAL device to handle the LVGL initialization and the GUI launch. The ```void setup_ui(lv_ui *ui)``` will be called in this HAL device, therefore you must replace the "generated" folder with your GUI code in the CM4 project, and the whole UI be launched during the start-up.

You can also refer the LVGL Display HAL device implementation for the Coffee Machine demo and Elevator demo as below:

-- Coffee Machine [hal_display_lvgl_coffeemachine.c](../../framework/hal/display/hal_display_lvgl_coffeemachine.c)

-- Elevator [hal_display_lvgl_elevator.c](../../framework/hal/display/hal_display_lvgl_elevator.c)

To learn more about Display HAL device refer to [display.md](../../framework/docs/hal_devices/display.md)

Remark: There is an application note to introduce the detailed steps to integrate the LVGL GUI codes to the smart HMI platform. To refer to it, may check the "Documentation" tab on the below official website page: 
https://www.nxp.com/design/designs/nxp-edgeready-smart-hmi-solution-based-on-i-mx-rt117h-with-ml-vision-voice-and-graphical-ui:SLN-TLHMI-IOT-RD (TODO :DITA modify URL)

# Build the phoneme-based voice recognition model

We enabled the Far-Field Voice Recognition by phoneme-based Automatic Speech Recognition (ASR) engine. NXP provides two inference engines(VIT/DSMT) for ASR.

We created two Voice Algorithm HAL devices to handle the whole voice recognition based on VIT and DSMT respectively.

Refer VIT Voice Algorithm HAL device implementation [hal_voice_algo_vit_asr.c](../../framework/hal/voice/hal_voice_algo_vit_asr.c)

Refer DSMT Voice Algorithm HAL device implementation [hal_voice_algo_dsmt_asr.c](../../framework/hal/voice/hal_voice_algo_dsmt_asr.c)

By default, Coffee Machine and Elevator Apps are built with DSMT Voice Algorithm HAL device and Smart Panel App is built with VIT Voice Algorithm HAL device.

-- Coffee Machine voice recognition models [local_voice](../../coffee_machine/cm7/local_voice/) folder

-- Elevator voice recognition models [local_voice](../../elevator/cm7/local_voice) folder

-- Smart Panel voice recognition models [local_voice](../../home_panel/cm7/local_voice/oob_demo_vit/) folder

VIT is total NXP intellectual property. We can get its basic knowledge from [VIT main page](https://www.nxp.com/design/software/embedded-software/voice-intelligent-technology:VOICE-INTELLIGENT-TECHNOLOGY). How to build your VIT phoneme-based voice engine? Please refer sub-section [vit_instructions.md](voice/vit_instructions.md).

DSMT is provided by NXP partner Cyberon. How to build your DSMT phoneme-based voice engine? Please refer sub-section [dsmt_instructions.md](voice/dsmt_instructions.md)

## Voice recognition flow

Refer the Smart Panel App implementation. VIT inference engine recognizes voice command based on VIT model.
`VIT_Process` returns the detection status by `VIT_DetectionResults`.

```c
   VIT_Status = VIT_Process(VITHandle, cleanSound, &VIT_DetectionResults); 
```
Detection status is defined as following:

```c
/* VIT Detection Status
 *    Status returned by VIT_Process() API.
 *    Indicates if the Wake Word or a Voice Command has been detected on the frame processed
 */
typedef enum
{
    VIT_NO_DETECTION    = 0,               // Nothing detected
    VIT_WW_DETECTED     = 1,               // WakeWord Detected
    VIT_VC_DETECTED     = 2,               // a Voice Command Detected
    VIT_DUMMY_DETECTION = PL_MAXENUM
}VIT_DetectionStatus_en;
```

Based on the value of VIT_DetectionResults, further check Wake Word of Command information.

```c
    if (VIT_DetectionResults == VIT_WW_DETECTED)
    {
        VIT_Status = VIT_GetWakeWordFound(VITHandle, &s_WakeWord);
        ...
    }
    else if (VIT_DetectionResults == VIT_VC_DETECTED)
    {
        /* Retrieve id of the Voice Command detected
            String of the Command can also be retrieved (when WW and CMDs strings are integrated in Model) */
        VIT_Status = VIT_GetVoiceCommandFound(VITHandle, &s_VoiceCommand);
        ...
```

if  voice recognition result is true even confirmed by `confirmDetectedCommand`, not triggered by Residual noise from board speaker, the recognition result will be sent to output device by ` static void voice_algo_asr_result_notify(asr_inference_result_t *result, uint32_t utteranceLength)`.

if the vaule of utteranceLength is not 0, there is another message created for AFE device to calibrate AFE alogrithm. 

After output device receives voice recognition result, `_InferComplete_Voice` will handle all voice command.

```c
static hal_output_status_t _InferComplete_Voice(const output_dev_t *dev,
                                                void *inferResult,
                                                home_panel_screen_id_t currentScreenId)
{
    hal_output_status_t error               = kStatus_HAL_OutputSuccess;
    asr_inference_result_t *voiceAlgoResult = (asr_inference_result_t *)inferResult;
    LOGD("[UI] Screen:%d voice command status:%d  cmd:%d", currentScreenId, voiceAlgoResult->status,
         voiceAlgoResult->keywordID);
         ...
```
Customer also can customize the UI actions based on different product design. The field of keywordID in `asr_inference_result_t` is used to store command ID.

```c
typedef struct _asr_inference_result
{
    asr_voice_detect_status_t status;
    asr_language_t language;
    int32_t keywordID;
    asr_inference_t demo;
} asr_inference_result_t;
```

The voice inference engine is running in CM7 and the whole Voice algorithm HAL device and voice models are built into CM7 project.

# face recognition and database operations

The face recognition algorithm and database operations are both implemented in the framework.

There are two kinds of databases used in the application:
- The face feature database is used to store the user's face feature data when recognized by the face recognition algorithm
- The user's profile database is used to store the user's application specific data (f.e user's coffect selection in the coffee machine demo).

When a user is recognized, a unique face_id will be generated, and it can be used as the search key to the face feature database and user's profile database.

The face recognition algorithm and database operations are both running on the CM7 and built into CM7 project.

You can refer to the implementation for the Coffee Machine demo, Elevator demo and the Smart Panel demo as below:

- Face recognition algorithm for Coffee Machine [hal_vision_algo_oasis_coffeemachine.c](../../framework/hal/vision/hal_vision_algo_oasis_coffeemachine.c)
- Face recognition algorithm for Elevator [hal_vision_algo_oasis_coffeemachine.c](../../framework/hal/vision/hal_vision_algo_oasis_coffeemachine.c)
- Face recognition algorithm for Elevator [hal_vision_algo_oasis_homepanel.c](../../framework/hal/vision/hal_vision_algo_oasis_homepanel.c)

- Face feature database [hal_sln_facedb.c](../../framework/hal/vision/hal_sln_facedb.c)

We have implemented the framework flash APIs based on the little fs. You can define the user's profile data structure and implement the user's profile database base on these well-defined APIs.

You can refer to the user's profile database implementation as below:

- User's profile data base for Coffee Machine [hal_sln_coffeedb.c](../../coffee_machine/cm7/source/hal_sln_coffeedb.c)
- User's profile data base for Elevator [hal_sln_elevatordb.c](../../elevator/cm7/source/hal_sln_elevatordb.c)
- User's thermostat profile data base for HomePanel [hal_sln_thermostatdb.c](../../home_panel/cm7/source/hal_sln_thermostatdb.c)

## Implement user case flow with face recognition results
The face recognition algorithm and database operations are runing on the CM7 core (f.e. [hal_vision_algo_oasis_coffeemachine.c](../../framework/hal/vision/hal_vision_algo_coffee_machine.c)), it is controlled by the UI HAL output device (f.e. [hal_output_ui_coffee_machine.c](../../framework/hal/output/hal_output_ui_coffee_machine.c)), which runs on the CM4 CORE.

### start / stop the face recognition algorithm
The UI hal output device controls the start / stop of the face recognition algorithm according to the application UI flow logic, f.e. when the application enters into the user register screen trying to register a new user.
```c
uint8_t UI_EnterScreen_Callback(screen_t screenId)
{
    switch (screenId)
    {
        case kScreen_Home:
        {
            ...

            _StopFaceRec(0);
        }
        ...
        case kScreen_Standby:
        {
            _StopFaceRec(1);
        }
    }
}

```
`_StopFaceRec` will send a event_face_rec_t to the face recognition algorithm, which starts/stops the face recognition algorithm.
```c

static void _StopFaceRec(int stop)
{
    static event_face_rec_t s_FaceRecEvent;
    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_VisionAlgoInputNotify;
    output_event.data      = &s_FaceRecEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_FaceRecEvent);
    output_event.eventInfo = kEventInfo_Remote;

    // notify the face rec to start
    s_FaceRecEvent.eventBase.eventId = kEventFaceRecID_OasisSetState;

    if (stop)
    {
        s_FaceRecEvent.oasisState.state = kOASISLiteState_Stopped;
    }
    else
    {
        s_FaceRecEvent.oasisState.state = kOASISLiteState_Running;
    }
    uint8_t fromISR = __get_IPSR();
    s_OutputDev_UiCoffeeMachine.cap.callback(s_OutputDev_UiCoffeeMachine.id, output_event, fromISR);
}
```
When the face recognition algorithm receives the event, it will start / stop the face recognition processing accordingly. When starting the face recognition process, it will request camera frames to do the recognition.
```c
static hal_valgo_status_t HAL_VisionAlgoDev_OasisCoffeeMachine_InputNotify(const vision_algo_dev_t *receiver,
                                                                           void *data)
{
    switch (eventBase.eventId)
    {
        ...
        case kEventFaceRecID_OasisSetState:
        {
            if (start)
            {
                _oasis_start();
            }
            else if (stop)
            {
                _oasis_stop();
            }
        }
    }

}
```
When receiving a camera frame, the face recognition algorithm will do the recognition, and it will report the result back to the UI HAL output device.
```c
static hal_valgo_status_t HAL_VisionAlgoDev_OasisCoffeeMachine_Run(const vision_algo_dev_t *dev, void *data)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;

    if (s_OasisCoffeeMachine.currRunFlag != OASIS_RUN_FLAG_NUM &&
        s_OasisCoffeeMachine.currRunFlag != OASIS_RUN_FLAG_STOP)
    {
        ...

        int oasis_ret = OASISLT_run_extend(s_OasisCoffeeMachine.pframes, s_OasisCoffeeMachine.currRunFlag,
                                           s_OasisCoffeeMachine.config.minFace, &s_OasisCoffeeMachine);
        ...
        /* Take decision regarding the inference results */
        _process_inference_result(&s_OasisCoffeeMachine);
    }
    ...
}
```
### handling the face recognition results
The UI HAL output device starts application specific logic when receiving a face recognition result. F.e. in the coffee machine demo, we ask the user whether to save his coffee selection to the database after we recognize a new user. Or if we recognize an already registered user, we update the coffee selection on the UI to his previous selection.
```c
static hal_output_status_t _InferComplete_Vision(const output_dev_t *dev, void *inferResult, screen_t currentScreenId)
{
    ...
    if ((pResult->face_recognized) && (pResult->face_id != INVALID_FACE_ID))
    {
        // store the user's selection
        coffee_result_t *pAttr = (coffee_result_t *)pResult->userData;
        s_Recognized           = 1;
        s_UserId               = pResult->face_id;
        s_UserCoffeeType       = pAttr->coffeeType;
        s_UserCoffeeSize       = pAttr->coffeeSize;
        s_UserCoffeeStrength   = pAttr->coffeeStrength;
        ...
        // update the UI to user's coffee selection
        gui_home_set_language(_ConvertASRLanguageToUILanguage(s_UserLanguage));
        gui_set_home_coffee_type(s_UserCoffeeType);
        gui_set_home_coffee_size(s_UserCoffeeSize);
        gui_set_home_coffee_strength(s_UserCoffeeStrength);
    }
    ...
}
```
## Implement the user's profile database with face recognition
The user profile database is a separate database (besides the face feature database), it is used to store application specific data for a user. We use the face_id as the key to the user profile database, which is generated after the face recognition algorithm recognizes a new user.
### define user profile data structure and database ops
For the coffee machine demo, we user the following data structure to remember a user's coffee selection [hal_sln_coffeedb.h](../../coffee_machine/cm7/source/hal_sln_coffeedb.h).
The detailed implementation of the database API is in [hal_sln_coffeedb.c](../../coffee_machine/cm7/source/hal_sln_coffeedb.c).
```c
typedef struct _coffee_attribute
{
    uint16_t id;
    uint8_t type;
    uint8_t size;
    uint8_t strength;
    uint8_t language;
    uint8_t reserved[15];
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
### save user profile data into the database
When the face recognition algorithm detects a new user, the UI output HAL device decides when to save the user's face feature data and the user profile data after receiving the face recognition result. F.e. in coffee machine demo, after the user confirms to save the coffee selection. We will send a event with the user's coffee selection to the face recognition algorithm.
```c
static hal_output_status_t _InferComplete_Voice(const output_dev_t *dev, void *inferResult, screen_t currentScreenId)
{
    ...
        case (VOICE_CMD_CONFIRM):
        {
           if (s_IsWaitingRegisterSelection)
            {
                coffee_type_t curType         = get_coffee_type();
                coffee_size_t curSize         = get_coffee_size();
                coffee_strength_t curStrength = get_coffee_strength();
                asr_language_t language       = _ConvertUILanguageToASRLanguage(get_language());
                _RegisterCoffeeSelection(curType, curSize, curStrength, language);
            }
        }
}

static void _RegisterCoffeeSelection(coffee_type_t type,
                                     coffee_size_t size,
                                     coffee_strength_t strength,
                                     asr_language_t language)
{
    static event_smart_tlhmi_t s_TlhmiEvent;
    LOGD("[UI] Register user:%d coffee selection %d:%d:%d:%d", s_UserId, type, size, strength, language);

    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_VisionAlgoInputNotify;
    output_event.data      = &s_TlhmiEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_TlhmiEvent);
    output_event.eventInfo = kEventInfo_Remote;

    s_TlhmiEvent.eventBase.eventId                            = kEventFaceRecId_RegisterCoffeeSelection;
    s_TlhmiEvent.eventBase.eventInfo                          = kEventInfo_Remote;
    s_TlhmiEvent.regCoffeeSelection.id                        = s_UserId;
    s_TlhmiEvent.regCoffeeSelection.coffeeInfo.coffeeType     = type;
    s_TlhmiEvent.regCoffeeSelection.coffeeInfo.coffeeSize     = size;
    s_TlhmiEvent.regCoffeeSelection.coffeeInfo.coffeeStrength = strength;
    s_TlhmiEvent.regCoffeeSelection.coffeeInfo.language       = language;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_UiCoffeeMachine.cap.callback(s_OutputDev_UiCoffeeMachine.id, output_event, fromISR);
}
```
When the face recognition algorithm receives the event, it will save the user face feature data and the user profile data, using the face_id as the key for both databases.
```c
static hal_valgo_status_t HAL_VisionAlgoDev_OasisCoffeeMachine_InputNotify(const vision_algo_dev_t *receiver, void *data)
{
    ...
    case kEventFaceRecId_RegisterCoffeeSelection:
    {
         s_pFacedbOps->addFace(s_faceId, NULL, s_pFaceFeature, OASISLT_getFaceItemSize());
         s_pCoffeedbOps->addWithId(s_faceId, &attr);
    }
    ...
}
```

# Implement the use case flow with gesture recognition results

The gesture recognition is only used in Smart Panel APP. The use case flow is handled in the UI Output HAL device implemented in the file hal_output_ui_home_panel.c. The basic process of the use case flow for gesture recoginition is:

- Start the gesture control by enabing it with calling the API _EnableGestureControl(true)

- Trigger the user's use cases with the inference results of gesture recognition model

- End the gesture control by disabling it with calling _EnableGestureControl(false)

The inference result related information is defined as a data struct for the use case flow. See below:

```c
typedef struct
{
    bool has_hand;                            /**< whether the hand is detected or not; */
    DetBox_t box;                             /**< hand location: left, top, right, bottom. */
    float conf;                               /**< hand confidence. */
    hand_lr left_right;                       /**< left hand or right hand. */
    gesture_type gtype;                       /**< gesture type. */
    float handmark[HAND_LANDMARK_NUMBER * 3]; /**< hand landmark location: x1, y1, z1, x2, y2, z2... */
} uvita_gesture_out;
```

The process to trigger the use cases with the results from the gesture algorithm model is implemented in the function _InferComplete_VisionAudioPlayer():

- Check whether the hand is detected firstly. 
For example:
```c
 if (pHandResult->hand.has_hand == true)
```
- If detected, trigger the differnt use cases according to the gesture type, such as fist, palm and so on. 
For example:
```c
if (pHandResult->hand.gtype == GESTURE_FIST)
{
    //implement your use case
}
```
The user can implement his APP use case on the corresponding gesture type.
For more details, check the file [hal_output_ui_home_panel.c](../../framework/hal/output/hal_output_ui_home_panel.c). 

# Implement multi-core communication

For better performance, different tasks are running on m4 and m7 core. Both cores need to communicate with each other for cooperation. Based on framework design, both cores communicate via events covering different framework managers and HAL devices. On Smart HMI platform, the multi-core communication mechanism has been built and there has implemented much multi-core communications with much defined events in the APPs. A user can implement his multi-core communication in his application. Below is the guidance for it.

The event type indicating multi-core communication is defined in the [fwk_common.h](../../framework/inc/fwk_common.h):

```c
typedef enum _event_info
{
    kEventInfo_DualCore = 0, /* default */
    kEventInfo_Local,
    kEventInfo_Remote,
    kEventInfo_Invalid
} event_info_t;
```
The default value kEventInfo_DualCore means the event will send to both cores M4 and M7. kEventInfo_Local means the local core and kEventInfo_Remote means the other core. Usually, kEventInfo_Remote is used for multi-core communication in APP.

There could be a little difference from the event definitions on the different HAL devices for multi-core communication. But they have common control information, e.g. the event struct is defined for output device as below:

```c
typedef struct _output_event
{
    /* Eventid from the list above.*/
    output_event_id_t eventId;
    event_info_t eventInfo;
    /* Pointer to a struct of data that needs to be forwarded. */
    void *data;
    /* Size of the struct that needs to be forwarded. */
    unsigned int size;
    /* If copy is set to 1, the framework will forward a copy of the data. */
    unsigned char copy;
} output_event_t;
```

Below is a simple and typical example from Elevator APP to show how to send an event on the output device for multi-core communication. The function of the example codes is that the M4 core is notifying the output device on M7 core to stop playing the prompt.

```c
void StopPrompt(void)
{
    static event_common_t s_StopPromptEvent;
    LOGD("[UI] Stop prompt");

    output_event_t output_event = {0};

    output_event.eventId   = kOutputEvent_OutputInputNotify;
    output_event.data      = &s_StopPromptEvent;
    output_event.copy      = 1;
    output_event.size      = sizeof(s_StopPromptEvent);
    output_event.eventInfo = kEventInfo_Remote;

    s_StopPromptEvent.eventBase.eventId = kEventID_StopPrompt;

    uint8_t fromISR = __get_IPSR();
    s_OutputDev_UiElevator.cap.callback(s_OutputDev_UiElevator.id, output_event, fromISR);
}
```

In the above codes, the eventID value indicates the event case handled by the output device. The "data" is the information that the specific event required to be handled, here is to stop prompt. The eventInfo value indicates that the event needs to send to the other core, that is multi-core communication.

Finally, a new eventID value could be required for the user case. Suggest to define it in the head file - smart_tlhmi_event_descriptor.h under the path "source/event_handlers/" of each APP for the application layer, e.g. the eventID definitions for smart Home which is in [smart_tlhmi_event_descriptor.h](../../home_panel/cm4/source/event_handlers/smart_tlhmi_event_descriptor.h):

```c
typedef enum _event_smart_tlhmi_id
{
    kEventID_HomePanelType = kEventType_App,
    kEventID_SetKeyboardOverlay,
    kEventID_GetPreviewMode,
    kEventID_SetPreviewMode,
    kEventID_MediaPlayer_TrackInfo,
    kEventID_MediaPlayer_TrackPosition,
    kEventID_MediaPlayerInfo,
    kEventFaceRecId_RegisterSecuirtyUser,
    kEventFaceRecId_RespondAddUser,
    kEventFaceRecId_RespondUpdateUser,
    kEventFaceRecId_RespondAddSecurityUser,
    kEventFaceRecId_RespondUpdateSecurityUser,
    kEventFaceRecId_DelSecurityUser,
    kEventID_SmartTLHMIID_COUNT
} event_smart_tlhmi_id_t;
```
