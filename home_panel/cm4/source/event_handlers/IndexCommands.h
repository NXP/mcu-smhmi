/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef INDEXCOMMANDS_H_
#define INDEXCOMMANDS_H_

#define ENABLE_HOME_PANEL 1

/* Number of Voice Commands Models */
#define CMD_MODELS_COUNT (4)

enum _hp_home_menu_action
{
    kHpHomeMenuActionThermostat,
    kHpHomeMenuActionSecurity,
    kHpHomeMenuActionAudioPlayer,
    kHpHomeMenuActionInvalid
};

enum _hp_thermostat_action
{
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

enum _hp_security_action
{
    kHpSecurityActionMainMenu,
    kHpSecurityActionGoBack,
    kHpSecurityActionActivate,
    kHpSecurityActionDisable,
    kHpSecurityActionOpenFrontDoor,
    kHpSecurityActionCloseFrontDoor,
    kHpSecurityActionOpenBackDoor,
    kHpSecurityActionCloseBackDoor,
    kHpSecurityActionOpenGarageDoor,
    kHpSecurityActionCloseGarageDoor,
    kHpSecurityActionInvalid
};

enum _hp_audio_player_action
{
    kHpAudioPlayerActionMainMenu,
    kHpAudioPlayerActionGoBack,
    kHpAudioPlayerActionNextSong,
    kHpAudioPlayerActionPreviousSong,
    kHpAudioPlayerActionPlay,
    kHpAudioPlayerActionPause,
    kHpAudioPlayerActionVolumeUp,
    kHpAudioPlayerActionVolumeDown,
    kHpAudioPlayerActionMute,
    kHpAudioPlayerActionMaxVolume,
    kHpAudioPlayerActionInvalid
};

extern unsigned int get_cmd_number(asr_language_t asrLang, asr_inference_t infCMDType);
extern unsigned int get_action_index_from_keyword(asr_language_t asrLang, asr_inference_t infCMDType, unsigned int keywordId);

#endif /* INDEXCOMMANDS_H_ */
