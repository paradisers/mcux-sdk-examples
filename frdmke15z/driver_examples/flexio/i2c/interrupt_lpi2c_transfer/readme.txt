Overview
========
The flexio_lpi2c_interrupt example shows how to use flexio i2c master  driver in interrupt way:

In this example, a flexio simulated i2c master connect to a LPI2C slave

Toolchain supported
===================
- GCC ARM Embedded  12.2
- MCUXpresso  11.8.0

Hardware requirements
=====================
- Micro USB cable
- FRDM-KE15Z board
- Personal Computer

Board settings
==============
The flexio_i2c_interrupt example is requires connecting the FLEXIO pins with the I2C pins
The connection should be set as following:
   FLEXIO_I2C        connected to  LPI2C0
SCL     PTE11(J2-15)     -->       PTA3(J4-12)
SDA     PTE10(J2-17)     -->       PTA2(J4-10)

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
FlexIO I2C interrupt - LPI2C interrupt
Master will send data :
0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07
0x08  0x09  0x0A  0x0B  0x0C  0x0D  0x0E  0x0F
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1A  0x1B  0x1C  0x1D  0x1E  0x1F

Slave received data :
0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07
0x08  0x09  0x0A  0x0B  0x0C  0x0D  0x0E  0x0F
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1A  0x1B  0x1C  0x1D  0x1E  0x1F
~~~~~~~~~~~~~~~~~~~~~
