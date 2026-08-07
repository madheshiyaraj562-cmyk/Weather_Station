//Test code to test if wifi is working correctly

#include <WiFi.h>

const char* ssid = "YOUR_WIFI_SSID";     
const char* password = "YOUR_WIFI_PASSWORD";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal strength (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\nFailed to connect to WiFi.");
  }
}

void loop() {
  // Periodically check connection status
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi is working ✅  :)");
  } else {
    Serial.println("WiFi is NOT connected ❌  :(");
  }
  delay(5000);
}
