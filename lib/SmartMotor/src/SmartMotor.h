#ifndef SMART_MOTOR_H
#define SMART_MOTOR_H

#include <Arduino.h>
#include "../../Debug/src/Debug.h"
#include "../../Motor/src/Motor.h"
#include "../../TractionEncoder/src/TractionEncoder.h"
#include "../../TractionEncoder/src/MovingAvgFilter.h"

#include "../../TractionEncoder/src/ExpSmoothingFilter.h"
#include "../../PID/src/PID.h"


/**
 * Class used to control DC motors at a constant speed.
 * Relies on a PID controller receiving data from a rotary encoder that uses a PIO state machine to reduce CPU usage and increase reliability.
 */
class SmartMotor {
  public:
    SmartMotor(byte pwm, byte dir, byte enc_a, byte enc_b, bool invert = false, PIO pio = pio0);
    void begin();
    void update();

    void setSpeed(float value);
    float getSpeed();
    void stop();

    void calibrate(float target = 45.f);

  private:
    int speedToPower(float speed);

    Motor motor;
    TractionEncoder encoder;
    PID pid;

    bool invert;
    float speed;
    unsigned long enc_last;
    unsigned long pid_last;
};

#endif
