# Car Black Box

## Overview
The Car Black Box is an Embedded C project developed using the PIC16F877A microcontroller. It records important vehicle events along with their timestamps, stores them in EEPROM, and allows users to view or download the recorded logs. The project simulates the functionality of an automotive black box used for event monitoring and analysis.

## Features
- Real-Time Clock (RTC) integration for accurate timestamping
- Event logging with date and time
- Gear position monitoring
- Collision event detection
- EEPROM-based data storage
- CLCD display for user interface
- UART communication to transfer logs to a PC
- Menu-driven navigation using push buttons

## Hardware Components
- PIC16F877A Microcontroller
- 16×2 Character LCD (CLCD)
- DS1307 RTC Module
- External EEPROM
- Digital Keypad
- Potentiometer
- UART Interface
- Power Supply

## Software Requirements
- MPLAB X IDE
- MPLAB XC8 Compiler
- Embedded C Programming Language

## Project Structure
```
25038_Akshitha_Car_Black_Box/
├── main.c
├── adc.c
├── adc.h
├── clcd.c
├── clcd.h
├── eeprom.c
├── eeprom.h
├── uart.c
├── uart.h
├── ds1307.c
├── ds1307.h
├── i2c.c
├── i2c.h
├── README.md
└── Project_Report.pdf
```

## Working
1. The RTC module continuously provides the current time.
2. Vehicle events such as gear changes and collisions are detected.
3. Each event is recorded with a timestamp.
4. The event log is stored in EEPROM.
5. Users can view logs on the CLCD.
6. Stored logs can be transmitted to a computer through UART.

## Applications
- Vehicle event monitoring
- Accident analysis
- Driver behavior monitoring
- Embedded Systems learning project

## Author
**Akshitha Reddy**  
Batch: **25038**
