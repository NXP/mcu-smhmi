/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#define BLOCK_SIZE 8
typedef struct stream_header
{
    /*
     * 0: louder speaker is not playing. 1: louder speaker is playing
     */
    unsigned char amp_flag;
    /*
     *  UNKNOW: not consumed by asr yet. ASR_WW_MODEL: consumed by WW model. ASR_CMD_MODEL: consumed by CMD model.
     */
    unsigned char clean_consumer;
    unsigned short frame_size_in_byte;
    unsigned int frame_seq;
} stream_header_t __attribute__((aligned(4)));

typedef enum clean_consumer
{
    ASR_UNKNOWN,
    ASR_SKIPPED,
    ASR_WW_MODEL,
    ASR_CMD_MODEL
} clean_consumer_t;

#define ASR_SKIPPED_FILLS   0x00
#define ASR_WW_MODEL_FILLS  0x22
#define ASR_CMD_MODEL_FILLS 0x7F

typedef struct
{
    //RIFF-Chunk
    char chunk_id[4]; // 4 bytes, RIFF flag
    int chunk_size;   // 4 bytes, total chunk size
    char format[4];   // 4 bytes, WAVE flag

    // Format-Chunk 16 bytes
    char subchunk1_id[4];  // 4 bytes, fmt flag
    int subchunk1_size;    // 4 bytes, subchunk size
    short audio_format;    // 2 bytes
    short num_channels;    // 2 bytes, channel
    int sample_rate;       // 4 bytes, sample rate
    int byte_rate;         // 4 bytes, bit rate
    short block_align;     // 2 bytes
    short bits_per_sample; // 2 bytes

    // char  chDATA[4];       // ＂data ＂ optional
    // int   dwDATALen;       // audio data length

    // (Fact-Chunk) 

    // (Data-Chunk) 
    char subchunk2_id[4]; // 4 bytes, data flag
    int subchunk2_size;   // subchunk size

} wavfile_header_t;

#define SUBCHUNK1_SIZE  (16)
#define AUDIO_FORMAT    (1) /* PCM */
#define SUBCHUNK2_SIZE  (0)
#define CHUNK_SIZE      (4 + (8 + SUBCHUNK1_SIZE) + (8 + SUBCHUNK2_SIZE))
#define SAMPLE_RATE     (16000)
#define BITS_PER_SAMPLE (16)
#define NUM_CHANNELS    (1)

#define BYTE_RATE   (SAMPLE_RATE * NUM_CHANNELS * BITS_PER_SAMPLE / 8)
#define BLOCK_ALIGN (NUM_CHANNELS * BITS_PER_SAMPLE / 8)

#define BLOCK_AUDIO_LEN_IN_BYTE (160 * 2)

static unsigned char wav_buf[BLOCK_AUDIO_LEN_IN_BYTE];

static wavfile_header_t wav_header_default = {
    .chunk_id   = {'R', 'I', 'F', 'F'},
    .chunk_size = CHUNK_SIZE,
    .format     = {'W', 'A', 'V', 'E'},

    .subchunk1_id    = {'f', 'm', 't', ' '},
    .subchunk1_size  = SUBCHUNK1_SIZE,
    .audio_format    = AUDIO_FORMAT,
    .num_channels    = NUM_CHANNELS,
    .sample_rate     = SAMPLE_RATE,
    .byte_rate       = BYTE_RATE,
    .block_align     = BLOCK_ALIGN,
    .bits_per_sample = BITS_PER_SAMPLE,

    .subchunk2_id   = {'d', 'a', 't', 'a'},
    .subchunk2_size = SUBCHUNK2_SIZE,
};

