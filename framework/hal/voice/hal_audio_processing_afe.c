/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board_define.h"
#ifdef ENABLE_AUDIO_PROCESSING_DEV_Afe
#include "stdint.h"
#include "fwk_audio_processing.h"
#include "fwk_log.h"
#include "fwk_platform.h"
#include "hal_audio_processing_dev.h"
#include "hal_audio_defs.h"
#include "hal_event_descriptor_voice.h"
#include "sln_afe.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if !AMP_LOOPBACK_DISABLED
/* Defines used to sync microphones with speaker */
#define PCM_CYCLE_DURATION_US  10000
#define SPEAKER_CONST_DELAY_US 3210

/* Should be the same with MQS_FEEDBACK_SLOT_CNT */
#define SPEAKER_FEEDBACK_SLOTS_CNT 3

typedef enum _sln_speaker_feedback_state
{
    kSpeakerFeedbackIdle,
    kSpeakerFeedbackFirstPacket,
    kSpeakerFeedbackSyncing,
    kSpeakerFeedbackPlaying,
} sln_speaker_feedback_state_t;

typedef struct _sln_speaker_feedback
{
    int16_t *audio_stream;
    uint32_t audio_length;
    uint32_t audio_played;
    uint32_t start_time;
} sln_speaker_feedback_t;
#endif /* !AMP_LOOPBACK_DISABLED */

#ifdef ENABLE_OUTPUT_DEV_AudioDump
typedef enum clean_consumer
{
    ASR_UNKNOWN,
    ASR_SKIPPED,
    ASR_WW_MODEL,
    ASR_CMD_MODEL
} clean_consumer_t;

typedef struct stream_header
{
    /*
     * 0: louder speaker is not playing. 1: louder speaker is playing
     */
    uint8_t amp_flag;
    /*
     *  UNKNOW: not consumed by asr yet. ASR_WW_MODEL: consumed by WW model. ASR_CMD_MODEL: consumed by CMD model.
     */
    uint8_t clean_consumer;
    uint16_t frame_size_in_byte;
    uint32_t frame_seq;
} stream_header_t;

#define STREAM_HEADER_SIZE    8
#define AUDIO_DUMP_CHUNKS_CNT 3
#define AUDIO_DUMP_SLOTS_CNT  (ASR_INPUT_CYCLE_SLOTS * AUDIO_DUMP_CHUNKS_CNT)
#define AUDIO_DUMP_SLOT_SIZE \
    (STREAM_HEADER_SIZE + AFE_INPUT_MIC_BUFFER_SIZE + AFE_INPUT_AMP_BUFFER_SIZE + AFE_OUTPUT_BUFFER_SIZE)

/* Skip first frames of audio dump to be sure that audio dump task is ready. */
#if AQT_TEST
#define AUDIO_DUMP_SKIP_FIRST_FRAMES_CNT 0
#else
#define AUDIO_DUMP_SKIP_FIRST_FRAMES_CNT 99
#endif /*AQT_TEST*/

AT_NONCACHEABLE_SECTION_ALIGN_SDRAM(static uint8_t s_dumpOutPool[AUDIO_DUMP_SLOTS_CNT][AUDIO_DUMP_SLOT_SIZE], 4);

static uint32_t s_dumpBufferProduceSeq = 0;
static uint32_t s_dumpBufferConsumeSeq = 0;
#define SEQ_LEN_IN_BYTES 4

static void _preMarkcleanStreamConsumed(clean_consumer_t Type);
static void _preMarkcleanStreamSkipped(uint32_t seq);

#endif /* ENABLE_OUTPUT_DEV_AudioDump */

#if AQT_TEST
static uint8_t s_AFE_ByPass = 1;
#else
static uint8_t s_AFE_ByPass = 0;
#endif /*AQT_TEST*/

#if SELF_WAKE_UP_PROTECTION
/* Forward streamer's data to ASR for a self wake up detection check.
 * This requires an extra slot in communication buffer. */
#define ASR_INPUT_BUFFER_SLOTS         2
#define ASR_SPEAKER_PADDING_SILENCE_1S 100
#else
#define ASR_INPUT_BUFFER_SLOTS 1
#endif /* SELF_WAKE_UP_PROTECTION */

/*******************************************************************************
 * Variables
 ******************************************************************************/

static AT_NONCACHEABLE_SECTION_ALIGN_DTC(uint8_t s_afeExternalMemory[AFE_MEM_SIZE_2MICS], 4);

AT_NONCACHEABLE_SECTION_ALIGN_DTC(static uint8_t s_afeMicIn[AFE_INPUT_MIC_BUFFER_SIZE], 4);

AT_NONCACHEABLE_SECTION_ALIGN_DTC(
    static uint8_t s_asrInput[ASR_INPUT_CYCLE_SLOTS][ASR_INPUT_BUFFER_SLOTS][ASR_INPUT_BUFFER_SIZE], 4);

