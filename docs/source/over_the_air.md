# OTA (Over the Air) updates

The following section gives instructions on how to generate, sign, deploy, and update the firmware.
It also describes all the tools provided with this solution to give context to what is happening.
This section assumes that the SLN-TLHMI-IOT kit has been migrated to communicate with a non-NXP AWS IoT Cloud server and the reader has access with the correct permissions.
OTA (Over-the-Air) updates are the process of pushing new firmware from a remote service down to a connected device.
When it happens,
the device programs the new image into the flash and reboots into that image assuming all necessary checks have passed.
As shown in the architecture section of this document,
there are two application partitions.
The application is always going to run into one of these sections.
It means that the second section is free to write into without affecting the existing image.
It also ensures that the device is safe to jump into the new image without worrying about being compromised assuming the relevant checks have been made.
The SLN-TLHMI-IOT kit leverages the Amazon OTA service within AWS IoT.
This also leverages the Amazon FreeRTOS OTA client to check for updates and download the image.

<fig>
<title>OTA high-level architecture</title>
<image href="./img/OTA_Flow.png">
</image></fig>

## Migration guide

This section provides the steps to migrate the SLN-TLHMI-IOT kit to a developer/organization own fully controlled AWS
account. If the SLN-TLHMI-IOT kit is left connected to the default server, it is managed by NXP and restricts developer’s access
and control of certain features. The unavailable features are described in the SLN-TLHMI-IOT-DG.

The advantages of doing migrating are:

* Full control of OTA jobs and deployment
* Customization of firmware/cloud control

To fully use the aws environment, create an AWS Account.

To communicate with AWS, the device must provide the certain artifacts and securely connect to AWS IoT.
If the artifacts are provided on the cloud, the device cannot connect successfully.
For steps to create an Amazon “Thing”, see https://docs.aws.amazon.com/iot/latest/developerguide/create-iot-resources.html.
The communication between the device and the AWS IoT cloud is secured based on the private key and on the device certificates
created together with the Amazon “Thing”.

```{note}
These steps are not required, as our manufacturing tool scripts (Ivaldi) do all the necessary setups, including
“Thing” creation. For more details on Ivaldi, see :ref:`Automated manufacturing tools`
```

### RT117H firmware changes

This section provides an overview of steps to make the necessary source code changes to ensure that the firmware communicates
with the correct AWS Account

As prerequisites:

* an AWS Account is created.
* the Get Started with MCUXpresso Tool suite and Building and Programming sections in the SLN-TLHMI-IOT-DG guide
are read.
* the projects are in your workspace and you are ready to make code changes

The change is required only in the coffee machine application.. The changes are a must to ensure that the
device connects to the correct AWS Endpoint for OTA.

To get started:

