Overview
========
The lpuart_interrupt_ring_buffer Example project is to demonstrate usage of the KSDK lpuart driver.
In the example, you can send characters to the console back and they will be printed out onto console
 in a group of 8 characters.

Toolchain supported
===================
- GCC ARM Embedded  12.2
- MCUXpresso  11.8.0

Hardware requirements
=====================
- Mini/Micro USB cable
- FRDM-K22F board
- Personal Computer
- USB to Com Converter

Board settings
==============
Connect pin:
- RX of USB2COM to J1-4
- TX of USB2COM to J1-2
- GND of USB2COM to J2-14

Prepare the Demo
================
1.  Connect USB to Com Converter between the PC host and the port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
================
When the demo runs successfully, the log would be seen on the LPUART Terminal port which connected to the USB2COM like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPUART RX ring buffer example
Send back received data
Echo every 8 bytes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
