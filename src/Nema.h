#ifndef __NEMA_H
#define __NEMA_H

#include "GPIO.h"
#include "clock.h"

#define ENABLE_PIN          ARDUINO_D8          // Active Low
#define COOLANT_EN_PIN      ARDUINO_A3
#define RESET_PIN           ARDUINO_A2          // Active Low
#define FEED_HOLD_PIN       ARDUINO_A1          // Active Low?
#define ABORT_PIN           ARDUINO_A0
#define X_DIR_PIN           ARDUINO_D5
#define Y_DIR_PIN           ARDUINO_D6
#define Z_DIR_PIN           ARDUINO_D7
#define A_DIR_PIN           ARDUINO_D13
#define X_STEP_PIN          ARDUINO_D2
#define Y_STEP_PIN          ARDUINO_D3
#define Z_STEP_PIN          ARDUINO_D4
#define A_STEP_PIN          ARDUINO_D12

#define STEP_CW             true
#define STEP_CCW            false

#define DUAL_Y_MOTOR        true


class Nema {

    public:

        Nema() {}
        Nema(gpio_t step_pin, gpio_t dir_pin);
        void step(bool dir);
        void stepCW();
        void stepCCW();

        static void initAll();

    private:

        gpio_t step_pin, dir_pin;

        static void enable_all(bool enable = true);
        static void disable_all();

};

#if DUAL_Y_MOTOR
    extern Nema yNema1;
    extern Nema yNema2;
#else
    extern Nema yNema;
#endif

extern Nema xNema;
extern Nema zNema;


#endif