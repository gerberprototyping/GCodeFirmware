#include "LimitSwitch.h"


LimitSwitch xLimitSw;
LimitSwitch yLimitSw;
LimitSwitch zLimitSw;


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
  xLimitSw = LimitSwitch(X_LIMIT_PIN, X_NORMALLY_OPEN);
  yLimitSw = LimitSwitch(Y_LIMIT_PIN, Y_NORMALLY_OPEN);
  zLimitSw = LimitSwitch(Z_LIMIT_PIN, Z_NORMALLY_OPEN);
}
