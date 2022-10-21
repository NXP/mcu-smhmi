/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef INDEXCOMMANDS_VIT_H_
#define INDEXCOMMANDS_VIT_H_

/* These defines are currently used only for displaying in the commands shell */
#define NUMBER_OF_COFFEE_MACHINE_CMDS_EN (sizeof(action_coffee_machine_en) / sizeof(action_coffee_machine_en[0]))
#define NUMBER_OF_COFFEE_MACHINE_CMDS_CN (sizeof(action_coffee_machine_cn) / sizeof(action_coffee_machine_cn[0]))
#define NUMBER_OF_COFFEE_MACHINE_CMDS_FR (sizeof(action_coffee_machine_fr) / sizeof(action_coffee_machine_fr[0]))
#define NUMBER_OF_COFFEE_MACHINE_CMDS_DE (sizeof(action_coffee_machine_de) / sizeof(action_coffee_machine_de[0]))

unsigned int action_coffee_machine_en[] = {
    kCoffeeMachineActionInvalid,    // unknown
    kCoffeeMachineActionStart,      // "START"
    kCoffeeMachineActionEspresso,   // "ESPRESSO"
    kCoffeeMachineActionAmericano,  // "AMERICANO"
    kCoffeeMachineActionCappuccino, // "CAPPUCCINO"
    kCoffeeMachineActionLatte,      // "CAFE LATTE"
    kCoffeeMachineActionSmall,      // "SMALL"
    kCoffeeMachineActionMedium,     // "MEDIUM"
    kCoffeeMachineActionLarge,      // "LARGE"
    kCoffeeMachineActionSoft,       // "SOFT"
    kCoffeeMachineActionMild,       // "MILD"
    kCoffeeMachineActionStrong,     // "STRONG"
    kCoffeeMachineActionDeregister, // "DEREGISTER"
    kCoffeeMachineActionConfirm,    // "CONFIRM"
    kCoffeeMachineActionCancel,     // "CANCEL"
};

unsigned int action_coffee_machine_cn[] = {
    kCoffeeMachineActionInvalid,    // unknown
    kCoffeeMachineActionStart,      // "开始"
    kCoffeeMachineActionEspresso,   // "意式 咖啡"
    kCoffeeMachineActionAmericano,  // "美式 咖啡"
    kCoffeeMachineActionCappuccino, // "卡布奇诺"
    kCoffeeMachineActionLatte,      // "拿 铁"
    kCoffeeMachineActionSmall,      // "小杯"
    kCoffeeMachineActionMedium,     // "中 杯"
    kCoffeeMachineActionLarge,      // "大杯"
    kCoffeeMachineActionSoft,       // "柔润"
    kCoffeeMachineActionMild,       // "温和"
    kCoffeeMachineActionStrong,     // "浓烈"
    kCoffeeMachineActionDeregister, // "取消 注册"
    kCoffeeMachineActionConfirm,    // "确定"
    kCoffeeMachineActionCancel,     // "取消"
};

unsigned int action_coffee_machine_fr[] = {
    kCoffeeMachineActionInvalid, // unknown
};

unsigned int action_coffee_machine_de[] = {
    kCoffeeMachineActionInvalid,    // unknown
    kCoffeeMachineActionStart,      // "STARTEN"
    kCoffeeMachineActionEspresso,   // "ESPRESSO"
    kCoffeeMachineActionAmericano,  // "AMERICANO"
    kCoffeeMachineActionCappuccino, // "CAPPUCCINO"
    kCoffeeMachineActionLatte,      // "KAFFEE LATTE"
    kCoffeeMachineActionSmall,      // "KLEIN"
    kCoffeeMachineActionMedium,     // "MITTEL"
    kCoffeeMachineActionLarge,      // "GROß"
    kCoffeeMachineActionSoft,       // "LEICHT"
    kCoffeeMachineActionMild,       // "MILD"
    kCoffeeMachineActionStrong,     // "STARK"
    kCoffeeMachineActionDeregister, // "ABMELDEN"
    kCoffeeMachineActionConfirm,    // "BESTäTIGEN"
    kCoffeeMachineActionCancel,     // "ABBRECHEN"
};

#endif /* INDEXCOMMANDS_VIT_H_ */
