/*
 * @brief Configuration file needed for USB ROM stack based applications.
 *
 * @note
 * Copyright  2013, NXP
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */
#include "lpc_types.h"
#include "error.h"

#ifndef __APP_USB_CFG_H_
#define __APP_USB_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @ingroup EXAMPLES_USBDROM_18XX43XX_MSC_RAM
 * @{
 */

/* Comment below and uncomment USE_USB1 to enable USB1 */
#define USE_USB0
/* #define USE_USB1 */

/* Manifest constants used by USBD ROM stack. These values SHOULD NOT BE CHANGED
   for advance features which require usage of USB_CORE_CTRL_T structure.
   Since these are the values used for compiling USB stack.
 */
#define USB_MAX_IF_NUM         8    /*!< Max interface number used for building USBD ROM. DON'T CHANGE. */
#define USB_MAX_EP_NUM         6    /*!< Max number of EP used for building USBD ROM. DON'T CHANGE. */
#define USB_MAX_PACKET0        64   /*!< Max EP0 packet size used for building USBD ROM. DON'T CHANGE. */
#define USB_FS_MAX_BULK_PACKET 64   /*!< MAXP for FS bulk EPs used for building USBD ROM. DON'T CHANGE. */
#define USB_HS_MAX_BULK_PACKET 512  /*!< MAXP for HS bulk EPs used for building USBD ROM. DON'T CHANGE. */
#define USB_DFU_XFER_SIZE      2048 /*!< Max DFU transfer size used for building USBD ROM. DON'T CHANGE. */

/* Manifest constants defining interface numbers and endpoints used by a
   particular interface in this application.
 */
#define USB_MSC_IF_NUM 0
#define USB_MSC_IN_EP  0x81
#define USB_MSC_OUT_EP 0x01

/* On LPC18xx/43xx the USB controller requires endpoint queue heads to start on
   a 4KB aligned memory. Hence the mem_base value passed to USB stack init should
   be 4KB aligned. The following manifest constants are used to define this memory.
 */

#define USB_STACK_MEM_SIZE 2048

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __APP_USB_CFG_H_ */
