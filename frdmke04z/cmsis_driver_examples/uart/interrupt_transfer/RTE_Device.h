/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void UART0_InitPins();
extern void UART0_DeinitPins();

/* Driver name mapping. */
/* User needs to provide the implementation of UARTX_GetFreq/UARTX_InitPins/UARTX_DeinitPins for the enabled UART
 * instance. */
#define RTE_USART0            1
#define RTE_USART0_PIN_INIT   UART0_InitPins
#define RTE_USART0_PIN_DEINIT UART0_DeinitPins

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART0_RX_BUFFER_ENABLE 1

#endif /* _RTE_DEVICE_H */
