#ifndef gyroscope_h
#define gyroscope_h

#include "periph/LSM9DS1.h"
#include "sensor.h"

class GyroscopeData: public SensorData {
  using SensorData::SensorData;
public:
  float x;
  float y;
  float z;
  using Type = decltype(x);
};

class Gyroscope: public Sensor<Gyroscope, GyroscopeData> {
protected:
  void init() override {
    if (!_LSM9DS1.begin()) { halt(); }
  }
  void poll(msecu32_t const s) override {
    if (_LSM9DS1.lock()) {
      if (_LSM9DS1.gyroscopeAvailable()) {
        GyroscopeData d(s);
        _LSM9DS1.readGyroscope(d.x, d.y, d.z);
        push(d);
      }
      _LSM9DS1.unlock();
    }
  }
  std::string name() override { return "Gyroscope"; }
};

#endif // gyroscope_h
