Overview
========
The lpuart_functioncal_interrupt example shows how to use lpuart driver functional
API to receive data with interrupt method:

In this example, one lpuart instance connect to PC, the board will
send back all characters that PC send to the board.

Toolchain supported
===================
- GCC ARM Embedded  12.2
- MCUXpresso  11.8.0

Hardware requirements
=====================
- Micro USB cable
- TWR-KM35Z75M board
- Personal Computer
- USB to Com Converter

Board settings
==============
Connect pin:
- RX of USB2COM to J14-1
- TX of USB2COM to J16-1
- GND of USB2COM to J25-10

Prepare the Demo
================
1.  Connect USB to Com Converter between the host PC and the port on the board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
================
When the example runs successfully, the log would be seen on the UART Terminal port which connected to the USB2COM like:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Lpuart functional interrupt example
Board receives characters then sends them out
Now please input:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
