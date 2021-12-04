#include "LimitSwitch.h"


#if DUAL_DRIVE_X
    LimitSwitch xLimitSw1;
    LimitSwitch xLimitSw2;
#else
    LimitSwitch xLimitSw;
#endif

#if DUAL_DRIVE_Y
    LimitSwitch yLimitSw1;
    LimitSwitch yLimitSw2;
#else
    LimitSwitch yLimitSw;
#endif

#if DUAL_DRIVE_Z
    LimitSwitch zLimitSw1;
    LimitSwitch zLimitSw2;
#else
    LimitSwitch zLimitSw;
#endif


LimitSwitch::LimitSwitch(gpio_t pin, bool normally_open): pin(pin), normally_open(normally_open) {
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
      xLimitSw1 = LimitSwitch(X_LIMIT_PIN, X_NORMALLY_OPEN);
      xLimitSw2 = LimitSwitch(X2_LIMIT_PIN, X_NORMALLY_OPEN);
  #else
      xLimitSw = LimitSwitch(X_LIMIT_PIN, X_NORMALLY_OPEN);
  #endif

  #if DUAL_DRIVE_Y
      yLimitSw1 = LimitSwitch(Y_LIMIT_PIN, Y_NORMALLY_OPEN);
      yLimitSw2 = LimitSwitch(Y2_LIMIT_PIN, Y_NORMALLY_OPEN);
  #else
      yLimitSw = LimitSwitch(Y_LIMIT_PIN, Y_NORMALLY_OPEN);
  #endif

  #if DUAL_DRIVE_Z
      zLimitSw1 = LimitSwitch(Z_LIMIT_PIN, Z_NORMALLY_OPEN);
      zLimitSw2 = LimitSwitch(Z2_LIMIT_PIN, Z_NORMALLY_OPEN);
  #else
      zLimitSw = LimitSwitch(Z_LIMIT_PIN, Z_NORMALLY_OPEN);
  #endif

}
