# Smart Factory IoT Monitoring using ESP32 and ThingsBoard

## Project Overview

This project implements a Smart Factory IoT Monitoring System using ESP32 and the ThingsBoard Cloud platform. The system consists of two independent monitoring modules:

* **Production Unit Monitoring**
* **Warehouse Monitoring**

Each ESP32 device collects real-time sensor data from its respective environment and transmits the telemetry to ThingsBoard over Wi-Fi using the MQTT protocol. The dashboard enables centralized monitoring of factory operations, environmental conditions, equipment status, and warehouse safety.

---

## Objectives

* Monitor production and warehouse environments in real time.
* Upload sensor data to the ThingsBoard Cloud.
* Detect abnormal environmental conditions.
* Generate alerts for unsafe operating conditions.
* Visualize telemetry through a cloud dashboard.

---

# Production Monitoring Module

## Features

* Monitors production temperature and humidity.
* Detects worker movement using a PIR sensor.
* Measures object distance using an ultrasonic sensor.
* Measures ambient light intensity using an LDR.
* Automatically turns on an LED when humidity exceeds the configured threshold.
* Uploads all telemetry data to ThingsBoard.

### Sensors Used

| Sensor                    | Purpose                             |
| ------------------------- | ----------------------------------- |
| DHT11                     | Temperature and Humidity Monitoring |
| PIR Sensor                | Motion Detection                    |
| HC-SR04 Ultrasonic Sensor | Distance Measurement                |
| LDR                       | Light Intensity Measurement         |
| LED                       | Humidity Status Indicator           |

### GPIO Connections

| Device             | ESP32 GPIO |
| ------------------ | ---------: |
| DHT11              |    GPIO 13 |
| PIR Sensor         |    GPIO 14 |
| Ultrasonic Trigger |    GPIO 25 |
| Ultrasonic Echo    |    GPIO 26 |
| LDR                |    GPIO 39 |
| LED                |     GPIO 2 |

### Telemetry Sent

* Temperature
* Humidity
* PIR
* Ultrasonic
* LDR

---

# Warehouse Monitoring Module

## Features

* Monitors warehouse temperature and humidity.
* Detects worker or intruder movement.
* Detects abnormal rack vibration.
* Generates warehouse safety alerts.
* Uploads all telemetry data to ThingsBoard.

### Sensors Used

| Sensor                  | Purpose                             |
| ----------------------- | ----------------------------------- |
| DHT11                   | Temperature and Humidity Monitoring |
| PIR Sensor              | Motion Detection                    |
| SW-420 Vibration Sensor | Rack Vibration Detection            |
| LED                     | Alert Indicator                     |

### GPIO Connections

| Device           | ESP32 GPIO |
| ---------------- | ---------: |
| DHT11            |    GPIO 14 |
| PIR Sensor       |    GPIO 25 |
| Vibration Sensor |    GPIO 26 |
| LED              |     GPIO 2 |

### Telemetry Sent

* Warehouse_Temperature
* Warehouse_Humidity
* Motion_Status
* Rack_Vibration
* Warehouse_Alert

---

# Software Requirements

* Arduino IDE
* ESP32 Board Package
* ThingsBoard Library
* Arduino MQTT Client Library
* DHT Sensor Library

---

# Hardware Requirements

* ESP32 Development Board (2 Units)
* DHT11 Temperature & Humidity Sensors
* PIR Motion Sensors
* HC-SR04 Ultrasonic Sensor
* SW-420 Vibration Sensor
* LDR Sensor
* LEDs
* Breadboard and Jumper Wires
* Wi-Fi Network

---

# ThingsBoard Cloud

**Server**

```text
mqtt.thingsboard.cloud
```

Each ESP32 uses a unique Device Access Token to authenticate and upload telemetry.

---

# System Workflow

```text
Production ESP32
│
├── Read Temperature
├── Read Humidity
├── Read Motion
├── Read Distance
├── Read Light Intensity
└── Upload Telemetry
          │
          ▼

     ThingsBoard Cloud
          ▲
          │

Warehouse ESP32
│
├── Read Temperature
├── Read Humidity
├── Detect Motion
├── Detect Rack Vibration
├── Generate Alert
└── Upload Telemetry
```

---

# Project Structure

```text
Smart_Factory_IoT/
│
├── Production/
│   ├── Production.ino
│   └── README.md
│
├── Warehouse/
│   ├── Warehouse.ino
│   └── README.md
│
├── Images/
│
└── Main_README.md
```

---

# Applications

* Smart Manufacturing
* Industrial Automation
* Warehouse Safety Monitoring
* Predictive Maintenance
* Inventory Storage Monitoring
* Industrial IoT
* Factory Environment Monitoring

---

# Future Enhancements

* Integrate gas, smoke, and fire sensors.
* Add relay control for industrial equipment.
* Send SMS or email notifications for critical alerts.
* Store historical telemetry for predictive analytics.
* Develop a mobile application for remote monitoring.
* Integrate with Node-RED and Home Assistant.
* Enable secure MQTT communication using TLS/SSL.

---

# Author

**Student Name:** Ramanesh M

**Project Title:** Smart Factory IoT Monitoring using ESP32 and ThingsBoard

**Technologies Used:** ESP32, Arduino IDE, Wi-Fi, MQTT, ThingsBoard Cloud, DHT11, PIR Sensor, HC-SR04 Ultrasonic Sensor, SW-420 Vibration Sensor, LDR
