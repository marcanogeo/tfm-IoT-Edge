# TFM IoT Edge

Repositorio de firmware y simulación edge del sistema IoT distribuido del TFM.

## Objetivo

Implementar nodos ESP32 simulados en Wokwi y compilados con PlatformIO para publicar telemetría ambiental vía MQTT.

## Sensores

- DHT22: temperatura y humedad
- MQ2: lectura digital y analógica de gas/humo
- LDR: luminosidad

## Entornos de compilación

- `nodo1` → `nodo_1` / `zona_norte`
- `nodo2` → `nodo_2` / `zona_sur`
- `nodo3` → `nodo_3` / `zona_este`
- `nodo4` → `nodo_4` / `zona_oeste`

## Compilación

```bash
platformio run -e nodo1
platformio run -e nodo2
platformio run -e nodo3
platformio run -e nodo4
```
