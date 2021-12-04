#include "Nucleo-L476RG.h"
#include "Config.h"
#include "Stepper.h"
#include "LimitSwitch.h"
#include "StepDriver.h"


int main() {

    initClock40();

    
    LimitSwitch::initAll();
    Stepper::initAll();

    StepDriver y(yStepper1, yStepper2, yLimitSw1, yLimitSw2);
    y.home();

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