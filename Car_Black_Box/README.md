# Car Black Box

A small "black box" for cars, built in embedded C on a PIC18F4580. It records what the car is doing (speed, gear, time) whenever something important happens, so you can look back at it later, on the display or on a PC.

## What it does

- Reads **speed** (0–100) from a potentiometer through the ADC
- Tracks **gear** changes: Neutral, G1–G5, Reverse
- Handles a **collision** event: the car drops to neutral and it gets logged
- Saves each event with a **timestamp** from the DS1307 RTC
- Stores the **last 12 events** in external EEPROM, overwriting the oldest when full
- Sends the whole log to a PC over **UART** (9600 baud)

## Menu

| Option | What it does |
| --- | --- |
| View Log | Scroll through saved events on the LCD |
| Clear Log | Wipe all saved events |
| Download Log | Send the log to a serial terminal |
| Set Time | Edit hour, minute and second on the RTC |

## Controls

| Key | Action |
| --- | --- |
| SW1 / SW2 | Gear up / gear down |
| SW3 | Simulate a collision |
| SW11 | Short press: up or previous. Long press: go back |
| SW12 | Short press: down or next. Long press: select |

## Hardware

- PIC18F4580 @ 20 MHz
- 16x2 character LCD
- 4x3 matrix keypad
- DS1307 RTC (I2C)
- 24-series external EEPROM (I2C)
- Potentiometer on AN4 for speed
- UART to a PC

## Project layout

| File | Purpose |
| --- | --- |
| `main.c` | Startup, main screen, gear monitoring |
| `menu.c` | Menu, view, clear and download log |
| `log.c` | Event logging and set time |
| `dashboard.c` | Dashboard screen |
| `sensor.c`, `adc.c` | Speed and gear input |
| `clcd.c`, `matrix_keypad.c` | Display and keypad drivers |
| `i2c.c`, `ds1307.c`, `external_eeprom.c` | I2C, RTC and EEPROM drivers |
| `uart.c`, `timer2.c`, `isr.c` | Serial output, timer and blink interrupt |

## Build and run

1. Open `Car_Black_Box.X` in **MPLAB X IDE**
2. Use the **XC8** compiler
3. Build, then program the PIC18F4580
4. For log download, open a serial terminal at **9600 baud, 8N1**

Each log line looks like this:

```
LOG 1: 045 G2 10:32:05
```
