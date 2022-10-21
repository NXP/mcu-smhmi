# Introduction


This repository contains all the source code for the coffee machine demo which belongs to Smart TLHMI solution.  The coffee machine demo is based on dual core (CM7 and CM4) architecture, thus it includes the CM7 master project which is under the cm7 folder and the CM4 slave project which is under the cm4 folder.

## How to flash the firmware and resource binary

#### 1. Flash the resource binary

The LVGL image and the audio prompts have been split from the firmware, you need to flash the resource into the dedicate flash region before you flash the firmware:

Flash the "resource\coffee_machine_resource.bin" into flash region 0x30700000

**Notice**: the resource binary only needed to be flashed once. Also if you do an erase operation, you just need to re-flash the resource once again.

#### 2. Build and flash the firmware binary
Refer below "How to build the projects" section to build and flash the firmware.

## How to build the projects

#### Build Environment:
 ```c
MCUXpresso IDE: v11.5.0
SDK version:  MIMXRT1170-EVK 2.11.0
 ```

Currently we have one issue(https://jira.sw.nxp.com/browse/MCUX-47922) in the latest MCUXpresso IDE v11.5.0, MCUXpresso IDE team is debugging in progress and will fix this issue with new MCUXpresso IDE release.
We need to add below workaround manually to make this project running successfully:

#### 1. Build the cm4 project from the clean code base

   1. Enable the "Manage link script" in the Setting->MCU C++ Linker->Managed Linker Script temporally.
   2. Build
   3. Disable the "Manage link script" in the Setting->MCU C++ Linker->Managed Linker Script again

#### 2. Manually update the link file
Manually update the ".ARM.exidx" section of the CM4 slave link file "[Debug | Release]\sln_smart_tlhmi_cm4_[Debug|Release].ld" as below:

   ```c
    .ARM.exidx : ALIGN(0x20)
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        . += 24;
        __exidx_end = .;
    } > BOARD_SDRAM

   ```

#### 3.  Clean and build the CM4 project again
Do the "lean" and build for the [Debug|Release] configuration of the CM4 again as the "[Debug\Relese]\sln_smart_tlhmi_cm4_[Debug|Release].ld" has been manually updated.

#### 4. Clean and build the CM7 project
Do the clean" and build for the [Debug|Release] configuration of the CM4 project to genearate the final firmware which included both the CM4 and CM4

#### 5. Launch "Debug" of CM7
Launch the debug of the CM7 project to flash the firmware into your TLHMI board and start the debug. The CM4 firmware will be automatically loaded into SDRAM and be launched.


## How to update lvgl/vglite library
By default, the debug configuration uses a compiled library binary for lvgl+vglite b/c compilation + creating makefiles for that code heavily impacts how quickly changes can be tested.


## Hardware and software change for the first version TLHMI board

#### 1. Switch to use "MT9M114" RGB camera
The default software is configured to use the "MT9M114" camera, you need to do below hardware change:

1. Set the j3, j5 to position 2-3 on LCD PCB
2. replace the GC0308 RGB camera with MT9M114 RGB camera(right camera slot on the board)

#### 2. Use the default "GC0308" RGB camera
The first version TLHMI board has the "GC0308" RGB camera by default, if you don't have the "MT9M114" camera in hand, you need to do below hardware and software updates to enable the "GC0308" RGB camera and display.

**Notice**:
The sensor of the default "GC0308" RGB camera is physically rotated CW 90 degree(portrait mode), however our UI and Face recognition algorithm need the landscape camera frame, so you need to rotate your board CCW 90 degree to make the face recognition algorithm running properly.

**Hardware:**

1. Set the j3, j5 to position 2-3 on LCD PCB

**Software:**

You need to switch back to "GC0308" RGB camera with below code update in the CM4 project:

1.  sln_smart_tlhmi/cm4/board/board_define.h
```c
diff --git a/cm4/board/board_define.h b/cm4/board/board_define.h
index 622b445..b8f3bce 100644
--- a/cm4/board/board_define.h
+++ b/cm4/board/board_define.h
@@ -28,9 +28,9 @@
  */
 #define ENABLE_GFX_DEV_Pxp
 #define ENABLE_DISPLAY_DEV_LVGLCoffeeMachine
-//#define ENABLE_CAMERA_DEV_CsiGc0308
+#define ENABLE_CAMERA_DEV_CsiGc0308
 //#define ENABLE_CAMERA_DEV_FlexioGc0308
-#define ENABLE_CAMERA_DEV_CsiMt9m114
+//#define ENABLE_CAMERA_DEV_CsiMt9m114
```

2. sln_smart_tlhmi/cm4/source/sln_smart_tlhmi_cm4.cpp
```c
diff --git a/cm4/source/sln_smart_tlhmi_cm4.cpp b/cm4/source/sln_smart_tlhmi_cm4.cpp
index a706868..713e422 100755
--- a/cm4/source/sln_smart_tlhmi_cm4.cpp
+++ b/cm4/source/sln_smart_tlhmi_cm4.cpp
@@ -162,9 +162,9 @@ int APP_RegisterHalDevices(void)

     HAL_GFX_DEV_REGISTER(Pxp, ret);
     HAL_DISPLAY_DEV_REGISTER(LVGLCoffeeMachine, ret);
-//    HAL_CAMERA_DEV_REGISTER(CsiGc0308, ret);
+    HAL_CAMERA_DEV_REGISTER(CsiGc0308, ret);
 //    HAL_CAMERA_DEV_REGISTER(FlexioGc0308, ret);
-    HAL_CAMERA_DEV_REGISTER(CsiMt9m114, ret);
+    // HAL_CAMERA_DEV_REGISTER(CsiMt9m114, ret);
```

## Tips for internal development and debug

To speed up the internal development, we provide below configurations to only build and run CM7 and CM4 as standalone projects.

**Notice**:
Dual core and IPC comuncation have been disabled with these CM7 and CM4 standalone projects.

#### How to only run CM7 as a standalone project

1. Remove the Debug folder and the "sln_smart_tlhmi_cm7 JLink Debug.launch" debug launch file

2. Remove the reference to the slave cm4 project: Setting->Project Reference->unselect the cm4 slave project reference

3. Remove the Multicore slave: Settings->C/C++ Build->Settings->MCU C++ Linker->Multicore->unselect the M4SLAVE

4. Set the FWK_SUPPORT_MULTICORE=0 and ENABLE_MASTER=0 from: Settings->C/C++ Build->Settings->Preprocessor(Both c and c++)

5. Rebuild and launch the debug

#### How to only run CM4 as a standalone project

1. Erase the flash
2. Remove the Debug folder and the "sln_smart_tlhmi_cm4 JLink Debug.launch" debug launch file
3. Enable the "Manage linker script": Settings->C/C++ Build->Settings->MCU C++ Linker->select the "Manage linker script"
4. Set the FWK_SUPPORT_MULTICORE=0 and ENABLE_SLAVE=0 from: from: Settings->C/C++ Build->Settings->Preprocessor(Both c and c++)
5. Rebuild and debug to generate the "sln_smart_tlhmi_cm4 JLink Debug.launch" debug launch file
6. Disable the "Attach to a running target" in the launch file: JLink Debugger->Additional Options->disable "Attach to a running target"
7. Add the jlinkscript to initialize the SDRAM in the launch file: JLink Debugger->Additional Options-> Select the "evkmimxrt1170_connect_cm4_cm4side_sdram_16bit.jlinkscript" script file from the cm4 folder

