#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>

// =====================================================
//  WIFI
// =====================================================

const char* WIFI_SSID     = "Name1234"; // Wifi Name
const char* WIFI_PASSWORD = "Password9876"; // Wifi Password
  
// =====================================================
// TIME & LOCATION CONFIGURATION
// =====================================================

// {India Standard Time = UTC +5:30, no daylight saving} 
// Enter as your Time Zone 
const long  GMT_OFFSET_SEC      = 5 * 3600 + 1800; // +5:30
const int   DAYLIGHT_OFFSET_SEC = 0;

// Multiple NTP servers = redundancy if one is slow/unreachable
// Multiple NTP servers are used for redundancy.
// If one server is unavailable, another can be used.
const char* NTP_SERVER_1 = "pool.ntp.org";
const char* NTP_SERVER_2 = "time.google.com";
const char* NTP_SERVER_3 = "time.nist.gov";

// =====================================================
// LOCATION
// =====================================================
// Enter as per your location
const float LATITUDE  =  0;
const float LONGITUDE =  0;


// =====================================================
// WEATHER API
// =====================================================

// Open-Meteo API endpoint.
// The API provides:
// - Current temperature
// - Relative humidity
// - Wind speed
// - Sunrise
// - Sunset
//
// timezone=Asia/Kolkata makes the sunrise/sunset
// times appear in Indian Standard Time.
const char* WEATHER_API_URL =
  "https://api.open-meteo.com/v1/forecast"
  "?latitude=26.7606&longitude=83.3732"
  "&current=temperature_2m,relative_humidity_2m,wind_speed_10m"
  "&daily=sunrise,sunset"
  "&timezone=Asia%2FKolkata";

// =====================================================
// WEATHER VARIABLES
// =====================================================

// These variables store the latest successful
// weather information received from the API.
float currentTempC   = 0;
int   currentHumidity = 0;
float currentWindKmh  = 0;

String sunriseStr = "--:--";              // Sunrise and sunset are stored as text
String sunsetStr  = "--:--";              // because the API returns them as strings.
bool   weatherValid = false;              // Becomes true after a valid weather response
                                          // has been successfully received.

// =====================================================
// TIMING VARIABLES
// =====================================================

// millis() is used instead of delay() for periodic tasks.
// This allows the ESP32 to continue running other tasks
// while waiting for the next update.
unsigned long lastPrint        = 0;
unsigned long lastTimeResync   = 0;
unsigned long lastWeatherFetch = 0;


// UPDATE INTERVALS
const unsigned long TIME_RESYNC_INTERVAL    = 30UL * 60UL * 1000UL; // 30 min
const unsigned long WEATHER_FETCH_INTERVAL  = 10UL * 60UL * 1000UL; // 10 min


// =====================================================
// CONNECT WIFI
// =====================================================
void connectWiFi() {
  Serial.println();
  Serial.println("Connecting to WiFi...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi CONNECTED! IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi FAILED to connect.");
  }
}


// =====================================================
// SYNC TIME VIA NTP
// =====================================================
bool syncTime() {
  Serial.println("Syncing time via NTP...");
  // Configure the ESP32's system clock.
  //
  // GMT_OFFSET_SEC converts UTC to IST (+5:30).
  // Multiple NTP servers provide redundancy.
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC,
             NTP_SERVER_1, NTP_SERVER_2, NTP_SERVER_3);

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 10000)) {
    Serial.println("NTP sync FAILED.");
    return false;
  }

  Serial.println("NTP sync SUCCESS.");
  return true;
}