#ifdef ENABLE_OUTPUT_DEV_AudioDump
AT_NONCACHEABLE_SECTION_ALIGN_DTC(static uint32_t s_dumpSEQ[ASR_INPUT_CYCLE_SLOTS][ASR_INPUT_FRAMES], 4);
#endif /* ENABLE_OUTPUT_DEV_AudioDump */

static volatile uint32_t s_utteranceLength = 0;

#if !AMP_LOOPBACK_DISABLED
AT_NONCACHEABLE_SECTION_ALIGN_DTC(static uint8_t s_afeAmpIn[AFE_INPUT_AMP_BUFFER_SIZE], 4);

static sln_speaker_feedback_state_t s_speakerState                          = kSpeakerFeedbackIdle;
static uint8_t s_speakerFeedbackSlotWrite                                   = 0;
static uint8_t s_speakerFeedbackSlotRead                                    = 0;
static uint8_t s_speakerFeedbackSlotFull                                    = 0;
static sln_speaker_feedback_t s_speakerFeedback[SPEAKER_FEEDBACK_SLOTS_CNT] = {0};
#endif /* !AMP_LOOPBACK_DISABLED */

/* g_MQSPlaying is used to disable ASR processing while speaker is streaming prompts.
 * g_MQSPlaying should not be set to true in case barge-in is required. */
extern volatile bool g_MQSPlaying;

/* Performance Statistics. */
volatile uint32_t s_afeDataGenerated = 0;
volatile uint32_t s_afeDataProcessed = 0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void _convertMicDataForAfe(int32_t *src, void *dst);
static uint8_t _forwardDataToAsr(const audio_processing_dev_t *dev, void *afeCleanOut, void *afeAmp);

#if !AMP_LOOPBACK_DISABLED
static void _addSpeakerFeedback(int16_t *buffer, uint32_t length, uint32_t timeUs);
static uint32_t _consumeSpeakerFeedback(int16_t *dst, uint32_t length);
static int16_t *_getSpeakerFeedback(void);
#endif /* !AMP_LOOPBACK_DISABLED */

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * @brief Convert PCM stream buffer from LRLRLR...LR to LLL...LR...RRR.
 *
 * @param src Pointer to the buffer containing mic data from PCM component.
 * @param dst Pointer to the buffer where to store converted data.
 */
static void _convertMicDataForAfe(int32_t *src, void *dst)
{
    int32_t leftIdx  = 0;
    int32_t rightIdx = AUDIO_PCM_SINGLE_CH_SMPL_COUNT;

#if AFE_INPUT_MIC_SAMPLE_BYTES == 2
    int16_t *dstPtr = (int16_t *)dst;
#elif AFE_INPUT_MIC_SAMPLE_BYTES == 4
    int32_t *dstPtr = (int32_t *)dst;
#endif /* AFE_INPUT_MIC_SAMPLE_BYTES */

    for (int32_t i = 0; i < AUDIO_PCM_SAMPLE_COUNT; i += 2)
    {
#if AFE_INPUT_MIC_SAMPLE_BYTES == 2
        dstPtr[leftIdx++]  = (int16_t)(src[i] >> 16);
        dstPtr[rightIdx++] = (int16_t)(src[i + 1] >> 16);
#elif AFE_INPUT_MIC_SAMPLE_BYTES == 4
        dstPtr[leftIdx++]  = src[i];
        dstPtr[rightIdx++] = src[i + 1];
#endif /* AFE_INPUT_MIC_SAMPLE_BYTES */
    }
}

/**
 * @brief Store chunks of clean audio in a bigger buffer. Once accumulated enough chunks, send them to ASR.
 *
 * @param dev Pointer to the current device.
 * @param afeCleanOut Chunk to be stored.
 * @param afeAmp Chunk of streamer data to be stored.
 *
 * @return Number of currently stored chunks.
 */
