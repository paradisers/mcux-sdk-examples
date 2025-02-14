/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017, 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_hid.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "fsl_device_registers.h"
#include "mouse.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

#include "fsl_power.h"
#include "fsl_adapter_gpio.h"
#include "fsl_adapter_timer.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                               \
    (SYSCON_PDRUNCFG_PDEN_SRAMX_MASK | SYSCON_PDRUNCFG_PDEN_SRAM0_MASK | SYSCON_PDSLEEPCFG_PDEN_SRAM1_2_3_MASK | \
     SYSCON_PDSLEEPCFG_PDEN_USB_RAM_MASK | SYSCON_PDRUNCFG_PDEN_VD6_MASK)
#define APP_EXCLUDE_FROM_DEEPSLEEP1 0U
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                               \
    (SYSCON_PDRUNCFG_PDEN_SRAMX_MASK | SYSCON_PDRUNCFG_PDEN_SRAM0_MASK | SYSCON_PDSLEEPCFG_PDEN_SRAM1_2_3_MASK | \
     SYSCON_PDSLEEPCFG_PDEN_USB_RAM_MASK | SYSCON_PDRUNCFG_PDEN_VD2_ANA_MASK | SYSCON_PDRUNCFG_PDEN_VD5_MASK |   \
     SYSCON_PDRUNCFG_PDEN_VD6_MASK)
#define APP_EXCLUDE_FROM_DEEPSLEEP1 ((SYSCON_PDRUNCFG_PDEN_USB1_PHY_MASK | SYSCON_PDRUNCFG_PDEN_SYSOSC_MASK))
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif

static usb_status_t USB_DeviceHidMouseAction(void);
static usb_status_t USB_DeviceHidInterruptIn(usb_device_handle deviceHandle,
                                             usb_device_endpoint_callback_message_struct_t *event,
                                             void *arg);
static void USB_DeviceApplicationInit(void);

