/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v9.0
processor: LPC54628J512
package_id: LPC54628J512ET180
mcu_data: ksdk2_0
processor_version: 9.0.1
board: LPCXpresso54628
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_iocon.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A2, peripheral: FLEXCOMM0, signal: TXD_SCL_MISO, pin_signal: PIO0_30/FC0_TXD_SCL_MISO/CTIMER0_MAT0/SCT0_OUT9/TRACEDATA(1), mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: B13, peripheral: FLEXCOMM0, signal: RXD_SDA_MOSI, pin_signal: PIO0_29/FC0_RXD_SDA_MOSI/CTIMER2_MAT3/SCT0_OUT8/TRACEDATA(2), mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: B1, peripheral: SDIF, signal: SD_CLK, pin_signal: PIO2_3/ENET_TXD2/SD_CLK/FC1_RXD_SDA_MOSI/CTIMER2_MAT0, mode: inactive, invert: disabled, glitch_filter: disabled,
    slew_rate: fast, open_drain: disabled}
  - {pin_num: D3, peripheral: SDIF, signal: SD_CMD, pin_signal: PIO2_4/ENET_TXD3/SD_CMD/FC1_TXD_SCL_MISO/CTIMER2_MAT1, mode: inactive, invert: disabled, glitch_filter: disabled,
    slew_rate: fast, open_drain: disabled}
  - {pin_num: C1, peripheral: SDIF, signal: SD_POW_EN, pin_signal: PIO2_5/ENET_TX_ER/SD_POW_EN/FC1_CTS_SDA_SSEL0/CTIMER1_MAT2, mode: inactive, invert: disabled, glitch_filter: disabled,
    slew_rate: standard, open_drain: disabled}
  - {pin_num: F3, peripheral: SDIF, signal: 'SD_D, 0', pin_signal: PIO2_6/ENET_TX_CLK/SD_D(0)/FC1_RTS_SCL_SSEL1/CTIMER0_CAP0, mode: inactive, invert: disabled, glitch_filter: disabled,
    slew_rate: fast, open_drain: disabled}
  - {pin_num: J2, peripheral: SDIF, signal: 'SD_D, 1', pin_signal: PIO2_7/ENET_COL/SD_D(1)/FREQME_GPIO_CLK_B/CTIMER0_CAP1, mode: inactive, invert: disabled, glitch_filter: disabled,
    slew_rate: fast, open_drain: disabled}
  - {pin_num: F4, peripheral: SDIF, signal: 'SD_D, 2', pin_signal: PIO2_8/ENET_RXD2/SD_D(2)/CTIMER0_MAT0, mode: inactive, invert: disabled, glitch_filter: disabled,
    slew_rate: fast, open_drain: disabled}
  - {pin_num: K2, peripheral: SDIF, signal: 'SD_D, 3', pin_signal: PIO2_9/ENET_RXD3/SD_D(3)/CTIMER0_MAT1, mode: inactive, invert: disabled, glitch_filter: disabled,
    slew_rate: fast, open_drain: disabled}
  - {pin_num: P1, peripheral: SDIF, signal: SD_CARD_DET, pin_signal: PIO2_10/ENET_RX_ER/SD_CARD_DET_N, mode: inactive, invert: disabled, glitch_filter: disabled,
    slew_rate: standard, open_drain: disabled}
  - {pin_num: D2, peripheral: SDIF, signal: SD_WR_PRT, pin_signal: PIO3_15/FC8_SCK/SD_WR_PRT, mode: inactive, invert: disabled, glitch_filter: disabled, slew_rate: standard,
    open_drain: disabled}
  - {pin_num: B12, peripheral: USBFSH, signal: USB_VBUS, pin_signal: PIO0_22/FC6_TXD_SCL_MISO_WS/UTICK_CAP1/CTIMER3_CAP3/SCT0_OUT3/USB0_VBUS, mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: A12, peripheral: USBHSH, signal: USB_PORTPWRN, pin_signal: PIO4_9/ENET_TXD1/FC2_RXD_SDA_MOSI/USB1_PORTPWRN/USB1_FRAME/SCT0_GPI2, mode: pullUp, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: A14, peripheral: USBFSH, signal: USB_PORTPWRN, pin_signal: PIO4_7/CTIMER4_CAP3/USB0_PORTPWRN/USB0_FRAME/SCT0_GPI0, mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: K12, peripheral: USBHSH, signal: USB_OVERCURRENTN, pin_signal: PIO1_1/FC3_RXD_SDA_MOSI/CTIMER0_CAP3/SCT0_GPI5/USB1_OVERCURRENTN, mode: pullUp, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: B14, peripheral: USBFSH, signal: USB_OVERCURRENTN, pin_signal: PIO4_8/ENET_TXD0/FC2_SCK/USB0_OVERCURRENTN/USB0_UP_LED/SCT0_GPI1, mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: F10, peripheral: SDIF, signal: 'SD_D, 4', pin_signal: PIO1_27/FC2_RTS_SCL_SSEL1/SD_D(4)/CTIMER0_MAT3/CLKOUT/EMC_A(9), mode: pullUp, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: E12, peripheral: SDIF, signal: 'SD_D, 5', pin_signal: PIO1_28/FC7_SCK/SD_D(5)/CTIMER0_CAP2/EMC_D(12), mode: pullUp, invert: disabled, glitch_filter: disabled,
    slew_rate: standard, open_drain: disabled}
  - {pin_num: C11, peripheral: SDIF, signal: 'SD_D, 6', pin_signal: PIO1_29/FC7_RXD_SDA_MOSI_DATA/SD_D(6)/SCT0_GPI6/USB1_PORTPWRN/USB1_FRAME/EMC_D(13), mode: pullUp,
    invert: disabled, glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: A8, peripheral: SDIF, signal: 'SD_D, 7', pin_signal: PIO1_30/FC7_TXD_SCL_MISO_WS/SD_D(7)/SCT0_GPI7/USB1_OVERCURRENTN/USB1_UP_LED/EMC_D(14), mode: pullUp,
    invert: disabled, glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M4F */
void BOARD_InitPins(void)
{
    /* Enables the clock for the IOCON block. 0 = Disable; 1 = Enable.: 0x01u */
    CLOCK_EnableClock(kCLOCK_Iocon);

    const uint32_t USB0_VBUS = (/* Pin is configured as USB0_VBUS */
                                IOCON_PIO_FUNC7 |
                                /* No addition pin function */
                                IOCON_PIO_MODE_INACT |
                                /* Input function is not inverted */
                                IOCON_PIO_INV_DI |
                                /* Enables digital function */
                                IOCON_PIO_DIGITAL_EN |
                                /* Input filter disabled */
                                IOCON_PIO_INPFILT_OFF |
                                /* Standard mode, output slew rate control is enabled */
                                IOCON_PIO_SLEW_STANDARD |
                                /* Open drain is disabled */
                                IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN22 (coords: B12) is configured as USB0_VBUS */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_USB0_VBUS_PORT, BOARD_INITPINS_USB0_VBUS_PIN, USB0_VBUS);

    const uint32_t ISP_FC0_RXD = (/* Pin is configured as FC0_RXD_SDA_MOSI */
                                  IOCON_PIO_FUNC1 |
                                  /* No addition pin function */
                                  IOCON_PIO_MODE_INACT |
                                  /* Input function is not inverted */
                                  IOCON_PIO_INV_DI |
                                  /* Enables digital function */
                                  IOCON_PIO_DIGITAL_EN |
                                  /* Input filter disabled */
                                  IOCON_PIO_INPFILT_OFF |
                                  /* Standard mode, output slew rate control is enabled */
                                  IOCON_PIO_SLEW_STANDARD |
                                  /* Open drain is disabled */
                                  IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN29 (coords: B13) is configured as FC0_RXD_SDA_MOSI */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_ISP_FC0_RXD_PORT, BOARD_INITPINS_ISP_FC0_RXD_PIN, ISP_FC0_RXD);

    const uint32_t ISP_FC0_TXD = (/* Pin is configured as FC0_TXD_SCL_MISO */
                                  IOCON_PIO_FUNC1 |
                                  /* No addition pin function */
                                  IOCON_PIO_MODE_INACT |
                                  /* Input function is not inverted */
                                  IOCON_PIO_INV_DI |
                                  /* Enables digital function */
                                  IOCON_PIO_DIGITAL_EN |
                                  /* Input filter disabled */
                                  IOCON_PIO_INPFILT_OFF |
                                  /* Standard mode, output slew rate control is enabled */
                                  IOCON_PIO_SLEW_STANDARD |
                                  /* Open drain is disabled */
                                  IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN30 (coords: A2) is configured as FC0_TXD_SCL_MISO */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_ISP_FC0_TXD_PORT, BOARD_INITPINS_ISP_FC0_TXD_PIN, ISP_FC0_TXD);

    const uint32_t SW5 = (/* Pin is configured as USB1_OVERCURRENTN */
                          IOCON_PIO_FUNC7 |
                          /* Selects pull-up function */
                          IOCON_PIO_MODE_PULLUP |
                          /* Input function is not inverted */
                          IOCON_PIO_INV_DI |
                          /* Enables digital function */
                          IOCON_PIO_DIGITAL_EN |
                          /* Input filter disabled */
                          IOCON_PIO_INPFILT_OFF |
                          /* Standard mode, output slew rate control is enabled */
                          IOCON_PIO_SLEW_STANDARD |
                          /* Open drain is disabled */
                          IOCON_PIO_OPENDRAIN_DI);
    /* PORT1 PIN1 (coords: K12) is configured as USB1_OVERCURRENTN */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_SW5_PORT, BOARD_INITPINS_SW5_PIN, SW5);

    const uint32_t EMC_A9 = (/* Pin is configured as SD_D(4) */
                             IOCON_PIO_FUNC2 |
                             /* Selects pull-up function */
                             IOCON_PIO_MODE_PULLUP |
                             /* Input function is not inverted */
                             IOCON_PIO_INV_DI |
                             /* Enables digital function */
                             IOCON_PIO_DIGITAL_EN |
                             /* Input filter disabled */
                             IOCON_PIO_INPFILT_OFF |
                             /* Standard mode, output slew rate control is enabled */
                             IOCON_PIO_SLEW_STANDARD |
                             /* Open drain is disabled */
                             IOCON_PIO_OPENDRAIN_DI);
    /* PORT1 PIN27 (coords: F10) is configured as SD_D(4) */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_EMC_A9_PORT, BOARD_INITPINS_EMC_A9_PIN, EMC_A9);

    const uint32_t EMC_D12 = (/* Pin is configured as SD_D(5) */
                              IOCON_PIO_FUNC2 |
                              /* Selects pull-up function */
                              IOCON_PIO_MODE_PULLUP |
                              /* Input function is not inverted */
                              IOCON_PIO_INV_DI |
                              /* Enables digital function */
                              IOCON_PIO_DIGITAL_EN |
                              /* Input filter disabled */
                              IOCON_PIO_INPFILT_OFF |
                              /* Standard mode, output slew rate control is enabled */
                              IOCON_PIO_SLEW_STANDARD |
                              /* Open drain is disabled */
                              IOCON_PIO_OPENDRAIN_DI);
    /* PORT1 PIN28 (coords: E12) is configured as SD_D(5) */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_EMC_D12_PORT, BOARD_INITPINS_EMC_D12_PIN, EMC_D12);

    const uint32_t EMC_D13 = (/* Pin is configured as SD_D(6) */
                              IOCON_PIO_FUNC2 |
                              /* Selects pull-up function */
                              IOCON_PIO_MODE_PULLUP |
                              /* Input function is not inverted */
                              IOCON_PIO_INV_DI |
                              /* Enables digital function */
                              IOCON_PIO_DIGITAL_EN |
                              /* Input filter disabled */
                              IOCON_PIO_INPFILT_OFF |
                              /* Standard mode, output slew rate control is enabled */
                              IOCON_PIO_SLEW_STANDARD |
                              /* Open drain is disabled */
                              IOCON_PIO_OPENDRAIN_DI);
    /* PORT1 PIN29 (coords: C11) is configured as SD_D(6) */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_EMC_D13_PORT, BOARD_INITPINS_EMC_D13_PIN, EMC_D13);

    const uint32_t EMC_D14 = (/* Pin is configured as SD_D(7) */
                              IOCON_PIO_FUNC2 |
                              /* Selects pull-up function */
                              IOCON_PIO_MODE_PULLUP |
                              /* Input function is not inverted */
                              IOCON_PIO_INV_DI |
                              /* Enables digital function */
                              IOCON_PIO_DIGITAL_EN |
                              /* Input filter disabled */
                              IOCON_PIO_INPFILT_OFF |
                              /* Standard mode, output slew rate control is enabled */
                              IOCON_PIO_SLEW_STANDARD |
                              /* Open drain is disabled */
                              IOCON_PIO_OPENDRAIN_DI);
    /* PORT1 PIN30 (coords: A8) is configured as SD_D(7) */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_EMC_D14_PORT, BOARD_INITPINS_EMC_D14_PIN, EMC_D14);

    const uint32_t SD_CDn = (/* Pin is configured as SD_CARD_DET_N */
                             IOCON_PIO_FUNC2 |
                             /* No addition pin function */
                             IOCON_PIO_MODE_INACT |
                             /* Input function is not inverted */
                             IOCON_PIO_INV_DI |
                             /* Enables digital function */
                             IOCON_PIO_DIGITAL_EN |
                             /* Input filter disabled */
                             IOCON_PIO_INPFILT_OFF |
                             /* Standard mode, output slew rate control is enabled */
                             IOCON_PIO_SLEW_STANDARD |
                             /* Open drain is disabled */
                             IOCON_PIO_OPENDRAIN_DI);
    /* PORT2 PIN10 (coords: P1) is configured as SD_CARD_DET_N */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_SD_CDn_PORT, BOARD_INITPINS_SD_CDn_PIN, SD_CDn);

    const uint32_t SD_CLK = (/* Pin is configured as SD_CLK */
                             IOCON_PIO_FUNC2 |
                             /* No addition pin function */
                             IOCON_PIO_MODE_INACT |
                             /* Input function is not inverted */
                             IOCON_PIO_INV_DI |
                             /* Enables digital function */
                             IOCON_PIO_DIGITAL_EN |
                             /* Input filter disabled */
                             IOCON_PIO_INPFILT_OFF |
                             /* Fast mode, slew rate control is disabled */
                             IOCON_PIO_SLEW_FAST |
                             /* Open drain is disabled */
                             IOCON_PIO_OPENDRAIN_DI);
    /* PORT2 PIN3 (coords: B1) is configured as SD_CLK */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_SD_CLK_PORT, BOARD_INITPINS_SD_CLK_PIN, SD_CLK);

    const uint32_t SD_CMD = (/* Pin is configured as SD_CMD */
                             IOCON_PIO_FUNC2 |
                             /* No addition pin function */
                             IOCON_PIO_MODE_INACT |
                             /* Input function is not inverted */
                             IOCON_PIO_INV_DI |
                             /* Enables digital function */
                             IOCON_PIO_DIGITAL_EN |
                             /* Input filter disabled */
                             IOCON_PIO_INPFILT_OFF |
                             /* Fast mode, slew rate control is disabled */
                             IOCON_PIO_SLEW_FAST |
                             /* Open drain is disabled */
                             IOCON_PIO_OPENDRAIN_DI);
    /* PORT2 PIN4 (coords: D3) is configured as SD_CMD */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_SD_CMD_PORT, BOARD_INITPINS_SD_CMD_PIN, SD_CMD);

    const uint32_t SD_POW_EN = (/* Pin is configured as SD_POW_EN */
                                IOCON_PIO_FUNC2 |
                                /* No addition pin function */
                                IOCON_PIO_MODE_INACT |
                                /* Input function is not inverted */
                                IOCON_PIO_INV_DI |
                                /* Enables digital function */
                                IOCON_PIO_DIGITAL_EN |
                                /* Input filter disabled */
                                IOCON_PIO_INPFILT_OFF |
                                /* Standard mode, output slew rate control is enabled */
                                IOCON_PIO_SLEW_STANDARD |
                                /* Open drain is disabled */
                                IOCON_PIO_OPENDRAIN_DI);
    /* PORT2 PIN5 (coords: C1) is configured as SD_POW_EN */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_SD_POW_EN_PORT, BOARD_INITPINS_SD_POW_EN_PIN, SD_POW_EN);

    const uint32_t SD_D0 = (/* Pin is configured as SD_D(0) */
                            IOCON_PIO_FUNC2 |
                            /* No addition pin function */
                            IOCON_PIO_MODE_INACT |
                            /* Input function is not inverted */
                            IOCON_PIO_INV_DI |
                            /* Enables digital function */
                            IOCON_PIO_DIGITAL_EN |
                            /* Input filter disabled */
                            IOCON_PIO_INPFILT_OFF |
                            /* Fast mode, slew rate control is disabled */
                            IOCON_PIO_SLEW_FAST |
                            /* Open drain is disabled */
                            IOCON_PIO_OPENDRAIN_DI);
    /* PORT2 PIN6 (coords: F3) is configured as SD_D(0) */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_SD_D0_PORT, BOARD_INITPINS_SD_D0_PIN, SD_D0);

    const uint32_t SD_D1 = (/* Pin is configured as SD_D(1) */
                            IOCON_PIO_FUNC2 |
                            /* No addition pin function */
                            IOCON_PIO_MODE_INACT |
                            /* Input function is not inverted */
                            IOCON_PIO_INV_DI |
                            /* Enables digital function */
                            IOCON_PIO_DIGITAL_EN |
                            /* Input filter disabled */
                            IOCON_PIO_INPFILT_OFF |
                            /* Fast mode, slew rate control is disabled */
                            IOCON_PIO_SLEW_FAST |
                            /* Open drain is disabled */
                            IOCON_PIO_OPENDRAIN_DI);
    /* PORT2 PIN7 (coords: J2) is configured as SD_D(1) */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_SD_D1_PORT, BOARD_INITPINS_SD_D1_PIN, SD_D1);

    const uint32_t SD_D2 = (/* Pin is configured as SD_D(2) */
                            IOCON_PIO_FUNC2 |
                            /* No addition pin function */
                            IOCON_PIO_MODE_INACT |
                            /* Input function is not inverted */
                            IOCON_PIO_INV_DI |
                            /* Enables digital function */
                            IOCON_PIO_DIGITAL_EN |
                            /* Input filter disabled */
                            IOCON_PIO_INPFILT_OFF |
                            /* Fast mode, slew rate control is disabled */
                            IOCON_PIO_SLEW_FAST |
                            /* Open drain is disabled */
                            IOCON_PIO_OPENDRAIN_DI);
    /* PORT2 PIN8 (coords: F4) is configured as SD_D(2) */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_SD_D2_PORT, BOARD_INITPINS_SD_D2_PIN, SD_D2);

    const uint32_t SD_D3 = (/* Pin is configured as SD_D(3) */
                            IOCON_PIO_FUNC2 |
                            /* No addition pin function */
                            IOCON_PIO_MODE_INACT |
                            /* Input function is not inverted */
                            IOCON_PIO_INV_DI |
                            /* Enables digital function */
                            IOCON_PIO_DIGITAL_EN |
                            /* Input filter disabled */
                            IOCON_PIO_INPFILT_OFF |
                            /* Fast mode, slew rate control is disabled */
                            IOCON_PIO_SLEW_FAST |
                            /* Open drain is disabled */
                            IOCON_PIO_OPENDRAIN_DI);
    /* PORT2 PIN9 (coords: K2) is configured as SD_D(3) */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_SD_D3_PORT, BOARD_INITPINS_SD_D3_PIN, SD_D3);

    const uint32_t SD_WPn = (/* Pin is configured as SD_WR_PRT */
                             IOCON_PIO_FUNC2 |
                             /* No addition pin function */
                             IOCON_PIO_MODE_INACT |
                             /* Input function is not inverted */
                             IOCON_PIO_INV_DI |
                             /* Enables digital function */
                             IOCON_PIO_DIGITAL_EN |
                             /* Input filter disabled */
                             IOCON_PIO_INPFILT_OFF |
                             /* Standard mode, output slew rate control is enabled */
                             IOCON_PIO_SLEW_STANDARD |
                             /* Open drain is disabled */
                             IOCON_PIO_OPENDRAIN_DI);
    /* PORT3 PIN15 (coords: D2) is configured as SD_WR_PRT */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_SD_WPn_PORT, BOARD_INITPINS_SD_WPn_PIN, SD_WPn);

    const uint32_t port4_pin7_config = (/* Pin is configured as USB0_PORTPWRN */
                                        IOCON_PIO_FUNC3 |
                                        /* No addition pin function */
                                        IOCON_PIO_MODE_INACT |
                                        /* Input function is not inverted */
                                        IOCON_PIO_INV_DI |
                                        /* Enables digital function */
                                        IOCON_PIO_DIGITAL_EN |
                                        /* Input filter disabled */
                                        IOCON_PIO_INPFILT_OFF |
                                        /* Standard mode, output slew rate control is enabled */
                                        IOCON_PIO_SLEW_STANDARD |
                                        /* Open drain is disabled */
                                        IOCON_PIO_OPENDRAIN_DI);
    /* PORT4 PIN7 (coords: A14) is configured as USB0_PORTPWRN */
    IOCON_PinMuxSet(IOCON, 4U, 7U, port4_pin7_config);

    const uint32_t ENET_TXD0 = (/* Pin is configured as USB0_OVERCURRENTN */
                                IOCON_PIO_FUNC3 |
                                /* No addition pin function */
                                IOCON_PIO_MODE_INACT |
                                /* Input function is not inverted */
                                IOCON_PIO_INV_DI |
                                /* Enables digital function */
                                IOCON_PIO_DIGITAL_EN |
                                /* Input filter disabled */
                                IOCON_PIO_INPFILT_OFF |
                                /* Standard mode, output slew rate control is enabled */
                                IOCON_PIO_SLEW_STANDARD |
                                /* Open drain is disabled */
                                IOCON_PIO_OPENDRAIN_DI);
    /* PORT4 PIN8 (coords: B14) is configured as USB0_OVERCURRENTN */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_ENET_TXD0_PORT, BOARD_INITPINS_ENET_TXD0_PIN, ENET_TXD0);

    const uint32_t port4_pin9_config = (/* Pin is configured as USB1_PORTPWRN */
                                        IOCON_PIO_FUNC3 |
                                        /* Selects pull-up function */
                                        IOCON_PIO_MODE_PULLUP |
                                        /* Input function is not inverted */
                                        IOCON_PIO_INV_DI |
                                        /* Enables digital function */
                                        IOCON_PIO_DIGITAL_EN |
                                        /* Input filter disabled */
                                        IOCON_PIO_INPFILT_OFF |
                                        /* Standard mode, output slew rate control is enabled */
                                        IOCON_PIO_SLEW_STANDARD |
                                        /* Open drain is disabled */
                                        IOCON_PIO_OPENDRAIN_DI);
    /* PORT4 PIN9 (coords: A12) is configured as USB1_PORTPWRN */
    IOCON_PinMuxSet(IOCON, 4U, 9U, port4_pin9_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
