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
#define NUMBER_OF_ELEVATOR_CMDS_EN (sizeof(action_elevator_en) / sizeof(action_elevator_en[0]))
#define NUMBER_OF_ELEVATOR_CMDS_CN (sizeof(action_elevator_cn) / sizeof(action_elevator_cn[0]))
#define NUMBER_OF_ELEVATOR_CMDS_FR (sizeof(action_elevator_fr) / sizeof(action_elevator_fr[0]))
#define NUMBER_OF_ELEVATOR_CMDS_DE (sizeof(action_elevator_de) / sizeof(action_elevator_de[0]))

unsigned int action_elevator_en[] = {
    kElevatorAction_Invalid,    // unknown
    kElevatorAction_FloorOne,   // "MAIN LOBBY"
    kElevatorAction_FloorOne,   // "FLOOR ONE"
    kElevatorAction_FloorTwo,   // "FLOOR TWO"
    kElevatorAction_FloorThree, // "FLOOR THREE"
    kElevatorAction_FloorFour,  // "FLOOR FOUR"
    kElevatorAction_FloorFive,  // "FLOOR FIVE"
    kElevatorAction_FloorSix,   // "FLOOR SIX"
    kElevatorAction_Deregister, // "DEREGISTER"
    kElevatorAction_Confirm,    // "CONFIRM"
    kElevatorAction_Cancel,     // "CANCEL"
};

unsigned int action_elevator_cn[] = {
    kElevatorAction_Invalid,    // unknown
    kElevatorAction_FloorOne,   // "大堂"
    kElevatorAction_FloorOne,   // "一楼"
    kElevatorAction_FloorTwo,   // "二楼"
    kElevatorAction_FloorThree, // "三楼"
    kElevatorAction_FloorFour,  // "四楼"
    kElevatorAction_FloorFive,  // "五楼"
    kElevatorAction_FloorSix,   // "六楼"
    kElevatorAction_Deregister, // "取消 注册"
    kElevatorAction_Confirm,    // "确定"
    kElevatorAction_Cancel,     // "取消"
};

unsigned int action_elevator_fr[] = {
    kElevatorAction_Invalid, // unknown
};

unsigned int action_elevator_de[] = {
    kElevatorAction_Invalid,    // unknown
    kElevatorAction_FloorOne,   // "HAUPTLOBBY"
    kElevatorAction_FloorOne,   // "ETAGE EINS"
    kElevatorAction_FloorTwo,   // "ETAGE ZWEI"
    kElevatorAction_FloorThree, // "ETAGE DREI"
    kElevatorAction_FloorFour,  // "ETAGE VIER"
    kElevatorAction_FloorFive,  // "ETAGE FüNF"
    kElevatorAction_FloorSix,   // "ETAGE SECHS"
    kElevatorAction_Deregister, // "ABMELDEN"
    kElevatorAction_Confirm,    // "BESTäTIGEN"
    kElevatorAction_Cancel,     // "ABBRECHEN"
};

#endif /* INDEXCOMMANDS_VIT_H_ */
