#ifndef temperature_h
#define temperature_h

#include "periph/HTS221.h"
#include "sensor.h"

class TemperatureData: public SensorData {
  using SensorData::SensorData;
public:
  float fahrenheit;
  using Type = decltype(fahrenheit);
};

class Temperature: public Sensor<Temperature, TemperatureData> {
protected:
  void init() override {
    if (!_HTS221.begin()) { halt(); }
  }
  void poll(msecu32_t const s) override {
    if (_LSM9DS1.lock()) {
      TemperatureData d(s);
      d.fahrenheit = _HTS221.readTemperature(FAHRENHEIT);
      push(d);
      _LSM9DS1.unlock();
    }
  }
  std::string name() override { return "Temperature"; }
};

#endif // temperature_h
