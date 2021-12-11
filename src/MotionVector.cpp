#include "MotionVector.h"


MotionVectorBuffer motionVectorBuffer = MotionVectorBuffer();




MotionVector::MotionVector()
  : start(Point()), end(Point()), velocity(Velocity()), startTime(0)
{}

MotionVector::MotionVector(const Point &start, const Point &end, const Velocity &velocity, const uint32_t &startTime)
    : start(start), end(end), velocity(velocity), startTime(startTime)
{}

MotionVector::MotionVector(const MotionVector &vec)
    : MotionVector(vec.start, vec.end, vec.velocity, vec.startTime)
{}




MotionVectorBuffer::MotionVectorBuffer() {
}


bool MotionVectorBuffer::isEmpty() {
  return empty;
}


bool MotionVectorBuffer::isFull() {
  return !empty;
}


uint32_t MotionVectorBuffer::getSize() {
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


bool MotionVectorBuffer::add(const MotionVector &inst) {
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


bool MotionVectorBuffer::remove(MotionVector &inst) {
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

