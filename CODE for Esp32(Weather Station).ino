#define TOUCH_WAKE_PIN 10   // wake / status check (and multi-touch modifier)
#define TOUCH_TIME_PIN 11   // speak the current time only
#define TOUCH_SONG_PIN 12   // start/stop music



#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_NeoPixel.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include "webpage.h"

// =====================================================
// WIFI & LOCATION
// =====================================================
const char* WIFI_SSID = "   ";
const char* WIFI_PASSWORD = "   ";
const float LATITUDE  = 26.7606;
const float LONGITUDE = 83.3732;


// =====================================================
// HARDWARE PINS & SENSORS
// =====================================================
#define DHT_PIN   7
#define DHT_TYPE  DHT22
#define SDA_PIN   8
#define SCL_PIN   9
#define MIC_PIN   4
#define RGB_LED_PIN 21
#define RGB_LED_COUNT 1

// DFPlayer Pins
#define DFPLAYER_RX_PIN 6
#define DFPLAYER_TX_PIN 5
#define BUSY_PIN 2
#define MAX_TRACK_MS 300000
#define START_WAIT_MS 1500
#define GAP_AFTER_CLIP_MS 150

// Voice event chimes
#define LISTENING_CHIME 85
#define SLEEP_CHIME     86

// Touch sensor pins (TTP223 modules, active HIGH on touch)
#define TOUCH_WAKE_PIN 10   
#define TOUCH_TIME_PIN 11   
#define TOUCH_SONG_PIN 12   

// Music tracks
const int MUSIC_TRACKS[] = {88, 99, 100, 101};
const int MUSIC_TRACK_COUNT = 4;
int currentMusicIndex = 0;

// Pin 12 music control:
// short press = play/stop
// hold for 3 seconds = next song
#define MUSIC_HOLD_MS 3000UL

// Minimum time between two accepted touch presses on the same sensor
#define TOUCH_DEBOUNCE_MS 350UL

// Minimum time between two accepted claps
#define CLAP_DEBOUNCE_MS 2100UL

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP085 bmp;
Adafruit_NeoPixel rgbLED(RGB_LED_COUNT, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);
HardwareSerial dfSerial(1);
DFRobotDFPlayerMini dfPlayer;

// =====================================================
// GLOBAL VARIABLES
// =====================================================
float localTemp = 0.0, localHum = 0.0, localPressure = 0.0, localAlt = 0.0;
float espTemp = 0.0; 
float prevPressure = 0.0;
String pressureTrend = "Steady";

float currentTempC = 0.0, feelsLikeC = 0.0, currentWindKmh = 0.0, currentUV = 0.0;
int currentHumidity = 0, weatherCode = 0, usAQI = 0;
float pm25 = 0.0;
String sunriseStr = "--:--", sunsetStr = "--:--", lastWeatherUpdate = "--:--:--";

// Voice report mapped integers
int insideTempWhole = 0, insideTempDec = 0, insideHumidity = 0;
int pressureValue = 0, altitudeValue = 0;
int outsideTempWhole = 0, outsideTempDec = 0;
int feelsLikeWhole = 0, feelsLikeDec = 0;
bool feelsLikeHigh = false;
int outsideHumidity = 0, windSpeed = 0, uvIndex = 0;
int aqiValue = 0;
bool rainExpected = false;

// TIME VARIABLES
int currentHour = 7;
int currentMinute = 0;
int currentSecond = 0;
int rawHour24 = 0;
bool isPM = true;

// Proactive Alert Flags
bool aqiAlertTriggered = false;
bool rainAlertTriggered = false;

// Clap Detection
const int CLAP_THRESHOLD = 1450;
int micBaseline = 2048;
long baselineSum = 0;
int baselineCount = 0;
unsigned long lastClapTime = 0;
String clapTimeStr = "--:--:--";
unsigned long clapEventId = 0;

// Music playback state
bool isMusicPlaying = false;

// Touch button debounce tracking
struct TouchButton {
  uint8_t pin;
  bool lastState;
  unsigned long lastTrigger;
};
TouchButton touchWake = { TOUCH_WAKE_PIN, LOW, 0 };
TouchButton touchTime = { TOUCH_TIME_PIN, LOW, 0 };
TouchButton touchSong = { TOUCH_SONG_PIN, LOW, 0 };

// Pin 12 long-press tracking
bool songButtonWasDown = false;
bool songHoldHandled = false;
unsigned long songPressStart = 0;

// WiFi / NTP state tracking
unsigned long lastWifiCheck = 0;
#define WIFI_CHECK_INTERVAL_MS 30000UL
bool ntpConfigured = false;
bool wasConnected = false;

