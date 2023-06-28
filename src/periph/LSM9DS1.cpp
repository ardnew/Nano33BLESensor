#include "LSM9DS1.h"

#ifdef ARDUINO_ARDUINO_NANO33BLE
PeriphLSM9DS1 _LSM9DS1(Wire1);
#else
PeriphLSM9DS1 _LSM9DS1(Wire);
#endif
