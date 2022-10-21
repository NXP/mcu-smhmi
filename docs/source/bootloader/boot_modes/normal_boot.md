
[enable_image_verification]: ../../img/bootloader/bootloader_EnableImageVerification.png "Enable Image Verification"

# Normal Boot

By default, if no other boot flags are set during the boot phase, the Normal boot mode is used.
During Normal boot, the Bootloader will simply boot to the "main" application which is flashed at the current application bank flash address (see Application Banks for more information).
For example, if the current flash bank is set to Bank A, then the Bootloader will jump to the flash address associated with Bank A and begin running the application at that address.

The OOBE has a set of three application that can be booted into at startup.
By default the application will always boot in the `Bank A`, which for now corresponds to `coffee_machine app`.
In order to change the boot app use buttons labeled from `SW1-SW3` when powering the board.

The following list shows the associations of boot app to switch.

- `SW1` - Bank A - coffee_machine
- `SW2` - Bank B - elevator
- `SW3` - Bank C - TBD

The decision to what application to jump is handle inside the bootloader.
In order to reach the bootloader a soft or hard reset is needed.

Eg: In order to boot in elevator application:
1. Unplug the board
2. Press and hold SW2 button
3. Plug the board in.

From the Bootloader's perspective, it is unaware what application it is jumping into, because it uses addresses and not names.
After an update procedure, the application that was written in an inactive bank will be overwritten, so the links between banks and demos will not be valid anymore.

## Turn on image Verification

In the OOBE Bootloader demo, Image Verification is disabled to encourage developers to play with the code.
If Image Verification is enabled, Normal boot will also check that the image certificate for the firmware image to run has been signed by a trusted certificate authority to ensure that the application comes from a trusted source.
Should the signature check fail, the Bootloader will not run the application in order to avoid executing untrusted, potentially malicious firmware.

For more details regarding image verification, see Image Verification in the "security/image_verification.md" file.

In order to enable the image verification, “DISABLE_IMAGE_VERIFICATION” must be set to 0 inside the <b>Preprocessors</b> sections:
1. Within the MCUXpresso Bootloader project, right click on the root project and navigate to <b>Properties > C/C++ Build > Settings > Preprocessor </b>.
2. Inside the <b>Preprocessors</b> section, change the MACRO <b>“DISABLE_IMAGE_VERIFICATION”</b> to “0” and click the <b>Apply and Close button </b> as describe in [Figure](#enable_image_verification)

![Enable Image Verification](../../img/bootloader/bootloader_EnableImageVerification.png)

3. After that change, rebuild the Bootloader.
4. Use the automated manufacturing tool 'Ivaldi' (refer to "security/ivaldi.md") in order to flash the device with proper FICA and certificates.

## Disable Debug Console

In the OOBE Bootloader demo, Debug Console is enable to help developers test and debug their code.
This feature introduce unwanted message being display and an increase in the boot up time.
In order to disable this, set `ENABLE_LOGGING` to 0 in "FreeRTOSConfig.h"

```{note}
Current implementation of the debug console adds about 150 ms in boot time.
```