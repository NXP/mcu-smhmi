/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board_define.h"
#ifdef ENABLE_INPUT_DEV_PdmMic
#include "fsl_pdm.h"
#include "fsl_pdm_edma.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"

#include "fwk_log.h"
#include "fwk_input_manager.h"
#include "hal_event_descriptor_voice.h"
#include "hal_input_dev.h"

#include "hal_audio_defs.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MIC_PDM                      PDM
#define MIC_PDM_CLK_FREQ             8192500 /* 8.192MHz */
#define MIC_PDM_FIFO_WATERMARK       (4)
#define MIC_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define MIC_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define MIC_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define MIC_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define MIC_PDM_SAMPLE_CLOCK_RATE    (2048000U) /* 2.048MHz */

#define MIC_EDMA               DMA0
#define MIC_DMAMUX             DMAMUX0
#define MIC_PDM_EDMA_CHANNEL   1
#define MIC_PDM_REQUEST_SOURCE kDmaRequestMuxPdm

#define MIC_PDM_DMA_IRQ_PRIO (configMAX_SYSCALL_INTERRUPT_PRIORITY - 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void pdmCallback(PDM_Type *base, pdm_edma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN_DTC(static pdm_edma_handle_t s_pdmRxHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN_DTC(static edma_handle_t s_pdmDmaHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN_DTC(static edma_tcd_t s_edmaTcd_0[2], 32U);
AT_NONCACHEABLE_SECTION_ALIGN_DTC(static pcm_input_t s_pcmStream, 4);

pdm_edma_transfer_t pdmXfer[2] = {
    {
        .data         = (uint8_t *)s_pcmStream,
        .dataSize     = AUDIO_PCM_BUFFER_SIZE,
        .linkTransfer = &pdmXfer[1],
    },
    {
        .data         = (uint8_t *)&s_pcmStream[AUDIO_PCM_BUFFER_COUNT - 1],
        .dataSize     = AUDIO_PCM_BUFFER_SIZE,
        .linkTransfer = &pdmXfer[0]
    },
};

static const pdm_config_t pdmConfig = {
    .enableDoze        = false,
    .fifoWatermark     = MIC_PDM_FIFO_WATERMARK,
    .qualityMode       = MIC_PDM_QUALITY_MODE,
    .cicOverSampleRate = MIC_PDM_CIC_OVERSAMPLE_RATE,
};
static const pdm_channel_config_t channelConfig = {
    .cutOffFreq = kPDM_DcRemoverCutOff152Hz,
    .gain       = kPDM_DfOutputGain2,
};

static event_voice_t event;

/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM) / (2^POST)
 *                              = 24 * (32 + 77/100)  / 2
 *                              = 393.24MHZ
 */
static const clock_audio_pll_config_t micAudioPllConfig = {
    .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 0, 1, 2, 3, 4, 5 */
    .numerator   = 77,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 100, /* 30 bit denominator of fractional loop divider */
};

void PDM_ERROR_IRQHandler(void)
{
    uint32_t fifoStatus = 0U;
    if (PDM_GetStatus(MIC_PDM) & PDM_STAT_LOWFREQF_MASK)
    {
        PDM_ClearStatus(MIC_PDM, PDM_STAT_LOWFREQF_MASK);
    }

    fifoStatus = PDM_GetFifoStatus(MIC_PDM);
    if (fifoStatus)
    {
        PDM_ClearFIFOStatus(MIC_PDM, fifoStatus);
    }
    __DSB();
}

