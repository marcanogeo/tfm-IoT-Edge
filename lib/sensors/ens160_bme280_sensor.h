#ifndef ENS160_BME280_SENSOR_H
#define ENS160_BME280_SENSOR_H

#include <Arduino.h>

// ─────────────────────────────────────────────
// Estructura de lectura combinada ENS160 + BME280
// ─────────────────────────────────────────────
struct AtmoReading {
  // BME280
  float temperature_c;    // Temperatura en grados Celsius
  float humidity_pct;     // Humedad relativa en porcentaje
  float pressure_hpa;     // Presión atmosférica en hPa (hectopascales)

  // ENS160
  int   aqi;              // Índice de Calidad del Aire (1=Excelente … 5=Malo)
  int   eco2_ppm;         // CO₂ equivalente en ppm
  int   tvoc_ppb;         // Compuestos orgánicos volátiles en ppb
  uint8_t ens160_status;  // Estado interno del ENS160 (0=Normal, 1=Calentando, 2=Calibrando)

  bool  valid;            // false si alguna lectura falló
};

// ─────────────────────────────────────────────
// Clase AtmoSensor
// Gestiona el módulo combinado ENS160/BME280 via I²C.
// El BME280 alimenta al ENS160 con temperatura y humedad
// para mejorar la precisión de sus lecturas de calidad del aire.
// Requiere que Wire.begin() se haya llamado antes de begin().
// ─────────────────────────────────────────────
class AtmoSensor {
  public:
    bool        begin();
    AtmoReading read();
};

#endif