// =====================================================
// GET WEATHER (temp, humidity, wind, sunrise, sunset)
// =====================================================
bool getWeather() {
  // There is no point requesting the API
  // if Wi-Fi is not connected.
  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  http.begin(WEATHER_API_URL);           // Open a connection to the Open-Meteo API.
  http.setTimeout(15000);                // Maximum time allowed for the HTTP request.

  Serial.println("Fetching weather...");

  int httpCode = http.GET();              // Send HTTP GET request.

  if (httpCode != HTTP_CODE_OK) {
    Serial.print("Weather HTTP ERROR: ");
    Serial.println(httpCode);
    http.end();
    return false;
  }

  String payload = http.getString();        // Read the complete API response.
  http.end();                               // Close the HTTP connection.

  JsonDocument doc;                         // Create a JSON document for ArduinoJson.
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {                                // Check whether JSON parsing succeeded.
    Serial.print("Weather JSON ERROR: ");
    Serial.println(error.c_str());
    return false;
  }
  // EXTRACT CURRENT WEATHER
  currentTempC    = doc["current"]["temperature_2m"]      | 0.0;          // Temperature in degrees Celsius.
  currentHumidity = doc["current"]["relative_humidity_2m"] | 0;           // Relative humidity in percent.
  currentWindKmh  = doc["current"]["wind_speed_10m"]       | 0.0;         // Wind speed in km/h.

  // daily.sunrise[0] / daily.sunset[0] look like "2026-08-07T05:42"
  String sunriseRaw = doc["daily"]["sunrise"][0] | "";
  String sunsetRaw   = doc["daily"]["sunset"][0]  | "";

  int tIndex;
  tIndex = sunriseRaw.indexOf('T');
  sunriseStr = (tIndex != -1) ? sunriseRaw.substring(tIndex + 1) : "--:--";

  tIndex = sunsetRaw.indexOf('T');
  sunsetStr = (tIndex != -1) ? sunsetRaw.substring(tIndex + 1) : "--:--";

  weatherValid = true;                              // Weather data is now considered valid.

  // ===================================================
  // PRINT WEATHER INFORMATION
  // ===================================================
  Serial.println("Weather updated:");
  Serial.print("  Temp     : "); Serial.print(currentTempC); Serial.println(" C");
  Serial.print("  Humidity : "); Serial.print(currentHumidity); Serial.println(" %");
  Serial.print("  Wind     : "); Serial.print(currentWindKmh); Serial.println(" km/h");
  Serial.print("  Sunrise  : "); Serial.println(sunriseStr);
  Serial.print("  Sunset   : "); Serial.println(sunsetStr);

  return true;
}


// =====================================================
// PRINT TIME + WEATHER LINE
// =====================================================
void printStatus(struct tm &timeinfo) {
  char buf[64];
  strftime(buf, sizeof(buf), "%H:%M:%S  %d/%m/%Y  %A", &timeinfo);            // HH:MM:SS DD/MM/YYYY Day

  Serial.print("\r");
  Serial.print(buf);

  if (weatherValid) {                        // Only display weather if valid data
    Serial.print("  |  ");                   // has already been received.
    Serial.print(currentTempC, 1);
    Serial.print("C  ");
    Serial.print(currentHumidity);
    Serial.print("%  Wind ");
    Serial.print(currentWindKmh, 1);
    Serial.print("km/h  Sunrise ");
    Serial.print(sunriseStr);
    Serial.print("  Sunset ");
    Serial.println(sunsetStr);
  }
    
  Serial.print("        ");
}


// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println();
  Serial.println("======================================");
  Serial.println("  ESP32-S3 IST CLOCK + WEATHER");
  Serial.println("  Gorakhpur, Uttar Pradesh, India");
  Serial.println("======================================");

  connectWiFi();
  // Continue with network-dependent tasks
  // only if Wi-Fi connected successfully.
  if (WiFi.status() == WL_CONNECTED) {
    syncTime();                                // Synchronize the clock using NTP.
    lastTimeResync = millis();                 // Remember when the last synchronization occurred.

    getWeather();                              // Get the first weather reading immediately.
    lastWeatherFetch = millis();
  }
}


// =====================================================
// LOOP
// =====================================================
void loop() {
  unsigned long now = millis();

  // Print once a second
  if (now - lastPrint >= 1000) {
    lastPrint = now;

    struct tm timeinfo;
    if (getLocalTime(&timeinfo, 0)) {
      printStatus(timeinfo);
    } else {
      Serial.print("\rTime not synced yet...          ");
    }
  }

  // Re-sync clock every 30 min
  if (now - lastTimeResync >= TIME_RESYNC_INTERVAL) {
    if (WiFi.status() != WL_CONNECTED) connectWiFi();
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.println("Re-syncing time with NTP...");
      syncTime();
    }
    lastTimeResync = now;
  }

  // Refresh weather every 10 min
  if (now - lastWeatherFetch >= WEATHER_FETCH_INTERVAL) {
    if (WiFi.status() != WL_CONNECTED) connectWiFi();
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.println("Refreshing weather...");
      getWeather();
    }
    lastWeatherFetch = now;
  }
}

