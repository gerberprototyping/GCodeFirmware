#include "clock.h"
#include "stm32l476xx.h"
#include "GPIO.h"

#define HSI_FREQ            16000000    // High Speed Internal oscillator is 16MHz


volatile uint32_t milliseconds = 0;


// SysTick Register Map
typedef struct {
  __IO uint32_t CSR;          // Control Status Register
  __IO uint32_t RVR;          // Reload Value Register
  __IO uint32_t CVR;          // Current Value Register
  __I  uint32_t CALIB;        // Calibration value register (10ms)
} SYST_TypeDef;

#define SYST_BASE   0xE000E010
#define SYST        ((SYST_TypeDef *) SYST_BASE)




void initClock() {
  initClockHSI();
}


void initClockHSI() {
  RCC->CR |= RCC_CR_HSION;                                        // Turn on High Speed Internal 16 MHz clock
  while( !(RCC->CR & RCC_CR_HSIRDY) );                            // Wait for HSI ready
  uint32_t x = RCC->CFGR & ~0x03;
  RCC->CFGR = x | RCC_CFGR_SW_HSI;                                // Set System Clock to HSI
  while( RCC_CFGR_SWS_HSI != (RCC->CFGR & RCC_CFGR_SWS_HSI) );    // Wait for SYSCLK to be set to HSE
  initSysTick(SYSTICK_1MS_HSI);
}


void initClockHSE() {
  RCC->CR |= RCC_CR_HSEON | RCC_CR_HSEBYP;                        // Turn on High Speed External clock and bypass crystal oscillator
  while( !(RCC->CR & RCC_CR_HSERDY) );                            // Wait for HSE ready
  uint32_t x = RCC->CFGR & ~0x03;
  RCC->CFGR = x | RCC_CFGR_SW_HSE;                                // Set System Clock to HSE
  while( RCC_CFGR_SWS_HSE != (RCC->CFGR & RCC_CFGR_SWS) );        // Wait for SYSCLK to be set to HSE
  initSysTick(SYSTICK_1MS_HSE);
}


void initClockPLL(uint32_t n, uint32_t m, uint32_t r) {
  bool valid = (8 <= n) && (n <= 20);
  valid &= (1 <= m) && (m <= 4);
  switch (r) {
    case 2:
      r = 0x00;
      break;
    case 4:
      r = 0x01;
      break;
    case 6:
      r = 0x02;
      break;
    case 8:
      r = 0x03;
      break;
    default:
      valid = false;
      break;
  }
  if (valid) {

    // Turn off PLL
    RCC->CR &= ~RCC_CR_PLLON;                                       // Turn off the PLL
    while(RCC->CR & RCC_CR_PLLRDY);                                 // Wait for PLL to fully stop

    // Configure PLL
    uint32_t x = RCC->PLLCFGR;
    x &= ~RCC_PLLCFGR_PLLN;                                         // Clear bits for value n
    x &= ~RCC_PLLCFGR_PLLM;                                         // Clear bits for value m
    x &= ~RCC_PLLCFGR_PLLR & ~RCC_PLLCFGR_PLLREN;                   // Clear bits for value r
    x &= ~RCC_PLLCFGR_PLLSRC;                                       // Clear bits for source select
    x |= n << 8;                                                    // Insert value for n
    x |= (m-1) << 4;                                                // Insert value for m
    x |= r << 25;                                                   // Insert value for r
    x |= RCC_PLLCFGR_PLLREN;                                        // Enable output for f(PLL_R)
    x |= RCC_PLLCFGR_PLLSRC_HSE;                                    // Set source to HSE
    RCC->PLLCFGR = x;

    // Turn on HSE
    RCC->CR |= RCC_CR_HSEON | RCC_CR_HSEBYP;                        // Turn on High Speed External clock and bypass crystal oscillator
    while( !(RCC->CR & RCC_CR_HSERDY) );                            // Wait for HSE ready

    // Turn on PLL
    RCC->CR |= RCC_CR_PLLON;                                        // Turn on the PLL
    while( !(RCC->CR & RCC_CR_PLLRDY) );                            // Wait for PLL ready

    // Set SYSCLK
    x = RCC->CFGR & ~0x03;
    RCC->CFGR = x | RCC_CFGR_SW_PLL;                                // Set System Clock to PLL
    while( RCC_CFGR_SWS_PLL != (RCC->CFGR & RCC_CFGR_SWS) );        // Wait for SYSCLK to be set to PLL

  }
}


void initClock24() {
  initClockPLL(6,1,2);
  initSysTick(SYSTICK_1MS_24MHZ);
}


void initClock32() {
  initClockPLL(8,1,2);
  initSysTick(SYSTICK_1MS_32MHZ);
}


void initClock40() {
  initClockPLL(10, 1, 2);
  initSysTick(SYSTICK_1MS_40MHZ);
}

void initClock48() {
  uint32_t x = RCC->CFGR & ~RCC_CFGR_PPRE1 & ~RCC_CFGR_PPRE2;
  RCC->CFGR = x | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV4;
  initClockPLL(12, 1, 2);
  initSysTick(SYSTICK_1MS_48MHZ);
}

void initClock64() {
  initClockPLL(16, 1, 2);
  initSysTick(SYSTICK_1MS_64MHZ);
}

void initClock72() {
  initClockPLL(18, 1, 2);
  initSysTick(SYSTICK_1MS_72MHZ);
}

void initClock80() {
  initClockPLL(20, 1, 2);
  initSysTick(SYSTICK_1MS_80MHZ);
}




void initSysTick(uint32_t reload_value) {
  milliseconds   = 0;
  SYST->CSR = 0;
  SYST->CVR = 0;
  SYST->RVR = reload_value - 1;   // 24-bit reload value
  SYST->CSR = 0x07;               // Enable SysTick with core clock reference
  #ifdef GREEN_LED_PIN
    enableGPIO(GREEN_LED_PIN);
    setMODER(GREEN_LED_PIN, OUTPUT);
    digitalWrite(GREEN_LED_PIN, HIGH);
  #endif
}


extern "C" void SysTick_Handler(void) {
  milliseconds++;
}


uint32_t millis() {
  return milliseconds;
}


void delay(uint32_t ms) {
  uint32_t start_tick = SYST->CVR;
  uint32_t end_milli = milliseconds + ms;
  while (milliseconds < end_milli);
  while ( (start_tick < SYST->CVR) && (milliseconds == end_milli) );
}


void delay_microseconds(uint32_t us) {
  uint32_t start_tick = SYST->CVR;
  uint32_t start_milli = milliseconds;
  if (SYST->CVR > start_tick) {   // Check for CVR reload as it may cause inconsistency between start_tick and start_milli
    start_tick = SYST->CVR;
    start_milli = milliseconds;
  }
  uint32_t reload_value = SYST->RVR;

  // Separate out milli and micro portions
  uint32_t end_milli = start_milli + (us/1000);
  us = us%1000;

  // Calc number of ticks required. Use LSB as fractional for acurate rounding up
  uint32_t delta_tick = ((reload_value << 1) * (us << 1)) / (1000 << 1);      // (TotalTick x us) / 1000
  delta_tick = (delta_tick >> 1) + (delta_tick & 0x01);                       // Remove fractional bit rounding up

  uint32_t end_tick = start_tick - delta_tick;
  if (start_tick < delta_tick) {
    end_milli++;
    end_tick = reload_value - (delta_tick - start_tick);
  }
  while(milliseconds < end_milli);
  while(end_tick < SYST->CVR);
}