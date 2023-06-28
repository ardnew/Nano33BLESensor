#ifndef periph_h
#define periph_h

#include <mbed.h>

// Periph provides a base class that the various peripheral IC singleton classes
// derive from.
//
// Accessing sensors implemented on a single peripheral IC must be coordinated
// to prevent conflicts observed in testing (e.g., accessing the LSM9DS1's IMU
// and Gyro simultaneously from separate RTOS threads). Hardware access among
// sensor threads on a shared peripheral IC is synchronized.
//
// However, accessing sensors implemented on separate peripherals seems to be
// implicitly coordinated by the I²C protocol. Hardware access among sensor
// threads on separate peripheral ICs is concurrent (i.e., NOT synchronized).
template <class C>
class Periph: public C {
  using C::C;
protected:
  static rtos::Mutex _mutex;
public:
  static inline bool lock() { return _mutex.trylock(); }
  static inline void unlock() { _mutex.unlock(); }
};

template <class C>
rtos::Mutex Periph<C>::_mutex;

#endif // periph_h
