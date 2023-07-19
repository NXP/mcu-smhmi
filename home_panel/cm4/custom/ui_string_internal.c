
#include "custom.h"
#include "ui_strings_internal.h"

const char *s_ScreenName[kLanguage_Ids][kScreen_Num + 1] = {
    [kLanguage_EN] = {[kScreen_Home]        = "Home ",
                      [kScreen_Thermostat]  = "Thermostat",
                      [kScreen_Security]    = "Security",
                      [kScreen_AudioPlayer] = "Audio Player",
                      [kScreen_Num]         = ""},
    [kLanguage_CN] = {[kScreen_Home]        = "确定 ",
                      [kScreen_Thermostat]  = "温控系统",
                      [kScreen_Security]    = "安防系统",
                      [kScreen_AudioPlayer] = "娱乐系统",
                      [kScreen_Num]         = ""},

};

//
// Home screen
//

const char *s_CalendarString[kLanguage_Ids][1] = {
    [kLanguage_EN] = {"Monday, May 18"},
    [kLanguage_CN] = {"星期一， 五月十八"},
};

//
// Audio Player screen
//
const char *s_AudioPlayerTitleStr[kLanguage_Ids][1] = {
    [kLanguage_EN] =
        {
            "Audio Player",
        },
    [kLanguage_CN] =
        {
            "娱乐系统",
        },
};

//
// Security screen
//

const char *s_SecurityControlStr[kLanguage_Ids][1] = {
    [kLanguage_EN] =
        {
            "SECURITY CONTROL",
        },
    [kLanguage_CN] =
        {
            "安防系统",
        },
};

const char *s_DoorStr[kLanguage_Ids][kSecurity_Last] = {
    [kLanguage_EN] =
        {
            [kSecurity_Front]  = "FrontDoor",
            [kSecurity_Back]   = "Back Door",
            [kSecurity_Garage] = "Garage Door",
        },
    [kLanguage_CN] =
        {
            [kSecurity_Front]  = "前门",
            [kSecurity_Back]   = "后门",
            [kSecurity_Garage] = "车库门",
        },
};

const char *s_SecurityStateStr[kLanguage_Ids][kSecurity_On + 1] = {
    [kLanguage_EN] =
        {
            [kSecurity_Uninited] = "System Disarmed",
            [kSecurity_Off]      = "System Disarmed",
            [kSecurity_On]       = "System Armed",
        },
    [kLanguage_CN] =
        {
            [kSecurity_Uninited] = "安防系统已关闭",
            [kSecurity_Off]      = "安防系统已关闭",
            [kSecurity_On]       = "安防系统已启动",
        },
};

const char *s_AccessGrantedStr[kLanguage_Ids][1] = {
    [kLanguage_EN] = { "Access Granted", },
    [kLanguage_CN] = { "已授权用户", },
};

const char *s_PincodeHintStr[kLanguage_Ids][kSecurity_Auth_ConfirmPin + 3] = {
    [kLanguage_EN] =
        {
            "Set PIN to arm your system",
            "Enter PIN to arm your system",
            "Enter PIN to disarm your system",
            "Enter PIN to delete User",
            "Enter current PIN",
            "Enter new PIN",
            "Confirm new PIN",
        },
    [kLanguage_CN] =
        {
            "设置密码",
            "输入密码",
            "输入密码",
            "输入密码",
            "输入当前密码",
            "输入新的密码",
            "再次输入新的密码",
        },
};

const char *s_CountDownStr[kLanguage_Ids][kSecurity_On + 1] = {
    [kLanguage_EN] =
        {
            [kSecurity_Uninited] = "",
            [kSecurity_Off]      = "Arming in %d seconds",
            [kSecurity_On]       = "Disarming in %d seconds",
        },
    [kLanguage_CN] =
        {
            [kSecurity_Uninited] = "",
            [kSecurity_Off]      = "%d秒后启动安防系统",
            [kSecurity_On]       = "%d秒后关闭安防系统",
        },
};

