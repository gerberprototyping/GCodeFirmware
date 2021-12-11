#ifndef __STEP_INSTRUCTION_H
#define __STEP_INSTRUCTION_H

#include <stdint.h>
#include "Config.h"
#include "Cartesian.h"


// Volatility built into class
class StepVector{
  public:
    Point start, end;
    Velocity velocity;
    uint32_t startTime;
    StepVector();
    StepVector(const Point &start, const Point &end, const Velocity &velocity, const uint32_t &startTime);
    StepVector(const StepVector &vec);
};


// FIFO buffer for holding StepVectors
class StepVectorBuffer {

  public:

    StepVectorBuffer();

    bool isEmpty();
    bool isFull();
    uint32_t getSize();

    bool add(const StepVector &inst);
    bool remove(StepVector &inst);

  private:

    StepVector buff[STEP_INSTRUCTION_BUFFER_SIZE];
    volatile uint32_t head = 0;
    volatile uint32_t tail = 0;
    volatile bool empty = true;

};

extern StepVectorBuffer stepVectorBuffer;


#endif