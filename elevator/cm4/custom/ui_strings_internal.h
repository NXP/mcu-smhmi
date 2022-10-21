// SPDX-License-Identifier: MIT
// Copyright 2022 NXP

#ifndef _UI_STRINGS_INTERNAL_H_
#define _UI_STRINGS_INTERNAL_H_

//
// Only include this file in the custom.c for ui strings
//
static char *s_ScreenName[kScreen_Num + 1] = {"Main", "Home", "Help", ""};

//
// Home screen
//
static const char *s_ConfirmStr[kLanguageType_NumTypes][1] = {
    {
        "Confirm",
    },
    {
        "确定",
    },
    {
        "Bestätigen",
    },
    {
        "Confirmer",
    },
};

static const char *s_CancelStr[kLanguageType_NumTypes][1] = {
    {
        "Cancel",
    },
    {
        "取消",
    },
    {
        "Abbrechen",
    },
    {
        "Annuler",
    },
};

static const char *s_HelpScreenStr[kLanguageType_NumTypes][1] = {
    {
        "Help Screen",
    },
    {
        "帮助",
    },
    {
        "Hilfe",
    },
    {
        "Aider",
    },
};

static const char *s_FaceRecStateStr[kLanguageType_NumTypes][kFaceRec_NumStates] = {
    {"No user detected", "New user detected", "User identified"},
    {"没有检测到用户", "新的用户", "已注册的用户"},
    {"Kein Benutzer erkannt", "Neuer Benutzer erkannt", "Benutzer identifiziert"},
    {"Aucun utilisateur détecté", "Nouvel utilisateur détecté", "Utilisateur identifié"},
};

static const char *s_ArrivedStr[kLanguageType_NumTypes][1] = {
    {
        "YOU HAVE ARRIVED",
    },
    {
        "你已经到达",
    },
    {
        "DU BIST ANGEKOMMEN",
    },
    {
        "VOUS ÊTES ARRIVÊS",
    },
};

//
// Help screen
//

static const char *s_TipsStr[kLanguageType_NumTypes][3] = {
    {"Say \"Hey NXP\"", "to wake up the", "voice command"},
    {"请说：“你好，恩智浦”", "来唤醒语音控制", ""},
    {"Sagen \"Hallo NXP\"", "um den Sprachbefehl", "aufzuwecken"},
    {"Dites \"Salut NXP\"", "activer la", "commande vocale"},
};

static const char *s_BackToHomeStr[kLanguageType_NumTypes][1] = {
    {
        "Back to home",
    },
    {
        "返回",
    },
    {
        "Der Rücken",
    },
    {
        "Menu principal",
    },
};

static const char *s_VoiceCommandsStr[kLanguageType_NumTypes][1] = {
    {
        "Voice commands",
    },
    {
        "语音命令",
    },
    {
        "Sprachbefehle",
    },
    {
        "Commandes vocales",
    },
};

static const char *s_VoiceCommandsListStr[kLanguageType_NumTypes][9] = {
    {"First floor", "Second floor", "Third floor", "Fourth floor", "Fifth floor", "Sixth floor", "Confirm", "Cancel",
     "Delete user"},
    {"一楼", "二楼", "三楼", "四楼", "五楼", "六楼", "确认", "取消", "删除用户"},
    {"Erste etage", "Zweite etage", "Dritte etage", "Vierte etage", "Fünfte etage", "Sechste etage", "Bestätigen",
     "Abbrechen", "Abmelden"},
    {"Premier etage", "Deuxième etage", "Troisième etage", "Quatrième etage", "Cinquième etage", "Sixième etage",
     "Confirmer", "Annuler", "Supprimer préférence"},
};

#endif /**/
