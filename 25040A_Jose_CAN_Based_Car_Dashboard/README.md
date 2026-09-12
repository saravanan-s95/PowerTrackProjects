# CAN-Based Distributed Car Dashboard Cluster

> A distributed automotive dashboard system using three PIC18F4580 microcontrollers communicating via CAN protocol to display real-time speed, RPM, gear, and indicator status on a CLCD.


#### 1.  BRD1
* **Microcontroller:** PIC18F4580
* **Inputs:** Potentiometer (ADC Channel) for RPM; Digital Keypad for indicator signaling (Left/Right/Hazard).
* **CAN Transmit IDs:** `0x10` (RPM Payload), `0x20` (Indicator State Payload).

#### 2. BRD2
* **Microcontroller:** PIC18F4580
* **Inputs:** Potentiometer (ADC Channel 4) for vehicle Speed tracking; Digital Keypad for step-up/step-down manual Gear selection (`N`, `1-5`, `R`).
* **CAN Transmit IDs:** `0x30` (Speed Payload), `0x40` (Gear Position Payload).

#### 3. BRD3 (Central Dashboard)
* **Microcontroller:** PIC18F4580
* **Peripherals:** 16x2 Character LCD (CLCD) on PORTD/PORTC; Status Indication LEDs on PORTB.
* **Operation:** To process and display incoming telemetry frames onto fixed coordinates without display scrolling.
