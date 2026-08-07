# 🌦️ AI Voice Weather Station

> A wall-mounted smart weather station that combines environmental sensing, voice interaction, Internet APIs, AI, cloud monitoring, custom electronics, and 3D-printed mechanical design.


<p align="center">
  <a href="https://projectmasterpiecev1.framer.website/">
    🌐 <strong>PROJECT WEBSITE</strong>
  </a>
</p>

---

## 📖 About the Project

I wanted to build a weather station that was more than just a screen showing temperature and humidity.

The idea behind this project is to create a **smart, voice-controlled weather station** that can measure its surroundings, connect to the Internet, understand what the user is asking, and respond with useful information.

The main controller is an **ESP32-S3**, which connects the hardware together and handles communication with sensors, online APIs, and other services.

The station can combine information from its own sensors with information from the Internet. For example, the BME280 can measure the temperature and humidity inside the room, while an online weather API can provide the outdoor forecast, wind speed, rain probability, and other information.

Eventually, I want the device to feel less like an electronics experiment and more like a **real consumer device** that could be mounted on a wall and used every day.

---

# 🎯 What I Want This Project to Do

The main idea is simple:

**You should be able to walk up to the weather station and ask it something.**

For example:

> "What's the temperature?"

or

> "Will it rain today?"

The system should be able to understand the request, collect the required information, and respond naturally through the speaker.

At the same time, the station continuously monitors its local environment and can send data to the cloud for graphs and long-term analysis.

The project is therefore not just about weather measurement. It combines several areas of engineering:

- 🔌 Embedded systems
- 📡 IoT and wireless communication
- 🌡️ Environmental sensing
- 🎤 Digital audio
- 🗣️ Speech recognition
- 🤖 AI
- 🌐 Internet APIs
- ☁️ Cloud computing
- 🔋 Power management
- 🖥️ PCB design
- 🧩 CAD and 3D printing
- 🔄 OTA firmware updates

---

# 🧠 How the System Works

The system can be thought of as several parts working together.

First, the **INMP441 microphone** listens for the user's voice.

The audio is then processed by the system and converted into text using a speech-to-text service.

The **ESP32-S3** acts as the main controller. It decides what information is needed and communicates with the appropriate sensors or online services.

For example, if the user asks:

> "What's the temperature?"

the ESP32 can read the temperature directly from the **BME280**.

If the user asks:

> "Will it rain today?"

the device needs information that cannot be measured by the BME280, so it can request forecast data from an online weather API.

After processing the request, the response can be converted into speech using a text-to-speech service and played through the speaker.

At the same time, the OLED and RGB LED can provide visual feedback.

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
                         │       ESP32-S3        │
                         │                       │
                         │ Command Processing    │
                         │ Sensor Processing     │
                         │ Wi-Fi Communication   │
                         │ API Communication     │
                         │ Device Control        │
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
```

---

# 🔧 Hardware

The hardware is designed around the ESP32-S3.

## Main Components

| Component | What it does |
|---|---|
| **ESP32-S3** | Main controller and Wi-Fi communication |
| **BME280** | Measures temperature, humidity, and pressure |
| **INMP441** | Captures voice through an I²S digital microphone |
| **MAX98357A** | Drives the speaker using I²S audio |
| **3W 4Ω Speaker** | Produces spoken responses |
| **WS2812B RGB LED** | Shows system and environmental status |
| **TTP223 Touch Sensor** | Provides touch-based interaction |
| **OLED Display** | Displays local information |
| **Li-ion/LiPo Battery** | Allows battery-powered operation |
| **TP4056 USB-C Module** | Handles battery charging |
| **USB-C** | Provides external power |

### Optional Hardware

Some parts are optional and depend on how the final version develops:

- DFPlayer Mini
- Secondary ESP32 display
- ESP-NOW communication
- Battery fuel gauge
- Ambient light sensor
- Additional sensors

---

# 🌡️ Environmental Monitoring

The **BME280** is responsible for measuring the local environment.

It provides three important measurements:

- 🌡️ Temperature
- 💧 Humidity
- 🌬️ Atmospheric pressure

These values can also be used to calculate or analyze additional information such as heat index, environmental conditions, and long-term trends.

One important design consideration is **sensor placement**.

Because the ESP32, voltage regulators, and audio amplifier can produce heat, the BME280 needs to be positioned so that it measures the surrounding environment rather than the heat generated by the electronics.

---

# 🎤 Voice Interaction

One of the features I want to make most interesting is the voice interface.

Instead of having to press buttons or navigate menus, the user can simply ask the station a question.

For example:

```text
"What is the temperature?"

"What's the weather today?"

"What's the humidity?"

"What's the air quality?"

"What's the wind speed?"

"Will it rain today?"

"What is the heat index?"

"What time is it?"

"What's the battery level?"

