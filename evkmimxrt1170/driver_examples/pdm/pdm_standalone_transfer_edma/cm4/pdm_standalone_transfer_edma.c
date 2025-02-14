/*
 * Copyright (c) 2022, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_pdm.h"
#include "fsl_debug_console.h"
#include "fsl_pdm_edma.h"
#include "fsl_dmamux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PDM                      PDM
#define DEMO_PDM_ERROR_IRQn           PDM_ERROR_IRQn
#define DEMO_PDM_ERROR_IRQHandler     PDM_ERROR_IRQHandler
#define DEMO_PDM_CLK_FREQ             24576000
#define DEMO_PDM_FIFO_WATERMARK       (4)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_SAMPLE_CLOCK_RATE    (2048000U) /* 2.048MHZ */
#define DEMO_EDMA                     DMA1
#define DEMO_DMAMUX                   DMAMUX1
#define DEMO_EDMA_CHANNEL             0
#define DEMO_PDM_REQUEST_SOURCE       kDmaRequestMuxPdm
#define DEMO_AUDIO_SAMPLE_RATE        16000

#define DEMO_DMIC_NUM             4U
#define DEMO_PDM_ENABLE_CHANNEL_0 0
#define DEMO_PDM_ENABLE_CHANNEL_1 1
#define DEMO_PDM_ENABLE_CHANNEL_2 2
#define DEMO_PDM_ENABLE_CHANNEL_3 3
#define DEMO_PDM_ENABLE_CHANNEL_4 4
#define DEMO_PDM_ENABLE_CHANNEL_5 5
#define DEMO_PDM_ENABLE_CHANNEL_6 6
#define DEMO_PDM_ENABLE_CHANNEL_7 7
#define CHANNEL_SAMPLE (128U)
#define BUFFER_SAMPLE  (CHANNEL_SAMPLE * 4U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitDebugConsole(void);
static void pdmEdmallback(PDM_Type *base, pdm_edma_handle_t *handle, status_t status, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(pdm_edma_handle_t pdmRxHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(edma_handle_t dmaHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_buffer[2][BUFFER_SAMPLE], 4);
AT_QUICKACCESS_SECTION_DATA_ALIGN(edma_tcd_t s_edmaTcd[2], 32U);
#if (defined(FSL_FEATURE_PDM_HAS_STATUS_LOW_FREQ) && (FSL_FEATURE_PDM_HAS_STATUS_LOW_FREQ == 1U))
static volatile bool s_lowFreqFlag = false;
#endif
static volatile bool s_fifoErrorFlag = false;
static volatile bool s_pdmRxFinished = false;

static const pdm_config_t pdmConfig = {
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
#ifdef DEMO_PDM_CHANNEL_GAIN
    .gain = DEMO_PDM_CHANNEL_GAIN,
#else
    .gain       = kPDM_DfOutputGain4,
#endif
};

pdm_edma_transfer_t pdmXfer[2] = {
    {
        .data         = (volatile uint8_t *)s_buffer[0],
        .dataSize     = BUFFER_SAMPLE * 4U,
        .linkTransfer = &pdmXfer[1],
    },

    {
        .data         = (volatile uint8_t *)s_buffer[1U],
        .dataSize     = BUFFER_SAMPLE * 4U,
        .linkTransfer = &pdmXfer[0],
    },
};
static volatile uint32_t s_pdmReceiveCount = 0U;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM) / (2^POST)
 *                              = 24 * (32 + 77/100)  / 2
 *                              = 393.24MHZ
 */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 0, 1, 2, 3, 4, 5 */
    .numerator   = 77,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 100, /* 30 bit denominator of fractional loop divider */
};

static void pdmEdmallback(PDM_Type *base, pdm_edma_handle_t *handle, status_t status, void *userData)
{
    s_pdmReceiveCount++;
    if (s_pdmReceiveCount == 200U)
    {
        PDM_TransferTerminateReceiveEDMA(DEMO_PDM, &pdmRxHandle);
        s_pdmRxFinished = true;
    }
}

