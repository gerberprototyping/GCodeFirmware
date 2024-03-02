#ifndef __ARDUINO_H
#define __ARDUINO_H

#include <string.h>
#include <math.h>

#include "main.h"


inline uint32_t millis() {
    return HAL_GetTick();
}

inline void delay(uint32_t ms) {
    osDelay(ms);
}

inline uint8_t pgm_read_byte(const uint8_t* addr) {
    return *addr;
}


#endif // __ARDUINO_H
