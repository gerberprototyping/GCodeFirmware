#ifndef __TIMER_INTERRUPT
#define __TIMER_INTERRUPT

#include "Nucleo-L476RG.h"


typedef void (*ISR)(void);


// Only one instance allowed
class TimerInterrupt {

  public:
  
    TimerInterrupt(uint32_t period, ISR interruptHandler);

    static void callISR();

  private:

    static bool initialized;

    static ISR isr;

};


#endif