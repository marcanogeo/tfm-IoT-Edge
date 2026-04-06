#ifndef MQ2_SENSOR_H
#define MQ2_SENSOR_H

struct MQ2Reading {
  int raw;
  int digital;
  float voltage;
  float percent;
  float rs;
  float ratio;
  float ppm_est;
};

class MQ2Sensor {
  private:
    int analogPin;
    int digitalPin;
    float vcc;
    float rl;
    float r0;

  public:
    MQ2Sensor(int analogPin, int digitalPin, float vcc, float rl, float r0);
    void begin();
    MQ2Reading read();
};

#endif