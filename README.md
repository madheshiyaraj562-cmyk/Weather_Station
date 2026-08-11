🌦️ Smart Weather Station

A voice-controlled, connected weather station built around the ESP32-S3 — combining real-time environmental sensing, online weather data, speech recognition, physical interaction, and spoken responses in a custom wall-mounted device.

---

<p align="center"> <a href="https://projectmasterpiecev1.framer.website/"> <strong>🌐 Visit the Project Website</strong> </a> </p>

---

## 📖 Overview

The **AI-Powered Smart Weather Station** is an ESP32-S3-based smart environmental monitoring system designed to make weather information accessible through natural voice interaction.

Instead of relying only on a screen, the station can **listen to a user's request, retrieve live weather information, and respond using spoken audio**. It combines environmental sensors, online APIs, a DFPlayer Mini audio system, touch controls, clap detection, RGB lighting, and an Arduino Cloud dashboard into a single integrated device.

The project is designed as an **open-source hardware and software platform**, including firmware, PCB files, enclosure designs, audio assets, wiring documentation, and setup guides.

---

## ✨ What Makes It Different?

Most DIY weather stations simply measure temperature and display numbers.

This project is designed to behave more like a **physical voice-enabled weather assistant**.

You can interact with it using commands such as:

> **"Clap "**

The ESP32-S3 processes the request, obtains the required information, and assembles a spoken response from a library of **98 carefully mapped audio clips** stored on a DFPlayer Mini.

The result is a compact device that combines:

**Sensors → Internet → API → Data Processing → Voice Response**

---

# 🚀 Features

### 🎙️ Voice Interaction

The station supports voice-based weather queries using speech recognition.

Supported requests include:

* 🌡️ Temperature
* 💧 Humidity
* 📈 Atmospheric pressure
* ⛰️ Altitude
* 🌦️ Current weather
* 💨 Wind speed
* ☀️ UV index
* 🌫️ Air quality
* 🕐 Time
* 📅 Date
* 📊 Complete weather report
* 🎵 Music
* 😴 Sleep mode

The system uses a physical touch/button interaction to begin listening rather than continuously monitoring for a wake word.

---

### 🌍 Live Weather & Air Quality

Weather information is retrieved from **Open-Meteo**, providing live online weather data without requiring a conventional weather API key.

The system can retrieve information such as:

* Temperature
* Relative humidity
* Wind speed
* Weather conditions
* UV index
* Air-quality information
* Sunrise and sunset information
* Other forecast data depending on the configured API request

The default prototype location is **Gorakhpur, Uttar Pradesh, India**, but the coordinates can be changed in the firmware.

---

### 🔊 Offline Spoken Responses

Instead of generating speech dynamically on the ESP32, the project uses a **DFPlayer Mini with a curated library of approximately 98 MP3 clips**.

Individual clips can be combined to construct complete responses.

For example:

```text
"The temperature is"
+ "thirty"
+ "one"
+ "degrees Celsius"
```

This approach provides:

* Fast response playback
* Consistent audio quality
* No on-device TTS processing
* Reduced memory requirements
* Offline availability for the response library
* Easy customization of individual phrases

The audio system is one of the most important parts of the project because it allows the ESP32-S3 to produce dynamic spoken sentences using a relatively small collection of recordings.

---

### 👋 Physical Touch Controls

Dedicated touch sensors provide direct interaction with the device.

Possible actions include:

* Wake the station
* Start a voice interaction
* Ask for the time
* Start music
* Control device states

This allows the station to remain inactive when not being used.

---

### 👏 Clap Detection

The station also includes clap detection.

During music playback, a clap can be detected and used as an interaction event, allowing the user to stop playback without pressing a physical button.

---

### 💡 RGB Status Lighting

A WS2812B RGB LED provides visual feedback about the current state of the station.

Different lighting states can represent:

* 🟢 Idle / ready
* 🔵 Listening
* 🟡 Confirmation / interaction
* 🔴 Alert
* 🌈 Other system states

