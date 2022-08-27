#include <cstdio>
#include "Nucleo-L476RG.h"
#include "Config.h"
#include "Stepper.h"
#include "LimitSwitch.h"
#include "StepDriver.h"
#include "TimerInterrupt.h"
#include "uart.h"
#include "GCodeController.h"


int main() {

    char str[128];
    int t;

    initClock40();

    UART uart = UART(USART2, 9600);
    GCode::Controller controller;

    initLED();
    delay(500);
    setLED(false);

    uart.println();
    uart.println("Gerber Prototyping PCB Mill");
    uart.println("Initializing...\t");

    /******************************************
     * Some Debug Stuff
     *****************************************
    for (int i=0; i<3000; i++) {
        xStepDriver.step(false);
        delay(1);
    }
    while(true);
    /*****************************************/

    controller.run(uart, uart);
    while(true);



    /*GCode::Line line = GCode::Line();
    while (true) {
        scanner.getNext(line);
        scanner.print("Recieved line: '");
        uint32_t count = line.getCount();
        for (uint32_t i=0; i<count; i++) {
            if (line[i].letter == '\0') {
                sprintf(str, "[NULL]%f", line[i].number);
            } else {
                sprintf(str, "%c%f", line[i].letter, line[i].number);
            }
            scanner.print(str);
            if (i != count-1) {
                scanner.print(' ');
            }
        }
        scanner.println("'");
        line.makeEmpty();
    }*/
    
    //StepDriver::initAll();
    //StepDriver::homeAll();


    /*Point p0 = StepDriver::getCurrLocation();
    Point p1 = Point::fromMM(20,20,0);
    Point p2 = Point::fromMM(50,30,0);
    Point p3 = Point::fromMM(10,10,0);
    MotionVector vec1 = MotionVector(p0, p1, MAX_SPEED);
    MotionVector vec2 = MotionVector(p1, p2, MAX_SPEED);
    MotionVector vec3 = MotionVector(p2, p3, MAX_SPEED);

    motionVectorBuffer.add(vec1);
    motionVectorBuffer.add(vec2);
    motionVectorBuffer.add(vec3);
    StepDriver::start();
    while(!motionVectorBuffer.isEmpty());
    StepDriver::stop();
    setLED(true);*/


    /*uart.println();
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
    uart.println(str);*/


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
    
    Stepper::disableAll();
    while(1);

}
