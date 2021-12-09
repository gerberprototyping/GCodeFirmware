#include "Nucleo-L476RG.h"
#include "Config.h"
#include "Stepper.h"
#include "LimitSwitch.h"
#include "StepDriver.h"
#include "TimerInterrupt.h"


void myFunc() {
    static bool state = false;
    digitalWrite(ARDUINO_D13, state);
}


int main() {

    initClock40();

    /*enableGPIO(ARDUINO_D13);
    setMODER(ARDUINO_D13, OUTPUT);
    TimerInterrupt timInt(40000000, &myFunc);*/

    
    StepDriver::initAll();

    StepDriver::homeAll();

    /*int max = 2000;
    int tolerance = 1;
    for (int i=0; i<max; i++) {
        yStepper1.stepPos();
        delay_microseconds(500);
    }
    for (int i=0; i<max-tolerance; i++) {
        yStepper1.stepNeg();
        delay(1);
        if (yLimitSw1.isActive()) {
            break;
        }
    }
    for (int i=0; i<2*tolerance; i++) {
        if (yLimitSw1.isActive()) {
            break;
        }
        yStepper1.stepNeg();
        delay(1);
    }
    if (yLimitSw1.isActive()) {
        Stepper::disableAll();
    }*/

    Stepper::disableAll();
    while(1);

}
