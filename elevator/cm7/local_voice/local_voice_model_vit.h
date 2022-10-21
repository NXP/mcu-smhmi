/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef LOCAL_VOICE_MODEL_H_
#define LOCAL_VOICE_MODEL_H_

#include "hal_voice_algo_asr_local.h"

#include "VIT_Model_en.h"
#include "VIT_Model_cn.h"
#include "VIT_Model_de.h"

const uint8_t* get_model_address(asr_language_t asrLang, asr_inference_t infCMDType)
{
    const uint8_t *modelAddr = NULL;

    switch (asrLang)
    {
        case ASR_CHINESE:
            if (infCMDType == ASR_CMD_ELEVATOR)
            {
                modelAddr = VIT_Model_cn;
            }
            else if (infCMDType == ASR_CMD_FLOOR_REGISTER)
            {
                modelAddr = VIT_Model_cn;
            }
            break;
#if 0
        /* NOT supported yet */
        case ASR_FRENCH:
            if (infCMDType == ASR_CMD_ELEVATOR)
            {
                modelAddr = VIT_Model_fr;
            }
            else if (infCMDType == ASR_CMD_FLOOR_REGISTER)
            {
                asrLmodelAddrang = VIT_Model_fr;
            }
            break;
#endif
        case ASR_ENGLISH:
            if (infCMDType == ASR_CMD_ELEVATOR)
            {
                modelAddr = VIT_Model_en;
            }
            else if (infCMDType == ASR_CMD_FLOOR_REGISTER)
            {
                modelAddr = VIT_Model_en;
            }
            break;
        case ASR_GERMAN:
            if (infCMDType == ASR_CMD_ELEVATOR)
            {
                modelAddr = VIT_Model_de;
            }
            else if (infCMDType == ASR_CMD_FLOOR_REGISTER)
            {
                modelAddr = VIT_Model_de;
            }
            break;
        default:
            modelAddr = NULL;
            break;
    }

    return modelAddr;
}

unsigned int get_model_size(asr_language_t asrLang, asr_inference_t infCMDType)
{
    unsigned int modelSize = 0;

    switch (asrLang)
    {
        case ASR_CHINESE:
            if (infCMDType == ASR_CMD_ELEVATOR)
            {
                modelSize = sizeof(VIT_Model_cn);
            }
            else if (infCMDType == ASR_CMD_FLOOR_REGISTER)
            {
                modelSize = sizeof(VIT_Model_cn);
            }
            break;
#if 0
        /* NOT supported yet */
        case ASR_FRENCH:
            if (infCMDType == ASR_CMD_ELEVATOR)
            {
                modelSize = sizeof(VIT_Model_fr);
            }
            else if (infCMDType == ASR_CMD_FLOOR_REGISTER)
            {
                modelSize = sizeof(VIT_Model_fr);
            }
            break;
#endif
        case ASR_ENGLISH:
            if (infCMDType == ASR_CMD_ELEVATOR)
            {
                modelSize = sizeof(VIT_Model_en);
            }
            else if (infCMDType == ASR_CMD_FLOOR_REGISTER)
            {
                modelSize = sizeof(VIT_Model_en);
            }
            break;
        case ASR_GERMAN:
            if (infCMDType == ASR_CMD_ELEVATOR)
            {
                modelSize = sizeof(VIT_Model_de);
            }
            else if (infCMDType == ASR_CMD_FLOOR_REGISTER)
            {
                modelSize = sizeof(VIT_Model_de);
            }
            break;
        default:
            modelSize = 0;
            break;
    }

    return modelSize;
}

#endif /* LOCAL_VOICE_MODEL_H_ */
