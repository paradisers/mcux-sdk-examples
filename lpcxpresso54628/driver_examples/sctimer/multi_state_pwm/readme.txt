Overview
========
The SCTImer multi-state project is a demonstration program of the SCTimer state machine. It shows how to set up events to be triggered in a certain state
and transitioning between states.
This project has two states: State 0 and State 1.

For State 0, PWM0 (24kHz 10% duty cycle) is setup, PWM0 works based on event eventFirstNumberOutput
and event (eventFirstNumberOutput + 1). Meanwhile, another event is scheduled to monitor the rising edge on input 1,
when rising edge detected on input 1, the SCTimer switches to State 1.

For State 1, PWM1 (24kHz 50% duty cycle) is setup, PWM1 works based on event eventSecondNumberOutput
and event (eventSecondNumberOutput + 1). Meanwhile, another event is scheduled to monitor the rising edge on input 1,
when rising edge detected on input 1, then SCTimer switches to State 0. To make PWM0 also work in State 1, the PWM0's
events eventFirstNumberOutput and (eventFirstNumberOutput + 1) are also enabled in State 1, using API
SCTIMER_ScheduleEvent. As a result, both PWM0 and PWM1 works in State 1.

The SCTimer input 1 is routed to a button GPIO, when press the button, the SCTimer switches between State 0 and State 1.
In State 0, there is only PWM0 output, in State 1, there are both PWM0 and PWM1 output.

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
The log below shows example output of the SCTimer multi-state demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SCTimer example to output edge-aligned PWM signal

When user presses a switch the PWM signal will be seen from Out 2
When user presses the switch again PWM signal on Out 2 will turn off
The PWM signal from Out 4 will remain active all the time
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
- Use a Digital osc probe on J13_5(Out 4) and J13_16(Out 2) to watch the PWM.
- When press sw2 the PWM signal will be seen from J13_16.
- When press sw2 again the PWM signal will turn off from J13_16. 
