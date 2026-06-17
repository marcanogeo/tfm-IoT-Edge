#include "app_publish.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include "time.h"
#include "config.h"
#include "node_metadata.h"
#include "telemetry_builder.h"
#include "topics.h"
#include "ens160_bme280_sensor.h"
#include "bh1750_sensor.h"

extern PubSubClient  client;
extern AtmoSensor    atmoSensor;
extern BH1750Sensor  lightSensor;

void publishTelemetry() {
  if (!client.connected()) {
    Serial.println("MQTT no conectado, publicación de telemetría cancelada");
    return;
  }

 // ── Leer sensores ─────────────────────────
  AtmoReading    atmo  = atmoSensor.read();
  BH1750Reading  light = lightSensor.read();

  if (!atmo.valid) {
    Serial.println("[PUBLISH] Lectura atmosférica inválida, omitiendo publicación");
    return;
  }

  // ── Construir y publicar payload ──────────
  unsigned long ts      = time(nullptr);
  String        payload = buildTelemetryPayload(DEVICE_ID, ZONE_ID, ts, atmo, light);

  bool ok = client.publish(topicTelemetry().c_str(), payload.c_str());

  
  Serial.println("──── TELEMETRÍA ────────────────────────");
  Serial.println(payload);
  Serial.printf("[MQTT] Publicación: %s\n", ok ? "OK" : "ERROR");

  // ── Log de estado del ENS160 ─────────────
  // status 0 = Normal | 1 = Calentamiento | 2 = Calibración inicial
  if (atmo.ens160_status != 0) {
    Serial.printf("[ENS160] Estado del sensor: %d (puede afectar precisión)\n",
                  atmo.ens160_status);
  }
}
