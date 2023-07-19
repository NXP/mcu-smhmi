/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef HAL_VOICE_ALGO_ASR_LOCAL_H_
#define HAL_VOICE_ALGO_ASR_LOCAL_H_

#include <sln_asr.h>
#include <stdint.h>
#include <string.h>
#include <board_define.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define IMXRT105S (0)

/* Max number of languages that are supported. Can be increased if needed. */
#define MAX_NUM_LANGUAGES (4)

/* Max number of groups that are supported. 3 slots are reserved. Can be increased if needed. */
#define MAX_GROUPS (7)

#ifdef ENABLE_DSMT_ASR
#define DEFAULT_ACTIVE_LANGUAGE (ASR_ENGLISH | ASR_CHINESE | ASR_GERMAN | ASR_FRENCH)

#if SELF_WAKE_UP_PROTECTION
#define MAX_INTERFACES_LANG (MAX_NUM_LANGUAGES * 2)
#define MAX_INTERFACES_WW   (MAX_NUM_LANGUAGES * 2)
#define MAX_INTERFACES_CMD  (2)
#else
#define MAX_INTERFACES_LANG (MAX_NUM_LANGUAGES)
#define MAX_INTERFACES_WW   (MAX_NUM_LANGUAGES)
#define MAX_INTERFACES_CMD  (1)
#endif /* SELF_WAKE_UP_PROTECTION */

#else
#define DEFAULT_ACTIVE_LANGUAGE (ASR_ENGLISH)
#endif /* ENABLE_DSMT_ASR */

// set to 500 from v2.2.16.2
#define k_nMaxTime (500)

// the response waiting time in ASR session
#define TIMEOUT_TIME_IN_MS 60000

// Out-of-box demo languages. Developers can add more language. Note that the runtime max number is up to four
// languages.
typedef enum _asr_languages
{
    UNDEFINED_LANGUAGE = 0,

    ASR_ENGLISH        = (1U << 0U),
    ASR_CHINESE        = (1U << 1U),
    ASR_GERMAN         = (1U << 2U),
    ASR_FRENCH         = (1U << 3U),

    LAST_LANGUAGE,

    /* Below entries are used for self protection mechanism.
     * Don't guard below entries to avoid the need of defining SELF_WAKE_UP_PROTECTION on CM4. */
    ASR_ENGLISH_SELF        = (1U << 10U),
    ASR_CHINESE_SELF        = (1U << 11U),
    ASR_GERMAN_SELF         = (1U << 12U),
    ASR_FRENCH_SELF         = (1U << 13U),
} asr_language_t;

typedef enum _asr_inference
{
    UNDEFINED_INFERENCE = 0,

    /* Wake Word model - used by all demos */
    ASR_WW = (1U << 0U),

    /* Voice Commands models for Coffee Machine demo */
    ASR_CMD_COFFEE_MACHINE = (1U << 1U), /* Coffee configuration (ex: Cappuccino, Strong, Small etc.) */

    /* Voice Commands models for Elevator demo */
    ASR_CMD_ELEVATOR       = (1U << 1U), /* Floor selection (ex: Floor one, Floor two etc.) */

    /* Voice Commands models for Home Panel demo */
    ASR_CMD_HP_MAIN_MENU    = (1U << 1U), /* Sub-demo selection (ex: Thermostat, Security etc.) */
    ASR_CMD_HP_THERMOSTAT   = (1U << 2U), /* Thermostat configuration (ex: Slow, Twenty etc.) */
    ASR_CMD_HP_SECURITY     = (1U << 3U), /* Security configuration (ex: Activate, Front door etc.) */
    ASR_CMD_HP_AUDIO_PLAYER = (1U << 4U), /* Audio Player configuration (ex: Next Song, Volume Up etc.) */
} asr_inference_t;

struct asr_language_model;   // will be used to install the selected languages.
struct asr_inference_engine; // will be used to install and set the selected WW/CMD inference engines.

