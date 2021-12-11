#include <cstdio>
#include "Nucleo-L476RG.h"
#include "Config.h"
#include "Stepper.h"
#include "LimitSwitch.h"
#include "StepDriver.h"
#include "TimerInterrupt.h"
#include "uart.h"


void prefillMotionPlan(const StepInstruction &inst) {
    for (int i=0; i<100; i++) {
        stepInstructionBuffer.add(inst);
    }
}


int main() {

    char str[128];
    int t;

    initClock40();

    UART uart(USART2, 9600);

    initLED();
    delay(500);
    setLED(false);
    
    StepDriver::initAll();
    StepDriver::homeAll();


    uart.println();
    uart.println("Running extended stepping test...");
    for (int i=0; i<50*STEPS_PER_MM; i++) {
        xStepDriver.stepPos();
        yStepDriver.stepPos();
        delay_microseconds(MAX_PACE);
    }
    delay(500);
    for (int i=0; i<50; i++) {
        for (int i=0; i<10*STEPS_PER_MM; i++) {
            xStepDriver.stepNeg();
            yStepDriver.stepNeg();
            delay_microseconds(MAX_PACE);
        }
        delay(500);
        for (int i=0; i<10*STEPS_PER_MM; i++) {
            xStepDriver.stepPos();
            yStepDriver.stepPos();
            delay_microseconds(MAX_PACE);
        }
        delay(500);
    }
    int xSteps = 50*STEPS_PER_MM;
    int ySteps = 50*STEPS_PER_MM;
    while (!xStepDriver.checkLimit()) {
        xStepDriver.stepNeg();
        delay_microseconds(MAX_PACE);
        xSteps--;
    }
    sprintf(str, "  x:%d", xSteps);
    uart.println(str);
    while (!yStepDriver.checkLimit()) {
        yStepDriver.stepNeg();
        delay_microseconds(MAX_PACE);
        ySteps--;
    }
    sprintf(str, "  y:%d", ySteps);
    uart.println(str);


    /*uart.println();
    uart.println("Running stepping test...");
    char str[128];
    int t = xStepDriver.testStepping(MAX_SPEED);
    sprintf(str, "  x: %d", t);
    uart.println(str);
    t = yStepDriver.testStepping(MAX_SPEED);
    sprintf(str, "  y: %d", t);
    uart.println(str);
    t = zStepDriver.testStepping(MAX_SPEED);
    sprintf(str, "  z: %d", t);
    uart.println(str);*/


    /*StepInstruction inst(true, true, false, true, true, false);
    prefillMotionPlan(inst);
    StepDriver::start();


    for (int i=0; i<49900; i++) {
        while(!stepInstructionBuffer.add(inst));
    }

    while(!stepInstructionBuffer.isEmpty());
    StepDriver::stop();*/
    
    Stepper::disableAll();
    while(1);

}
