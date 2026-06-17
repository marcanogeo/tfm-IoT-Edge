# Documentación de Sensores — Prototipo Físico

> **Rama:** `feature/physical-proto`
> Esta rama documenta el prototipo físico del nodo IoT.
> Para la simulación Wokwi consulta la rama `main`.

---

## Módulo ENS160 + BME280 (SparkFun Qwiic)

Un único módulo integra dos sensores que trabajan de forma conjunta:
el BME280 proporciona temperatura y humedad al ENS160 para compensar
sus lecturas de calidad del aire, mejorando su precisión.

### BME280 — Sensor atmosférico (Bosch)

| Parámetro       | Valor                        |
|-----------------|------------------------------|
| Variables        | Temperatura, humedad, presión |
| Interfaz         | I²C                          |
| Dirección I²C    | `0x77` (por defecto)         |
| Pin SDA          | GPIO 21                      |
| Pin SCL          | GPIO 22                      |
| Tensión          | 3.3 V                        |
| Precisión temp.  | ± 0.5 °C                     |
| Precisión hum.   | ± 3 %                        |

### ENS160 — Sensor de calidad del aire (ScioSense)

| Parámetro       | Valor                                      |
|-----------------|--------------------------------------------|
| Variables        | AQI, eCO₂ equivalente, TVOCs              |
| Interfaz         | I²C                                       |
| Dirección I²C    | `0x53` (por defecto)                      |
| Pin SDA          | GPIO 21                                   |
| Pin SCL          | GPIO 22                                   |
| Tensión          | 3.3 V                                     |
| Rango eCO₂       | 400 – 65000 ppm                           |
| Rango TVOC       | 0 – 65000 ppb                             |
| AQI              | 1 (Excelente) → 5 (Malo)                  |

> **Nota técnica:** el ENS160 requiere un período de calentamiento de
> aproximadamente 60 segundos tras el encendido antes de ofrecer lecturas
> estables. Durante ese tiempo `ens160_status` será distinto de 0.
> Las lecturas de ese período se publican igualmente para trazabilidad,
> pero no deben usarse como referencia en análisis histórico.

---

## BH1750 — Sensor de luminosidad

| Parámetro       | Valor                        |
|-----------------|------------------------------|
| Variable         | Iluminancia (lux)            |
| Interfaz         | I²C                          |
| Dirección I²C    | `0x23` (ADDR pin a GND)      |
| Pin SDA          | GPIO 21                      |
| Pin SCL          | GPIO 22                      |
| Tensión          | 3.3 V                        |
| Rango            | 1 – 65535 lx                 |
| Modo             | `CONTINUOUS_HIGH_RES_MODE`   |

> **Nota técnica:** a diferencia del LDR utilizado en la simulación,
> el BH1750 devuelve valores calibrados en lux reales, lo que permite
> definir umbrales absolutos en Node-RED sin conversiones adicionales.

---

## Esquema de conexión (bus I²C compartido)

```
ESP32 (GPIO 21 SDA, GPIO 22 SCL, 3.3V, GND)
        │
        ├──── ENS160/BME280 (módulo SparkFun Qwiic)
        │       SDA → GPIO 21
        │       SCL → GPIO 22
        │       VCC → 3.3V
        │       GND → GND
        │
        └──── BH1750
                SDA  → GPIO 21
                SCL  → GPIO 22
                VCC  → 3.3V
                GND  → GND
                ADDR → GND  (dirección 0x23)
```

Los tres sensores comparten el bus I²C sin conflictos de dirección:
`0x53` (ENS160), `0x77` (BME280), `0x23` (BH1750).

---

## Comparativa simulación vs prototipo físico

| Variable          | Simulación (`main`) | Físico (`feature/physical-proto`) |
|-------------------|---------------------|-----------------------------------|
| Temperatura       | DHT22 — relativa    | BME280 — ± 0.5 °C                |
| Humedad           | DHT22 — relativa    | BME280 — ± 3 %                   |
| Presión           | ❌ no disponible    | BME280 — hPa                     |
| Luminosidad       | LDR — valor ADC raw | BH1750 — lux calibrados          |
| Calidad del aire  | MQ2 — ppm estimada  | ENS160 — AQI + eCO₂ + TVOC      |
