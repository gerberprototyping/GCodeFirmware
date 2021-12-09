#include "StepDriver.h"


StepDriver xStepDriver;
StepDriver yStepDriver;
StepDriver zStepDriver;


StepDriver::StepDriver(Stepper stepper, LimitSwitch limitSw, bool homeDir)
    : stepper1(stepper), limitSw1(limitSw), homeDir(homeDir)
{
  dual = false;
}


StepDriver::StepDriver(Stepper stepper1, Stepper stepper2, LimitSwitch limitSw1, LimitSwitch limitSw2, bool homeDir)
    : stepper1(stepper1), stepper2(stepper2), limitSw1(limitSw1), limitSw2(limitSw2), homeDir(homeDir)
{
  dual = true;
}


void StepDriver::home() {
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
}


void StepDriver::step(bool dir) {
  stepper1.step(dir);
  if (dual) stepper2.step(dir);
}

void StepDriver::stepPos() {
  if (homeDir) {    // home is at negative end of axis
    stepper1.stepPos();
    if (dual) stepper2.stepPos();
  }
}

void StepDriver::stepNeg() {
  if (!homeDir) {   // home is at positive end of axis
    stepper1.stepNeg();
    if (dual) stepper2.stepNeg();
  }
}


bool StepDriver::checkLimit() {
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
}


void StepDriver::homeAll() {
  xStepDriver.home();
  yStepDriver.home();
  zStepDriver.home();
}
