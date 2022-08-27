#ifndef __MOTION_VECTOR_H
#define __MOTION_VECTOR_H

#include <cstdint>
#include <atomic>
#include <array>

#include "Config.h"
#include "Cartesian.h"
#include "Concurrent.h"




// Forward declare MotionVector and MotionVectorBuffer
class MotionVector;
class MotionVectorBuffer;

class AtomicMotionVector {

    public:

        AtomicMotionVector();
        AtomicMotionVector(const AtomicPoint &start, const AtomicPoint &end, const AtomicVelocity &velocity);
        void store(const MotionVector &vec) volatile;
        MotionVector load() volatile;

    private:

        volatile Semaphore lock;
        volatile AtomicPoint start;
        volatile AtomicPoint end;
        volatile AtomicVelocity velocity;

};

class MotionVector {

    public:

        MotionVector();
        MotionVector(const Point &start, const Point &end, const double &velocity);
        MotionVector(const MotionVector &vec);

        Point getStart() const;
        Point getEnd() const;
        Velocity getVelocity() const;

        friend bool operator>=(const Point &curr, const MotionVector &vec);

        friend class AtomicMotionVector;
        friend class MotionVectorBuffer;

    private:

        Point start;
        Point end;
        Velocity velocity;

};

bool operator>=(const Point &curr, const MotionVector &vec);




/*
 * FIFO buffer to hold future movements
 */
class MotionVectorBuffer {

    public:

        MotionVectorBuffer();

        bool isEmpty() const;
        bool isFull() const;
        uint32_t getSize() const;

        bool add(const MotionVector &vec);
        bool remove(MotionVector* vec);
        bool peek(MotionVector* vec);

    private:

        AtomicMotionVector buff[STEP_INSTRUCTION_BUFFER_SIZE];
        std::atomic<uint32_t> head;
        std::atomic<uint32_t> tail;
        std::atomic<bool> empty;

};

extern MotionVectorBuffer motionVectorBuffer;


#endif // __MOTION_VECTOR_H