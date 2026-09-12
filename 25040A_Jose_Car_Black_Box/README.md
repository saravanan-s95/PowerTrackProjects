# Car Black Box (Event Data Recorder)

## Overview
The Car Black Box is a bare-metal embedded system designed to act as an Event Data Recorder (EDR) for a vehicle. Built on a PIC18F-series microcontroller, the system continuously monitors and logs critical vehicle parameters—such as speed, gear position, and time—leading up to a collision. It features a robust, non-volatile First-In-First-Out (FIFO) memory architecture to ensure crash data survives power loss and can be downloaded to a PC via serial communication for post-incident analysis.

## Core Technical Concepts Demonstrated
* **Bare-Metal C Programming:** Direct hardware register configuration and bitwise operations.
* **State Machine Architecture:** Non-blocking super-loop design handling interactive UI menus and transient hardware tasks without freezing the CPU.
* **Memory Management:** Implemented a rolling FIFO buffer in an external I2C EEPROM, including a "Magic Word" boot sequence for power-loss recovery and memory initialization.
* **Communication Protocols:** I2C (EEPROM & RTC) and UART (PC data transmission).

## Hardware Components
* **Microcontroller:** PIC18F4580 (8-bit)
* **Real-Time Clock (RTC):** DS1307 (I2C) with battery backup
* **Memory:** External EEPROM (I2C)
* **Display:** 16x2 Character LCD (CLCD)
* **Input:** 4x4 Matrix Keypad
* **Sensors/Simulation:** Potentiometer (connected to internal ADC to simulate vehicle speed)

## Key Features
### 1. Live Dashboard
Continuously displays the live vehicle speed (0-100 km/h mapped via ADC), current gear status (R, N, 1-5), and real-time clock (HH:MM:SS in 24-hour format) read directly from the DS1307.

### 2. Event Logging & Collision Detection
Logs 5 bytes of critical data (Speed, Gear, Hour, Minute, Second) per event. A dedicated collision switch instantly triggers the "Crash" state (Gear 'C').. 

### 3. Smart Non-Volatile Storage (FIFO)
Stores up to 10 distinct events in the external EEPROM. Once full, the system autonomously shifts older memory blocks down, discarding the oldest log to make room for the newest, ensuring the 10 most recent events are always retained even if the battery is disconnected.

### 4. Interactive Log Management (UI)
* **View Log:** Scroll through the EEPROM history chronologically via the CLCD.
* **Set Time:** Dynamically reprogram the DS1307 RTC registers.
* **Clear Log:** Wipes the software tracking variables and rewrites the EEPROM save-state to safely format the drive.
* **Download Log:** Transmits the entire crash history over UART (9600 Baud) formatted sequentially for viewing on a PC terminal (e.g., Tera Term).

## Tools Used
* **IDE:** MPLAB X IDE
* **Compiler:** XC8