The lighting system makes the device easier to understand without requiring a screen.

---

### ☁️ Arduino Cloud

The station can connect to **Arduino Cloud** for remote monitoring and control.

The dashboard can be used for:

* Environmental data
* Device status
* Remote controls
* Monitoring
* OTA firmware updates

This provides a second interface alongside the physical device.

---

### 🔋 Battery Powered

The station is designed to operate from a rechargeable Li-ion/LiPo battery.

A **TP4056 USB-C charging circuit** provides convenient charging and power management for the prototype.

---

### 📡 OTA Updates

Firmware can be updated wirelessly, reducing the need to physically connect the ESP32-S3 every time the software needs to be changed.

---

### 🖥️ Optional Secondary Display

An additional ESP32-based display can be connected using **ESP-NOW**.

This allows information to be displayed on a separate screen while the main station remains focused on voice interaction.

---

# 🧠 System Architecture

The overall system can be thought of as several interconnected layers:

```text
                    ┌─────────────────────┐
                    │       USER          │
                    │ Voice / Touch / Clap│
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │      ESP32-S3       │
                    │   Main Controller   │
                    └──────┬───────┬──────┘
                           │       │
              ┌────────────┘       └─────────────┐
              ▼                                  ▼
       ┌──────────────┐                    ┌──────────────┐
       │   Sensors    │                    │    Wi-Fi     │
       │ BME280/DHT22 │                    │   Internet   │
       └──────────────┘                    └──────┬───────┘
                                                  │
                          ┌───────────────────────┼──────────────┐
                          ▼                       ▼              ▼
                    ┌───────────┐         ┌────────────┐  ┌────────────┐
                    │ Open-Meteo│         │     API    │  │  Arduino   │
                    │ Weather   │         │            │  │   Cloud    │
                    └───────────┘         └────────────┘  └────────────┘
                          │                       │
                          └───────────┬───────────┘
                                      ▼
                              ┌───────────────┐
                              │ Response Logic│
                              └───────┬───────┘
                                      │
                                      ▼
                              ┌───────────────┐
                              │  DFPlayer Mini│
                              │  98 MP3 Clips │
                              └───────┬───────┘
                                      │
                                      ▼
                              ┌───────────────┐
                              │ MAX98357A Amp │
                              └───────┬───────┘
                                      │
                                      ▼
                                  🔊 Speaker
```

---

# 🛠️ Hardware

## Main Components

| Component                   | Function                     |
| --------------------------- | ---------------------------- |
| **Wavwshare ESP32-S3 ZERO** | Main microcontroller         |
| **BME280 / DHT22 + BMP180** | Environmental sensing        |
| **INMP441 / MAX9814**       | Microphone input             |
| **DFPlayer Mini**           | MP3 voice and music playback |
| **MAX98357A**               | Audio amplification          |
| **3W Speaker**              | Voice/audio output           |
| **WS2812B**                 | RGB status lighting          |
| **TTP223**                  | Touch interaction            |
| **Li-ion/LiPo Battery**     | Portable power               |
| **TP4056**                  | USB-C battery charging       |
| **OLED**                    | Optional visual interface    |

---

# 📌 Prototype Pin Configuration

The current prototype uses the following connections:

| Device             | ESP32-S3 Pin |
| ------------------ | ------------ |
| DHT22              | GPIO 7       |
| BMP180 SDA         | GPIO 8       |
| BMP180 SCL         | GPIO 9       |
| MAX9814 microphone | GPIO 4       |
| DFPlayer RX        | GPIO 6       |
| DFPlayer TX        | GPIO 5       |

> ⚠️ **Note:** The pin mapping represents the current prototype and may change in future PCB revisions.

A battery-voltage divider is planned but is **not currently connected in the prototype**.

---

# 🔊 Audio System

One of the project's key design decisions is the use of a **pre-recorded audio response library**.

