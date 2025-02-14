/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_sai_sdma.h"
#include "music.h"
#include "fsl_codec_common.h"
#include "fsl_asrc.h"
#include "fsl_wm8960.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_codec_adapter.h"
#include "fsl_sai.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_SAI          (I2S3)
#define DEMO_ASRC         ASRC
#define DEMO_SAI_CLK_FREQ 24576000U

#define DEMO_IRQn             I2S3_IRQn
#define DEMO_DMA              SDMAARM3
#define DEMO_SAI_SDMA_CHANNEL (1)
#define DEMO_ASRC_CONTEXT     kASRC_Context0
#define DEMO_SAI_TX_SOURCE    (5)
#define SAI_UserIRQHandler    I2S3_IRQHandler
/*set Bclk source to Mclk clock*/
#define DEMO_SAI_CLOCK_SOURCE     (1U)
#define SDMA_FREQ_EQUALS_ARM      (1U)
#define DEMO_ASRC_IN_SAMPLE_RATE  16000
#define DEMO_ASRC_OUT_SAMPLE_RATE 48000

#define I2C_RELEASE_SDA_GPIO  GPIO5
#define I2C_RELEASE_SDA_PIN   19U
#define I2C_RELEASE_SCL_GPIO  GPIO5
#define I2C_RELEASE_SCL_PIN   18U
#define I2C_RELEASE_BUS_COUNT 100U

#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_DATA_CHANNEL (2U)
/* demo audio bit width */
#define DEMO_AUDIO_BUFFER_SAMPLES (24000)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_I2C_ReleaseBus(void);
void BOARD_MASTER_CLOCK_CONFIG(void);
static void saiCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData);
static void saiPlayAudio(uint8_t *data, uint32_t dataSize);
/*******************************************************************************
 * Variables
 ******************************************************************************/
wm8960_config_t wm8960Config = {
    .i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .route     = kWM8960_RoutePlaybackandRecord,
    .rightInputSource = kWM8960_InputDifferentialMicInput2,
    .playSource       = kWM8960_PlaySourceDAC,
    .slaveAddress     = WM8960_I2C_ADDR,
    .bus              = kWM8960_BusI2S,
    .format           = {.mclk_HZ    = 24576000U,
               .sampleRate = kWM8960_AudioSampleRate16KHz,
               .bitWidth   = kWM8960_AudioBitWidth16bit},
    .master_slave     = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8960, .codecDevConfig = &wm8960Config};
sai_master_clock_t mclkConfig;
AT_NONCACHEABLE_SECTION_ALIGN(sai_sdma_handle_t txHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_handle_t dmaHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t s_saiContext, 4);
volatile uint8_t isSaiFinishedCount = false;
extern codec_config_t boardCodecConfig;
codec_handle_t codecHandle;

AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_asrcOutBuffer[DEMO_AUDIO_BUFFER_SAMPLES * 2U], 4);
static asrc_context_config_t s_asrcContextConfig;
/*******************************************************************************
 * Code
 ******************************************************************************/

static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __NOP();
    }
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i                    = 0;
    gpio_pin_config_t pin_config = {kGPIO_DigitalOutput, 1, kGPIO_NoIntmode};

    IOMUXC_SetPinMux(IOMUXC_I2C3_SCL_GPIO5_IO18, 0U);
    IOMUXC_SetPinConfig(IOMUXC_I2C3_SCL_GPIO5_IO18, IOMUXC_SW_PAD_CTL_PAD_DSE(3U) | IOMUXC_SW_PAD_CTL_PAD_FSEL_MASK |
                                                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_I2C3_SDA_GPIO5_IO19, 0U);
    IOMUXC_SetPinConfig(IOMUXC_I2C3_SDA_GPIO5_IO19, IOMUXC_SW_PAD_CTL_PAD_DSE(3U) | IOMUXC_SW_PAD_CTL_PAD_FSEL_MASK |
                                                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();
}

void BOARD_MASTER_CLOCK_CONFIG(void)
{
    mclkConfig.mclkOutputEnable = true;
    mclkConfig.mclkHz           = DEMO_AUDIO_MASTER_CLOCK;
    mclkConfig.mclkSourceClkHz  = DEMO_SAI_CLK_FREQ;
    SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
}
static void saiCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData)
{
    isSaiFinishedCount++;
}

/*!
 * @brief Main function
 */
