#ifndef __LIMIT_SWITCH_H
#define __LIMIT_SWITCH_H

#include "Config.h"
#include "GPIO.h"


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