/*
** ###################################################################
**     Processors:          MIMXRT685SFAWBR_cm33
**                          MIMXRT685SFFOB_cm33
**                          MIMXRT685SFVKB_cm33
**
**     Compiler:            GNU C Compiler
**     Reference manual:    MIMXRT685 User manual Rev. 0.95 11 November 2019
**     Version:             rev. 1.0, 2018-06-19
**     Build:               b210303
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2021 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/



/* Entry Point */
ENTRY(Reset_Handler)

/* NVM */
/* By default, the NVM is not used. */
UseNVMLink = DEFINED(gUseNVMLink_d) ? gUseNVMLink_d : 0;
EraseNVMLink = DEFINED(gEraseNVMLink_d) ? gEraseNVMLink_d : 0;
NVMSectorCountLink = DEFINED(gNVMSectorCountLink_d) ? gNVMSectorCountLink_d : 4;

_ROM_START_ = 0x08001130;
_ROM_END_ = 0x087FFFFF;

STORAGE_AREA_LENGTH = 0x10000;
STORAGE_AREA_SIZE  = STORAGE_AREA_LENGTH;
STORAGE_AREA_START = _ROM_END_ - STORAGE_AREA_SIZE + 1;
m_text_end         = STORAGE_AREA_START -1;
m_text_start       = _ROM_START_;
m_text_size        = STORAGE_AREA_START - m_text_start;

/*** NVM Configuration Start ***/
NVM_AREA_START = STORAGE_AREA_START;

NV_STORAGE_SECTOR_SIZE = 0x1000;  /* 4k flash sector size */
NV_STORAGE_MAX_SECTORS = NVMSectorCountLink;
NVM_AREA_SIZE = NV_STORAGE_SECTOR_SIZE * NV_STORAGE_MAX_SECTORS;
NV_STORAGE_END_ADDRESS = NVM_AREA_START + NVM_AREA_SIZE - 1;
NV_STORAGE_START_ADDRESS = NVM_AREA_START;
/*** NVM Configuration End ***/

/*** littleFS configuration Start ***/
EDGEFAST_BT_LITTLEFS_STORAGE_START_ADDRESS = _ROM_END_ + 1;
EDGEFAST_BT_LITTLEFS_STORAGE_SECTOR_SIZE = 0x1000;  /* 4k flash sector size */
EDGEFAST_BT_LITTLEFS_STORAGE_MAX_SECTORS = (0x08FFFFFF - EDGEFAST_BT_LITTLEFS_STORAGE_START_ADDRESS) / EDGEFAST_BT_LITTLEFS_STORAGE_SECTOR_SIZE;
/*** littleFS configuration End ***/

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x1000;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0400;
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x00000130 : 0;

/* Specify the memory areas */
/* The SRAM region [0x10000-0x1BFFF] is reserved for ROM code. */
/* The SRAM region [0x0-0xFFFF], [0x1C000-0x1FFFF] is reserved for app-specific use cases. */
/* The SRAM region [0x20000-0x7FFFF] is reserved for Non-cached shared memory between M33 and DSP. */
MEMORY
{
  m_flash_config        (RX)  : ORIGIN = 0x08000400, LENGTH = 0x00000200
  m_interrupts          (RX)  : ORIGIN = 0x08001000, LENGTH = 0x00000130
  m_text                (RX)  : ORIGIN = m_text_start, LENGTH = m_text_size
  NVM_region            (RW)  : ORIGIN = STORAGE_AREA_START, LENGTH = STORAGE_AREA_SIZE
  m_data                (RW)  : ORIGIN = 0x20080000, LENGTH = 0x00180000
  m_usb_sram            (RW)  : ORIGIN = 0x40140000, LENGTH = 0x00004000
}

