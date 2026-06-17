#include "telemetry_builder.h"

String buildTelemetryPayload(
  const char*          deviceId,
  const char*          zoneId,
  unsigned long        timestamp,
  const AtmoReading&   atmo,
  const BH1750Reading& light
) {
  String payload = "{";

  // ── Metadatos del nodo ────────────────────
  payload += "\"device_id\":\"" + String(deviceId) + "\",";
  payload += "\"zone\":\""      + String(zoneId)   + "\",";
  payload += "\"timestamp\":"   + String(timestamp) + ",";

  // ── Lecturas de sensores ──────────────────
  payload += "\"sensors\":{";

  // BME280
  payload += "\"temperature_c\":"  + String(atmo.temperature_c, 2) + ",";
  payload += "\"humidity_pct\":"   + String(atmo.humidity_pct,  2) + ",";
  payload += "\"pressure_hpa\":"   + String(atmo.pressure_hpa,  2) + ",";

  // BH1750
  payload += "\"light_lux\":"      + String(light.valid ? light.lux : -1.0f, 2) + ",";

  // ENS160
  payload += "\"aqi\":"            + String(atmo.aqi)          + ",";
  payload += "\"eco2_ppm\":"       + String(atmo.eco2_ppm)     + ",";
  payload += "\"tvoc_ppb\":"       + String(atmo.tvoc_ppb)     + ",";
  payload += "\"ens160_status\":"  + String(atmo.ens160_status);

  payload += "}}";
  return payload;
}