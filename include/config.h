#ifndef CONFIG_H
#define CONFIG_H

// ─────────────────────────────────────────────
// Bus I²C — compartido por ENS160, BME280 y BH1750
// ─────────────────────────────────────────────
#define I2C_SDA_PIN        21
#define I2C_SCL_PIN        22

// Direcciones I²C de los sensores
#define BH1750_I2C_ADDR    0x23   // ADDR pin a GND (por defecto)
#define BME280_I2C_ADDR    0x77   // SDO pin a VCC (por defecto)
#define ENS160_I2C_ADDR    0x53   // ADDR pin a GND (por defecto)

// ─────────────────────────────────────────────
// Comunicación serie
// ─────────────────────────────────────────────
#define SERIAL_BAUDRATE    115200

// ─────────────────────────────────────────────
// Intervalo de publicación MQTT (ms)
// ─────────────────────────────────────────────
#define PUBLISH_INTERVAL_MS 5000

// ─────────────────────────────────────────────
// Umbrales de alerta ambiental
// ─────────────────────────────────────────────
#define ALERT_AQI_MAX       3       // AQI > 3 = calidad de aire deficiente
#define ALERT_ECO2_MAX      1000    // eCO2 > 1000 ppm = ventilación necesaria
#define ALERT_TVOC_MAX      500     // TVOC > 500 ppb = nivel de atención
#define ALERT_TEMP_MAX      35.0f   // Temperatura > 35 °C
#define ALERT_HUMIDITY_MAX  80.0f   // Humedad > 80 %
#define ALERT_LUX_MIN       10.0f   // Luminosidad < 10 lux = oscuridad

#endif
