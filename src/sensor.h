#ifndef sensor_h
#define sensor_h

#include <mbed.h>
#include <chrono>

#include "buffer.h"

typedef std::chrono::duration<uint32_t, std::milli> msec_t; // milliseconds
inline msec_t msec() { return static_cast<msec_t>(millis()); }

class SensorData {
public:
  SensorData(msec_t const s = msec()): _whence(s) {}
protected:
  msec_t _whence;
};

template<class T, class D,
  uint32_t R = 25U, osPriority_t P = osPriorityNormal, uint32_t S = 1024U>
class Sensor: public Buffer<D> {
public:
  Sensor(uint32_t period = R, osPriority_t priority = P, uint32_t bytesz = S):
    _period(static_cast<msec_t>(period)), _thread(priority, bytesz) {}
  static inline void halt() { osSignalWait(0x0001, osWaitForever); }
  void start() {
    init();
    _thread.start(mbed::callback([this] () {
      while (1) {
        msec_t start = msec();
        poll(start);
        idle(start); // subtract poll duration from remaining idle time
      }
    }));
  }

private:
  virtual void init() = 0;
  virtual void poll(msec_t const) = 0;
  virtual std::string name() = 0;

  bool _trace;
  msec_t _period;
  rtos::Thread _thread;
  inline void idle(msec_t const start) {
    msec_t delta = msec() - start;
    msec_t delay = _period; // always restore shortest period
    // increment delay if delta > delay
    delay += delay * (delta / delay);
    msec_t sleep = delay - delta;
    rtos::ThisThread::sleep_for(sleep);
  }
};

#endif // sensor_h
