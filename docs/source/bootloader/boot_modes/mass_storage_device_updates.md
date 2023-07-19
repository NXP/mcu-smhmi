

# Mass Storage Device updates (MSD)

The MSD feature allows the device to be updated using USB instead of the Segger tool. Only the main application or its resources (coffee_machine/elevator) can be flashed in this manner. If the bootloader must to be updated, the Segger tool or the Factory Programming flow is necessary.
The MSD feature, by default, bypasses the signature verification to simply the development flow, since signing images can be unsuitable for quick debugging and validation.

<!-- ![Drag and Drop Interface](dragAndDropInterface.png) -->

## Enabling MSD mode

To enable MSD mode on the SLN-TLHMI-IOT, press and hold the `SW0` button while powering on the board.
If done correctly, the board's onboard LED changes to purple and begin blinking at an interval of roughly 1 second.

```{note}

To properly use `SW0` as a general-purpose switch the `SW0` dip switch must be set as 0001.
```

Additionally, if connected to a Windows PC, your computer must make a sound indicating a new USB device has been connected.
After observing the LED blinking behavior, navigate to “My Computer”, and confirm that the SLN-TLHMI-IOT kit has mounted as a Mass Storage Device as shown in the picture below.

<fig>
    <image href="../../img/bootloader/bootloader_MSD_FileExplorer.png"/>
</fig>

The size of the new storage device is equal to the Bank Size of the device from which you subtract the file system metadata.

## Flashing a new binary

The binary size increases exponentially when adding the GUI resources. Almost 70% of the total size is occupied by these sounds and images. To speed up the development and to decrease the load on the updating mechanism, the large images have been split into `code and resources`, both with fixed addresses in the flash.
Update operations can be done on individual components, or all together into a bundle.

Right now the MSD can be used to update :

* Main Application
* Resources
* Bundle update (Main Application + Resources)

The MSD update is compose of 4 stages:

* Waiting: the board is waiting for the user to drag and drop a binary. This is signaled by a purple led.
* Transfer: the board is downloading the file from the host. This is signaled by a blinking blue led
* Post-process: the board is handling memory copy, memory verification. This is signaled by a blinking blue led
* Finished: the board has finalized the update operation. This is signaled either via a green(success) or a red(failure) led

### Main application

To update the main application, a binary must be built for the address `0x30100000`.
Because of the remap functionality enabled in the bootloader, this binary can be placed in each of the three banks, and still work as it is running from the base address. The bootloader checks for the current unused bank and tries to write the image in that specific bank.
When dragging and dropping a binary for the main application, the bootloader checks if the reset handler of the new image is a flash address.
No other verification is done; the functionality correctness must be handled by the developer.
After the new image has been written, a resource copy is done. This means that during the update procedure, the resources will stay the same.
<!--What do you mean by a resource copy is being done?-->

```{image} ../../img/Update_MainApp.png
:align: center
```

### Resources

When updating the resources, the binary needs to be renamed into `RES.bin`.
The Bootloader contains a list of known files, res.bin is one of those file.
No verification is done on the resources binary.

In the same way as updating the main application, the bootloader checks for active bank and writes the binary in the unused one.
After the write is completed, the older firmware is copied and the new bank is activated.

```{image} ../../img/Update_Resources.png
:align: center
```

### Bundle

To update using the bundle method, a python script is used to generate the bundle.
The script is part of the ivaldi suite of scripts which is delivered to the customer.
The script is called `bundle_generate_tlhmi.py`. When calling it, two parameters must be set, both being the locations for two important files:

* bundle configuration file (-bf) - contains a list of files that are going to be fused to generate the bundle.
* board configuration file (-cf) - position of the files in flash in order to build the metadata.

In the released version of ivaldi, both bundle config and board config are placed under the platform config folder.
A full linux bash command to call this script looks like:

```bash
python bundle_generate_tlhmi.py -bf ../../../Scripts/sln_platforms_config/sln_tlhmi_iot_config/ -cf ../../../Scripts/sln_platforms_config/sln_tlhmi_iot_config/
```

After this, in the `Scripts\ota_signing\sign\output` folder, four files are present.

```{image} ../../img/Update_BundleGenerateScript.png
:align: center
```

 For MSD only `bundle.bin` is of interest, the other three are relevant for the Over-The-Air(OTA) updates, where validation is an important feature.
 To update using the `bundle.bin` drag and drop the binary. The name must not be modified, as this name is part of a hardcoded list of known files.

```{image} ../../img/Update_Bundle.png
:align: center
```

In order for the bootloader to parse and write all the modules to their designated addresses, metadata must be added to the package.
Two types of metadata exists:

* Bundle metadata is placed at the end of the bundle and contains:
    * Bundle size
    * Number of modules
    * Signature of the whole bundle
* Module metadata is placed after every module and contains:
    * Module type (Application or Resources)
    * Module starting address
    * Module length
    * Module signature

<!-- ![Flashing in Progress](msdFlashInProgress.png) -->

Upon completion, the board automatically reboots itself into the new firmware, which was just flashed. To verify this, open the serial CLI, type typing the version command,
and check that the application is running from the alternate flash bank.
