Overview
========
The uart_functioncal_interrupt example shows how to use uart driver functional
API to receive data with interrupt method:

In this example, one uart instance connect to PC through uart, the board will
send back all characters that PC send to the board.

Toolchain supported
===================
- GCC ARM Embedded  12.2

Hardware requirements
=====================
- Micro USB cable
- MIMX8MN6-EVK  board
- J-Link Debug Probe
- 12V power supply
- Personal Computer

Board settings
==============
No special settings are required.



Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch SW101 to power on the board
2.  Connect a USB cable between the host PC and the J901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the demo.


Running the demo
================
When the demo runs successfully, the log would be seen on the debug terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Uart functional interrupt example
Board receives characters then sends them out
Now please input:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