void BOARD_InitPins(void);
void BOARD_DeinitPins(void);
void SW_IntControl(uint8_t enable);
char *SW_GetName(void);
void HW_TimerControl(uint8_t enable);
void USB_LowpowerModeInit(void);
void USB_PreLowpowerMode(void);
uint8_t USB_EnterLowpowerMode(void);
void USB_PostLowpowerMode(void);
void USB_ControllerSuspended(void);
void USB_WaitClockLocked(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;
uint32_t g_halTimerHandle[(HAL_TIMER_HANDLE_SIZE + 3) / 4];
uint32_t g_gpioHandle[(HAL_GPIO_HANDLE_SIZE + 3) / 4];

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_SetupOutBuffer[8];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_MouseBuffer[USB_HID_MOUSE_REPORT_LENGTH];
usb_hid_mouse_struct_t g_UsbDeviceHidMouse;

extern uint8_t g_UsbDeviceCurrentConfigure;
extern uint8_t g_UsbDeviceInterface[USB_HID_MOUSE_INTERFACE_COUNT];

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_DeinitPins(void)
{
}
void SW_IntControl(uint8_t enable)
{
    if (enable)
    {
        g_UsbDeviceHidMouse.selfWakeup = 0U;
    }
    HAL_GpioWakeUpSetting(g_gpioHandle, enable);
}
void SW_Callback(void *param)
{
    g_UsbDeviceHidMouse.selfWakeup = 1U;
    SW_IntControl(0);
}
void SW_Init(void)
{
    hal_gpio_pin_config_t s_GpioInputPin;
    s_GpioInputPin.direction = kHAL_GpioDirectionIn;
    s_GpioInputPin.port      = BOARD_SW2_GPIO_PORT;
    s_GpioInputPin.pin       = BOARD_SW2_GPIO_PIN;

    HAL_GpioInit(g_gpioHandle, &s_GpioInputPin);
    HAL_GpioInstallCallback(g_gpioHandle, SW_Callback, NULL);
    HAL_GpioSetTriggerMode(g_gpioHandle, kHAL_GpioInterruptFallingEdge);
}
char *SW_GetName(void)
{
    return BOARD_SW2_NAME;
}
void HW_TimerCallback(void *param)
{
    g_UsbDeviceHidMouse.hwTick++;
    USB_DeviceUpdateHwTick(g_UsbDeviceHidMouse.deviceHandle, g_UsbDeviceHidMouse.hwTick);
}
void HW_TimerInit(void)
{
    hal_timer_config_t halTimerConfig;
    halTimerConfig.timeout            = 1000;
    halTimerConfig.srcClock_Hz        = TIMER_SOURCE_CLOCK;
    halTimerConfig.instance           = 0U;
    hal_timer_handle_t halTimerHandle = &g_halTimerHandle[0];
    HAL_TimerInit(halTimerHandle, &halTimerConfig);
    HAL_TimerInstallCallback(halTimerHandle, HW_TimerCallback, NULL);
}
void HW_TimerControl(uint8_t enable)
{
    if (enable)
    {
        HAL_TimerEnable(g_halTimerHandle);
    }
    else
    {
        HAL_TimerDisable(g_halTimerHandle);
    }
}
void USB_LowpowerModeInit(void)
{
    SW_Init();
    HW_TimerInit();
}
void USB_PreLowpowerMode(void)
{
    __disable_irq();
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
    if (SYSCON->USB0CLKSTAT & (SYSCON_USB0CLKSTAT_HOST_NEED_CLKST_MASK))
    {
        /* enable usb0 host clock */
        CLOCK_EnableClock(kCLOCK_Usbhmr0);
        CLOCK_EnableClock(kCLOCK_Usbhsl0);
        while (SYSCON->USB0CLKSTAT & (SYSCON_USB0CLKSTAT_HOST_NEED_CLKST_MASK))
        {
            __ASM("nop");
        }
        /* disable usb0 host clock */
        CLOCK_DisableClock(kCLOCK_Usbhsl0);
        CLOCK_DisableClock(kCLOCK_Usbhmr0);
    }
    NVIC_ClearPendingIRQ(USB0_NEEDCLK_IRQn);
    EnableDeepSleepIRQ(USB0_NEEDCLK_IRQn);
    SYSCON->STARTER[0] |= SYSCON_STARTER_USB0_NEEDCLK_MASK;
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    if (SYSCON->USB1CLKSTAT & (SYSCON_USB1CLKSTAT_HOST_NEED_CLKST_MASK))
    {
        /* enable usb1 host clock */
        CLOCK_EnableClock(kCLOCK_Usbh1);
        while (SYSCON->USB1CLKSTAT & (SYSCON_USB1CLKSTAT_HOST_NEED_CLKST_MASK))
        {
            __ASM("nop");
        }
        /* disable usb1 host clock */
        CLOCK_DisableClock(kCLOCK_Usbh1);
    }
    NVIC_ClearPendingIRQ(USB1_NEEDCLK_IRQn);
    EnableDeepSleepIRQ(USB1_NEEDCLK_IRQn);

    SYSCON->STARTER[1] |= SYSCON_STARTER_USB1_ACT_MASK;
    CLOCK_EnableClock(kCLOCK_Usbh1);
    /*According to reference mannual, device mode setting has to be set by access usb host register */
    *((uint32_t *)(USBHSH_BASE + 0x50)) |= (USBHSH_PORTMODE_SW_CTRL_PDCOM_MASK);
    *((uint32_t *)(USBHSH_BASE + 0x50)) |= USBHSH_PORTMODE_SW_PDCOM_MASK;
    CLOCK_DisableClock(kCLOCK_Usbh1);
#endif
#if 0
    CLOCK_AttachClk(
        kFRO12M_to_MAIN_CLK);          /*!< Switch to 12MHz first to ensure we can change voltage without accidentally
                                       being below the voltage for current speed */
    SYSCON->FROCTRL &= ~(SYSCON_FROCTRL_USBCLKADJ_MASK | SYSCON_FROCTRL_HSPDCLK_MASK);
    POWER_SetVoltageForFreq(12000000U); /*!< Set voltage for core */
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
    SYSCON->USB0CLKDIV  = (1 << 29);
    SYSCON->MAINCLKSELA = 0;
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    SYSCON->USB1CLKDIV  = (1 << 29);
    SYSCON->MAINCLKSELA = 0;
#endif
}
uint8_t USB_EnterLowpowerMode(void)
{
    /* Enter Deep Sleep mode */
    POWER_EnterDeepSleep((uint64_t)(APP_EXCLUDE_FROM_DEEPSLEEP | ((uint64_t)APP_EXCLUDE_FROM_DEEPSLEEP1 << 32)));
    return kStatus_Success;
}
void USB_PostLowpowerMode(void)
{
    __enable_irq();
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
    DisableDeepSleepIRQ(USB0_NEEDCLK_IRQn);
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    CLOCK_EnableClock(kCLOCK_Usbh1);
    /*According to reference mannual, device mode setting has to be set by access usb host register */
    *((uint32_t *)(USBHSH_BASE + 0x50)) &= ~USBHSH_PORTMODE_SW_PDCOM_MASK;
    *((uint32_t *)(USBHSH_BASE + 0x50)) &= ~USBHSH_PORTMODE_SW_CTRL_PDCOM_MASK;
    CLOCK_DisableClock(kCLOCK_Usbh1);
    DisableDeepSleepIRQ(USB1_NEEDCLK_IRQn);
#endif
#if 0
    BOARD_InitBootClocks();
#endif
}
void USB_ControllerSuspended(void)
{
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
    while (SYSCON->USB0CLKSTAT & (SYSCON_USB0CLKSTAT_DEV_NEED_CLKST_MASK))
    {
        __ASM("nop");
    }
    SYSCON->USB0CLKCTRL |= SYSCON_USB0CLKCTRL_POL_FS_DEV_CLK_MASK;
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    while (SYSCON->USB1CLKSTAT & (SYSCON_USB1CLKSTAT_DEV_NEED_CLKST_MASK))
    {
        __ASM("nop");
    }
    SYSCON->USB1CLKCTRL |= SYSCON_USB1CLKCTRL_POL_FS_DEV_CLK_MASK;
#endif
}
void USB0_NEEDCLK_IRQHandler(void)
{
}
void USB1_NEEDCLK_IRQHandler(void)
{
}
void USB_WaitClockLocked(void)
{
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
    SYSCON->MAINCLKSELA = 3; /* only applies to BOARD_InitBootClocks */
    SYSCON->USB0CLKDIV  = 1; /* only applies to kCLOCK_UsbSrcFro and BOARD_InitBootClocks */
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    SYSCON->MAINCLKSELA = 3; /* only applies to BOARD_InitBootClocks */
    SYSCON->USB1CLKDIV  = 0;
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    if ((SYSCON->USB1CLKSEL & SYSCON_USB1CLKSEL_SEL_MASK) == SYSCON_USB1CLKSEL_SEL(0x2))
    {
        while (CLOCK_IsUsbPLLLocked() == false)
        {
        }
    }
#endif
}
#if (defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
void USB0_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}
#endif
#if (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
void USB1_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}
#endif
void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    /* enable USB IP clock */
    CLOCK_EnableUsbfs0DeviceClock(kCLOCK_UsbSrcFro, CLOCK_GetFroHfFreq());
