#include "StepVector.h"


StepVectorBuffer stepVectorBuffer = StepVectorBuffer();




StepVector::StepVector()
  : start(Point()), end(Point()), velocity(Velocity()), startTime(0)
{}

StepVector::StepVector(const Point &start, const Point &end, const Velocity &velocity, const uint32_t &startTime)
    : start(start), end(end), velocity(velocity), startTime(startTime)
{}

StepVector::StepVector(const StepVector &vec)
    : StepVector(vec.start, vec.end, vec.velocity, vec.startTime)
{}




StepVectorBuffer::StepVectorBuffer() {
}


bool StepVectorBuffer::isEmpty() {
  return empty;
}


bool StepVectorBuffer::isFull() {
  return !empty;
}


uint32_t StepVectorBuffer::getSize() {
  uint32_t size = 0;
  if (!empty) {
    if (head < tail) {
      size = tail - head;
    } else {
      size = tail + (STEP_INSTRUCTION_BUFFER_SIZE - head);
    }
  }
  return size;
}


bool StepVectorBuffer::add(const StepVector &inst) {
  bool success = false;
  if (head != tail || empty) {
    buff[tail] = inst;
    tail++;
    if (tail >= STEP_INSTRUCTION_BUFFER_SIZE) {
      tail = 0;
    }
    empty = false;
    success = true;
  }
  return success;
}


bool StepVectorBuffer::remove(StepVector &inst) {
  bool success = false;
  if (!empty) {
    inst = buff[head];
    head++;
    if (head >= STEP_INSTRUCTION_BUFFER_SIZE) {
      head = 0;
    }
    if (head == tail) {
      empty = true;
    }
  }
  return success;
}

