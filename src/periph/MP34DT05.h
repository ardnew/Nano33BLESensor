#ifndef MP34DT05_h
#define MP34DT05_h

#include <PDM.h>

#include "periph.h"

// Provides sensors: Microphone
template <typename T = int16_t, size_t N = 256U>
class PeriphMP34DT05: public Periph<PDMClass> {
  using Periph<PDMClass>::Periph;
public:
 	static constexpr T * buf() { return _buf; }
  static constexpr size_t buflen() { return N; }
  static constexpr size_t bufsize() { return sizeof(_buf); }
  void receive() {
    // query the number of bytes available
    int bytesAvailable = available();
    if (bytesAvailable == bufsize()) {
      read(_buf, bufsize());
      unlock();
    }
  }
private:
  static T _buf[N];
};

template <typename T, size_t N>
T PeriphMP34DT05<T, N>::_buf[N];

extern PeriphMP34DT05<> _MP34DT05;

#endif // MP34DT05_h
