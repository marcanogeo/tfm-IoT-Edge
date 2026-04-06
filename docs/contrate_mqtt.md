# Contrato MQTT

## Patrón de tópicos

`tfm/ambiental/{zona}/{nodo}/{tipo}`

## Tipos

- `telemetria`
- `estado`
- `cmd`
- `alerta`

## Ejemplo

`tfm/ambiental/zona_norte/nodo_1/telemetria`

## Payload de telemetría

```json
{
  "device_id": "nodo_1",
  "zone": "zona_norte",
  "timestamp": 1712345678,
  "sensors": {
    "temperature_c": 24.5,
    "humidity_pct": 61.2,
    "light_raw": 3200,
    "gas_detected": 0,
    "gas_mq2_raw": 1420,
    "gas_mq2_ratio": 2.18,
    "gas_mq2_ppm_est": 410.2
  }
}