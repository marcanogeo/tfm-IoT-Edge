# TFM IoT Edge - Firmware y Simulación

**Repositorio de firmware y simulación del lado edge** para un sistema IoT distribuido con múltiples nodos sensores conectados vía MQTT.

Este proyecto implementa nodos ESP32 virtualizados con sensores ambientales que publican telemetría a un broker MQTT. Incluye configuraciones para simular 4 nodos diferentes usando Wokwi y compilar con PlatformIO.

## 📋 Tabla de contenidos

- [Sensores](#sensores)
- [Compilación de nodos](#compilación-de-nodos)
- [Simulación en Wokwi](#simulación-en-wokwi)
- [Tópicos MQTT](#tópicos-mqtt)
- [Payload de telemetría](#payload-de-telemetría)
- [Estructura del proyecto](#estructura-del-proyecto)
- [Requisitos](#requisitos)
- [Repositorio complementario](#repositorio-complementario)

## Sensores

Cada nodo está equipado con los siguientes sensores ambientales:

| Sensor | Descripción | Medida | Pin (ESP32) |
|--------|-------------|--------|-------------|

| **DHT22** | Temperatura y Humedad | °C, %                   | GPIO 27                                |
|-----------|-----------------------|-------------------------|----------------------------------------|
| **LDR**   | Luminosidad           | Luxes / Valor analógico | GPIO 35                                |
|-----------|-----------------------|-------------------------|----------------------------------------|
| **MQ2**   | Detección de gas/humo | PPM                     | GPIO 14 (digital), GPIO 35 (analógico) |
|-----------|-----------------------|-------------------------|----------------------------------------|

## Compilación de nodos

### Requisitos previos

- [VS Code](https://code.visualstudio.com/) con extensión [PlatformIO](https://platformio.org/)
- O [PlatformIO CLI](https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html#super-quick-mac-linux)

### Compilar cada nodo

Cada nodo se compila como un entorno diferente en PlatformIO:

```bash
# Compilar Nodo 1 (Zona Norte)
platformio run -e nodo1

# Compilar Nodo 2 (Zona Sur)
platformio run -e nodo2

# Compilar Nodo 3 (Zona Este)
platformio run -e nodo3

# Compilar Nodo 4 (Zona Oeste)
platformio run -e nodo4

# Compilar todos los nodos
platformio run
```

### Configuración de compilación

Cada nodo se compila con un **NODE_NUM** diferente (1-4) que se define en `platformio.ini` como `build_flags`:

```ini
[env:nodo1]
build_flags =
    -D NODE_NUM=1
```

Esta definición determina automáticamente:

- El ID del nodo
- La zona geográfica
- Los tópicos MQTT específicos

## Simulación en Wokwi

### Requisitos para Simulación

- Cuenta activa en [Wokwi](https://wokwi.com/)
- Conexión al simulador desde VS Code o navegador web

### Lanzar simulación para cada nodo

Cada nodo tiene su própria carpeta de configuración Wokwi:

``` plain
wokwi/
├── nodo1/
│   ├── diagram.json
│   └── wokwi.toml
├── nodo2/
│   ├── diagram.json
│   └── wokwi.toml
├── nodo3/
│   ├── diagram.json
│   └── wokwi.toml
└── nodo4/
    ├── diagram.json
    └── wokwi.toml
```

**Opción 1: Desde VS Code

1. Instala la extensión [Wokwi Simulator](https://docs.wokwi.com/en/vscode/getting-started)
2. Abre la carpeta `wokwi/nodoX/` en VS Code
3. Presiona `F1` → `Wokwi: Start Simulator`

**Opción 2: Desde Wokwi web

1. Navega a [https://wokwi.com/](https://wokwi.com/)
2. Importa o crea un nuevo proyecto
3. Copia la configuración del `diagram.json` y `wokwi.toml` del nodo deseado

### Configuración de red (Wokwi)

La simulación se conecta automáticamente a:

- **WiFi SSID**: `Wokwi-GUEST` (red simulada)
- **Broker MQTT**: `broker.hivemq.com` (público)
- **Puerto MQTT**: 1883

## Tópicos MQTT

Los nodos publican en los siguientes tópicos:

### Patrón general

``` plain
tfm/ambiental/{zona}/{nodo_id}/{tipo}
```

### Tópicos por nodo

| Nodo | Zona | Tópico Telemetría | Tópico Estado | Tópico Comandos | Tópico Alertas |
|------|------|-------------------|---------------|-----------------|----------------|

| **Nodo 1** | Zona Norte | `tfm/ambiental/zona_norte/nodo_1/telemetria` | `tfm/ambiental/zona_norte/nodo_1/estado` | `tfm/ambiental/zona_norte/nodo_1/cmd` | `tfm/ambiental/zona_norte/nodo_1/alerta` |
| **Nodo 2** | Zona Sur | `tfm/ambiental/zona_sur/nodo_2/telemetria` | `tfm/ambiental/zona_sur/nodo_2/estado` | `tfm/ambiental/zona_sur/nodo_2/cmd` | `tfm/ambiental/zona_sur/nodo_2/alerta` |
| **Nodo 3** | Zona Este | `tfm/ambiental/zona_este/nodo_3/telemetria` | `tfm/ambiental/zona_este/nodo_3/estado` | `tfm/ambiental/zona_este/nodo_3/cmd` | `tfm/ambiental/zona_este/nodo_3/alerta` |
| **Nodo 4** | Zona Oeste | `tfm/ambiental/zona_oeste/nodo_4/telemetria` | `tfm/ambiental/zona_oeste/nodo_4/estado` | `tfm/ambiental/zona_oeste/nodo_4/cmd` | `tfm/ambiental/zona_oeste/nodo_4/alerta` |

## Payload de telemetría

### Formato JSON

Los nodos publican un payload JSON con los siguientes campos:

```json
{
  "timestamp": "2026-04-06T14:30:45Z",
  "nodo_id": "nodo_1",
  "zona_id": "zona_norte",
  "sensores": {
    "temperatura": 22.5,
    "humedad": 65.3,
    "luminosidad": 450,
    "gas_ppm": 15.2,
    "gas_detectado": false
  }
}
```

### Campos del payload

| Campo | Descripción | Unidad | Rango |
|-------|-------------|--------|-------|

| `timestamp` | Marca de tiempo ISO 8601 | - | UTC |
| `nodo_id` | Identificador del nodo | - | nodo_1 a nodo_4 |
| `zona_id` | Zona geográfica | - | zona_norte, zona_sur, zona_este, zona_oeste |
| `temperatura` | Temperatura ambiente | °C | -40 a +80 |
| `humedad` | Humedad relativa | % | 0 a 100 |
| `luminosidad` | Nivel de luz | Luxes | 0 a 4095 (analógico) |
| `gas_ppm` | Concentración de gas | PPM | 0 a ~10000 |
| `gas_detectado` | Alerta de gas | bool | true/false |

## Estructura del proyecto

``` plain
.
├── src/                          # Código fuente principal
│   ├── main.cpp                  # Punto de entrada
│   ├── app_config.cpp            # Configuración de WiFi y MQTT
│   ├── app_loop.cpp              # Loop principal
│   └── app_publish.cpp           # Publicación de datos
├── include/                      # Archivos de cabecera
│   ├── config.h
│   ├── mqtt_config.h
│   ├── wifi_config.h
│   ├── topics.h
│   └── node_metadata.h
├── lib/                          # Librerías personalizadas
│   ├── mqtt_client/
│   │   ├── mqtt_client.h
│   │   └── mqtt_client.cpp
│   ├── sensors/
│   │   ├── dht_sensor.h
│   │   ├── dht_sensor.cpp
│   │   ├── ldr_sensor.h
│   │   ├── ldr_sensor.cpp
│   │   ├── mq2_sensor.h
│   │   └── mq2_sensor.cpp
│   └── telemetry/
│       ├── telemetry_builder.h
│       └── telemetry_builder.cpp
├── wokwi/                        # Simulaciones Wokwi
│   ├── nodo1/
│   ├── nodo2/
│   ├── nodo3/
│   └── nodo4/
├── docs/                         # Documentación
│   ├── arquitecture_edge.md
│   ├── contrate_mqtt.md
│   ├── payloads.md
│   └── sensors.md
├── platformio.ini                # Configuración PlatformIO
└── README.md                     # Este archivo
```

## Requisitos

### Hardware (para deployment real)

- ESP32 Feather u otro microcontrolador compatible
- DHT22 (Sensor temperatura/humedad)
- LDR (Fotoresistor)
- MQ2 (Sensor gas/humo)
- Pulsadores, LEDs y resistencias

### Software

- [VS Code](https://code.visualstudio.com/)
- [PlatformIO IDE](https://platformio.org/install/ide?install=vscode) o [PlatformIO CLI](https://docs.platformio.org/en/latest/core/installation/)
- [Wokwi Simulator](https://docs.wokwi.com/en/vscode/getting-started) (para simulación)
- [Git](https://git-scm.com/)

## Repositorio complementario

El backend y la orquestación de servicios se encuentran en:

**[tfm-IoT-Backend](https://github.com/marcanogeo/tfm-IoT-Backend)**
*(o repositorio complementario)*

Contiene la infraestructura completa:

- **Docker Compose**: Orquestación de servicios
- **Node-RED**: Procesamiento y automatización de flujos MQTT
- **InfluxDB**: Base de datos de series temporales
- **MongoDB**: Base de datos NoSQL para metadatos
- **Grafana**: Visualización y dashboards

### Flujo de datos

``` plain
Nodos ESP32 (this repo)
    ↓
MQTT Broker (HiveMQ)
    ↓
Node-RED (backend repo)
    ↓
InfluxDB + MongoDB
    ↓
Grafana Dashboards
```

## Documentación adicional

- [Arquitectura del sistema](./docs/arquitecture_edge.md)
- [Contrato MQTT](./docs/contrate_mqtt.md)
- [Especificación de payloads](./docs/payloads.md)
- [Detalles de sensores](./docs/sensors.md)

## Licencia

Consulta el archivo [LICENSE](./LICENSE) para más detalles.

## Autor

**marcanogeo** - [GitHub](https://github.com/marcanogeo)

---

**Última actualización**: Abril 2026
