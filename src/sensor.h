#ifndef sensor_h
#define sensor_h

#include <mbed.h>
#include <chrono>

#include <stdint/ticks.h>

#include "buffer.h"

class SensorData {
public:
  SensorData(msecu32_t const s = msecu32()): _whence(s) {}
  uint32_t time() const { return _whence.count(); }
protected:
  msecu32_t _whence;
};

template<class T, class D,
  uint32_t R = 200U, osPriority_t P = osPriorityNormal, uint32_t S = 1024U>
class Sensor: public Buffer<D> {
public:
  Sensor(uint32_t period = R, osPriority_t priority = P, uint32_t bytesz = S):
    _period(msecu32_t{period}), _thread(priority, bytesz) {}
  static inline void halt() { osSignalWait(0x0001, osWaitForever); }
  void start() {
    init();
    _thread.start(mbed::callback([&] () {
      while (1) {
        msecu32_t start = msecu32();
        poll(start);
        idle(start); // subtract poll duration from remaining idle time
      }
    }));
  }

private:
  virtual void init() = 0;
  virtual void poll(msecu32_t const) = 0;
  virtual std::string name() = 0;

  bool _trace;
  msecu32_t _period;
  rtos::Thread _thread;
  inline void idle(msecu32_t const start) {
    msecu32_t delta = msecu32() - start;
    msecu32_t delay = _period; // always restore shortest period
    // increment delay if delta > delay
    delay += delay * (delta / delay);
    msecu32_t sleep = delay - delta;
    rtos::ThisThread::sleep_for(sleep);
  }
};

#endif // sensor_h
