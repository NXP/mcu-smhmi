/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef INDEXCOMMANDS_H_
#define INDEXCOMMANDS_H_

#define ENABLE_COFFEE_MACHINE 1

/* Number of Voice Commands Models */
#define CMD_MODELS_COUNT (1)

enum _coffee_machine_action
{
    kCoffeeMachineActionStart      = 0,
    kCoffeeMachineActionCancel     = 1,
    kCoffeeMachineActionConfirm    = 2,
    kCoffeeMachineActionEspresso   = 3,
    kCoffeeMachineActionAmericano  = 4,
    kCoffeeMachineActionCappuccino = 5,
    kCoffeeMachineActionLatte      = 6,
    kCoffeeMachineActionSmall      = 7,
    kCoffeeMachineActionMedium     = 8,
    kCoffeeMachineActionLarge      = 9,
    kCoffeeMachineActionSoft       = 10,
    kCoffeeMachineActionMild       = 11,
    kCoffeeMachineActionStrong     = 12,
    kCoffeeMachineActionDeregister = 13,
    kCoffeeMachineActionInvalid
};

#ifdef ENABLE_DSMT_ASR
#include "IndexCommands_dsmt.h"
#elif defined(ENABLE_VIT_ASR)
#include "IndexCommands_vit.h"
#endif /* ENABLE_DSMT_ASR */

unsigned int get_cmd_number(asr_language_t asrLang, asr_inference_t infCMDType)
{
    unsigned int cmd_number = 0;

    switch (asrLang)
    {
        case ASR_CHINESE:
            if (infCMDType == ASR_CMD_COFFEE_MACHINE)
            {
                cmd_number = NUMBER_OF_COFFEE_MACHINE_CMDS_CN;
            }
            else if (infCMDType == ASR_CMD_USER_REGISTER)
            {
                cmd_number = NUMBER_OF_COFFEE_MACHINE_CMDS_CN;
            }
            break;
        case ASR_FRENCH:
            if (infCMDType == ASR_CMD_COFFEE_MACHINE)
            {
                cmd_number = NUMBER_OF_COFFEE_MACHINE_CMDS_FR;
            }
            else if (infCMDType == ASR_CMD_USER_REGISTER)
            {
                cmd_number = NUMBER_OF_COFFEE_MACHINE_CMDS_FR;
            }
            break;
        case ASR_ENGLISH:
            if (infCMDType == ASR_CMD_COFFEE_MACHINE)
            {
                cmd_number = NUMBER_OF_COFFEE_MACHINE_CMDS_EN;
            }
            else if (infCMDType == ASR_CMD_USER_REGISTER)
            {
                cmd_number = NUMBER_OF_COFFEE_MACHINE_CMDS_EN;
            }
            break;
        case ASR_GERMAN:
            if (infCMDType == ASR_CMD_COFFEE_MACHINE)
            {
                cmd_number = NUMBER_OF_COFFEE_MACHINE_CMDS_DE;
            }
            else if (infCMDType == ASR_CMD_USER_REGISTER)
            {
                cmd_number = NUMBER_OF_COFFEE_MACHINE_CMDS_DE;
            }
            break;
        default:
            cmd_number = NUMBER_OF_COFFEE_MACHINE_CMDS_EN;
            break;
    }

    return cmd_number;
}

unsigned int get_action_index_from_keyword(asr_language_t asrLang, asr_inference_t infCMDType, unsigned int keywordId)
{
    unsigned int action_index = 0;

    switch (asrLang)
    {
        case ASR_CHINESE:
            if (infCMDType == ASR_CMD_COFFEE_MACHINE)
            {
                action_index = action_coffee_machine_cn[keywordId];
            }
            else if (infCMDType == ASR_CMD_USER_REGISTER)
            {
                action_index = action_coffee_machine_cn[keywordId];
            }
            break;
        case ASR_FRENCH:
            if (infCMDType == ASR_CMD_COFFEE_MACHINE)
            {
                action_index = action_coffee_machine_fr[keywordId];
            }
            else if (infCMDType == ASR_CMD_USER_REGISTER)
            {
                action_index = action_coffee_machine_fr[keywordId];
            }
            break;
        case ASR_ENGLISH:
            if (infCMDType == ASR_CMD_COFFEE_MACHINE)
            {
                action_index = action_coffee_machine_en[keywordId];
            }
            else if (infCMDType == ASR_CMD_USER_REGISTER)
            {
                action_index = action_coffee_machine_en[keywordId];
            }
            break;
        case ASR_GERMAN:
            if (infCMDType == ASR_CMD_COFFEE_MACHINE)
            {
                action_index = action_coffee_machine_de[keywordId];
            }
            else if (infCMDType == ASR_CMD_USER_REGISTER)
            {
                action_index = action_coffee_machine_de[keywordId];
            }
            break;
        default:
            action_index = kCoffeeMachineActionInvalid;
            break;
    }

    return action_index;
}

#endif /* INDEXCOMMANDS_H_ */
