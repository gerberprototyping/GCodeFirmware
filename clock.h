#ifndef __CLOCK_H
#define __CLOCK_H

#include "stm32l476xx.h"

#define GREEN_LED_PIN           A5


#define SYSTICK_1MS_8MHZ        0x001F40
#define SYSTICK_1MS_16MHZ       0x003E80
#define SYSTICK_1MS_24MHZ       0x005DC0
#define SYSTICK_1MS_32MHZ       0x007D00
#define SYSTICK_1MS_40MHZ       0x009C40
#define SYSTICK_1MS_48MHZ       0x00BB80
#define SYSTICK_1MS_64MHZ       0x00FA00
#define SYSTICK_1MS_72MHZ       0x011940
#define SYSTICK_1MS_80MHZ       0x013880

#define SYSTICK_1MS_HSI         SYSTICK_1MS_16MHZ
#define SYSTICK_1MS_HSE         SYSTICK_1MS_8MHZ




void initClock(void);
void initClockHSI(void);
void initClockHSE(void);
void initClockPLL(uint32_t n, uint32_t m, uint32_t r);
void initClock40();
void initClock48();
void initClock64();
void initClock72();
void initClock80();

void initSysTick(uint32_t reload_value);


uint32_t millis();
void delay(uint32_t ms);
void delay_microseconds(uint32_t us);


#endif
