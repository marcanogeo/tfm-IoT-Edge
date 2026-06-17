#include "ens160_bme280_sensor.h"
#include <SparkFun_ENS160.h>
#include <SparkFunBME280.h>

static SparkFun_ENS160 ens160;
static BME280          bme280;

bool AtmoSensor::begin() {
  bool ok = true;

  // ── BME280 ────────────────────────────────
  bme280.setI2CAddress(0x77);
  if (!bme280.beginI2C()) {
    Serial.println("[BME280] Error al inicializar");
    ok = false;
  } else {
    Serial.println("[BME280] Inicializado correctamente");
  }

  // ── ENS160 ────────────────────────────────
  if (!ens160.begin()) {
    Serial.println("[ENS160] Error al inicializar");
    ok = false;
  } else {
    // Modo estándar: mediciones cada segundo
    ens160.setOperatingMode(SFE_ENS160_STANDARD);
    Serial.println("[ENS160] Inicializado correctamente");
  }

  return ok;
}

AtmoReading AtmoSensor::read() {
  AtmoReading result{};

  // ── Leer BME280 primero ───────────────────
  result.temperature_c = bme280.readTempC();
  result.humidity_pct  = bme280.readFloatHumidity();
  result.pressure_hpa  = bme280.readFloatPressure() / 100.0f; // Pa → hPa

  bool bme_ok = !isnan(result.temperature_c) &&
                !isnan(result.humidity_pct)  &&
                result.pressure_hpa > 0;

  if (!bme_ok) {
    Serial.println("[BME280] Lectura inválida");
    result.valid = false;
    return result;
  }

  // ── Compensación temperatura/humedad en ENS160 ──
  // Mejora la precisión de AQI y eCO2 con los datos del BME280
  ens160.setTempCompensationCelsius(result.temperature_c);
  ens160.setRHCompensationFloat(result.humidity_pct);

  // ── Leer ENS160 ──────────────────────────
  result.ens160_status = ens160.getFlags();
  result.aqi           = ens160.getAQI();
  result.eco2_ppm      = ens160.getECO2();
  result.tvoc_ppb      = ens160.getTVOC();

  bool ens_ok = (result.aqi >= 1 && result.aqi <= 5) &&
                (result.eco2_ppm > 0) &&
                (result.tvoc_ppb >= 0);

  if (!ens_ok) {
    Serial.println("[ENS160] Lectura inválida o sensor en calentamiento");
    // Permitimos continuar: durante el calentamiento inicial los valores
    // pueden ser 0, pero son informativos para el log
  }

  result.valid = bme_ok; // consideramos válida si al menos el BME280 responde
  return result;
}
