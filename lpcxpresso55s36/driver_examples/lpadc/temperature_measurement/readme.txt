Overview
========

The lpadc_temperature_measurement example shows how to measure the temperature within the internal sensor.

In this example, the ADC input channel is mapped to an internal temperature sensor. When running the project, typing
any key into debug console would trigger the conversion. ADC watermark interrupt would be asserted once the number of
datawords stored in the ADC Result FIFO is greater than watermark value. In ADC ISR, the watermark flag would be
cleared by reading the conversion result value. When the conversion done, two valid result will be put in the FIFO,
then the temperature can be calculated within the two results and a specific formula. 

Toolchain supported
===================
- GCC ARM Embedded  10.2.1
- MCUXpresso  11.4.1

Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55S36 board
- Personal Computer

Board settings
==============

Prepare the Demo
================
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J1) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the temperature measurement example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC Temperature Measurement Example
ADC Full Range: 65536
Please press any key to get temperature from the internal temperature sensor.
Current temperature: 29.342
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
