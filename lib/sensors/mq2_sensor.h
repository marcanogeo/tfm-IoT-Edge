#ifndef MQ2_SENSOR_H
#define MQ2_SENSOR_H

struct MQ2Reading{
    int raw;
    float voltage;
    float percent;
    float rs;
    float ratio;
    float ppm_est;
};
class MQ2Sensor{
  private:
    int pin;
    float vcc;
    float rl;
    float r0;
  public:
    MQ2Sensor(int analogPin, float supplyVoltage = 3.3, float loadResistance = 5.0, float baseResistance = 10.0){
      MQ2Reading read();
    }
};

//float CalculateRS(int adcValue, float vcc = 3.3, float rl = 5.0);

#endif // MQ2_SENSOR_H  