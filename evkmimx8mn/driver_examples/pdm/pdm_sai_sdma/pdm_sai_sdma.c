/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_pdm.h"
#include "fsl_pdm_sdma.h"
#include "fsl_sdma.h"
#include "fsl_sai.h"
#include "fsl_sai_sdma.h"
#include "fsl_debug_console.h"
#include "fsl_sdma_script.h"
#include "fsl_codec_common.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_wm8524.h"
#include "fsl_codec_adapter.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PDM_DMA                  SDMAARM3
#define DEMO_SAI_DMA                  SDMAARM3
#define DEMO_PDM                      PDM
#define DEMO_PDM_CLK_FREQ             (24576000U)
#define DEMO_PDM_FIFO_WATERMARK       (4U)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_SAMPLE_CLOCK_RATE    (1536000U * 4) /* 6.144MHZ */
#define DEMO_PDM_DMA_REQUEST_SOURCE   (24U)
#define DEMO_PDM_DMA_CHANNEL          (2U)
#define DEMO_PDM_DMA_CHANNEL_PRIORITY (4U)
#define DEMO_PDM_HWVAD_SIGNAL_GAIN    0

#define DEMO_SAI                       (I2S3)
#define DEMO_SAI_CLK_FREQ              (24576000U)
#define DEMO_SAI_DMA_CHANNEL           (1)
#define DEMO_SAI_DMA_CHANNEL_PRIORITY  (3U)
#define DEMO_SAI_TX_DMA_REQUEST_SOURCE (5)
#define DEMO_SAI_CLOCK_SOURCE          (kSAI_BclkSourceMclkDiv)
#define DEMO_SAI_IRQn                  I2S3_IRQn

#define DEMO_CODEC_WM8524       (1)
#define DEMO_CODEC_BUS_PIN      (NULL)
#define DEMO_CODEC_BUS_PIN_NUM  (0)
#define DEMO_CODEC_MUTE_PIN     (GPIO5)
#define DEMO_CODEC_MUTE_PIN_NUM (21)

#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate48KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define BUFFER_SIZE   (1024)
#define BUFFER_NUMBER (4)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_WM8524_Mute_GPIO(uint32_t output);
void BOARD_MasterClockConfig(void);
static void pdmSdmallback(PDM_Type *base, pdm_sdma_handle_t *handle, status_t status, void *userData);
static void saiCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static wm8524_config_t wm8524Config = {
    .setMute     = BOARD_WM8524_Mute_GPIO,
    .setProtocol = NULL,
    .protocol    = kWM8524_ProtocolI2S,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8524, .codecDevConfig = &wm8524Config};
sai_master_clock_t mclkConfig;
AT_NONCACHEABLE_SECTION_ALIGN(pdm_sdma_handle_t s_pdmRxHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_handle_t s_pdmDmaHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_handle_t s_saiDmaHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sai_sdma_handle_t s_saiTxHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t s_pdmSdmaContext, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t s_saiSdmaContext, 4);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_buffer[BUFFER_SIZE * BUFFER_NUMBER], 4);
static volatile uint32_t s_bufferValidBlock = BUFFER_NUMBER;
static volatile uint32_t s_readIndex        = 0U;
static volatile uint32_t s_writeIndex       = 0U;
static const pdm_config_t pdmConfig         = {
    .enableDoze        = false,
    .fifoWatermark     = DEMO_PDM_FIFO_WATERMARK,
    .qualityMode       = DEMO_PDM_QUALITY_MODE,
    .cicOverSampleRate = DEMO_PDM_CIC_OVERSAMPLE_RATE,
};
static const pdm_channel_config_t channelConfig = {
#if (defined(FSL_FEATURE_PDM_HAS_DC_OUT_CTRL) && (FSL_FEATURE_PDM_HAS_DC_OUT_CTRL))
    .outputCutOffFreq = kPDM_DcRemoverCutOff40Hz,
#else
    .cutOffFreq = kPDM_DcRemoverCutOff152Hz,
#endif
    .gain = kPDM_DfOutputGain4,
};

codec_handle_t codecHandle;

extern codec_config_t boardCodecConfig;
/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_WM8524_Mute_GPIO(uint32_t output)
{
    GPIO_PinWrite(DEMO_CODEC_MUTE_PIN, DEMO_CODEC_MUTE_PIN_NUM, output);
}

void BOARD_MasterClockConfig(void)
{
    mclkConfig.mclkOutputEnable = true, mclkConfig.mclkHz = DEMO_AUDIO_MASTER_CLOCK;
    mclkConfig.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;
    SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
}
static void pdmSdmallback(PDM_Type *base, pdm_sdma_handle_t *handle, status_t status, void *userData)
{
    s_bufferValidBlock--;
}

static void saiCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_TxError == status)
    {
        /* Handle the error. */
    }
    else
    {
        s_bufferValidBlock++;
    }
}

void PDM_ERROR_IRQHandler(void)
{
    uint32_t fifoStatus = 0U;
#if (defined(FSL_FEATURE_PDM_HAS_STATUS_LOW_FREQ) && (FSL_FEATURE_PDM_HAS_STATUS_LOW_FREQ == 1U))
    if (PDM_GetStatus(DEMO_PDM) & PDM_STAT_LOWFREQF_MASK)
    {
        PDM_ClearStatus(DEMO_PDM, PDM_STAT_LOWFREQF_MASK);
    }
#endif
    fifoStatus = PDM_GetFifoStatus(DEMO_PDM);
    if (fifoStatus)
    {
        PDM_ClearFIFOStatus(DEMO_PDM, fifoStatus);
    }
    SDK_ISR_EXIT_BARRIER;
}

