
[open_prog_full]: ../../img/Ivaldi_openprogrfull.png "Enable Image Verification"

# Automated manufacturing tools
This section provides an overview about Ivaldi, prerequisites, platform configuration, and open boot programming.

## About Ivaldi

Ivaldi is a package that is responsible for manufacturing and re-programming without needing J-Link. It uses the serial downloader mode within the RT117H boot ROM to communicate with an application called Flashloader that is programmed into the RT117H.
It then communicates with a program called blhost which controls various parts of the chip and flash.
Ivaldi was created to focus on the build infrastructure of a customer’s development and manufacturing cycle. Its primary focuses are:
- Factory programming and setting up a new device/product
- Generating AWS IoT Devices
- Creating certificate/key pairs for devices
- Associating policies with devices
- Signing images for OTA and HAB
- Writing and Accessing OTP fuses

The following section is going to discuss the general flashing of a device without debugging tools.

```{note}

In order to use Ivaldi, the board must be put in Serial Download Mode. For doing that, move the jumper J203, which
is located on the top of the board into position “0”. Check Hardware Guide for more info.
```

## Requirements

Ivaldi has the following requirements before usage:
* Migration Guide in the "over_the_air.md" file must have been followed
* OpenSSL
* AWS CLI installed
    * https://docs.aws.amazon.com/cli/latest/userguide/cli-chap-install.html
    * https://docs.aws.amazon.com/cli/latest/userguide/cli-chap-configure.html#cli-quick-configuration
* Python 3.6.x
* Linux / Windows CMD / Ubuntu for Windows
* README.md from ivaldi root folder should be followed

##  Platform configuration

Ivaldi uses a platform configuration file “Scripts/sln_platforms_config/sln_tlhmi_iot_config/board_config.py”. This file describes:

* The names of the binaries (from Image_Binaries folder) which will be flashed:
    * BOOTLOADER_NAME
    * DEMO1_NAME
    * DEMO1_NAME_RESOURCES
    * DEMO2_NAME
    * DEMO2_NAME_RESOURCES
* Flash configurations:
    * FLASH_TYPE
    * FLASH_START_ADDR
    * FLASH_SIZE
* Flash Map
    * Binaries’ images addresses
    * Filesystem starting address and size
    * FICA table addresses

In order to configure Ivaldi to use specific image binaries from Image_Binaries folder, it is
enough to update “Scripts/sln_platforms_config/sln_tlhmi_iot_config/board_config.py” file.



```{warning}
ANY CHANGES IN “Scripts/sln_platforms_config/sln_tlhmi_iot_config/board_config.py” (EXCEPT BINARIES’
NAMES) WILL ALSO REQUIRE UPDATING THE EMBEDDED CODE AND CONFIGURATIONS.
```

## Open Boot Programming

The Open Boot Programming tool is responsible for creating a device and programming it with the correct images, certificates and artifacts.
This method is a quick and easy way of taking a device/product that has come from the assembly line and getting it ready to ship. It is also good practice to run the Open Boot Programming script before enabling the security features to ensure all images and artifacts are in working order.
The Open Boot Programming script should only be run when all the images and artifacts are obtained. Before running the script, ensure the following files and folders exist in the “Image_Binaries” directory of Ivaldi root and ensure that all the files mentioned in the board_config.py exists. After the script was executed,
do not forget to exit the serial downloader mode by moving back the J203 jumper.

Within the Ivaldi package, there is a directory called “Scripts/sln_tlhmi_iot_open_boot” which contains the “open_prog_full.py”
script and a README. The README contains the build requirements for each image before running the script as if not done
correctly could cause a boot failure.

To program the firmware and artifacts, execute open_prog_full.py script which will perform the following actions:
* Communicate with the BootROM to program Flashloader
* Create a device with
    * Certificate
    * Private Key
    * Policy Attached in the cloud
* Erase the flash
* Generate littlefs format filesystem, which will contain files specified in the littlefs_file_list.py
* Programming the images
    * Bootloader
    * demo1
    * demo1_resources
    * demo2
    * demo2_resources
    * Program the FICA
    * Program the littlefs

In the current open_prog_full.py python script, the littlefs is being generated to contains all the files mentioned in littlefs_file_list.py. Four files are expected:
* Root CA certificate
* AppA sign certificate - validated by the CA certificate and used to sign all the images that are being written or send for update
* AWS certificate - used to validate connection with AWS  server
* AWK public key - used to communicate with AWS MQTT server

One drawback of the current littlefs implementation is that it doesn't support the attributes. Used in the SLN_TLHMI_IOT project to generate encrypted files.

```{warning}
OPEN PROGRAMMING SCRIPT ASSUMES THE POLICY IS CALLED `tlhmi_deployment`. UPDATE THE SCRIPT TO USE THE CORRECT POLICY NAME IN THE CUSTOMERS AWS ACCOUNT.
```


