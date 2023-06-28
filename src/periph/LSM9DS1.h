#ifndef LSM9DS1_h
#define LSM9DS1_h

#include <Arduino_LSM9DS1.h>

#include "periph.h"

// Provides sensors: Accelerometer, Gyroscope, Magnetometer
class PeriphLSM9DS1: public Periph<LSM9DS1Class> {
  using Periph<LSM9DS1Class>::Periph;
};

extern PeriphLSM9DS1 _LSM9DS1;

#endif // LSM9DS1_h
