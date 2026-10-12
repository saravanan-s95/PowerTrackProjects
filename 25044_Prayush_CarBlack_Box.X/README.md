# Car Black Box (Event Data Recorder)

A PIC Microcontroller-based **Car Black Box (Event Data Recorder)** system developed in C using **MPLAB X IDE** and the **XC8 Compiler**. 

This system continuously monitors critical vehicle parameters—such as current speed, gear position, and real-time clock timestamp—and automatically logs event data to non-volatile external EEPROM when state changes occur (such as gear shifts or collision events).

---

## 📌 Features

- **Real-Time Dashboard**: Displays current Time (`HH:MM:SS`), Gear position (`GN`, `G1`-`G5`, `GR`, `C`), and Vehicle Speed (`00`-`99` km/h) on a 16x2 Character LCD (CLCD).
- **Automatic Event Logging**: Detects gear changes or collision events and writes event records (Timestamp, Gear, Speed) into external non-volatile EEPROM memory in a circular log buffer.
- **Interactive On-Screen Menu**:
  - **View Logs**: Scroll through stored event records directly on the LCD interface.
  - **Set Time**: Interactively update the DS1307 Real-Time Clock (RTC) with active digit selection.
  - **Download Logs**: Export recorded logs to a PC via UART serial communication at 9600 baud.
  - **Clear Logs**: Erase stored event history from EEPROM.

---

## 🛠️ Hardware & Peripherals

| Component | Description | Interface / Pinout |
| :--- | :--- | :--- |
| **Microcontroller** | Microchip PIC16F877A (20 MHz Clock) | Core Controller |
| **Display** | 16x2 Character LCD (CLCD) | PORTD (Data), PORTC (Control: RC0, RC1, RC2) |
| **RTC** | DS1307 Real-Time Clock | I2C Protocol (SCL, SDA) |
| **EEPROM** | External EEPROM (AT24Cxx series) | I2C Protocol (SCL, SDA) |
| **Speed Sensor** | Potentiometer connected to ADC | Channel 4 (AN4) |
| **User Inputs** | 3x4 Matrix Keypad | PORTB (RB1-RB4 Inputs, RB5-RB7 Outputs) |
| **Serial Interface** | Hardware UART (9600 Baud, 8N1) | RC6 (TX), RC7 (RX) |

---

## ⚙️ Gear State Mapping

| Keypad Switch | Action / Gear State | LCD Display Code |
| :--- | :--- | :--- |
| **MK_SW1** | Shift Gear Up (`N` -> `1` -> `2` -> `3` -> `4` -> `5`) | `GN`, `G1` - `G5` |
| **MK_SW2** | Shift Gear Down (`5` -> `4` -> `3` -> `2` -> `1` -> `N`) | `GN`, `G1` - `G5` |
| **MK_SW3** | Trigger Collision Event | `C` |
| **MK_SW11** | Menu Enter / Confirm Selection | — |
| **MK_SW12** | Switch between Dashboard & Main Menu / Back | — |

---

## 📁 Repository Structure

```text
Car_Black_Box.X/
├── main.c           # Main application entry point & state machine loop
├── blackbox.h       # Top-level header file & global function definitions
├── dashboard.c      # Vehicle dashboard & gear/speed acquisition logic
├── menu.c           # Menu navigation and UI state management
├── event_logs.c     # Log storage, retrieval, EEPROM routines, & time setting
├── adc.c / adc.h    # Analog-to-Digital Converter driver (Speed potentiometer)
├── clcd.c / clcd.h  # 16x2 Character LCD driver
├── ds1307.c / ds1307.h # Real-Time Clock driver (I2C)
├── e_eeprom.c / e_eeprom.h # External EEPROM driver (I2C)
├── i2c.c / i2c.h    # I2C communication protocol driver
├── matrix_keypad.c / matrix_keypad.h # 3x4 Matrix Keypad scanner driver
├── uart.c / uart.h  # Serial UART transmitter/receiver driver
└── Makefile         # MPLAB X build Makefile
```

---

## 🚀 Getting Started

### Prerequisites
- **MPLAB X IDE** (v5.xx or later recommended)
- **Microchip XC8 Compiler**
- **PICkit 3 / 4** Programmer/Debugger or **PicSimLab** (for simulation)

### Building the Project
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/Car_Black_Box.git
   ```
2. Open MPLAB X IDE and open the project directory (`Car_Black_Box.X`).
3. Set compiler toolchain to XC8 and target device to `PIC16F877A`.
4. Click **Clean and Build Project** or press `Shift + F11`.
5. Flash the generated `.hex` file from the `dist/` directory onto your microcontroller using PICkit or load it into PicSimLab.

### Viewing UART Logs on PC
- Connect the PIC16F877A UART pins (TX/RX) to your PC using a USB-to-TTL converter.
- Open a serial terminal (e.g., PuTTY, Tera Term, Minicom, or Serial Monitor) with settings:
  - **Baud Rate**: `9600`
  - **Data Bits**: `8`
  - **Parity**: `None`
  - **Stop Bits**: `1`
- Select **DOWNLOAD LOGS** in the black box menu to transmit saved records to the terminal.

---

## 📜 License

This project is open-source and available under the [MIT License](LICENSE).
