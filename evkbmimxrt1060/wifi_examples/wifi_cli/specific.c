/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifdef CONFIG_HOST_SLEEP

#include "specific.h"
#include "fsl_common.h"
#include "fsl_clock.h"
#include "lpm.h"
#include "fsl_iomuxc.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

const clock_arm_pll_config_t armPllConfig = {
    .loopDivider = 100, /* PLL loop divider, Fout = Fin * 50 */
    .src         = 0,   /* Bypass clock source, 0 - OSC 24M, 1 - CLK1_P and CLK1_N */
};
const clock_sys_pll_config_t sysPllConfig = {
    .loopDivider = 1, /* PLL loop divider, Fout = Fin * ( 20 + loopDivider*2 + numerator / denominator ) */
    .numerator   = 0, /* 30 bit numerator of fractional loop divider */
    .denominator = 1, /* 30 bit denominator of fractional loop divider */
    .src         = 0, /* Bypass clock source, 0 - OSC 24M, 1 - CLK1_P and CLK1_N */
};
const clock_usb_pll_config_t usbPllConfig = {
    .loopDivider = 0, /* PLL loop divider, Fout = Fin * 20 */
    .src         = 0, /* Bypass clock source, 0 - OSC 24M, 1 - CLK1_P and CLK1_N */
};
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator   = 77,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 100, /* 30 bit denominator of fractional loop divider */
};
const clock_video_pll_config_t videoPllConfig = {
    .loopDivider = 31, /* PLL loop divider, Fout = Fin * ( loopDivider + numerator / denominator ) */
    .postDivider = 8,  /* Divider after PLL */
    .numerator   = 0,  /* 30 bit numerator of fractional loop divider */
    .denominator = 1,  /* 30 bit denominator of fractional loop divider */
    .src         = 0,  /* Bypass clock source, 0 - OSC 24M, 1 - CLK1_P and CLK1_N */
};
const clock_enet_pll_config_t enetPllConfig = {
    .enableClkOutput    = false, /* Disable the PLL providing the ENET 125MHz reference clock */
    .enableClkOutput25M = false, /* Disable the PLL providing the ENET 25MHz reference clock */
    .loopDivider        = 1,     /* Set frequency of ethernet reference clock to 50 MHz */
    .src                = 0,     /* Bypass clock source, 0 - OSC 24M, 1 - CLK1_P and CLK1_N */
};

AT_QUICKACCESS_SECTION_CODE(void SwitchSystemClocks(lpm_power_mode_t power_mode));

