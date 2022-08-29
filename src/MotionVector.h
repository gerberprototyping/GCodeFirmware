#ifndef __MOTION_VECTOR_H
#define __MOTION_VECTOR_H

#include <cstdint>
#include <atomic>
#include <array>

#include "Config.h"
#include "Cartesian.h"
#include "Concurrent.h"




class MotionVector {

    public:

        MotionVector();
        MotionVector(const Point &start, const Point &end, const double &velocity);
        MotionVector(const MotionVector &vec);

        Point getStart() const;
        Point getEnd() const;
        Velocity getVelocity() const;

        friend bool operator>=(const Point &curr, const MotionVector &vec);

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

        bool isEmpty();
        bool isFull();
        uint32_t getSize();

        bool add(const MotionVector &vec);
        bool remove(MotionVector* vec);
        bool peek(MotionVector* vec);

    private:

        volatile Atomic<MotionVector> buff[STEP_INSTRUCTION_BUFFER_SIZE];
        volatile uint32_t head;
        volatile uint32_t tail;
        volatile bool empty;

};

extern MotionVectorBuffer motionVectorBuffer;


#endif // __MOTION_VECTOR_H