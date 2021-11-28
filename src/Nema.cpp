#include "Nema.h"


#if DUAL_Y_MOTOR
    Nema yNema1;
    Nema yNema2;
#else
    Nema yNema;
#endif

Nema xNema;
Nema zNema;


Nema::Nema(gpio_t step_pin, gpio_t dir_pin): step_pin(step_pin), dir_pin(dir_pin) {
    enableGPIO(step_pin);
    enableGPIO(dir_pin);
    setMODER(step_pin, OUTPUT);
    setMODER(dir_pin, OUTPUT);
    digitalWrite(step_pin, LOW);
}


void Nema::step(bool dir) {
    digitalWrite(dir_pin, dir);
    digitalWrite(step_pin, HIGH);
    digitalWrite(step_pin, LOW);
}


void Nema::stepCW() {
    step(STEP_CW);
}

void Nema::stepCCW() {
    step(STEP_CCW);
}


void Nema::initAll() {
    enableGPIO(ENABLE_PIN);
    enableGPIO(COOLANT_EN_PIN);
    enableGPIO(RESET_PIN);
    enableGPIO(FEED_HOLD_PIN);
    enableGPIO(ABORT_PIN);
    setMODER(ENABLE_PIN, OUTPUT);
    setMODER(COOLANT_EN_PIN, OUTPUT);
    setMODER(RESET_PIN, OUTPUT);
    setMODER(FEED_HOLD_PIN, OUTPUT);
    setMODER(ABORT_PIN, OUTPUT);
    digitalWrite(ENABLE_PIN, LOW);
    digitalWrite(COOLANT_EN_PIN, LOW);
    digitalWrite(RESET_PIN, LOW);
    digitalWrite(FEED_HOLD_PIN, LOW);
    digitalWrite(ABORT_PIN, LOW);

    #if DUAL_Y_MOTOR
        yNema1 = Nema(Y_STEP_PIN, Y_DIR_PIN);
        yNema2 = Nema(A_STEP_PIN, A_DIR_PIN);
    #else
        yNema = Nema(Y_STEP_PIN, Y_DIR_PIN);
    #endif

    xNema = Nema(X_STEP_PIN, X_DIR_PIN);
    zNema = Nema(Z_STEP_PIN, Z_DIR_PIN);
}


void Nema::enable_all(bool enable) {
    digitalWrite(ENABLE_PIN, ~enable);
}

void Nema::disable_all() {
    digitalWrite(ENABLE_PIN, HIGH);
}