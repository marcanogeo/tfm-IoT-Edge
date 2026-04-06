#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include "wifi_config.h"

const char* NTP_SERVER = "pool.ntp.org";
const long GMT_OFFSET_SEC = 3600;
const int DAYLIGHT_OFFSET_SEC = 3600;

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Conectando WiFi");
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi conectado");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Error al conectar WiFi");
  }
}

void initTime() {
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Error obteniendo hora NTP");
    return;
  }

  Serial.printf(
    "%02d/%02d/%04d %02d:%02d:%02d\n",
    timeinfo.tm_mday,
    timeinfo.tm_mon + 1,
    timeinfo.tm_year + 1900,
    timeinfo.tm_hour,
    timeinfo.tm_min,
    timeinfo.tm_sec
  );
}