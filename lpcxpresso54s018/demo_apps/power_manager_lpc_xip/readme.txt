Overview
========
The power_manager_lpc application shows the usage of normal power mode control APIs for entering the three kinds of
low power mode: Sleep mode, Deep Sleep mode and Sleep Power Down mode. When the application runs to each low power
mode, the device would cut off the power for specific modules to save energy. The device can be also waken up by
prepared wakeup source from external event.

 Tips:
 This demo is to show how the various power mode can switch to each other. However, in actual low power use case, to save energy and reduce the consumption even more, many things can be done including:
 - Disable the clock for unnecessary modules during low power mode. That means, programmer can disable the clocks before entering the low power mode and re-enable them after exiting the low power mode when necessary.
 - Disable the function for unnecessary part of a module when other part would keep working in low power mode. At the most time, more powerful function means more power consumption. For example, disable the digital function for the unnecessary pin mux, and so on.
 - Set the proper pin state (direction and logic level) according to the actual application hardware. Otherwise, there would be current leakage on the pin, which will increase the power consumption.
 - Other low power consideration based on the actual application hardware.
 - In order to meet typedef power consumption of DateSheet manual, Please configure MCU under the following conditions.
     • Configure all pins as GPIO with pull-up resistor disabled in the IOCON block.
     • Configure GPIO pins as outputs using the GPIO DIR register.
     • Write 1 to the GPIO CLR register to drive the outputs LOW.
     • All peripherals disabled.

Toolchain supported
===================
- MCUXpresso  11.8.0
- GCC ARM Embedded  12.2

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso54S018 board
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
This demo is an external flash XIP demo, CPU will execute the code in external flash:
1. Build the demo to generate a bin file.
   Note: If need to generate bin file using MCUXpresso IDE, below steps need to be followed:
         Set in example Properties->C/C++ Build->Settings->Build steps->Post-build steps->Edit
         enbable arm-none-eabi-objcopy -v -O binary "&{BuildArtifactFileName}" "&{BuildArtifactFileBaseName}.bin" 
         
         This plainload example linked the vector table to 0x10000000, but program to external flash 0x10000000.

2. Program the bin file to external on board flash via SEGGER J-FLASH Lite(V6.22 or higher):

   a. Open SEGGER J-FLASH Lite, select device LPC54S018.

   b. Click the 'Erase Chip' to erase the extrenal flash.(if can not success, press SW4 button and reset the board, and try to erase again)

   c. Select the bin data file, set the '.bin/Erase Start' address to 0x10000000, then click 'Program Device'
Note: Please use above way to program the binary file built by armgcc tool chain to external flash. 
      For IAR, KEIL, MCUXpresso IDE, you can use the IDE tool to program the external flash.  

3. Press the reset button.

The log below shows in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Power Manager Demo for LPC device.
The "user key" is: SW5
Select an option
        1. Sleep mode
        2. Deep Sleep mode
        3. Deep power down mode
/* Type in '1' into UART terminal */
Entering Sleep [Press the user key to wakeup] ...
/* Press the user key on board */
Pin event occurs
Wakeup.
Select an option
        1. Sleep mode
        2. Deep Sleep mode
        3. Deep power down mode
/* Type in '2' into UART terminal */
Entering Deep Sleep [Press the user key to wakeup] ...
/* Press the user key on board */
Pin event occurs
Wakeup.
Select an option
        1. Sleep mode
        2. Deep Sleep mode
        3. Deep power down mode
/* Type in '3' into UART terminal */
Entering Deep Powerdown [Reset to wakeup] ...
Press any key to confirm to enter the deep sleep mode and wakeup the device by reset.
/* Type in any key into UART terminal */

/* Press the reset key on board. */
Power Manager Demo for LPC device.
The "user key" is: SW5
Select an option
        1. Sleep mode
        2. Deep Sleep mode
        3. Deep power down mode
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
