#ifndef __STEP_DRIVER_H
#define __STEP_DRIVER_H

#include "Config.h"
#include "Stepper.h"
#include "LimitSwitch.h"

#define MAX_PACE        500
#define CALIB_PACE      1000
#define BACKUP_STEPS    (10*STEPS_PER_MM)


class StepDriver {

  public:

    StepDriver() {}
    StepDriver(Stepper stepper, LimitSwitch limitSw, bool homeDir);
    StepDriver(Stepper stepper1, Stepper stepper2, LimitSwitch limitSw1, LimitSwitch limitSw2, bool homeDir);

    void home();

    /*static void start(MotionPlan motionPlan);
    static void pause();
    static void resume();
    static void stop();*/
    
    //static void interuptHandler();

    static void initAll();
    static void homeAll();

  private:

    bool dual;
    bool homeDir;

    Stepper stepper1;
    Stepper stepper2;
    LimitSwitch limitSw1;
    LimitSwitch limitSw2;

    void step(bool dir);
    void stepPos();
    void stepNeg();

    bool checkLimit();

    //static MotionPlan::Axis motionPlan;
    //static bool running;

};


extern StepDriver xStepDriver;
extern StepDriver yStepDriver;
extern StepDriver zStepDriver;


#endif