hal_input_status_t input_dev_pdm_mic_init(input_dev_t *dev, input_dev_callback_t callback)
{
    clock_root_config_t root_clock_config = {0};
    dev->cap.callback                     = callback;

    /* 393.24MHz */
    CLOCK_InitAudioPll(&micAudioPllConfig);
    /* AudioPll */
    /* 8.1925m mic root clock */
    root_clock_config.mux = kCLOCK_MIC_ClockRoot_MuxAudioPllOut;
    root_clock_config.div = 48;

    CLOCK_SetRootClock(kCLOCK_Root_Mic, &root_clock_config);

    DMAMUX_Init(MIC_DMAMUX);
    DMAMUX_SetSource(MIC_DMAMUX, MIC_PDM_EDMA_CHANNEL, MIC_PDM_REQUEST_SOURCE);
    DMAMUX_EnableChannel(MIC_DMAMUX, MIC_PDM_EDMA_CHANNEL);

    NVIC_SetPriority(DMA1_DMA17_IRQn, MIC_PDM_DMA_IRQ_PRIO);

    /* Create EDMA handle */
    BOARD_InitEDMA(MIC_EDMA);
    EDMA_CreateHandle(&s_pdmDmaHandle, MIC_EDMA, MIC_PDM_EDMA_CHANNEL);

    /* Setup PDM */
    PDM_Init(MIC_PDM, &pdmConfig);

    PDM_TransferCreateHandleEDMA(MIC_PDM, &s_pdmRxHandle, pdmCallback, NULL, &s_pdmDmaHandle);
    PDM_TransferInstallEDMATCDMemory(&s_pdmRxHandle, s_edmaTcd_0, 2);
    PDM_TransferSetChannelConfigEDMA(MIC_PDM, &s_pdmRxHandle, MIC_PDM_ENABLE_CHANNEL_LEFT, &channelConfig);
    PDM_TransferSetChannelConfigEDMA(MIC_PDM, &s_pdmRxHandle, MIC_PDM_ENABLE_CHANNEL_RIGHT, &channelConfig);
    if (PDM_SetSampleRateConfig(MIC_PDM, MIC_PDM_CLK_FREQ, AUDIO_PCM_SAMPLE_RATE) != kStatus_Success)
    {
        LOGE("PDM configure sample rate failed.\r\n");
        return kStatus_HAL_InputError;
    }

    return kStatus_HAL_InputSuccess;
}

hal_input_status_t input_dev_pdm_mic_deinit(const input_dev_t *dev)
{
    hal_input_status_t error = 0;
    return error;
}

hal_input_status_t input_dev_pdm_mic_start(const input_dev_t *dev)
{
    hal_input_status_t error = 0;

    PDM_Reset(MIC_PDM);

    PDM_TransferReceiveEDMA(MIC_PDM, &s_pdmRxHandle, pdmXfer);

    return error;
}

hal_input_status_t input_dev_pdm_mic_stop(const input_dev_t *dev)
{
    hal_input_status_t error = 0;

    PDM_TransferTerminateReceiveEDMA(MIC_PDM, &s_pdmRxHandle);

    CLOCK_DisableClock(kCLOCK_Pdm);

    return error;
}

hal_input_status_t input_dev_pdm_mic_notify(const input_dev_t *dev, void *param)
{
    hal_input_status_t error = kStatus_HAL_InputSuccess;
    return error;
}

const static input_dev_operator_t input_dev_pdm_mic_ops = {
    .init        = input_dev_pdm_mic_init,
    .deinit      = input_dev_pdm_mic_deinit,
    .start       = input_dev_pdm_mic_start,
    .stop        = input_dev_pdm_mic_stop,
    .inputNotify = input_dev_pdm_mic_notify,
};

static input_dev_t input_dev_pdm_mic = {.id = 1, .ops = &input_dev_pdm_mic_ops, .cap = {.callback = NULL}};
static input_event_t input_dev_event;

static void pdmCallback(PDM_Type *base, pdm_edma_handle_t *handle, status_t status, void *userData)
{
    static volatile uint8_t pingPongIdx = 0;

    /* Callback sends message to input_manager to forward to audio */
    if (input_dev_pdm_mic.cap.callback != NULL)
    {
        uint8_t fromISR             = __get_IPSR();
        event.event_base.eventId    = AUDIO_IN;
        event.audio_in.audio_stream = (int32_t *)&s_pcmStream[pingPongIdx];
        input_dev_event.u.audioData = &event;
        input_dev_event.eventId     = kInputEventID_AudioRecv;

        input_dev_pdm_mic.cap.callback(&input_dev_pdm_mic, &input_dev_event, fromISR);
    }
    pingPongIdx = pingPongIdx ? 0 : 1;
}

int HAL_InputDev_PdmMic_Register()
{
    int error = 0;
    LOGD("HAL_InputDev_PdmMic_Register");
    error = FWK_InputManager_DeviceRegister(&input_dev_pdm_mic);
    return error;
}
#endif /* ENABLE_INPUT_DEV_PdmMic */
