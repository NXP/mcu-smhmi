// SPDX-License-Identifier: MIT
// Copyright 2022-2023 NXP

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

#define BUTTON_PRESS_ANIMATION 500

static uint8_t s_audioPlayerBargeIn = false;

static audio_player_obj_t s_AudioPlayerObj = {
    .audio_info.track_name  = NULL,
    .audio_info.artist_name = NULL,
    //.audio_info.album_img    = &_Home_Control_panel__main_audio_player_album_340x340,
    .audio_info.elapsed_time = 0,
    .audio_info.total_time   = 30,
    .state                   = kAudioState_Pause,
    .speaker_volume          = kAudioVolume_Default,
};

static void gui_unpress_button(struct _lv_timer_t *pTimer)
{
    lv_obj_clear_state(pTimer->user_data, LV_STATE_PRESSED);
    lv_obj_add_state(pTimer->user_data, LV_STATE_DEFAULT);
    lv_timer_pause(pTimer);
}

static void gui_press_button(lv_obj_t *obj)
{
    lv_obj_clear_state(obj, LV_STATE_DEFAULT);
    lv_obj_add_state(obj, LV_STATE_PRESSED);
}

void refresh_audio_player_state()
{
    audio_state_t new_state = get_audio_player_state();
    if (new_state == kAudioState_Play)
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
    lv_label_set_text_fmt(guider_ui.screen_audio_player_label_track_time, "%02lu:%02lu",
                          s_AudioPlayerObj.audio_info.elapsed_time / 60, s_AudioPlayerObj.audio_info.elapsed_time % 60);
    // set audio current progress
    uint16_t bg_angle_end = lv_arc_get_bg_angle_end(guider_ui.screen_audio_player_arc_progress);
    lv_arc_set_angles(guider_ui.screen_audio_player_arc_progress, 0,
                      s_AudioPlayerObj.audio_info.elapsed_time * bg_angle_end / s_AudioPlayerObj.audio_info.total_time);
}

void refresh_audio_player_info()
{
    // set audio name
    lv_label_set_text(guider_ui.screen_audio_player_label_audio_name, s_AudioPlayerObj.audio_info.artist_name);
    lv_label_set_text(guider_ui.screen_audio_player_label_title, s_AudioPlayerObj.audio_info.track_name);
    // set album img
    // lv_img_set_src(guider_ui.screen_audio_player_img_album, s_AudioPlayerObj.audio_info.album_img);

    // set audio current and total time
    lv_label_set_text_fmt(guider_ui.screen_audio_player_label_track_time, "%02lu:%02lu",
                          s_AudioPlayerObj.audio_info.elapsed_time / 60, s_AudioPlayerObj.audio_info.elapsed_time % 60);
    lv_label_set_text_fmt(guider_ui.screen_audio_player_label_album_time, "%02lu:%02lu",
                          s_AudioPlayerObj.audio_info.total_time / 60, s_AudioPlayerObj.audio_info.total_time % 60);
    // set audio current progress
    uint16_t bg_angle_end = lv_arc_get_bg_angle_end(guider_ui.screen_audio_player_arc_progress);
    lv_arc_set_angles(guider_ui.screen_audio_player_arc_progress, 0,
                      s_AudioPlayerObj.audio_info.elapsed_time * bg_angle_end / s_AudioPlayerObj.audio_info.total_time);
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

        UI_VolumeChangeNotify(s_AudioPlayerObj.speaker_volume);

        if (s_audioPlayerBargeIn == true)
        {
            s_audioPlayerBargeIn = false;
        }
    }
}

void set_speaker_volume_alone(audio_volume_t volume)
{
    if (kAudioVolume_Invalid < volume && volume < kAudioVolume_Max)
    {
        s_AudioPlayerObj.speaker_volume = volume;
    }
}

void audio_volume_up(void)
{
    uint8_t volume = s_AudioPlayerObj.speaker_volume;

    if (volume < kAudioVolume_Max)
    {
        volume += VOLUME_STEP_SIZE;
    }

    if (volume > 100)
    {
        volume = 100;
    }
    set_speaker_volume(volume);
}

void audio_volume_max(void)
{
    set_speaker_volume(100);
}

void audio_volume_mute(void)
{
    set_speaker_volume(0);
}

void audio_volume_down(void)
{
    uint8_t volume = s_AudioPlayerObj.speaker_volume;
    if (volume < VOLUME_STEP_SIZE)
    {
        volume = 0;
    }
    else
    {
        volume -= VOLUME_STEP_SIZE;
    }
    set_speaker_volume(volume);
}

/* touch volume up/down event handler */
void gui_volume_up(void)
{
    audio_volume_up();
    refresh_audio_player_volume();
}

void gui_volume_down(void)
{
    audio_volume_down();
    refresh_audio_player_volume();
}

