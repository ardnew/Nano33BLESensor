#ifndef barometer_h
#define barometer_h

#include "periph/LPS22HB.h"
#include "sensor.h"

class BarometerData: public SensorData {
  using SensorData::SensorData;
public:
  float psi;
  using Type = decltype(psi);
};

class Barometer: public Sensor<Barometer, BarometerData> {
protected:
  void init() override {
    if (!_LPS22HB.begin()) { halt(); }
  }
  void poll(msecu32_t const s) override {
    if (_LPS22HB.lock()) {
      BarometerData d(s);
      d.psi = _LPS22HB.readPressure(PSI);
      push(d);
      _LPS22HB.unlock();
    }
  }
  std::string name() override { return "Barometer"; }
};

#endif // barometer_h