// Timers & Server
WebServer server(80);
unsigned long lastSensorRead = 0, lastWeatherFetch = 0;

// =====================================================
// LED CONTROLS
// =====================================================
void setLED(uint8_t r, uint8_t g, uint8_t b) {
  rgbLED.setPixelColor(0, rgbLED.Color(r, g, b));
  rgbLED.show();
}
void ledIdle()    { setLED(80, 220, 150); }   // Soft Emerald
void ledClap()    { setLED(255, 200, 100); }  // Warm Gold
void ledAlert()   { setLED(235, 70, 85); }    // Deep Ruby
void ledMusic()   { setLED(90, 160, 255); }   // Soft Sapphire

// =====================================================
// AUDIO / DFPLAYER LOGIC
// =====================================================
bool playTrack(int trackNum) {
  dfPlayer.playMp3Folder(trackNum);
  unsigned long waitStart = millis();

  while (digitalRead(BUSY_PIN) == HIGH) {
    if (millis() - waitStart > START_WAIT_MS) return false;
    server.handleClient();
    delay(5);
  }

  unsigned long playStart = millis();
  while (digitalRead(BUSY_PIN) == LOW) {
    if (millis() - playStart > MAX_TRACK_MS) return false;
    server.handleClient();
    delay(5);
  }

  unsigned long gapStart = millis();
  while (millis() - gapStart < GAP_AFTER_CLIP_MS) {
    server.handleClient();
    delay(5);
  }

  return true;
}

// 0085 starts every voice/data event.
// 0086 ends every voice/data event.
void startVoiceEvent() {
  playTrack(LISTENING_CHIME);
}

void endVoiceEvent() {
  playTrack(SLEEP_CHIME);
}

void playNumber(int n) {
  if (n < 0) { playTrack(76); n = -n; } 
  int hundreds = n / 100;
  int remainder = n % 100;
  if (hundreds > 0) { playTrack(hundreds + 1); playTrack(29); } 
  if (remainder == 0 && hundreds > 0) return;
  if (remainder < 20) { playTrack(remainder + 1); }
  else {
    int tens = remainder / 10; int ones = remainder % 10;
    playTrack(tens + 19); if (ones > 0) playTrack(ones + 1);
  }
}

void playDecimal(int whole, int decimalDigit) {
  playNumber(whole); playTrack(30); playTrack(decimalDigit + 1); 
}

void playTimeHHMM(int hour12, int minute, bool pm) {
  playNumber(hour12);
  if (minute == 0) {
    playTrack(38); 
  } else {
    playNumber(minute); 
  }
  playTrack(pm ? 42 : 41); 
}

void speakTimeOnly() {
  startVoiceEvent();

  struct tm tinfo;
  if (!getLocalTime(&tinfo, 500)) {
    playTrack(95);
    endVoiceEvent();
    return;
  }

  int hr24 = tinfo.tm_hour;
  int mn   = tinfo.tm_min;
  int hr12 = (hr24 % 12 == 0) ? 12 : (hr24 % 12);
  bool pm  = (hr24 >= 12);

  playTrack(37);
  playTimeHHMM(hr12, mn, pm);

  endVoiceEvent();
}

void speakReport() {
  startVoiceEvent();

  playTrack(37);
  playTimeHHMM(currentHour, currentMinute, isPM);

  playTrack(43); playDecimal(insideTempWhole, insideTempDec); playTrack(31);
  playTrack(47); playDecimal(outsideTempWhole, outsideTempDec); playTrack(31);
  playTrack(48); playDecimal(feelsLikeWhole, feelsLikeDec); playTrack(31);
  if (feelsLikeHigh) playTrack(82);

  playTrack(44); playNumber(insideHumidity); playTrack(32);
  playTrack(49); playNumber(outsideHumidity); playTrack(32);

  if (pressureValue < 1000) { playTrack(45); playNumber(pressureValue); playTrack(34); }
  playTrack(46); playNumber(altitudeValue); playTrack(35);

  playTrack(50); playNumber(windSpeed); playTrack(33);
  playTrack(51); playNumber(uvIndex);

  playTrack(54); playNumber(aqiValue);
  playTrack(rainExpected ? 83 : 84);

  endVoiceEvent();
}

// =====================================================
// MUSIC PLAYBACK
// =====================================================
void startMusic(int track) {
  dfPlayer.stop();
  delay(50);
  dfPlayer.playMp3Folder(track);
  isMusicPlaying = true;
  ledMusic();
}