static uint8_t _forwardDataToAsr(const audio_processing_dev_t *dev, void *afeCleanOut, void *afeAmp)
{
    static uint8_t s_accSamples = 0;
    static uint8_t s_slotIdx    = 0;
    static bool s_dirtySamples  = false;

    uint32_t dataToSendSize = (ASR_INPUT_BUFFER_SIZE / ASR_INPUT_SAMPLE_BYTES);

    if (g_MQSPlaying == true)
    {
        dataToSendSize = 0;
    }
#if SELF_WAKE_UP_PROTECTION
    else
    {
        static uint32_t s_afeAmpClear = 0;

        if (afeAmp != NULL)
        {
            memcpy(&s_asrInput[s_slotIdx][1][s_accSamples * AFE_OUTPUT_BUFFER_SIZE], afeAmp, AFE_OUTPUT_BUFFER_SIZE);
            dataToSendSize *= 2;
            s_afeAmpClear = ASR_SPEAKER_PADDING_SILENCE_1S;
        }
        else if (s_afeAmpClear > 0)
        {
            memset(&s_asrInput[s_slotIdx][1][s_accSamples * AFE_OUTPUT_BUFFER_SIZE], 0, AFE_OUTPUT_BUFFER_SIZE);
            dataToSendSize *= 2;
            s_afeAmpClear--;
        }
    }
#endif /* SELF_WAKE_UP_PROTECTION */

    if (dataToSendSize != 0)
    {
        s_dirtySamples = true;

        /* Pass output of AFE to wake word */
        memcpy(&s_asrInput[s_slotIdx][0][s_accSamples * AFE_OUTPUT_BUFFER_SIZE], afeCleanOut, AFE_OUTPUT_BUFFER_SIZE);

#ifdef ENABLE_OUTPUT_DEV_AudioDump
        s_dumpSEQ[s_slotIdx][s_accSamples] = s_dumpBufferProduceSeq;
        _preMarkcleanStreamConsumed(ASR_UNKNOWN);
#endif /* ENABLE_OUTPUT_DEV_AudioDump */

        /* If we've accumulated enough audio, send it to ASR */
        if (s_accSamples == (ASR_INPUT_FRAMES - 1))
        {
            if (dev->cap.callback != NULL)
            {
                audio_processing_event_t audio_processing = {0};
                if ((s_afeDataGenerated - s_afeDataProcessed) > (ASR_INPUT_CYCLE_SLOTS - 2))
                {
                    LOGE("ASR failed to process AFE data in time! %d != %d", s_afeDataGenerated, s_afeDataProcessed);
                }
                s_afeDataGenerated++;

                audio_processing.eventId   = kAudioProcessingEvent_Done;
                audio_processing.eventInfo = kEventInfo_Local;
                audio_processing.data      = s_asrInput[s_slotIdx];

#ifdef ENABLE_OUTPUT_DEV_AudioDump
                audio_processing.data_marks = s_dumpSEQ[s_slotIdx];
#endif /* ENABLE_OUTPUT_DEV_AudioDump */

                audio_processing.size = dataToSendSize;
                audio_processing.copy = 0;
                s_dirtySamples        = false;

                dev->cap.callback(dev, audio_processing, 0);
                s_slotIdx = (s_slotIdx + 1) % ASR_INPUT_CYCLE_SLOTS;
            }
        }
    }
    else
    {
        if (s_dirtySamples)
        {
            s_dirtySamples = false;
            memset(s_asrInput[s_slotIdx], 0, ASR_INPUT_BUFFER_SIZE);
#ifdef ENABLE_OUTPUT_DEV_AudioDump
            uint8_t dirtyCNT = s_accSamples + 1;
            uint8_t i;
            for (i = 0; i < dirtyCNT; i++)
            {
                s_dumpSEQ[s_slotIdx][i] = 0;
            }

            dirtyCNT = s_dumpBufferProduceSeq > dirtyCNT ? dirtyCNT : s_dumpBufferProduceSeq;
            while (dirtyCNT > 0)
            {
                _preMarkcleanStreamSkipped(s_dumpBufferProduceSeq - dirtyCNT);
                dirtyCNT--;
            }
#endif /* ENABLE_OUTPUT_DEV_AudioDump */
        }
#ifdef ENABLE_OUTPUT_DEV_AudioDump
        s_dumpSEQ[s_slotIdx][s_accSamples] = 0;
        _preMarkcleanStreamSkipped(s_dumpBufferProduceSeq);
#endif /* ENABLE_OUTPUT_DEV_AudioDump */
    }

    s_accSamples = (s_accSamples + 1) % ASR_INPUT_FRAMES;

    return s_accSamples;
}

#if !AMP_LOOPBACK_DISABLED
/**
 * @brief  Add speaker's audio packet to a feedback circular buffer in order to be used by AFE for AEC.
 *
 * @param  buffer Pointer to the buffer containing speaker's audio packet.
 * @param  length Length of speaker's audio packet.
 * @param  timeUs Timestamp when speaker started to play current audio packet.
 */
static void _addSpeakerFeedback(int16_t *buffer, uint32_t length, uint32_t timeUs)
{
    if (s_speakerFeedbackSlotFull < SPEAKER_FEEDBACK_SLOTS_CNT)
    {
        s_speakerFeedback[s_speakerFeedbackSlotWrite].audio_stream = buffer;
        s_speakerFeedback[s_speakerFeedbackSlotWrite].audio_length = length;
        s_speakerFeedback[s_speakerFeedbackSlotWrite].audio_played = 0;
        s_speakerFeedback[s_speakerFeedbackSlotWrite].start_time   = timeUs;

        if (s_speakerState == kSpeakerFeedbackIdle)
        {
            s_speakerState = kSpeakerFeedbackFirstPacket;
        }

        s_speakerFeedbackSlotWrite = (s_speakerFeedbackSlotWrite + 1) % SPEAKER_FEEDBACK_SLOTS_CNT;
        s_speakerFeedbackSlotFull++;
    }
    else
    {
        LOGE("[AFE ERROR] s_speakerFeedback buffer is full, discarding feedback packet");
    }
}

