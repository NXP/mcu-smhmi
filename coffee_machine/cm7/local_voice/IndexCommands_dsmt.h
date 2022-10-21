/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef INDEXCOMMANDS_DSMT_H_
#define INDEXCOMMANDS_DSMT_H_

#include "IndexToCommand_en.h"
#include "IndexToCommand_cn.h"
#include "IndexToCommand_de.h"
#include "IndexToCommand_fr.h"

/* These defines are currently used only for displaying in the commands shell */
#define NUMBER_OF_COFFEE_MACHINE_CMDS_EN (sizeof(action_coffee_machine_en) / sizeof(action_coffee_machine_en[0]))
#define NUMBER_OF_COFFEE_MACHINE_CMDS_CN (sizeof(action_coffee_machine_cn) / sizeof(action_coffee_machine_cn[0]))
#define NUMBER_OF_COFFEE_MACHINE_CMDS_FR (sizeof(action_coffee_machine_fr) / sizeof(action_coffee_machine_fr[0]))
#define NUMBER_OF_COFFEE_MACHINE_CMDS_DE (sizeof(action_coffee_machine_de) / sizeof(action_coffee_machine_de[0]))

unsigned int action_coffee_machine_en[] = {
    kCoffeeMachineActionStart,      // "Start"
    kCoffeeMachineActionCancel,     // "Cancel"
    kCoffeeMachineActionConfirm,    // "Confirm"
    kCoffeeMachineActionConfirm,    // "Confirm"
    kCoffeeMachineActionConfirm,    // "Confirm"
    kCoffeeMachineActionConfirm,    // "Confirm"
    kCoffeeMachineActionConfirm,    // "Confirm"
    kCoffeeMachineActionConfirm,    // "Confirm"
    kCoffeeMachineActionConfirm,    // "Confirm"
    kCoffeeMachineActionConfirm,    // "Confirm"
    kCoffeeMachineActionEspresso,   // "Espresso"
    kCoffeeMachineActionAmericano,  // "Americano"
    kCoffeeMachineActionCappuccino, // "Cappuccino"
    kCoffeeMachineActionLatte,      // "Cafe latte"
    kCoffeeMachineActionLatte,      // "Cafe latte"
    kCoffeeMachineActionSmall,      // "Small"
    kCoffeeMachineActionMedium,     // "Medium"
    kCoffeeMachineActionLarge,      // "Large"
    kCoffeeMachineActionSoft,       // "Soft"
    kCoffeeMachineActionSoft,       // "Soft"
    kCoffeeMachineActionMild,       // "Mild"
    kCoffeeMachineActionStrong,     // "Strong"
    kCoffeeMachineActionDeregister, // "Delete User"
};

unsigned int action_coffee_machine_fr[] = {
    kCoffeeMachineActionStart,      // "Commencer"
    kCoffeeMachineActionCancel,     // "Annuler"
    kCoffeeMachineActionConfirm,    // "Confirmer"
    kCoffeeMachineActionEspresso,   // "Espresso"
    kCoffeeMachineActionAmericano,  // "Café Americano"
    kCoffeeMachineActionCappuccino, // "Cappuccino"
    kCoffeeMachineActionCappuccino, // "Cappuccino"
    kCoffeeMachineActionLatte,      // "Café Latté"
    kCoffeeMachineActionSmall,      // "Court"
    kCoffeeMachineActionMedium,     // "Moyen"
    kCoffeeMachineActionLarge,      // "Long"
    kCoffeeMachineActionSoft,       // "Léger"
    kCoffeeMachineActionMild,       // "Normal"
    kCoffeeMachineActionStrong,     // "Serré"
    kCoffeeMachineActionStrong,     // "Serré"
    kCoffeeMachineActionDeregister, // "Supprimer préférence"
};

unsigned int action_coffee_machine_cn[] = {
    kCoffeeMachineActionStart,      //开始
    kCoffeeMachineActionCancel,     //取消
    kCoffeeMachineActionCancel,     //不需要 谢谢
    kCoffeeMachineActionCancel,     //不需要 谢谢
    kCoffeeMachineActionCancel,     //不需要 谢谢
    kCoffeeMachineActionCancel,     //不需要 谢谢
    kCoffeeMachineActionCancel,     //不用 谢谢
    kCoffeeMachineActionCancel,     //不用 谢谢
    kCoffeeMachineActionCancel,     //不用 谢谢
    kCoffeeMachineActionCancel,     //不用 谢谢
    kCoffeeMachineActionCancel,     // No Thank you
    kCoffeeMachineActionCancel,     // No Thank you~1
    kCoffeeMachineActionCancel,     // No Thanks
    kCoffeeMachineActionConfirm,    //确定
    kCoffeeMachineActionConfirm,    //完成
    kCoffeeMachineActionEspresso,   //意式咖啡
    kCoffeeMachineActionAmericano,  //美式咖啡
    kCoffeeMachineActionCappuccino, //卡布奇诺
    kCoffeeMachineActionLatte,      //拿铁
    kCoffeeMachineActionSmall,      //小杯
    kCoffeeMachineActionMedium,     //中杯
    kCoffeeMachineActionMedium,     //中杯
    kCoffeeMachineActionLarge,      //大杯
    kCoffeeMachineActionSoft,       //柔润
    kCoffeeMachineActionMild,       //温和
    kCoffeeMachineActionStrong,     //浓烈
    kCoffeeMachineActionDeregister, //取消注册"
};

unsigned int action_coffee_machine_de[] = {
    kCoffeeMachineActionStart,      // "Starten"
    kCoffeeMachineActionCancel,     // "Abbrechen"
    kCoffeeMachineActionConfirm,    // "Bestätigen"
    kCoffeeMachineActionEspresso,   // "Espresso"
    kCoffeeMachineActionAmericano,  // "Americano"
    kCoffeeMachineActionCappuccino, // "Cappuccino"
    kCoffeeMachineActionLatte,      // "Kaffee Latte"
    kCoffeeMachineActionSmall,      // "Klein"
    kCoffeeMachineActionMedium,     // "Mittel"
    kCoffeeMachineActionLarge,      // "Groß"
    kCoffeeMachineActionLarge,      // "Groß"
    kCoffeeMachineActionSoft,       // "Leicht"
    kCoffeeMachineActionMild,       // "Mild"
    kCoffeeMachineActionStrong,     // "Stark"
    kCoffeeMachineActionDeregister, // "Abmelden"
};

char **get_cmd_strings(asr_language_t asrLang, asr_inference_t infCMDType)
{
    char **retString = NULL;

    switch (asrLang)
    {
        case ASR_CHINESE:
            if (infCMDType == ASR_CMD_COFFEE_MACHINE)
            {
                retString = cmd_coffee_machine_cn;
            }
            break;
        case ASR_FRENCH:
            if (infCMDType == ASR_CMD_COFFEE_MACHINE)
            {
                retString = cmd_coffee_machine_fr;
            }
            break;
        case ASR_ENGLISH:
            if (infCMDType == ASR_CMD_COFFEE_MACHINE)
            {
                retString = cmd_coffee_machine_en;
            }
            break;
        case ASR_GERMAN:
            if (infCMDType == ASR_CMD_COFFEE_MACHINE)
            {
                retString = cmd_coffee_machine_de;
            }
            break;
        default:
            retString = cmd_coffee_machine_en;
            break;
    }

    return retString;
}
#endif /* INDEXCOMMANDS_DSMT_H_ */
