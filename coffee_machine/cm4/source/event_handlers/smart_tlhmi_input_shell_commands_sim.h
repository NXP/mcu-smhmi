/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief  simulate the shell commands for testing.
 */

#ifdef ENABLE_SHELL_COMMANDS_SIM

#include "fwk_message.h"
#include "hal_event_descriptor_common.h"
#include "hal_event_descriptor_face_rec.h"
#include "hal_event_descriptor_voice.h"
#include "custom.h"

/**
 * simulated commands
 */
enum
{
    SIM_ASR_RESULT = 0,

    SIM_INVALID,
};

static fwk_message_t s_fwkMsgCommon;
static asr_inference_result_t s_asrResult;

static void _AsrResultUpdate()
{
    fwk_message_t *pMsg = &s_fwkMsgCommon;
    memset(pMsg, 0, sizeof(fwk_message_t));
    pMsg->id            = kFWKMessageID_VAlgoASRResultUpdate;
    pMsg->payload.devId = 0;
    pMsg->payload.size  = sizeof(s_asrResult);
    pMsg->payload.data  = &s_asrResult;
    pMsg->msgInfo       = kMsgInfo_Local;

    FWK_Message_PutFromIsr(kFWKTaskID_Output, &pMsg);
}

/**
 * info 0 status id
 */
static shell_status_t _simAsrResult(shell_handle_t shellContextHandle, int32_t argc, char **argv)
{
    char *pEnd;
    if (argc == 4)
    {
        uint32_t status = strtol(argv[2], &pEnd, 10);
        if (argv[1] == pEnd)
        {
            SHELL_Printf(shellContextHandle, "\"%s\" not a number.\r\n", argv[1]);
            return kStatus_SHELL_Error;
        }
        uint32_t keywordID = strtol(argv[3], &pEnd, 10);
        if (argv[2] == pEnd)
        {
            SHELL_Printf(shellContextHandle, "\"%s\" not a number.\r\n", argv[2]);
            return kStatus_SHELL_Error;
        }
        SHELL_Printf(shellContextHandle, "[SIM]: asr status: %d keywordID:%d\r\n", status, keywordID);
        s_asrResult.status    = status;
        s_asrResult.keywordID = keywordID;
        s_asrResult.language  = keywordID;
        _AsrResultUpdate();
    }
    return kStatus_SHELL_Success;
}

/**
 * sub simulated command for the "info" command
 */
shell_status_t _simCmd(shell_handle_t shellContextHandle, int32_t argc, char **argv)
{
    char *pEnd;
    uint32_t cmd = strtol(argv[1], &pEnd, 10);
    if (argv[1] == pEnd)
    {
        SHELL_Printf(shellContextHandle, "\"%s\" not a number.\r\n", argv[1]);
        return kStatus_SHELL_Error;
    }

    switch (cmd)
    {
        case SIM_ASR_RESULT:
        {
            _simAsrResult(shellContextHandle, argc, argv);
        }
        break;

        default:
            break;
    }

    return kStatus_SHELL_Success;
}

#endif /* ENABLE_SHELL_COMMANDS_SIM */