#if defined(FSL_FEATURE_USB_USB_RAM) && (FSL_FEATURE_USB_USB_RAM)
    for (int i = 0; i < FSL_FEATURE_USB_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USB_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif

#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    /* enable USB IP clock */
    CLOCK_EnableUsbhs0DeviceClock(kCLOCK_UsbSrcUsbPll, (48000000U));
#if defined(FSL_FEATURE_USBHSD_USB_RAM) && (FSL_FEATURE_USBHSD_USB_RAM)
    for (int i = 0; i < FSL_FEATURE_USBHSD_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USBHSD_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif
#endif
}
void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USB_IRQS;
    irqNumber                    = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Fs0];
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USBHSD_IRQS;
    irqNumber                    = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Hs0];
#endif
    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    USB_DeviceLpcIp3511TaskFunction(deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    USB_DeviceLpcIp3511TaskFunction(deviceHandle);
#endif
}
#endif

/* Update mouse pointer location. Draw a rectangular rotation*/
static usb_status_t USB_DeviceHidMouseAction(void)
{
    static int8_t x = 0U;
    static int8_t y = 0U;
    enum
    {
        RIGHT,
        DOWN,
        LEFT,
        UP
    };
    static uint8_t dir = RIGHT;

    switch (dir)
    {
        case RIGHT:
            /* Move right. Increase X value. */
            g_UsbDeviceHidMouse.buffer[1] = 2U;
            g_UsbDeviceHidMouse.buffer[2] = 0U;
            x++;
            if (x > 99U)
            {
                dir++;
            }
            break;
        case DOWN:
            /* Move down. Increase Y value. */
            g_UsbDeviceHidMouse.buffer[1] = 0U;
            g_UsbDeviceHidMouse.buffer[2] = 2U;
            y++;
            if (y > 99U)
            {
                dir++;
            }
            break;
        case LEFT:
            /* Move left. Discrease X value. */
            g_UsbDeviceHidMouse.buffer[1] = (uint8_t)(-2);
            g_UsbDeviceHidMouse.buffer[2] = 0U;
            x--;
            if (x < 2U)
            {
                dir++;
            }
            break;
        case UP:
            /* Move up. Discrease Y value. */
            g_UsbDeviceHidMouse.buffer[1] = 0U;
            g_UsbDeviceHidMouse.buffer[2] = (uint8_t)(-2);
            y--;
            if (y < 2U)
            {
                dir = RIGHT;
            }
            break;
        default:
            break;
    }
    /* Send mouse report to the host */
    return USB_DeviceSendRequest(g_UsbDeviceHidMouse.deviceHandle, USB_HID_MOUSE_ENDPOINT_IN,
                                 g_UsbDeviceHidMouse.buffer, USB_HID_MOUSE_REPORT_LENGTH);
}

