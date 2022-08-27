#include "MotionVector.h"


MotionVectorBuffer motionVectorBuffer;


/***********************************************************
 * AtomicMotionVector
 **********************************************************/

AtomicMotionVector::AtomicMotionVector()
    : start(AtomicPoint()), end(AtomicPoint()), velocity(AtomicVelocity())
{
    // Intentionally left blank
}

AtomicMotionVector::AtomicMotionVector(const AtomicPoint &start, const AtomicPoint &end, const AtomicVelocity &velocity)
    : start(start), end(end), velocity(velocity)
{
    // Intentionally left blank
}

void AtomicMotionVector::store(MotionVector const &vec) volatile {
    while(!lock.give());
        start.store(vec.start);
        end.store(vec.end);
        velocity.store(vec.velocity);
    lock.clear();
}

MotionVector AtomicMotionVector::load() volatile {
    MotionVector vec = MotionVector();
    while(!lock.give());
        vec.start    = start.load();
        vec.end      = end.load();
        vec.velocity = velocity.load();
    lock.clear();
    return vec;
}




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


bool MotionVectorBuffer::isEmpty() const {
    return empty;
}

bool MotionVectorBuffer::isFull() const {
    return !empty && (head != tail);
}

uint32_t MotionVectorBuffer::getSize() const {
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
    if (head != tail || empty) {
        buff[tail].store(vec);
        tail++;
        if (tail >= STEP_INSTRUCTION_BUFFER_SIZE) {
            tail = 0;
        }
        empty = false;
        return true;
    }
    return false;
}

bool MotionVectorBuffer::remove(MotionVector* vec) {
    bool success = peek(vec);
    if (success) {
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

bool MotionVectorBuffer::peek(MotionVector* vec) {
    if (!empty) {
        *vec = buff[head].load();
        return true;
    }
    return false;
}
