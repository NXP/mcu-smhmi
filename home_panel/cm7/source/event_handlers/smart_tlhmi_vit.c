/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board_define.h"
#ifdef ENABLE_VIT_ASR
#include "stdint.h"
#include "hal_voice_algo_asr_local.h"
#include "hal_event_descriptor_common.h"
#include "fwk_log.h"

#define LANGUAGE_COUNT 4

enum
{
    MAIN_MENU,
    THERMOSTAT,
    SECURITY,
    AUDIO_PLAYER,
    APPLICATION_INVALID
};

typedef struct _vit_model_info
{
    void *data;
    uint32_t len;
} vit_model_info_t;

static vit_model_info_t s_VITModels[LANGUAGE_COUNT][APPLICATION_INVALID];

void LoadVITModels(void *base)
{
    //English VIT models
    s_VITModels[0][MAIN_MENU].data = (base + 0);
    s_VITModels[0][MAIN_MENU].len = 360732;
    s_VITModels[0][THERMOSTAT].data = (base + 360768);
    s_VITModels[0][THERMOSTAT].len = 415244;
    s_VITModels[0][SECURITY].data = (base + 776064);
    s_VITModels[0][SECURITY].len = 362084;
    s_VITModels[0][AUDIO_PLAYER].data = (base + 1138176);
    s_VITModels[0][AUDIO_PLAYER].len = 362436;

    //Chinese VIT models
    s_VITModels[1][MAIN_MENU].data = (base + 1500672);
    s_VITModels[1][MAIN_MENU].len = 337308;
    s_VITModels[1][THERMOSTAT].data = (base + 1838016);
    s_VITModels[1][THERMOSTAT].len = 340924;
    s_VITModels[1][SECURITY].data = (base + 2178944);
    s_VITModels[1][SECURITY].len = 338420;
    s_VITModels[1][AUDIO_PLAYER].data = (base + 2517376);
    s_VITModels[1][AUDIO_PLAYER].len = 338828;
}

int APP_GetVITModels(uint8_t **model, uint32_t *len, asr_language_t language, int applicationId)
{
    const vit_model_info_t *VITmodel = NULL;
    if (applicationId >= MAIN_MENU && applicationId < APPLICATION_INVALID)
    {
        switch (language)
        {
            case ASR_ENGLISH:
                VITmodel = s_VITModels[0];
                break;
            case ASR_CHINESE:
                VITmodel = s_VITModels[1];
                break;
            case ASR_FRENCH:
                VITmodel = s_VITModels[2];
                break;
            case ASR_GERMAN:
                VITmodel = s_VITModels[3];
                break;
            default:
                VITmodel = s_VITModels[0];
                break;
        }
        *model = VITmodel[applicationId].data;
        *len   = VITmodel[applicationId].len;
    }

    return 0;
}

int get_voice_model(uint8_t **modelAddr, uint32_t *modelSize, asr_language_t asrLang, asr_inference_t infCMDType)
{
    switch (infCMDType)
    {
        case ASR_CMD_HP_MAIN_MENU:
            APP_GetVITModels(modelAddr, modelSize, asrLang, MAIN_MENU);
            break;
        case ASR_CMD_HP_THERMOSTAT:
            APP_GetVITModels(modelAddr, modelSize, asrLang, THERMOSTAT);
            break;
        case ASR_CMD_HP_SECURITY:
            APP_GetVITModels(modelAddr, modelSize, asrLang, SECURITY);
            break;
        case ASR_CMD_HP_AUDIO_PLAYER:
            APP_GetVITModels(modelAddr, modelSize, asrLang, AUDIO_PLAYER);
            break;
        default:
            modelAddr = NULL;
            break;
    }

    return 0;
}

#endif
