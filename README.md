# 🌦️ AI Voice Weather Station

> A wall-mounted, open-source smart weather station combining embedded systems, environmental sensing, voice interaction, cloud APIs, IoT connectivity, custom electronics, and 3D-printed mechanical design.

<p align="center">
  <a href="https://projectmasterpiecev1.framer.website/">
    🌐 <strong>PROJECT WEBSITE</strong>
  </a>
</p>

---

## 📖 Overview

The **AI Voice Weather Station** is an ESP32-S3-based environmental monitoring and voice-assistant system designed to go beyond a conventional weather station.

The device combines local environmental sensing with information retrieved from online APIs. Users can interact with the system using natural voice commands, while the device can provide spoken responses, visual status feedback, and cloud-based historical data.

The long-term goal is to build a polished, wall-mounted device that combines:

- Embedded systems
- IoT connectivity
- Environmental sensing
- Voice interaction
- API integration
- Artificial intelligence
- Cloud data visualization
- Custom PCB design
- 3D-printed mechanical design
- OTA firmware updates
- Open-source documentation

---

# 🎯 Project Goals

The primary objectives are to build a device that can:

1. Measure local environmental conditions.
2. Retrieve live weather and air-quality information from online services.
3. Understand natural-language voice commands.
4. Respond using synthesized speech.
5. Display important information locally.
6. Upload sensor data to Arduino Cloud.
7. Provide historical graphs and comparisons.
8. Support wireless firmware updates.
9. Operate as a self-contained wall-mounted device.
10. Be fully documented and released as an open-source project.

---

# 🧠 System Architecture

```text
                         ┌───────────────────────┐
                         │      USER VOICE       │
                         └───────────┬───────────┘
                                     │
                                     ▼
                         ┌───────────────────────┐
                         │   INMP441 MICROPHONE  │
                         └───────────┬───────────┘
                                     │
                                     ▼
                         ┌───────────────────────┐
                         │      ESP32-S3         │
                         │                       │
                         │  Command Processing   │
                         │  Sensor Processing    │
                         │  Wi-Fi Communication  │
                         │  API Communication    │
                         │  Device Control       │
                         └───────┬───────┬───────┘
                                 │       │
                    ┌────────────┘       └─────────────┐
                    ▼                                  ▼
          ┌──────────────────┐              ┌──────────────────┐
          │ Local Sensors    │              │ Internet / APIs  │
          │                  │              │                  │
          │ BME280           │              │ Weather          │
          │ Temperature      │              │ Air Quality      │
          │ Humidity         │              │ Time             │
          │ Pressure         │              │ Speech-to-Text   │
          └────────┬─────────┘              │ AI               │
                   │                        │ Text-to-Speech   │
                   │                        └────────┬─────────┘
                   └──────────────┬─────────────────┘
                                  ▼
                         ┌───────────────────────┐
                         │    ESP32 Processing   │
                         └───────┬───────┬───────┘
                                 │       │
                         ┌───────┘       └────────┐
                         ▼                        ▼
                ┌─────────────────┐      ┌──────────────────┐
                │ Audio Output    │      │ Local Interface  │
                │                 │      │                  │
                │ MAX98357A       │      │ OLED             │
                │ 3W Speaker      │      │ RGB LED          │
                └─────────────────┘      │ Touch Sensor     │
                                         └──────────────────┘
                                                 │
                                                 ▼
                                      ┌────────────────────┐
                                      │   Arduino Cloud    │
                                      │                    │
                                      │ Data Logging       │
                                      │ Graphs             │
                                      │ Device Status      │
                                      └────────────────────┘


| Component               | Purpose                                                   |
| ----------------------- | --------------------------------------------------------- |
| **ESP32-S3**            | Main controller, Wi-Fi, processing, and API communication |
| **BME280**              | Temperature, humidity, and atmospheric pressure           |
| **INMP441**             | Digital I²S microphone for voice input                    |
| **MAX98357A**           | I²S audio amplifier                                       |
| **3W 4Ω Speaker**       | Spoken audio output                                       |
| **WS2812B RGB LED**     | Device status and environmental feedback                  |
| **TTP223 Touch Sensor** | User interaction and wake/sleep control                   |
| **OLED Display**        | Local sensor and system information                       |
| **Li-ion/LiPo Battery** | Portable power source                                     |
| **TP4056 USB-C Module** | Battery charging                                          |
| **USB-C Power Input**   | External power                                            |




Optional Components
DFPlayer Mini
Secondary ESP32 display using ESP-NOW
Battery fuel gauge
Ambient light sensor
Additional environmental sensors
🌡️ Environmental Monitoring

The BME280 provides local environmental measurements including:

Temperature
Relative humidity
Atmospheric pressure

Additional values can be calculated from these measurements, including:

Heat index
Environmental status
Historical trends
Temperature comparisons

🎤 Voice Assistant

A major objective of the project is to allow the user to interact with the station through voice.

The INMP441 microphone captures the user's command, which is then processed by the system.

Example Commands
"What is the temperature?"

"What's the weather today?"

"What's the humidity?"

"What's the air quality?"

"What's the wind speed?"

"Will it rain today?"

"What is the heat index?"

"What time is it?"

"What is today's date?"

"What's the battery level?"

"Is the device online?"
The system is designed to support natural-language queries rather than requiring fixed command phrases.
