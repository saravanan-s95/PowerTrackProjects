
# Home Automation Using HTML

## Project Description

The **Home Automation System Using HTML** is an IoT-based application that enables users to monitor and control home appliances through a web browser. The system uses an **HTML webpage** as the user interface, **ESP32** as the automation controller, and the **MQTT protocol** for communication between the web page and the ESP32.

The HTML webpage provides an easy-to-use dashboard with buttons for controlling appliances such as lights, fans, and other electrical devices. When a user clicks a button, the webpage publishes an MQTT message to the broker. The ESP32 subscribes to the corresponding MQTT topics, receives the command, and controls the connected relay modules. It also publishes the current device status back to the MQTT broker, allowing the webpage to display real-time updates.

This project demonstrates how web technologies and IoT can be integrated to build a simple, reliable, and scalable home automation system.

---

## Objectives

- Develop a browser-based home automation system.
- Control electrical appliances remotely using an HTML webpage.
- Enable real-time communication between the webpage and ESP32 using MQTT.
- Display the current status of connected devices.
- Provide a responsive and user-friendly interface.
- Demonstrate IoT-based home automation using low-cost hardware.

---

## Features

- HTML-based dashboard
- Real-time appliance control
- MQTT Publish/Subscribe communication
- ESP32-based controller
- Relay module integration
- Device status monitoring
- Responsive web interface
- Easy to expand with additional appliances

---

## Technologies Used

- HTML
- CSS
- JavaScript
- ESP32
- MQTT Protocol
- Mosquitto MQTT Broker
- Arduino IDE
- Wi-Fi

---

## Hardware Requirements

- ESP32 Development Board
- Relay Module
- LED/Bulb (for demonstration)
- Power Supply
- Wi-Fi Router
- Jumper Wires

---

## Software Requirements

- Arduino IDE
- ESP32 Board Package
- Mosquitto MQTT Broker
- Google Chrome / Microsoft Edge / Mozilla Firefox
- HTML
- CSS
- JavaScript

---

## System Architecture

```
                +----------------------+
                |     HTML Web Page    |
                |   (Browser Dashboard)|
                +----------+-----------+
                           |
                     MQTT Publish
                           |
                    Mosquitto Broker
                           |
                    MQTT Subscribe
                           |
                     +-----+------+
                     |    ESP32    |
                     +-----+------+
                           |
                     Relay Module
                           |
                  Home Appliances
```

---

## Working Principle

1. The user opens the HTML webpage in a web browser.
2. The webpage connects to the MQTT broker.
3. The user clicks an ON/OFF button.
4. JavaScript publishes an MQTT message.
5. The Mosquitto broker forwards the message.
6. The ESP32 receives the MQTT command.
7. The ESP32 switches the corresponding relay.
8. The connected appliance turns ON or OFF.
9. The ESP32 publishes the updated device status.
10. The webpage receives the status and updates the interface in real time.

---

## MQTT Topics

| Topic | Purpose |
|--------|---------|
| home/light | Light Control |
| home/fan | Fan Control |
| home/relay1 | Relay 1 |
| home/relay2 | Relay 2 |
| home/status | Device Status |

---

## Project Folder Structure

```
Home_Automation/
│
├── index.html
├── style.css
├── script.js
├── images/
│
├── esp32/
│   └── HomeAutomation.ino
│
├── README.md
│
└── screenshots/
```

---

## User Interface

The HTML dashboard includes:

- Light ON/OFF Button
- Fan ON/OFF Button
- Device Status Display
- MQTT Connection Status
- Responsive Layout
- Real-Time Updates

---

## ESP32 Functionality

The ESP32 performs the following operations:

- Connects to the Wi-Fi network.
- Connects to the MQTT broker.
- Subscribes to appliance control topics.
- Receives commands from the webpage.
- Controls relay modules.
- Publishes appliance status to MQTT topics.
- Maintains continuous communication with the web interface.

---

## Advantages

- Easy to use
- Low-cost implementation
- Wireless control
- Browser-based access
- Real-time communication
- Scalable design
- Platform independent
- Suitable for IoT applications

---

## Applications

- Smart Home Automation
- Office Automation
- Smart Classrooms
- Industrial Monitoring
- Laboratory Automation
- Hostel Room Automation
- Remote Appliance Control

---

## Future Enhancements

- Mobile application support
- Voice Assistant Integration
- User Authentication
- Cloud Database Integration
- Energy Consumption Monitoring
- Automatic Scheduling
- Sensor-Based Automation
- AI-Based Smart Control
- Notification and Alert System

---

## Output

- Users can control home appliances from a web browser.
- ESP32 receives commands instantly through MQTT.
- Appliances respond in real time.
- Device status is updated automatically on the webpage.
- Multiple appliances can be controlled simultaneously.

---

## Conclusion

The **Home Automation Using HTML** project demonstrates a simple, efficient, and cost-effective IoT solution for controlling household appliances through a web browser. By integrating an HTML interface with an ESP32 and MQTT communication, users can remotely monitor and control connected devices in real time. The modular architecture makes the system easy to expand with additional appliances, sensors, and advanced automation features. This project serves as an excellent foundation for developing modern smart home applications using web technologies and embedded systems.
