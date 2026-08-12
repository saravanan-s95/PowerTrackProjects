# ESP32 Home Automation using MQTT (EMQX Public Broker)

## Project Overview

This project demonstrates a basic Home Automation system using an ESP32 and the MQTT protocol. The ESP32 connects to a Wi-Fi network and subscribes to multiple MQTT topics hosted on the public EMQX broker. Whenever a message is published to one of the subscribed topics, the ESP32 receives it and displays the corresponding topic and payload on the Arduino IDE Serial Monitor.

This project serves as a foundation for building IoT-based home automation systems where devices can be remotely controlled using MQTT.

---

## Features

* Connects ESP32 to a Wi-Fi network.
* Connects to the public EMQX MQTT broker.
* Automatically reconnects if the MQTT connection is lost.
* Subscribes to multiple MQTT topics.
* Receives and processes MQTT messages in real time.
* Displays the received topic and payload on the Serial Monitor.
* Supports independent control topics for multiple home appliances.

---

## Technologies Used

* ESP32 Development Board
* Arduino IDE
* Wi-Fi
* MQTT Protocol
* EMQX Public Broker
* PubSubClient Library

---

## Hardware Requirements

* ESP32 Development Board
* USB Cable
* Wi-Fi Connection
* PC/Laptop with Arduino IDE

---

## Software Requirements

* Arduino IDE
* ESP32 Board Package
* PubSubClient Library
* MQTT Client (MQTTX, MQTT Explorer, or any MQTT Publisher)

---

## MQTT Broker Details

**Broker Address**

```text
broker.emqx.io
```

**Port**

```text
1883
```

---

## MQTT Topics

| Topic        | Description                  |
| ------------ | ---------------------------- |
| `Hall/light` | Controls the hall light      |
| `Hall/fan`   | Controls the hall fan        |
| `Hall/TV`    | Controls the hall television |

---

## Supported Payloads

```text
ON
OFF
```

---

## Project Workflow

1. ESP32 powers on.
2. Connects to the configured Wi-Fi network.
3. Connects to the EMQX MQTT broker.
4. Subscribes to the following topics:

   * `Hall/light`
   * `Hall/fan`
   * `Hall/TV`
5. Waits for MQTT messages.
6. When a message is received, the callback function is executed.
7. The topic and payload are displayed on the Arduino IDE Serial Monitor.

---

## MQTT Message Flow

```text
MQTT Publisher
      │
      │ Publish (ON/OFF)
      ▼
+----------------------+
|  EMQX MQTT Broker    |
|   broker.emqx.io     |
+----------------------+
          │
          ▼
+----------------------+
|       ESP32          |
|  MQTT Subscriber     |
+----------------------+
          │
          ▼
Arduino IDE Serial Monitor
```

---

## Example Publish Commands

### Turn Hall Light ON

```bash
mosquitto_pub -h broker.emqx.io -t Hall/light -m "ON"
```

### Turn Hall Light OFF

```bash
mosquitto_pub -h broker.emqx.io -t Hall/light -m "OFF"
```

### Turn Hall Fan ON

```bash
mosquitto_pub -h broker.emqx.io -t Hall/fan -m "ON"
```

### Turn Hall TV OFF

```bash
mosquitto_pub -h broker.emqx.io -t Hall/TV -m "OFF"
```

---

## Expected Serial Monitor Output

```text
WiFi Connected
Connecting to MQTT...
Connected

Topic: Hall/light
Payload : ON

Topic: Hall/fan
Payload : OFF

Topic: Hall/TV
Payload : ON
```

---

## Project Structure

```text
ESP32_Home_Automation_MQTT/
│
├── ESP32_Home_Automation_MQTT.ino
├── README.md
└── images/
```

---

## Applications

* Home Automation
* Smart Home Systems
* IoT Device Communication
* MQTT Learning Projects
* Remote Appliance Monitoring
* Wireless Device Control

---

## Future Enhancements

* Control actual LEDs, relays, and appliances.
* Publish sensor data (temperature, humidity, motion, etc.).
* Add OLED or LCD status display.
* Secure communication using MQTT authentication and TLS.
* Integrate with Node-RED, Home Assistant, or ThingsBoard.
* Develop a mobile application for remote control.

---

## Author

**Name:** Ramanesh M

**Project Title:** ESP32 Home Automation using MQTT (EMQX Public Broker)

**Technologies:** ESP32, Arduino IDE, Wi-Fi, MQTT, EMQX, PubSubClient
