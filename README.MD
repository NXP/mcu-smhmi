# Introduction

This project contains all the source code for [NXP's Smart HMI solution](Smart HMI Website).
For information about getting up and running with the Smart HMI solution, check out the [Smart HMI Getting Started Guide].

## Out-of-Box Demos

The Smart HMI Solution comes with a few different demo applications flashed right out of the box.
These applications include the Coffee Machine and Elevator demo applications,
with new functionality and demos being added in as part of update releases.
Each of these apps are intended to demonstrate and highlight different functionality that the solution supports,
while giving a software reference design and boilerplate code to accelerate development of your own applications.

### Coffee Machine

This folder contains the source code for the Coffee Machine out-of-box demo application.
By default, the firmware for the Coffee Machine application is located at `0x30100000`
By default, resources (images, audio files, etc.) for the Coffee Machine application are located at `0x30700000`

### Elevator

This folder contains the source code for the Elevator out-of-box demo application.
By default, the firmware for the Elevator application is located at `0x31500000`
By default, resources (images, audio files, etc.) for the Elevator application are located at `0x31B00000`

### Smart Home Control Panel

This folder contains the source code for the Elevator out-of-box demo application.
By default, the firmware for the Elevator application is located at `0x32900000`
By default, resources (images, audio files, etc.) for the Elevator application are located at `0x32F00000`

## Bootloader

The Smart HMI solution utilizes a multi-stage boot process to enable additional security features, update mechanisms, and more.
As part of this multi-stage boot process, the out-of-box firmware uses a bootloader project which is responsible for,
among other things,

- Selecting which application to run based on the button held during boot
- Enabling [Mass Storage Device (MSD) Drag-and-Drop](#mass-storage-device-msd-drag-and-drop-updates) updates

The source code for the bootloader application can be found under the `bootloader` folder.
By default, the firmware for the bootloader project is located at `0x30000000`.

Additional details regarding the bootloader and the features it supports can be found in the [Smart HMI Software Developer Guide].

## Framework

The Smart HMI Solution makes use of NXP's "Framework Architecture" which is designed to accelerate development by:

- Reducing the amount of tightly coupled code that exists in each of the projects
- Allow for code reuse among different projects and platforms by making use of a Hardware Abstraction Layer (HAL)
- Enabling easier communication between cores in multi-core projects

More details regarding the Framework architecture, including its design and usage, can be found in the [Smart HMI Software Developer Guide].

# coffee_machine

This folder contains the coffee machine demo.
Main application should be placed at 0x30100000
Resources for coffee_machine should be placed at 0x30700000

# elevator

This folder contains the elevator demo.
Main application should be placed at 0x31500000
Resources for elevator should be placed at 0x31B00000

# home_control_panel

## Additional Resources

[Smart HMI Software Developer Guide]
[Smart HMI Getting Started Guide]
[Smart HMI Website]

[smart hmi website]: https://www.nxp.com/mcu-smhmi
[smart hmi getting started guide]: https://www.nxp.com/document/guide/getting-started-with-the-sln-tlhmi-iot:GS-SLN-TLHMI-IOT
[smart hmi software developer guide]: https://www.nxp.com/docs/en/user-guide/MCU-SMHMI-SDUG.pdf
