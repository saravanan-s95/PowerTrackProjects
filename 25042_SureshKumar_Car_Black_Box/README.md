# CAR BLACK BOX USING PIC16F877A

## 1. Introduction

Nowadays, road accidents are increasing due to over-speeding and careless driving. After an accident, it is often difficult to know what happened before the collision. To solve this problem, a Car Black Box can be used to record important vehicle information.

This project is developed using the PIC16F877A microcontroller. It records the vehicle speed, gear position, collision event and current time. The recorded data is stored in external EEPROM and can be viewed later through the LCD or downloaded to a computer using UART communication.

---

## 2. Objective

The main objective of this project is to develop a simple event recording system for vehicles. It continuously monitors the vehicle status and stores important events with their corresponding time and speed.

---

## 3. Hardware Components

* PIC16F877A Microcontroller
* 16×2 Character LCD
* DS1307 Real Time Clock
* 24Cxx External EEPROM
* Digital Push Buttons
* Potentiometer
* UART Interface
* Crystal Oscillator
* Power Supply

---

## 4. Software Requirements

* MPLAB X IDE
* XC8 Compiler
* Proteus (for simulation)
* PuTTY / Tera Term (for UART output)

---

## 5. Working

When power is supplied, the microcontroller initializes all the peripherals such as ADC, LCD, keypad, RTC, UART and I2C communication.

The dashboard screen continuously displays the current time, event and speed. The speed value is obtained from the ADC by varying the potentiometer.

Whenever the driver changes the gear or a collision occurs, the current time, event and speed are stored in the external EEPROM.

To access the menu, the user must enter a four-bit password. After successful login, different options such as View Log, Clear Log, Download Log, Set Time and Change Password become available.

---

## 6. Features

### Dashboard

Displays the current time, event and speed continuously.

### Event Recording

Stores every gear change and collision event with the corresponding time and speed.

### Login Security

A password is required to access the menu. After five incorrect attempts, the system is locked for 120 seconds.

### View Log

Displays all the stored records one by one on the LCD.

### Clear Log

Deletes all previously stored event records from EEPROM.

### Download Log

Transfers all stored logs to a computer through UART communication.

### Set Time

Allows the user to modify the current RTC time.

### Change Password

Allows the user to update the login password.

---

## 7. Memory Storage

Each event contains:

* Time
* Event
* Speed

Each record occupies 10 bytes in EEPROM. The project can store up to 10 records. When the memory becomes full, the oldest records are overwritten.

---

## 8. Button Functions

 Button | Function                                          |
 ------ | ------------------------------------------------- |
  SW1    | Increase Gear / Increment / Move Down / Enter '1' |
 SW2    | Decrease Gear / Decrement / Move Up / Enter '0'   |
SW3    | Collision Event / Back                            |
SW4    | Login / Select / Save                             |

---

## 9. Advantages

* Simple and low-cost design.
* Easy to operate.
* Password protected.
* Stores events even after power failure.
* Real-time monitoring.
* UART communication for downloading logs.

---

## 10. Applications

* Vehicle monitoring
* Accident analysis
* Driver behaviour recording
* Fleet management
* Educational embedded system project

---

## 11. Future Scope

The project can be enhanced by adding GPS to record vehicle location, GSM to send accident alerts, SD card support for larger storage and IoT connectivity for cloud monitoring.

---

## 12. Conclusion

The Car Black Box project successfully records vehicle events along with time and speed. The stored information helps in understanding the sequence of events during vehicle operation. The use of RTC, EEPROM and UART makes the system reliable and suitable for basic automotive event recording applications. This project also provides good exposure to embedded system programming and peripheral interfacing using the PIC16F877A microcontroller