1. Follow the [IoT Console Sign-in](https://docs.aws.amazon.com/iot/latest/developerguide/setting-up.html) online resource to log into the desired account.
2. Navigate to the AWS IoT Core service which opens the console.
3. Within the AWS IoT Console, select the `Settings` button down toward the bottom left section of the page as shown in the next figure

```{figure} ./img/AWS_IoT_Monitor_Console.png
:align: center
:alt: AWS IoT monitor console

AWS IoT monitor console
```

```{Warning}
Ensure that the correct server location for the device that was created is used. If the wrong server is used, it causes a connection issue.
```

4. It opens the Settings page which has controls for logging and events. At the top of the page, there are “Endpoint Settings”. Copy the endpoint string which has the following structure `"id".iot."server".amazon.com.`

```{figure} ./img/AWS_Custom_endpoint_url.png
:align: center
:alt: AWS Custom endpoint URL

AWS Custom endpoint URL
```

5. The endpoint is obtained and must be inserted into the firmware. Within the bootloader application, navigate to the
“source > aws_clientcredential.h” file. Within the aws_clientcredential.h file, locate the array called `clientcredentialMQTT_BROKER_ENDPOINT` and change the existing contents to the endpoint obtained from AWS IoT
Endpoint Settings.

```{figure} ./img/AWS_Code_Endpoint_change.png
:align: center
:alt: AWS broker endpoint update in aws_clientcredential.h for coffee_machine

AWS broker endpoint update in aws_clientcredential.h for coffee_machine
```
AWS_Code_Endpoint_change.png

### Ivaldi Guide

The following section describes the steps to setup the Ivaldi environment.
This chapter assumes that the client has already downloaded and unzipped the Ivaldi_sln_tlhmi_iot.zip package.
For additional details,
check the “Automated Manufacturing Tool ”chapter from the SLN-TLHMI-IOT-DG.

Perform the following steps to configure the Ivaldi environment.

```{note}
These steps should be executed only once.
Ensure that none of the commands return errors.
For additional details check the “Ivaldi_sln_tlhmi_iot/README.md” and “Ivaldi_sln_tlhmi_iot/Scripts/ota_signing/README.md” files.
The Ivaldi tool was tested on the below Operating Systems and the corresponding Command-Line Interfaces:
• Linux – Bash CLI
• Windows – WSL (Windows Subsystem for Linux) CLI
• Windows – CMD (Command Prompt) CLI
```

1. Install the following tools.
   * OpenSSL # to check if installed: openssl version
   * AWS CLI # to check if installed: aws --version
       * Should be configure according to your account # to configure: aws configure
       * https://docs.aws.amazon.com/cli/latest/userguide/cli-chap-install.html
       * https://docs.aws.amazon.com/cli/latest/userguide/cli-configure-quickstart.html
   * Python 3.6.x

2. Setup the environment and install the requirements. Open a CLI (from the list mentioned above) and run the below
commands.
    * cd Ivaldi_sln_tlhmi_iot/
    * pip install virtualenv # Install the virtual environment tool
    * virtualenv env # Generate a new virtual environment
    * source env/bin/activate # Activate the virtual environment (on Linux or WSL)
    * env\Scripts\activate # Activate the virtual environment (on CMD)
    * (env) pip install -r requirements.txt # Install the python dependencies
    * (env) python setup.py install # Setup the environment.

3. Generate the certificates. Adjust the below command’s parameters according to your needs (replace: \[code], \[country],
\[state], \[org]) and run it within the same terminal opened in the previous step. The script below will asks for password for
several times, each time provide the same password. As result, the “Ivaldi_sln_tlhmi_iot/Scripts/ ota_signing/ca/” folder
containing all the required certificates will be created.
    * cd Scripts/ota_signing/
    * (env) python generate_signing_artifacts.py prod \[code] \[country] \[state] \[org]

    Example: (env) python generate_signing_artifacts.py prod FR France Normandy NXP
4. Add the previously generated certificates in the filesystem that is going to be deployed on the board. To do that, add the path for the file in file `Scripts/sln_platforms_config/sln_tlhmi_iot_config/littlefs_file_list.py`

5. Add the password provided at Step 3 to the ivaldi scripts. This approach of providing the password
is not recommended due to security reasons, but may be used for a quick test of the setup.
    * Open the `Scripts/ota_signing/sign/sign_me.py` file and add the password on line 49 (example: PKEY_PASS = 'my_password').
    * Open the `Scripts/ota_signing/sign/bundle_generate_tlhmi.py` file and add the password on line 139 (PKEY_PASS = 'my_password').

6. Test the environment by flashing an open boot device. Connect the device to the PC via USB. Make sure you
have all the required demo inside the Image Binaries folder. Also make sure that the serial mode jumper is properly set.
Within the same terminal as before, run the below commands.
• (env) cd ../sln_tlhmi_iot_open_boot/
• (env) python open_prog_full.py

## Preparing an OTA image

This section describes the steps to create a binary to update the demo app.
When building an OTA image, make sure to properly sign the image that will be sent.
Image authentication is a key factor in the AWS high-level architecture.
As the SLN-TLHMI-IOT kit is built to communicate with an NXP demonstration AWS IoT account, OTA is managed by NXP.
For OTA to be managed by the developer, the Migration Guide must be executed to provide access to an AWS IoT Core implementation for OTA management.
Without this process, OTA is not manageable for the developer. Before starting, check the Ivaldi tool [Ivaldi tool](./bootloader/security/ivaldi.md)

## Building image

As mentioned before in the [MSD](./bootloader/boot_modes/mass_storage_device_updates.md) chapter,
the current bootloader enables the remapping feature that
helps customer to easy deploy new image, without keeping tracks of the current active bank.
All bootable images must be built with Flash address at `0x30100000`
The current implementation supports update with same images version or older version.
Best practices dictate that the version should be always higher. To re-enable this functionality set `otaconfigAllowDowngrade` to 0 inside the `ota_config.h` file.

## Sign Image

The following section describes what the NXP Application Image Signing Tool (Signing Tool) is and how to use it. The Signing Tool
is a python application that is responsible for using a signed Certificate Signing Request (CSR) to sign the binaries and append
the certificate to the binary ready to be deployed to the AWS IoT OTA service.
The Signing Tool requires Python3 to run.
The following instructions assume that the README file in the Ivaldi root directory has been followed to set up the Python virtual environment.
If this is not done, the scripts fail.
Navigate to the “Scripts/ota_signing” directory inside Ivaldi. For more details, check the “QUICK SETUP” section from the
“Scripts/ota_signing/README.md” file.

### Creating a root, intermediate pair with sign server, and certificates

A tool was created to generate all the artifacts needed for OTA signing. This tool is called “generate_signing_artifacts.py” and was
derived from publicly available information for generating CA certificate artifacts.
The “generate_signing_artifacts.py” takes 5 parameters that are all used to create the artifacts. The ca_name is the entity where
all the file names are labeled and used as the common name. It asks you to enter a “pass phrase”, enter the same each
time.
Once “generate_signing_artifacts.py” succeeds, a “ca” folder inside “Scripts/ota_signing” appears. Inside the “ca” folder you can
find: “certs” and “private” folders.

Inside the “certs” folder there are 3 files:

* “<ca_name>.app.a.crt.pem”
* “<ca_name>.app.b.crt.pem”
* “<ca_name>.root.ca.crt.pem”.

Inside the “private” folder there are 3 files:

* “<ca_name>.app.a.key.pem”
* “<ca_name>.app.b.key.pem”
* “<ca_name>.root.ca.key.pem”

```{figure} ./img/OTA_GenerateCerts.png
:align: center
:alt: generate_signing_artifacts.py description, usage and logs

generate_signing_artifacts.py description, usage and logs
```

The script has been run from Windows Linux subsystem, but it can be run from any terminal.

The Ivaldi tools should have access to the password used in the previous step for running the “generate_signing_artifacts.py”
script. To achieve this, two files should be modified:

* Open the “Scripts/ota_signing/sign/sign_me.py” file and add the password on line 49 (example: PKEY_PASS = 'my_password').
* Open the “Scripts/ota_signing/sign/bundle_generate_tlhmi.py” file and add the password on line 139 (PKEY_PASS = 'my_password').

```{note}
This approach of providing the password is not recommended due to the security reasons, but may be used for a
quick test of the setup.
```

Navigate into the “Scripts/ota_signing/sign” folder and run the “sign_me.py” tool with the name of the binary to sign (for example
ais_ffs_demo binary) and the certificate name (for example, the “prod.app.a” that we have generated in the previous step) for the
entity.

### Formatting the CA and the application certificate

For the device to be able to verify the image signature, it must to have the root CA certificate.
<b>(“ca/certs/<cert_name>.root.ca.crt.pem”)</b> and the application certificate derived from the signing entity <b>(“ca/
certs/<cert_name>.app.a.crt.pem”) </b>.

The certificates do not have a specific address at which to be written, both need to be included into filesystem.
The obtained filesystem is going to be transformed into binary format and loaded with the rest of images.
It is done when running open_prog_full.py script. Be sure to generate all needed certificate before running the script.

## OTA Workflow with AWS IoT Console

On the device side, if the filesystem has been properly loaded and the board is connected to a WiFi network, the application will create a secure MQTT connection with the AWS cloud.
MQTT connection is used to receive push update request from the AWS cloud.

To use Amazon OTA, you must configure various roles to allow AWS IoT access to the S3 Bucket (this is the server that holds
your images). The following link was used by NXP to configure their own OTA service:
<https://docs.aws.amazon.com/freertos/latest/userguide/ota-prereqs.html>

To create an OTA Job, follow these steps:

1. Navigate to the following link: <https://docs.aws.amazon.com/freertos/latest/userguide/ota-console-workflow.html> . Focus on the area named “Use my custom signed firmware image” as this is the process that focuses on custom signed image
creation. No other way of deploying images is currently supported.
Click the <b>Create job</b> button inside the <b>AWS IoT > Jobs tab</b>.

2. A new window appears. Inside this window, select Create FreeRTOS OTA update job as shown in shown in the next figure

    ```{figure} ./img/OTA_CreateOTAJobs.png
    :align: center
    :alt: Create OTA job – Job types

    Create OTA job – Job types
    ```

3. The OTA Job Properties window appears. Provide a job name as shown in:

    ```{figure} ./img/OTA_JobName.png
    :align: center
    :alt: Create OTA job – Job name

    Create OTA job – Job name
    ```

4. The OTA File Configuration window appears. Specify the serial numbers of the devices to be updated. Select the MQTT option as the protocol for file transfer as shown in

    ```{figure} ./img/OTA_DeviceToUpdateAndProtocol.png
    :align: center
    :alt: Create OTA job – Devices to update and protocol for file transfer

    Create OTA job – Devices to update and protocol for file transfer
    ```

5. Select the image that is going to be delivered to the remote device.
To do this, select <b>Use my custom signed file</b> and copy in the <b>Signature</b> textbox the content that has been obtained as the output of the Signing Tool (sln_demo_new_img.bin.sha256.txt). The following fields must be properly set:

* Original hash algorithm - `SHA-256`
* Original encryption algorithm - `RSA`
* Path name of code signing certificate on device - `app_a_sign_cert.dat` (check littlefs_file_list.py for the name of the file)

Check the images from below more information.

If a new image is going to be loaded, check `"Upload a new file"`, click `"Choose file"` and select the image. S3 storage address must be specified in the `"S3 URL"` field.
If the loaded binary image already exists in the location, the user can select the checkbox corresponding to `"Select an existing file"` and use the existing image.

The binary size increases exponentially when adding the GUI resources. Almost 70% of the total size is occupied by those. To speed up the development and to decrease the load on the updating mechanism, the image has been split into `code and resources`, both with the fixed address in the flash.
Update operation can be done on components, or all together into a bundle.
Right now the OTA can be used to update:

* Main Application
* Resources
* Bundle update (Main App + Resources)

```{figure} ./img/OTA_FileInfo.png
:align: center
:alt: Create OTA job – File info

Create OTA job – File info
```

Until now the configuration for the update was the same. The difference, as was for the MSD, is in the name of the file that must be updated in the `Path name of the file on the device`. The files should be completed with:

* AppA , when updating the main application
* Resources, when updating only resources
* Bundle, update both at the same time

### Update Main App

Because of the remap functionality enabled in the bootloader,
this binary can be placed in each of the three banks and still work as it is running from the base address.
When receiving an OTA request, the OTA_Agent checks for the unused bank.
The empty bank is erased to prepare it for the update. All the erase is done before starting to receive actual data. It is a measure to work around the not-in-order MQTT packets' arrival. After the new image has been written, verification is done to check the signature.
Using the <b>Signature</b> field and <b> Path name of the code signing certificate on device </b> field, the main application can start validating the new image.
If everything is right, a resource copy is done, and the empty bank is set as an active bank.
It means that during the update procedure the resources will stay the same.

```{image} img/Update_MainApp.png
:align: center
```

### Update Resources

Same as updating the main application, the OTA_Agent on request checks for active bank and writes the binary in the opposite one. A complete erase is done beforehand.
After the write is completed, the older firmware is copied and the new bank is activated.

```{image} ./img/Update_Resources.png
:align: center
```

### Update with Bundle

To update with a bundle, a python script is used to generate the bundle.
The script is part of the ivaldi suites of scripts that is delivered to the customer.
The script is called `bundle_generate_tlhmi.py`. When calling it, two parameters must be set, both being the location for two important files:

* bundle configuration file (-bf) - contains a list of files that are going to be fused to generate the bundle
* board configuration file (-cf) - position of the files in flash to build the metadata.

After running the script there is no need to pass the binary through the singing process as this script generates also a signature used by the device to validate the new image.

```{image} ./img/Update_Bundle.png
:align: center
```

The current firmware sets all the images at the right positions based on the metadata.
After the parsing of the bundle is complete and all images are placed accordingly to fica_definitions.h file, the new bank is activated.

After completion, the application reboots in self-test mode.
For now, nothing is done in self-test mode except checking for the version of the new application.
Reboot to make sure self-test mode is not used.
