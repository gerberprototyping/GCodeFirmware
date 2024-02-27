/*
 * maintask.cpp
 *
 *  Created on: Feb 24, 2024
 *      Author: Andrew Gerber
 */

#include "maintask.h"

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#include "builtin.h"
#include "serial.h"


Serial serial;


extern "C" void main_init(osMutexId_t RXBuffLock) {
    // Init USB
    serial.init(RXBuffLock);

    // builtin_led_set(0);
}


uint8_t buff[64];
extern "C" void StartMainTask(void *argument) {
    while (1) {
        // infinite loop
        // uint32_t len = serial.readline(buff, 64-2);
        // buff[len++] = '\r';
        // buff[len++] = '\n';
        // serial_tx(buff, len);

        builtin_led_toggle();
        osDelay(500);
    }

}
