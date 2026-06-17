# Contrato de Payloads MQTT — Prototipo Físico

> **Rama:** `feature/physical-proto`

---

## Tópicos MQTT

La estructura de tópicos es idéntica a la rama `main`:

| Tópico | Dirección | Descripción |
|--------|-----------|-------------|
| `tfm/ambiental/{zone}/{device_id}/telemetria` | Edge → Broker | Telemetría periódica de sensores |
| `tfm/ambiental/{zone}/{device_id}/estado`     | Edge → Broker | Estado de conexión (`online`) |
| `tfm/ambiental/{zone}/{device_id}/cmd`        | Broker → Edge | Comandos remotos al nodo |
| `tfm/ambiental/{zone}/{device_id}/alerta`     | Edge → Broker | Alertas por umbral superado |

---

## Payload de telemetría

**Tópico:** `tfm/ambiental/{zone}/{device_id}/telemetria`
**Intervalo:** cada 5000 ms (configurable en `config.h`)

### Ejemplo

```json
{
  "device_id": "nodo_1",
  "zone": "zona_norte",
  "timestamp": 1718123456,
  "sensors": {
    "temperature_c": 23.45,
    "humidity_pct":  55.10,
    "pressure_hpa":  1013.25,
    "light_lux":     342.50,
    "aqi":           2,
    "eco2_ppm":      645,
    "tvoc_ppb":      112,
    "ens160_status": 0
  }
}
```

### Descripción de campos

| Campo             | Tipo    | Unidad | Rango           | Descripción |
|-------------------|---------|--------|-----------------|-------------|
| `device_id`       | string  | —      | —               | Identificador del nodo |
| `zone`            | string  | —      | —               | Zona geográfica del nodo |
| `timestamp`       | integer | s      | Unix epoch      | Marca temporal UTC (NTP) |
| `temperature_c`   | float   | °C     | -40 … 85        | Temperatura ambiente (BME280) |
| `humidity_pct`    | float   | %      | 0 … 100         | Humedad relativa (BME280) |
| `pressure_hpa`    | float   | hPa    | 300 … 1100      | Presión atmosférica (BME280) |
| `light_lux`       | float   | lux    | 0 … 65535       | Iluminancia (BH1750). `-1` si falla |
| `aqi`             | integer | —      | 1 … 5           | Índice calidad del aire (ENS160) |
| `eco2_ppm`        | integer | ppm    | 400 … 65000     | CO₂ equivalente (ENS160) |
| `tvoc_ppb`        | integer | ppb    | 0 … 65000       | Compuestos orgánicos volátiles (ENS160) |
| `ens160_status`   | integer | —      | 0, 1, 2         | 0=Normal · 1=Calentando · 2=Calibrando |

### Escala AQI (ENS160)

| Valor | Categoría     | Acción recomendada                    |
|-------|---------------|---------------------------------------|
| 1     | Excelente     | Sin acción                            |
| 2     | Buena         | Sin acción                            |
| 3     | Moderada      | Ventilar si es posible                |
| 4     | Deficiente    | Ventilar; reducir actividad intensa   |
| 5     | Muy deficiente| Ventilar urgentemente; alerta activa  |

---

## Umbrales de alerta

Definidos en `include/config.h`. Node-RED evalúa estos umbrales
y publica en el tópico `/alerta` cuando se superan.

| Variable        | Umbral        | Condición        |
|-----------------|---------------|------------------|
| `aqi`           | > 3           | Calidad deficiente |
| `eco2_ppm`      | > 1000 ppm    | Ventilación necesaria |
| `tvoc_ppb`      | > 500 ppb     | Nivel de atención |
| `temperature_c` | > 35.0 °C     | Temperatura alta |
| `humidity_pct`  | > 80 %        | Humedad excesiva |
| `light_lux`     | < 10 lux      | Oscuridad |

---

## Diferencias respecto a la rama `main` (simulación)

| Campo simulación   | Campo físico      | Cambio |
|--------------------|-------------------|--------|
| `temperature_c`    | `temperature_c`   | Mismo nombre, sensor distinto (DHT22 → BME280) |
| `humidity_pct`     | `humidity_pct`    | Mismo nombre, sensor distinto (DHT22 → BME280) |
| ❌ no existía      | `pressure_hpa`    | Campo nuevo |
| `light_raw` (ADC)  | `light_lux`       | Renombrado + unidades reales |
| `gas_mq2_*`        | `aqi`, `eco2_ppm`, `tvoc_ppb` | Sustituido por ENS160 |
| ❌ no existía      | `ens160_status`   | Campo nuevo para trazabilidad |