struct asr_language_model
{
    asr_language_t iWhoAmI; // language types for d-spotter language model. A model is language specific.
    uint8_t
        nGroups; // base, group1 (ww), group2 (commands set 1), group3 (commands set 2), ..., groupN (commands set N)
    unsigned char *addrBin;                        // d-spotter model binary address
    unsigned char *addrGroup[MAX_GROUPS];          // addresses for base, group1, group2, ...
    unsigned char *addrGroupMapID[MAX_GROUPS - 1]; // addresses for mapIDs for group1, group2, ...
    struct asr_language_model *next;               // pointer to next language model in this linked list
};

struct asr_inference_engine
{
    asr_inference_t iWhoAmI_inf;   // inference types for WW engine or CMD engine
    asr_language_t iWhoAmI_lang;   // language for inference engine
    void *handler;                 // d-spotter handler
    uint8_t nGroups;               // the number of groups for an inference engine. Default is 2 and it's enough.
    unsigned char *addrGroup[2];   // base + keyword group. default nGroups is 2
    unsigned char *addrGroupMapID; // mapID group. default nGroups is 1
    char **idToKeyword;            // the string list
    unsigned char *memPool;        // memory pool in ram for inference engine
    uint32_t memPoolSize;          // memory pool size
    struct asr_inference_engine
        *next; // pointer to next inference engine, if this is linked list. The end of "next" should be NULL.
};

typedef struct _asr_voice_control
{
    struct asr_language_model *langModel;      // linked list
    struct asr_inference_engine *infEngineWW;  // linked list
    struct asr_inference_engine *infEngineCMD; // not linked list
    uint32_t sampleCount;                      // to measure the waiting response time
    asr_result_t result;                       // results of the command processing
} asr_voice_control_t;

typedef enum _app_flash_status
{
    READ_SUCCESS  = (1U << 0U),
    READ_FAIL     = (1U << 1U),
    READ_READY    = (1U << 2U),
    WRITE_SUCCESS = (1U << 3U),
    WRITE_FAIL    = (1U << 4U),
    WRITE_READY   = (1U << 5U),
} app_flash_status_t;

typedef enum _asr_mute
{
    ASR_MUTE_OFF = 0,
    ASR_MUTE_ON,
} asr_mute_t;

typedef enum _asr_followup
{
    ASR_FOLLOWUP_OFF = 0,
    ASR_FOLLOWUP_ON,
} asr_followup_t;

typedef enum _asr_ptt
{
    ASR_PTT_OFF = 0,
    ASR_PTT_ON,
} asr_ptt_t;

typedef enum _asr_cmd_res
{
    ASR_CMD_RES_OFF = 0,
    ASR_CMD_RES_ON,
} asr_cmd_res_t;

typedef enum _asr_cfg_demo
{
    ASR_CFG_DEMO_NO_CHANGE = (1U << 0U), // OOB demo type or languages unchanged
    ASR_CFG_CMD_INFERENCE_ENGINE_CHANGED =
        (1U << 1U),                             // OOB demo type (iot, elevator, audio, wash, led, dialog) changed
    ASR_CFG_DEMO_LANGUAGE_CHANGED = (1U << 2U), // OOB language type changed
} asr_cfg_demo_t;

typedef struct _asr_voice_config
{
    app_flash_status_t status;
    asr_cfg_demo_t asrCfg;
    asr_mute_t mute;
    uint32_t timeout; // in millisecond
    asr_followup_t followup;
    asr_inference_t demo;        // demo types: LED (demo #1) / iot, elevator, audio, wash (demo #2) / dialog (demo #3)
    asr_language_t multilingual; // runtime language types (demo #2 and #3)
    asr_language_t currentLanguage;
    asr_ptt_t ptt;
    asr_cmd_res_t cmdresults;
} asr_voice_config_t;

typedef enum _asr_voice_detect_status
{
    ASR_WW_DETECT = 0,
    ASR_CMD_DETECT,
    ASR_TIMEOUT,
    ASR_NONE
} asr_voice_detect_status_t;

#define ASR_KEYWORDID_INVALID (-1)

typedef struct _asr_inference_result
{
    asr_voice_detect_status_t status;
    asr_language_t language;
    int32_t keywordID;
    asr_inference_t demo;
} asr_inference_result_t;

#if defined(__cplusplus)
}
#endif

#endif /* HAL_VOICE_ALGO_ASR_LOCAL_H_ */
