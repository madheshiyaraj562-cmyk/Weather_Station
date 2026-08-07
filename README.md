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
