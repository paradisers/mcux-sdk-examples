/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_cache.h"
/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_FLEXSPI           FLEXSPI2
#define FLASH_SIZE                0x1000 /* 32Mb/KByte */
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI2_AMBA_BASE
#define FLASH_PAGE_SIZE           256
#define EXAMPLE_SECTOR            10
#define SECTOR_SIZE               0x1000 /* 4K */
#define EXAMPLE_FLEXSPI_CLOCK     kCLOCK_FlexSpi2

#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL        0
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST          1
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD     2
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS         3
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE        4
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR        5
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE 6
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD   7
#define NOR_CMD_LUT_SEQ_IDX_READID             8
#define NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG     9
#define NOR_CMD_LUT_SEQ_IDX_ENTERQPI           10
#define NOR_CMD_LUT_SEQ_IDX_EXITQPI            11
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG      12
#define NOR_CMD_LUT_SEQ_IDX_ERASECHIP          13

#define CUSTOM_LUT_LENGTH        60
#define FLASH_QUAD_ENABLE        0x02
#define FLASH_BUSY_STATUS_POL    1
#define FLASH_BUSY_STATUS_OFFSET 0
#define FLASH_ERROR_STATUS_MASK  0x0e

/* DMA related. */
#define EXAMPLE_FLEXSPI_DMA_MUX (DMAMUX)
#define EXAMPLE_FLEXSPI_DMA     (DMA0)

#define FLEXSPI_TX_DMA_REQUEST_SOURCE kDmaRequestMuxFlexSPI2Tx
#define FLEXSPI_RX_DMA_REQUEST_SOURCE kDmaRequestMuxFlexSPI2Rx

#define FLEXSPI_TX_DMA_CHANNEL 0U
#define FLEXSPI_RX_DMA_CHANNEL 1U

#define CACHE_MAINTAIN 1

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
static inline void flexspi_clock_init(void)
{
#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x0); /* Choose PLL2 PFD2 clock as flexspi2 source clock. 396M */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 2);   /* flexspi clock 133M. */
#else
    const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};
    CLOCK_InitUsb1Pll(&g_ccmConfigUsbPll);
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 24);   /* Set PLL3 PFD0 clock 360MHZ. */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x0); /* Choose PLL3 PFD0 clock as flexspi2 source clock. */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 2);   /* flexspi clock 120M. */
#endif
}
/*${prototype:end}*/

#endif /* _APP_H_ */
