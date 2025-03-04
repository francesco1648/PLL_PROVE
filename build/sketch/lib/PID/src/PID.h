#line 1 "C:\\Users\\Titania\\Desktop\\isaac\\picolowlevel_x_vs\\PicoLowLevel\\lib\\PID\\src\\PID.h"
#ifndef PID_H
#define PID_H

#include <Arduino.h>

/**
 * PID controller library.
 */
class PID {
  public:
    PID(float kp, float ki, float kd, float max_output, float alpha);
    void updateReferenceValue(float ref);
    void setKp(float kp);
    void setKi(float ki);
    void setKd(float kd);
    float getOutput();
    float getReferenceValue();
    void calculate();
    void updateFeedback(float fb);
    void resetState();

  private:
    float kp, ki, kd; // gains
    float max_output, alpha; // constants
    float referenceValue, output, feedback; // variables
    float old_fe, old_integral, old_error; // state
    unsigned long tempo;
};


#endif
