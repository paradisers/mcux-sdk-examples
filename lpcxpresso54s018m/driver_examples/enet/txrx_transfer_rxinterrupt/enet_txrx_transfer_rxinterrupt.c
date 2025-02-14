/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdlib.h>
#include <string.h>
#include "fsl_debug_console.h"
#include "fsl_enet.h"
#include "fsl_phy.h"
#include "fsl_silicon_id.h"

#include "fsl_phylan8720a.h"
#include "pin_mux.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
extern phy_lan8720a_resource_t g_phy_resource;
#define EXAMPLE_ENET_BASE    ENET
#define EXAMPLE_PHY_ADDRESS  0x00U
#define EXAMPLE_PHY_OPS      &phylan8720a_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_resource
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define ENET_RXBD_NUM               (4)
#define ENET_TXBD_NUM               (4)
#define ENET_RXBUFF_SIZE            (ENET_FRAME_MAX_FRAMELEN)
#define ENET_BuffSizeAlign(n)       ENET_ALIGN(n, ENET_BUFF_ALIGNMENT)
#define ENET_ALIGN(x, align)        ((unsigned int)((x) + ((align)-1)) & (unsigned int)(~(unsigned int)((align)-1)))
#define ENET_EXAMPLE_FRAME_HEADSIZE (14U)
#define ENET_EXAMPLE_DATA_LENGTH    (1000U)
#define ENET_EXAMPLE_FRAME_SIZE     (ENET_EXAMPLE_DATA_LENGTH + ENET_EXAMPLE_FRAME_HEADSIZE)
#define ENET_EXAMPLE_PACKAGETYPE    (4U)
#define ENET_EXAMPLE_LOOP_COUNT     (20U)
#ifndef PHY_AUTONEGO_TIMEOUT_COUNT
#define PHY_AUTONEGO_TIMEOUT_COUNT (500000U)
#endif

#if defined(__GNUC__)
#ifndef __ALIGN_END
#define __ALIGN_END __attribute__((aligned(ENET_BUFF_ALIGNMENT)))
#endif
#ifndef __ALIGN_BEGIN
#define __ALIGN_BEGIN
#endif
#else
#ifndef __ALIGN_END
#define __ALIGN_END
#endif
#ifndef __ALIGN_BEGIN
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define __ALIGN_BEGIN __attribute__((aligned(ENET_BUFF_ALIGNMENT)))
#elif defined(__ICCARM__)
#define __ALIGN_BEGIN
#endif
#endif
#endif

/* @TEST_ANCHOR */

#ifndef MAC_ADDRESS
#define MAC_ADDRESS                        \
    {                                      \
        0x54, 0x27, 0x8d, 0x00, 0x00, 0x00 \
    }
#else
#define USER_DEFINED_MAC_ADDRESS
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ENET_BuildBroadCastFrame(void);
static void ENET_RXIRQHandler(uint8_t channel);

/*******************************************************************************
 * Variables
 ******************************************************************************/
phy_lan8720a_resource_t g_phy_resource;
#if defined(__ICCARM__)
#pragma data_alignment = ENET_BUFF_ALIGNMENT
#endif
__ALIGN_BEGIN enet_rx_bd_struct_t g_rxBuffDescrip[ENET_RXBD_NUM] __ALIGN_END;
#if defined(__ICCARM__)
#pragma data_alignment = ENET_BUFF_ALIGNMENT
#endif
__ALIGN_BEGIN enet_tx_bd_struct_t g_txBuffDescrip[ENET_TXBD_NUM] __ALIGN_END;

static enet_handle_t g_handle = {0};
static enet_tx_reclaim_info_t g_txDirty[ENET_TXBD_NUM];
static volatile bool tx_frame_over = false;

/* The MAC address for ENET device. */
static uint8_t g_macAddr[6] = MAC_ADDRESS;
static uint8_t g_frame[ENET_EXAMPLE_PACKAGETYPE][ENET_EXAMPLE_FRAME_SIZE];
static uint32_t g_txIdx      = 0;
static uint32_t g_testIdx    = 0;
static uint32_t g_rxIndex    = 0;
static uint32_t g_rxCheckIdx = 0;

/*! @brief Enet PHY and MDIO interface handler. */
static phy_handle_t phyHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET_BASE)]);
    ENET_SetSMI(EXAMPLE_ENET_BASE);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET_BASE, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET_BASE, phyAddr, regAddr, pData);
}


