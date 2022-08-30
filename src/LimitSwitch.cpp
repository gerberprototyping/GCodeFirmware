#include "LimitSwitch.h"


#if DUAL_DRIVE_X
    #ifdef X_LIMIT_NEG_PIN
        #ifdef X_LIMIT_POS_PIN
            LimitSwitch xLimitSw1 = LimitSwitch(X_LIMIT_NEG_PIN, X_LIMIT_POS_PIN);
            LimitSwitch xLimitSw2 = LimitSwitch(X2_LIMIT_NEG_PIN, X2_LIMIT_POS_PIN);
        #else
            LimitSwitch xLimitSw1 = LimitSwitch(X_LIMIT_NEG_PIN, false);
            LimitSwitch xLimitSw2 = LimitSwitch(X2_LIMIT_NEG_PIN, false);
        #endif
    #else
        LimitSwitch xLimitSw1 = LimitSwitch(false, X_LIMIT_POS_PIN);
        LimitSwitch xLimitSw2 = LimitSwitch(false, X2_LIMIT_POS_PIN);
    #endif
#else
    #ifdef X_LIMIT_NEG_PIN
        #ifdef X_LIMIT_POS_PIN
            LimitSwitch xLimitSw = LimitSwitch(X_LIMIT_NEG_PIN, X_LIMIT_POS_PIN);
        #else
            LimitSwitch xLimitSw = LimitSwitch(X_LIMIT_NEG_PIN, false);
        #endif
    #else
        LimitSwitch xLimitSw = LimitSwitch(false, X_LIMIT_POS_PIN);
    #endif
#endif


#if DUAL_DRIVE_Y
    #ifdef Y_LIMIT_NEG_PIN
        #ifdef Y_LIMIT_POS_PIN
            LimitSwitch yLimitSw1 = LimitSwitch(Y_LIMIT_NEG_PIN, Y_LIMIT_POS_PIN);
            LimitSwitch yLimitSw2 = LimitSwitch(Y2_LIMIT_NEG_PIN, Y2_LIMIT_POS_PIN);
        #else
            LimitSwitch yLimitSw1 = LimitSwitch(Y_LIMIT_NEG_PIN, false);
            LimitSwitch yLimitSw2 = LimitSwitch(Y2_LIMIT_NEG_PIN, false);
        #endif
    #else
        LimitSwitch yLimitSw1 = LimitSwitch(false, Y_LIMIT_POS_PIN);
        LimitSwitch yLimitSw2 = LimitSwitch(false, Y2_LIMIT_POS_PIN);
    #endif
#else
    #ifdef Y_LIMIT_NEG_PIN
        #ifdef Y_LIMIT_POS_PIN
            LimitSwitch yLimitSw = LimitSwitch(Y_LIMIT_NEG_PIN, Y_LIMIT_POS_PIN);
        #else
            LimitSwitch yLimitSw = LimitSwitch(Y_LIMIT_NEG_PIN, false);
        #endif
    #else
        LimitSwitch yLimitSw = LimitSwitch(false, Y_LIMIT_POS_PIN);
    #endif
#endif


#if DUAL_DRIVE_Z
    #ifdef Z_LIMIT_NEG_PIN
        #ifdef Z_LIMIT_POS_PIN
            LimitSwitch zLimitSw1 = LimitSwitch(Z_LIMIT_NEG_PIN, Z_LIMIT_POS_PIN);
            LimitSwitch zLimitSw2 = LimitSwitch(Z2_LIMIT_NEG_PIN, Z2_LIMIT_POS_PIN);
        #else
            LimitSwitch zLimitSw1 = LimitSwitch(Z_LIMIT_NEG_PIN, false);
            LimitSwitch zLimitSw2 = LimitSwitch(Z2_LIMIT_NEG_PIN, false);
        #endif
    #else
        LimitSwitch zLimitSw1 = LimitSwitch(false, Z_LIMIT_POS_PIN);
        LimitSwitch zLimitSw2 = LimitSwitch(false, Z2_LIMIT_POS_PIN);
    #endif
#else
    #ifdef Z_LIMIT_NEG_PIN
        #ifdef Z_LIMIT_POS_PIN
            LimitSwitch zLimitSw = LimitSwitch(Z_LIMIT_NEG_PIN, Z_LIMIT_POS_PIN);
        #else
            LimitSwitch zLimitSw = LimitSwitch(Z_LIMIT_NEG_PIN, false);
        #endif
    #else
        LimitSwitch zLimitSw = LimitSwitch(false, Z_LIMIT_POS_PIN);
    #endif
#endif




LimitSwitch::LimitSwitch(gpio_t negPin, bool noPos)
    : has_neg(true), has_pos(false), neg_pin(negPin)
{}

LimitSwitch::LimitSwitch(bool noNeg, gpio_t posPin)
    : has_neg(false), has_pos(true), pos_pin(posPin)
{}

LimitSwitch::LimitSwitch(gpio_t negPin, gpio_t posPin)
    : has_neg(true), has_pos(true), neg_pin(negPin), pos_pin(posPin)
{}


void LimitSwitch::init() {
    if (has_neg) {
        enableGPIO(neg_pin);
        setMODER(neg_pin, INPUT);
        setPUPDR(neg_pin, PULLUP);
    }
    if (has_pos) {
        enableGPIO(pos_pin);
        setMODER(pos_pin, INPUT);
        setPUPDR(pos_pin, PULLUP);
    }
}


bool LimitSwitch::isActive() {
    #if LIMIT_NORMALLY_OPEN
        return (has_neg && !digitalRead(neg_pin)) || (has_pos && !digitalRead(pos_pin));
    #else
        return (has_neg && digitalRead(neg_pin)) || (has_pos && digitalRead(pos_pin));
    #endif
}

bool LimitSwitch::isActive(bool dir) {
    #if LIMIT_NORMALLY_OPEN
        if (POSITIVE == dir) {
            return has_pos && !digitalRead(pos_pin);
        } else {
            return has_neg && !digitalRead(neg_pin);
        }
    #else
        if (POSITIVE == dir) {
            return has_pos && digitalRead(pos_pin);
        } else {
            return has_neg && digitalRead(neg_pin);
        }
    #endif
}
    
bool LimitSwitch::isNegActive() {
    #if LIMIT_NORMALLY_OPEN
        return has_neg && !digitalRead(neg_pin);
    #else
        return has_neg && digitalRead(neg_pin);
    #endif
}

bool LimitSwitch::isPosActive() {
    #if LIMIT_NORMALLY_OPEN
        return has_pos && !digitalRead(pos_pin);
    #else
        return has_pos && digitalRead(pos_pin);
    #endif
}


void LimitSwitch::initAll() {

    #if DUAL_DRIVE_X
        xLimitSw1.init();
        xLimitSw2.init();
    #else
        xLimitSw.init();
    #endif

    #if DUAL_DRIVE_Y
        yLimitSw1.init();
        yLimitSw2.init();
    #else
        yLimitSw.init();
    #endif

    #if DUAL_DRIVE_Z
        zLimitSw1.init();
        zLimitSw2.init();
    #else
        zLimitSw.init();
    #endif

}
