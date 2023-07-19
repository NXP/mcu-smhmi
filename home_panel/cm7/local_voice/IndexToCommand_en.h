#ifndef INDEXCOMMANDS_EN_H_
#define INDEXCOMMANDS_EN_H_

#include "IndexCommands.h"

char *ww_en[] = {
        "Hey NXP",
};

char *cmd_hp_home_menu_en[] = {
        [kHpHomeMenuActionThermostat] = "THERMOSTAT",
        [kHpHomeMenuActionSecurity] = "SECURITY",
        [kHpHomeMenuActionAudioPlayer] = "AUDIO PLAYER",
};

char *cmd_hp_thermostat_en[] = {
        [kHpThermostatActionMainMenu] = "MAIN MENU",
        [kHpThermostatActionGoBack] = "GO BACK",
        [kHpThermostatActionSetCoolMode] ="SET COOL MODE",
        [kHpThermostatActionSetAutoMode] = "SET AUTO MODE",
        [kHpThermostatActionSetHeatMode] = "SET HEAT MODE",
        [kHpThermostatActionThermostatOff] = "TURN OFF THE THERMOSTAT",
        [kHpThermostatActionSlow] = "SMALL WIND",
        [kHpThermostatActionMedium] = "MEDIUM WIND",
        [kHpThermostatActionFast] = "BIG WIND",
        [kHpThermostatActionEighteen] = "EIGHTEEN DEGREE",
        [kHpThermostatActionNineteen] = "NINETEEN DEGREE",
        [kHpThermostatActionTwenty] = "TWENTY DEGREE",
        [kHpThermostatActioTwentyOne] = "TWENTY ONE DEGREE",
        [kHpThermostatActionTemperatureUp] = "TEMPERATURE HIGHER",
        [kHpThermostatActionTemperatureDown] = "TEMPERATURE LOWER"
};

char *cmd_hp_security_en[] = {
        [kHpSecurityActionMainMenu] = "MAIN MENU",
        [kHpSecurityActionGoBack] = "GO BACK",
        [kHpSecurityActionActivate] = "ACTIVATE SECURITY SYSTEM",
        [kHpSecurityActionDisable] = "DISABLE SECURITY SYSTEM",
        [kHpSecurityActionOpenFrontDoor] = "OPEN THE FRONT DOOR",
        [kHpSecurityActionCloseFrontDoor] = "CLOSE THE FRONT DOOR",
        [kHpSecurityActionOpenBackDoor] = "OPEN THE BACK DOOR",
        [kHpSecurityActionCloseBackDoor] = "CLOSE THE BACK DOOR",
        [kHpSecurityActionOpenGarageDoor] = "OPEN THE GARAGE DOOR",
        [kHpSecurityActionCloseGarageDoor] = "CLOSE THE GARAGE DOOR"
};

char *cmd_hp_audio_player_en[] = {
        [kHpAudioPlayerActionMainMenu] = "MAIN MENU",
        [kHpAudioPlayerActionGoBack] = "GO BACK",
        [kHpAudioPlayerActionNextSong] = "NEXT SONG",
        [kHpAudioPlayerActionPreviousSong] = "PREVIOUS SONG",
        [kHpAudioPlayerActionPlay] = "START PLAYER",
        [kHpAudioPlayerActionPause] = "PAUSE PLAYER",
        [kHpAudioPlayerActionVolumeUp] = "VOLUME UP",
        [kHpAudioPlayerActionVolumeDown] = "VOLUME DOWN",
        [kHpAudioPlayerActionInvalid] = ""
};

#endif /* INDEXCOMMANDS_EN_H_ */