/**
 * @brief  Extract speaker's audio chunk from feedback buffer in order to be used by AFE for AEC.
 *
 * @param  dst Pointer to the buffer where to read speaker's audio chunk. Pass NULL to discard data.
 * @param  length Length of speaker's audio chunk that should be read (or discarded).
 *
 * @return Number of extracted samples.
 */
static uint32_t _consumeSpeakerFeedback(int16_t *dst, uint32_t length)
{
    uint32_t dataPlayed = 0;

    while ((dataPlayed < length) && (s_speakerFeedbackSlotFull > 0))
    {
        sln_speaker_feedback_t *speakerSlot = NULL;
        int16_t *data                       = NULL;
        uint32_t dataAvailable              = 0;

        speakerSlot   = &s_speakerFeedback[s_speakerFeedbackSlotRead];
        data          = &speakerSlot->audio_stream[speakerSlot->audio_played];
        dataAvailable = speakerSlot->audio_length - speakerSlot->audio_played;

        if (dataAvailable >= (length - dataPlayed))
        {
            dataAvailable = (length - dataPlayed);
        }

        if (dst != NULL)
        {
            memcpy((uint8_t *)(&dst[dataPlayed]), data, dataAvailable * sizeof(uint16_t));
        }

        dataPlayed += dataAvailable;
        speakerSlot->audio_played += dataAvailable;
        if (speakerSlot->audio_played >= speakerSlot->audio_length)
        {
            s_speakerFeedbackSlotRead = (s_speakerFeedbackSlotRead + 1) % SPEAKER_FEEDBACK_SLOTS_CNT;
            s_speakerFeedbackSlotFull--;
        }
    }

    if (dst != NULL)
    {
        /* Fill with zeroes dst buffer in case there is no enough data in the feedback buffer */
        memset(&dst[dataPlayed], 0, (length - dataPlayed) * sizeof(int16_t));
    }

    return dataPlayed;
}

/**
 * @brief  Get speaker's audio chunk from feedback buffer in order to be used by AFE for AEC.
 *
 * @return Pointer to the buffer containing feedback data from Speaker.
 */
