Overview
========
The DMA memory to memory example is a simple demonstration program that uses the SDK software.
It executes one shot transfer from source buffer to destination buffer using the SDK DMA drivers.
The purpose of this example is to show how to use the DMA and to provide a simple example for
debugging and further development.

Toolchain supported
===================
- MCUXpresso  11.8.0
- GCC ARM Embedded  12.2

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso54628 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J8) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~
DMA memory to memory transfer example begin.

Destination Buffer:
0       0       0       0

DMA memory to memory transfer example finish.

Destination Buffer:
1       2       3       4
~~~~~~~~~~~~~~~~~~~~~

