/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_dac.h"

#include "fsl_power.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_DAC_BASEADDR DAC0
#define DEMO_DAC_VREF     kDAC_ReferenceVoltageSourceAlt1

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t index;
    dac_config_t dacConfigStruct;
    uint32_t dacValue;

    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u, true);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_BootClockPLL150M();
    BOARD_InitDebugConsole();

    CLOCK_SetClkDiv(kCLOCK_DivDac0Clk, 1U, true);
    CLOCK_AttachClk(kMAIN_CLK_to_DAC0);

    /* Disable DAC0 power down */
    POWER_DisablePD(kPDRUNCFG_PD_DAC0);

    PRINTF("\r\nDAC Basic Example.\r\n");

    /* Configure the DAC. */
    DAC_GetDefaultConfig(&dacConfigStruct);
    dacConfigStruct.referenceVoltageSource = DEMO_DAC_VREF;
    DAC_Init(DEMO_DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DEMO_DAC_BASEADDR, true); /* Enable the logic and output. */

    while (1)
    {
        index    = 0;
        dacValue = 0;
        PRINTF("\r\nPlease input a value (0 - 4095) to output with DAC: ");
        while (index != 0x0D)
        {
            index = GETCHAR();
            if ((index >= '0') && (index <= '9'))
            {
                PUTCHAR(index);
                dacValue = dacValue * 10 + (index - 0x30U);
            }
        }
        PRINTF("\r\nInput value is %d\r\n", dacValue);
        if (dacValue > 0xFFFU)
        {
            PRINTF("Your value is output of range.\r\n");
            continue;
        }
        DAC_SetData(DEMO_DAC_BASEADDR, dacValue);
        PRINTF("DAC out: %d\r\n", dacValue);
        /*
         * The value in the first item would be converted. User can measure the output voltage from DACx_OUT pin.
         */
    }
}
