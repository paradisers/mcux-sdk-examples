Overview
========
The Hello World SWO demo prints the "SWO: Hello World" string to the SWO viewer. The purpose of this demo is to
show how to use the swo, and to provide a simple project for debugging and further development.

Toolchain supported
===================
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.6.0

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso55S06 board
- Personal Computer
- Jlink plus

Board settings
==============

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and J1 on the target board, open jumper JP13,JP15,JP16,JP17.
2.  Connect Jlink plus probe to the SWD connector(J17).
3.  Download the program to the target board.
4.	Open JlinkerSWOView(C:\Program Files (x86)\SEGGER\JLink_xxx), select the target device, such as LPC55S06 and make sure the core clock and swo clock frequency is equal to the demo setting, the demo use 4MHZ as SWO clock frequency and 48MHZ as core clock frequency.
5. 	After swo/core clock measure successfully, press ok to continue, make sure the bit 0 is selected in data from stimulus port item.
6.  Press the reset button on your board.
7.	Press SW4.
8.  Note: If use MCUxpresso IDE, you need to set the SDK debug console to UART when importing projects at
    Project Options -> SDK Debug Console -> UART.

Running the demo
================
The log below shows the output of the hello world swo demo in the JlinkerSWOView window and the demo will print output periodically or when SW4 is pressed:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SWO: timer_trigger
SWO: timer_trigger
SWO: timer_trigger
SWO: hello_world
SWO: timer_trigger
SWO: timer_trigger
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