void refresh_audio_player_volume(void)
{
    if (s_AudioPlayerObj.speaker_volume < 10)
    {
        lv_label_set_text_fmt(guider_ui.screen_audio_player_label_volume, " %d/100", s_AudioPlayerObj.speaker_volume);
    }
    else
    {
        lv_label_set_text_fmt(guider_ui.screen_audio_player_label_volume, "%02d/100", s_AudioPlayerObj.speaker_volume);
    }
}

void set_audio_position(uint32_t elapsed_time)
{
    s_AudioPlayerObj.audio_info.elapsed_time = elapsed_time;
}

void set_audio_info(const audio_info_t *audioInfo)
{
    /* load current audio info.*/
    s_AudioPlayerObj.audio_info.track_name  = audioInfo->track_name;
    s_AudioPlayerObj.audio_info.artist_name = audioInfo->artist_name;
    // s_AudioPlayerObj.audio_info.album_img = s_AlbumImg[index];
    s_AudioPlayerObj.audio_info.total_time   = audioInfo->total_time;
    s_AudioPlayerObj.audio_info.elapsed_time = audioInfo->elapsed_time;
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
        gui_audio_player_set_language(new_language);
    }
}

void gui_audio_player_set_language(language_t language)
{
    lv_dropdown_set_selected(guider_ui.screen_audio_player_ddlist_language, language);
}

void gui_audio_player_init_state(language_t language)
{
    set_audio_player_state(kAudioState_Pause);
    refresh_audio_player_info();
    refresh_audio_player_state();
    refresh_audio_player_volume();
    UI_VolumeChangeNotify(s_AudioPlayerObj.speaker_volume);
    gui_audio_player_set_language(language);
}

void GUI_AudioPlayer_RefreshHandHolder(const void *handBuffer)
{
    _audio_player_hand_holder_180x200.data = handBuffer;
    lv_img_set_src(guider_ui.screen_audio_player_hand_holder, &_audio_player_hand_holder_180x200);
}

void GUI_AudioPlayer_VolumeUp()
{
    static lv_timer_t *s_pTimer = NULL;

    if (s_pTimer == NULL)
    {
        s_pTimer = lv_timer_create(&gui_unpress_button, BUTTON_PRESS_ANIMATION,
                                   (void *)guider_ui.screen_audio_player_imgbtn_volume_up);
    }

    if (s_pTimer)
    {
        lv_timer_reset(s_pTimer);
        lv_timer_resume(s_pTimer);
        gui_press_button(guider_ui.screen_audio_player_imgbtn_volume_up);
    }

    gui_volume_up();
}

void GUI_AudioPlayer_VolumeDown()
{
    static lv_timer_t *s_pTimer = NULL;

    if (s_pTimer == NULL)
    {
        s_pTimer = lv_timer_create(&gui_unpress_button, BUTTON_PRESS_ANIMATION,
                                   (void *)guider_ui.screen_audio_player_imgbtn_volume_down);
    }

    if (s_pTimer)
    {
        lv_timer_reset(s_pTimer);
        lv_timer_resume(s_pTimer);
        gui_press_button(guider_ui.screen_audio_player_imgbtn_volume_down);
    }

    gui_volume_down();
}

void GUI_AudioPlayer_VolumeMute()
{
    audio_volume_mute();
    refresh_audio_player_volume();
}

void GUI_AudioPlayer_VolumeMax()
{
    audio_volume_max();
    refresh_audio_player_volume();
}

void GUI_AudioPlayer_BargeIn(void)
{
    if (s_audioPlayerBargeIn == false)
    {
        /* Cut volume in 3rd */
        uint8_t volume       = s_AudioPlayerObj.speaker_volume / 3;
        s_audioPlayerBargeIn = true;
        UI_VolumeChangeNotify(volume);
    }
}

void GUI_AudioPlayer_BargeOut(void)
{
    if (s_audioPlayerBargeIn == true)
    {
        s_audioPlayerBargeIn = false;
        set_speaker_volume(s_AudioPlayerObj.speaker_volume);
        refresh_audio_player_volume();
    }
}

void GUI_AudioPlayer_Next()
{
    static lv_timer_t *s_pTimer = NULL;

    if (s_pTimer == NULL)
    {
        s_pTimer = lv_timer_create(&gui_unpress_button, BUTTON_PRESS_ANIMATION,
                                   (void *)guider_ui.screen_audio_player_imgbtn_next);
    }

    if (s_pTimer)
    {
        lv_timer_reset(s_pTimer);
        lv_timer_resume(s_pTimer);
        gui_press_button(guider_ui.screen_audio_player_imgbtn_next);
    }
}

void GUI_AudioPlayer_Previous()
{
    static lv_timer_t *s_pTimer = NULL;

    if (s_pTimer == NULL)
    {
        s_pTimer = lv_timer_create(&gui_unpress_button, BUTTON_PRESS_ANIMATION,
                                   (void *)guider_ui.screen_audio_player_imgbtn_previous);
    }

    if (s_pTimer)
    {
        lv_timer_reset(s_pTimer);
        lv_timer_resume(s_pTimer);
        gui_press_button(guider_ui.screen_audio_player_imgbtn_previous);
    }
}