void SwitchSystemClocks(lpm_power_mode_t power_mode)
{
#if (defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
    LPM_EnterCritical();
    while (!((FLEXSPI_INST->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) && (FLEXSPI_INST->STS0 & FLEXSPI_STS0_SEQIDLE_MASK)))
    {
    }
    FLEXSPI_INST->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;

    /* Disable clock gate of flexspi. */
    CCM->CCGR6 &= (~CCM_CCGR6_CG5_MASK);
#endif
    switch (power_mode)
    {
        case LPM_PowerModeOverRun:
            CLOCK_SET_DIV(kCLOCK_SemcDiv, 3); // SEMC CLK should not exceed 166MHz
            CLOCK_SET_DIV(kCLOCK_FlexspiDiv, 6);
            CLOCK_SET_MUX(kCLOCK_FlexspiMux, 3); // FLEXSPI mux to PLL3 PFD0
            /* CORE CLK to 600MHz, AHB, IPG to 150MHz, PERCLK to 75MHz */
            CLOCK_SET_DIV(kCLOCK_PerclkDiv, 1);
            CLOCK_SET_DIV(kCLOCK_IpgDiv, 3);
            CLOCK_SET_DIV(kCLOCK_AhbDiv, 0);
            CLOCK_SET_MUX(kCLOCK_PerclkMux, 0);    // PERCLK mux to IPG CLK
            CLOCK_SET_MUX(kCLOCK_PrePeriphMux, 3); // PRE_PERIPH_CLK mux to ARM PLL
            CLOCK_SET_MUX(kCLOCK_PeriphMux, 0);    // PERIPH_CLK mux to PRE_PERIPH_CLK
            break;
        case LPM_PowerModeFullRun:
            CLOCK_SET_DIV(kCLOCK_SemcDiv, 3); // SEMC CLK should not exceed 166MHz
            CLOCK_SET_DIV(kCLOCK_FlexspiDiv, 3);
            CLOCK_SET_MUX(kCLOCK_FlexspiMux, 2); // FLEXSPI mux to PLL2 PFD2
            /* CORE CLK to 528MHz, AHB, IPG to 132MHz, PERCLK to 66MHz */
            CLOCK_SET_DIV(kCLOCK_PerclkDiv, 1);
            CLOCK_SET_DIV(kCLOCK_IpgDiv, 3);
            CLOCK_SET_DIV(kCLOCK_AhbDiv, 0);
            CLOCK_SET_MUX(kCLOCK_PerclkMux, 0);    // PERCLK mux to IPG CLK
            CLOCK_SET_MUX(kCLOCK_PrePeriphMux, 0); // PRE_PERIPH_CLK mux to SYS PLL
            CLOCK_SET_MUX(kCLOCK_PeriphMux, 0);    // PERIPH_CLK mux to PRE_PERIPH_CLK
            break;
        case LPM_PowerModeLowSpeedRun:
        case LPM_PowerModeSysIdle:
            CLOCK_SET_DIV(kCLOCK_SemcDiv, 3); // SEMC CLK should not exceed 166MHz
            CLOCK_SET_DIV(kCLOCK_FlexspiDiv, 3);
            CLOCK_SET_MUX(kCLOCK_FlexspiMux, 2); // FLEXSPI mux to PLL2 PFD2
            /* CORE CLK to 132MHz and AHB, IPG, PERCLK to 33MHz */
            CLOCK_SET_DIV(kCLOCK_PerclkDiv, 0);
            CLOCK_SET_DIV(kCLOCK_IpgDiv, 3);
            CLOCK_SET_DIV(kCLOCK_AhbDiv, 3);
            CLOCK_SET_MUX(kCLOCK_PerclkMux, 0);    // PERCLK mux to IPG CLK
            CLOCK_SET_MUX(kCLOCK_PrePeriphMux, 0); // Switch PRE_PERIPH_CLK to SYS PLL
            CLOCK_SET_MUX(kCLOCK_PeriphMux, 0);    // Switch PERIPH_CLK to PRE_PERIPH_CLK
            break;
        case LPM_PowerModeLowPowerRun:
        case LPM_PowerModeLPIdle:
            CLOCK_SET_DIV(kCLOCK_PeriphClk2Div, 0);
            CLOCK_SET_MUX(kCLOCK_PeriphClk2Mux, 1); // PERIPH_CLK2 mux to OSC
            CLOCK_SET_MUX(kCLOCK_PeriphMux, 1);     // PERIPH_CLK mux to PERIPH_CLK2
            CLOCK_SET_DIV(kCLOCK_SemcDiv, 0);
            CLOCK_SET_MUX(kCLOCK_SemcMux, 0); // SEMC mux to PERIPH_CLK
            CLOCK_SET_DIV(kCLOCK_FlexspiDiv, 0);
            CLOCK_SET_MUX(kCLOCK_FlexspiMux, 0); // FLEXSPI mux to semc_clk_root_pre
            /* CORE CLK to 24MHz and AHB, IPG, PERCLK to 12MHz */
            CLOCK_SET_DIV(kCLOCK_PerclkDiv, 0);
            CLOCK_SET_DIV(kCLOCK_IpgDiv, 1);
            CLOCK_SET_DIV(kCLOCK_AhbDiv, 0);
            CLOCK_SET_MUX(kCLOCK_PerclkMux, 0); // PERCLK mux to IPG CLK
            break;
        default:
            break;
    }

#if (defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
    /* Enable clock gate of flexspi. */
    CCM->CCGR6 |= (CCM_CCGR6_CG5_MASK);

    FLEXSPI_INST->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
    FLEXSPI_INST->MCR0 |= FLEXSPI_MCR0_SWRESET_MASK;
    while (FLEXSPI_INST->MCR0 & FLEXSPI_MCR0_SWRESET_MASK)
    {
    }
    while (!((FLEXSPI_INST->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) && (FLEXSPI_INST->STS0 & FLEXSPI_STS0_SEQIDLE_MASK)))
    {
    }
    LPM_ExitCritical();
#endif
}

