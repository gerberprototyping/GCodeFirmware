#include "MotionVector.h"


MotionVectorBuffer motionVectorBuffer;




/***********************************************************
 * MotionVector
 **********************************************************/

MotionVector::MotionVector()
    : start(Point()), end(Point()), velocity(Velocity())
{
    // Intentionally left blank
}

MotionVector::MotionVector(const Point &start, const Point &end, const double &velocityMagnitude)
    : start(start), end(end)
{
    CartesianDouble dir = CartesianDouble(end - start).normalize();
    velocity = Velocity(dir * velocityMagnitude);
}

MotionVector::MotionVector(const MotionVector &vec)
    : start(vec.start), end(vec.end), velocity(vec.velocity)
{
    // Intentionally left blank
}

MotionVector::MotionVector(const volatile MotionVector &vec) {
    start    = *static_cast<const volatile    Point*>(&vec.start);
    end      = *static_cast<const volatile    Point*>(&vec.end);
    velocity = *static_cast<const volatile Velocity*>(&vec.velocity);
}

MotionVector& MotionVector::operator=(const MotionVector &vec) {
    start    = vec.start;
    end      = vec.end;
    velocity = vec.velocity;
    return *this;
}

MotionVector& MotionVector::operator=(const volatile MotionVector &vec) {
    start    = *static_cast<const volatile    Point*>(&vec.start);
    end      = *static_cast<const volatile    Point*>(&vec.end);
    velocity = *static_cast<const volatile Velocity*>(&vec.velocity);
    return *this;
}

volatile MotionVector& MotionVector::operator=(const MotionVector &vec) volatile {
    *static_cast<volatile    Point*>(&start)    = vec.start;
    *static_cast<volatile    Point*>(&end)      = vec.end;
    *static_cast<volatile Velocity*>(&velocity) = vec.velocity;
    return *this;
}


Point MotionVector::getStart() const {
    return start;
}

Point MotionVector::getEnd() const {
    return end;
}

Velocity MotionVector::getVelocity() const {
    return velocity;
}


CartesianBool operator>=(const Point &curr, const MotionVector &vec) {
    CartesianBool result = CartesianBool(true, true, true);
    // X Axis
    if (vec.start.getXSteps() <= vec.end.getXSteps()) {
        if (curr.getXSteps() < vec.end.getXSteps()) {
            result.setX(false);
        }
    } else {
        if (curr.getXSteps() > vec.end.getXSteps()) {
            result.setX(false);
        }
    }
    // Y Axis
    if (vec.start.getYSteps() <= vec.end.getYSteps()) {
        if (curr.getYSteps() < vec.end.getYSteps()) {
            result.setY(false);
        }
    } else {
        if (curr.getYSteps() > vec.end.getYSteps()) {
            result.setY(false);
        }
    }
    // Z Axis
    if (vec.start.getZSteps() <= vec.end.getZSteps()) {
        if (curr.getZSteps() < vec.end.getZSteps()) {
            result.setZ(false);
        }
    } else {
        if (curr.getZSteps() > vec.end.getZSteps()) {
            result.setZ(false);
        }
    }
    return result;
}




/***********************************************************
 * MotionVectorBuffer
 **********************************************************/

MotionVectorBuffer::MotionVectorBuffer()
    : head(0), tail(0), empty(true)
{
    // Intentionally left blank
}


bool MotionVectorBuffer::isEmpty() {
    return empty;
}

bool MotionVectorBuffer::isFull() {
    return !empty && (head != tail);
}

uint32_t MotionVectorBuffer::getSize() {
    uint32_t size = 0;
    if (!empty) {
        uint32_t _head = head;
        uint32_t _tail = tail;
        if (_head < _tail) {
            size = _tail - _head;
        } else {
            size = _tail + (STEP_INSTRUCTION_BUFFER_SIZE - _head);
        }
    }
    return size;
}


bool MotionVectorBuffer::add(const MotionVector &vec) {
    if (!empty && head == tail) {
        return false;
    }
    buff[tail] = vec;
    uint32_t _tail = tail + 1;
    if (_tail >= STEP_INSTRUCTION_BUFFER_SIZE) {
        _tail = 0;
    }
    tail = _tail;
    empty = false;
    return true;
}

bool MotionVectorBuffer::remove(MotionVector* vec) {
    if (empty) {
        return false;
    }
    *vec = buff[head];
    uint32_t _head = head + 1;
    if (_head >= STEP_INSTRUCTION_BUFFER_SIZE) {
        _head = 0;
    }
    head = _head;
    if (_head == tail) {
        empty = true;
    }
    return true;
}

bool MotionVectorBuffer::peek(MotionVector* vec) {
    if (empty) {
        return false;
    }
    *vec = buff[head];
    return true;
}
