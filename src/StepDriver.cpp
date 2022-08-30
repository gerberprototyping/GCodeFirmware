#include "StepDriver.h"


#if DUAL_DRIVE_X
    StepDriver xStepDriver(xStepper1, xStepper2,
                            xLimitSw1, xLimitSw2,
                            X_MIN_MM*STEPS_PER_MM, X_MAX_MM*STEPS_PER_MM, X_HOME_POSITION);
#else
    StepDriver xStepDriver(xStepper, xLimitSw,
                            X_MIN_MM*STEPS_PER_MM, X_MAX_MM*STEPS_PER_MM, X_HOME_POSITION);
#endif

#if DUAL_DRIVE_Y
    StepDriver yStepDriver(yStepper1, yStepper2,
                            yLimitSw1, yLimitSw2,
                            Y_MIN_MM*STEPS_PER_MM, Y_MAX_MM*STEPS_PER_MM, Y_HOME_POSITION);
#else
    StepDriver yStepDriver(yStepper, yLimitSw,
                            Y_MIN_MM*STEPS_PER_MM, Y_MAX_MM*STEPS_PER_MM, Y_HOME_POSITION);
#endif

#if DUAL_DRIVE_Z
    StepDriver zStepDriver(zStepper1, zStepper2,
                            zLimitSw1, zLimitSw2,
                            Y_MIN_MM*STEPS_PER_MM, Z_MAX_MM*STEPS_PER_MM, Z_HOME_POSITION);
#else
    StepDriver zStepDriver(zStepper, zLimitSw,
                            Y_MIN_MM*STEPS_PER_MM, Z_MAX_MM*STEPS_PER_MM, Z_HOME_POSITION);
#endif


TimerInterrupt StepDriver::timerInterrupt;
volatile bool StepDriver::running = 0;;
volatile CartesianInt StepDriver::atomic_prevStepTime = CartesianInt();




StepDriver::StepDriver(Stepper stepper, LimitSwitch limitSw,
                        int64_t minStep, int64_t maxStep, bool homeDir)
    : dual(false),
      calibrated(0),
      stepper1(stepper),
      limitSw1(limitSw),
      minStep(minStep), maxStep(maxStep),
      homeDir(homeDir)
{
    // Intentionally left blank
}


StepDriver::StepDriver(Stepper stepper1, Stepper stepper2,
                        LimitSwitch limitSw1, LimitSwitch limitSw2,
                        int64_t minStep, int64_t maxStep, bool homeDir)
    : dual(true),
      calibrated(0),
      stepper1(stepper1), stepper2(stepper2),
      limitSw1(limitSw1), limitSw2(limitSw2),
      minStep(minStep), maxStep(maxStep),
      homeDir(homeDir)
{
    // Intentionally left blank
}


void StepDriver::home() {
    if (!running) {
        // Travel home fast
        while (!checkLimit(homeDir)) {
            step(homeDir);
            delay_microseconds(MAX_PACE);
        }
        delay(1);
        // Move to deactivate limit switch
        while (checkLimit(homeDir)) {
            step(!homeDir);
            delay_microseconds(MAX_PACE);
        }
        // Move away from home
        for (int i=0; i<BACKUP_STEPS; i++) {
            step(!homeDir);
            delay_microseconds(MAX_PACE);
        }
        // Approach home slowly
        while (!checkLimit(homeDir)) {
            step(homeDir);
            delay_microseconds(CALIB_PACE);
        }
        // Align dual axis
        if (dual) {
            if (!limitSw1.isActive(homeDir)) {
                // Bring primary axis into alignment with secondary axis
                while (!limitSw1.isActive(homeDir)) {
                    stepper1.step(homeDir);;
                    delay_microseconds(CALIB_PACE);
                }
            } else {
                // Bring secondary axis into alignment with primary axis
                while (!limitSw2.isActive(homeDir)) {
                    stepper2.step(homeDir);;
                    delay_microseconds(CALIB_PACE);
                }
            }
        }
        calibrated = 1;
        currStep = 0;
    }
}

int StepDriver::testStepping(double maxSpeed) {
    if (!running) {

        uint32_t pace = (uint32_t)(1000000/(maxSpeed*STEPS_PER_MM));
        int maxTolerance = 0;

        for (int mm=10; mm<=10; mm+=10) {

            home();
            delay_microseconds(2*MAX_PACE);
            int steps = mm*STEPS_PER_MM;

            for (int i=0; i<steps; i++) {
                step(!homeDir);
                delay_microseconds(pace);
            }
            delay_microseconds(2*MAX_PACE);

            while (!checkLimit(homeDir)) {
                step(homeDir);
                delay_microseconds(MAX_PACE);
                steps--;
            }
            if (std::abs(steps) > std::abs(maxTolerance)) {
                maxTolerance = steps;
            }

        }
        return maxTolerance;

    } else {
        return -999;
    }
}

