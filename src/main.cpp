#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"
#include "config.h"
#include "mqtt_config.h"
#include "node_metadata.h"
#include "topics.h"
#include "app_config.h"
#include "app_publish.h"
#include "app_loop.h"
#include "ens160_bme280_sensor.h"
#include "bh1750_sensor.h"

// ── Clientes globales ─────────────────────────
WiFiClient   espClient;
PubSubClient client(espClient);

// ── Instancias de sensores físicos ────────────
AtmoSensor   atmoSensor;   // ENS160 + BME280 (I²C)
BH1750Sensor lightSensor;  // BH1750 (I²C)

unsigned long lastPublish = 0;

// ─────────────────────────────────────────────
// Callback MQTT: procesa comandos recibidos
// ─────────────────────────────────────────────
void mqttCallback(char* topic, byte* message, unsigned int length) {
  Serial.print("[MQTT] Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();
}

// ─────────────────────────────────────────────
// Reconexión MQTT con reintentos
// ─────────────────────────────────────────────
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("[MQTT] Conectando...");
    String clientId = String(DEVICE_ID) + "_" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println(" conectado");
      client.subscribe(topicCmd().c_str());
      client.publish(topicStatus().c_str(), "online", true);
    } else {
      Serial.printf(" fallo rc=%d, reintentando en 5s\n", client.state());
      delay(5000);
    }
  }
}

// ─────────────────────────────────────────────
// Setup
// ─────────────────────────────────────────────
void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  delay(500);

  Serial.println("========================================");
  Serial.println(" TFM — Nodo IoT físico");
  Serial.printf ("  Dispositivo : %s\n", DEVICE_ID);
  Serial.printf ("  Zona        : %s\n", ZONE_ID);
  Serial.println("========================================");

  // ── Bus I²C ──────────────────────────────
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Serial.printf("[I2C] Bus iniciado en SDA=%d SCL=%d\n", I2C_SDA_PIN, I2C_SCL_PIN);

  // ── Sensores ─────────────────────────────
  if (!atmoSensor.begin()) {
    Serial.println("[SETUP] Advertencia: ENS160/BME280 no inicializado correctamente");
  }
  if (!lightSensor.begin()) {
    Serial.println("[SETUP] Advertencia: BH1750 no inicializado correctamente");
  }

  // ── Red y tiempo ─────────────────────────
  connectWiFi();
  initTime();
  printLocalTime();

  // ── MQTT ─────────────────────────────────
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqttCallback);
  client.setBufferSize(1024);

  Serial.println("[SETUP] Inicialización completa");
}

// ─────────────────────────────────────────────
// Loop principal
// ─────────────────────────────────────────────
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();
  runMainLoop();
}