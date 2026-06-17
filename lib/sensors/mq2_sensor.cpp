#include <Arduino.h>
#include "mq2_sensor.h"

MQ2Sensor::MQ2Sensor(int analogPin, int digitalPin, float vcc, float rl, float r0)
  : analogPin(analogPin), digitalPin(digitalPin), vcc(vcc), rl(rl), r0(r0) {}

void MQ2Sensor::begin() {
  pinMode(digitalPin, INPUT);
}

MQ2Reading MQ2Sensor::read() {
  MQ2Reading result{};
  result.raw = analogRead(analogPin);
  result.digital = digitalRead(digitalPin);

  result.voltage = (result.raw / 4095.0f) * vcc;
  result.percent = (result.raw / 4095.0f) * 100.0f;

  if (result.voltage <= 0.01f) {
    result.rs = -1;
    result.ratio = -1;
    result.ppm_est = -1;
    return result;
  }

  result.rs = ((vcc - result.voltage) * rl) / result.voltage;
  result.ratio = result.rs / r0;
  result.ppm_est = 1000.0f / result.ratio;

  if (result.ppm_est < 0) {
    result.ppm_est = 0;
  }

  return result;
}