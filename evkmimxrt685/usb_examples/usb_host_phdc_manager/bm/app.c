/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_host_config.h"
#include "usb_host.h"
#include "fsl_device_registers.h"
#include "usb_host_phdc.h"
#include "fsl_debug_console.h"
#include "usb.h"
#include "ieee11073_types.h"
#include "ieee11073_nomenclature.h"
#include "host_phdc_manager.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */
#include "app.h"

#if ((!USB_HOST_CONFIG_KHCI) && (!USB_HOST_CONFIG_EHCI) && (!USB_HOST_CONFIG_OHCI) && (!USB_HOST_CONFIG_IP3516HS))
#error Please enable USB_HOST_CONFIG_KHCI, USB_HOST_CONFIG_EHCI, USB_HOST_CONFIG_OHCI, or USB_HOST_CONFIG_IP3516HS in file usb_host_config.
#endif

#include "usb_phy.h"
#include "fsl_power.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief application initialization.
 * This function is the entry for the application (or other usage)
 *
 * @return None.
 */
static void USB_HostApplicationInit(void);

/*!
 * @brief host callback function.
 *
 * device attach/detach callback function.
 *
 * @param deviceHandle           device handle.
 * @param configurationHandle    attached device's configuration descriptor information.
 * @param eventCode              callback event code, please reference to enumeration host_event_t.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_NotSupported         The application don't support the configuration.
 */
static usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                                  usb_host_configuration_handle configurationHandle,
                                  uint32_t eventCode);
extern void USB_HostClockInit(void);
extern void USB_HostIsrEnable(void);
extern void USB_HostTaskFn(void *param);
extern void BOARD_InitHardware(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief host handle variable */
usb_host_handle hostHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

void USB_IRQHandler(void)
{
    USB_HostIp3516HsIsrFunction(hostHandle);
}

void USB_HostClockInit(void)
{
    uint8_t usbClockDiv = 1;
    uint32_t usbClockFreq;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
    /* enable USB IP clock */
    CLOCK_SetClkDiv(kCLOCK_DivPfc1Clk, 5);
    CLOCK_AttachClk(kXTALIN_CLK_to_USB_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivUsbHsFclk, usbClockDiv);
    CLOCK_EnableUsbhsHostClock();
    RESET_PeripheralReset(kUSBHS_PHY_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSBHS_DEVICE_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSBHS_HOST_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSBHS_SRAM_RST_SHIFT_RSTn);
    /*Make sure USDHC ram buffer has power up*/
    POWER_DisablePD(kPDRUNCFG_APD_USBHS_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_USBHS_SRAM);
    POWER_ApplyPD();

    /* save usb ip clock freq*/
    usbClockFreq = g_xtalFreq / usbClockDiv;
    /* enable USB PHY PLL clock, the phy bus clock (480MHz) source is same with USB IP */
    CLOCK_EnableUsbHs0PhyPllClock(kXTALIN_CLK_to_USB_CLK, usbClockFreq);

#if ((defined FSL_FEATURE_USBHSH_USB_RAM) && (FSL_FEATURE_USBHSH_USB_RAM > 0U))

    for (int i = 0; i < (FSL_FEATURE_USBHSH_USB_RAM >> 2); i++)
    {
        ((uint32_t *)FSL_FEATURE_USBHSH_USB_RAM_BASE_ADDRESS)[i] = 0U;
    }
#endif
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL_SYS_CLK_HZ, &phyConfig);

    CLOCK_EnableClock(kCLOCK_UsbhsDevice);
    USBHSH->PORTMODE &= ~USBHSH_PORTMODE_DEV_ENABLE_MASK;
    while (SYSCTL0->USBCLKSTAT & SYSCTL0_USBCLKSTAT_DEV_NEED_CLKST_MASK)
    {
        __ASM("nop");
    }
    /* disable usb1 device clock */
    CLOCK_DisableClock(kCLOCK_UsbhsDevice);
}
void USB_HostIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbHOSTEhciIrq[] = USBHSH_IRQS;
    irqNumber                = usbHOSTEhciIrq[CONTROLLER_ID - kUSB_ControllerIp3516Hs0];
    /* USB_HOST_CONFIG_EHCI */

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);

    EnableIRQ((IRQn_Type)irqNumber);
}
void USB_HostTaskFn(void *param)
{
#if ((defined USB_HOST_CONFIG_IP3516HS) && (USB_HOST_CONFIG_IP3516HS > 0U))
    USB_HostIp3516HsTaskFunction(param);
#endif /* USB_HOST_CONFIG_IP3516HS */
}

/*!
 * @brief USB Interrupt service routine.
 * This function serves as the USB interrupt service routine.
 *
 * @return None.
 */

/*!
 * @brief host callback function.
 *
 * device attach/detach callback function.
 *
 * @param deviceHandle           device handle.
 * @param configurationHandle    attached device's configuration descriptor information.
 * @param eventCode              callback event code, please reference to enumeration host_event_t.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_NotSupported         The application don't support the configuration.
 */
static usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                                  usb_host_configuration_handle configurationHandle,
                                  uint32_t eventCode)
{
    usb_status_t status = kStatus_USB_Success;
    switch (eventCode & 0x0000FFFFU)
    {
        case kUSB_HostEventAttach:
            status = HOST_PhdcManagerEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventNotSupported:
            usb_echo("device not supported.\r\n");
            break;

        case kUSB_HostEventEnumerationDone:
            status = HOST_PhdcManagerEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventDetach:
            status = HOST_PhdcManagerEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventEnumerationFail:
            usb_echo("enumeration failed\r\n");
            break;

        default:
            break;
    }
    return status;
}

/*!
 * @brief application initialization.
 * This function is the entry for the application (or other usage)
 *
 * @return None.
 */
static void USB_HostApplicationInit(void)
{
    usb_status_t status = kStatus_USB_Success;

    USB_HostClockInit();

#if ((defined FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    status = USB_HostInit(CONTROLLER_ID, &hostHandle, USB_HostEvent);
    if (status != kStatus_USB_Success)
    {
        usb_echo("host init error\r\n");
        return;
    }
    USB_HostIsrEnable();

    usb_echo("host init done\r\n");
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    USB_HostApplicationInit();

    while (1U)
    {
        USB_HostTaskFn(hostHandle);
        HOST_PhdcManagerTask(&g_phdcManagerInstance);
    }
}
