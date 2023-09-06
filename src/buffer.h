#ifndef buffer_h
#define buffer_h

#include <CircularBuffer.h>
#include <cstdint>

template<class T, uint32_t N = 100U>
class Buffer {
public:
  uint32_t size(void) { return _buf.size(); }
  void push(T& b) { _buf.push(b); }
  bool pop(T& b) { return _buf.pop(b); }
private:
  mbed::CircularBuffer<T, N> _buf;
};

#endif // buffer_h