void ENET_IntCallback(ENET_Type *base,
                      enet_handle_t *handle,
                      enet_event_t event,
                      uint8_t channel,
                      enet_tx_reclaim_info_t *txReclaimInfo,
                      void *param)
{
    switch (event)
    {
        case kENET_TxIntEvent:
            /* Get frame info after whole frame transmits out */
            if (txReclaimInfo != NULL)
            {
                /* Free tx buffers. */
                free((*txReclaimInfo).context);
                tx_frame_over = true;
            }
            break;
        case kENET_RxIntEvent:
            ENET_RXIRQHandler(channel);
        default:
            break;
    }
}

int main(void)
{
    uint32_t count = 0;
    bool link      = false;
    bool autonego  = false;
    uint32_t rxbuffer[ENET_RXBD_NUM];
    phy_config_t phyConfig;
    enet_config_t config;
    phy_speed_t speed;
    phy_duplex_t duplex;
    status_t status;
    uint8_t *buffer;
    uint8_t index;
    void *buff;

    for (index = 0; index < ENET_RXBD_NUM; index++)
    {
        /* This is for rx buffers, static alloc and dynamic alloc both ok. use as your wish. */
        buff = malloc(ENET_RXBUFF_SIZE);
        if (buff)
        {
            rxbuffer[index] = (uint32_t)buff;
        }
        else
        {
            PRINTF("Mem Alloc fail\r\n");
        }
    }

    /* prepare the buffer configuration. */
    enet_buffer_config_t buffConfig[1] = {{
        ENET_RXBD_NUM,
        ENET_TXBD_NUM,
        &g_txBuffDescrip[0],
        &g_txBuffDescrip[0],
        &g_txDirty[0],
        &g_rxBuffDescrip[0],
        &g_rxBuffDescrip[ENET_RXBD_NUM],
        &rxbuffer[0],
        ENET_BuffSizeAlign(ENET_RXBUFF_SIZE),
    }};

    /* Hardware Initialization. */
    CLOCK_EnableClock(kCLOCK_InputMux);
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_BootClockPLL180M();
    BOARD_InitDebugConsole();

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    PRINTF("\r\n ENET example start.\r\n");

    phyConfig.phyAddr  = EXAMPLE_PHY_ADDRESS;
    phyConfig.ops      = EXAMPLE_PHY_OPS;
    phyConfig.resource = EXAMPLE_PHY_RESOURCE;
    phyConfig.autoNeg  = true;

    /* Initialize PHY and wait auto-negotiation over. */
    PRINTF("Wait for PHY init...\r\n");
    do
    {
        status = PHY_Init(&phyHandle, &phyConfig);
        if (status == kStatus_Success)
        {
            PRINTF("Wait for PHY link up...\r\n");
            /* Wait for auto-negotiation success and link up */
            count = PHY_AUTONEGO_TIMEOUT_COUNT;
            do
            {
                PHY_GetAutoNegotiationStatus(&phyHandle, &autonego);
                PHY_GetLinkStatus(&phyHandle, &link);
                if (autonego && link)
                {
                    break;
                }
            } while (--count);
            if (!autonego)
            {
                PRINTF("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
            }
        }
    } while (!(link && autonego));

#ifndef USER_DEFINED_MAC_ADDRESS
    /* Set special address for each chip. */
    SILICONID_ConvertToMacAddr(&g_macAddr);
#endif

    /* Get default configuration 100M RMII. */
    ENET_GetDefaultConfig(&config);

    /* Use the actual speed and duplex when phy success to finish the autonegotiation. */
    PHY_GetLinkSpeedDuplex(&phyHandle, &speed, &duplex);
    config.miiSpeed  = (enet_mii_speed_t)speed;
    config.miiDuplex = (enet_mii_duplex_t)duplex;
    config.interrupt = (kENET_DmaRx) | (kENET_DmaTx);

    /* Initialize ENET. */
    ENET_Init(EXAMPLE_ENET_BASE, &config, &g_macAddr[0], 50000000);

    /* Initialize Descriptor. */
    ENET_DescriptorInit(EXAMPLE_ENET_BASE, &config, &buffConfig[0]);

    /* Create the handler. */
    ENET_CreateHandler(EXAMPLE_ENET_BASE, &g_handle, &config, &buffConfig[0], ENET_IntCallback, NULL);
    /* Active TX/RX. */
    ENET_StartRxTx(EXAMPLE_ENET_BASE, 1, 1);

    /* Build broadcast for sending and active for receiving. */
    ENET_BuildBroadCastFrame();

    PRINTF("\r\nTransmission start now!\r\n");
    while (1)
    {
        /* Check the total number of received number. */
        if (g_rxCheckIdx != g_rxIndex)
        {
            g_rxCheckIdx = g_rxIndex;
            PRINTF("%d frame has been successfully received\r\n", g_rxCheckIdx);
            if (g_rxCheckIdx == ENET_EXAMPLE_LOOP_COUNT)
            {
                break;
            }
        }

        if (g_testIdx < ENET_EXAMPLE_LOOP_COUNT)
        {
            /* Send a multicast frame when the PHY is link up. */
            PHY_GetLinkStatus(&phyHandle, &link);
            if (link)
            {
                /* Create the frame to be send. */
                buffer = (uint8_t *)malloc(ENET_EXAMPLE_FRAME_SIZE);
                if (buffer)
                {
                    memcpy(buffer, &g_frame[g_txIdx], ENET_EXAMPLE_FRAME_SIZE);

                    enet_buffer_struct_t txBuff;
                    enet_tx_frame_struct_t txFrame = {0};

                    txBuff.buffer              = buffer;
                    txBuff.length              = ENET_EXAMPLE_FRAME_SIZE;
                    txFrame.txBuffArray        = &txBuff;
                    txFrame.txBuffNum          = 1;
                    txFrame.txConfig.intEnable = true;
                    txFrame.txConfig.tsEnable  = true;
                    txFrame.context            = buffer;

                    tx_frame_over = false;
                    status        = ENET_SendFrame(EXAMPLE_ENET_BASE, &g_handle, &txFrame, 0);
                    while (status == kStatus_ENET_TxFrameBusy)
                    {
                        status = ENET_SendFrame(EXAMPLE_ENET_BASE, &g_handle, &txFrame, 0);
                    }

                    if (kStatus_Success == status)
                    {
                        /* Wait for Tx over then check timestamp */
                        while (!tx_frame_over)
                        {
                        }

                        g_testIdx++;
                        /* Make each transmit different.*/
                        g_txIdx = (g_txIdx + 1) % ENET_EXAMPLE_PACKAGETYPE;
                        PRINTF("The %d frame transmitted success!\r\n", g_testIdx);
                    }
                }
                else
                {
                    PRINTF("No avail tx buffers\r\n");
                }
            }
            else
            {
                PRINTF(" \r\nThe PHY link down!\r\n");
            }
        }
    }

    ENET_Deinit(EXAMPLE_ENET_BASE);
    while (1)
    {
    }
}

/*! @brief Build Frame for transmit. */
static void ENET_BuildBroadCastFrame(void)
{
    uint32_t count = 0;
    uint32_t index = 0;

    /* Create different transmit frames. */
    for (index = 0; index < ENET_EXAMPLE_PACKAGETYPE; index++)
    {
        for (count = 0; count < 6U; count++)
        {
            g_frame[index][count] = 0xFFU;
        }

        memcpy(&g_frame[index][6], &g_macAddr[0], 6U);
        g_frame[index][12] = (ENET_EXAMPLE_DATA_LENGTH >> 8) & 0xFFU;
        g_frame[index][13] = ENET_EXAMPLE_DATA_LENGTH & 0xFFU;

        for (count = ENET_EXAMPLE_FRAME_HEADSIZE; count < ENET_EXAMPLE_FRAME_SIZE; count++)
        {
            g_frame[index][count] = (count + index) % 0xFFU;
        }
    }
}

static void ENET_RXIRQHandler(uint8_t channel)
{
    uint32_t length = 0;
    status_t status;

    /* Get the Frame size */
    do
    {
        status = ENET_GetRxFrameSize(EXAMPLE_ENET_BASE, &g_handle, &length, channel);
        /* Call ENET_ReadFrame when there is a received frame. */
        if ((status == kStatus_Success) && (length != 0))
        {
            /* Received valid frame. Deliver the rx buffer with the size equal to length. */
            uint8_t *data = (uint8_t *)malloc(length);
            if (data)
            {
                status = ENET_ReadFrame(EXAMPLE_ENET_BASE, &g_handle, data, length, channel, NULL);
                if (status == kStatus_Success)
                {
                    /* Increase the received frame numbers. */
                    if (g_rxIndex < ENET_EXAMPLE_LOOP_COUNT)
                    {
                        g_rxIndex++;
                    }
                    /* Process the data as you wish. */
                }
                free(data);
            }
            else
            {
                /* discard due to the lack of the buffers.*/
                ENET_ReadFrame(EXAMPLE_ENET_BASE, &g_handle, NULL, 0, channel, NULL);
            }
        }
        else if (status == kStatus_ENET_RxFrameError)
        {
            /* update the receive buffer. */
            ENET_ReadFrame(EXAMPLE_ENET_BASE, &g_handle, NULL, 0, channel, NULL);
        }
    } while (status != kStatus_ENET_RxFrameEmpty);
    SDK_ISR_EXIT_BARRIER;
}
