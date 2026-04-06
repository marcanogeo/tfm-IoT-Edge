#include "telemetry_builder.h"

String buildTelemetryPayload(
  const char* deviceId,
  const char* zoneId,
  unsigned long timestamp,
  float temperature,
  float humidity,
  int lightRaw,
  const MQ2Reading& gas
) {
  String payload = "{";
  payload += "\"device_id\":\"" + String(deviceId) + "\",";
  payload += "\"zone\":\"" + String(zoneId) + "\",";
  payload += "\"timestamp\":" + String(timestamp) + ",";
  payload += "\"sensors\":{";
  payload += "\"temperature_c\":" + String(temperature, 2) + ",";
  payload += "\"humidity_pct\":" + String(humidity, 2) + ",";
  payload += "\"light_raw\":" + String(lightRaw) + ",";
  payload += "\"gas_detected\":" + String(gas.digital == 0 ? 1 : 0) + ",";
  payload += "\"gas_mq2_raw\":" + String(gas.raw) + ",";
  payload += "\"gas_mq2_voltage\":" + String(gas.voltage, 3) + ",";
  payload += "\"gas_mq2_percent\":" + String(gas.percent, 2) + ",";
  payload += "\"gas_mq2_ratio\":" + String(gas.ratio, 2) + ",";
  payload += "\"gas_mq2_ppm_est\":" + String(gas.ppm_est, 2);
  payload += "}}";
  return payload;
}