void SAI_UserIRQHandler(void)
{
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    pdm_transfer_t pdmXfer;
    sdma_config_t dmaConfig = {0};
    sai_transfer_t saiXfer;
    sai_transceiver_t config;

    /* M7 has its local cache and enabled by default,
     * need to set smart subsystems (0x28000000 ~ 0x3FFFFFFF)
     * non-cacheable before accessing this address region */
    BOARD_InitMemory();

    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootMux(kCLOCK_RootPdm, kCLOCK_PdmRootmuxAudioPll1); /* Set PDM source to AUDIO PLL1 393215996HZ*/
    CLOCK_SetRootDivider(kCLOCK_RootPdm, 1U, 16U);                /* Set root clock to 393215996HZ / 16 = 24.575999M */

    CLOCK_SetRootMux(kCLOCK_RootSai3, kCLOCK_SaiRootmuxAudioPll1); /* Set SAI source to AUDIO PLL1 393215996HZ*/
    CLOCK_SetRootDivider(kCLOCK_RootSai3, 1U, 16U);                /* Set root clock to 393215996HZ / 16 = 24.575999M */

    /* gpio initialization */
    gpio_pin_config_t gpioConfig = {kGPIO_DigitalOutput, 1};
    GPIO_PinInit(DEMO_CODEC_MUTE_PIN, DEMO_CODEC_MUTE_PIN_NUM, &gpioConfig);

    PRINTF("PDM SAI sdma example started!\n\r");

    /* Create SDMA handle */
    SDMA_GetDefaultConfig(&dmaConfig);
    dmaConfig.ratio = kSDMA_ARMClockFreq;

    SDMA_Init(DEMO_PDM_DMA, &dmaConfig);
    SDMA_Init(DEMO_SAI_DMA, &dmaConfig);
    SDMA_CreateHandle(&s_pdmDmaHandle, DEMO_PDM_DMA, DEMO_PDM_DMA_CHANNEL, &s_pdmSdmaContext);
    SDMA_SetChannelPriority(DEMO_PDM_DMA, DEMO_PDM_DMA_CHANNEL, DEMO_PDM_DMA_CHANNEL_PRIORITY);

    SDMA_CreateHandle(&s_saiDmaHandle, DEMO_SAI_DMA, DEMO_SAI_DMA_CHANNEL, &s_saiSdmaContext);
    SDMA_SetChannelPriority(DEMO_SAI_DMA, DEMO_SAI_DMA_CHANNEL, DEMO_SAI_DMA_CHANNEL_PRIORITY);

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandleSDMA(DEMO_SAI, &s_saiTxHandle, saiCallback, NULL, &s_saiDmaHandle,
                                   DEMO_SAI_TX_DMA_REQUEST_SOURCE);
    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);

    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }

    config.bitClock.bclkSource = DEMO_SAI_CLOCK_SOURCE;
    SAI_TransferTxSetConfigSDMA(DEMO_SAI, &s_saiTxHandle, &config);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MasterClockConfig();

    /* Setup pdm */
    PDM_Init(DEMO_PDM, &pdmConfig);
    PDM_TransferCreateHandleSDMA(DEMO_PDM, &s_pdmRxHandle, pdmSdmallback, NULL, &s_pdmDmaHandle,
                                 DEMO_PDM_DMA_REQUEST_SOURCE);
    PDM_SetChannelConfigSDMA(DEMO_PDM, &s_pdmRxHandle, DEMO_PDM_ENABLE_CHANNEL_LEFT, &channelConfig);
    PDM_SetChannelConfigSDMA(DEMO_PDM, &s_pdmRxHandle, DEMO_PDM_ENABLE_CHANNEL_RIGHT, &channelConfig);
    PDM_SetSampleRate(DEMO_PDM, (1U << DEMO_PDM_ENABLE_CHANNEL_LEFT) | (1U << DEMO_PDM_ENABLE_CHANNEL_RIGHT),
                      pdmConfig.qualityMode, pdmConfig.cicOverSampleRate,
                      DEMO_PDM_CLK_FREQ / DEMO_PDM_SAMPLE_CLOCK_RATE);
    PDM_Reset(DEMO_PDM);

    while (1)
    {
        /* wait one buffer idle to recieve data */
        if (s_bufferValidBlock > 0)
        {
            pdmXfer.data     = (uint8_t *)((uint32_t)s_buffer + s_readIndex * BUFFER_SIZE);
            pdmXfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == PDM_TransferReceiveSDMA(DEMO_PDM, &s_pdmRxHandle, &pdmXfer))
            {
                s_readIndex++;
            }
            if (s_readIndex == BUFFER_NUMBER)
            {
                s_readIndex = 0U;
            }
        }
        /* wait one buffer busy to send data */
        if (s_bufferValidBlock < BUFFER_NUMBER)
        {
            saiXfer.data     = (uint8_t *)((uint32_t)s_buffer + s_writeIndex * BUFFER_SIZE);
            saiXfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferSendSDMA(DEMO_SAI, &s_saiTxHandle, &saiXfer))
            {
                s_writeIndex++;
            }
            if (s_writeIndex == BUFFER_NUMBER)
            {
                s_writeIndex = 0U;
            }
        }
    }
}
