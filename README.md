# TFM IoT Edge

Firmware y simulación edge para el TFM: nodos ESP32 que publican telemetría ambiental vía MQTT.

Estado de ramas
----------------
- `main` — Rama orientada a la **simulación en Wokwi**. Contiene entornos y ejemplos pensados para pruebas sin hardware físico.
- `physical-prototype` — Rama del **prototipo físico**. Ajustes y código para sensores reales, esquemas de cableado y notas de montaje.

Propósito
---------
Este repositorio agrupa dos líneas de trabajo compatibles: simulación (rápida verificación en Wokwi) y prototipo físico (pruebas con sensores reales). Ambas comparten la estructura del proyecto y la infraestructura de publicación MQTT, pero difieren en drivers, pines y calibración de sensores.

Estructura del repositorio
--------------------------
- `src/` — código principal (aplicación, loop, publicación).
- `lib/` — librerías por sensor y telemetría (`lib/sensors`, `lib/mqtt_client`, `lib/telemetry`).
- `include/` — archivos de configuración y topics (`wifi_config.h`, `mqtt_config.h`, `topics.h`).
- `platformio.ini` — entornos PlatformIO por nodo y variante.
- `docs/` — documentación: arquitectura, payloads, sensores y notas de montaje.

Diferencias clave por rama
--------------------------
- `main` (simulación):
	- Sensores típicos simulados: DHT22, MQ2, LDR (ver `docs/sensors.md`).
	- Diseñado para ejecutarse en Wokwi o emuladores.
	- Entornos PlatformIO con parámetros pensados para pruebas sin hardware.
- `physical-prototype` (físico):
	- Código adaptado a los sensores reales del prototipo (consultar `lib/sensors/*` y `docs/sensors.md`).
	- Añade notas de cableado y requerimientos de alimentación.
	- Puede incluir calibraciones y filtros adicionales en `src/`.

Compilación (ejemplos)
----------------------
Usa PlatformIO; los entornos se definen en `platformio.ini`. Reemplaza `<env>` por el entorno deseado (por ejemplo `nodo1`).

```bash
platformio run -e nodo1
platformio run -e nodo2
platformio run -e nodo3
platformio run -e nodo4
```

Si trabajas sobre la rama `main` (simulación) o `physical-prototype` (físico), cambia a la rama correspondiente antes de compilar:

```bash
git checkout main
# o
git checkout physical-prototype
```

Configuración de red y MQTT
---------------------------
- Edita `include/wifi_config.h` para tu SSID y contraseña.
- Edita `include/mqtt_config.h` para configurar broker, puerto y credenciales.
- Topics y metadatos están en `include/topics.h` y `include/node_metadata.h`.

Pruebas y verificación (MQTT)
----------------------------
Ejemplo rápido para ver telemetría (reemplaza `broker.example.com` y el topic según config):

```bash
mosquitto_sub -h broker.example.com -t 'zona_norte/telemetry' -v
```

Simulación en Wokwi
-------------------
En `main` hay ejemplos orientados a Wokwi. Abre los archivos de proyecto/escena (si existen) o sigue `docs/` para recrear el circuito en Wokwi. La simulación permite iterar sin necesidad de hardware.

Prototipo físico
----------------
- Consulta `docs/sensors.md` para el listado de sensores y su conexión.
- Recomendación: verificar tensión y conexiones antes de alimentar el dispositivo.
- Pines críticos y ejemplos de conexionado se encuentran en los archivos de `lib/sensors` y en `docs/`.

Flujo de contribución
---------------------
- Para cambios de simulación, crea ramas a partir de `main` y abre PR contra `main`.
- Para cambios de hardware o calibración del prototipo, crea ramas a partir de `physical-prototype` y abre PR contra esa rama.
- Reporta issues para fallos, mejoras o dudas sobre sensores/telemetría.

Documentación adicional
-----------------------
- `docs/arquitecture_edge.md` — arquitectura del sistema.
- `docs/payloads.md` — formato de mensajes MQTT.
- `docs/sensors.md` — lista y notas de sensores por rama.

Licencia
--------
Proyecto con licencia en el fichero `LICENSE`.

Contacto
-------
Para dudas o soporte, abre un issue en el repo.