int main(int argc, char *argv[])
{
    FILE *inputFile   = NULL;
    int print_verbose = 0;
    int file_s, file_e;
    char block_content[BLOCK_SIZE];
    if (argc < 2)
    {
        printf("Usage:\r\n ./parse_audio_runtime_info {file}\r\n");
        return -1;
    }
    if (argc == 3)
    {
        if (strncmp(argv[2], "-v", 2) == 0)
        {
            print_verbose = 1;
        }
    }

    inputFile = fopen(argv[1], "rb");

    fseek(inputFile, 0, SEEK_END);
    file_e = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);
    file_s = ftell(inputFile);

    int filesize  = file_e - file_s;
    int frame_cnt = (file_e - file_s) / BLOCK_SIZE;

    printf("File size: %d, Frame count: %d, sample count:%d\r\n", filesize, frame_cnt, frame_cnt * 160);

    if (sizeof(stream_header_t) != BLOCK_SIZE)
    {
        printf("block size: %d\r\n", sizeof(stream_header_t));
        stream_header_t *test = 0;
        printf("[offset]:\r\n amp_flag: %p, clean_consumer:%p, frame_size_in_byte:%p,frame_seq:%p\r\n", &test->amp_flag,
               &test->clean_consumer, &test->frame_size_in_byte, &test->frame_seq);
    }

    int ret;
    unsigned long first_frame_seq = 0, last_frame_seq = 0;

    ret = fread(block_content, 1, BLOCK_SIZE, inputFile);
    if (ret == BLOCK_SIZE)
    {
        stream_header_t *p = (stream_header_t *)block_content;
        printf("First frame information:\r\n");
        printf("amp_flag: %d, clean_consumer:%d, frame_seq: %d\r\n", p->amp_flag, p->clean_consumer, p->frame_seq);
        first_frame_seq = p->frame_seq;
    }

    fseek(inputFile, -BLOCK_SIZE, SEEK_END);
    ret = fread(block_content, 1, BLOCK_SIZE, inputFile);
    if (ret == BLOCK_SIZE)
    {
        stream_header_t *p = (stream_header_t *)block_content;
        printf("Last frame information:\r\n");
        printf("amp_flag: %d, clean_consumer:%d, frame_seq: %d\r\n", p->amp_flag, p->clean_consumer, p->frame_seq);
        last_frame_seq = p->frame_seq;
    }

    if ((last_frame_seq - first_frame_seq) != (frame_cnt - 1))
    {
        printf("%d frames must be lost!!!\r\n", (last_frame_seq - first_frame_seq) - (frame_cnt - 1));
        printf("Please execute command with '-v' parameter to check more information.\r\n");
    }

    if (print_verbose)
    {
        printf("print all frame_seq \r\n");
        fseek(inputFile, 0, SEEK_SET);
        ret                  = fread(block_content, 1, BLOCK_SIZE, inputFile);
        unsigned int pre_seq = 0;

        FILE *fp = fopen("ww_cmd_indicator.wav", "wb+");
        if (fp == NULL)
        {
            printf("Generating indicator file failed %p\r\n", fp);
            return -1;
        }
        wav_header_default.subchunk2_size = frame_cnt * 160 * 2;
        wav_header_default.chunk_size =
            (4 + (8 + wav_header_default.subchunk1_size) + (8 + wav_header_default.subchunk2_size));
        fwrite(&wav_header_default, sizeof(wavfile_header_t), 1, fp);

        while (ret == BLOCK_SIZE)
        {
            stream_header_t *p = (stream_header_t *)block_content;
            printf("[%d]:", p->frame_seq);
            printf("amp_flag: %d, clean_consumer:%d, file_offset: %d\r\n", p->amp_flag, p->clean_consumer,
                   ftell(inputFile));
            if (pre_seq == 0)
            {
                pre_seq = p->frame_seq;
            }
            else if ((p->frame_seq - pre_seq) != 1)
            {
                printf("Warning:\r\n");
                printf("%d frames lost between above two frames\r\n", p->frame_seq - pre_seq - 1);
            }
            switch (p->clean_consumer)
            {
                case ASR_SKIPPED:
                    memset(wav_buf, ASR_SKIPPED_FILLS, BLOCK_AUDIO_LEN_IN_BYTE);
                    break;
                case ASR_WW_MODEL:
                    memset(wav_buf, ASR_WW_MODEL_FILLS, BLOCK_AUDIO_LEN_IN_BYTE);
                    break;
                case ASR_CMD_MODEL:
                    memset(wav_buf, ASR_CMD_MODEL_FILLS, BLOCK_AUDIO_LEN_IN_BYTE);
                    break;
                default:
                    printf("Invalid consumer type:  %d\r\n", p->clean_consumer);
                    return -1;
                    break;
            }
            fwrite(wav_buf, BLOCK_AUDIO_LEN_IN_BYTE, 1, fp);

            pre_seq = p->frame_seq;
            ret     = fread(block_content, 1, BLOCK_SIZE, inputFile);
        }

        if (feof(inputFile))
        {
            printf("Reach the file end\r\n");
        }
        else if (ferror(inputFile))
        {
            printf("[ERROR]:fread file\r\n");
        }
        fclose(inputFile);
        fclose(fp);
    }

    return 0;
}
