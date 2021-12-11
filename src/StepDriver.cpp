#include "StepDriver.h"


StepDriver xStepDriver;
StepDriver yStepDriver;
StepDriver zStepDriver;

bool StepDriver::running = false;




StepDriver::StepDriver(Stepper stepper, LimitSwitch limitSw, bool homeDir)
    : stepper1(stepper), limitSw1(limitSw), homeDir(homeDir)
{
  dual = false;
  calibrated = false;
}


StepDriver::StepDriver(Stepper stepper1, Stepper stepper2, LimitSwitch limitSw1, LimitSwitch limitSw2, bool homeDir)
    : stepper1(stepper1), stepper2(stepper2), limitSw1(limitSw1), limitSw2(limitSw2), homeDir(homeDir)
{
  dual = true;
  calibrated = false;
}


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
  currStep += dir ? 1 : -1;
  stepper1.step(dir);
  if (dual) stepper2.step(dir);
}

void StepDriver::stepPos() {
  //TODO: add limit check
  currStep++;
  stepper1.stepPos();
  if (dual) stepper2.stepPos();
}

void StepDriver::stepNeg() {
  //TODO: add limit check
  currStep--;
  stepper1.stepNeg();
  if (dual) stepper2.stepNeg();
}


bool StepDriver::checkLimit() {
  //TODO: add virtual limits
  return limitSw1.isActive() || (dual && limitSw2.isActive());
}




void StepDriver::initAll() {
  LimitSwitch::initAll();
  Stepper::initAll();
  #if DUAL_DRIVE_X
    xStepDriver = StepDriver(xStepper1, xStepper2, xLimitSw1, xLimitSw2, X_HOME_POS);
  #else
    xStepDriver = StepDriver(xStepper, xLimitSw, X_HOME_POS);
  #endif

  #if DUAL_DRIVE_Y
    yStepDriver = StepDriver(yStepper1, yStepper2, yLimitSw1, yLimitSw2, Y_HOME_POS);
  #else
    yStepDriver = StepDriver(yStepper, yLimitSw, Y_HOME_POS);
  #endif

  #if DUAL_DRIVE_Z
    zStepDriver = StepDriver(zStepper1, zStepper2, zLimitSw1, zLimitSw2, Z_HOME_POS);
  #else
    zStepDriver = StepDriver(zStepper, zLimitSw, Z_HOME_POS);
  #endif
  timerInterrupt = TimerInterrupt(STEP_INTERRUPT_PERIOD, &StepDriver::interruptHandler);
}


void StepDriver::homeAll() {
  if (!running) {
    xStepDriver.home();
    yStepDriver.home();
    zStepDriver.home();
  }
}

bool StepDriver::testSteppingAll(double maxSpeed, int tolerance) {
  return xStepDriver.testStepping(maxSpeed, tolerance)
      && yStepDriver.testStepping(maxSpeed, tolerance)
      && zStepDriver.testStepping(maxSpeed, tolerance);
}


void StepDriver::start() {
  running = true;
  //TODO: stop timer interrupt
}


void StepDriver::stop() {
  running = false;
  //TODO: start timer interrupt
}


bool StepDriver::isRunning() {
  return running;
}


void StepDriver::interruptHandler() {
  if (running) {
    StepVector vec;
    bool empty = motionVectorBuffer.remove(vec);
    if (!empty) {

      double t = millis() - vec.startTime;
      Cartesian<double> deltaMM = (t/1000) * vec.velocity;
      Point waypoint = vec.start + Point::fromMM(deltaMM);

    }
  }
}
