#include "StepDriver.h"


//bool StepDriver::running = false;


StepDriver::StepDriver(Stepper stepper, LimitSwitch limitSw)
    : stepper1(stepper), limitSw1(limitSw)
{
  dual = false;
}


StepDriver::StepDriver(Stepper stepper1, Stepper stepper2, LimitSwitch limitSw1, LimitSwitch limitSw2)
    : stepper1(stepper1), stepper2(stepper2), limitSw1(limitSw1), limitSw2(limitSw2)
{
  dual = true;
}


void StepDriver::home() {
  // Travel home fast
  while (!checkLimit()) {
    stepNeg();
    delay_microseconds(MAX_PACE);
  }
  delay(1);
  // Move to deactivate limit switch
  while (checkLimit()) {
    stepPos();
    delay_microseconds(MAX_PACE);
  }
  // Move away from home
  for (int i=0; i<BACKUP_STEPS; i++) {
    stepPos();
    delay_microseconds(MAX_PACE);
  }
  // Approach home slowly
  while (!checkLimit()) {
    stepNeg();
    delay_microseconds(CALIB_PACE);
  }
  // Align dual axis
  if (dual) {
    if (!limitSw1.isActive()) {
      // Bring primary axis into alignment with secondary axis
      while (!limitSw1.isActive()) {
        stepper1.stepNeg();
        delay_microseconds(CALIB_PACE);
      }
    } else {
      // Bring secondary axis into alignment with primary axis
      while (!limitSw2.isActive()) {
        stepper2.stepNeg();
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
  stepper1.stepPos();
  if (dual) stepper2.stepPos();
}

void StepDriver::stepNeg() {
  stepper1.stepNeg();
  if (dual) stepper2.stepNeg();
}


bool StepDriver::checkLimit() {
  return limitSw1.isActive() || (dual && limitSw2.isActive());
}

