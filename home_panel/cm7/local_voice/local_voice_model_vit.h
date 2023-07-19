/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef LOCAL_VOICE_VIT_MODEL_H_
#define LOCAL_VOICE_VIT_MODEL_H_

#include "hal_voice_algo_asr_local.h"

#if defined(__cplusplus)
extern "C" {
#endif

int APP_GetVITModels(uint8_t **model, uint32_t *len, asr_language_t language, int applicationId);
int get_voice_model(uint8_t **modelAddr, uint32_t *modelSize, asr_language_t asrLang, asr_inference_t infCMDType);

#if defined(__cplusplus)
}
#endif

#endif /* LOCAL_VOICE_MODEL_H_ */