void ClockSetToOverDriveRun(void)
{
    // CORE CLK mux to 24M before reconfigure PLLs
    SwitchSystemClocks(LPM_PowerModeLowPowerRun);

    /* Init ARM PLL */
    CLOCK_SetDiv(kCLOCK_ArmDiv, 1);
    CLOCK_InitArmPll(&armPllConfig);

    /* Init SYS PLL*/
    CLOCK_InitSysPll(&sysPllConfig);
    /* Init System pfd0. */
    CLOCK_InitSysPfd(kCLOCK_Pfd0, 27);
    /* Init System pfd1. */
    CLOCK_InitSysPfd(kCLOCK_Pfd1, 16);
    /* Init System pfd2. */
    CLOCK_InitSysPfd(kCLOCK_Pfd2, 24);
    /* Init System pfd3. */
    CLOCK_InitSysPfd(kCLOCK_Pfd3, 16);

    /* Init USB1 PLL. */
    CLOCK_InitUsb1Pll(&usbPllConfig);
    /* Init Usb1 pfd0. */
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 12);
    /* Init Usb1 pfd1. */
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd1, 16);
    /* Init Usb1 pfd2. */
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd2, 17);
    /* Init Usb1 pfd3. */
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd3, 19);

    /* Init USB2 PLL*/
    CLOCK_InitUsb2Pll(&usbPllConfig);

    /* Init AUDIO PLL */
    CLOCK_InitAudioPll(&audioPllConfig);

    /* Init VIDEO PLL */
    CLOCK_InitVideoPll(&videoPllConfig);

    /* Init ENET PLL */
    CLOCK_InitEnetPll(&enetPllConfig);

    SwitchSystemClocks(LPM_PowerModeOverRun);
}

void ClockSetToFullSpeedRun(void)
{
    // CORE CLK mux to 24M before reconfigure PLLs
    SwitchSystemClocks(LPM_PowerModeLowPowerRun);

    /* Init ARM PLL */
    CLOCK_SetDiv(kCLOCK_ArmDiv, 1);
    CLOCK_InitArmPll(&armPllConfig);

    /* Init SYS PLL. */
    CLOCK_InitSysPll(&sysPllConfig);
    /* Init System pfd0. */
    CLOCK_InitSysPfd(kCLOCK_Pfd0, 27);
    /* Init System pfd1. */
    CLOCK_InitSysPfd(kCLOCK_Pfd1, 16);
    /* Init System pfd2. */
    CLOCK_InitSysPfd(kCLOCK_Pfd2, 24);
    /* Init System pfd3. */
    CLOCK_InitSysPfd(kCLOCK_Pfd3, 16);

    /* Init USB1 PLL. */
    CLOCK_InitUsb1Pll(&usbPllConfig);
    /* Init Usb1 pfd0. */
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 12);
    /* Init Usb1 pfd1. */
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd1, 16);
    /* Init Usb1 pfd2. */
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd2, 17);
    /* Init Usb1 pfd3. */
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd3, 19);

    /* Init USB2 PLL*/
    CLOCK_InitUsb2Pll(&usbPllConfig);

    /* Init AUDIO PLL */
    CLOCK_InitAudioPll(&audioPllConfig);

    /* Init VIDEO PLL */
    CLOCK_InitVideoPll(&videoPllConfig);

    /* Init ENET PLL */
    CLOCK_InitEnetPll(&enetPllConfig);

    SwitchSystemClocks(LPM_PowerModeFullRun);
}

void ClockSetToLowSpeedRun(void)
{
    // CORE CLK mux to 24M before reconfigure PLLs
    SwitchSystemClocks(LPM_PowerModeLowPowerRun);

    /* Deinit ARM PLL */
    CLOCK_DeinitArmPll();

    /* Init SYS PLL */
    CLOCK_InitSysPll(&sysPllConfig);

    /* Deinit SYS PLL PFD 0 1 3 */
    CLOCK_DeinitSysPfd(kCLOCK_Pfd0);
    CLOCK_DeinitSysPfd(kCLOCK_Pfd1);
    /* Init System pfd2. */
    CLOCK_InitSysPfd(kCLOCK_Pfd2, 24);
    CLOCK_DeinitSysPfd(kCLOCK_Pfd3);

    /* Deinit USB1 PLL */
    CLOCK_DeinitUsb1Pll();

    /* Deinit USB1 PLL PFD 0 1 2 3 */
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd0);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd1);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd2);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd3);

    /* Deinit USB2 PLL */
    CLOCK_DeinitUsb2Pll();

    /* Deinit AUDIO PLL */
    CLOCK_DeinitAudioPll();

    /* Deinit VIDEO PLL */
    CLOCK_DeinitVideoPll();

    /* Deinit ENET PLL */
    CLOCK_DeinitEnetPll();

    SwitchSystemClocks(LPM_PowerModeLowSpeedRun);
}

