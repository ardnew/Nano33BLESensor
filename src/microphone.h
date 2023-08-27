#ifndef microphone_h
#define microphone_h

#include <arm_math.h>

#include "periph/MP34DT05.h"
#include "sensor.h"

class MicrophoneData: public SensorData {
  using SensorData::SensorData;
public:
  int16_t rms;
  using GattType = decltype(rms);
};

class Microphone: public Sensor<Microphone, MicrophoneData> {
protected:
  static void receive() { _MP34DT05.receive(); }
  void init() override {
    _MP34DT05.onReceive(Microphone::receive);
    // 1 channel, 16 kHz sample rate
    if (!_MP34DT05.begin(1, 16000)) { halt(); }
    _MP34DT05.setGain(80);
  }
  void poll(msecu32_t const s) override {
    if (_MP34DT05.lock()) {
      MicrophoneData d(s);
      arm_rms_q15(_MP34DT05.buf(), _MP34DT05.buflen(), &d.rms);
      push(d);
    }
  }
  std::string name() override { return "Microphone"; }
};

#endif // microphone_h
