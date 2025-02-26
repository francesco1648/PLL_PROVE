#line 1 "C:\\Users\\Titania\\Desktop\\isaac\\picolowlevel_x_vs\\PicoLowLevel\\lib\\Battery\\src\\Battery.h"
#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>
#include "../../Debug/src/Debug.h"
#include "../../../include/definitions.h"

/**
 * Class to measure battery level and status.
 */
class Battery {
  public:
    Battery(byte pin = BAT_PIN, int r1 = BAT_R1, int r2 = BAT_R2) : pin(pin), r1(r1), r2(r2) {}
    float readVoltage();
    float chargePercent();
    bool charged();
    bool USB();

  private:
    byte pin;
    int r1, r2;
};

extern Battery battery;

#endif