#ifndef TOPICS_H
#define TOPICS_H

#include <Arduino.h>
#include "node_metadata.h"

inline String topicTelemetry() {
  return "tfm/ambiental/" + String(ZONE_ID) + "/" + String(DEVICE_ID) + "/telemetria";
}

inline String topicStatus() {
  return "tfm/ambiental/" + String(ZONE_ID) + "/" + String(DEVICE_ID) + "/estado";
}

inline String topicCmd() {
  return "tfm/ambiental/" + String(ZONE_ID) + "/" + String(DEVICE_ID) + "/cmd";
}

inline String topicAlert() {
  return "tfm/ambiental/" + String(ZONE_ID) + "/" + String(DEVICE_ID) + "/alerta";
}

#endif