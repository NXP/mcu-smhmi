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
    PROMPT_TIMEOUT_TONE,
    PROMPT_ALARM_TONE,
    PROMPT_OPEN_DOOR_TONE,
    PROMPT_FLOORONE,
    PROMPT_FLOORTWO,
    PROMPT_FLOORTHREE,
    PROMPT_FLOORFOUR,
    PROMPT_FLOORFIVE,
    PROMPT_FLOORSIX,
    PROMPT_ONE_CONFIRM_CANCEL,
    PROMPT_TWO_CONFIRM_CANCEL,
    PROMPT_THREE_CONFIRM_CANCEL,
    PROMPT_FOUR_CONFIRM_CANCEL,
    PROMPT_FIVE_CONFIRM_CANCEL,
    PROMPT_SIX_CONFIRM_CANCEL,
    PROMPT_REGISTER_SELECTION,
    PROMPT_WHICH_FLOOR,
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
    // ENGLISH PROMPT
    s_Prompts[0][PROMPT_CONFIRM_TONE].data         = (base + 0);
    s_Prompts[0][PROMPT_CONFIRM_TONE].len          = 12246;
    s_Prompts[0][PROMPT_TIMEOUT_TONE].data         = (base + 12288);
    s_Prompts[0][PROMPT_TIMEOUT_TONE].len          = 6624;
    s_Prompts[0][PROMPT_ALARM_TONE].data           = (base + 18944);
    s_Prompts[0][PROMPT_ALARM_TONE].len            = 136254;
    s_Prompts[0][PROMPT_OPEN_DOOR_TONE].data       = (base + 155200);
    s_Prompts[0][PROMPT_OPEN_DOOR_TONE].len        = 31922;

    s_Prompts[0][PROMPT_FLOORONE].data             = (base + 187136);
    s_Prompts[0][PROMPT_FLOORONE].len              = 28490;
    s_Prompts[0][PROMPT_FLOORTWO].data             = (base + 215680);
    s_Prompts[0][PROMPT_FLOORTWO].len              = 30998;
    s_Prompts[0][PROMPT_FLOORTHREE].data           = (base + 246720);
    s_Prompts[0][PROMPT_FLOORTHREE].len            = 29326;
    s_Prompts[0][PROMPT_FLOORFOUR].data            = (base + 276096);
    s_Prompts[0][PROMPT_FLOORFOUR].len             = 28490;
    s_Prompts[0][PROMPT_FLOORFIVE].data            = (base + 304640);
    s_Prompts[0][PROMPT_FLOORFIVE].len             = 27654;
    s_Prompts[0][PROMPT_FLOORSIX].data             = (base + 332352);
    s_Prompts[0][PROMPT_FLOORSIX].len              = 29326;
    s_Prompts[0][PROMPT_ONE_CONFIRM_CANCEL].data   = (base + 361728);
    s_Prompts[0][PROMPT_ONE_CONFIRM_CANCEL].len    = 107066;
    s_Prompts[0][PROMPT_TWO_CONFIRM_CANCEL].data   = (base + 468800);
    s_Prompts[0][PROMPT_TWO_CONFIRM_CANCEL].len    = 107066;
    s_Prompts[0][PROMPT_THREE_CONFIRM_CANCEL].data = (base + 575872);
    s_Prompts[0][PROMPT_THREE_CONFIRM_CANCEL].len  = 107066;
    s_Prompts[0][PROMPT_FOUR_CONFIRM_CANCEL].data  = (base + 682944);
    s_Prompts[0][PROMPT_FOUR_CONFIRM_CANCEL].len   = 107902;
    s_Prompts[0][PROMPT_FIVE_CONFIRM_CANCEL].data  = (base + 790848);
    s_Prompts[0][PROMPT_FIVE_CONFIRM_CANCEL].len   = 107902;
    s_Prompts[0][PROMPT_SIX_CONFIRM_CANCEL].data   = (base + 898752);
    s_Prompts[0][PROMPT_SIX_CONFIRM_CANCEL].len    = 107066;
    s_Prompts[0][PROMPT_REGISTER_SELECTION].data   = (base + 1005824);
    s_Prompts[0][PROMPT_REGISTER_SELECTION].len    = 96198;
    s_Prompts[0][PROMPT_WHICH_FLOOR].data          = (base + 1102080);
    s_Prompts[0][PROMPT_WHICH_FLOOR].len           = 56842;

    // CHINESE PROMPT
    s_Prompts[1][PROMPT_CONFIRM_TONE].data         = (base + 0);
    s_Prompts[1][PROMPT_CONFIRM_TONE].len          = 12246;
    s_Prompts[1][PROMPT_TIMEOUT_TONE].data         = (base + 12288);
    s_Prompts[1][PROMPT_TIMEOUT_TONE].len          = 6624;
    s_Prompts[1][PROMPT_ALARM_TONE].data           = (base + 18944);
    s_Prompts[1][PROMPT_ALARM_TONE].len            = 136254;
    s_Prompts[1][PROMPT_OPEN_DOOR_TONE].data       = (base + 155200);
    s_Prompts[1][PROMPT_OPEN_DOOR_TONE].len        = 31922;

    s_Prompts[1][PROMPT_FLOORONE].data             = (base + 1158976);
    s_Prompts[1][PROMPT_FLOORONE].len              = 17554;
    s_Prompts[1][PROMPT_FLOORTWO].data             = (base + 1176576);
    s_Prompts[1][PROMPT_FLOORTWO].len              = 20898;
    s_Prompts[1][PROMPT_FLOORTHREE].data           = (base + 1197504);
    s_Prompts[1][PROMPT_FLOORTHREE].len            = 22570;
    s_Prompts[1][PROMPT_FLOORFOUR].data            = (base + 1220096);
    s_Prompts[1][PROMPT_FLOORFOUR].len             = 20898;
    s_Prompts[1][PROMPT_FLOORFIVE].data            = (base + 1241024);
    s_Prompts[1][PROMPT_FLOORFIVE].len             = 19226;
    s_Prompts[1][PROMPT_FLOORSIX].data             = (base + 1260288);
    s_Prompts[1][PROMPT_FLOORSIX].len              = 22570;
    s_Prompts[1][PROMPT_ONE_CONFIRM_CANCEL].data   = (base + 1282880);
    s_Prompts[1][PROMPT_ONE_CONFIRM_CANCEL].len    = 86936;
    s_Prompts[1][PROMPT_TWO_CONFIRM_CANCEL].data   = (base + 1369856);
    s_Prompts[1][PROMPT_TWO_CONFIRM_CANCEL].len    = 85264;
    s_Prompts[1][PROMPT_THREE_CONFIRM_CANCEL].data = (base + 1455168);
    s_Prompts[1][PROMPT_THREE_CONFIRM_CANCEL].len  = 90280;
    s_Prompts[1][PROMPT_FOUR_CONFIRM_CANCEL].data  = (base + 1545472);
    s_Prompts[1][PROMPT_FOUR_CONFIRM_CANCEL].len   = 89444;
    s_Prompts[1][PROMPT_FIVE_CONFIRM_CANCEL].data  = (base + 1634944);
    s_Prompts[1][PROMPT_FIVE_CONFIRM_CANCEL].len   = 86936;
    s_Prompts[1][PROMPT_SIX_CONFIRM_CANCEL].data   = (base + 1721920);
    s_Prompts[1][PROMPT_SIX_CONFIRM_CANCEL].len    = 88608;
    s_Prompts[1][PROMPT_REGISTER_SELECTION].data   = (base + 1810560);
    s_Prompts[1][PROMPT_REGISTER_SELECTION].len    = 100310;
    s_Prompts[1][PROMPT_WHICH_FLOOR].data          = (base + 1910912);
    s_Prompts[1][PROMPT_WHICH_FLOOR].len           = 59350;

    // FRENCH PROMPT
    s_Prompts[2][PROMPT_CONFIRM_TONE].data         = (base + 0);
    s_Prompts[2][PROMPT_CONFIRM_TONE].len          = 12246;
    s_Prompts[2][PROMPT_TIMEOUT_TONE].data         = (base + 12288);
    s_Prompts[2][PROMPT_TIMEOUT_TONE].len          = 6624;
    s_Prompts[2][PROMPT_ALARM_TONE].data           = (base + 18944);
    s_Prompts[2][PROMPT_ALARM_TONE].len            = 136254;
    s_Prompts[2][PROMPT_OPEN_DOOR_TONE].data       = (base + 155200);
    s_Prompts[2][PROMPT_OPEN_DOOR_TONE].len        = 31922;

    s_Prompts[2][PROMPT_FLOORONE].data             = (base + 1970304);
    s_Prompts[2][PROMPT_FLOORONE].len              = 30998;
    s_Prompts[2][PROMPT_FLOORTWO].data             = (base + 2001344);
    s_Prompts[2][PROMPT_FLOORTWO].len              = 33504;
    s_Prompts[2][PROMPT_FLOORTHREE].data           = (base + 2034880);
    s_Prompts[2][PROMPT_FLOORTHREE].len            = 34340;
    s_Prompts[2][PROMPT_FLOORFOUR].data            = (base + 2069248);
    s_Prompts[2][PROMPT_FLOORFOUR].len             = 33504;
    s_Prompts[2][PROMPT_FLOORFIVE].data            = (base + 2102784);
    s_Prompts[2][PROMPT_FLOORFIVE].len             = 36012;
    s_Prompts[2][PROMPT_FLOORSIX].data             = (base + 2138816);
    s_Prompts[2][PROMPT_FLOORSIX].len              = 32670;
    s_Prompts[2][PROMPT_ONE_CONFIRM_CANCEL].data   = (base + 2171520);
    s_Prompts[2][PROMPT_ONE_CONFIRM_CANCEL].len    = 88676;
    s_Prompts[2][PROMPT_TWO_CONFIRM_CANCEL].data   = (base + 2260224);
    s_Prompts[2][PROMPT_TWO_CONFIRM_CANCEL].len    = 88676;
    s_Prompts[2][PROMPT_THREE_CONFIRM_CANCEL].data = (base + 2348928);
    s_Prompts[2][PROMPT_THREE_CONFIRM_CANCEL].len  = 87840;
    s_Prompts[2][PROMPT_FOUR_CONFIRM_CANCEL].data  = (base + 2436800);
    s_Prompts[2][PROMPT_FOUR_CONFIRM_CANCEL].len   = 93692;
    s_Prompts[2][PROMPT_FIVE_CONFIRM_CANCEL].data  = (base + 2530496);
    s_Prompts[2][PROMPT_FIVE_CONFIRM_CANCEL].len   = 90348;
    s_Prompts[2][PROMPT_SIX_CONFIRM_CANCEL].data   = (base + 2620864);
    s_Prompts[2][PROMPT_SIX_CONFIRM_CANCEL].len    = 90348;
    s_Prompts[2][PROMPT_REGISTER_SELECTION].data   = (base + 2711232);
    s_Prompts[2][PROMPT_REGISTER_SELECTION].len    = 102050;
    s_Prompts[2][PROMPT_WHICH_FLOOR].data          = (base + 2813312);
    s_Prompts[2][PROMPT_WHICH_FLOOR].len           = 54334;

    // GERMAN PROMPT
    s_Prompts[3][PROMPT_CONFIRM_TONE].data         = (base + 0);
    s_Prompts[3][PROMPT_CONFIRM_TONE].len          = 12246;
    s_Prompts[3][PROMPT_TIMEOUT_TONE].data         = (base + 12288);
    s_Prompts[3][PROMPT_TIMEOUT_TONE].len          = 6624;
    s_Prompts[3][PROMPT_ALARM_TONE].data           = (base + 18944);
    s_Prompts[3][PROMPT_ALARM_TONE].len            = 136254;
    s_Prompts[3][PROMPT_OPEN_DOOR_TONE].data       = (base + 155200);
    s_Prompts[3][PROMPT_OPEN_DOOR_TONE].len        = 31922;

    s_Prompts[3][PROMPT_FLOORONE].data             = (base + 2867648);
    s_Prompts[3][PROMPT_FLOORONE].len              = 34340;
    s_Prompts[3][PROMPT_FLOORTWO].data             = (base + 2902016);
    s_Prompts[3][PROMPT_FLOORTWO].len              = 36012;
    s_Prompts[3][PROMPT_FLOORTHREE].data           = (base + 2938048);
    s_Prompts[3][PROMPT_FLOORTHREE].len            = 34340;
    s_Prompts[3][PROMPT_FLOORFOUR].data            = (base + 2972416);
    s_Prompts[3][PROMPT_FLOORFOUR].len             = 36012;
    s_Prompts[3][PROMPT_FLOORFIVE].data            = (base + 3008448);
    s_Prompts[3][PROMPT_FLOORFIVE].len             = 36848;
    s_Prompts[3][PROMPT_FLOORSIX].data             = (base + 3045312);
    s_Prompts[3][PROMPT_FLOORSIX].len              = 38520;
    s_Prompts[3][PROMPT_ONE_CONFIRM_CANCEL].data   = (base + 3083840);
    s_Prompts[3][PROMPT_ONE_CONFIRM_CANCEL].len    = 135486;
    s_Prompts[3][PROMPT_TWO_CONFIRM_CANCEL].data   = (base + 3219328);
    s_Prompts[3][PROMPT_TWO_CONFIRM_CANCEL].len    = 137158;
    s_Prompts[3][PROMPT_THREE_CONFIRM_CANCEL].data = (base + 3356544);
    s_Prompts[3][PROMPT_THREE_CONFIRM_CANCEL].len  = 132144;
    s_Prompts[3][PROMPT_FOUR_CONFIRM_CANCEL].data  = (base + 3488704);
    s_Prompts[3][PROMPT_FOUR_CONFIRM_CANCEL].len   = 135486;
    s_Prompts[3][PROMPT_FIVE_CONFIRM_CANCEL].data  = (base + 3624192);
    s_Prompts[3][PROMPT_FIVE_CONFIRM_CANCEL].len   = 136322;
    s_Prompts[3][PROMPT_SIX_CONFIRM_CANCEL].data   = (base + 3760576);
    s_Prompts[3][PROMPT_SIX_CONFIRM_CANCEL].len    = 136322;
    s_Prompts[3][PROMPT_REGISTER_SELECTION].data   = (base + 3896960);
    s_Prompts[3][PROMPT_REGISTER_SELECTION].len    = 119604;
    s_Prompts[3][PROMPT_WHICH_FLOOR].data          = (base + 4016576);
    s_Prompts[3][PROMPT_WHICH_FLOOR].len           = 61022;
}

int APP_OutputDev_MqsAudio_InputNotifyDecode(
    const output_dev_t *dev, void *inputData, void **audio, uint32_t *len, asr_language_t language)
{
    event_base_t *pEventBase           = (event_base_t *)inputData;
    const audio_prompt_info_t *pPrompt = NULL;

    if (pEventBase->eventId == kEventID_PlayPrompt)
    {
        int promptId = (int)((event_common_t *)pEventBase)->data;
        LOGD("[MQS] Play prompt:%d", promptId);
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