The current library contains approximately **98 mapped MP3 files**.

The files are stored on the DFPlayer Mini's microSD card and can be assembled into different spoken responses by the ESP32.

Example:

```text
0085 → Listening chime

Temperature:
"the temperature is"
+ number
+ "degrees Celsius"

0086 → Sleep / completion chime
```

This architecture makes it possible to generate many different sentences without requiring a full text-to-speech engine to run on the microcontroller.

It also makes the voice system highly customizable.

---

# 🎵 Music Mode

The DFPlayer Mini is not limited to voice responses.

It can also play music stored on the microSD card.

The station supports:

* Music playback
* Track selection
* Touch-based control
* Clap-to-stop interaction
* Separate audio cues for system events

This turns the weather station into a small **ambient smart speaker**, rather than simply a weather sensor.

---

# 🌐 APIs & Online Services

### Weather

**Open-Meteo**

Used for weather and environmental forecast data.

Advantages:

* Free
* No conventional API key required
* Simple REST API
* Suitable for embedded projects

### Speech Recognition

**Google Cloud Speech-to-Text**

Used to convert recorded speech into text before command processing.

### Cloud Dashboard

**Arduino Cloud**

Used for remote monitoring, controls, and OTA functionality.

---

# 📁 Repository Structure

```text
.
├── firmware/
│   └── ESP32-S3 source code
│
├── hardware/
│   ├── schematic/
│   ├── PCB/
│   ├── Gerbers/
│   ├── BOM/
│   └── EasyEDA project
│
├── enclosure/
│   ├── STL/
│   └── assembly files
│
├── audio/
│   ├── 0001.mp3
│   ├── 0002.mp3
│   ├── ...
│   └── 0101.mp3
│
├── prototypes/
│   └── uno-r4-wifi/
│
├── docs/
│   ├── wiring/
│   ├── API setup/
│   ├── Arduino Cloud/
│   └── troubleshooting/
│
├── LICENSE
└── README.md
```

---

# 🚀 Getting Started

## 1. Assemble the Hardware

Connect the ESP32-S3, sensors, microphone, DFPlayer Mini, amplifier, speaker, touch sensors, and RGB LED according to the wiring documentation.

---

## 2. Prepare the Audio SD Card

Format a microSD card as **FAT32**.

Place the MP3 files inside the appropriate folder and maintain the required numerical filenames:

```text
0001.mp3
0002.mp3
0003.mp3
...
0101.mp3
```

The filenames are important because the firmware uses the track numbers to construct spoken responses.

---

## 3. Configure the Firmware

Before uploading the firmware, configure:

* Wi-Fi SSID
* Wi-Fi password
* Weather coordinates
* Time zone
* Arduino Cloud configuration
* Audio settings

---

## 4. Upload the Firmware

The firmware can be developed and uploaded using:

* Arduino IDE
* PlatformIO

The project is primarily designed for the **ESP32-S3**.

---

## 5. Configure Arduino Cloud

Connect the device to Arduino Cloud and configure the required variables and dashboard controls.

The dashboard provides a remote interface for monitoring and controlling the station.

---

## 6. Assemble the Enclosure

The station uses a custom **hexagonal wall-mounted enclosure** designed specifically around the electronics.

The enclosure includes:

* 🔊 Speaker grille
* 🎙️ Microphone opening
* 💡 RGB diffuser
* 🖥️ OLED window
* 🔌 USB-C opening
* 🔋 Battery compartment
* 🌬️ Sensor airflow vents
* 🔩 PCB mounting posts

---

# 🧪 Development Status

