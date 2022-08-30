#ifndef __LIMIT_SWITCH_H
#define __LIMIT_SWITCH_H

#include "Config.h"
#include "GPIO.h"


class LimitSwitch {

  public:

    LimitSwitch() {}
    LimitSwitch(gpio_t negPin, bool noPos);
    LimitSwitch(bool noNeg, gpio_t posPin);
    LimitSwitch(gpio_t negPin, gpio_t posPin);

    bool isActive();
    bool isActive(bool dir);
    bool isNegActive();
    bool isPosActive();

    static void initAll();

  private:

    bool has_neg;
    bool has_pos;
    gpio_t neg_pin;
    gpio_t pos_pin;

    void init();

};


#if DUAL_DRIVE_X
    extern LimitSwitch xLimitSw1;
    extern LimitSwitch xLimitSw2;
#else
    extern LimitSwitch xLimitSw;
#endif

#if DUAL_DRIVE_Y
    extern LimitSwitch yLimitSw1;
    extern LimitSwitch yLimitSw2;
#else
    extern LimitSwitch yLimitSw;
#endif

#if DUAL_DRIVE_Z
    extern LimitSwitch zLimitSw1;
    extern LimitSwitch zLimitSw2;
#else
    extern LimitSwitch zLimitSw;
#endif


#endif