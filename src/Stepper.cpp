#include "Stepper.h"


#if DUAL_DRIVE_X
    Stepper xStepper1 = Stepper(X_STEP_PIN, X_DIR_PIN);
    Stepper xStepper2 = Stepper(X2_STEP_PIN, X2_DIR_PIN);
#else
    Stepper xStepper = Stepper(X_STEP_PIN, X_DIR_PIN);
#endif

#if DUAL_DRIVE_Y
    Stepper yStepper1 = Stepper(Y_STEP_PIN, Y_DIR_PIN);
    Stepper yStepper2 = Stepper(Y2_STEP_PIN, Y2_DIR_PIN);
#else
    Stepper yStepper = Stepper(Y_STEP_PIN, Y_DIR_PIN);
#endif

#if DUAL_DRIVE_Z
    Stepper zStepper1 = Stepper(Z_STEP_PIN, Z_DIR_PIN);
    Stepper zStepper2 = Stepper(Z2_STEP_PIN, Z2_DIR_PIN);
#else
    Stepper zStepper = Stepper(Z_STEP_PIN, Z_DIR_PIN);
#endif


Stepper::Stepper(gpio_t step_pin, gpio_t dir_pin)
    : step_pin(step_pin), dir_pin(dir_pin)
{}


void Stepper::init(){
    enableGPIO(step_pin);
    enableGPIO(dir_pin);
    setMODER(step_pin, OUTPUT);
    setMODER(dir_pin, OUTPUT);
    digitalWrite(step_pin, LOW);
}


void Stepper::step(bool dir) {
    digitalWrite(dir_pin, dir);
    digitalWrite(step_pin, HIGH);
    digitalWrite(step_pin, LOW);
}


void Stepper::stepPos() {
    digitalWrite(dir_pin, HIGH);
    digitalWrite(step_pin, HIGH);
    digitalWrite(step_pin, LOW);
}

void Stepper::stepNeg() {
    digitalWrite(dir_pin, LOW);
    digitalWrite(step_pin, HIGH);
    digitalWrite(step_pin, LOW);
}


void Stepper::initAll() {
    enableGPIO(ENABLE_PIN);
    //enableGPIO(COOLANT_EN_PIN);
    //enableGPIO(RESET_PIN);
    //enableGPIO(FEED_HOLD_PIN);
    //enableGPIO(ABORT_PIN);
    setMODER(ENABLE_PIN, OUTPUT);
    //setMODER(COOLANT_EN_PIN, OUTPUT);
    //setMODER(RESET_PIN, OUTPUT);
    //setMODER(FEED_HOLD_PIN, OUTPUT);
    //setMODER(ABORT_PIN, OUTPUT);
    digitalWrite(ENABLE_PIN, LOW);
    //digitalWrite(COOLANT_EN_PIN, LOW);
    //digitalWrite(RESET_PIN, LOW);
    //digitalWrite(FEED_HOLD_PIN, LOW);
    //digitalWrite(ABORT_PIN, LOW);

    #if DUAL_DRIVE_X
        xStepper1.init();
        xStepper2.init();
    #else
        xStepper.init();
    #endif

    #if DUAL_DRIVE_Y
        yStepper1.init();
        yStepper2.init();
    #else
        yStepper.init();
    #endif

    #if DUAL_DRIVE_Z
        zStepper1.init();
        zStepper2.init();
    #else
        zStepper.init();
    #endif
}


void Stepper::enableAll(bool enable) {
    digitalWrite(ENABLE_PIN, !enable);
}

void Stepper::disableAll() {
    digitalWrite(ENABLE_PIN, HIGH);
}