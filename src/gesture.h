#ifndef gesture_h
#define gesture_h

#include "periph/APDS9960.h"
#include "sensor.h"

class GestureData: public SensorData {
  using SensorData::SensorData;
public:
  int gesture;
  using GattType = decltype(gesture);
};

class Gesture: public Sensor<Gesture, GestureData> {
protected:
  void init() override {
    _APDS9960.setGestureSensitivity(50U);
    if (!_APDS9960.begin()) { halt(); }
  }
  void poll(msecu32_t const s) override {
    if (_APDS9960.lock()) {
      if (_APDS9960.gestureAvailable()) {
        GestureData d(s);
        d.gesture = _APDS9960.readGesture();
        push(d);
      }
      _APDS9960.unlock();
    }
  }
  std::string name() override { return "Gesture"; }
};

#endif // gesture_h
