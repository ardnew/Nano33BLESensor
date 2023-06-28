#ifndef HTS221_h
#define HTS221_h

#include <Arduino_HTS221.h>

#include "periph.h"

// Provides sensors: Temperature, Humidity
class PeriphHTS221: public Periph<HTS221Class> {
  using Periph<HTS221Class>::Periph;
};

extern PeriphHTS221 _HTS221;

#endif // HTS221_h
