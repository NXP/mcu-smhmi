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
#define NUMBER_OF_ELEVATOR_CMDS_EN (sizeof(action_elevator_en) / sizeof(action_elevator_en[0]))
#define NUMBER_OF_ELEVATOR_CMDS_CN (sizeof(action_elevator_cn) / sizeof(action_elevator_cn[0]))
#define NUMBER_OF_ELEVATOR_CMDS_FR (sizeof(action_elevator_fr) / sizeof(action_elevator_fr[0]))
#define NUMBER_OF_ELEVATOR_CMDS_DE (sizeof(action_elevator_de) / sizeof(action_elevator_de[0]))

unsigned int action_elevator_en[] = {
    kElevatorAction_FloorOne,   // "Main Lobby"
    kElevatorAction_FloorOne,   // "First Floor"
    kElevatorAction_FloorTwo,   // "Second Floor"
    kElevatorAction_FloorTwo,   // "Second Floor"
    kElevatorAction_FloorThree, // "Third Floor"
    kElevatorAction_FloorFour,  // "Fourth Floor"
    kElevatorAction_FloorFive,  // "Fifth Floor"
    kElevatorAction_FloorFive,  // "Fifth Floor"
    kElevatorAction_FloorSix,   // "Sixth Floor"
    kElevatorAction_Confirm,    // "Confirm"
    kElevatorAction_Confirm,    // "Confirm"
    kElevatorAction_Confirm,    // "Confirm"
    kElevatorAction_Confirm,    // "Confirm"
    kElevatorAction_Confirm,    // "Confirm"
    kElevatorAction_Confirm,    // "Confirm"
    kElevatorAction_Confirm,    // "Confirm"
    kElevatorAction_Confirm,    // "Confirm"
    kElevatorAction_Cancel,     // "Cancel"
    kElevatorAction_Deregister, // "Delete User"
};

unsigned int action_elevator_fr[] = {
    kElevatorAction_FloorOne,   // "Main Lobby"
    kElevatorAction_FloorOne,   // "First Floor"
    kElevatorAction_FloorTwo,   // "Second Floor"
    kElevatorAction_FloorThree, // "Third Floor"
    kElevatorAction_FloorFour,  // "Fourth Floor"
    kElevatorAction_FloorFive,  // "Fifth Floor"
    kElevatorAction_FloorSix,   // "Sixth Floor"
    kElevatorAction_Confirm,    // "Confirm"
    kElevatorAction_Cancel,     // "Cancel"
    kElevatorAction_Deregister, // "Delete User"
};

unsigned int action_elevator_cn[] = {
    kElevatorAction_FloorOne,   // "Main Lobby"
    kElevatorAction_FloorOne,   // "Floor One"
    kElevatorAction_FloorOne,   // "Floor One"
    kElevatorAction_FloorTwo,   // "Floor Two"
    kElevatorAction_FloorTwo,   // "Floor Two"
    kElevatorAction_FloorTwo,   // "Floor Two"
    kElevatorAction_FloorTwo,   // "Floor Two"
    kElevatorAction_FloorThree, // "Floor Three"
    kElevatorAction_FloorThree, // "Floor Three"
    kElevatorAction_FloorThree, // "Floor Three"
    kElevatorAction_FloorFour,  // "Floor Four"
    kElevatorAction_FloorFour,  // "Floor Four"
    kElevatorAction_FloorFour,  // "Floor Four"
    kElevatorAction_FloorFour,  // "Floor Four"
    kElevatorAction_FloorFive,  // "Floor Five"
    kElevatorAction_FloorSix,   // "Floor Six"
    kElevatorAction_FloorSix,   // "Floor Six"
    kElevatorAction_Confirm,    // "Confirm"
    kElevatorAction_Cancel,     // "Cancel"
    kElevatorAction_Deregister, // "Delete User"
    kElevatorAction_Deregister, // "Delete User"
};

unsigned int action_elevator_de[] = {
    kElevatorAction_FloorOne,   // "Main Lobby"
    kElevatorAction_FloorOne,   // "First Floor"
    kElevatorAction_FloorTwo,   // "Second Floor"
    kElevatorAction_FloorThree, // "Third Floor"
    kElevatorAction_FloorFour,  // "Fourth Floor"
    kElevatorAction_FloorFive,  // "Fifth Floor"
    kElevatorAction_FloorSix,   // "Sixth Floor"
    kElevatorAction_Confirm,    // "Confirm"
    kElevatorAction_Cancel,     // "Cancel"
    kElevatorAction_Deregister, // "Delete User"
};

char **get_cmd_strings(asr_language_t asrLang, asr_inference_t infCMDType)
{
    char **retString = NULL;

    switch (asrLang)
    {
        case ASR_CHINESE:
            if (infCMDType == ASR_CMD_ELEVATOR)
            {
                retString = cmd_elevator_cn;
            }
            break;
        case ASR_FRENCH:
            if (infCMDType == ASR_CMD_ELEVATOR)
            {
                retString = cmd_elevator_fr;
            }
            break;
        case ASR_ENGLISH:
            if (infCMDType == ASR_CMD_ELEVATOR)
            {
                retString = cmd_elevator_en;
            }
            break;
        case ASR_GERMAN:
            if (infCMDType == ASR_CMD_ELEVATOR)
            {
                retString = cmd_elevator_de;
            }
            break;
        default:
            retString = cmd_elevator_en;
            break;
    }

    return retString;
}
#endif /* INDEXCOMMANDS_DSMT_H_ */
