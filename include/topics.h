#ifndef TOPICS_H
#define TOPICS_H

#include <Arduino.h>
#include "node_metadata.h"

inline String topicTelemetry() {
  return "tfm/ambiental/" + String(ZONE_ID_STR) + "/" + String(DEVICE_ID_STR) + "/telemetria";
}

inline String topicStatus() {
  return "tfm/ambiental/" + String(ZONE_ID_STR) + "/" + String(DEVICE_ID_STR) + "/estado";
}

inline String topicCmd() {
  return "tfm/ambiental/" + String(ZONE_ID_STR) + "/" + String(DEVICE_ID_STR) + "/cmd";
}

inline String topicAlert() {
  return "tfm/ambiental/" + String(ZONE_ID_STR) + "/" + String(DEVICE_ID_STR) + "/alerta";
}

#endif