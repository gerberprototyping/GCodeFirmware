#include "StepDriver.h"


#if DUAL_DRIVE_X
  StepDriver xStepDriver = StepDriver(xStepper1, xStepper2, xLimitSw1, xLimitSw2, X_HOME_POS);
#else
  StepDriver xStepDriver = StepDriver(xStepper, xLimitSw, X_HOME_POS);
#endif

#if DUAL_DRIVE_Y
  StepDriver yStepDriver = StepDriver(yStepper1, yStepper2, yLimitSw1, yLimitSw2, Y_HOME_POS);
#else
  StepDriver yStepDriver = StepDriver(yStepper, yLimitSw, Y_HOME_POS);
#endif

#if DUAL_DRIVE_Z
  StepDriver zStepDriver = StepDriver(zStepper1, zStepper2, zLimitSw1, zLimitSw2, Z_HOME_POS);
#else
  StepDriver zStepDriver = StepDriver(zStepper, zLimitSw, Z_HOME_POS);
#endif

volatile bool StepDriver::running = false;

TimerInterrupt StepDriver::timerInterrupt = TimerInterrupt();




StepDriver::StepDriver(Stepper stepper, LimitSwitch limitSw, bool homeDir)
    : dual(false), homeDir(homeDir), calibrated(false), stepper1(stepper), limitSw1(limitSw)
{}


StepDriver::StepDriver(Stepper stepper1, Stepper stepper2, LimitSwitch limitSw1, LimitSwitch limitSw2, bool homeDir)
    : dual(true), homeDir(homeDir), calibrated(false), stepper1(stepper1), stepper2(stepper2), limitSw1(limitSw1), limitSw2(limitSw2)
{}


void StepDriver::home() {
  if (!running) {
    // Travel home fast
    while (!checkLimit()) {
      step(homeDir);;
      delay_microseconds(MAX_PACE);
    }
    delay(1);
    // Move to deactivate limit switch
    while (checkLimit()) {
      step(!homeDir);;
      delay_microseconds(MAX_PACE);
    }
    // Move away from home
    for (int i=0; i<BACKUP_STEPS; i++) {
      step(!homeDir);;
      delay_microseconds(MAX_PACE);
    }
    // Approach home slowly
    while (!checkLimit()) {
      step(homeDir);;
      delay_microseconds(CALIB_PACE);
    }
    // Align dual axis
    if (dual) {
      if (!limitSw1.isActive()) {
        // Bring primary axis into alignment with secondary axis
        while (!limitSw1.isActive()) {
          stepper1.step(homeDir);;
          delay_microseconds(CALIB_PACE);
        }
      } else {
        // Bring secondary axis into alignment with primary axis
        while (!limitSw2.isActive()) {
          stepper2.step(homeDir);;
          delay_microseconds(CALIB_PACE);
        }
      }
    }
    calibrated = true;
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

      while (!checkLimit()) {
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
  //TODO: add limit check
  //if (homeDir != dir || !checkLimit()) {
    currStep += dir ? 1 : -1;
    stepper1.step(dir);
    if (dual) stepper2.step(dir);
  //}
}

void StepDriver::stepPos() {
  //TODO: add limit check
  //if (homeDir == NEGATIVE || !checkLimit()) {
    currStep++;
    stepper1.stepPos();
    if (dual) stepper2.stepPos();
  //}
}

void StepDriver::stepNeg() {
  //TODO: add limit check
  //if (homeDir == POSITIVE || !checkLimit()) {
    currStep--;
    stepper1.stepNeg();
    if (dual) stepper2.stepNeg();
  //}
}


bool StepDriver::checkLimit() {
  //TODO: add virtual limits
  return limitSw1.isActive() || (dual && limitSw2.isActive());
}




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
  running = true;
  TimerInterrupt::start();
}


void StepDriver::stop() {
  running = false;
  TimerInterrupt::stop();
}


bool StepDriver::isRunning() {
  return running;
}


void StepDriver::interruptHandler() {
  static AtomicCartesianInt atomic_prevStepTime = AtomicCartesianInt();
  if (running) {
    MotionVector motionVec = MotionVector();
    if (motionVectorBuffer.peek(&motionVec)) {

      Point currLocation = getCurrLocation();
      if (currLocation >= motionVec) {
        motionVectorBuffer.remove(&motionVec);
      } else {

        int64_t usec = microseconds();

        CartesianInt prevStepTime = atomic_prevStepTime.load();           // make local copy of atomic object
        CartesianInt deltaT = CartesianInt(usec) - prevStepTime;
        Velocity velocity = motionVec.getVelocity();
        CartesianInt pace = 60000000 / (velocity.abs() * STEPS_PER_MM);   // from mm/min to usec/step

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

        atomic_prevStepTime.store(prevStepTime);                          // write back to atomic object

      }

    }
  }
}




Point StepDriver::getCurrLocation() {
  return Point::fromSteps(xStepDriver.currStep, yStepDriver.currStep, zStepDriver.currStep);
}
