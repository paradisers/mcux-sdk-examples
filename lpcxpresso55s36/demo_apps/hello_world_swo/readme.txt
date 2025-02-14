Overview
========
The Hello World SWO demo prints the "SWO: Hello World" string to the SWO viewer. The purpose of this demo is to
show how to use the swo, and to provide a simple project for debugging and further development.

Toolchain supported
===================
- GCC ARM Embedded  10.2.1
- MCUXpresso  11.4.1

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso55S36 board
- Personal Computer
- Jlink plus

Board settings
==============
Close JP27

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and J1 on the target board, the J8 must be left unconnected so that the Link2 Debug probe is left unpowered and does not contend with SWD.
2.  Connect Jlink plus probe to the SWD connector(J38).
3.  Download the program to the target board.
4.  Open JlinkerSWOView(C:\Program Files (x86)\SEGGER\JLink_xxx), select the target device, such as LPC55S36 and make sure the core clock and swo clock frequency is equal to the demo setting, the demo use 4MHZ as SWO clock frequency and 48MHZ as core clock frequency.
5.   After swo/core clock measure successfully, press ok to continue, make sure the bit 0 is selected in data from stimulus port item.
6.  Press the reset button on your board.
7.  Press SW3.
8.  Note: If use MCUxpresso IDE, should change SDK debug console to UART manually.

Running the demo
================
The log below shows the output of the hello world swo demo in the JlinkerSWOView window and the demo will print output periodically or when SW3 is pressed:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SWO: timer_trigger
SWO: timer_trigger
SWO: timer_trigger
SWO: hello_world
SWO: timer_trigger
SWO: timer_trigger
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
