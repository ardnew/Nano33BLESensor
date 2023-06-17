/*
  NanoBLESensor.h
  Copyright (c) 2023 ardnew. All rights reserved..

  This class implements a way to store and access the circular buffer
  that each sensor will have assigned to it.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*****************************************************************************/
/*INLCUDE GUARD                                                              */
/*****************************************************************************/
#ifndef NANO33BLESENSOR_H_
#define NANO33BLESENSOR_H_

/*****************************************************************************/
/*INLCUDES                                                                   */
/*****************************************************************************/
#include "Nano33BLESensorBuffer.h"
#include "Thread.h"

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
#define DEFAULT_SENSOR_THREAD_READ_PERIOD_MS   (0U)
#define DEFAULT_SENSOR_THREAD_STACK_SIZE_BYTES (1024U)

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*CLASS DECLARATION                                                          */
/*****************************************************************************/
/**
 * This class declares the init and read functions your sensor will use to
 * initialise the sensor and get the data.
 */
template<class T, class D,
  uint32_t R = DEFAULT_SENSOR_THREAD_READ_PERIOD_MS,
  osPriority_t P = osPriorityNormal,
  uint32_t S = DEFAULT_SENSOR_THREAD_STACK_SIZE_BYTES>
class Nano33BLESensor: public Nano33BLESensorBuffer<D>
{
  public:
   /**
     * @brief Initialises the sensor and starts the Mbed OS Thread.
     *
     */
    void begin()
    {
      init();
      readThread.start(mbed::callback([this](){ while (1) { read(); } }));
    }

    Nano33BLESensor(
      uint32_t readPeriod_ms = R,
      osPriority_t threadPriority = osPriorityNormal,
      uint32_t threadSize = S) :
        readPeriod(readPeriod_ms),
        readThread(
          threadPriority,
          threadSize){};

  protected:
    uint32_t readPeriod;

  private:
    /**
     * @brief Initialises the accelerometer sensor.
     *
     */
    virtual void init(void) = 0;
    /**
     * @brief Takes one reading from the accelerometer sensor if a reading
     * is available.
     *
     */
    virtual void read(void) = 0;

    rtos::Thread readThread;
};

#endif /* NANO33BLESENSOR_H_ */
