#ifndef TELEMETRY_BUILDER_H
#define TELEMETRY_BUILDER_H

#include <Arduino.h>
#include "mq2_sensor.h"

String buildTelemetryPayload(
  const char* deviceId,
  const char* zoneId,
  unsigned long timestamp,
  float temperature,
  float humidity,
  int lightRaw,
  const MQ2Reading& gas
);

#endif