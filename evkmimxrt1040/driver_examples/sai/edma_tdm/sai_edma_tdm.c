/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
#include "fsl_debug_console.h"
#include "fsl_sai_edma.h"
#include "fsl_sd.h"
#include "ff.h"
#include "diskio.h"
#include "ffconf.h"
#include "fsl_sd_disk.h"
#include "fsl_codec_common.h"
#include "sdmmc_config.h"
#include "fsl_common.h"
#include "fsl_cs42448.h"
#include "fsl_codec_adapter.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI */
#define DEMO_SAI          SAI1

/* IRQ */
#define DEMO_SAI_TX_IRQ SAI1_IRQn
#define DEMO_SAI_RX_IRQ SAI1_IRQn

/* DMA */
#define EXAMPLE_DMA           DMA0
#define DMAMUX0               DMAMUX
#define EXAMPLE_CHANNEL       (0U)
#define EXAMPLE_RX_CHANNEL    (1U)
#define EXAMPLE_SAI_TX_SOURCE kDmaRequestMuxSai1Tx
#define EXAMPLE_SAI_RX_SOURCE kDmaRequestMuxSai1Rx

/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (3U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (7U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                        \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/* I2C instance and clock */
#define DEMO_I2C          LPI2C3
#define DEMO_I2C_INSTANCE 3
/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

#define DEMO_CODEC_POWER_GPIO     GPIO2
#define DEMO_CODEC_POWER_GPIO_PIN 8
#define DEMO_CODEC_RESET_GPIO     GPIO2
#define DEMO_CODEC_RESET_GPIO_PIN 6
#define BOARD_MASTER_CLOCK_CONFIG()
#ifndef DEMO_DMAMUX
#define DEMO_DMAMUX DMAMUX0
#endif

#define BUFFER_SIZE (2048U)
#define BUFFER_NUM  (4U)

/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate48KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (8U)
/* demo audio bitwidth */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth32bits
#define DEMO_FRMAE_SYNC_LEN  kSAI_FrameSyncLenOneBitClk
#define DEMO_SAI_CHANNEL     kSAI_Channel0Mask
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BORAD_CodecReset(bool state);

static void tx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);
static status_t DEMO_MountFileSystem(void);
extern void BORAD_CodecReset(bool state);
static void DEMO_InitCodec(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
 *                              = 24 * (32 + 77/100)
 *                              = 786.48 MHz
 */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator   = 77,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 100, /* 30 bit denominator of fractional loop divider */
};

cs42448_config_t cs42448Config = {
    .DACMode      = kCS42448_ModeSlave,
    .ADCMode      = kCS42448_ModeSlave,
    .reset        = BORAD_CodecReset,
    .master       = false,
    .i2cConfig    = {.codecI2CInstance = DEMO_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .format       = {.mclk_HZ = 24576000, .sampleRate = 48000U, .bitWidth = 24U},
    .bus          = kCS42448_BusTDM,
    .slaveAddress = CS42448_I2C_ADDR,
};

codec_config_t boardCodecConfig = {.codecDevType = kCODEC_CS42448, .codecDevConfig = &cs42448Config};
AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t txHandle);
edma_handle_t dmaTxHandle = {0};
extern codec_config_t boardCodecConfig;
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_buffer[BUFFER_NUM][BUFFER_SIZE], 4);
volatile bool isFinished      = false;
volatile uint32_t finishIndex = 0U;
volatile uint32_t emptyBlock  = BUFFER_NUM;
/*! @brief Card descriptor. */
extern sd_card_t g_sd;
static uint32_t volatile s_writeIndex = 0U;
static uint32_t volatile s_readIndex  = 0U;
static uint32_t volatile s_emptyBlock = BUFFER_NUM;
static FATFS s_fileSystem; /* File system object */
static FIL s_fileObject;
static FILINFO s_fileInfo;
static volatile bool s_saiTransferFinish = false;
codec_handle_t codecHandle;

sai_transfer_t saiTxPingPongTransfer[4U] = {
    {
        .data     = s_buffer[0],
        .dataSize = BUFFER_SIZE,
    },
    {
        .data     = s_buffer[1],
        .dataSize = BUFFER_SIZE,
    },
    {
        .data     = s_buffer[2],
        .dataSize = BUFFER_SIZE,
    },
    {
        .data     = s_buffer[3],
        .dataSize = BUFFER_SIZE,
    },
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_USDHCClockConfiguration(void)
{
    /*configure system pll PFD0 fractional divider to 24, output clock is 528MHZ * 18 / 24 = 396 MHZ*/
    CLOCK_InitSysPfd(kCLOCK_Pfd0, 24U);
    /* Configure USDHC clock source and divider */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 1U);
}

void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
    }
    else
    {
        IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK);
    }
}


status_t BOARD_Codec_CS42888_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(DEMO_I2C, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff, txBuffSize);
}

status_t BOARD_Codec_CS42888_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(DEMO_I2C, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize);
}

void BORAD_CodecReset(bool state)
{
    if (state)
    {
        GPIO_PinWrite(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, 0U);
    }
}

