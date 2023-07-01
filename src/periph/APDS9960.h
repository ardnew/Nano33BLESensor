#ifndef APDS9960_h
#define APDS9960_h

#include <Arduino_APDS9960.h>

#include "periph.h"

// Provides sensors: Color, Gesture, Proximity
class PeriphAPDS9960: public Periph<APDS9960> {
  using Periph<APDS9960>::Periph;
};

extern PeriphAPDS9960 _APDS9960;

#endif // APDS9960_h
