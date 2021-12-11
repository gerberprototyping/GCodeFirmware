#ifndef __NUCLEO_L476_RG_H
#define __NUCLEO_L476_RG_H

#include "stm32l476xx.h"
#include "GPIO.h"
#include "clock.h"


void init();

void initLED();
void setLED(bool state);


#endif