void playSelectedMusic() {
  startMusic(MUSIC_TRACKS[currentMusicIndex]);
}

void nextMusic() {
  currentMusicIndex++;
  if (currentMusicIndex >= MUSIC_TRACK_COUNT) {
    currentMusicIndex = 0;
  }

  startMusic(MUSIC_TRACKS[currentMusicIndex]);

  Serial.print("Selected song: ");
  Serial.println(MUSIC_TRACKS[currentMusicIndex]);
}

void stopMusic() {
  dfPlayer.stop();
  delay(50); 
  isMusicPlaying = false;
  ledIdle();
}

// =====================================================
// DATA FETCHING & MAPPING
// =====================================================
void splitDecimal(float value, int &whole, int &dec) {
  long scaled = lround(value * 10.0f);
  whole = (int)(scaled / 10);
  dec = (int)abs(scaled % 10);
}

void readLocalSensors() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (!isnan(t) && !isnan(h)) { localTemp = t; localHum = h; }

  #if CONFIG_IDF_TARGET_ESP32 || defined(ARDUINO_ARCH_ESP32)
    espTemp = temperatureRead();
    if (isnan(espTemp)) espTemp = 25.0;
  #else
    espTemp = 0.0;
  #endif

  float p = bmp.readPressure() / 100.0F;
  if (localPressure > 0) {
    if (p > localPressure + 0.5) pressureTrend = "Rising";
    else if (p < localPressure - 0.5) pressureTrend = "Falling";
    else pressureTrend = "Steady";
  }
  localPressure = p;
  localAlt = bmp.readAltitude();
}

void checkProactiveAlerts() {
  if (usAQI >= 150 && !aqiAlertTriggered) {
    ledAlert(); startVoiceEvent(); playTrack(54); playNumber(usAQI); endVoiceEvent();
    aqiAlertTriggered = true; ledIdle();
  } else if (usAQI < 100) { aqiAlertTriggered = false; }

  if ((weatherCode >= 80 && weatherCode <= 82) || weatherCode >= 95) {
    if (!rainAlertTriggered) {
      ledAlert();
      playTrack(83); // Rain alert
      rainAlertTriggered = true; ledIdle();
    }
  } else { rainAlertTriggered = false; }
}

void getWeather(bool announce = false) {
  if (WiFi.status() != WL_CONNECTED) return;
  HTTPClient http; WiFiClientSecure client; client.setInsecure();
  bool weatherOk = false;

  String wUrl = "https://api.open-meteo.com/v1/forecast?latitude=" + String(LATITUDE, 4) +
                "&longitude=" + String(LONGITUDE, 4) +
                "&current=temperature_2m,relative_humidity_2m,apparent_temperature,weather_code,wind_speed_10m,uv_index" +
                "&daily=sunrise,sunset&timezone=Asia%2FKolkata";

  if (http.begin(client, wUrl) && http.GET() == HTTP_CODE_OK) {
    JsonDocument doc; deserializeJson(doc, http.getString());
    currentTempC = doc["current"]["temperature_2m"] | 0.0;
    feelsLikeC = doc["current"]["apparent_temperature"] | 0.0;
    currentHumidity = doc["current"]["relative_humidity_2m"] | 0;
    currentWindKmh = doc["current"]["wind_speed_10m"] | 0.0;
    currentUV = doc["current"]["uv_index"] | 0.0;
    weatherCode = doc["current"]["weather_code"] | 0;

    String sr = doc["daily"]["sunrise"][0] | "";
    String ss = doc["daily"]["sunset"][0] | "";
    if (sr.length() >= 16) sunriseStr = sr.substring(11, 16);
    if (ss.length() >= 16) sunsetStr = ss.substring(11, 16);
    weatherOk = true;
  }
  http.end();

  String aUrl = "https://air-quality-api.open-meteo.com/v1/air-quality?latitude=" + String(LATITUDE, 4) +
                "&longitude=" + String(LONGITUDE, 4) + "&current=us_aqi,pm2_5";
  if (http.begin(client, aUrl) && http.GET() == HTTP_CODE_OK) {
    JsonDocument doc; deserializeJson(doc, http.getString());
    usAQI = doc["current"]["us_aqi"] | 0;
    pm25 = doc["current"]["pm2_5"] | 0.0;
  }
  http.end();

  struct tm tinfo;
  if (getLocalTime(&tinfo, 500)) {
    char buf[20]; strftime(buf, sizeof(buf), "%H:%M:%S", &tinfo);
    lastWeatherUpdate = String(buf);
  }
  checkProactiveAlerts();

  if (announce) {
    playTrack(weatherOk ? 92 : 93); // Weather updated 
  }
}

