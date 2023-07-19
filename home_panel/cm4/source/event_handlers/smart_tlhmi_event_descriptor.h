/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief smart tlhmi-specific event descriptor declarations.
 */

#ifndef _SMART_TLHMI_EVENT_DESCRIPTOR_H_
#define _SMART_TLHMI_EVENT_DESCRIPTOR_H_

#include "hal_event_descriptor_common.h"
#include "hal_event_descriptor_face_rec.h"
#include "custom.h"

#define TEMPERATURE_VALUE_SIZE 6

typedef enum _event_smart_tlhmi_id
{
    kEventID_HomePanelType = kEventType_App,
    kEventID_SetKeyboardOverlay,
    kEventID_GetPreviewMode,
    kEventID_SetPreviewMode,
    kEventID_MediaPlayer_TrackInfo,
    kEventID_MediaPlayer_TrackPosition,
    kEventID_MediaPlayerInfo,
    kEventFaceRecId_RegisterSecuirtyUser,
    kEventFaceRecId_RespondAddUser,
    kEventFaceRecId_RespondUpdateUser,
    kEventFaceRecId_RespondAddSecurityUser,
    kEventFaceRecId_RespondUpdateSecurityUser,
    kEventFaceRecId_DelSecurityUser,
    kEventID_SmartTLHMIID_COUNT
} event_smart_tlhmi_id_t;

typedef enum _preview_mode
{
    PREVIEW_MODE_CAMERA,
    PREVIEW_MODE_VIRTUAL_FACE,
} preview_mode_t;

typedef enum _display_source
{
    DISPLAY_SOURCE_RGB,
    DISPLAY_SOURCE_IR,
} display_source_t;

typedef struct _track_info
{
    uint8_t songArtist[32];
    uint8_t songName[32];
    uint32_t songDurationS;
    uint32_t offset;
} track_info_t;

typedef struct _media_player_info
{
    uint8_t isPlaying;
    uint8_t volume;
} media_player_info_t;

typedef struct _register_security_user_event
{
    uint16_t id;
    uint8_t name[FACE_NAME_MAX_LEN];
} register_security_user_event_t;

typedef struct
{
    uint16_t id;
    uint8_t name[FACE_NAME_MAX_LEN];
    uint8_t mode;
    uint8_t fan_speed;
    uint8_t temperature[TEMPERATURE_VALUE_SIZE];
} respond_add_thermostat_user_event_t;

typedef struct _event_smart_tlhmi
{
    event_base_t eventBase;

    union
    {
        void *data;
        preview_mode_t previewMode;
        track_info_t trackInfo;
        media_player_info_t mediaPlayerInfo;
        register_security_user_event_t securityUser;
        respond_add_thermostat_user_event_t thermostatUserInfo;
    };
} event_smart_tlhmi_t;

typedef struct _thermostat_user_list_obj_
{
    uint16_t face_id;
    uint8_t user_name[FACE_NAME_MAX_LEN];
    uint8_t temperature[TEMPERATURE_VALUE_SIZE];
    uint8_t mode;
    uint8_t fan_speed;
} thermostat_user_list_obj;

typedef struct _security_user_list_obj_t
{
    uint16_t face_id;
    uint8_t  user_name[FACE_NAME_MAX_LEN];
} security_user_list_obj_t;

typedef struct _security_users_share_db
{
    uint32_t count;
    security_user_list_obj_t objs[MAX_FACE_DB_SIZE];
} security_users_share_db_t;

typedef struct _thermostat_share_db
{
    uint32_t count;
    thermostat_user_list_obj objs[MAX_FACE_DB_SIZE];
} thermostat_users_share_db_t;

typedef struct
{
    thermostat_users_share_db_t thermostat_list;
    security_users_share_db_t security_list;
} users_share_db_t;

#endif /* _SMART_TLHMI_EVENT_DESCRIPTOR_H_ */
