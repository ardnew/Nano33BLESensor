The current version of `arduino:mbed_nano` core (4.0.2) does not properly include headers for or link the Cortex-M4F DSP libraries provided with GCC's CMSIS 5 implementation. 

The initial error generated with this `Nano33BLESensor` library is:

> ```
> fatal error: arm_math.h: No such file or directory
> ```

Adding the relevant include path to your compile flags will then generate a link error:

> ```
> undefined reference to `arm_rms_q15'
> ```

The [`boards.local.txt`](boards.local.txt) included here is a template to resolve these issues. It also adds a few missing preprocessor definitions. Be sure to replace the prefix I use `/opt/arduino/lib` with your Arduino data directory.

Once updated, you then need to copy this file to the same directory as `boards.txt` that comes with the `arduino:mbed_nano` core. By default, I believe it should be:

> ```
> </arduino/data>/packages/arduino/hardware/mbed_nano/<version>
> ```