void updateVocalVariables() {
  splitDecimal(localTemp, insideTempWhole, insideTempDec);
  insideHumidity = (int)localHum;
  pressureValue = (int)localPressure; altitudeValue = (int)localAlt;

  splitDecimal(currentTempC, outsideTempWhole, outsideTempDec);
  splitDecimal(feelsLikeC, feelsLikeWhole, feelsLikeDec);
  feelsLikeHigh = (feelsLikeC >= 30.0);

  outsideHumidity = currentHumidity; windSpeed = (int)currentWindKmh; uvIndex = (int)currentUV;
  aqiValue = usAQI; rainExpected = (weatherCode >= 51 && weatherCode <= 99);

  struct tm tinfo;
  if (getLocalTime(&tinfo, 500)) {
    rawHour24 = tinfo.tm_hour;
    currentMinute = tinfo.tm_min;
    currentSecond = tinfo.tm_sec;
    isPM = (rawHour24 >= 12);
    currentHour = (rawHour24 % 12 == 0) ? 12 : (rawHour24 % 12);
  }
}

// =====================================================
// TOUCH SENSORS
// =====================================================
bool touchPressed(TouchButton &t, unsigned long now) {
  bool state = digitalRead(t.pin);
  bool pressed = false;
  if (state == HIGH && t.lastState == LOW && (now - t.lastTrigger > TOUCH_DEBOUNCE_MS)) {
    pressed = true;
    t.lastTrigger = now;
  }
  t.lastState = state;
  return pressed;
}

void handleTouchSensors(unsigned long now) {
  bool pin10Pressed = touchPressed(touchWake, now);

  // =====================================================
  // PIN 12 MUSIC CONTROL
  // Short press = play / stop
  // Hold 3 seconds = next song and play it
  // =====================================================
  bool songButtonDown = (digitalRead(TOUCH_SONG_PIN) == HIGH);

  if (songButtonDown && !songButtonWasDown) {
    songPressStart = now;
    songHoldHandled = false;
  }

  if (songButtonDown && !songHoldHandled &&
      (now - songPressStart >= MUSIC_HOLD_MS)) {
    songHoldHandled = true;
    nextMusic();
  }

  if (!songButtonDown && songButtonWasDown) {
    unsigned long pressDuration = now - songPressStart;

    if (!songHoldHandled && pressDuration >= TOUCH_DEBOUNCE_MS) {
      if (isMusicPlaying) {
        stopMusic();
      } else {
        playSelectedMusic();
      }
    }
  }

  songButtonWasDown = songButtonDown;

  // Pin 10: Ready & Wi-Fi status
  if (pin10Pressed) {
    startVoiceEvent();
    playTrack(96);

    if (WiFi.status() == WL_CONNECTED) {
      playTrack(90);
    } else {
      playTrack(91);
    }

    endVoiceEvent();
    ledIdle();
  }

  // Pin 11: Speak time
  if (touchPressed(touchTime, now)) {
    speakTimeOnly();
    ledIdle();
  }
}

// =====================================================
// WEB SERVER ROUTES
// =====================================================
void handleRoot() { server.send_P(200, "text/html; charset=UTF-8", webpage); }

void handlePlayMusic() {
  playSelectedMusic();
  server.send(200, "text/plain", isMusicPlaying ? "Playing music" : "Failed to start music");
}

void handleStopMusic() {
  stopMusic();
  server.send(200, "text/plain", "Music stopped");
}

void handleAPI() {
  struct tm tinfo; char tStr[20]="--:--:--", dStr[30]="---";
  if (getLocalTime(&tinfo, 500)) {
    strftime(tStr, sizeof(tStr), "%H:%M:%S", &tinfo);
    strftime(dStr, sizeof(dStr), "%A, %B %d, %Y", &tinfo);
  }

  JsonDocument doc;
  doc["time"] = tStr; doc["date"] = dStr;
  doc["temp"] = currentTempC; doc["feels_like"] = feelsLikeC; doc["humidity"] = currentHumidity;
  doc["wind"] = currentWindKmh; doc["uv"] = currentUV; doc["weather_code"] = weatherCode;
  doc["aqi"] = usAQI; doc["pm25"] = pm25;
  doc["loc_temp"] = localTemp; doc["loc_hum"] = localHum;
  doc["esp_temp"] = espTemp;
  doc["pressure"] = localPressure;
  doc["pressure_trend"] = pressureTrend;
  doc["sunrise"] = sunriseStr; doc["sunset"] = sunsetStr;
  doc["last_update"] = lastWeatherUpdate;
  doc["clap_id"] = clapEventId; doc["clap_time"] = clapTimeStr;
  doc["is_music_playing"] = isMusicPlaying;
  doc["wifi_connected"] = (WiFi.status() == WL_CONNECTED);

  String out; serializeJson(doc, out);
  server.send(200, "application/json", out);
}

