#ifndef __MOTION_VECTOR_H
#define __MOTION_VECTOR_H

#include <stdint.h>
#include "Config.h"
#include "Cartesian.h"


// Volatility built into class
class MotionVector{
  public:
    volatile Point start, end;
    volatile Velocity velocity;
    MotionVector();
    MotionVector(const Point &start, const Point &end, const double &velocity);
    MotionVector(const MotionVector &vec);
    MotionVector(const volatile MotionVector &vec);
    void operator=(const volatile MotionVector &vec) volatile;
};

bool operator>=(const Point &curr, const MotionVector vec);


// FIFO buffer for holding MotionVectors
class MotionVectorBuffer {

  public:

    MotionVectorBuffer();

    bool isEmpty() volatile;
    bool isFull() volatile;
    uint32_t getSize() volatile;

    bool add(const MotionVector &vec) volatile;
    bool remove(volatile MotionVector** vec) volatile;
    bool peek(volatile MotionVector** vec) volatile;

  private:

    volatile MotionVector buff[STEP_INSTRUCTION_BUFFER_SIZE];
    volatile uint32_t head = 0;
    volatile uint32_t tail = 0;
    volatile bool empty = true;

};

extern volatile MotionVectorBuffer motionVectorBuffer;


#endif