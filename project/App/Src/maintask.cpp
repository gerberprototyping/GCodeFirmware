/*
 * maintask.cpp
 *
 *  Created on: Feb 24, 2024
 *      Author: Andrew Gerber
 */

#include "maintask.h"

#include "stm32f4xx_hal.h"

#include "builtin.h"
#include "serial.h"


Serial serial;
uint8_t buff[8];


extern "C" void StartMainTask(void *argument) {
    // Init USB
    serial.init(RXBuffLockHandle);

    builtin_led_set(1);

    while (1) {
        // infinite loop
        uint32_t len = serial.read(buff, 8-2);
        buff[len++] = '\r';
        buff[len++] = '\n';
        serial_tx(buff, len);
    }

}