const char *s_ArmedStateStr[kLanguage_Ids][kSecurity_On + 1] = {
    [kLanguage_EN] =
        {
            [kSecurity_Uninited] = "",
            [kSecurity_Off]      = "Your Home is now armed and secure",
            [kSecurity_On]       = "You home is now disarmed",
        },
    [kLanguage_CN] =
        {
            [kSecurity_Uninited] = "",
            [kSecurity_Off]      = "安防系统已启动",
            [kSecurity_On]       = "安防系统已关闭",
        },
};

//
// Thermostat screen
//

const char *s_ThermostatTitleStr[kLanguage_Ids][1] = {
    [kLanguage_EN] =
        {
            "THERMOSTAT",
        },
    [kLanguage_CN] =
        {
            "温控系统",
        },
};
const char *s_DeleteInteractionStr[kLanguage_Ids][1] = {
    [kLanguage_EN] =
        {
            "Delete selected user?",
        },
    [kLanguage_CN] =
        {
            "删除选择的用户",
        },
};

const char *s_UserListStr[kLanguage_Ids][1] = {
    [kLanguage_EN] =
        {
            "User List",
        },
    [kLanguage_CN] =
        {
            "用户列表",
        },
};

const char *s_UserInteractionStr[kLanguage_Ids][kUserState_Max] = {
    [kLanguage_EN] = {[kUserState_NoUser]  = "No user",
                      [kUserState_Known]   = "User identified",
                      [kUserState_Unknown] = "New user detected"},

    [kLanguage_CN] = {[kUserState_NoUser]  = "没有检测到用户",
                      [kUserState_Known]   = "已注册的用户",
                      [kUserState_Unknown] = "新的用户"},
};

//
// Thermostat screen user list
//

const char *s_ModeStr[kLanguage_Ids][kThermostat_NumMode] = {
    [kLanguage_EN] =
        {
            [kThermostat_Auto] = "Auto",
            [kThermostat_Cool] = "Cool",
            [kThermostat_Heat] = "Heat",
        },
    [kLanguage_CN] =
        {
            [kThermostat_Auto] = "自动",
            [kThermostat_Cool] = "冷气",
            [kThermostat_Heat] = "暖气",
        },
};
const char *s_FanSpeedStr[kLanguage_Ids][kThermostat_NumFanSpeed] = {

    [kLanguage_EN] =
        {
            [kThermostat_Fan_Low]  = "Slow",
            [kThermostat_Fan_Mid]  = "Middle",
            [kThermostat_Fan_High] = "Fast",
            [kThermostat_Fan_Auto] = "Auto",
        },
    [kLanguage_CN] =
        {
            [kThermostat_Fan_Low]  = "慢速",
            [kThermostat_Fan_Mid]  = "中速",
            [kThermostat_Fan_High] = "快速",
            [kThermostat_Fan_Auto] = "自动",
        },

};

const char *s_UserNameErrTitleStr[kLanguage_Ids] = {
	[kLanguage_EN] =
		{
			"Error!",
		},
	[kLanguage_CN] =
		{
			"错误!",
		},
};
const char *s_UserNameErrContentStr[kLanguage_Ids][USER_NAME_ERR_NO] = {
	[kLanguage_EN] =
		{
			[USER_NAME_ERR_EMPTY] 	 = "Empty user name!",
			[USER_NAME_ERR_ENTER]    = "'Enter' not allowed for user name!",
			[USER_NAME_ERR_REPEATED] = "Repeated user name!"
		},
	[kLanguage_CN] =
		{
			[USER_NAME_ERR_EMPTY] 	 = "空用户名!",
			[USER_NAME_ERR_ENTER]    = "用户名不能包含回车键!",
			[USER_NAME_ERR_REPEATED] = "用户名重复!"
		},
};

const char *s_BtnLabelStr[kLanguage_Ids][BTN_LABEL_INVALID] = {
	[kLanguage_EN] =
		{
			[BTN_LABEL_CONFIRM]	= "Confirm",
			[BTN_LABEL_CANCEL]  = "Cancel",
		},
	[kLanguage_CN] =
		{
			[BTN_LABEL_CONFIRM]	= "确定",
			[BTN_LABEL_CANCEL]  = "取消",
		},
};
