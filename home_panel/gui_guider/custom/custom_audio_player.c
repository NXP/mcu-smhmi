// SPDX-License-Identifier: MIT
// Copyright 2022 NXP

/**
 * @file custom.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "custom.h"
#include "lvgl/lvgl.h"
#include <stdio.h>

static audio_player_obj_t s_AudioPlayerObj = {
    .audio_info.name = NULL,
    //.audio_info.album_img    = &_Home_Control_panel__main_audio_player_album_340x340,
    .audio_info.elapsed_time = 0,
    .audio_info.total_time   = 0,
    .state                   = kAudioState_Pause,
    .speaker_volume          = kAudioVolume_Default,
};

void refresh_audio_player_state()
{
    audio_state_t new_state = get_audio_player_state();
    if (new_state == kAudioState_Pause)
    {
        lv_obj_add_flag(guider_ui.screen_audio_player_imgbtn_play, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_audio_player_imgbtn_pause, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_clear_flag(guider_ui.screen_audio_player_imgbtn_play, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_audio_player_imgbtn_pause, LV_OBJ_FLAG_HIDDEN);
    }
}

void set_audio_player_state(audio_state_t state)
{
    s_AudioPlayerObj.state = state;
}

audio_state_t get_audio_player_state(void)
{
    return s_AudioPlayerObj.state;
}

void switch_audio_player_state(audio_state_t new_state)
{
    if (kAudioState_Invalid < new_state && new_state < kAudioState_Max)
    {
        set_audio_player_state(new_state);

        if (new_state == kAudioState_Play)
        {
            UI_AudioPlaying_Callback(kAudioPlayer_Play);
        }
        else
        {
            UI_AudioPlaying_Callback(kAudioPlayer_Pause);
        }

        refresh_audio_player_state();
    }
}

void refresh_audio_position()
{
    // set audio current and total time
    lv_label_set_text_fmt(guider_ui.screen_audio_player_label_track_time, "%02d:%02d",
                          s_AudioPlayerObj.audio_info.elapsed_time / 60, s_AudioPlayerObj.audio_info.elapsed_time % 60);
}

void refresh_audio_player_info()
{
    // set audio name
    lv_label_set_text(guider_ui.screen_audio_player_label_audio_name, s_AudioPlayerObj.audio_info.name);

    // set album img
    // lv_img_set_src(guider_ui.screen_audio_player_img_album, s_AudioPlayerObj.audio_info.album_img);

    // set audio current and total time
    lv_label_set_text_fmt(guider_ui.screen_audio_player_label_track_time, "%02d:%02d",
                          s_AudioPlayerObj.audio_info.elapsed_time / 60, s_AudioPlayerObj.audio_info.elapsed_time % 60);
    lv_label_set_text_fmt(guider_ui.screen_audio_player_label_album_time, "%02d:%02d",
                          s_AudioPlayerObj.audio_info.total_time / 60, s_AudioPlayerObj.audio_info.total_time % 60);
}

audio_volume_t get_speaker_volume(void)
{
    return s_AudioPlayerObj.speaker_volume;
}

void set_speaker_volume(audio_volume_t volume)
{
    if (kAudioVolume_Invalid < volume && volume < kAudioVolume_Max)
    {
        s_AudioPlayerObj.speaker_volume = volume;
    }
}

void gui_volume_up(void)
{
    if (s_AudioPlayerObj.speaker_volume < kAudioVolume_Max)
    {
        s_AudioPlayerObj.speaker_volume += VOLUME_STEP_SIZE;
    }
    if (s_AudioPlayerObj.speaker_volume > 100)
    {
        s_AudioPlayerObj.speaker_volume = 100;
    }
    refresh_audio_player_volume();
}

void gui_volume_down(void)
{
    if (s_AudioPlayerObj.speaker_volume > 0)
    {
        s_AudioPlayerObj.speaker_volume -= VOLUME_STEP_SIZE;
    }
    if (s_AudioPlayerObj.speaker_volume < 0)
    {
        s_AudioPlayerObj.speaker_volume = 0;
    }
    refresh_audio_player_volume();
}

void refresh_audio_player_volume(void)
{
    lv_label_set_text_fmt(guider_ui.screen_audio_player_label_volume, "%02d/100", s_AudioPlayerObj.speaker_volume);
    UI_VolumeChangeNotify(s_AudioPlayerObj.speaker_volume);
}

void set_audio_position(uint32_t elapsed_time)
{
    s_AudioPlayerObj.audio_info.elapsed_time = elapsed_time;
}

void set_audio_info(audio_info_t audioInfo)
{
    /* load current audio info.*/
    s_AudioPlayerObj.audio_info.name = audioInfo.name;
    // s_AudioPlayerObj.audio_info.album_img = s_AlbumImg[index];
    s_AudioPlayerObj.audio_info.total_time   = audioInfo.total_time;
    s_AudioPlayerObj.audio_info.elapsed_time = audioInfo.elapsed_time;
}

void switch_audio_clip(audio_event_t switch_event, audio_state_t state)
{
    /* switch to next or previous song clip */
    UI_AudioPlaying_Callback(switch_event);

    set_audio_player_state(state);
    refresh_audio_player_info();
    refresh_audio_player_state();
}

void gui_audio_player_language_changed_cb(void)
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.screen_audio_player_ddlist_language);
    language_t new_language = (language_t)opt_id;
    if ((new_language >= kLanguage_EN) && (new_language < kLanguage_Ids))
    {
        UI_SetLanguage_Callback(new_language);
    }
}
