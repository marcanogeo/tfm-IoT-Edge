#include <Arduino.h>
#include "mq2_sensor.h"

MQ2Sensor::MQ2Sensor(int analogPin, float supplyVoltage, float loadResistance, float baseResistance){
  pin = analogPin;
  vcc = supplyVoltage;
  rl = loadResistance;
  r0 = baseResistance; // This should be determined through calibration
}
MQ2Reading MQ2Sensor::read(){
    MQ2Reading result;
    result.raw = analogRead(pin); // Read the raw ADC value from the sensor
    result.voltage = (result.raw / 4095.0) * vcc; // Convert raw ADC value to voltage (assuming 12-bit ADC)
    result.percent = (result.raw / 4095.0) * 100,0; // Convert raw value to percentage (0-100%)
  
    if(result.voltage <= 0.01) {
      result.rs = -1; // Invalid RS value for very low voltage
      result.ratio = -1; // Invalid ratio if RS is invalid
      result.ppm_est = -1; // Cannot estimate ppm if ratio is invalid
      return result;
    }
    result.ppm_est = 1000.0 / result.ratio; // Placeholder estimation, should be based on sensor characteristics
    if(result.ppm_est < 0) result.ppm_est = 0; // Ensure ppm estimation is not negative

    return result;
}