static int16_t *_getSpeakerFeedback(void)
{
    static int32_t speakerTotalDelayUs = 0;
    static uint32_t speakerDelayedUs   = 0;

    int16_t *afeAmpIn = (int16_t *)s_afeAmpIn;

    if (s_speakerState == kSpeakerFeedbackFirstPacket)
    {
        uint32_t currentTime = FWK_CurrentTimeUs();
        /* Once received first chunk of an audio from the amplifier, it is required to calculate and add a "pre" delay
         * in order to synchronize speaker and mics streams. There is a chance that the first chunk is received too
         * late,
         * in this case instead of adding a "pre" delay, it is required to skip some chunks from the beginning of the
         * audio. */
        speakerTotalDelayUs = (int32_t)(currentTime - s_speakerFeedback[s_speakerFeedbackSlotRead].start_time);
        if (speakerTotalDelayUs <= 0)
        {
            speakerTotalDelayUs = 0;
        }

        speakerTotalDelayUs = PCM_CYCLE_DURATION_US - speakerTotalDelayUs + SPEAKER_CONST_DELAY_US;
        if (speakerTotalDelayUs == 0)
        {
            s_speakerState = kSpeakerFeedbackPlaying;
            LOGD("[AFE] No need for sync");
        }
        else if (speakerTotalDelayUs > 0)
        {
            if (speakerTotalDelayUs < PCM_CYCLE_DURATION_US)
            {
                s_speakerState = kSpeakerFeedbackSyncing;
                LOGD("[AFE] Sync required: Pre-Delay = %d [us]", speakerTotalDelayUs);
            }
            else
            {
                /* Current delay is too big, let the next AFE cycle handle it. */
                LOGD("[AFE] Sync required: Skip Pre-Delay = %d [us].", speakerTotalDelayUs);
            }
        }
        else
        {
            uint32_t samplesToSkip = (-1) * ((speakerTotalDelayUs * 16) / 1000);
            _consumeSpeakerFeedback(NULL, samplesToSkip);
            LOGD("[AFE] Sync required: Post-Skip = %d [us]   Samples skipped = %d", ((-1) * speakerTotalDelayUs),
                 samplesToSkip);

            s_speakerState = kSpeakerFeedbackPlaying;
        }
    }

    if (s_speakerState == kSpeakerFeedbackSyncing)
    {
        uint16_t currentDelayUs      = 0;
        uint16_t currentDelaySamples = 0;

        /* Max delayUs per cycle is 10000us */
        currentDelayUs = speakerTotalDelayUs - speakerDelayedUs;
        if (currentDelayUs > PCM_CYCLE_DURATION_US)
        {
            currentDelayUs = PCM_CYCLE_DURATION_US;
        }

        currentDelaySamples = (currentDelayUs * 16) / 1000;

        memset((uint8_t *)afeAmpIn, 0, currentDelaySamples * sizeof(int16_t));
        speakerDelayedUs += currentDelayUs;

        /* If the "pre" delay was consumed, fill the remaining bytes in the amplifier buffer with
         * the beginning of the audio packet and move to the second phase where audio packet is consumed
         * chunk by chunk for Audio Echo Cancellation. */
        if (speakerDelayedUs >= speakerTotalDelayUs)
        {
            _consumeSpeakerFeedback(&afeAmpIn[currentDelaySamples],
                                    (AUDIO_PCM_SINGLE_CH_SMPL_COUNT - currentDelaySamples));
            s_speakerState = kSpeakerFeedbackPlaying;

            LOGD("[AFE] Synced, Delayed = %d [us]  Consumed = %d [samples]", speakerDelayedUs,
                 (AUDIO_PCM_SINGLE_CH_SMPL_COUNT - currentDelaySamples));
        }
    }
    else if (s_speakerState == kSpeakerFeedbackPlaying)
    {
        uint32_t extractedFeedbackSize = 0;

        extractedFeedbackSize = _consumeSpeakerFeedback(afeAmpIn, AUDIO_PCM_SINGLE_CH_SMPL_COUNT);
        if (extractedFeedbackSize == 0)
        {
            LOGD("[AFE] Feedback buffer is empty");

            memset(afeAmpIn, 0, AUDIO_PCM_SINGLE_CH_SMPL_COUNT * sizeof(int16_t));

            s_speakerState             = kSpeakerFeedbackIdle;
            s_speakerFeedbackSlotWrite = 0;
            s_speakerFeedbackSlotRead  = 0;
            s_speakerFeedbackSlotFull  = 0;
            memset(s_speakerFeedback, 0, sizeof(s_speakerFeedback));

            speakerTotalDelayUs = 0;
            speakerDelayedUs    = 0;

            afeAmpIn = NULL;
        }
    }
    else
    {
        afeAmpIn = NULL;
    }

    return afeAmpIn;
}
#endif /* !AMP_LOOPBACK_DISABLED */

#ifdef ENABLE_OUTPUT_DEV_AudioDump
/**
 * @brief Send mic1, mic2, amp and clean (processed by AFE) streams to Audio Dump component.
 *
 * @param dev Pointer to the current device.
 * @param mic Pointer to the mics (mic1 + mic2) stream.
 * @param micSize Size in bytes of mic stream.
 * @param amp Pointer to the amp  stream.
 * @param ampSize Size in bytes of amp stream.
 * @param clean Pointer to the clean stream.
 * @param cleanSize Size in bytes of clean stream.
 */
static void _packDataToAudioDumpBuf(void *mic1,
                                    uint32_t mic1Size,
                                    void *mic2,
                                    uint32_t mic2Size,
                                    void *amp,
                                    uint32_t ampSize,
                                    void *clean,
                                    uint32_t cleanSize)
{
    uint8_t *dumpBuffer   = NULL;
    uint8_t dumpBufferIdx = 0;

    assert((s_dumpBufferProduceSeq - s_dumpBufferConsumeSeq) < AUDIO_DUMP_SLOTS_CNT);

    dumpBufferIdx = s_dumpBufferProduceSeq % AUDIO_DUMP_SLOTS_CNT;

    stream_header_t *dumpBufferHeader = (stream_header_t *)s_dumpOutPool[dumpBufferIdx];

    if (amp == NULL)
    {
        dumpBufferHeader->amp_flag = 0;
    }
    else
    {
        dumpBufferHeader->amp_flag = 1;
    }

    dumpBufferHeader->frame_seq          = s_dumpBufferProduceSeq;
    dumpBufferHeader->frame_size_in_byte = STREAM_HEADER_SIZE + mic1Size + mic2Size + ampSize + cleanSize;

    dumpBuffer = (uint8_t *)s_dumpOutPool[dumpBufferIdx];

    dumpBuffer += STREAM_HEADER_SIZE;
    memcpy(dumpBuffer, mic1, mic1Size);

    dumpBuffer += mic1Size;
    memcpy(dumpBuffer, mic2, mic2Size);

    dumpBuffer += mic2Size;
    if (dumpBufferHeader->amp_flag == 0)
    {
        /*
         *  magic digits of A5A5A5... identifies that speaker does not play back.
         *  This filling operation is not necessary. On the PC tool we also can check
         *  louder speaker's status by reading the audio stream header metrics.
         *  With these fillings, it's convenient for us to listen to the generated wav file on PC.
         */
        memset(dumpBuffer, 0xA5, ampSize);
    }
    else
    {
        memcpy(dumpBuffer, amp, ampSize);
    }

    dumpBuffer += ampSize;
    memcpy(dumpBuffer, clean, cleanSize);

    s_dumpBufferProduceSeq++;
}