"Is the device online?"
```

The long-term goal is to support **natural language**, meaning the user shouldn't have to memorize specific commands.

---

# 🌐 Internet & API Integration

The ESP32-S3 connects to the Internet through Wi-Fi.

This allows the station to access information that its physical sensors cannot measure.

For example:

### Weather

The station can retrieve:

- Current weather
- Forecasts
- Wind speed
- Rain probability
- UV index
- Cloud cover
- Weather conditions

### Air Quality

The system can retrieve:

- AQI
- PM2.5
- PM10

### Time

Network time services can provide:

- Current time
- Date
- Day
- Automatic clock synchronization

This combination of **local sensors + Internet data** is one of the main ideas behind the project.

---

# 🤖 AI & Speech Processing

The voice system can use several stages.

```text
User speaks
     │
     ▼
INMP441 Microphone
     │
     ▼
Speech-to-Text
     │
     ▼
AI / Intent Processing
     │
     ▼
Weather / Sensor / API Data
     │
     ▼
Response Generation
     │
     ▼
Text-to-Speech
     │
     ▼
MAX98357A
     │
     ▼
Speaker
```

The AI layer is intended to help the system understand what the user actually wants rather than simply matching exact words.

For example:

> "Is it going to rain?"

and

> "Do I need an umbrella today?"

could potentially be interpreted as related requests.

---

# 🔊 Audio System

The audio system uses an **INMP441 microphone** for digital audio input and a **MAX98357A** amplifier for audio output.

The amplifier drives a small 3W speaker.

This allows the station to provide spoken responses instead of relying entirely on a display.

An important part of development will be finding the right balance between:

- Microphone sensitivity
- Speaker volume
- Audio quality
- Power consumption
- Voice recognition reliability

---

# 🌈 RGB Status System

The WS2812B RGB LED provides a simple way for the device to communicate its current state.

For example:

| Color | Meaning |
|---|---|
| 🔵 Blue | Normal / cool conditions |
| 🟢 Green | Pleasant conditions |
| 🔴 Red | High temperature |
| 🟠 Orange | Poor air quality |
| 🟣 Purple | Listening |
| 🔷 Cyan | Speaking |
| 🟡 Yellow | Updating |
| 🔴 Flashing | Error |
| ⚫ Off | Sleep mode |

The exact colors can be changed during testing.

---

# 📱 OLED & Touch Interface

Although voice interaction is a major part of the project, I don't want the device to depend entirely on voice.

The OLED can provide quick visual information such as:

- Temperature
- Humidity
- Pressure
- Time
- Weather
- Wi-Fi status
- Battery level
- System messages

The **TTP223 touch sensor** can be used for simple physical interaction, such as waking the device or putting it into sleep mode.

---

# ☁️ Arduino Cloud

The station can also send information to **Arduino Cloud**.

This allows the data to be viewed remotely and makes it possible to create graphs showing how the environment changes over time.

Possible data includes:

```text
Temperature
Humidity
Pressure
Heat Index
Air Quality
Wind Speed
Weather Condition
Battery Percentage
Wi-Fi Signal Strength
Device Uptime
Online / Offline Status
```

This is particularly useful because the project becomes more than a device that shows the current weather.

It can become a **long-term environmental data logger**.

---

# 🔄 OTA Firmware Updates

Because the final device is intended to be mounted on a wall, physically connecting a USB cable every time the firmware needs to be updated would be inconvenient.

That's why I plan to implement **OTA (Over-The-Air) updates**.

The basic idea is:

```text
Computer
   │
   │ Wi-Fi
   ▼
ESP32-S3
   │
   ▼
New Firmware
   │
   ▼
Reboot
   │
   ▼
Updated Device
```

This means future firmware updates can be installed wirelessly.

---

# 📡 ESP-NOW Expansion

Another possible feature is a secondary ESP32 display.

The main weather station could collect and process the information and then transmit selected data to another ESP32 using **ESP-NOW**.

```text
                Main Weather Station
                        │
                        │ ESP-NOW
                        ▼
                 Secondary ESP32
                        │
                        ▼
                     Display