int main(void)
{
    asrc_transfer_t asrcTransfer;
    sai_transceiver_t config;

    /* M7 has its local cache and enabled by default,
     * need to set smart subsystems (0x28000000 ~ 0x3FFFFFFF)
     * non-cacheable before accessing this address region */
    BOARD_InitMemory();

    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();

    CLOCK_SetRootMux(kCLOCK_RootSai3, kCLOCK_SaiRootmuxAudioPll1); /* Set SAI source to AUDIO PLL1 393216000HZ*/
    CLOCK_SetRootDivider(kCLOCK_RootSai3, 1U, 16U);                /* Set root clock to 393216000HZ / 16 = 24.576MHz */

    /* SAI bit clock source */
    AUDIOMIX_AttachClk(AUDIOMIX, kAUDIOMIX_Attach_SAI3_MCLK1_To_SAI3_ROOT);

    CLOCK_SetRootMux(kCLOCK_RootI2c3, kCLOCK_I2cRootmuxSysPll1Div5); /* Set I2C source to SysPLL1 Div5 160MHZ */
    CLOCK_SetRootDivider(kCLOCK_RootI2c3, 1U, 10U);                  /* Set root clock to 160MHZ / 10 = 16MHZ */

    PRINTF("ASRC memory to memory polling example.\n\r");

    memset(s_asrcOutBuffer, 0U, sizeof(s_asrcOutBuffer));

    /* peripheral initialization */
    sdma_config_t dmaConfig = {0};

    /* Create SDMA handle */
    SDMA_GetDefaultConfig(&dmaConfig);
#ifdef SDMA_FREQ_EQUALS_ARM
    dmaConfig.ratio = kSDMA_ARMClockFreq;
#endif
    SDMA_Init(DEMO_DMA, &dmaConfig);

    ASRC_Init(DEMO_ASRC);
    ASRC_GetContextDefaultConfig(&s_asrcContextConfig, DEMO_AUDIO_DATA_CHANNEL, DEMO_ASRC_IN_SAMPLE_RATE,
                                 DEMO_ASRC_OUT_SAMPLE_RATE);
    if (ASRC_SetContextConfig(DEMO_ASRC, DEMO_ASRC_CONTEXT, &s_asrcContextConfig) != kStatus_Success)
    {
        PRINTF("ASRC context configure failed, please check.\r\n");
        return -1;
    }

    /* create sai sdma handle */
    SDMA_CreateHandle(&dmaHandle, DEMO_DMA, DEMO_SAI_SDMA_CHANNEL, &s_saiContext);
    SDMA_SetChannelPriority(DEMO_DMA, DEMO_SAI_SDMA_CHANNEL, 2);

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandleSDMA(DEMO_SAI, &txHandle, saiCallback, NULL, &dmaHandle, DEMO_SAI_TX_SOURCE);
    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
    config.bitClock.bclkSource = (sai_bclk_source_t)DEMO_SAI_CLOCK_SOURCE;
    SAI_TransferTxSetConfigSDMA(DEMO_SAI, &txHandle, &config);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_ASRC_IN_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MASTER_CLOCK_CONFIG();

    /* Enable SAI interrupt to clear the FIFO error */
    EnableIRQ(DEMO_IRQn);
    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);

    /* Use default setting to init codec */
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }

    PRINTF("Playback raw audio data\r\n");
    PRINTF("    sample rate : %d\r\n", DEMO_ASRC_IN_SAMPLE_RATE);
    PRINTF("    channel number: %d\r\n", DEMO_AUDIO_DATA_CHANNEL);
    PRINTF("    frequency: 215HZ.\r\n\r\n");

    /*  xfer structure */
    saiPlayAudio((uint8_t *)music, MUSIC_LEN);

    asrcTransfer.inDataAddr  = (uint32_t *)music;
    asrcTransfer.inDataSize  = MUSIC_LEN;
    asrcTransfer.outDataAddr = s_asrcOutBuffer;
    asrcTransfer.outDataSize =
        ASRC_GetContextOutSampleSize(DEMO_ASRC_IN_SAMPLE_RATE, MUSIC_LEN, 2U, DEMO_ASRC_OUT_SAMPLE_RATE, 2U);
    ASRC_TransferBlocking(DEMO_ASRC, DEMO_ASRC_CONTEXT, &asrcTransfer);

    /* reconfigure the sai clock */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_ASRC_OUT_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    PRINTF("Playback converted audio data\r\n");
    PRINTF("    sample rate : %d\r\n", DEMO_ASRC_OUT_SAMPLE_RATE);
    PRINTF("    channel number: %d\r\n", DEMO_AUDIO_DATA_CHANNEL);
    PRINTF("    frequency: 215HZ.\r\n\r\n");

    saiPlayAudio((uint8_t *)s_asrcOutBuffer,
                 ASRC_GetContextOutSampleSize(DEMO_ASRC_IN_SAMPLE_RATE, MUSIC_LEN, 2U, DEMO_ASRC_OUT_SAMPLE_RATE, 2U));

    /* Once transfer all finish, de-initializes the SAI peripheral */
    SAI_Deinit(DEMO_SAI);
    PRINTF("ASRC memory to memory polling example finished.\n\r ");
    while (1)
    {
    }
}

static void saiPlayAudio(uint8_t *data, uint32_t dataSize)
{
    sai_transfer_t xfer;
    uint32_t size = dataSize;
    uint8_t *addr = data;
    uint8_t count = 0U;

    while (size)
    {
        /*  xfer structure */
        xfer.data     = addr;
        xfer.dataSize = size > 64000 ? 64000 : size;
        if (SAI_TransferSendSDMA(DEMO_SAI, &txHandle, &xfer) == kStatus_Success)
        {
            size -= xfer.dataSize;
            addr += xfer.dataSize;
            count++;
        }
    }

    while (isSaiFinishedCount != count)
    {
    }
    isSaiFinishedCount = 0U;

    /* Once transfer finish, disable SAI instance and clear all the internal information. */
    SAI_TransferTerminateSendSDMA(DEMO_SAI, &txHandle);
}

void SAI_UserIRQHandler(void)
{
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    SDK_ISR_EXIT_BARRIER;
}