static void _preMarkcleanStreamSkipped(uint32_t seq)
{
    uint8_t dumpBufferIdxS = 0;
    stream_header_t *dumpBufferHeader;

    if (seq == 0)
    {
        return; /* DumpBuf is not ready*/
    }

    dumpBufferIdxS                   = (seq - 1) % AUDIO_DUMP_SLOTS_CNT;
    dumpBufferHeader                 = (stream_header_t *)s_dumpOutPool[dumpBufferIdxS];
    dumpBufferHeader->clean_consumer = ASR_SKIPPED;
}

static void _preMarkcleanStreamConsumed(clean_consumer_t type)
{
    uint8_t dumpBufferIdx = 0;
    stream_header_t *dumpBufferHeader;

    if (s_dumpBufferProduceSeq == 0)
    {
        return; /* DumpBuf is not ready*/
    }

    dumpBufferIdx                    = (s_dumpBufferProduceSeq - 1) % AUDIO_DUMP_SLOTS_CNT;
    dumpBufferHeader                 = (stream_header_t *)s_dumpOutPool[dumpBufferIdx];
    dumpBufferHeader->clean_consumer = type;
}

/* Below Three APIs are called in ASR task content */
void postMarkCleanStreamSKIPPED(uint32_t *seq, uint8_t cnt)
{
    uint8_t i, dumpBufferIdx = 0;
    stream_header_t *dumpBufferHeader;

    for (i = 0; i < cnt; i++)
    {
        if (seq[i] == 0)
        {
            continue; /* DumpBuf is not ready*/
        }
        else
        {
            dumpBufferIdx    = (seq[i] - 1) % AUDIO_DUMP_SLOTS_CNT;
            dumpBufferHeader = (stream_header_t *)s_dumpOutPool[dumpBufferIdx];
            assert((seq[i] - 1) == dumpBufferHeader->frame_seq);
            dumpBufferHeader->clean_consumer = ASR_SKIPPED;
        }
    }
}

void postMarkCleanStreamConsumedByWW(uint32_t *seq, uint8_t cnt)
{
    uint8_t i, dumpBufferIdx = 0;
    stream_header_t *dumpBufferHeader;

    for (i = 0; i < cnt; i++)
    {
        if (seq[i] == 0)
        {
            continue; /* DumpBuf is not ready*/
        }
        else
        {
            dumpBufferIdx    = (seq[i] - 1) % AUDIO_DUMP_SLOTS_CNT;
            dumpBufferHeader = (stream_header_t *)s_dumpOutPool[dumpBufferIdx];
            assert((seq[i] - 1) == dumpBufferHeader->frame_seq);
            if (dumpBufferHeader->clean_consumer == ASR_UNKNOWN)
            {
                dumpBufferHeader->clean_consumer = ASR_WW_MODEL;
            }
            else
            {
                assert(0);
            }
        }
    }
}

void postMarkCleanStreamConsumedByCMD(uint32_t *seq, uint8_t cnt)
{
    uint8_t i, dumpBufferIdx = 0;
    stream_header_t *dumpBufferHeader;

    for (i = 0; i < cnt; i++)
    {
        if (seq[i] == 0)
        {
            continue; /* DumpBuf is not ready*/
        }
        else
        {
            dumpBufferIdx    = (seq[i] - 1) % AUDIO_DUMP_SLOTS_CNT;
            dumpBufferHeader = (stream_header_t *)s_dumpOutPool[dumpBufferIdx];
            assert((seq[i] - 1) == dumpBufferHeader->frame_seq);
            if (dumpBufferHeader->clean_consumer == ASR_UNKNOWN)
            {
                dumpBufferHeader->clean_consumer = ASR_CMD_MODEL;
            }
            else
            {
                assert(0);
            }
        }
    }
}

