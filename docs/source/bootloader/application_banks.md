---
sidebar_position: 3
---

# Application Banks

For this project, we enabled three application flash banks, **Bank A**, **Bank B** and **Bank C**. It is done to showcase in our OOBE all projects (`coffee_machine, elevator`) simultaneously.

In a real-life scenario, only 2 banks are needed. In the updating mechanism that has been implemented, we use 2 banks by doing a ping-pong between `Bank A` and `Bank B`.

The SLN-TLHMI-IOT utilizes a series of dual "application flash banks" used as a redundancy mechanism when updating the firmware via one of the bootloader's update mechanisms in the "boot_modes/mass_storage_device_updates.md" file or via the AWS OTA mechanism.

## Banks

The application we developed for SLN-TLHMI has 2 inter-dependent parts:
- Application (code)
- Resources (icons, sounds, pictures)

So a bank is a reserved space in the flash that stores both of these components.
The application running tries to read resources from the same bank.

In the OOBE the size of a bank is 20MB (0x1400000),
6 MB (0x600000) for the code area and 14 MB (0xE00000) for resources.
If there is a need to increase or decrease this value, check "fica_definitions.h" file.

```{figure} ../img/Bank_Components.png
:align: center
:alt: Bank Components

Bank Components
```

## Addresses

The flash address for each of the application flash banks is as follows:

* Bank A - `0x30100000`
    * Bank A App - `0x30100000`
    * Bank A resources - `0x30700000`
* Bank B - `0x31500000`
    * Bank B App - `0x31500000`
    * Bank B resources - `0x31B00000`
* Bank C - `0x32900000`
    * Bank C App - `0x32900000`
    * Bank C resources - `0x32F00000`

## Remapping

The i.MXRT117H chip supports the flash remapping function,
which allows users to remap flash address to the FlexSPI interface.
The flash remapping function is beneficial in the following use-cases:

- To flash multiple firmware.
- To switch one of the firmware to run when the condition is met.
- To update the firmware in the wireless application (the usual process is to download the firmware to flash, perform the validity check, and then switch to new firmware to run. The flash remapping function helps to directly run the firmware wherever it locates to XIP flash.)

For more information, check:
[How to Use Flash Remapping Function](https://www.nxp.com/docs/en/application-note/AN12255.pdf)

In older Solution's projects like [SLN-VIZN3D-IOT](https://www.nxp.com/design/designs/nxp-edgeready-mcu-based-solution-for-3d-face-recognition:VIZN3D)
and [SLN-VIZNAS-IOT](https://www.nxp.com/design/designs/nxp-edgeready-mcu-based-solution-for-face-recognition-with-liveness-detection:SLN-VIZNAS-IOT), the images were built for a specific bank.
With the enablement of the remapping functionality,
all applications must be built having the Flash Starting Address set to `0x30100000`.

The updating mechanisms implemented in the bootloader or the main application, also leverage this feature.
Because of this, the updating procedure does not have to keep track of what bank the application is running from. The binary that is going to be used for an update, is always going to be built with the `BANK A` memory settings and is going to be placed in the non-active slot.

```{warning}
 The OOBE is meant to showcase all 3 application. After an update procedure, the application that was written in a not active bank is going to be overwritten.
```

### Convert .axf to .bin

When building a project in MCUXpresso IDE,
the default behavior is to create a `.axf` file.
However, some of the bootloader update mechanisms including [MSD updates](boot_modes/mass_storage_device_updates.md) require the use of a `.bin` file.

Converting a `.axf` file to `.bin` can be done in MCUXpresso without any additional setup.

To perform this conversion, navigate to the project directory that contains your compiled project binary and right-click the `.axf` file in that directory.

:::info
The binary for your project is likely located in either the **Debug** or **Release** folder depending on your current build config.
:::

In the context menu, select **Binary Utilities->Create binary**.

```{figure} ../img/bootloader/convertToBinary.png
:align: center
:alt: Convert to binary

Convert to binary
```

Verify that the binary has been successfully created.
