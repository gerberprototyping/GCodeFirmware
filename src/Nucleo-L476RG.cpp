#include "Nucleo-L476RG.h"


void init() {

  initClock();

}


void initLED() {
  enableGPIO(ARDUINO_D13);
  setMODER(ARDUINO_D13, OUTPUT);
}


void setLED(bool state) {
  digitalWrite(ARDUINO_D13, state);
}
