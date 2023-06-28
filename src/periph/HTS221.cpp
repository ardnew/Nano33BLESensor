#include "HTS221.h"

#ifdef ARDUINO_ARDUINO_NANO33BLE
PeriphHTS221 _HTS221(Wire1);
#else
PeriphHTS221 _HTS221(Wire);
#endif
