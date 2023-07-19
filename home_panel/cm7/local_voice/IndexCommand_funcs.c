/*
 * Copyright 2023 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */
#include "IndexCommands.h"
#include "IndexCommands_vit.h"

unsigned int get_cmd_number(asr_language_t asrLang, asr_inference_t infCMDType)
{
    unsigned int cmd_number = 0;

    switch (asrLang)
    {
        case ASR_CHINESE:
            if (infCMDType == ASR_CMD_HP_MAIN_MENU)
            {
                cmd_number = NUMBER_OF_HP_MAIN_MENU_CMDS_CN;
            }
            else if (infCMDType == ASR_CMD_HP_THERMOSTAT)
            {
                cmd_number = NUMBER_OF_HP_THERMOSTAT_CMDS_CN;
            }
            else if (infCMDType == ASR_CMD_HP_SECURITY)
            {
                cmd_number = NUMBER_OF_HP_SECURITY_CMDS_CN;
            }
            else if (infCMDType == ASR_CMD_HP_AUDIO_PLAYER)
            {
                cmd_number = NUMBER_OF_HP_AUDIO_PLAYER_CMDS_CN;
            }
            break;
        case ASR_ENGLISH:
            if (infCMDType == ASR_CMD_HP_MAIN_MENU)
            {
                cmd_number = NUMBER_OF_HP_MAIN_MENU_CMDS_EN;
            }
            else if (infCMDType == ASR_CMD_HP_THERMOSTAT)
            {
                cmd_number = NUMBER_OF_HP_THERMOSTAT_CMDS_EN;
            }
            else if (infCMDType == ASR_CMD_HP_SECURITY)
            {
                cmd_number = NUMBER_OF_HP_SECURITY_CMDS_EN;
            }
            else if (infCMDType == ASR_CMD_HP_AUDIO_PLAYER)
            {
                cmd_number = NUMBER_OF_HP_AUDIO_PLAYER_CMDS_EN;
            }
            break;
        default:
            cmd_number = NUMBER_OF_HP_MAIN_MENU_CMDS_EN;
            break;
    }

    return cmd_number;
}

unsigned int get_action_index_from_keyword(asr_language_t asrLang, asr_inference_t infCMDType, unsigned int keywordId)
{
    unsigned int action_index = 0;
    unsigned int maxCmdNumber = get_cmd_number(asrLang, infCMDType);

    if (keywordId >= maxCmdNumber)
    {
        asrLang = LAST_LANGUAGE;
    }

    switch (asrLang)
    {
        case ASR_CHINESE:
            if (infCMDType == ASR_CMD_HP_MAIN_MENU)
            {
                action_index = action_hp_main_menu_cn[keywordId];
            }
            else if (infCMDType == ASR_CMD_HP_THERMOSTAT)
            {
                action_index = action_hp_thermostat_cn[keywordId];
            }
            else if (infCMDType == ASR_CMD_HP_SECURITY)
            {
                action_index = action_hp_security_cn[keywordId];
            }
            else if (infCMDType == ASR_CMD_HP_AUDIO_PLAYER)
            {
                action_index = action_hp_audio_player_cn[keywordId];
            }
            break;
        case ASR_ENGLISH:
            if (infCMDType == ASR_CMD_HP_MAIN_MENU)
            {
                action_index = action_hp_main_menu_en[keywordId];
            }
            else if (infCMDType == ASR_CMD_HP_THERMOSTAT)
            {
                action_index = action_hp_thermostat_en[keywordId];
            }
            else if (infCMDType == ASR_CMD_HP_SECURITY)
            {
                action_index = action_hp_security_en[keywordId];
            }
            else if (infCMDType == ASR_CMD_HP_AUDIO_PLAYER)
            {
                action_index = action_hp_audio_player_en[keywordId];
            }
            break;
        default:
            action_index = kHpHomeMenuActionInvalid;
            break;
    }

    return action_index;
}
