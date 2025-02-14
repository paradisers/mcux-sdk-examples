/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v9.0
processor: MKE17Z256xxx7
package_id: MKE17Z256VLL7
mcu_data: ksdk2_0
processor_version: 0.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
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
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '54', peripheral: LPUART0, signal: RX, pin_signal: TSI1_CH15/PTB0/LPUART0_RX/LPSPI0_PCS0/LPTMR0_ALT3/PWT_IN3}
  - {pin_num: '53', peripheral: LPUART0, signal: TX, pin_signal: TSI1_CH16/PTB1/LPUART0_TX/LPSPI0_SOUT/TCLK0}
  - {pin_num: '99', peripheral: TRGMUX0, signal: 'OUT, 5', pin_signal: PTA9/FXIO_D7/TRGMUX_OUT1}
  - {pin_num: '100', peripheral: TRGMUX0, signal: 'OUT, 4', pin_signal: PTA8/FXIO_D6/TRGMUX_OUT0}
  - {pin_num: '20', peripheral: TRGMUX0, signal: 'OUT, 6', pin_signal: PTD17/FTM0_FLT2/LPUART2_RX/TRGMUX_OUT2}
  - {pin_num: '19', peripheral: TRGMUX0, signal: 'OUT, 7', pin_signal: PTE12/FTM0_FLT3/LPUART2_TX/TRGMUX_OUT3}
  - {pin_num: '6', peripheral: TRGMUX0, signal: 'OUT, 8', pin_signal: TSI0_CH10/PTE10/CLKOUT/FXIO_D4/TRGMUX_OUT4}
  - {pin_num: '5', peripheral: TRGMUX0, signal: 'OUT, 9', pin_signal: TSI0_CH9/PTE11/PWT_IN1/LPTMR0_ALT1/FXIO_D5/TRGMUX_OUT5}
  - {pin_num: '2', peripheral: TRGMUX0, signal: 'OUT, 10', pin_signal: TSI0_CH14/PTE15/FXIO_D2/TRGMUX_OUT6}
  - {pin_num: '1', peripheral: TRGMUX0, signal: 'OUT, 11', pin_signal: TSI0_CH13/PTE16/FXIO_D3/TRGMUX_OUT7}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA8 (pin 100) is configured as TRGMUX_OUT0 */
    PORT_SetPinMux(PORTA, 8U, kPORT_MuxAlt7);

    /* PORTA9 (pin 99) is configured as TRGMUX_OUT1 */
    PORT_SetPinMux(PORTA, 9U, kPORT_MuxAlt7);

    /* PORTB0 (pin 54) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt2);

    /* PORTB1 (pin 53) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt2);

    /* PORTD17 (pin 20) is configured as TRGMUX_OUT2 */
    PORT_SetPinMux(PORTD, 17U, kPORT_MuxAlt7);

    /* PORTE10 (pin 6) is configured as TRGMUX_OUT4 */
    PORT_SetPinMux(PORTE, 10U, kPORT_MuxAlt7);

    /* PORTE11 (pin 5) is configured as TRGMUX_OUT5 */
    PORT_SetPinMux(PORTE, 11U, kPORT_MuxAlt7);

    /* PORTE12 (pin 19) is configured as TRGMUX_OUT3 */
    PORT_SetPinMux(PORTE, 12U, kPORT_MuxAlt7);

    /* PORTE15 (pin 2) is configured as TRGMUX_OUT6 */
    PORT_SetPinMux(PORTE, 15U, kPORT_MuxAlt7);

    /* PORTE16 (pin 1) is configured as TRGMUX_OUT7 */
    PORT_SetPinMux(PORTE, 16U, kPORT_MuxAlt7);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
