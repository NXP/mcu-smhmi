# Introduction

This Smart Panel application demonstrates the smart control panel use-case with the following core functionalities:
- Smart panel GUI (including thermostat, security and audio player mini applications) with touch support
- Local voice command to control the use cases of Smart panel
- Face recognition to store user's thermostat preferences and store security users
- Hand gesture recognition to control the Audio Player mini-application

For leveraging the full computational power of the RT117H,
the application image is split into two images that are running in parallel on the CM7 and CM4 cores. The Smart Panel CM7 acts as an AI block, handling all the machine learning tasks, such as face recognition, voice command and gesture recognition. The operation has been optimized to obtain the best performance on this type of MCU. The Smart Panel CM4 holds the user interaction (display, shell, buttons). The CM4 image is loaded into the memory by the CM7 core.

By default, i.MX RT117H boot from CM7. By fusing BT_CORE_SEL (Bit 12 in 0x960), the chip switches to CM4 as the main core. For more info on this topic, check [Application Note 13264](https://www.nxp.com/docs/en/application-note/AN13264.pdf).

The Smart Panel uses the following HW components and peripherals:
- 2 x PDM MIC - configured to work with 16 kHz sampling. The conversion to PCM is done in hardware using the PDM microphone interface.
- 16 KHz raw data to RT117x MQS HW peripheral that generates PWM data output.
- External filtering and coupling.
- Analog audio amplifier
- MIPI GC2145 Camera - configured to work with 600x800 resolution.
- Flexio GC0308 IR Camera -  configured to work with 640x480 resolution.
- LCDIFV2 Rocktech RK055MHD091 - configured to work at the HD resolution of 1280x720

To change this configuration, check the HAL code and "How to develop a user application" section.

It uses NXP's below core technologies:
- LVGL-based GUI
- Local voice command algorithm
- Face recognition algorithm
- Gesture recognition algorithm
- Dual core architecture based on multi-core manager (mcmgr) middleware component.

# Architecture
  <fig>
      <image href="img/home_panel_software_architecture.JPG"/>
  </fig>

# Software block diagram
  <fig>
      <image href="img/home_panel_software_diagram.JPG"/>
  </fig>

It includes two projects as below:

-Host CM7 project

-Slave CM4 project

Each project uses a two layer architecture containing the "Framework + HAL" layer, and the "Application" layer. For the details, refer to the documentation on each project.