bool StepDriver::testStepping(double maxSpeed, int tolerance) {
    int t = testStepping(maxSpeed);
    return std::abs(t) <= std::abs(tolerance);
}


void StepDriver::step(bool dir) {
    if (!checkLimit(dir)) {
        currStep += dir ? 1 : -1;
        stepper1.step(dir);
        if (dual) stepper2.step(dir);
    }
}

void StepDriver::stepPos() {
    if (!checkPosLimit()) {
        currStep++;
        stepper1.stepPos();
        if (dual) stepper2.stepPos();
    }
}

void StepDriver::stepNeg() {
    if (!checkNegLimit()) {
        currStep--;
        stepper1.stepNeg();
        if (dual) stepper2.stepNeg();
    }
}


bool StepDriver::checkLimit() {
    return limitSw1.isPosActive()
            || limitSw1.isNegActive()
            || (dual && limitSw2.isPosActive())
            || (dual && limitSw2.isNegActive())
            || (calibrated && currStep >= maxStep)
            || (calibrated && currStep <= minStep);
}

bool StepDriver::checkLimit(bool dir) {
    if (POSITIVE == dir) {
        return limitSw1.isPosActive()
                || (dual && limitSw2.isPosActive())
                || (calibrated && currStep >= maxStep);
    } else {
        return limitSw1.isNegActive()
                || (dual && limitSw2.isNegActive())
                || (calibrated && currStep <= minStep);
    }
}

bool StepDriver::checkNegLimit() {
    return limitSw1.isNegActive()
            || (dual && limitSw2.isNegActive())
            || (calibrated && currStep <= minStep);
}

bool StepDriver::checkPosLimit() {
    return limitSw1.isPosActive()
            || (dual && limitSw2.isPosActive())
            || (calibrated && currStep >= maxStep);
}




/***********************************************************
 * Static Methods
 **********************************************************/

void StepDriver::initAll() {
    LimitSwitch::initAll();
    Stepper::initAll();
    timerInterrupt.init(STEP_INTERRUPT_PERIOD, &StepDriver::interruptHandler);
}

void StepDriver::homeAll() {
    if (!running) {
        zStepDriver.home();
        xStepDriver.home();
        yStepDriver.home();
    }
}

bool StepDriver::testSteppingAll(double maxSpeed, int tolerance) {
    return xStepDriver.testStepping(maxSpeed, tolerance)
            && yStepDriver.testStepping(maxSpeed, tolerance)
            && zStepDriver.testStepping(maxSpeed, tolerance);
}


void StepDriver::start() {
    TimerInterrupt::start();
    running = 1;
}

void StepDriver::stop() {
    running = 0;
    TimerInterrupt::stop();
}

bool StepDriver::isRunning() {
    return running;
}

Point StepDriver::getCurrLocation() {
    return Point::fromSteps(
          xStepDriver.currStep,
          yStepDriver.currStep,
          zStepDriver.currStep
    );
}


void StepDriver::interruptHandler() {
    if (running) {
        MotionVector motionVec = MotionVector();
        if (motionVectorBuffer.peek(&motionVec)) {

            Point currLocation = getCurrLocation();
            if (currLocation >= motionVec) {
                motionVectorBuffer.remove(&motionVec);
            } else {

                int64_t usec = microseconds();

                CartesianInt prevStepTime = atomic_prevStepTime;                    // make local copy of atomic object
                CartesianInt deltaT = CartesianInt(usec) - prevStepTime;
                Velocity velocity = motionVec.getVelocity();
                CartesianInt pace = 60000000 / (velocity.abs() * STEPS_PER_MM);     // from mm/min to usec/step

                if (deltaT.getX() >= pace.getX()) {
                    bool xDir = (velocity.getX() >= 0);
                    xStepDriver.step(xDir);
                    prevStepTime.setX(usec);
                }
                if (deltaT.getY() >= pace.getY()) {
                    bool yDir = (velocity.getY() >= 0);
                    yStepDriver.step(yDir);
                    prevStepTime.setY(usec);
                }
                if (deltaT.getZ() >= pace.getZ()) {
                    bool zDir = (velocity.getZ() >= 0);
                    zStepDriver.step(zDir);
                    prevStepTime.setZ(usec);
                }

                atomic_prevStepTime = prevStepTime;                                 // write back to atomic object

            }

        }
    }
}
