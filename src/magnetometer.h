#ifndef magnetometer_h
#define magnetometer_h

#include "periph/LSM9DS1.h"
#include "sensor.h"

class MagnetometerData: public SensorData {
  using SensorData::SensorData;
public:
  float x;
  float y;
  float z;
  using GattType = decltype(x);
};

class Magnetometer: public Sensor<Magnetometer, MagnetometerData> {
protected:
  void init() override {
    if (!_LSM9DS1.begin()) { halt(); }
  }
  void poll(msecu32_t const s) override {
    if (_LSM9DS1.lock()) {
      if (_LSM9DS1.magneticFieldAvailable()) {
        MagnetometerData d(s);
        _LSM9DS1.readMagneticField(d.x, d.y, d.z);
        push(d);
      }
      _LSM9DS1.unlock();
    }
  }
  std::string name() override { return "Magnetometer"; }
};

#endif // magnetometer_h
