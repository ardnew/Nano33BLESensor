#ifndef LPS22HB_h
#define LPS22HB_h

#include <Arduino_LPS22HB.h>

#include "periph.h"

// Provides sensors: Barometer
class PeriphLPS22HB: public Periph<LPS22HBClass> {
  using Periph<LPS22HBClass>::Periph;
};

extern PeriphLPS22HB _LPS22HB;

#endif // LPS22HB_h