static void _forwardAudioDumpBufferToOutput(const audio_processing_dev_t *dev)
{
    uint32_t i;
    stream_header_t *dumpBufferHeader;
    uint8_t dumpBufferIdx;

    for (i = s_dumpBufferConsumeSeq; i < s_dumpBufferProduceSeq; i++)
    {
        dumpBufferIdx    = i % AUDIO_DUMP_SLOTS_CNT;
        dumpBufferHeader = (stream_header_t *)s_dumpOutPool[dumpBufferIdx];

        assert(dumpBufferHeader->frame_seq == i);

        if (dumpBufferHeader->clean_consumer == ASR_SKIPPED || dumpBufferHeader->clean_consumer != ASR_UNKNOWN)
        {
            if (((i - s_dumpBufferConsumeSeq) & 0x03) ==
                (AUDIO_DUMP_CHUNKS_CNT - 1)) /* accumulated AUDIO_DUMP_CHUNKS_CNT frames*/
            {
                if (dev->cap.callback != NULL)
                {
                    audio_processing_event_t audio_processing = {0};

                    audio_processing.eventId   = kAudioProcessingEvent_Dump;
                    audio_processing.eventInfo = kEventInfo_Remote;
                    audio_processing.data      = s_dumpOutPool[s_dumpBufferConsumeSeq % AUDIO_DUMP_SLOTS_CNT];
                    audio_processing.size      = dumpBufferHeader->frame_size_in_byte * AUDIO_DUMP_CHUNKS_CNT;
                    audio_processing.copy      = 0;

                    dev->cap.callback(dev, audio_processing, 0);
                }

                s_dumpBufferConsumeSeq += AUDIO_DUMP_CHUNKS_CNT;
            }
        }
        else
        {
            break;
        }
    }
}
#endif /* ENABLE_OUTPUT_DEV_AudioDump */

hal_audio_processing_status_t audio_processing_afe_init(audio_processing_dev_t *dev,
                                                        audio_processing_dev_callback_t callback)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;

    sln_afe_status_t afeStatus = kAfeSuccess;
    sln_afe_config_t afeConfig = {0};

    dev->cap.callback = callback;

    afeConfig.numberOfMics    = AUDIO_PDM_MIC_COUNT;
    afeConfig.afeMemBlock     = s_afeExternalMemory;
    afeConfig.afeMemBlockSize = sizeof(s_afeExternalMemory);

    afeConfig.postProcessedGain = 18;
    afeConfig.wakeWordMaxLength = WAKE_WORD_MAX_LENGTH_MS;

#if !AMP_LOOPBACK_DISABLED
    afeConfig.aecEnabled      = 1;
    afeConfig.aecFilterLength = AEC_FILTER_LENGTH;
#else
    afeConfig.aecEnabled      = 0;
    afeConfig.aecFilterLength = 0;
#endif /* !AMP_LOOPBACK_DISABLED */

    afeConfig.micsPosition[0][0] = -30;
    afeConfig.micsPosition[0][1] = 0;
    afeConfig.micsPosition[0][2] = 0;
    afeConfig.micsPosition[1][0] = 30;
    afeConfig.micsPosition[1][1] = 0;
    afeConfig.micsPosition[1][2] = 0;

#if AFE_INPUT_MIC_SAMPLE_BYTES == 2
    afeConfig.dataInType = kAfeTypeInt16;
#elif AFE_INPUT_MIC_SAMPLE_BYTES == 4
    afeConfig.dataInType      = kAfeTypeInt32;
#endif /* AFE_INPUT_MIC_SAMPLE_BYTES */

#if AFE_OUTPUT_SAMPLE_BYTES == 2
    afeConfig.dataOutType = kAfeTypeInt16;
#elif AFE_OUTPUT_SAMPLE_BYTES == 4
    afeConfig.dataOutType     = kAfeTypeInt32;
#endif /* AFE_OUTPUT_SAMPLE_BYTES */

    afeConfig.mallocFunc = FWK_MALLOC;
    afeConfig.freeFunc   = FWK_FREE;

    afeStatus = SLN_AFE_Init(&afeConfig);
    if (afeStatus != kAfeSuccess)
    {
        LOGE("[AFE] SLN_AFE_Init failed %d", afeStatus);
        error = kStatus_HAL_AudioProcessingError;
    }

    return error;
}

hal_audio_processing_status_t audio_processing_afe_deinit(const audio_processing_dev_t *dev)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    return error;
}

hal_audio_processing_status_t audio_processing_afe_start(const audio_processing_dev_t *dev)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    return error;
}

hal_audio_processing_status_t audio_processing_afe_stop(const audio_processing_dev_t *dev)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    return error;
}

hal_audio_processing_status_t audio_processing_afe_notify(const audio_processing_dev_t *dev, void *param)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    event_voice_t event                 = *(event_voice_t *)param;

    switch (event.event_base.eventId)
    {
        case ASR_TO_AFE_FEEDBACK:
            s_utteranceLength = event.asr_feedback.utterance_len;
            break;

#if !AMP_LOOPBACK_DISABLED
        case SPEAKER_TO_AFE_FEEDBACK:
            _addSpeakerFeedback(event.speaker_audio.audio_stream, event.speaker_audio.audio_length,
                                event.speaker_audio.start_time);
            break;
#endif /* !AMP_LOOPBACK_DISABLED */
        case SET_AQT_STATUS:
            s_AFE_ByPass = 0xFF & (uint32_t)event.data;
            break;

        default:
            break;
    }

    return error;
}

