#include <Arduino.h>
#include "config.h"

extern unsigned long lastPublish;
void publishTelemetry();

void runMainLoop() {
  unsigned long now = millis();
  if (now - lastPublish >= PUBLISH_INTERVAL_MS) {
    lastPublish = now;
    publishTelemetry();
  }
}