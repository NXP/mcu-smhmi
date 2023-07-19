# Getting started with VIT
VIT is based on state-of-the-art deep learning and speech recognition technologies.
The Smart Panel App uses VIT as Audio Speech Recognition technology. Below source code blocks referenced are copied from the Smart panel App.

# Obtaining new VIT models

First effective way is to create new model by VIT online tool: https://vit.nxp.com/#/generate. Need an nxp.com account for logining this website.
The online tool support maximum amount of command is 12. If you need create more commands or require some help for tuning more to get better recognition performance, please email to local‑commands@nxp.com for more help. Would like suggest to explain requirements as detailed as possible. For example, if you need to help for creating a new model, please write down your Wake word and command list and let us know your product usage better.
There is a FAQ list, which may be useful for you: https://vit.nxp.com/#/faq.

# Integrating a new VIT model

After obtaining new models, Need to pack new models into home_panel_resource.bin file.
Regarding the usage of resource build tool, refer [home_panel_resource](../resource_build.md)
-- Update resource file indexes in [home_panel_resource.txt](../../../home_panel/resource/home_panel_resource.txt)

  <fig>                
    <image href="img/vit_model_index.png">
    </image>
  </fig>

-- Execute resource pack tool
In Linux environment, Execute `bash home_panel_resource.sh`. For Windows environment, run `./home_panel_resource.bat`

-- Extract model position informations in resource binary file
After resource pack tool is successfully executed, Two files are generated: home_panel_resource.bin and resource_information_table.txt.
In resource_information_table.txt file, We can read the model position/size and models total size. 

  <fig>                
    <image href="img/vit_model_position_size.png">
    </image>
  </fig>

-- Update model size and position into device firmware

  <fig>                
    <image href="img/LoadVITModels.png">
    </image>
  </fig>

-- Update total model size in appConfig.h file

  <fig>                
    <image href="img/vit_total_model_size.png">
    </image>
  </fig>

-- Update command ID in IndexCommands.h file
A better practise is to define action ID matching UI designs. For example in Smart Panel App, on the main menul panel, there are three
voice commands detectable.

  <fig>                
    <image href="img/main_menu_action_id.png">
    </image>
  </fig>

There is a copy of IndexCommands.h in cm4 project, since UI device is running on cm4 and it needs these action IDs to update display.  

-- Update the array used for converting command ID to action ID in IndexCommands_vit.h
Some developers may would like to use command ID instead of action ID directly. In real practise, in order to get better voice performance, we may need to tune models many times. And the command ID may be changed during model tuning. After model ID being changed, just need to update the below arraies, without any changes into UI code.

  <fig>                
    <image href="img/command_id_to_action_id.png">
    </image>
  </fig>

We can get all command ID from model header file.

  <fig>                
    <image href="img/main_menu_command_id.png">
    </image>
  </fig>
-- Update command string in IndexToCommand_en/cn.h file
This step is optional. With the model version v4.8.0 or later, when a command is recognized, its string will be returned from model file. Currently firmware does not reference IndexToCommand_en/cn.h files.

-- Finally Flash programs into board.
rebuild Smart Panel App and flash binary at 0x30100000.
Flash home_panel_resource.bin binrary at 0x30800000.
Note: Assume only one App runs on board.

# Barge-in support

When audio player is playing back, AEC feature is enabled automatically to support Barge-in feature.

# Multilanguage support

VIT does not support listening for multiple wake words from different languages at the same time, as it is the case with DSMT. 
Hence, you will be able to say only one wake word at a time. To change to a different language, use the language menu from the display
