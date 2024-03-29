#ifndef __STEP_DRIVER_H
#define __STEP_DRIVER_H

#include <cmath>
#include <algorithm>
#include <atomic>

#include "Config.h"
#include "Stepper.h"
#include "LimitSwitch.h"
#include "TimerInterrupt.h"
#include "MotionVector.h"
#include "Cartesian.h"

#define CALIB_PACE      1000
#define BACKUP_STEPS    (1*STEPS_PER_MM)


class StepDriver {

    public:

        StepDriver() {}
        StepDriver(Stepper stepper, LimitSwitch limitSw, int64_t minStep, int64_t maxStep, bool homeDir);
        StepDriver(Stepper stepper1, Stepper stepper2, LimitSwitch limitSw1, LimitSwitch limitSw2, int64_t minStep, int64_t maxStep, bool homeDir);

        void home(bool homeMin = true);
        bool isAxisCalibrated();
        int testStepping(double maxSpeed);
        bool testStepping(double maxSpeed, int tolerance);

        void step(bool dir);
        void stepPos();
        void stepNeg();

        bool checkLimit();
        bool checkLimit(bool dir);
        bool checkNegLimit();
        bool checkPosLimit();

        static void initAll();
        static void homeAll();
        static bool testSteppingAll(double maxSpeed, int tolerance);

        static void start();
        static void stop();
        static bool isRunning();
        static bool isCalibrated();
        static Point getCurrLocation();

        static void interruptHandler();

    private:

        bool dual;
        bool homeDir;
        int64_t minStep;
        int64_t maxStep;

        volatile bool calibrated;
        volatile int64_t currStep;

        Stepper stepper1;
        Stepper stepper2;
        LimitSwitch limitSw1;
        LimitSwitch limitSw2;


        static TimerInterrupt timerInterrupt;
        static volatile bool running;
        static volatile CartesianInt atomic_prevStepTime;

};


extern StepDriver xStepDriver;
extern StepDriver yStepDriver;
extern StepDriver zStepDriver;


#endif