/* Define output sections */
SECTIONS
{
  .flash_config :
  {
    . = ALIGN(4);
    __FLASH_BASE = .;
    KEEP(* (.flash_conf))     /* flash config section */
    . = ALIGN(4);
  } > m_flash_config

  /* The startup code goes first into internal ram */
  .interrupts :
  {
    . = ALIGN(4);
    __VECTOR_TABLE = .;
    __Vectors = .;
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  /* The program code and other data goes into internal ram */
  .text :
  {
    . = ALIGN(4);
    *(EXCLUDE_FILE(
        /* Exclude flash and frequently executed functions from XIP */
        */fsl_adapter_flexspi_nor_flash.c.obj
        */NV_Flash.c.obj
        */mflash_drv.c.obj
        */fsl_flexspi.c.obj
    ) .text)                 /* .text sections (code) */
    *(EXCLUDE_FILE(
        /* Exclude flash and frequently executed functions from XIP */
        */fsl_adapter_flexspi_nor_flash.c.obj
        */NV_Flash.c.obj
        */mflash_drv.c.obj
        */fsl_flexspi.c.obj
    ) .text*)                /* .text* sections (code) */
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
    PROVIDE(__start_NVM_ADAPTER_TABLE = .);
    KEEP(*(.NVM_ADAPTER_TABLE));
    PROVIDE(__stop_NVM_ADAPTER_TABLE = .);
    . = ALIGN(4);
  } > m_text
  ._bt_settings_area :
  {
    . = ALIGN(4);
    _bt_settings_start = .;
    KEEP(*(SORT(._bt_settings.static.*)))
    _bt_settings_end = .;
    . = ALIGN(4);
  } > m_text

  ._settings_handler_static :
  {
    . = ALIGN(4);
    _settings_handler_static_list_start = .;
    KEEP(*(SORT(._settings_handler_static.static.*)))
    _settings_handler_static_list_end = .;
    . = ALIGN(4);
  } > m_text

  ._bt_conn_cb :
  {
    . = ALIGN(4);
    _bt_conn_cb_list_start = .;
    KEEP(*(SORT(._bt_conn_cb.static.*)))
    _bt_conn_cb_list_end = .;
    . = ALIGN(4);
  } > m_text

  ._bt_gatt_service_static :
  {
    . = ALIGN(4);
    _bt_gatt_service_static_list_start = .;
    KEEP(*(SORT(._bt_gatt_service_static.static.*)))
    _bt_gatt_service_static_list_end = .;
    . = ALIGN(4);
  } > m_text

  ._bt_l2cap_fixed_chan :
  {
    . = ALIGN(4);
    _bt_l2cap_fixed_chan_list_start = .;
    KEEP(*(SORT(._bt_l2cap_fixed_chan.static.*)))
    _bt_l2cap_fixed_chan_list_end = .;
    . = ALIGN(4);
  } > m_text

  ._bt_l2cap_br_fixed_chan :
  {
    . = ALIGN(4);
    _bt_l2cap_br_fixed_chan_list_start = .;
    KEEP(*(SORT(._bt_l2cap_br_fixed_chan.static.*)))
    _bt_l2cap_br_fixed_chan_list_end = .;
    . = ALIGN(4);
  } > m_text

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_text

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_text

 .ctors :
  {
    __CTOR_LIST__ = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_text

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_text

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_text

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  .interrupts_ram :
  {
    . = ALIGN(4);
    __VECTOR_RAM__ = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . += M_VECTOR_RAM_SIZE;
    . = ALIGN(4);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > m_data

  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(CodeQuickAccess)       /* CodeQuickAccess sections */
    *(DataQuickAccess)       /* DataQuickAccess sections */
    *(m_usb_dma_init_data)
    /* Explicit placement of flash and frequently executed functions in RAM */
    */fsl_adapter_flexspi_nor_flash.c.obj
    */NV_Flash.c.obj
    */mflash_drv.c.obj
    */fsl_flexspi.c.obj
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    PROVIDE(__start_NVM_TABLE = .);
    KEEP(*(.NVM_TABLE));
    PROVIDE(__stop_NVM_TABLE = .);
    . = ALIGN(4);
    PROVIDE(__start_NVM_ADAPTER_TABLE = .);
    KEEP(*(.NVM_ADAPTER_TABLE));
    PROVIDE(__stop_NVM_ADAPTER_TABLE = .);
    . = ALIGN(4);
    _k_mem_slab_list_start = .;
    KEEP(*(SORT(._k_mem_slab*)))
    _k_mem_slab_list_end = .;
    . = ALIGN(4);
    _net_buf_pool_list = .;
    KEEP(*(SORT(._net_buf_pool*)))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")
  _image_size = __DATA_END - __VECTOR_TABLE;

  /* Uninitialized data section */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    __START_BSS = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_data
  .NVM_TABLE :
  {
    . = ALIGN(4);
    PROVIDE(__start_NVM_TABLE = .);
    KEEP(*(.NVM_TABLE));
    PROVIDE(__stop_NVM_TABLE = .);
    . = ALIGN(4);
  } > m_data
  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_data

  .stack :
  {
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_data

  m_usb_bdt (NOLOAD) :
  {
    . = ALIGN(512);
    *(m_usb_bdt)
  } > m_usb_sram

  m_usb_global (NOLOAD) :
  {
    *(m_usb_global)
  } > m_usb_sram

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_data) + LENGTH(m_data);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }
  /* Remove this section to keep the nvm section on writting the device */
  .NVM :
  {
    FILL(0xFFFFFFFF);
    . = ORIGIN(NVM_region) + LENGTH(NVM_region) - 1;
    BYTE(0xFF)
  } > NVM_region
  .ARM.attributes 0 : { *(.ARM.attributes) }
  ASSERT(__StackLimit >= __HeapLimit, "region m_data overflowed with stack and heap")
}

