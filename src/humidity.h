#ifndef humidity_h
#define humidity_h

#include "periph/HTS221.h"
#include "sensor.h"

class HumidityData: public SensorData {
  using SensorData::SensorData;
public:
  float humidity;
  using GattType = decltype(humidity);
};

class Humidity: public Sensor<Humidity, HumidityData> {
protected:
  void init() override {
    if (!_HTS221.begin()) { halt(); }
  }
  void poll(msecu32_t const s) override {
    if (_HTS221.lock()) {
      HumidityData d(s);
      d.humidity = _HTS221.readHumidity();
      push(d);
      _HTS221.unlock();
    }
  }
  std::string name() override { return "Humidity"; }
};

#endif // humidity_h
