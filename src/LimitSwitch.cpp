#include "LimitSwitch.h"


#if DUAL_DRIVE_X
    LimitSwitch xLimitSw1 = LimitSwitch(X_LIMIT_PIN, X_NORMALLY_OPEN);
    LimitSwitch xLimitSw2 = LimitSwitch(X2_LIMIT_PIN, X_NORMALLY_OPEN);
#else
    LimitSwitch xLimitSw = LimitSwitch(X_LIMIT_PIN, X_NORMALLY_OPEN);
#endif

#if DUAL_DRIVE_Y
    LimitSwitch yLimitSw1 = LimitSwitch(Y_LIMIT_PIN, Y_NORMALLY_OPEN);
    LimitSwitch yLimitSw2 = LimitSwitch(Y2_LIMIT_PIN, Y_NORMALLY_OPEN);
#else
    LimitSwitch yLimitSw = LimitSwitch(Y_LIMIT_PIN, Y_NORMALLY_OPEN);
#endif

#if DUAL_DRIVE_Z
    LimitSwitch zLimitSw1 = LimitSwitch(Z_LIMIT_PIN, Z_NORMALLY_OPEN);
    LimitSwitch zLimitSw2 = LimitSwitch(Z2_LIMIT_PIN, Z_NORMALLY_OPEN);
#else
    LimitSwitch zLimitSw = LimitSwitch(Z_LIMIT_PIN, Z_NORMALLY_OPEN);
#endif




LimitSwitch::LimitSwitch(gpio_t pin, bool normally_open)
    : pin(pin), normally_open(normally_open)
{}


void LimitSwitch::init() {
  enableGPIO(pin);
  setMODER(pin, INPUT);
  setPUPDR(pin, PULLUP);
}


bool LimitSwitch::isActive() {
  bool val = digitalRead(pin);
  return normally_open ? !val : val;
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
