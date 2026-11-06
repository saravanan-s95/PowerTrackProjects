# Digital Alarm Clock (PIC16F877A)

An embedded alarm clock built on the PIC16F877A and simulated in PICsimLab (PICGenios board). It shows the time and date from a DS1307 real-time clock, lets the user set the time, date and up to 8 alarms from a 16x2 LCD and two keys, and shows the next alarm on the default screen.

## Hardware (PICsimLab, PICGenios board)

| Part | Use |
|------|-----|
| PIC16F877A @ 20 MHz | Microcontroller |
| DS1307 RTC (I2C) | Keeps the time and date |
| 16x2 character LCD | Menus and display |
| Digital keypad | UP and DOWN user keys (active low, PORTB) |

## Features

- **Default screen:** date and time (12-hour with AM/PM). Every 5 seconds the next alarm is shown on line 2 for 2 seconds.
- **Config menu:** reached by pressing UP or DOWN on the default screen. Returns to the default screen after 5 seconds without a key press.
- **Set Time and Set Date:** the selected field blinks at 1 Hz. UP increments it (with rollover), DOWN moves to the next field. Date limits follow the month and leap years.
- **Set / View Alarms:** 8 alarm slots, each shown as Active or Empty. An alarm has a time (entered in 12-hour AM/PM) and a repeat type: `O` once, `D` daily, `W` weekly. Alarms can be edited and deleted.

## Key controls

| Action | Result |
|--------|--------|
| Short UP / DOWN | Navigate, or change the blinking field |
| Long UP | Enter the selected item, or save |
| Long DOWN | Go back, or log out |

## How it works

- **I2C:** polled driver on the PIC's MSSP module (RC3 = SCL, RC4 = SDA, 100 kHz). The DS1307 is addressed at `0xD0` (write) and `0xD1` (read). Time and date registers are stored in BCD and converted in `read_rtc()`, `write_rtc()` and `write_rtc_date()`.
- **Timer1:** interrupt every 12.5 ms (20 MHz, prescaler 1:1, preload 3036). It drives the 5-second idle timeout (400 interrupts) and the 1 Hz blink (toggles every 40 interrupts).
- **Time handling:** the RTC runs in 24-hour mode and alarms are stored in 24-hour form. 12-hour AM/PM exists only for display and alarm entry.
- **Screens:** one function per screen. Each returns the next screen, and `main()` dispatches to it.
- **Next event:** each alarm is converted to minutes since midnight, and the smallest wrap-around difference from the current time is the next event.

## Project files

| File | Purpose |
|------|---------|
| `main.c` | Setup, main loop, screen dispatch |
| `screen.c` / `screen.h` | All screens, alarm data and helpers |
| `isr.c` | Timer1 interrupt (idle timer, blink phase) |
| `timer.c` | Timer1 setup |
| `i2c.c` | I2C driver and DS1307 read/write |
| `digital_keypad.c` / `.h` | Keypad driver (level and state detection) |
| `clcd.c` | LCD driver |

## Build and run

1. Open the project in MPLAB X with the XC8 compiler.
2. Build to produce the `.hex` file.
3. In PICsimLab, select the **PICGenios** board with the **PIC16F877A**, load the `.hex`, and run.

## Known limitations

- Alarms are stored in RAM only, so they are lost when the power is off.
- A weekly alarm is treated like a daily one, because the weekday is not stored.