// =====================================================
// MAIN SETUP & LOOP
// =====================================================
void setup() {
  Serial.begin(115200);
  pinMode(BUSY_PIN, INPUT_PULLUP);

  pinMode(TOUCH_WAKE_PIN, INPUT);
  pinMode(TOUCH_TIME_PIN, INPUT);
  pinMode(TOUCH_SONG_PIN, INPUT);

  rgbLED.begin(); rgbLED.setBrightness(10); setLED(255, 255, 255);

  dht.begin(); Wire.begin(SDA_PIN, SCL_PIN); bmp.begin();

  dfSerial.begin(9600, SERIAL_8N1, DFPLAYER_RX_PIN, DFPLAYER_TX_PIN);
  dfPlayer.begin(dfSerial, true, true);
  dfPlayer.volume(40);

  playTrack(89); // "connecting to WiFi"
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long wifiStart = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - wifiStart < 20000) {
    delay(300); Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    configTime(19800, 0, "pool.ntp.org", "time.google.com");
    ntpConfigured = true;
    wasConnected = true;
    playTrack(87); // Custom announcement chime
    playTrack(90); // "WiFi connected"
    Serial.println("\nWiFi connected at boot.");
  } else {
    Serial.println("\nWiFi not connected at boot - will finish connecting in background.");
  }

  for (int i = 0; i < 50; i++) { baselineSum += analogRead(MIC_PIN); delay(5); }
  micBaseline = baselineSum / 50;
  baselineSum = 0;

  getWeather(wasConnected); 
  readLocalSensors();

  server.on("/", handleRoot);
  server.on("/api", handleAPI);
  server.on("/play", handlePlayMusic);
  server.on("/stop", handleStopMusic);

  server.begin();
  ledIdle();
}

void loop() {
  unsigned long now = millis();
  server.handleClient();

  bool connectedNow = (WiFi.status() == WL_CONNECTED);

  if (connectedNow && !wasConnected) {
    if (!ntpConfigured) {
      configTime(19800, 0, "pool.ntp.org", "time.google.com");
      ntpConfigured = true;
    }
    playTrack(87); // Custom announcement chime
    playTrack(90); // "WiFi connected"
    getWeather(true);           
    lastWeatherFetch = now;
  }

  if (!connectedNow && wasConnected) {
    playTrack(87); // Custom announcement chime
    playTrack(91); // "WiFi disconnected"
  }

  if (!connectedNow && now - lastWifiCheck > WIFI_CHECK_INTERVAL_MS) {
    lastWifiCheck = now;
    WiFi.reconnect();
  }

  wasConnected = connectedNow;

  handleTouchSensors(now);

  // Clap detection runs continuously unless Pin 10 is actively being touched
  if (digitalRead(TOUCH_WAKE_PIN) == LOW) {
    int micVal = analogRead(MIC_PIN);
    if (abs(micVal - micBaseline) > CLAP_THRESHOLD && (now - lastClapTime > CLAP_DEBOUNCE_MS)) {
      delay(15);
      if (abs(analogRead(MIC_PIN) - micBaseline) < CLAP_THRESHOLD) {
        lastClapTime = now;
        ledClap();

        struct tm tinfo;
        if (getLocalTime(&tinfo, 500)) {
          char buf[16]; strftime(buf, sizeof(buf), "%H:%M:%S", &tinfo);
          clapTimeStr = String(buf);
        }
        clapEventId++;

        if (isMusicPlaying) {
          stopMusic();
        }

        readLocalSensors();
        updateVocalVariables();
        speakReport();

        ledIdle();
      }
    }

    if (!isMusicPlaying) {
      baselineSum += micVal;
      baselineCount++;
      if (baselineCount >= 400) {
        micBaseline = baselineSum / baselineCount;
        baselineSum = 0;
        baselineCount = 0;
      }
    }
  }

  if (now - lastSensorRead >= 2000) { readLocalSensors(); lastSensorRead = now; }
  if (now - lastWeatherFetch >= 600000 && WiFi.status() == WL_CONNECTED) { getWeather(); lastWeatherFetch = now; }
}