| Feature                   | Status                   |
| ------------------------- | ------------------------ |
| ESP32-S3 firmware         | 🟢 Implemented           |
| Weather API               | 🟢 Implemented           |
| Environmental sensing     | 🟢 Implemented           |
| DFPlayer voice responses  | 🟢 Implemented           |
| 98-clip audio library     | 🟢 Implemented           |
| Touch controls            | 🟢 Implemented           |
| Clap detection            | 🟢 Implemented           |
| RGB status system         | 🟢 Implemented           |
| Arduino Cloud             | 🟢 Implemented / Testing |
| OTA updates               | 🟢 Implemented / Testing |
| Custom PCB                | 🟡 In development        |
| 3D enclosure              | 🟡 In development        |
| Secondary ESP-NOW display | 🟡 Planned               |
| Wake-word detection       | 🔵 Planned               |
| Offline fallback          | 🔵 Planned               |

---

# 🗺️ Roadmap

Future versions are planned to include:

* [ ] Indoor vs. outdoor weather comparison
* [ ] Severe weather alerts
* [ ] Sunrise/sunset announcements
* [ ] Low-battery voice warnings
* [ ] Home Assistant integration
* [ ] Alexa / Google Assistant compatibility
* [ ] Multi-city weather queries
* [ ] Automatic display brightness
* [ ] Wake-word detection
* [ ] Fully offline fallback mode
* [ ] Improved enclosure and PCB revision
* [ ] More natural voice responses
* [ ] Additional environmental sensors

---

# 🔐 Privacy & Listening Behavior

The station is **not designed to continuously listen for a wake word** while sleeping.

A physical interaction is required to initiate a voice interaction.

The typical flow is:

```text
Touch / Button
      ↓
Listening Chime
      ↓
Record Voice
      ↓
     API
      ↓
Command Processing
      ↓
Weather / Device Action
      ↓
Audio Response
      ↓
Completion / Sleep Chime
```

This provides a more deliberate interaction model and avoids continuous microphone monitoring while the device is idle.

---

# 🧩 Simpler Arduino Uno R4 WiFi Prototype

The repository also contains a simpler version based on the **Arduino Uno R4 WiFi**.

Unlike the main ESP32-S3 system, this prototype does not require the complete physical sensor and voice hardware.

Instead, it retrieves weather information directly from an online API and presents the data through a display and web interface.

It is useful for:

* Testing API connectivity
* Testing weather-data parsing
* Experimenting with dashboards
* Understanding the basic architecture
* Rapid prototyping

---

# 🏗️ Design Philosophy

The project follows a few important principles:

### 1. Modular hardware

Individual components such as the microphone, audio system, sensors, display, and lighting can be replaced or upgraded.

### 2. Hybrid processing

The ESP32-S3 handles real-time hardware interaction and device logic, while cloud services provide capabilities that are impractical to run entirely on a microcontroller.

### 3. Offline audio

Pre-recorded audio keeps the response system lightweight and reliable.

### 4. Open hardware

The long-term goal is to publish the PCB, enclosure, firmware, documentation, and supporting files so other makers can reproduce and modify the project.

---

# 👨‍💻 Built By

**Raj Madheshiya**

Designed, wired, programmed, tested, and voice-tested as an end-to-end hardware/software project.

From the ESP32-S3 firmware and sensor integration to the custom PCB concept, enclosure, audio library, cloud dashboard, and physical interaction system, the project is being developed as a complete open-source smart-device platform.

---

# 📜 License

This project is open source.

See [`LICENSE`](LICENSE) for the complete license terms.

---

# ⭐ Support the Project

If you find this project useful or interesting:

* ⭐ Star the repository
* 🍴 Fork it
* 🛠️ Build your own version
* 🐛 Report issues
* 💡 Suggest improvements
* 🔧 Submit pull requests

More documentation, printable enclosure files, PCB files, and build instructions will be added as development continues.

---

## 🎥 Coming Soon

A complete build video will document the project from:

**Concept → Circuit → PCB → Firmware → Audio System → Enclosure → Final Assembly → Testing**

Printable enclosure files will also be published for makers who want to build their own version.

---

> **🌦️ A weather station that doesn't just show the weather — it talks to you.**

---

### Part of MasterPiece Project BY Raj Madheshiya.
### MIT License.