void DEMO_PDM_ERROR_IRQHandler(void)
{
    uint32_t status = 0U;

#if (defined(FSL_FEATURE_PDM_HAS_STATUS_LOW_FREQ) && (FSL_FEATURE_PDM_HAS_STATUS_LOW_FREQ == 1U))
    if (PDM_GetStatus(DEMO_PDM) & PDM_STAT_LOWFREQF_MASK)
    {
        PDM_ClearStatus(DEMO_PDM, PDM_STAT_LOWFREQF_MASK);
        s_lowFreqFlag = true;
    }
#endif
    status = PDM_GetFifoStatus(DEMO_PDM);
    if (status)
    {
        PDM_ClearFIFOStatus(DEMO_PDM, status);
        s_fifoErrorFlag = true;
    }

#if defined(FSL_FEATURE_PDM_HAS_RANGE_CTRL) && FSL_FEATURE_PDM_HAS_RANGE_CTRL
    status = PDM_GetRangeStatus(DEMO_PDM);
    if (status)
    {
        PDM_ClearRangeStatus(DEMO_PDM, status);
    }
#else
    status = PDM_GetOutputStatus(DEMO_PDM);
    if (status)
    {
        PDM_ClearOutputStatus(DEMO_PDM, status);
    }
#endif
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i = 0U, j = 0U, m = 0U;
    edma_config_t dmaConfig = {0};
    uint32_t *pdmBufferAddr = NULL;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClockMux(kCLOCK_Root_Bus_Lpsr, 0);
    CLOCK_InitAudioPll(&audioPllConfig);

    /* mic root clock = 24.576M */
    CLOCK_SetRootClockMux(kCLOCK_Root_Mic, 6);
    CLOCK_SetRootClockDiv(kCLOCK_Root_Mic, 16);

    DMAMUX_Init(DEMO_DMAMUX);
    DMAMUX_SetSource(DEMO_DMAMUX, DEMO_EDMA_CHANNEL, DEMO_PDM_REQUEST_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, DEMO_EDMA_CHANNEL);

    PRINTF("PDM edma multi channel standalone transfer example started!\n\r");

    memset(s_buffer, 0, sizeof(s_buffer));

    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_EDMA, &dmaConfig);
    EDMA_CreateHandle(&dmaHandle, DEMO_EDMA, DEMO_EDMA_CHANNEL);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(DEMO_EDMA, DEMO_EDMA_CHANNEL, DEMO_PDM_EDMA_CHANNEL);
#endif
    /* Set up pdm */
    PDM_Init(DEMO_PDM, &pdmConfig);
    PDM_TransferCreateHandleEDMA(DEMO_PDM, &pdmRxHandle, pdmEdmallback, NULL, &dmaHandle);
    PDM_TransferSetMultiChannelInterleaveType(&pdmRxHandle, kPDM_EDMAMultiChannelInterleavePerChannelBlock);
    PDM_TransferInstallEDMATCDMemory(&pdmRxHandle, s_edmaTcd, 2);

    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &pdmRxHandle, DEMO_PDM_ENABLE_CHANNEL_3, &channelConfig);
    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &pdmRxHandle, DEMO_PDM_ENABLE_CHANNEL_4, &channelConfig);
    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &pdmRxHandle, DEMO_PDM_ENABLE_CHANNEL_5, &channelConfig);
    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &pdmRxHandle, DEMO_PDM_ENABLE_CHANNEL_6, &channelConfig);

    if (PDM_SetSampleRateConfig(DEMO_PDM, DEMO_PDM_CLK_FREQ, DEMO_AUDIO_SAMPLE_RATE) != kStatus_Success)
    {
        PRINTF("PDM configure sample rate failed.\r\n");
        return -1;
    }
    PDM_Reset(DEMO_PDM);
    PDM_EnableInterrupts(DEMO_PDM, kPDM_ErrorInterruptEnable);
    EnableIRQ(DEMO_PDM_ERROR_IRQn);

    PDM_TransferReceiveEDMA(DEMO_PDM, &pdmRxHandle, pdmXfer);
    /* wait data read finish */
    while (!s_pdmRxFinished)
    {
    }

    PRINTF("PDM recieved channel data:\n\r");
    for (i = 0U; i < 2U; i++)
    {
        pdmBufferAddr = s_buffer[i];
        for (j = 0U; j < 4U; j++)
        {
            PRINTF("PDM channel %d\r\n", j + 3);
            for (m = 0U; m < CHANNEL_SAMPLE; m++)
            {
                PRINTF("%x ", pdmBufferAddr[m + j * 128U]);
            }
            PRINTF("\r\n");
        }
    }

    PDM_Deinit(DEMO_PDM);

    PRINTF("PDM edma multi channel standalone transfer example finished!\n\r");
    while (1)
    {
    }
}
