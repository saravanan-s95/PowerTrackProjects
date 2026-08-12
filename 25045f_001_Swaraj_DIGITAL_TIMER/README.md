# 25045F_001_Swaraj_DIGITAL_TIMER

## Project Title
Digital Timer using PIC16F877A

## Project Brief
This project is a Digital Timer developed using the PIC16F877A microcontroller. It provides real-time clock and date display, countdown timer, alarm functionality, and user configuration through a matrix keypad.

The current time and date are maintained using an RTC module. The RTC communicates with the PIC16F877A using the I2C protocol. A CLCD is used to display the time, date, timer settings, and alarm status.

## Technologies Used
- Embedded C
- PIC16F877A Microcontroller
- MPLAB X IDE
- XC8 Compiler
- I2C Communication
- Timer1 Interrupt
- CLCD
- Matrix Keypad
- RTC

## Hardware / Modules Used
- PIC16F877A
- RTC Module
- 16x2 CLCD
- Matrix Keypad
- Buzzer

## Features
- Real-time clock display
- Date display and configuration
- Time setting
- Alarm setting
- Alarm ON/OFF control
- Countdown timer
- Timer1 interrupt-based countdown
- Buzzer indication for alarm
- User input through matrix keypad
- CLCD-based user interface

## Working
1. The PIC16F877A initializes the CLCD, matrix keypad, RTC, and Timer1.
2. The RTC provides the current time and date to the microcontroller through I2C.
3. The current time is displayed on the home screen.
4. The user can enter the configuration menu using the RIGHT key.
5. The user can set the time and date using the matrix keypad.
6. The user can configure an alarm hour and minute and enable or disable the alarm.
7. The user can set the countdown timer in minutes and seconds.
8. Timer1 interrupt is used to generate the timing interval for the countdown.
9. When the configured alarm time matches the RTC time, the buzzer is activated.
10. The LCD displays an alarm message and the buzzer remains active for the configured duration.

## Key Concepts
- Microcontroller programming
- Embedded C
- RTC interfacing
- I2C communication
- Timer1 interrupt
- Interrupt Service Routine (ISR)
- Matrix keypad interfacing
- CLCD interfacing
- BCD to Decimal conversion
- Decimal to BCD conversion

## Challenges
- Interfacing the RTC with the PIC16F877A using I2C
- Handling keypad input correctly
- Implementing Timer1 interrupt for accurate countdown
- Managing alarm triggering without repeated triggering
- Displaying time and menu options properly on the CLCD

## Learning Outcomes
- Learned PIC16F877A microcontroller programming using Embedded C
- Learned RTC and I2C interfacing
- Understood Timer1 and interrupt handling
- Learned matrix keypad and CLCD interfacing
- Understood BCD and decimal data conversion
- Improved debugging and embedded system development skills

## Project Status
Completed

## Author
**Swaraj**
