#include "TimerInterrupt.h"


bool TimerInterrupt::initialized = false;

void emptyFunc() {}
ISR TimerInterrupt::isr = &emptyFunc;


TimerInterrupt::TimerInterrupt() {
}


void TimerInterrupt::init(uint32_t period, ISR interruptHandler) {
  if (!initialized) {
    initialized = true;
    isr = interruptHandler;
    NVIC_EnableIRQ(TIM2_IRQn);
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;                   // Enable Timer 2 clock
    TIM2->CR1  &= ~TIM_CR1_DIR;                             // Select up-counting
    TIM2->CR1  |=  TIM_CR1_URS;                             // Only overflow/underlfow generates update interrupt
    TIM2->CR1  &= ~TIM_CR1_UDIS;                            // Enable update event
    TIM2->PSC   =  0 & 0x0000FFFF;                          // Pescaler 16-bit (0 + 1) = 1
    TIM2->ARR   =  period - 1;                              // Set auto-reload value
    TIM2->DIER |=  TIM_DIER_UIE;                            // Update interrupt enable
  }
}


void TimerInterrupt::start() {
  if (initialized) {
    TIM2->CR1 |= TIM_CR1_CEN;                               // Enable counter
  }
}


void TimerInterrupt::stop() {
  TIM2->CR1 &= ~TIM_CR1_CEN;                                // Disable counter
}


void TimerInterrupt::callISR() {
  isr();
}


extern "C" void TIM2_IRQHandler(void) {
  TimerInterrupt::callISR();
  TIM2->SR &= ~TIM_SR_UIF;
  __NVIC_ClearPendingIRQ(TIM2_IRQn);
}
