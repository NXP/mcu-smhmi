---
sidebar_position: 12
---

# Getting started with VIT

Smart HMI demos are using by default DSMT as Audio Speech Recognition technology.
In order to enable VIT ASR in Smart HMI SDK demos you will need to do the following code modifications:
1. In cm7 board_define.h comment ENABLE_DSMT_ASR and uncomment ENABLE_VIT_ASR (path towards header: "cm7/board/board_define.h")
2. At the moment of this release, French is not supported on VIT. Hiding it from the available languages menu is done putting FRENCH_LANG_SUPPORTED define on 0 in "cm4/custom/custom.h".
3. After modifying the files, build lvgl library and then build the cm7 project and flash it

# Barge-in support when VIT is enabled

At the moment of this release, VIT is not compatible with the AFE which is integrated in the Smart HMI SDK.
As a consequence, barge-in is not available when VIT is enabled.
This should change in the future, as a compatibility between VIT and Voice Seeker is planned.

# Obtaining a new VIT model

In order to obtain a new model you can submit a request at this address: https://vit.nxp.com/#/

Please note that you'll need an nxp.com account first!

# Integrating a new VIT model

Place the newly obtained model in the same folder as the currently existing models, as shown in the screen capture below.

<fig><title>VIT models</title> <image href="img/vit_models.png"> </image></fig>

Other files that would need to be updated for VIT integration are the ones highlighted below.

<fig><title>VIT files</title> <image href="img/update_files_for_vit_integration.png"> </image></fig>

# Multilanguage support

Please note that VIT does not support listening for multiple wake words from different languages at the same time, as it is the case with DSMT.
Hence, you'll be able to say only one wake word at a time. In order to change to a different language you'll need to use the language menu from the display.

# Additional info and resources

For documentation and other resources please go at this page: [VIT page](https://www.nxp.com/design/software/embedded-software/voice-intelligent-technology:VOICE-INTELLIGENT-TECHNOLOGY)