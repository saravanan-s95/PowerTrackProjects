# Smart Microwave Oven - PIC Microcontroller Based

## Overview
This project is a Smart Microwave Oven control system implemented using PIC microcontroller. It supports 3 cooking modes (MICRO, GRILL, CONVECTION) with timer setting, pre-heating, pause/resume, and auto cooking done alert. The system uses state machine architecture for stable operation.

## Features
- 3 Modes: 1. MICRO 2. GRILL 3. CONVECTION
- Time setting in MM:SS format using matrix keypad
- Pre-heating function (60 sec) for Convection mode
- Real-time countdown display on 16x2 CLCD
- Pause and Resume functionality
- Auto Cooking Done alert with buzzer
- Debounced keypad handling

## Hardware Required
- PIC16F877A
- 16x2 CLCD Display
- Matrix Keypad
- Heater - RC5, Cooler/Fan - RC2, Buzzer - RC0
- 20MHz Crystal Oscillator

## Software / Technologies Used
- Embedded C (MPLAB XC8 Compiler)
- MPLAB X IDE
- Timer0 Interrupt for 1ms tick generation
- CLCD Driver, Matrix Keypad Driver
- State Machine Concept

## State Machine
- STATE 1: MENU - Mode selection
- STATE 2: SET TIME - Time entry MM:SS
- STATE 3: PRE-HEATING - Only for CONV mode (60 sec)
- STATE 4: COOKING - Cooking with countdown
- STATE 5: COOKING DONE - Buzzer alert and return to menu

