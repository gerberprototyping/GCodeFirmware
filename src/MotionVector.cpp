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


Point MotionVector::getStart() const {
    return start;
}

Point MotionVector::getEnd() const {
    return end;
}

Velocity MotionVector::getVelocity() const {
    return velocity;
}


bool operator>=(const Point &curr, const MotionVector &vec) {
    if (vec.velocity.getX() >= 0) {
        if (curr.getXSteps() < vec.end.getXSteps()) {
            return false;
        }
    } else {
        if (curr.getXSteps() > vec.end.getXSteps()) {
            return false;
        }
    }
    if (vec.velocity.getY() >= 0) {
        if (curr.getYSteps() < vec.end.getYSteps()) {
            return false;
        }
    } else {
        if (curr.getYSteps() > vec.end.getYSteps()) {
            return false;
        }
    }
    if (vec.velocity.getZ() >= 0) {
        if (curr.getZSteps() < vec.end.getZSteps()) {
            return false;
        }
    } else {
        if (curr.getZSteps() > vec.end.getZSteps()) {
            return false;
        }
    }
    return true;
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
    if (!buff[tail].tryStore(vec)) {
        return false;
    }
    uint32_t _tail = tail + 1;
    if (_tail >= STEP_INSTRUCTION_BUFFER_SIZE) {
        _tail = 0;
    }
    tail = _tail;
    empty = false;
    return true;
}

bool MotionVectorBuffer::remove(MotionVector* vec) {
    if (empty || !buff[head].tryLoad(vec)) {
        return false;
    }
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
    return !empty && buff[head].tryLoad(vec);
}
