Overview
========
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_spi_dma_b2b_transfer_slave example shows how to use spi driver as master to do board to board transfer 
with DMA:

In this example, one spi instance as master and another spi instance on the other board as slave. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

Toolchain supported
===================
- MCUXpresso  11.8.0
- GCC ARM Embedded  12.2

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso51U68 board
- Personal Computer

Board settings
==============
Populate jumper JP3.
Connect SPI master on board to SPI slave on other board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Master - SPI3                 CONNECTS TO      Slave - SPI3
Pin Name   Board Location                      Pin Name  Board Location
MISO       J4 pin 3                            MISO       J4 pin 3
MOSI       J4 pin 2                            MOSI       J4 pin 2
SCK        J4 pin 4                            SCK        J4 pin 4
PCS2       J4 pin 7                            PCS2       J4 pin 7
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the LPC-Link USB port (J6) on the board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
The following message shows in the terminal if the example runs successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CMSIS SPI board2board DMA example -- Slave transfer.


 Slave example is running...


 Slave receive:
      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
     10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
     20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
     30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
Succeed!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
