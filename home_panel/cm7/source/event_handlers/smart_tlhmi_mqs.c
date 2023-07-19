/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board_define.h"
#ifdef ENABLE_OUTPUT_DEV_MqsStreamerAudio
#include "stdint.h"
#include "hal_voice_algo_asr_local.h"
#include "hal_event_descriptor_common.h"
#include "hal_output_dev.h"
#include "fwk_log.h"
#include "sounds.h"
#include "smart_tlhmi_mqs.h"

#define LANGUAGE_COUNT 2

enum
{
    Flying_On_The_Wings_Of_Love,
    Feel_Alright_Main,
    Digital_Technology_Full,
    SONG_INVALID
};

static audio_prompt_info_t s_Prompts[LANGUAGE_COUNT][PROMPT_INVALID];
static track_element s_Songs[SONG_INVALID];

void LoadAudioPrompts(void *base)
{
    s_Songs[Flying_On_The_Wings_Of_Love].track  = (base + 0);
    s_Songs[Flying_On_The_Wings_Of_Love].length = 365893;
    s_Songs[Feel_Alright_Main].track            = (base + 365952);
    s_Songs[Feel_Alright_Main].length           = 365893;
    s_Songs[Digital_Technology_Full].track      = (base + 731904);
    s_Songs[Digital_Technology_Full].length     = 365852;

    // Prompts Total: 0x12b080, 1224832
    s_Prompts[0][Have_Your_Favorite_Setttings_Confirm_Cancel].data = (base + 1097792);
    s_Prompts[0][Have_Your_Favorite_Setttings_Confirm_Cancel].len  = 12213;
    s_Prompts[0][Delete_Selected_User_Confirm_Cancel].data         = (base + 1110016);
    s_Prompts[0][Delete_Selected_User_Confirm_Cancel].len          = 11025;
    s_Prompts[0][Start_Registration].data                          = (base + 1121088);
    s_Prompts[0][Start_Registration].len                           = 3681;
    s_Prompts[0][Identified_New_User_Verify_Pin].data              = (base + 1124800);
    s_Prompts[0][Identified_New_User_Verify_Pin].len               = 11025;
    s_Prompts[0][Invalid_Pin].data                                 = (base + 1135872);
    s_Prompts[0][Invalid_Pin].len                                  = 3465;
    s_Prompts[0][Registration_Successful].data                     = (base + 1139392);
    s_Prompts[0][Registration_Successful].len                      = 4005;
    s_Prompts[0][Verify_Pin_To_Delete_User].data                   = (base + 1143424);
    s_Prompts[0][Verify_Pin_To_Delete_User].len                    = 9405;
    s_Prompts[0][User_Deleted].data                                = (base + 1152832);
    s_Prompts[0][User_Deleted].len                                 = 4113;
    s_Prompts[0][Registered_User].data                             = (base + 1156992);
    s_Prompts[0][Registered_User].len                              = 4329;
    s_Prompts[0][Come_Closer].data                                 = (base + 1161344);
    s_Prompts[0][Come_Closer].len                                  = 4005;
    s_Prompts[0][Stand_Still].data                                 = (base + 1165376);
    s_Prompts[0][Stand_Still].len                                  = 3789;
    s_Prompts[0][Face_The_Camera].data                             = (base + 1169216);
    s_Prompts[0][Face_The_Camera].len                              = 4005;
    s_Prompts[0][Reposition_Face].data                             = (base + 1173248);
    s_Prompts[0][Reposition_Face].len                              = 4977;
    s_Prompts[1][Have_Your_Favorite_Setttings_Confirm_Cancel].data = (base + 1178240);
    s_Prompts[1][Have_Your_Favorite_Setttings_Confirm_Cancel].len  = 12213;
    s_Prompts[1][Delete_Selected_User_Confirm_Cancel].data         = (base + 1190464);
    s_Prompts[1][Delete_Selected_User_Confirm_Cancel].len          = 9405;
    s_Prompts[1][Start_Registration].data                          = (base + 1199872);
    s_Prompts[1][Start_Registration].len                           = 4761;
    s_Prompts[1][Identified_New_User_Verify_Pin].data              = (base + 1204672);
    s_Prompts[1][Identified_New_User_Verify_Pin].len               = 11889;
    s_Prompts[1][Invalid_Pin].data                                 = (base + 1216576);
    s_Prompts[1][Invalid_Pin].len                                  = 3465;
    s_Prompts[1][Registration_Successful].data                     = (base + 1220096);
    s_Prompts[1][Registration_Successful].len                      = 5409;
    s_Prompts[1][Verify_Pin_To_Delete_User].data                   = (base + 1225536);
    s_Prompts[1][Verify_Pin_To_Delete_User].len                    = 7893;
    s_Prompts[1][User_Deleted].data                                = (base + 1233472);
    s_Prompts[1][User_Deleted].len                                 = 3357;
    s_Prompts[1][Registered_User].data                             = (base + 1236864);
    s_Prompts[1][Registered_User].len                              = 3789;
    s_Prompts[1][Come_Closer].data                                 = (base + 1240704);
    s_Prompts[1][Come_Closer].len                                  = 3141;
    s_Prompts[1][Stand_Still].data                                 = (base + 1243904);
    s_Prompts[1][Stand_Still].len                                  = 3141;
    s_Prompts[1][Face_The_Camera].data                             = (base + 1247104);
    s_Prompts[1][Face_The_Camera].len                              = 3573;
    s_Prompts[1][Reposition_Face].data                             = (base + 1250688);
    s_Prompts[1][Reposition_Face].len                              = 4329;
    s_Prompts[0][Beep_Tone].data                                   = (base + 1255040);
    s_Prompts[0][Beep_Tone].len                                    = 981;
    s_Prompts[1][Beep_Tone].data                                   = (base + 1255040);
    s_Prompts[1][Beep_Tone].len                                    = 981;
}

int APP_OutputDev_MqsAudio_GetSongs(track_element **songs, uint32_t *number)
{
	*songs  = s_Songs;
	*number = SONG_INVALID;
	return 0;
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
        if (promptId >= Have_Your_Favorite_Setttings_Confirm_Cancel && promptId < PROMPT_INVALID)
        {
            switch (language)
            {
                case ASR_CHINESE:
                    pPrompt = s_Prompts[0];
                    break;
                case ASR_ENGLISH:
                    pPrompt = s_Prompts[1];
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