static void tx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_RxError == status)
    {
    }
    else
    {
        s_saiTransferFinish = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    edma_config_t dmaConfig = {0};
    sai_transceiver_t saiConfig;
    UINT oneTimeRW = 0U;
    UINT bytesRead;
    FRESULT error;
    uint32_t leftWAVData = 0U;

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    CLOCK_InitAudioPll(&audioPllConfig);
    BOARD_InitDebugConsole();
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

    /*Clock setting for SAI1*/
    CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);
    BOARD_USDHCClockConfiguration();
    GPIO_PinInit(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, &gpio_config);

    /*Enable MCLK clock*/
    BOARD_EnableSaiMclkOutput(true);

    /* enable codec power */
    GPIO_PinInit(DEMO_CODEC_POWER_GPIO, DEMO_CODEC_POWER_GPIO_PIN, &gpio_config);
    GPIO_PinWrite(DEMO_CODEC_POWER_GPIO, DEMO_CODEC_POWER_GPIO_PIN, 1U);
    BOARD_SD_Config(&g_sd, NULL, BOARD_SDMMC_SD_HOST_IRQ, NULL);

    PRINTF("\r\nSAI edma TDM example started.\n\r");

    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);
    EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_DMA, EXAMPLE_CHANNEL);

#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
    DMAMUX_Init(DEMO_DMAMUX);
    DMAMUX_SetSource(DEMO_DMAMUX, EXAMPLE_CHANNEL, EXAMPLE_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, EXAMPLE_CHANNEL);
#endif

    if (DEMO_MountFileSystem() != kStatus_Success)
    {
        PRINTF("Mount file system failed, make sure card is formatted.\r\n");
        return -1;
    }

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, tx_callback, NULL, &dmaTxHandle);

    /* TDM mode configurations */
    SAI_GetTDMConfig(&saiConfig, DEMO_FRMAE_SYNC_LEN, DEMO_AUDIO_BIT_WIDTH, DEMO_AUDIO_DATA_CHANNEL, DEMO_SAI_CHANNEL);
    saiConfig.frameSync.frameSyncEarly = true;
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &txHandle, &saiConfig);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MASTER_CLOCK_CONFIG();

    /* CS42888 initialization */
    DEMO_InitCodec();

    /* The 8_TDM.wav file process flow:
     * 1.Full fill the transfer buffer firstly, it is important to make sure the audio data is transferred continuously.
     * 2.Send one buffer block through SAI when there is at least one buffer block is full.
     * 3.read one buffer block from sdcard when there is at least one buffer block is empty and transfer done.
     * with step2/step3 repeat, audio data will be sent out continuously.
     * */
    if (s_fileInfo.fsize > BUFFER_SIZE * BUFFER_NUM)
    {
        oneTimeRW = BUFFER_SIZE * BUFFER_NUM;
    }
    else
    {
        oneTimeRW = s_fileInfo.fsize;
    }
    error = f_read(&s_fileObject, s_buffer, oneTimeRW, &bytesRead);
    if ((error) || (bytesRead != oneTimeRW))
    {
        PRINTF("Read file failed.\r\n");
        return -1;
    }
    s_emptyBlock -= BUFFER_NUM;
    s_readIndex += BUFFER_NUM;
    leftWAVData = s_fileInfo.fsize - oneTimeRW;
    oneTimeRW   = BUFFER_SIZE;

    PRINTF("\r\nStart play 8_TDM.wav file.\n\r");
    SAI_TransferSendLoopEDMA(DEMO_SAI, &txHandle, &saiTxPingPongTransfer[0], 4U);

    while (leftWAVData > BUFFER_SIZE)
    {
        if ((s_saiTransferFinish))
        {
            if (s_readIndex >= BUFFER_NUM)
            {
                s_readIndex = 0U;
            }

            error = f_read(&s_fileObject, (uint8_t *)((uint32_t)s_buffer[s_readIndex]), BUFFER_SIZE, &bytesRead);
            if ((error) || (bytesRead != BUFFER_SIZE))
            {
                PRINTF("Read file failed.\r\n");
                return -1;
            }

            s_readIndex++;
            leftWAVData -= BUFFER_SIZE;
            s_saiTransferFinish = false;
        }
    }
    f_close(&s_fileObject);
    /* Once transfer finish, disable SAI instance. */
    SAI_TransferAbortSendEDMA(DEMO_SAI, &txHandle);
    SAI_Deinit(DEMO_SAI);
    CODEC_Deinit(&codecHandle);
    PRINTF("\r\nSAI TDM EDMA example finished.\n\r ");
    while (1)
    {
    }
}

#if defined(SAI_ErrorIRQHandler)
void SAI_ErrorIRQHandler(void)
{
    /* Clear the FIFO error flag */
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);

    /* Reset FIFO */
    SAI_TxSoftwareReset(DEMO_SAI, kSAI_ResetTypeFIFO);
    SDK_ISR_EXIT_BARRIER;
}
#endif

static status_t DEMO_MountFileSystem(void)
{
    const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
    FRESULT error;

    if (f_mount(&s_fileSystem, driverNumberBuffer, 0U))
    {
        PRINTF("Mount volume failed.\r\n");
        return kStatus_Fail;
    }

#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        PRINTF("Change drive failed.\r\n");
        return kStatus_Fail;
    }
#endif

    error = f_open(&s_fileObject, _T("/8_TDM.wav"), FA_READ);
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("8_TDM.wav File exists.\r\n");
        }
        else
        {
            PRINTF("8_TDM file not exist.\r\n");
            return kStatus_Fail;
        }
    }

    error = f_stat(_T("/8_TDM.wav"), &s_fileInfo);
    if (error != FR_OK)
    {
        PRINTF("Get file status failed\r\n");
        return kStatus_Fail;
    }

    PRINTF("\r\n8_TDM.wav File is available\r\n");

    return kStatus_Success;
}

static void DEMO_InitCodec(void)
{
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        PRINTF("CODEC_Init failed!\r\n");
        assert(false);
    }

    PRINTF("\r\nCodec Init Done.\r\n");
}
