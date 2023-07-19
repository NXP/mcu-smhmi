/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef INDEXCOMMANDS_VIT_H_
#define INDEXCOMMANDS_VIT_H_

/* These defines are currently used only for displaying in the commands shell */
#define NUMBER_OF_HP_MAIN_MENU_CMDS_CN      sizeof(action_hp_main_menu_cn)/sizeof(action_hp_main_menu_cn[0])
#define NUMBER_OF_HP_THERMOSTAT_CMDS_CN     sizeof(action_hp_thermostat_cn)/sizeof(action_hp_thermostat_cn[0])
#define NUMBER_OF_HP_SECURITY_CMDS_CN       sizeof(action_hp_security_cn)/sizeof(action_hp_security_cn[0])
#define NUMBER_OF_HP_AUDIO_PLAYER_CMDS_CN   sizeof(action_hp_audio_player_cn)/sizeof(action_hp_audio_player_cn[0])

#define NUMBER_OF_HP_MAIN_MENU_CMDS_EN      sizeof(action_hp_main_menu_en)/sizeof(action_hp_main_menu_en[0])
#define NUMBER_OF_HP_THERMOSTAT_CMDS_EN     sizeof(action_hp_thermostat_en)/sizeof(action_hp_thermostat_en[0])
#define NUMBER_OF_HP_SECURITY_CMDS_EN       sizeof(action_hp_security_en)/sizeof(action_hp_security_en[0])
#define NUMBER_OF_HP_AUDIO_PLAYER_CMDS_EN   sizeof(action_hp_audio_player_en)/sizeof(action_hp_audio_player_en[0])

unsigned int action_hp_main_menu_cn[] = {
	kHpHomeMenuActionInvalid,
    kHpHomeMenuActionThermostat,
    kHpHomeMenuActionSecurity,
    kHpHomeMenuActionAudioPlayer
};

unsigned int action_hp_thermostat_cn[] = {
	kHpThermostatActionInvalid,
    kHpThermostatActionMainMenu,
    kHpThermostatActionGoBack,
    kHpThermostatActionSetCoolMode,
    kHpThermostatActionSetAutoMode,
    kHpThermostatActionSetHeatMode,
    kHpThermostatActionThermostatOn,
    kHpThermostatActionThermostatOff,
    kHpThermostatActionSlow,
    kHpThermostatActionMedium,
    kHpThermostatActionFast,
    kHpThermostatActionAuto,
    kHpThermostatActionFanOn,
    kHpThermostatActionFanOff,
    kHpThermostatActionEighteen,
    kHpThermostatActionNineteen,
    kHpThermostatActionTwenty,
    kHpThermostatActioTwentyOne,
    kHpThermostatActionTemperatureUp,
    kHpThermostatActionTemperatureDown,
    kHpThermostatActionConfirm,
    kHpThermostatActionCancel,
    kHpThermostatActionInvalid
};

unsigned int action_hp_security_cn[] = {
	kHpSecurityActionInvalid,
    kHpSecurityActionMainMenu,
    kHpSecurityActionGoBack,
    kHpSecurityActionActivate,
    kHpSecurityActionDisable,
    kHpSecurityActionOpenFrontDoor,
    kHpSecurityActionCloseFrontDoor,
    kHpSecurityActionOpenBackDoor,
    kHpSecurityActionCloseBackDoor,
    kHpSecurityActionOpenGarageDoor,
    kHpSecurityActionCloseGarageDoor
};

unsigned int action_hp_audio_player_cn[] = {
	kHpAudioPlayerActionInvalid,
    kHpAudioPlayerActionMainMenu,
    kHpAudioPlayerActionGoBack,
    kHpAudioPlayerActionNextSong,
    kHpAudioPlayerActionPreviousSong,
    kHpAudioPlayerActionPlay,
    kHpAudioPlayerActionPause,
    kHpAudioPlayerActionVolumeUp,
    kHpAudioPlayerActionVolumeDown,
    kHpAudioPlayerActionMute,
    kHpAudioPlayerActionMaxVolume
};

unsigned int action_hp_main_menu_en[] = {
	kHpHomeMenuActionInvalid,
    kHpHomeMenuActionThermostat,
    kHpHomeMenuActionSecurity,
    kHpHomeMenuActionAudioPlayer
};

unsigned int action_hp_thermostat_en[] = {
	kHpHomeMenuActionInvalid,
    kHpThermostatActionMainMenu,
    kHpThermostatActionGoBack,
    kHpThermostatActionSetCoolMode,
    kHpThermostatActionSetAutoMode,
    kHpThermostatActionSetHeatMode,
    kHpThermostatActionThermostatOn,
    kHpThermostatActionThermostatOff,
    kHpThermostatActionSlow,
    kHpThermostatActionMedium,
    kHpThermostatActionFast,
    kHpThermostatActionAuto,
    kHpThermostatActionFanOn,
    kHpThermostatActionFanOff,
    kHpThermostatActionEighteen,
    kHpThermostatActionNineteen,
    kHpThermostatActionTwenty,
    kHpThermostatActioTwentyOne,
    kHpThermostatActionTemperatureUp,
    kHpThermostatActionTemperatureDown,
    kHpThermostatActionConfirm,
    kHpThermostatActionCancel,
    kHpThermostatActionInvalid
};

unsigned int action_hp_security_en[] = {
	kHpHomeMenuActionInvalid,
    kHpSecurityActionMainMenu,
    kHpSecurityActionGoBack,
    kHpSecurityActionActivate,
    kHpSecurityActionDisable,
    kHpSecurityActionOpenFrontDoor,
    kHpSecurityActionCloseFrontDoor,
    kHpSecurityActionOpenBackDoor,
    kHpSecurityActionCloseBackDoor,
    kHpSecurityActionOpenGarageDoor,
    kHpSecurityActionCloseGarageDoor
};

unsigned int action_hp_audio_player_en[] = {
	kHpHomeMenuActionInvalid,
    kHpAudioPlayerActionMainMenu,
    kHpAudioPlayerActionGoBack,
    kHpAudioPlayerActionNextSong,
    kHpAudioPlayerActionPreviousSong,
    kHpAudioPlayerActionPlay,
    kHpAudioPlayerActionPause,
    kHpAudioPlayerActionVolumeUp,
    kHpAudioPlayerActionVolumeDown,
    kHpAudioPlayerActionMute,
    kHpAudioPlayerActionMaxVolume
};

#endif /* INDEXCOMMANDS_VIT_H_ */