/* HID mouse interrupt IN pipe callback */
static usb_status_t USB_DeviceHidInterruptIn(usb_device_handle deviceHandle,
                                             usb_device_endpoint_callback_message_struct_t *event,
                                             void *arg)
{
    /* Resport sent */
    if (g_UsbDeviceHidMouse.attach)
    {
        /* endpoint callback length is USB_CANCELLED_TRANSFER_LENGTH (0xFFFFFFFFU) when transfer is canceled */
        if ((NULL != event) && (event->length == USB_CANCELLED_TRANSFER_LENGTH))
        {
            return kStatus_USB_Error;
        }
        return USB_DeviceHidMouseAction();
    }

    return kStatus_USB_Error;
}

usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t *temp8     = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            /* USB bus reset signal detected */
            /* Initialize the control IN and OUT pipes */
            USB_DeviceControlPipeInit(g_UsbDeviceHidMouse.deviceHandle);
            g_UsbDeviceHidMouse.attach       = 0U;
            g_UsbDeviceHidMouse.remoteWakeup = 0U;
            g_UsbDeviceHidMouse.suspend      = kStatus_MouseIdle;
            g_UsbDeviceHidMouse.isResume     = 0U;
            error                            = kStatus_USB_Success;
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceGetStatus(g_UsbDeviceHidMouse.deviceHandle, kUSB_DeviceStatusSpeed,
                                                           &g_UsbDeviceHidMouse.speed))
            {
                USB_DeviceSetSpeed(g_UsbDeviceHidMouse.speed);
            }
#endif
        }
        break;
#if (defined(USB_DEVICE_CONFIG_DETACH_ENABLE) && (USB_DEVICE_CONFIG_DETACH_ENABLE > 0U))
        case kUSB_DeviceEventAttach:
        {
            error = kStatus_USB_Success;
#if (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
#else
            /*Add one delay here to make the DP pull down long enough to allow host to detect the previous
             * disconnection.*/
            SDK_DelayAtLeastUs(5000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
            USB_DeviceRun(g_UsbDeviceHidMouse.deviceHandle);
#endif
        }
        break;
        case kUSB_DeviceEventDetach:
        {
            g_UsbDeviceHidMouse.attach = 0;
            error                      = kStatus_USB_Success;
#if (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))

#else
            USB_DeviceStop(g_UsbDeviceHidMouse.deviceHandle);
#endif
        }
        break;
#endif
        case kUSB_DeviceEventSuspend:
        {
            /* USB device bus suspend signal detected */
            if (g_UsbDeviceHidMouse.attach)
            {
                usb_echo("USB device start suspend\r\n");
                USB_ControllerSuspended();
                g_UsbDeviceHidMouse.startTick = g_UsbDeviceHidMouse.hwTick;
                g_UsbDeviceHidMouse.suspend   = kStatus_MouseStartSuspend;
                error                         = kStatus_USB_Success;
            }
        }
        break;
        case kUSB_DeviceEventResume:
        {
            /* USB device bus resume signal detected */
            if ((g_UsbDeviceHidMouse.attach) && (kStatus_MouseIdle != g_UsbDeviceHidMouse.suspend))
            {
                g_UsbDeviceHidMouse.isResume = 1U;
                usb_echo("USB device start resume\r\n");
                error = kStatus_USB_Success;
            }
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (USB_HID_MOUSE_CONFIGURE_INDEX == (*temp8))
            {
                /* If the confguration is valid, initliaze the HID mouse interrupt IN pipe */
                usb_device_endpoint_init_struct_t epInitStruct;
                usb_device_endpoint_callback_struct_t epCallback;

                epCallback.callbackFn    = USB_DeviceHidInterruptIn;
                epCallback.callbackParam = handle;

                epInitStruct.zlt          = 0U;
                epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
                epInitStruct.endpointAddress =
                    USB_HID_MOUSE_ENDPOINT_IN | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
                if (USB_SPEED_HIGH == g_UsbDeviceHidMouse.speed)
                {
                    epInitStruct.maxPacketSize = HS_HID_MOUSE_INTERRUPT_IN_PACKET_SIZE;
                    epInitStruct.interval      = HS_HID_MOUSE_INTERRUPT_IN_INTERVAL;
                }
                else
                {
                    epInitStruct.maxPacketSize = FS_HID_MOUSE_INTERRUPT_IN_PACKET_SIZE;
                    epInitStruct.interval      = FS_HID_MOUSE_INTERRUPT_IN_INTERVAL;
                }

                USB_DeviceInitEndpoint(g_UsbDeviceHidMouse.deviceHandle, &epInitStruct, &epCallback);

                g_UsbDeviceHidMouse.attach = 1U;
                error                      = USB_DeviceHidMouseAction(); /* run the cursor movement code */
            }
            break;
        case kUSB_DeviceEventSetInterface:
            error = kStatus_USB_Success;
            break;
        default:
            break;
    }

    return error;
}

