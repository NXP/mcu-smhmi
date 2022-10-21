# Introduction

We created the template to demonstrate the Smart HMI application with LVGL GUI, Face Recognition and Far-Field Voice Recognition AI/ML algorithms integrated.

You can leverage this template to quickly build your own applications:

-- Create your fancy GUI with open-source LVGL library

-- Use the Face Recognition as the user identity

-- Use the Far-Field Voice Recognition as the touchless interface

# Build the LVGL GUI
LVGL is a free and open-source embedded graphic library with features that enable you to create embedded GUIs with intuitive graphical elements, beautiful visual effects and a low memory footprint.
The complete graphic framework includes a variety of widgets for you to use in the creation of your GUI, and supports more advanced functions such as animations and anti-aliasing.

To learn more about LVGL visit https://lvgl.io/

## Design and create the GUI with NXP's free GUI Guider tool
GUI Guider is a user-friendly graphical user interface development tool from NXP that enables the rapid development of high quality displays with the open-source LVGL graphics library. GUI Guider's drag-and-drop editor makes it easy to utilize the many features of LVGL such as widgets, animations and styles to create a GUI with minimal or no coding at all.

To learn more about GUI Guider visit https://www.nxp.com/design/software/development-software/gui-guider:GUI-GUIDER

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

We created the LVGL Display HAL device to handle the LVGL initialization and the GUI launch. The ``` "void setup_ui(lv_ui *ui)" ``` will be called in this HAL device, thus you just need to replace the "generated" folder with your GUI code in the CM4 project, and the whole UI be launched during the start up.

You can also refer the LVGL Display HAL device implementation for the Coffee Machine demo and Elevator demo as below:

-- Coffee Machine [hal_display_lvgl_coffeemachine.c](../../framework/hal/display/hal_display_lvgl_coffeemachine.c)

-- Elevator [hal_display_lvgl_elevator.c](../../framework/hal/display/hal_display_lvgl_elevator.c)

To learn more about Display HAL device refer to [display.md](../../framework/docs/hal_devices/display.md)

# Build the phoneme-based voice recognition model
We enabled the Far-Field Voice Recognition by phoneme-based Automatic Speech Recognition (ASR) engine. NXP partners with Cyberon for generating the phoneme-based voice engines.
Please refer our dedicate sections [dsmt_instructions.md](voice/dsmt_instructions.md) for how to build your phoneme-based voice engine.

We created the Voice Algorithm HAL device to handle the whole voice recognition.

Please also refer the Voice algorithm HAL device implementation for the Coffee Machine demo and Elevator demo as below:

-- Coffee Machine and Elevator Voice Algorithm HAL [hal_voice_algo_dsmt_asr.c](../../framework/hal/voice/hal_voice_algo_dsmt_asr.c)

-- Coffee Machine voice recognition models [local_voice](../../coffee_machine/cm7/local_voice/) folder

-- Coffee Machine voice recognition models [local_voice](../../elevator/cm7/local_voice) folder

The voice recognition is running in CM7 and the whole Voice algorithm HAL device and voice models are built into CM7 project.

# Bind the user's profile data with face recognition
The face recognition algorithm and face feature database have been well implemented. Basically you can just use them as the user identity for your application.

They are all running on the CM7 and built into CM7 project.

You can refer the implementation for the Coffee Machine demo and Elevator demo as below:

-- Face recognition algorithm for Coffee Machine [hal_vision_algo_oasis_coffeemachine.c](../../framework/hal/vision/hal_vision_algo_oasis_coffeemachine.c)

-- Face recognition algorithm for Elevator [hal_vision_algo_oasis_coffeemachine.c](../../framework/hal/vision/hal_vision_algo_oasis_coffeemachine.c)

-- Face feature database [hal_sln_facedb.c](../../framework/hal/vision/hal_sln_facedb.c)

We have implemented the framework flash APIs based on the little fs. You can just define the user's profile data structure and implement the user's profile database base on these well defined APIs.

You can refer the user's profile database implementation for the Coffee Machine demo and Elevator demo as below:

-- User's profile data base for Coffee Machine [hal_sln_coffeedb.c](../../coffee_machine/cm7/source/hal_sln_coffeedb.c)

-- User's profile data base for Elevator [hal_sln_elevatordb.c](../../elevator/cm7/source/hal_sln_elevatordb.c)

# Implement the use case flow for your application

We created the UI Output HAL device to handle the APP use case flow. It will control the face recognition HAL device, voice recognition HAL device and the LVGL UI. And the inference results from face recognition HAL device and voice recognition HAL device will also be posted into this output device.

To learn more about Output HAL device refer to [output.md](../../framework/docs/hal_devices/output.md)

You can refer the UI Output HAL device implementation for the Coffee Machine demo and Elevator demo as below:

-- Coffee Machine [hal_output_ui_coffee_machine.c](../../framework/hal/output/hal_output_ui_coffee_machine.c)

-- Elevator [hal_output_ui_elevator.c](../../framework/hal/output/hal_output_ui_elevator.c)
