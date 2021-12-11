#ifndef __STEP_DRIVER_H
#define __STEP_DRIVER_H

#include <cmath>
#include "Config.h"
#include "Stepper.h"
#include "LimitSwitch.h"
#include "TimerInterrupt.h"
#include "StepVector.h"

#define CALIB_PACE      1000
#define BACKUP_STEPS    (1*STEPS_PER_MM)


class StepDriver {

  public:

    StepDriver() {}
    StepDriver(Stepper stepper, LimitSwitch limitSw, bool homeDir);
    StepDriver(Stepper stepper1, Stepper stepper2, LimitSwitch limitSw1, LimitSwitch limitSw2, bool homeDir);

    void home();
    int testStepping(double maxSpeed);
    bool testStepping(double maxSpeed, int tolerance);

    void step(bool dir);
    void stepPos();
    void stepNeg();
    bool checkLimit();

    static void initAll();
    static void homeAll();
    static bool testSteppingAll(double maxSpeed, int tolerance);

    static void start();
    static void stop();
    static bool isRunning();

    static void interruptHandler();

  private:

    bool dual;
    bool homeDir;

    bool calibrated;
    volatile int32_t currStep;

    Stepper stepper1;
    Stepper stepper2;
    LimitSwitch limitSw1;
    LimitSwitch limitSw2;

    static bool running;
    static TimerInterrupt timerInterrupt;

};


extern StepDriver xStepDriver;
extern StepDriver yStepDriver;
extern StepDriver zStepDriver;


#endif