/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the license terms that accompany it. By expressly accepting
 * such terms or by downloading, installing, activating and/or otherwise using
 * the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software.
 */

/*
 * @brief sln face database reference declaration.
 * Simple database implementation for reference use and algorithm bring-up purposes.
 */

#ifndef _HAL_SLN_FACE_DB_H_
#define _HAL_SLN_FACE_DB_H_

#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define INVALID_ID        0xFFFF
#define MAX_FACE_DB_SIZE  (100U)
#define FACE_NAME_MAX_LEN (31U)

#ifndef AUTOSAVE
#define AUTOSAVE 1
#endif

typedef enum _facedb_status
{
    kFaceDBStatus_Success,
    kFaceDBStatus_AlreadyInit,
    kFaceDBStatus_NotInit,
    kFaceDBStatus_VersionMismatch,
    kFaceDBStatus_NotEnoughMemory,
    kFaceDBStatus_Full,
    kFaceDBStatus_WrongID,
    kFaceDBStatus_WrongParam,
    kFaceDBStatus_Failed,
} facedb_status_t;

typedef struct _facedb_ops
{
    facedb_status_t (*init)(uint16_t featureSize);
    facedb_status_t (*saveFace)(void);
    facedb_status_t (*addFace)(uint16_t id, char *name, void *face, int size);
    facedb_status_t (*delFaceWithId)(uint16_t id);
    facedb_status_t (*delFaceWithName)(char *name);
    facedb_status_t (*updNameWithId)(uint16_t id, char *name);
    facedb_status_t (*updFaceWithId)(uint16_t id, char *name, void *face, int size);
    facedb_status_t (*getFaceWithId)(uint16_t id, void **pFace);
    facedb_status_t (*getIdsAndFaces)(uint16_t *face_ids, void **pFace);
    facedb_status_t (*getIdWithName)(char *name, uint16_t *id);
    facedb_status_t (*genId)(uint16_t *new_id);
    facedb_status_t (*getIds)(uint16_t *face_ids);
    bool (*getSaveStatus)(uint16_t id);
    int (*getFaceCount)(void);
    char *(*getNameWithId)(uint16_t id);
} facedb_ops_t;

extern const facedb_ops_t g_facedb_ops;

#endif /*_HAL_SLN_FACE_DB_H_*/
