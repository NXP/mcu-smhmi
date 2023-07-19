/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
@brief voice event descriptor declaration.
*/

#ifndef _HAL_EVENT_DESCRIPTOR_VOICE_H_
#define _HAL_EVENT_DESCRIPTOR_VOICE_H_

#include "hal_event_descriptor_common.h"
#include "hal_voice_algo_asr_local.h"

typedef enum _event_voice_id
{
    AUDIO_IN = kEventType_Voice, // to prevent a scenario where events overlap
    ASR_TO_AFE_FEEDBACK,
    SPEAKER_TO_AFE_FEEDBACK,
    GET_TIMEOUT_DURATION,
    SET_TIMEOUT_DURATION,
    GET_FOLLOWUP_STATUS,
    SET_FOLLOWUP_STATUS,
    GET_MULTILINGUAL_CONFIG,
    SET_MULTILINGUAL_CONFIG,
    GET_VOICE_DEMO,
    SET_VOICE_DEMO,
    STOP_VOICE_CMD_SESSION,
    SET_VOICE_MODEL,
    ASR_TO_AFE_PROCESSED,
	SET_AQT_STATUS,
    LAST_VOICE_EVENT
} event_voice_id_t;

typedef struct _audio_in_event
{
    int32_t *audio_stream;
} audio_in_event_t;

typedef struct _audio_out_event
{
    int8_t sound_id;
} audio_out_event_t;

typedef struct _asr_feedback
{
    uint32_t utterance_len;
} asr_feedback_t;

typedef struct _speaker_playing_event
{
    int16_t *audio_stream;
    uint32_t audio_length;
    uint32_t start_time;
} speaker_playing_event_t;

typedef struct _set_timeout_duration_event
{
    uint32_t timeout;
} set_timeout_duration_event_t;

typedef struct _set_followup_status_event
{
    asr_followup_t followup;
} set_followup_status_event_t;

typedef struct _set_multilingual_status_event
{
    asr_language_t languages;
} set_multilingual_config_event_t;

typedef struct _set_voice_demo_event
{
    asr_inference_t demo;
} set_voice_demo_event_t;

typedef struct _set_asr_config_event
{
    asr_inference_t demo;
    asr_language_t lang;
    uint8_t ptt;
    /* ptt = 0 - device awaken via Wake Word.
     * ptt = 1 - device awaken via another source (Button press, LCD touch etc.) */
} set_asr_config_event_t;

typedef struct _event_voice
{
    event_base_t event_base;

    union
    {
        void *data;
        audio_in_event_t audio_in;
        audio_out_event_t audio_out;
        asr_feedback_t asr_feedback;
        speaker_playing_event_t speaker_audio;
        set_timeout_duration_event_t set_timeout_duration;
        set_followup_status_event_t set_followup_status;
        set_multilingual_config_event_t set_multilingual_config;
        set_voice_demo_event_t set_voice_demo;
        set_asr_config_event_t set_asr_config;
    };
} event_voice_t;

#endif /* _HAL_EVENT_DESCRIPTOR_VOICE_H_ */
