#ifndef proximity_h
#define proximity_h

#include "periph/APDS9960.h"
#include "sensor.h"

class ProximityData: public SensorData {
  using SensorData::SensorData;
public:
  int proximity;
  using GattType = decltype(proximity);
};

class Proximity: public Sensor<Proximity, ProximityData> {
protected:
  void init() override {
    if (!_APDS9960.begin()) { halt(); }
    _APDS9960.setLEDBoost(0U); // 100%
  }
  void poll(msecu32_t const s) override {
    if (_APDS9960.lock()) {
      if (_APDS9960.proximityAvailable()) {
        ProximityData d(s);
        d.proximity = _APDS9960.readProximity();
        push(d);
      }
      _APDS9960.unlock();
    }
  }
  std::string name() override { return "Proximity"; }
};

#endif // proximity_h
