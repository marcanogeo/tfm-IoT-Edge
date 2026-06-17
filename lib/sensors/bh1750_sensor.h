#ifndef BH1750_SENSOR_H
#define BH1750_SENSOR_H

#include <Arduino.h>

// ─────────────────────────────────────────────
// Estructura de lectura del BH1750
// ─────────────────────────────────────────────
struct BH1750Reading {
  float lux;    // Iluminancia en lux (0 – 65535 lx)
  bool  valid;  // false si la lectura falló
};

// ─────────────────────────────────────────────
// Clase BH1750Sensor
// Encapsula el sensor de luminosidad BH1750 via I²C.
// Requiere que Wire.begin() se haya llamado antes de begin().
// ─────────────────────────────────────────────
class BH1750Sensor {
  public:
    bool          begin();
    BH1750Reading read();
};

#endif
