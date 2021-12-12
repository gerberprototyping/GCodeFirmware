#include "MotionVector.h"


MotionVectorBuffer motionVectorBuffer = MotionVectorBuffer();




MotionVector::MotionVector()
  : start(Point()), end(Point()), velocity(Velocity()), startTime(0)
{}

MotionVector::MotionVector(const Point &start, const Point &end, const double &_velocity)
    : start(start), end(end), startTime(0)
{
  Cartesian<double> path = (end - start).toMM();
  Velocity v = _velocity * (path.normalize());
  velocity = v;
}

MotionVector::MotionVector(const MotionVector &vec)
    : start(vec.start), end(vec.end), velocity(vec.velocity), startTime(vec.startTime)
{}


MotionVector::MotionVector(const volatile MotionVector &vec) {
  start = vec.start;
  end = vec.end;
  velocity = vec.velocity;
  startTime = vec.startTime;
}


void MotionVector::operator=(const volatile MotionVector &vec) volatile {
  start = vec.start;
  end = vec.end;
  velocity = vec.velocity;
  startTime = vec.startTime;
}


bool operator>=(const Point &curr, const MotionVector vec) {
  if (vec.velocity.x >= 0) {
    if (curr.x < vec.end.x) {
      return false;
    }
  } else {
    if (curr.x > vec.end.x) {
      return false;
    }
  }
  if (vec.velocity.y >= 0) {
    if (curr.y < vec.end.y) {
      return false;
    }
  } else {
    if (curr.y > vec.end.y) {
      return false;
    }
  }
  if (vec.velocity.z >= 0) {
    if (curr.z < vec.end.z) {
      return false;
    }
  } else {
    if (curr.z > vec.end.z) {
      return false;
    }
  }
  return true;
}




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


bool MotionVectorBuffer::add(const MotionVector &vec) {
  bool success = false;
  if (head != tail || empty) {
    buff[tail] = vec;
    tail++;
    if (tail >= STEP_INSTRUCTION_BUFFER_SIZE) {
      tail = 0;
    }
    empty = false;
    success = true;
  }
  return success;
}


bool MotionVectorBuffer::remove(volatile MotionVector** vec) {
  bool success = false;
  if (!empty) {
    success = true;
    *vec = buff + head;
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


bool MotionVectorBuffer::peek(volatile MotionVector** vec) {
  bool success = false;
  if (!empty) {
    success = true;
    *vec = buff + head;
  }
  return success;
}

