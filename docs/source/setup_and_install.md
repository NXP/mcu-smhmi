---
sidebar_position: 2
---

# Setup and Installation

This section is focused on the setup and installation of the tools necessary to begin developing applications using NXP's framework architecture.

```{note}
This guide focuses on the use of [MCUXpresso IDE](https://www.nxp.com/design/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-integrated-development-environment-ide) for development.
```
## MCUXpresso IDE

MCUXpresso IDE brings developers an easy-to-use Eclipse-based development environment for NXP MCUs based on Arm Cortex-M cores, including its general-purpose crossover and Bluetooth-enabled MCUs. MCUXpresso IDE offers advanced editing, compiling, and debugging features with the addition of MCU-specific debugging views, code trace and profiling, multicore debugging, and integrated configuration tools. MCUXpresso IDE debug connections support Freedom, Tower system, LPCXpresso, i.MX RT-based EVKs, and your custom development boards with industry-leading open-source and commercial debug probes from NXP, P&E Micro, and SEGGER.

For more information, see the [NXP website](https://www.nxp.com/design/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-integrated-development-environment-ide)

## Install the toolchain

MCUXpresso IDE can be downloaded from the NXP website by using the below link:

[Get MCUXpresso IDE](https://www.nxp.com/webapp/swlicensing/sso/downloadSoftware.sp?catid=MCUXPRESSO)

<fig>
    <image href = "img/Download_MCUXpresso_IDE.jpg"/>
</fig>

To download the correct version of IDE, check out the [Getting Started Guide](https://www.nxp.com/document/guide/getting-started-with-the-sln-tlhmi-iot:GS-SLN-TLHMI-IOT). Once the download has been completed, follow the instructions in the installer to get started.

```{note}
There is a bug in version 11.5.1 of MCUXpresso IDE that prevents building projects for SLN-TLHMI-IOT, so version >= 11.7.0 is required.
```

<fig>
    <image href="img/Check_MCUXpresso_IDE_version.jpg"/>
</fig>

## Install the SDK

To build projects using MCUXpresso IDE, install an SDK for the platform you intend to use. A compatible SDK has the required dependencies and platform-specific drivers needed to compile projects.

A compatible SDK can be downloaded from the official [NXP SDK builder](https://mcuxpresso.nxp.com/en/select)

1. To build the SDK for your preferred setup, use MCUXpresso IDE to install the SDK.
2. To do this, open the application and click Download and Install SDKs on the MCUXpresso IDE welcome screen as shown below:

<fig>
    <image href="img/Download_and_Install_SDKs.jpg"/>
</fig>

3. A catalog of all the SDKs that can be downloaded through MCUXpresso is available. These SDKs provide device knowledge, drivers, middleware, and reference example applications for your development board or MCU. Type `evkmimxrt1170` in the filter section and download evkmimxrt1170 SDK. The current applications were developed and tested on SDK 2.11.1.

<fig>
    <image href="img/Download_RT1170_SDK.jpg"/>
</fig>

4. A prompt displays the license agreement associated with the 1170 SDK.
5. Read and accept the license to automatically start the SDK installation.
6. MCUXpresso proceeds to download the SDK.

<fig>
    <image href="img/Install_RT1170_SDK.jpg"/>
</fig>

## Import example projects

```{note}
To build example projects that you import regardless of how they are imported, you must have a compatible MCUXpresso SDK package for SLN-TLHMI-IOT installed.
```

MCUXpresso IDE allows you  open example projects from the source folder.

### Import from Github

```{note}
Before you begin, make sure you have [ Git ](https://git-scm.com/downloads) downloaded and installed on the machine you intend to use.
```

The latest software updates for the SLN-TLHMI-IOT application can be downloaded from our official [Github repository](https://github.com/NXP/sln_tlhmi_iot). Here, you will find the most up-to-date version of the code that contains the newest features available for the Smart TLHMI project.

To import the SLN-TLHMI-IOT Smart TLHMI application into MCUXpresso IDE using Github,
perform the following steps:

1. Clone the **sln_tlhmi_iot** repository.
    * Cloning directly to your MCUXpresso workspace location is recommended, but not required.
2. In MCUXpresso, navigate to the **File** from Toolbar.
3. Click **Open Projects from File System...**.
4. Select **Directory...**.
5. Navigate to the file path of the project cloned in the first step and click **Select Folder**.
6. Check the box next to each project (bootloader, coffee_machine\cm4, coffee_machine\cm7, coffee_machine\lvgl_vglite_lib and elevator\cm4, elevator\cm7, elevator\lvgl_vglite_lib) you wish to import.
7. Click **Finish**

<fig>
    <image href="img/Open_SLN-TLHMI-IOT_projects.jpg"/>
</fig>

After following the above steps,
confirm that the projects can be found in the **Project Explorer** panel to ensure they were successfully imported.

<fig>
    <image href="img/Example_projects.jpg"/>
</fig>

## Dual-core debug

The SLN-TLHMI-IOT runs under dual-core architecture. How to debug the dual-core application? Please refer to [AN13264](https://www.nxp.com.cn/docs/en/application-note/AN13264.pdf) for more details.

## JLink flash tool issue in MCUXpresso v11.7.0
When using the MCUXpresso GUI Flash Tool to erase the whole chip, the SEGGER J-Flash tool will be called to do the real work.

<fig>
    <image href="img/mcux-gui-flash.png"/>
</fig>

The J-Flash tool will try to erase two flash banks:
- bank 0 : 0x30000000
- bank 1 : 0x60000000

Since there is only one bank on the board, it will report errors when trying to get information about bank 1 @ 0x60000000.

<fig>
    <image href="img/mcux-gui-flash-error.png"/>
</fig>

This error can be ignored since bank 0 is erased correctly.
