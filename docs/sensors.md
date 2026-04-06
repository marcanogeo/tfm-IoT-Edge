
# Documentación de Sensores

En esta sección se describen los sensores utilizados en el proyecto, sus características y cómo se integran en el sistema.

## Sensores

## DHT22

- Mide temperatura y humedad
- Pin: GPIO 27

## LDR

- Lectura analógica de luminosidad
- Pin recomendado: GPIO 34

## MQ2

- Salida digital: presencia/no presencia
- Salida analógica: lectura continua
- Pines:
  - digital: GPIO 14
  - analógico: GPIO 35

## Nota técnica

El valor `gas_mq2_ppm_est` es una estimación relativa calculada a partir de la lectura analógica y una calibración base simplificada. No debe interpretarse como una medida certificada de laboratorio.