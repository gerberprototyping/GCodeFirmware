#ifndef __STEPPER_H
#define __STEPPER_H

#include "Config.h"
#include "GPIO.h"
#include "clock.h"


class Stepper {

    public:

        Stepper() {}
        Stepper(gpio_t step_pin, gpio_t dir_pin);
        void step(bool dir);
        void stepPos();
        void stepNeg();

        static void initAll();
        static void enableAll(bool enable = true);
        static void disableAll();

    private:

        gpio_t step_pin, dir_pin;

};


#if DUAL_DRIVE_X
    extern Stepper xStepper1;
    extern Stepper xStepper2;
#else
    extern Stepper xStepper;
#endif

#if DUAL_DRIVE_Y
    extern Stepper yStepper1;
    extern Stepper yStepper2;
#else
    extern Stepper yStepper;
#endif

#if DUAL_DRIVE_Z
    extern Stepper zStepper1;
    extern Stepper zStepper2;
#else
    extern Stepper zStepper;
#endif


#endif