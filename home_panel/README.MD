# Introduction

This repository contains all the source code for the smart panel demo which belongs to Smart TLHMI solution.  The Smart panel demo is based on dual core (CM7 and CM4) architecture, thus it includes the CM7 master project which is under the cm7 folder and the CM4 slave project which is under the cm4 folder.

## How to flash the firmware and resource binary

#### 1. Bootloader binary

By default the board comes with a bootloader written at the 0x3000_0000 address. If at any moment, a full erase of the board was done, you need to flash the bootloader again. The binary is going to be provided in the github repository under home_panel/binaries/

#### 2. Flash the resource binary

The LVGL image and the audio prompts have been split from the firmware, you need to flash the resource into the dedicate flash region before you flash the firmware:

Flash the "resource\home_panel_resource.bin" into flash region 0x3080_0000

**Notice**: the resource binary only needed to be flashed once. Also if you do an erase operation, you just need to re-flash the resource once again.

#### 3. Build and flash the firmware binary

Refer below "How to build the projects" section to build and flash the firmware.

## How to build the projects

### Build Environment

 ```c
MCUXpresso IDE: v11.7.0
SDK version:  MIMXRT1170-EVK 2.13.0
 ```

The current application has been tested and developed using MCUXpresso IDE v11.7.0 and v11.7.1. Any version older than V11.7.0 will not work because of an issue with building multicore projects.
The SDK version is 2.13.0 and can be downloaded NXP's website.
The application is build for the address 0x3010_0000.

## How to update lvgl/vglite library

By default, the debug configuration uses a compiled library binary for lvgl+vglite b/c compilation + creating makefiles for that code heavily impacts how quickly changes can be tested.