/* Get setup buffer */
usb_status_t USB_DeviceGetSetupBuffer(usb_device_handle handle, usb_setup_struct_t **setupBuffer)
{
    /* Keep the setup is 4-byte aligned */
    static uint32_t hid_mouse_setup[2];
    if (NULL == setupBuffer)
    {
        return kStatus_USB_InvalidParameter;
    }
    *setupBuffer = (usb_setup_struct_t *)&hid_mouse_setup;
    return kStatus_USB_Success;
}

/* Configure device remote wakeup */
usb_status_t USB_DeviceConfigureRemoteWakeup(usb_device_handle handle, uint8_t enable)
{
    if (g_UsbDeviceHidMouse.attach)
    {
        g_UsbDeviceHidMouse.remoteWakeup = enable;
        usb_echo("USB device remote wakeup state: %d\r\n", g_UsbDeviceHidMouse.remoteWakeup);
        return kStatus_USB_Success;
    }
    return kStatus_USB_InvalidRequest;
}

/* Configure the endpoint status (idle or stall) */
usb_status_t USB_DeviceConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    if (status)
    {
        if ((USB_HID_MOUSE_ENDPOINT_IN == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
            (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))
        {
            return USB_DeviceStallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    else
    {
        if ((USB_HID_MOUSE_ENDPOINT_IN == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
            (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))
        {
            return USB_DeviceUnstallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    return kStatus_USB_InvalidRequest;
}

/* Get class-specific request buffer */
usb_status_t USB_DeviceGetClassReceiveBuffer(usb_device_handle handle,
                                             usb_setup_struct_t *setup,
                                             uint32_t *length,
                                             uint8_t **buffer)
{
    if ((NULL == buffer) || ((*length) > sizeof(s_SetupOutBuffer)))
    {
        return kStatus_USB_InvalidRequest;
    }
    *buffer = s_SetupOutBuffer;
    return kStatus_USB_Success;
}

/* Handle class-specific request */
usb_status_t USB_DeviceProcessClassRequest(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;

    if (setup->wIndex != USB_HID_MOUSE_INTERFACE_INDEX)
    {
        return error;
    }

    switch (setup->bRequest)
    {
        case USB_DEVICE_HID_REQUEST_GET_REPORT:
            break;
        case USB_DEVICE_HID_REQUEST_GET_IDLE:
            break;
        case USB_DEVICE_HID_REQUEST_GET_PROTOCOL:
            break;
        case USB_DEVICE_HID_REQUEST_SET_REPORT:
            break;
        case USB_DEVICE_HID_REQUEST_SET_IDLE:
            break;
        case USB_DEVICE_HID_REQUEST_SET_PROTOCOL:
            break;
        default:
            break;
    }

    return error;
}

static void USB_DeviceApplicationInit(void)
{
    USB_DeviceClockInit();
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    /* Set HID mouse default state */
    g_UsbDeviceHidMouse.speed        = USB_SPEED_FULL;
    g_UsbDeviceHidMouse.attach       = 0U;
    g_UsbDeviceHidMouse.deviceHandle = NULL;
    g_UsbDeviceHidMouse.buffer       = s_MouseBuffer;
    g_UsbDeviceHidMouse.remoteWakeup = 0U;
    g_UsbDeviceHidMouse.suspend      = kStatus_MouseIdle;
    g_UsbDeviceHidMouse.selfWakeup   = 0U;
    g_UsbDeviceHidMouse.isResume     = 0U;

    /* Initialize the usb stack and class drivers */
    if (kStatus_USB_Success != USB_DeviceInit(CONTROLLER_ID, USB_DeviceCallback, &g_UsbDeviceHidMouse.deviceHandle))
    {
        usb_echo("USB device mouse failed\r\n");
        return;
    }
    else
    {
        usb_echo("USB device HID mouse demo\r\n");
    }

    /* Install isr, set priority, and enable IRQ. */
    USB_DeviceIsrEnable();

    /* Start USB device HID mouse */
    /*Add one delay here to make the DP pull down long enough to allow host to detect the previous disconnection.*/
    SDK_DelayAtLeastUs(5000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    USB_DeviceRun(g_UsbDeviceHidMouse.deviceHandle);
}

void USB_PowerPreSwitchHook(void)
{
    HW_TimerControl(0U);

    DbgConsole_Deinit();

    BOARD_DeinitPins();

    USB_PreLowpowerMode();
}

void USB_PowerPostSwitchHook(void)
{
    USB_WaitClockLocked();
    USB_PostLowpowerMode();
    BOARD_InitPins();
    BOARD_InitDebugConsole();
    HW_TimerControl(1U);
}

void USB_DeviceSuspendResumeTask(void)
{
    if (g_UsbDeviceHidMouse.isResume)
    {
        g_UsbDeviceHidMouse.isResume = 0U;
        if (kStatus_MouseIdle != g_UsbDeviceHidMouse.suspend)
        {
            g_UsbDeviceHidMouse.suspend = kStatus_MouseResumed;
        }
    }

    switch (g_UsbDeviceHidMouse.suspend)
    {
        case kStatus_MouseIdle:
            break;
        case kStatus_MouseStartSuspend:
            g_UsbDeviceHidMouse.suspend = kStatus_MouseSuspending;
            break;
        case kStatus_MouseSuspending:
            usb_echo("USB device suspended.\r\n");
            if (g_UsbDeviceHidMouse.remoteWakeup)
            {
                usb_echo("Please Press wakeup switch(%s) to remote wakeup the host.\r\n", SW_GetName());
            }
            g_UsbDeviceHidMouse.suspend = kStatus_MouseSuspended;
            break;
        case kStatus_MouseSuspended:
            USB_PowerPreSwitchHook();
            if (g_UsbDeviceHidMouse.remoteWakeup)
            {
                SW_IntControl(1);
            }

            USB_DeviceSetStatus(g_UsbDeviceHidMouse.deviceHandle, kUSB_DeviceStatusBusSuspend, NULL);
            if (kStatus_Success != USB_EnterLowpowerMode())
            {
                g_UsbDeviceHidMouse.selfWakeup = 1U;
                USB_PowerPostSwitchHook();
                usb_echo("Enter VLPS mode failed!\r\n");
            }
            else
            {
                USB_PowerPostSwitchHook();
            }
            if (g_UsbDeviceHidMouse.remoteWakeup)
            {
                SW_IntControl(0);
            }

            if (g_UsbDeviceHidMouse.attach)
            {
                g_UsbDeviceHidMouse.suspend = kStatus_MouseStartResume;
            }
            else
            {
                g_UsbDeviceHidMouse.suspend = kStatus_MouseIdle;
            }
            break;
        case kStatus_MouseStartResume:
            if (g_UsbDeviceHidMouse.selfWakeup)
            {
                g_UsbDeviceHidMouse.selfWakeup = 0U;
                if (g_UsbDeviceHidMouse.remoteWakeup)
                {
                    if (kStatus_USB_Success ==
                        USB_DeviceSetStatus(g_UsbDeviceHidMouse.deviceHandle, kUSB_DeviceStatusBusResume, NULL))
                    {
                        usb_echo("Remote wakeup the host.\r\n");
                        g_UsbDeviceHidMouse.suspend = kStatus_MouseResuming;
                    }
                    else
                    {
                        usb_echo("Send resume signal failed.\r\n");
                        g_UsbDeviceHidMouse.suspend = kStatus_MouseStartResume;
                    }
                }
                else
                {
                    g_UsbDeviceHidMouse.suspend = kStatus_MouseResuming;
                }
            }
            else
            {
                g_UsbDeviceHidMouse.suspend = kStatus_MouseResumed;
            }
            break;
        case kStatus_MouseResuming:
            break;
        case kStatus_MouseResumed:
            usb_echo("USB device resumed.\r\n");
            if (g_UsbDeviceHidMouse.attach)
            {
                USB_DeviceHidMouseAction();
            }
            g_UsbDeviceHidMouse.suspend = kStatus_MouseIdle;
            break;
        default:
            g_UsbDeviceHidMouse.suspend = kStatus_MouseIdle;
            break;
    }
}

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* reset USB0 and USB1 device */
    RESET_PeripheralReset(kUSB0D_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1D_RST_SHIFT_RSTn);

    NVIC_ClearPendingIRQ(USB0_IRQn);
    NVIC_ClearPendingIRQ(USB0_NEEDCLK_IRQn);
    NVIC_ClearPendingIRQ(USB1_IRQn);
    NVIC_ClearPendingIRQ(USB1_NEEDCLK_IRQn);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
#if (defined USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS)
    POWER_DisablePD(kPDRUNCFG_PD_USB1_PHY);
    /* enable USB IP clock */
    CLOCK_EnableUsbhs0DeviceClock(kCLOCK_UsbSrcUsbPll, (48000000U));
    /* enable usb1 host clock */
    CLOCK_EnableClock(kCLOCK_Usbh1);
    *((uint32_t *)(USBHSH_BASE + 0x50)) |= USBHSH_PORTMODE_DEV_ENABLE_MASK;
    *((uint32_t *)(USBHSH_BASE + 0x50)) &= ~USBHSH_PORTMODE_SW_CTRL_PDCOM_MASK;
    while (SYSCON->USB1CLKSTAT & (SYSCON_USB1CLKSTAT_HOST_NEED_CLKST_MASK))
    {
        __ASM("nop");
    }
    /*According to reference mannual, device mode setting has to be set by access usb host register */
    /* enable usb1 host clock */
    CLOCK_DisableClock(kCLOCK_Usbh1);
#endif
#if (defined USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS)
    POWER_DisablePD(kPDRUNCFG_PD_USB0_PHY); /*< Turn on USB Phy */
    CLOCK_SetClkDiv(kCLOCK_DivUsb0Clk, 1, false);
    CLOCK_AttachClk(kFRO_HF_to_USB0_CLK);
    /* enable usb0 host clock */
    CLOCK_EnableClock(kCLOCK_Usbhmr0);
    CLOCK_EnableClock(kCLOCK_Usbhsl0);
    /*According to reference mannual, device mode setting has to be set by access usb host register */
    *((uint32_t *)(USBFSH_BASE + 0x5C)) |= USBFSH_PORTMODE_DEV_ENABLE_MASK;
    while (SYSCON->USB0CLKSTAT & (SYSCON_USB0CLKSTAT_HOST_NEED_CLKST_MASK))
    {
        __ASM("nop");
    }
    /* disable usb0 host clock */
    CLOCK_DisableClock(kCLOCK_Usbhsl0);
    CLOCK_DisableClock(kCLOCK_Usbhmr0);
#endif

#if ((defined(USB_DEVICE_CONFIG_LOW_POWER_MODE)) && (USB_DEVICE_CONFIG_LOW_POWER_MODE > 0U))
    USB_LowpowerModeInit();
#endif

    USB_DeviceApplicationInit();

#if ((defined(USB_DEVICE_CONFIG_LOW_POWER_MODE)) && (USB_DEVICE_CONFIG_LOW_POWER_MODE > 0U))
    HW_TimerControl(1);
#endif

    while (1U)
    {
#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(g_UsbDeviceHidMouse.deviceHandle);
#endif
        USB_DeviceSuspendResumeTask();
    }
}
