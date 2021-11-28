#ifndef __LIMIT_SWITCH_H
#define __LIMIT_SWITCH_H

#include "GPIO.h"

#define X_LIMIT_PIN         ARDUINO_D9
#define Y_LIMIT_PIN         ARDUINO_D10
#define Z_LIMIT_PIN         ARDUINO_D11
#define X_NORMALLY_OPEN     false
#define Y_NORMALLY_OPEN     false
#define Z_NORMALLY_OPEN     false

class LimitSwitch {

  public:

    LimitSwitch() {}
    LimitSwitch(gpio_t pin, bool normally_open);
    bool isActive();

    static void initAll();

  private:

    gpio_t pin;
    bool normally_open;

};


extern LimitSwitch xLimitSw;
extern LimitSwitch yLimitSw;
extern LimitSwitch zLimitSw;


#endif