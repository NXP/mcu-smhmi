/*
 * Copyright 2021-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _SLN_AFE_H_
#define _SLN_AFE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SLN_AFE_NXP

/* Lower WAKE_WORD_MAX_LENGTH_MS reduces memory usage, but reduces longer wake words detection rate. */
#ifndef WAKE_WORD_MAX_LENGTH_MS
#define WAKE_WORD_MAX_LENGTH_MS 1500
#endif /* WAKE_WORD_MAX_LENGTH_MS */

/* Lower AEC_FILTER_LENGTH reduces CPU usage, but reduces AEC performance as well. */
#ifndef AEC_FILTER_LENGTH
#define AEC_FILTER_LENGTH 150
#endif /* AEC_FILTER_LENGTH */

/* SLN_AFE requires different amount of memory depending on configuration used:
 * WW length - 1500ms, AEC - enabled , mics count - 2 ===> 118KB
 * WW length - 1500ms, AEC - enabled , mics count - 3 ===> 171KB
 *
 * WW length - 1500ms, AEC - disabled, mics count - 2 ===>  42KB
 * WW length - 1500ms, AEC - disabled, mics count - 3 ===>  66KB
 *
 * WW length - 3000ms, AEC - enabled , mics count - 2 ===> 146KB
 * WW length - 3000ms, AEC - enabled , mics count - 3 ===> 221KB
 *
 * WW length - 3000ms, AEC - disabled, mics count - 2 ===>  73KB
 * WW length - 3000ms, AEC - disabled, mics count - 3 ===> 119KB
 */
#if defined(WAKE_WORD_MAX_LENGTH_MS) && (WAKE_WORD_MAX_LENGTH_MS > 0)
#if WAKE_WORD_MAX_LENGTH_MS <= 1500
#if !AMP_LOOPBACK_DISABLED
#define AFE_MEM_SIZE_2MICS (1024 * 118)
#define AFE_MEM_SIZE_3MICS (1024 * 171)
#else
#define AFE_MEM_SIZE_2MICS (1024 * 42)
#define AFE_MEM_SIZE_3MICS (1024 * 66)
#endif /* !AMP_LOOPBACK_DISABLED */
#elif WAKE_WORD_MAX_LENGTH_MS <= 3000
#if !AMP_LOOPBACK_DISABLED
#define AFE_MEM_SIZE_2MICS (1024 * 148)
#define AFE_MEM_SIZE_3MICS (1024 * 221)
#else
#define AFE_MEM_SIZE_2MICS (1024 * 73)
#define AFE_MEM_SIZE_3MICS (1024 * 119)
#endif /* !AMP_LOOPBACK_DISABLED */
#else
#error "WAKE_WORD_MAX_LENGTH_MS must not exceed value 3000"
#endif /* WAKE_WORD_MAX_LENGTH_MS */
#endif /* WAKE_WORD_MAX_LENGTH_MS */

/*!
 * @brief Error codes for sln_afe
 */
typedef enum _sln_afe_status
{
    kAfeUnsupported = -5,
    kAfeMemError    = -4,
    kAfeOutOfMemory = -3,
    kAfeNullPointer = -2,
    kAfeFail        = -1,
    kAfeSuccess     = 0
} sln_afe_status_t;

/*!
 * @brief Output type configuration for the AFE wrapper.
 */
typedef enum _afe_data_type
{
    kAfeTypeFloat, /*!< The AFE input/output stream will be an array of float values. */
    kAfeTypeInt16, /*!< The AFE input/output stream will be an array of int16 values. */
    kAfeTypeInt32  /*!< The AFE input/output stream will be an array of int32 values. */
} afe_data_type_t;

/* Allocate and Deallocate functions */
typedef void *(*afe_malloc_func_t)(size_t size);
typedef void (*afe_free_func_t)(void *);

/*!
 * @brief AFE configuration structure.
 */
typedef struct _sln_afe_config
{
    uint8_t numberOfMics;         /*!< The number of microphones. Only 2/3 microphones are supported. */
    uint8_t *afeMemBlock;         /*!< Private heap for the AFE. */
    uint32_t afeMemBlockSize;     /*!< Size of the private heap. */
    float postProcessedGain;      /*!< The amount of dynamic gain after processing. */
    uint32_t wakeWordMaxLength;   /*!< Max time duration of a wake word in milliseconds. */
    int32_t aecEnabled;           /*!< AEC feature enabled. */
    int32_t doaEnabled;           /*!< DOA feature enabled. */
    uint32_t aecFilterLength;     /*!< AEC filter length (bigger length => better performance). */
    float micsPosition[3][3];     /*!< Mics coordinates in mm */
    afe_data_type_t dataInType;   /*!< Used for configuring the input type */
    afe_data_type_t dataOutType;  /*!< Used for configuring the output type */
    afe_malloc_func_t mallocFunc; /*!< Hook used for allocating memory */
    afe_free_func_t freeFunc;     /*!< Hook used for freeing memory */
} sln_afe_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

/*!
 * @brief Initialize Audio Front End (AFE) engine
 *
 * @param afeConfig  Reference to AFE configuration used by application
 * @returns          Initialization status
 */
sln_afe_status_t SLN_AFE_Init(sln_afe_config_t *afeConfig);

/*!
 * @brief AFE Audio processing function
 *
 * @param audioBUff      Input audio capture buffer for audio front end processing
 * @param refSignal      Reference signal buffer containing audio playback signal for barge-in
 * @param processedAudio Address where the pointer towards the audio output signal processed by AFE will be stored
 * @returns              Processing status
 */
sln_afe_status_t SLN_AFE_Process_Audio(void *audioBuff, int16_t *refSignal, void **processedAudio);

/*!
 * @brief This function should be called (from application side) when a wake word is detected by ASR
 *        This way AFE is informed when a Wake Word was uttered so it can recalibrate and offer a better performance
 *
 * @param wakeWordLength Number of samples from the beginning of detected wake word until now
 * @returns              Callback status
 */
sln_afe_status_t SLN_AFE_Trigger_Found(uint32_t wakeWordLength);

/*!
 * @brief Check for Voice Activity inside provided audio buffer.
 *        In case the target is gating ASR, provide "clean" buffer processed by AFE (output of SLN_AFE_Process_Audio).
 *        In case the target is gating AFE, provide raw PCM data obtained from a microphone.
 *
 * @param audio         Pointer to the buffer containing audio to be analyzed
 * @param voiceDetected Pointer where will be stored True if human voice detected, False otherwise
 * @returns             Processing status
 */
sln_afe_status_t SLN_AFE_Voice_Detected(int16_t *audio, bool *voiceDetected);

#if defined(__cplusplus)
}
#endif /* _cplusplus */

#endif /* _SLN_AFE_H_ */