void ClockSetToLowPowerRun(void)
{
    // CORE CLK mux to 24M before reconfigure PLLs
    SwitchSystemClocks(LPM_PowerModeLowPowerRun);

    /* Deinit ARM PLL */
    CLOCK_DeinitArmPll();

    /* Deinit SYS PLL */
    CLOCK_DeinitSysPll();

    /* Deinit SYS PLL PFD 0 1 2 3 */
    CLOCK_DeinitSysPfd(kCLOCK_Pfd0);
    CLOCK_DeinitSysPfd(kCLOCK_Pfd1);
    CLOCK_DeinitSysPfd(kCLOCK_Pfd2);
    CLOCK_DeinitSysPfd(kCLOCK_Pfd3);

    /* Power Down USB1 PLL */
    CCM_ANALOG->PLL_USB1_SET = CCM_ANALOG_PLL_USB1_BYPASS_MASK;
    CCM_ANALOG->PLL_USB1_CLR = CCM_ANALOG_PLL_USB1_POWER_MASK;
    CCM_ANALOG->PLL_USB1_CLR = CCM_ANALOG_PLL_USB1_ENABLE_MASK;

    /* Deinit USB1 PLL PFD 0 1 2 3 */
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd0);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd1);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd2);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd3);

    /* Deinit USB2 PLL */
    CLOCK_DeinitUsb2Pll();

    /* Deinit AUDIO PLL */
    CLOCK_DeinitAudioPll();

    /* Deinit VIDEO PLL */
    CLOCK_DeinitVideoPll();

    /* Deinit ENET PLL */
    CLOCK_DeinitEnetPll();
}

void ConfigUartRxPinToGpio(void)
{
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_GPIO1_IO13, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_GPIO1_IO13,
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
}

void ReConfigUartRxPin(void)
{
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPUART1_RX, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_LPUART1_RX, IOMUXC_SW_PAD_CTL_PAD_SPEED(2));
}

#define GPR4_STOP_REQ_BITS                                                                                          \
    (IOMUXC_GPR_GPR4_ENET_STOP_REQ_MASK | IOMUXC_GPR_GPR4_SAI1_STOP_REQ_MASK | IOMUXC_GPR_GPR4_SAI2_STOP_REQ_MASK | \
     IOMUXC_GPR_GPR4_SAI3_STOP_REQ_MASK | IOMUXC_GPR_GPR4_SEMC_STOP_REQ_MASK | IOMUXC_GPR_GPR4_PIT_STOP_REQ_MASK |  \
     IOMUXC_GPR_GPR4_FLEXIO1_STOP_REQ_MASK | IOMUXC_GPR_GPR4_FLEXIO2_STOP_REQ_MASK)

#define GPR4_STOP_ACK_BITS                                                                                          \
    (IOMUXC_GPR_GPR4_ENET_STOP_ACK_MASK | IOMUXC_GPR_GPR4_SAI1_STOP_ACK_MASK | IOMUXC_GPR_GPR4_SAI2_STOP_ACK_MASK | \
     IOMUXC_GPR_GPR4_SAI3_STOP_ACK_MASK | IOMUXC_GPR_GPR4_SEMC_STOP_ACK_MASK | IOMUXC_GPR_GPR4_PIT_STOP_ACK_MASK |  \
     IOMUXC_GPR_GPR4_FLEXIO1_STOP_ACK_MASK | IOMUXC_GPR_GPR4_FLEXIO2_STOP_ACK_MASK)

#define GPR7_STOP_REQ_BITS                                                           \
    (IOMUXC_GPR_GPR7_LPI2C1_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPI2C2_STOP_REQ_MASK |   \
     IOMUXC_GPR_GPR7_LPI2C3_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPI2C4_STOP_REQ_MASK |   \
     IOMUXC_GPR_GPR7_LPSPI1_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPSPI2_STOP_REQ_MASK |   \
     IOMUXC_GPR_GPR7_LPSPI3_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPSPI4_STOP_REQ_MASK |   \
     IOMUXC_GPR_GPR7_LPUART1_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPUART2_STOP_REQ_MASK | \
     IOMUXC_GPR_GPR7_LPUART3_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPUART4_STOP_REQ_MASK | \
     IOMUXC_GPR_GPR7_LPUART5_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPUART6_STOP_REQ_MASK | \
     IOMUXC_GPR_GPR7_LPUART7_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPUART8_STOP_REQ_MASK)

