#include "LPS22HB.h"

#ifdef ARDUINO_ARDUINO_NANO33BLE
PeriphLPS22HB _LPS22HB(Wire1);
#else
PeriphLPS22HB _LPS22HB(Wire);
#endif
