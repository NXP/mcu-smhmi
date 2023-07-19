/*
 * Copyright 2023 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef INDEXCOMMAND_FUNCS_H_
#define INDEXCOMMAND_FUNCS_H_

#include "hal_voice_algo_asr_local.h"

extern unsigned int get_cmd_number(asr_language_t asrLang, asr_inference_t infCMDType);
extern unsigned int get_action_index_from_keyword(asr_language_t asrLang, asr_inference_t infCMDType, unsigned int keywordId);


#endif /* INDEXCOMMAND_FUNCS_H_ */
