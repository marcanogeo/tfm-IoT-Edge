#ifndef CONFIG_H
#define CONFIG_H

#define DHT_PIN 27  // Pin where the DHT22 sensor is connected
#define MQ2_DIGITAL_PIN 14 //Pin where the MQ2 sensor is connected
#define MQ2_ANALOG_PIN 35 //Pin for analog reading of MQ2 sensor (if needed)
#define LDR_PIN 34 //Pin for light sensor (LDR)

#define SERIAL_BAUDRATE 115200 // Baud rate for serial communication
#define ADC_RESOLUTION_BITS 12 // ADC resolution in bits (e.g., 12 bits for ESP32)
#define PUBLISH_INTERVAL_MS 5000 // Interval for publishing telemetry data in milliseconds

#define MQ2_VCC 3.3 // Supply voltage for MQ2 sensor
#define MQ2_RL 5.0 // Load resistance for MQ2 sensor (in kilo-ohms)
#define MQ2_R0 10.0 // Base resistance for MQ2 sensor (in kilo-ohms), should be determined through calibration

#endif