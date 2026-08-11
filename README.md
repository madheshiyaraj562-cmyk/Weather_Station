# 🌦️ AI Voice Weather Station

> A wall-mounted smart weather station that combines environmental sensing, voice interaction, Internet APIs, AI, cloud monitoring, custom electronics, and 3D-printed mechanical design.


<p align="center">
  <a href="https://projectmasterpiecev1.framer.website/">
    🌐 <strong>PROJECT WEBSITE</strong>
  </a>
</p>
---
This project is fully open source: firmware, PCB design, enclosure files, and setup guides are all included so anyone can build their own.

✨ Features
🎙️ Voice assistant — ask for temperature, humidity, pressure, altitude, weather, wind, UV index, air quality, time, date, a full report, music, or sleep
🌍 Live weather & AQI via Open-Meteo (free, no API key required)
🗣️ Speech-to-text via Google Cloud Speech-to-Text (free tier: 60 min/month)
🔊 Spoken responses via pre-recorded MP3 clips played through a DFPlayer Mini (dynamic TTS not possible on-device, so a curated ~98-clip response library covers all commands)
👋 Touch controls — dedicated touch sensors to wake the device, ask the time, and play music
👏 Clap detection — clapping during music playback stops the music and registers as a clap event
💡 RGB status ring (WS2812B) for listening / sleeping / confirmation states
☁️ Arduino Cloud dashboard with matching remote controls
🔋 Battery-powered with USB-C charging (TP4056)
📡 OTA firmware updates
🖥️ Optional ESP-NOW secondary display
🛠️ Hardware
Core components
Component	Purpose
ESP32-S3	Main microcontroller
BME280	Temperature / humidity / pressure
INMP441 (or MAX9814)	Microphone for voice input
MAX98357A + 3W 4Ω speaker	Audio output
DFPlayer Mini	Pre-recorded voice response playback
WS2812B RGB LED	Status indicator
TTP223 touch sensor(s)	Wake / time / music controls
Li-ion/LiPo battery + TP4056	Power & charging
USB-C	Power input
OLED (optional)	Visual display
Prototype pin mapping (ESP32-S3)
Sensor/Module	Pins
DHT22	GPIO7
BMP180	SDA: GPIO8, SCL: GPIO9
MAX9814 Mic	GPIO4
DFPlayer Mini	RX: GPIO6, TX: GPIO5

Battery voltage divider not yet wired in current prototype.

PCB

Custom PCB designed in EasyEDA. Gerbers and schematic files are in /hardware.

Enclosure

Custom hexagonal wall-mounted enclosure (STL files in /enclosure) with:

Speaker grille
Mic opening
RGB diffuser
OLED window
USB-C port cutout
Battery compartment
BME280 airflow venting
PCB mounting posts
🧩 Simpler Prototype (Arduino Uno R4 WiFi)

A lighter-weight companion build with no physical sensors — pulls temperature, humidity, wind, day, date, and time directly from an online weather API, displayed on a small OLED/LCD plus a web dashboard. See /prototypes/uno-r4-wifi.

🗣️ Voice Commands
"What's the temperature?"
"Humidity?"
"Pressure / altitude?"
"What's the weather like?"
"Wind?"
"UV index?"
"Air quality?"
"What time is it?" (reported in HH:MM format)
"What's the date?"
"Give me a report" (full summary)
"Play music"
"Sleep"

Wake behavior: the device does not passively listen for a wake word while asleep — a physical touch/button press is required to wake it. Distinct chimes indicate listening, sleep, and confirmation states.

🌐 APIs & Services
Weather / AQI: Open-Meteo — free, no key needed. Default location: Gorakhpur, Uttar Pradesh (configurable).
Speech-to-Text: Google Cloud Speech-to-Text — free tier, 60 min/month.
Dashboard: Arduino Cloud.
📁 Repository Structure
.
├── firmware/           # ESP32-S3 source code
├── hardware/           # PCB schematic, EasyEDA project, Gerbers, BOM
├── enclosure/          # STL files for 3D-printed hexagonal enclosure
├── audio/              # Pre-recorded MP3 response clips (DFPlayer SD card contents)
├── prototypes/
│   └── uno-r4-wifi/    # Simpler Arduino Uno R4 WiFi prototype
├── docs/                # Wiring diagrams, API setup guide, Arduino Cloud guide
└── README.md
🚀 Getting Started
Hardware: Build or order the PCB from /hardware, and wire components per the pinout above / wiring diagrams.
SD Card (audio): Copy the MP3 response clips from /audio onto a FAT32-formatted SD card, in a MP3 folder on the root, named 0001.mp3–0098.mp3.
Firmware: Flash /firmware to the ESP32-S3 via Arduino IDE / PlatformIO. Update your Wi-Fi credentials, weather location coordinates, and Google Speech-to-Text API key in the config file.
Arduino Cloud: Follow /docs to link your device to the Arduino Cloud dashboard.
Enclosure: 3D print the STL files from /enclosure and assemble.
🗺️ Roadmap
 Indoor/outdoor weather comparison
 Severe weather alerts
 Sunrise/sunset voice announcements
 Low-battery voice alerts
 Home Assistant integration
 Alexa / Google Assistant compatibility
 Multi-city weather queries
 Auto display brightness
 Voice wake word (active listening)
 Offline fallback mode
📜 License

Open source — see LICENSE for details.

🙌 Credits

Built by Raj Madheshiya — designed, wired, and voice-tested end to end, from custom PCB to 98 hand-mapped voice responses.

A build video is planned for YouTube, and printable files will also be published to MakerWorld.
