// SPDX-License-Identifier: MIT
// Copyright 2022 NXP

//
// Only include this file in the custom.c for ui strings
//
extern const char *s_ScreenName[kLanguage_Ids][kScreen_Num + 1];

//
// Home screen
//
extern const char *s_CalendarString[kLanguage_Ids][1];

//
// Audio Player screen
//
extern const char *s_AudioPlayerTitleStr[kLanguage_Ids][1];

//
// Security screen
//

extern const char *s_SecurityControlStr[kLanguage_Ids][1];

extern const char *s_DoorStr[kLanguage_Ids][kSecurity_Last];
extern const char *s_SecurityStateStr[kLanguage_Ids][kSecurity_On + 1];
extern const char *s_AccessGrantedStr[kLanguage_Ids][1];
extern const char *s_PincodeHintStr[kLanguage_Ids][kSecurity_Auth_ConfirmPin + 3];
extern const char *s_CountDownStr[kLanguage_Ids][kSecurity_On + 1];
extern const char *s_ArmedStateStr[kLanguage_Ids][kSecurity_On + 1];

//
// Thermostat screen
//
extern const char *s_DeleteInteractionStr[kLanguage_Ids][1];
extern const char *s_ThermostatTitleStr[kLanguage_Ids][1];
extern const char *s_UserListStr[kLanguage_Ids][1];
extern const char *s_UserInteractionStr[kLanguage_Ids][kUserState_Max];
extern const char *s_BtnLabelStr[kLanguage_Ids][BTN_LABEL_INVALID];
//
// Thermostat Add User screen
//

//
// Thermostat screen user list
//
extern const char *s_ModeStr[kLanguage_Ids][kThermostat_NumMode];

extern const char *s_FanSpeedStr[kLanguage_Ids][kThermostat_NumFanSpeed];


extern const char *s_UserNameErrTitleStr[kLanguage_Ids];
extern const char *s_UserNameErrContentStr[kLanguage_Ids][USER_NAME_ERR_NO];
