#ifndef __ARDUINO_H
#define __ARDUINO_H

#include <string.h>
#include <math.h>

#include "main.h"

#define TIMEBASE    TIM2


inline uint32_t millis() {
    return HAL_GetTick();
}

inline void delay(uint32_t ms) {
    osDelay(ms);
}


static uint64_t microseconds() {
    uint32_t tick = TIMEBASE->CNT;
    uint64_t milli = HAL_GetTick();
    // Check for counter reload as it may cause inconsistency between tick and milli
    if (TIMEBASE->CNT > tick) {
        tick = TIMEBASE->CNT;
        milli = HAL_GetTick();
    }
    uint32_t reload_value = TIMEBASE->ARR;
    return (milli * 1000) + ( (tick*1000) / reload_value);
}

static void delay_microseconds(uint32_t us) {
    uint32_t start_tick = TIMEBASE->CNT;
    uint32_t start_milli = HAL_GetTick();
    // Check for counter reload as it may cause inconsistency between start_tick and start_milli
    if (TIMEBASE->CNT > start_tick) {
        start_tick = TIMEBASE->CNT;
        start_milli = HAL_GetTick();
    }
    uint32_t reload_value = TIMEBASE->ARR;

    // Separate out milli and micro portions
    uint32_t end_milli = start_milli + (us/1000);
    us = us%1000;

    // Calc number of ticks required. Use LSB as fractional for acurate rounding up
    uint32_t delta_tick = ((reload_value << 1) * (us << 1)) / (1000 << 1);      // (TotalTick x us) / 1000
    delta_tick = (delta_tick >> 1) + (delta_tick & 0x01);                       // Remove fractional bit rounding up

    uint32_t end_tick = start_tick + delta_tick;
    if (end_tick >= reload_value) {
        end_milli++;
        end_tick = end_tick - reload_value;
    }
    while(HAL_GetTick() < end_milli);
    end_milli++;
    while(end_tick < TIMEBASE->CNT && HAL_GetTick() < end_milli);
}

inline uint8_t pgm_read_byte(const uint8_t* addr) {
    return *addr;
}


#endif // __ARDUINO_H
