# IoTSystem

Este proyecto es una plataforma IoT basada en PlatformIO, diseñada para gestionar múltiples nodos simulados en Wokwi. Incluye ejemplos de código, configuraciones y diagramas para facilitar el desarrollo y la simulación de sistemas IoT.

## Estructura del proyecto

- **src/**: Código fuente principal del proyecto.
- **include/**: Archivos de cabecera y definiciones.
- **lib/**: Librerías adicionales utilizadas por el proyecto.
- **test/**: Pruebas unitarias y de integración.
- **wokwi-nodo1/**, **wokwi-nodo2/**, **wokwi-nodo3/**, **wokwi-nodo4/**: Carpetas de simulación para cada nodo en Wokwi, cada una con su propio diagrama y configuración.
- **platformio.ini**: Archivo de configuración de PlatformIO para definir entornos y dependencias.
- **time.cpp**: Archivo fuente adicional relacionado con la gestión de tiempo.

## Requisitos

- [PlatformIO](https://platformio.org/)
- [Wokwi](https://wokwi.com/)

## Uso

1. Clona este repositorio.
2. Abre el proyecto en VS Code con la extensión de PlatformIO instalada.
3. Compila y sube el firmware a los nodos simulados usando los entornos definidos en `platformio.ini`.
4. Utiliza los diagramas de Wokwi para simular el hardware de cada nodo.

## Backend MQTT -> InfluxDB

Servicio Python que:

1. Se suscribe a `tfm/ambiental/+/telemetria`
2. Valida el payload JSON
3. Escribe telemetría en InfluxDB v2

## Ejecución local

```bash
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
cp .env.example .env
python app/main.py


Licencia

Consulta el archivo LICENSE para más detalles.