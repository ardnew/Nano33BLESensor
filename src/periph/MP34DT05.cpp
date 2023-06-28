#include "MP34DT05.h"

#ifdef ARDUINO_ARDUINO_NANO33BLE
PeriphMP34DT05<> _MP34DT05(PIN_PDM_DIN, PIN_PDM_CLK, PIN_PDM_PWR);
#else
#error unsupported target! (undefined symbol: ARDUINO_ARDUINO_NANO33BLE)
#endif
