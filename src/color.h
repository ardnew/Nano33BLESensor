#ifndef color_h
#define color_h

#include "periph/APDS9960.h"
#include "sensor.h"

class ColorData: public SensorData {
  using SensorData::SensorData;
public:
  int r;
  int g;
  int b;
  int c;
  using GattType = decltype(r);
};

class Color: public Sensor<Color, ColorData> {
protected:
  void init() override {
    if (!_APDS9960.begin()) { halt(); }
    _APDS9960.setLEDBoost(0U); // 100%
  }
  void poll(msecu32_t const s) override {
    if (_APDS9960.lock()) {
      if (_APDS9960.colorAvailable()) {
        ColorData d(s);
        _APDS9960.readColor(d.r, d.g, d.b, d.c);
        push(d);
      }
      _APDS9960.unlock();
    }
  }
  std::string name() override { return "Color"; }
};

#endif // color_h
