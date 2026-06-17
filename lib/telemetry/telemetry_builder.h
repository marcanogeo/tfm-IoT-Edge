#ifndef TELEMETRY_BUILDER_H
#define TELEMETRY_BUILDER_H

#include <Arduino.h>
#include "ens160_bme280_sensor.h"
#include "bh1750_sensor.h"

// ─────────────────────────────────────────────
// Construye el payload JSON de telemetría
// para el prototipo físico (ENS160 + BME280 + BH1750).
//
// Formato del payload resultante:
// {
//   "device_id": "nodo_1",
//   "zone": "zona_norte",
//   "timestamp": 1718000000,
//   "sensors": {
//     "temperature_c": 23.45,
//     "humidity_pct": 55.10,
//     "pressure_hpa": 1013.25,
//     "light_lux": 342.50,
//     "aqi": 2,
//     "eco2_ppm": 645,
//     "tvoc_ppb": 112,
//     "ens160_status": 0
//   }
// }
// ─────────────────────────────────────────────
String buildTelemetryPayload(
  const char*          deviceId,
  const char*          zoneId,
  unsigned long        timestamp,
  const AtmoReading&   atmo,
  const BH1750Reading& light
);

#endif