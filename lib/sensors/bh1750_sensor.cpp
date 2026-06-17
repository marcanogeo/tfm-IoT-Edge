#include "bh1750_sensor.h"
#include <BH1750.h>

static BH1750 bh1750;

bool BH1750Sensor::begin() {
  if (!bh1750.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("[BH1750] Error al inicializar");
    return false;
  }
  Serial.println("[BH1750] Inicializado correctamente");
  return true;
}

BH1750Reading BH1750Sensor::read() {
  BH1750Reading result{};

  if (!bh1750.measurementReady()) {
    // La medición aún no está lista, devolvemos el último valor conocido
    result.lux   = bh1750.readLightLevel();
    result.valid = (result.lux >= 0);
    return result;
  }

  result.lux   = bh1750.readLightLevel();
  result.valid = (result.lux >= 0);

  if (!result.valid) {
    Serial.println("[BH1750] Lectura inválida");
  }

  return result;
}