hal_audio_processing_status_t audio_processing_afe_run(const audio_processing_dev_t *dev, void *param)
{
    hal_audio_processing_status_t error = kStatus_HAL_AudioProcessingSuccess;
    event_voice_t event                 = *(event_voice_t *)param;

    if (event.event_base.eventId == AUDIO_IN)
    {
        sln_afe_status_t afeStatus = kAfeSuccess;

        if (s_AFE_ByPass)
        {
            return error;
        }

        void *afeMicIn    = s_afeMicIn;
        int16_t *afeAmpIn = NULL;
        void *afeCleanOut = NULL;

        /* Convert PCM stream buffer to LLRR format for AFE/ASR */
        _convertMicDataForAfe(event.audio_in.audio_stream, afeMicIn);

#if !AMP_LOOPBACK_DISABLED
        afeAmpIn = _getSpeakerFeedback();
#endif /* !AMP_LOOPBACK_DISABLED */

        /* Run mic streams through AFE */
        afeStatus = SLN_AFE_Process_Audio(afeMicIn, afeAmpIn, &afeCleanOut);
        if (afeStatus != kAfeSuccess)
        {
            LOGE("[AFE] SLN_AFE_Process_Audio failed %d", afeStatus);
            error = kStatus_HAL_AudioProcessingError;
        }

#ifdef ENABLE_OUTPUT_DEV_AudioDump
        static uint32_t s_skipFirstFrames = 0;
        if (s_skipFirstFrames >= AUDIO_DUMP_SKIP_FIRST_FRAMES_CNT)
        {
            _packDataToAudioDumpBuf((void *)&afeMicIn[0], (AFE_INPUT_MIC_BUFFER_SIZE / AUDIO_PDM_MIC_COUNT),
                                    (void *)&afeMicIn[(AFE_INPUT_MIC_BUFFER_SIZE / AUDIO_PDM_MIC_COUNT)],
                                    (AFE_INPUT_MIC_BUFFER_SIZE / AUDIO_PDM_MIC_COUNT), (void *)afeAmpIn,
                                    AFE_INPUT_AMP_BUFFER_SIZE, (void *)afeCleanOut, AFE_OUTPUT_BUFFER_SIZE);
            _forwardAudioDumpBufferToOutput(dev);
            assert(s_dumpBufferProduceSeq >= s_dumpBufferConsumeSeq);

            if (s_dumpBufferProduceSeq - s_dumpBufferConsumeSeq > AUDIO_DUMP_SLOTS_CNT / 2)
            {
                LOGD("[DUMP] DELTA between P and C %d\r\n", s_dumpBufferProduceSeq - s_dumpBufferConsumeSeq);
            }
        }
        else
        {
            s_skipFirstFrames++;
        }

#endif /* ENABLE_OUTPUT_DEV_AudioDump */

        /* Pass output of AFE to ASR.
         * Use asrSamplesStored to align with detected wake words. */
        uint8_t asrSamplesStored = _forwardDataToAsr(dev, afeCleanOut, afeAmpIn);

        if (s_utteranceLength != 0)
        {
            uint32_t wakeWordLengthMs = s_utteranceLength / (AUDIO_PCM_SINGLE_CH_SMPL_COUNT / 10);

            LOGD("[AFE] Wake Word Length = %d [ms], %d [samples] buffered: %d [samples]", wakeWordLengthMs, s_utteranceLength,
                 asrSamplesStored * AUDIO_PCM_SINGLE_CH_SMPL_COUNT);

            afeStatus = SLN_AFE_Trigger_Found(s_utteranceLength);
            if (afeStatus != kAfeSuccess)
            {
                LOGE("[AFE] SLN_AFE_Trigger_Found failed %d", afeStatus);
                error = kStatus_HAL_AudioProcessingError;
            }
            s_utteranceLength = 0;
        }
    }

    return error;
}

const static audio_processing_dev_operator_t audio_processing_afe_ops = {
    .init        = audio_processing_afe_init,
    .deinit      = audio_processing_afe_deinit,
    .start       = audio_processing_afe_start,
    .stop        = audio_processing_afe_stop,
    .run         = audio_processing_afe_run,
    .inputNotify = audio_processing_afe_notify,
};

static audio_processing_dev_t audio_processing_afe = {
    .id = 1, .name = "AFE", .ops = &audio_processing_afe_ops, .cap = {.callback = NULL}};

int HAL_AudioProcessingDev_Afe_Register()
{
    int error = 0;
    LOGD("HAL_AudioProcessingDev_Afe_Register");
    error = FWK_AudioProcessing_DeviceRegister(&audio_processing_afe);
    return error;
}
#endif /* ENABLE_AUDIO_PROCESSING_DEV_Afe */
