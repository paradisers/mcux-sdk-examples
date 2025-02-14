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
board: FRDM-KE17Z
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
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
    BOARD_Init_I2C_GPIO_pins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '54', peripheral: LPUART0, signal: RX, pin_signal: TSI1_CH15/PTB0/LPUART0_RX/LPSPI0_PCS0/LPTMR0_ALT3/PWT_IN3}
  - {pin_num: '53', peripheral: LPUART0, signal: TX, pin_signal: TSI1_CH16/PTB1/LPUART0_TX/LPSPI0_SOUT/TCLK0}
  - {pin_num: '35', peripheral: FTM2, signal: 'CH, 1', pin_signal: PTD11/FTM2_CH1/LPUART2_CTS}
  - {pin_num: '36', peripheral: FTM2, signal: 'CH, 0', pin_signal: PTD10/FTM2_CH0}
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
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTB0 (pin 54) is configured as LPUART0_RX */
    PORT_SetPinMux(BOARD_UART0_RX_PORT, BOARD_UART0_RX_PIN, kPORT_MuxAlt2);

    /* PORTB1 (pin 53) is configured as LPUART0_TX */
    PORT_SetPinMux(BOARD_UART0_TX_PORT, BOARD_UART0_TX_PIN, kPORT_MuxAlt2);

    /* PORTD10 (pin 36) is configured as FTM2_CH0 */
    PORT_SetPinMux(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, kPORT_MuxAlt2);

    /* PORTD11 (pin 35) is configured as FTM2_CH1 */
    PORT_SetPinMux(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_I2C_ConfigurePins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '77', peripheral: LPI2C0, signal: SCL, pin_signal: PTB8/LPI2C0_SCL/FXIO_D4}
  - {pin_num: '82', peripheral: LPI2C0, signal: SDA, pin_signal: PTA16/FTM1_CH3/LPI2C0_SDA}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_I2C_ConfigurePins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_I2C_ConfigurePins(void)
{
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    /* PORTA16 (pin 82) is configured as LPI2C0_SDA */
    PORT_SetPinMux(BOARD_FXOS_SDA_PORT, BOARD_FXOS_SDA_PIN, kPORT_MuxAlt4);

    /* PORTB8 (pin 77) is configured as LPI2C0_SCL */
    PORT_SetPinMux(BOARD_FXOS_SCL_PORT, BOARD_FXOS_SCL_PIN, kPORT_MuxAlt4);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_Init_ACCEL_Reset:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '76', peripheral: GPIOB, signal: 'GPIO, 9', pin_signal: PTB9/LPI2C0_SCLS/FXIO_D5, direction: OUTPUT, gpio_init_state: 'true'}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_Init_ACCEL_Reset
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_Init_ACCEL_Reset(void)
{
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    gpio_pin_config_t FXOS_RST_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTB9 (pin 76)  */
    GPIO_PinInit(BOARD_FXOS_RST_GPIO, BOARD_FXOS_RST_PIN, &FXOS_RST_config);

    /* PORTB9 (pin 76) is configured as PTB9 */
    PORT_SetPinMux(BOARD_FXOS_RST_PORT, BOARD_FXOS_RST_PIN, kPORT_MuxAsGpio);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_Init_I2C_GPIO_pins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '77', peripheral: GPIOB, signal: 'GPIO, 8', pin_signal: PTB8/LPI2C0_SCL/FXIO_D4, direction: OUTPUT, gpio_init_state: 'true'}
  - {pin_num: '82', peripheral: GPIOA, signal: 'GPIO, 16', pin_signal: PTA16/FTM1_CH3/LPI2C0_SDA, direction: OUTPUT, gpio_init_state: 'true'}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_Init_I2C_GPIO_pins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_Init_I2C_GPIO_pins(void)
{
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    gpio_pin_config_t FXOS_SDA_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTA16 (pin 82)  */
    GPIO_PinInit(BOARD_INIT_I2C_GPIO_PINS_FXOS_SDA_GPIO, BOARD_INIT_I2C_GPIO_PINS_FXOS_SDA_PIN, &FXOS_SDA_config);

    gpio_pin_config_t FXOS_SCL_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTB8 (pin 77)  */
    GPIO_PinInit(BOARD_INIT_I2C_GPIO_PINS_FXOS_SCL_GPIO, BOARD_INIT_I2C_GPIO_PINS_FXOS_SCL_PIN, &FXOS_SCL_config);

    /* PORTA16 (pin 82) is configured as PTA16 */
    PORT_SetPinMux(BOARD_INIT_I2C_GPIO_PINS_FXOS_SDA_PORT, BOARD_INIT_I2C_GPIO_PINS_FXOS_SDA_PIN, kPORT_MuxAsGpio);

    /* PORTB8 (pin 77) is configured as PTB8 */
    PORT_SetPinMux(BOARD_INIT_I2C_GPIO_PINS_FXOS_SCL_PORT, BOARD_INIT_I2C_GPIO_PINS_FXOS_SCL_PIN, kPORT_MuxAsGpio);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
