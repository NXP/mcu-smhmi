/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board_define.h"
#ifdef ENABLE_OUTPUT_DEV_MqsAudio
#include "stdint.h"
#include "hal_voice_algo_asr_local.h"
#include "hal_event_descriptor_common.h"
#include "hal_output_dev.h"
#include "fwk_log.h"

#define LANGUAGE_COUNT 4

enum
{
    PROMPT_CONFIRM_TONE = 0,
    PROMPT_TONE_TIMEOUT,
    PROMPT_ANOTHER_ESPRESSO,
    PROMPT_ANOTHER_AMERICANO,
    PROMPT_ANOTHER_CAPPUCCINO,
    PROMPT_ANOTHER_CAFE_LATTE,
    PROMPT_REGISTER_SELECTION,
    PROMPT_INVALID
};

typedef struct _audio_prompt_info
{
    void *data;
    uint32_t len;
} audio_prompt_info_t;

static audio_prompt_info_t s_Prompts[LANGUAGE_COUNT][PROMPT_INVALID];

void LoadAudioPrompts(void *base)
{

    //ENGLISH PROMPT
    s_Prompts[0][PROMPT_CONFIRM_TONE].data = (base + 0);
    s_Prompts[0][PROMPT_CONFIRM_TONE].len = 12246;
    s_Prompts[0][PROMPT_TONE_TIMEOUT].data = (base + 12288);
    s_Prompts[0][PROMPT_TONE_TIMEOUT].len = 6624;
    s_Prompts[0][PROMPT_ANOTHER_ESPRESSO].data = (base + 18944);
    s_Prompts[0][PROMPT_ANOTHER_ESPRESSO].len = 86100;
    s_Prompts[0][PROMPT_ANOTHER_AMERICANO].data = (base + 105088);
    s_Prompts[0][PROMPT_ANOTHER_AMERICANO].len = 92786;
    s_Prompts[0][PROMPT_ANOTHER_CAPPUCCINO].data = (base + 197888);
    s_Prompts[0][PROMPT_ANOTHER_CAPPUCCINO].len = 92786;
    s_Prompts[0][PROMPT_ANOTHER_CAFE_LATTE].data = (base + 290688);
    s_Prompts[0][PROMPT_ANOTHER_CAFE_LATTE].len = 92786;
    s_Prompts[0][PROMPT_REGISTER_SELECTION].data = (base + 383488);
    s_Prompts[0][PROMPT_REGISTER_SELECTION].len = 101146;

    //CHINESE PROMPT
    s_Prompts[1][PROMPT_CONFIRM_TONE].data = (base + 0);
    s_Prompts[1][PROMPT_CONFIRM_TONE].len = 12246;
    s_Prompts[1][PROMPT_TONE_TIMEOUT].data = (base + 12288);
    s_Prompts[1][PROMPT_TONE_TIMEOUT].len = 6624;
    s_Prompts[1][PROMPT_ANOTHER_ESPRESSO].data = (base + 484672);
    s_Prompts[1][PROMPT_ANOTHER_ESPRESSO].len = 119536;
    s_Prompts[1][PROMPT_ANOTHER_AMERICANO].data = (base + 604224);
    s_Prompts[1][PROMPT_ANOTHER_AMERICANO].len = 118700;
    s_Prompts[1][PROMPT_ANOTHER_CAPPUCCINO].data = (base + 722944);
    s_Prompts[1][PROMPT_ANOTHER_CAPPUCCINO].len = 115356;
    s_Prompts[1][PROMPT_ANOTHER_CAFE_LATTE].data = (base + 838336);
    s_Prompts[1][PROMPT_ANOTHER_CAFE_LATTE].len = 101146;
    s_Prompts[1][PROMPT_REGISTER_SELECTION].data = (base + 939520);
    s_Prompts[1][PROMPT_REGISTER_SELECTION].len = 121208;

    //FRENCH PROMPT
    s_Prompts[2][PROMPT_CONFIRM_TONE].data = (base + 0);
    s_Prompts[2][PROMPT_CONFIRM_TONE].len = 12246;
    s_Prompts[2][PROMPT_TONE_TIMEOUT].data = (base + 12288);
    s_Prompts[2][PROMPT_TONE_TIMEOUT].len = 6624;
    s_Prompts[2][PROMPT_ANOTHER_ESPRESSO].data = (base + 1060736);
    s_Prompts[2][PROMPT_ANOTHER_ESPRESSO].len = 81920;
    s_Prompts[2][PROMPT_ANOTHER_AMERICANO].data = (base + 1142656);
    s_Prompts[2][PROMPT_ANOTHER_AMERICANO].len = 85264;
    s_Prompts[2][PROMPT_ANOTHER_CAPPUCCINO].data = (base + 1227968);
    s_Prompts[2][PROMPT_ANOTHER_CAPPUCCINO].len = 86100;
    s_Prompts[2][PROMPT_ANOTHER_CAFE_LATTE].data = (base + 1314112);
    s_Prompts[2][PROMPT_ANOTHER_CAFE_LATTE].len = 86100;
    s_Prompts[2][PROMPT_REGISTER_SELECTION].data = (base + 1400256);
    s_Prompts[2][PROMPT_REGISTER_SELECTION].len = 107834;

    //GERMAN PROMPT
    s_Prompts[3][PROMPT_CONFIRM_TONE].data = (base + 0);
    s_Prompts[3][PROMPT_CONFIRM_TONE].len = 12246;
    s_Prompts[3][PROMPT_TONE_TIMEOUT].data = (base + 12288);
    s_Prompts[3][PROMPT_TONE_TIMEOUT].len = 6624;
    s_Prompts[3][PROMPT_ANOTHER_ESPRESSO].data = (base + 1508096);
    s_Prompts[3][PROMPT_ANOTHER_ESPRESSO].len = 106998;
    s_Prompts[3][PROMPT_ANOTHER_AMERICANO].data = (base + 1615104);
    s_Prompts[3][PROMPT_ANOTHER_AMERICANO].len = 110342;
    s_Prompts[3][PROMPT_ANOTHER_CAPPUCCINO].data = (base + 1725504);
    s_Prompts[3][PROMPT_ANOTHER_CAPPUCCINO].len = 109506;
    s_Prompts[3][PROMPT_ANOTHER_CAFE_LATTE].data = (base + 1835072);
    s_Prompts[3][PROMPT_ANOTHER_CAFE_LATTE].len = 111178;
    s_Prompts[3][PROMPT_REGISTER_SELECTION].data = (base + 1946304);
    s_Prompts[3][PROMPT_REGISTER_SELECTION].len = 131240;
    /* Prompts Total: 0x1fb380, 2077568 */
}

int APP_OutputDev_MqsAudio_InputNotifyDecode(
    const output_dev_t *dev, void *inputData, void **audio, uint32_t *len, asr_language_t language)
{
    event_base_t *pEventBase           = (event_base_t *)inputData;
    const audio_prompt_info_t *pPrompt = NULL;

    if (pEventBase->eventId == kEventID_PlayPrompt)
    {
        int promptId = (int)((event_common_t *)pEventBase)->data;
        LOGD("[MQS] Prompt id %d", promptId);
        if (promptId >= PROMPT_CONFIRM_TONE && promptId < PROMPT_INVALID)
        {
            switch (language)
            {
                case ASR_ENGLISH:
                    pPrompt = s_Prompts[0];
                    break;
                case ASR_CHINESE:
                    pPrompt = s_Prompts[1];
                    break;
                case ASR_FRENCH:
                    pPrompt = s_Prompts[2];
                    break;
                case ASR_GERMAN:
                    pPrompt = s_Prompts[3];
                    break;
                default:
                    pPrompt = s_Prompts[0];
                    break;
            }
            *audio = pPrompt[promptId].data;
            *len   = pPrompt[promptId].len;
        }
    }
    return 0;
}
#endif /* ENABLE_OUTPUT_DEV_MqsAudio */
