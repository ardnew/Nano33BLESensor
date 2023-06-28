#ifndef accelerometer_h
#define accelerometer_h

#include "periph/LSM9DS1.h"
#include "sensor.h"

class AccelerometerData: public SensorData {
  using SensorData::SensorData;
public:
  float x;
  float y;
  float z;
};

class Accelerometer: public Sensor<Accelerometer, AccelerometerData> {
protected:
  void init() override {
    if (!_LSM9DS1.begin()) { halt(); }
  }
  void poll(msec_t const s) override {
    if (_LSM9DS1.lock()) {
      if (_LSM9DS1.accelerationAvailable()) {
        AccelerometerData d(s);
        _LSM9DS1.readAcceleration(d.x, d.y, d.z);
        push(d);
      }
      _LSM9DS1.unlock();
    }
  }
  std::string name() override { return "Accelerometer"; }
};

#endif // accelerometer_h