#define GPR7_STOP_ACK_BITS                                                           \
    (IOMUXC_GPR_GPR7_LPI2C1_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPI2C2_STOP_ACK_MASK |   \
     IOMUXC_GPR_GPR7_LPI2C3_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPI2C4_STOP_ACK_MASK |   \
     IOMUXC_GPR_GPR7_LPSPI1_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPSPI2_STOP_ACK_MASK |   \
     IOMUXC_GPR_GPR7_LPSPI3_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPSPI4_STOP_ACK_MASK |   \
     IOMUXC_GPR_GPR7_LPUART1_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPUART2_STOP_ACK_MASK | \
     IOMUXC_GPR_GPR7_LPUART3_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPUART4_STOP_ACK_MASK | \
     IOMUXC_GPR_GPR7_LPUART5_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPUART6_STOP_ACK_MASK | \
     IOMUXC_GPR_GPR7_LPUART7_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPUART8_STOP_ACK_MASK)

#define GPR8_DOZE_BITS                                                               \
    (IOMUXC_GPR_GPR8_LPI2C1_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPI2C2_IPG_DOZE_MASK |   \
     IOMUXC_GPR_GPR8_LPI2C3_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPI2C4_IPG_DOZE_MASK |   \
     IOMUXC_GPR_GPR8_LPSPI1_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPSPI2_IPG_DOZE_MASK |   \
     IOMUXC_GPR_GPR8_LPSPI3_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPSPI4_IPG_DOZE_MASK |   \
     IOMUXC_GPR_GPR8_LPUART1_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPUART2_IPG_DOZE_MASK | \
     IOMUXC_GPR_GPR8_LPUART3_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPUART4_IPG_DOZE_MASK | \
     IOMUXC_GPR_GPR8_LPUART5_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPUART6_IPG_DOZE_MASK | \
     IOMUXC_GPR_GPR8_LPUART7_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPUART8_IPG_DOZE_MASK)

#define GPR8_STOP_MODE_BITS                                                                    \
    (IOMUXC_GPR_GPR8_LPI2C1_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR8_LPI2C2_IPG_STOP_MODE_MASK |   \
     IOMUXC_GPR_GPR8_LPI2C3_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR8_LPI2C4_IPG_STOP_MODE_MASK |   \
     IOMUXC_GPR_GPR8_LPSPI1_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR8_LPSPI2_IPG_STOP_MODE_MASK |   \
     IOMUXC_GPR_GPR8_LPSPI3_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR8_LPSPI4_IPG_STOP_MODE_MASK |   \
     IOMUXC_GPR_GPR8_LPUART2_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR8_LPUART3_IPG_STOP_MODE_MASK | \
     IOMUXC_GPR_GPR8_LPUART4_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR8_LPUART5_IPG_STOP_MODE_MASK | \
     IOMUXC_GPR_GPR8_LPUART6_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR8_LPUART7_IPG_STOP_MODE_MASK | \
     IOMUXC_GPR_GPR8_LPUART8_IPG_STOP_MODE_MASK)

#define GPR12_DOZE_BITS (IOMUXC_GPR_GPR12_FLEXIO1_IPG_DOZE_MASK | IOMUXC_GPR_GPR12_FLEXIO2_IPG_DOZE_MASK)

#define GPR12_STOP_MODE_BITS (IOMUXC_GPR_GPR12_FLEXIO1_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR12_FLEXIO2_IPG_STOP_MODE_MASK)

void PeripheralEnterDozeMode(void)
{
    IOMUXC_GPR->GPR8  = GPR8_DOZE_BITS;
    IOMUXC_GPR->GPR12 = GPR12_DOZE_BITS;
}

void PeripheralExitDozeMode(void)
{
    IOMUXC_GPR->GPR8  = 0x00000000;
    IOMUXC_GPR->GPR12 = 0x00000000;
}

void PeripheralEnterStopMode(void)
{
    IOMUXC_GPR->GPR4 = IOMUXC_GPR_GPR4_ENET_STOP_REQ_MASK;
    while ((IOMUXC_GPR->GPR4 & IOMUXC_GPR_GPR4_ENET_STOP_ACK_MASK) != IOMUXC_GPR_GPR4_ENET_STOP_ACK_MASK)
    {
    }
    IOMUXC_GPR->GPR4  = GPR4_STOP_REQ_BITS;
    IOMUXC_GPR->GPR7  = GPR7_STOP_REQ_BITS;
    IOMUXC_GPR->GPR8  = GPR8_DOZE_BITS | GPR8_STOP_MODE_BITS;
    IOMUXC_GPR->GPR12 = GPR12_DOZE_BITS | GPR12_STOP_MODE_BITS;
    while ((IOMUXC_GPR->GPR4 & GPR4_STOP_ACK_BITS) != GPR4_STOP_ACK_BITS)
    {
    }
    while ((IOMUXC_GPR->GPR7 & GPR7_STOP_ACK_BITS) != GPR7_STOP_ACK_BITS)
    {
    }
}

#endif
