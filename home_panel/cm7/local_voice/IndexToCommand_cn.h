#ifndef INDEXCOMMANDS_CN_H_
#define INDEXCOMMANDS_CN_H_

#include "IndexCommands.h"

char *ww_cn[] = {
        "你好 恩智浦",
};

char *cmd_hp_home_menu_cn[] = {
       [kHpHomeMenuActionThermostat] =  "温控系统",
       [kHpHomeMenuActionSecurity] = "安全系统",
       [kHpHomeMenuActionAudioPlayer] "娱乐系统",
};

char *cmd_hp_thermostat_cn[] = {
       [kHpThermostatActionMainMenu] = "主菜单",
       [kHpThermostatActionGoBack] = "返回",
       [kHpThermostatActionSetCoolMode] = "制冷模式",
       [kHpThermostatActionSetAutoMode] = "自动模式",
       [kHpThermostatActionSetHeatMode] =  "制热模式",
       [kHpThermostatActionThermostatOff] = "关机",
       [kHpThermostatActionSlow] =  "低速风",
       [kHpThermostatActionMedium] =  "中速风",
       [kHpThermostatActionFast] =  "高速风",
       [kHpThermostatActionEighteen] = "十八度",
       [kHpThermostatActionNineteen] =  "十九度",
       [kHpThermostatActionTwenty]=  "二十度",
       [kHpThermostatActioTwentyOne] =  "二十一度",
       [kHpThermostatActionTemperatureUp] =  "升高温度",
       [kHpThermostatActionTemperatureDown] =  "降低温度"
};

char *cmd_hp_security_cn[] = {
        [kHpSecurityActionMainMenu] = "主菜单",
        [kHpSecurityActionGoBack] = "返回",
        [kHpSecurityActionActivate] = "启动安全系统",
        [kHpSecurityActionDisable] = "关闭安全系统",
        [kHpSecurityActionOpenFrontDoor] ="打开前门",
        [kHpSecurityActionCloseFrontDoor] = "关闭前门",
        [kHpSecurityActionOpenBackDoor] = "打开后门",
        [kHpSecurityActionCloseBackDoor] = "关闭后门",
        [kHpSecurityActionOpenGarageDoor] = "打开车库门",
        [kHpSecurityActionCloseGarageDoor] = "关闭车库门"
};

char *cmd_hp_audio_player_cn[] = {
        [kHpAudioPlayerActionMainMenu] = "主菜单",
        [kHpAudioPlayerActionGoBack] = "返回",
        [kHpAudioPlayerActionNextSong] = "下一首",
        [kHpAudioPlayerActionPreviousSong] = "上一曲",
        [kHpAudioPlayerActionPlay] = "播放",
        [kHpAudioPlayerActionPause] = "暂停",
        [kHpAudioPlayerActionVolumeUp] = "增大音量",
        [kHpAudioPlayerActionVolumeDown] = "减小音量",
        [kHpAudioPlayerActionMute] = "静音",
        [kHpAudioPlayerActionMaxVolume] = "最大音量"
};

#endif /* INDEXCOMMANDS_ZH_H_ */
