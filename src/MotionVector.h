#ifndef __MOTION_VECTOR_H
#define __MOTION_VECTOR_H

#include <stdint.h>
#include "Config.h"
#include "Cartesian.h"


// Volatility built into class
class MotionVector{
  public:
    Point start, end;
    Velocity velocity;
    uint32_t startTime;
    MotionVector();
    MotionVector(const Point &start, const Point &end, const Velocity &velocity, const uint32_t &startTime);
    MotionVector(const MotionVector &vec);
};


// FIFO buffer for holding MotionVectors
class MotionVectorBuffer {

  public:

    MotionVectorBuffer();

    bool isEmpty();
    bool isFull();
    uint32_t getSize();

    bool add(const MotionVector &inst);
    bool remove(MotionVector &inst);

  private:

    MotionVector buff[STEP_INSTRUCTION_BUFFER_SIZE];
    volatile uint32_t head = 0;
    volatile uint32_t tail = 0;
    volatile bool empty = true;

};

extern MotionVectorBuffer motionVectorBuffer;


#endif