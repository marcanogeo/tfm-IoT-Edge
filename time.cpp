#include <WiFi.h>
#include "time.h"

// Configuración WiFi
const char* ssid     = "TU_SSID";       // Cambia por tu red WiFi
const char* password = "TU_PASSWORD";   // Cambia por tu contraseña

// Configuración NTP
const char* ntpServer = "pool.ntp.org"; // Servidor NTP
const long  gmtOffset_sec = 3600;       // Offset horario en segundos (España peninsular: UTC+1)
const int   daylightOffset_sec = 3600;  // Horario de verano: +1 hora

void setup() {
  Serial.begin(115200);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi");

  // Inicializar y sincronizar hora con NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Mostrar hora inicial
  printLocalTime();
}

void loop() {
  delay(1000); // Esperar 1 segundo
  printLocalTime();
}

// Función para imprimir la hora local
void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Error al obtener la hora");
    return;
  }
  Serial.printf("Fecha y hora: %02d/%02d/%04d %02d:%02d:%02d\n",
                timeinfo.tm_mday,
                timeinfo.tm_mon + 1,
                timeinfo.tm_year + 1900,
                timeinfo.tm_hour,
                timeinfo.tm_min,
                timeinfo.tm_sec);
}
