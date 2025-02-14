/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "pin_mux.h"
#include "board.h"
#include "fsl_rng.h"
#include "fsl_debug_console.h"

#include <stdbool.h>
#include "fsl_clock.h"
#include "fsl_power.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define RNG_EXAMPLE_RANDOM_NUMBERS     (4U)
#define RNG_EXAMPLE_RANDOM_BYTES       (16U)
#define RNG_EXAMPLE_RANDOM_NUMBER_BITS (RNG_EXAMPLE_RANDOM_NUMBERS * 8U * sizeof(uint32_t))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief RNG example.
 * @details Generate random numbers and print them to terminal.
 */

int main(void)
{
    uint32_t i;
    uint32_t data[RNG_EXAMPLE_RANDOM_NUMBERS];
    status_t status = kStatus_Fail;

    /* Init hardware*/
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* The clock & power for the random number generator are taken care by the ROM API */
    /*
     * POWER_DisablePD(kPDRUNCFG_PD_rng);
     * CLOCK_EnableClock(kCLOCK_Rng);
     * RESET_PeripheralReset(kRNG_RST_SHIFT_RSTn);
     */

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    PRINTF("RNG Peripheral Driver Example\r\n\r\n");

    while (1)
    {
        PRINTF("Generate %u-bit random number: \r\n", RNG_EXAMPLE_RANDOM_NUMBER_BITS);

        /* Get Random data */

        status = RNG_GetRandomData(&data, RNG_EXAMPLE_RANDOM_BYTES);
        if (status == kStatus_Success)
        {
            /* Print data */
            PRINTF("0x");
            for (i = 0; i < RNG_EXAMPLE_RANDOM_NUMBERS; i++)
            {
                PRINTF("%08X", data[i]);
            }
            PRINTF("\r\n");
        }
        else
        {
            PRINTF("RNG failed!\r\n");
        }

        /* Print a note */
        PRINTF(" Press any key to continue... \r\n\r\n");
        GETCHAR();
    }
}
