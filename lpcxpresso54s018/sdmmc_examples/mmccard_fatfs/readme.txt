Overview
========
The MMCCARD FATFS project is a demonstration program that uses the SDK software. It mounts a file 
system based on a MMC card then does "creat directory/read directory/create file/write file/read 
file" operation. The file sdhc_config.h has default SDHC configuration which can be adjusted to let
card driver has different performance. The purpose of this example is to show how to use MMCCARD 
driver based FATFS disk in SDK software.

Note:
User can use MMC plus card or emmc(on board IC, but not recommand use emmc socket,due to high speed timing restriction)

Toolchain supported
===================
- MCUXpresso  11.8.0
- GCC ARM Embedded  12.2

Hardware requirements
=====================
- Mini USB cable
- LPCXpresso54S018 board
- Personal Computer

Board settings
==============
Insert the card into the card slot

Prepare the Example
1. Connect a mini USB cable between the PC host and the OpenSDA USB port (J2) on twrk64f120m board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Reset the SoC and run the project.

Running the demo
================
This demo is an external flash plain load demo, ROM will copy image in external flash to SRAMX to run:
1. Build the demo to generate a bin file.
   Note: If need to generate bin file using MCUXpresso IDE, below steps need to be followed:
         Set in example Properties->C/C++ Build->Settings->Build steps->Post-build steps->Edit
         enbable arm-none-eabi-objcopy -v -O binary "&{BuildArtifactFileName}" "&{BuildArtifactFileBaseName}.bin" 
         
         This plainload example linked the vector table to 0x00000000, but program to external flash 0x10000000.

2. Program the bin file to external on board flash via SEGGER J-FLASH Lite(V6.22 or higher):

   a. Open SEGGER J-FLASH Lite, select device LPC54S018.

   b. Click the 'Erase Chip' to erase the extrenal flash.(if can not success, press SW4 button and reset the board, and try to erase again)

   c. Select the bin data file, set the '.bin/Erase Start' address to 0x10000000, then click 'Program Device'
Note: Please use above way to program the binary file built by armgcc tool chain to external flash. 
      For IAR, KEIL, MCUXpresso IDE, you can use the IDE tool to program the external flash.  
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

FATFS example to demonstrate how to use FATFS with MMC card.

Make file system......The time may be long if the card capacity is big.

Create directory......

Create a file in that directory......

List the file in that directory......
General file : F_1.DAT.

 Write/read file until encounters error......

Write to above created file.
Read from above created file.
Compare the read/write content.
The read/write content is consistent.

Input 'q' to quit read/write.
Input other char to read/write file again.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