```

This could allow the weather information to be displayed somewhere else in the room.

---

# 🔋 Power System

The station is being designed to work from both USB-C power and a rechargeable battery.

The planned power system includes:

- USB-C 5V input
- Li-ion/LiPo battery
- TP4056 charging module

Power consumption will be an important part of the design, especially when running:

- Wi-Fi
- Microphone
- Speaker
- RGB LEDs
- OLED
- Sensors

Sleep modes and automatic brightness control can help reduce power consumption.

---

# 🏠 Mechanical Design

The final version is planned as a **wall-mounted hexagonal enclosure**.

I want the enclosure to look like an actual finished product rather than a collection of development boards and wires.

The enclosure will need space for:

- PCB
- Speaker
- Microphone
- OLED
- RGB diffuser
- Battery
- USB-C connector
- Touch sensor

It will also need appropriate airflow around the BME280.

The enclosure will be designed using CAD and eventually 3D printed.

---

# 🖥️ Custom PCB

The prototype will initially use individual development boards and modules.

Once the electronics are stable, the next step is to design a custom PCB.

The PCB will bring the different parts of the system together into a single board.

### Planned PCB

The board may include:

- ESP32-S3
- Power management
- Battery connector
- Charging circuitry
- Audio connections
- BME280
- INMP441
- OLED interface
- WS2812B connection
- TTP223 connection
- Programming/debug interface
- Expansion connectors

The PCB will be designed using **KiCad**.

---

# 🧪 Testing

A major part of the project is testing.

I want to test not only whether the device works, but also **how well it works**.

Areas of testing include:

- Sensor accuracy
- Wi-Fi reliability
- API reliability
- Voice recognition
- Speech-to-text accuracy
- Text-to-speech latency
- Speaker quality
- Battery life
- Power consumption
- OTA reliability
- Temperature stability
- BME280 thermal effects
- Cloud synchronization
- Long-term reliability

Where possible, test results will be documented in this repository.

---

# 📊 Development Status

This project is being developed in stages.

| Feature | Status |
|---|---|
| ESP32-S3 | 🟡 In Development |
| BME280 | 🟡 In Development |
| Wi-Fi | 🟡 In Development |
| Weather API | 🟡 In Development |
| Time synchronization | 🟡 In Development |
| Voice input | 🟡 In Development |
| Speech-to-Text | 🟡 Planned |
| AI processing | 🟡 Planned |
| Text-to-Speech | 🟡 Planned |
| OLED | 🟡 Planned |
| RGB status system | 🟡 Planned |
| Arduino Cloud | 🟡 Planned |
| OTA updates | 🟡 Planned |
| ESP-NOW display | ⚪ Optional |
| Custom PCB | 🟡 Planned |
| 3D-printed enclosure | 🟡 Planned |

> The status will be updated as development continues.

---

# 🏗️ Project Development

The project is being developed in stages rather than trying to build everything at once.

### Stage 1 — Prototype

Get the ESP32-S3 communicating with the sensors and Wi-Fi.

### Stage 2 — Internet Integration

Connect the station to weather, air-quality, and time APIs.

### Stage 3 — Voice

Add microphone input, speech recognition, AI processing, and spoken responses.

### Stage 4 — Cloud

Connect the station to Arduino Cloud and begin collecting historical data.

### Stage 5 — Hardware

Design the custom PCB and improve power management.

### Stage 6 — Mechanical Design

Create the final hexagonal enclosure and 3D-print the housing.

### Stage 7 — Testing

Test the complete system for reliability, accuracy, power consumption, and long-term operation.

### Stage 8 — Open Source

Publish the firmware, PCB, CAD files, documentation, BOM, and build instructions.

---

# 🌍 Open Source

One of the goals of this project is to make the work useful to other people.

Once the hardware and software are stable, I plan to publish:

- Firmware
- Schematics
- PCB files
- BOM
- CAD files
- STL files
- Wiring diagrams
- Assembly instructions
- Documentation
- Testing results

The goal is that someone else should be able to look at this repository and understand **how the system works and how to build their own version**.

---

# 🚀 Future Ideas

There are several features I would like to explore in the future:

- 🌅 Sunrise and sunset announcements
- 🌧️ Automatic weather alerts
- 🔋 Low-battery voice warnings
- 🏠 Home Assistant integration
- 🗣️ Wake-word detection
- 🔌 Offline fallback mode
- 🌍 Multiple-city weather queries
- ☀️ Automatic display brightness
- 📊 Advanced environmental analytics
- 🌡️ Indoor/outdoor comparison
- 📡 Multiple wireless displays
- 🤖 More advanced AI interaction

---

# 🎥 Documentation

The development process will also be documented.

This will include:

- Build videos
- Hardware photographs
- Circuit diagrams
- PCB design
- CAD models
- Testing
- Firmware development
- Engineering decisions

I also plan to make a video explaining not only **how I built the project**, but also **why I made the engineering decisions behind it**.

---

# 🌐 Project Website

A dedicated website has been created to provide a visual overview of the project.

The website presents the concept, architecture, hardware, APIs, and future direction of the weather station.

👉 **[Visit the Project Website](https://projectmasterpiecev1.framer.website/)**

---

# 📜 License

This project is intended to be released as an open-source project under the **MIT License**.

See [`LICENSE`](LICENSE) for details.

---

# 👨‍💻 About the Project

### AI Voice Weather Station

This project is an exploration of:

**Embedded Systems × IoT × AI × Electronics × PCB Design × CAD × 3D Printing × Cloud Computing**

The goal isn't just to build a weather station.

The goal is to learn how all of these technologies can be combined into one complete engineering system.

---

⭐ If you find the project interesting, consider giving the repository a star!
