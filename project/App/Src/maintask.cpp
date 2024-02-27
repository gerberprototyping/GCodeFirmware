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

#define BUFF_SIZE       32
// 01234567890123456789


Serial serial;
uint8_t buff[BUFF_SIZE];
// char str[32];


extern "C" void StartMainTask(void *argument) {
    // Init USB
    serial.init(RXBuffLockHandle);

    builtin_led_set(1);

    while (1) {

        // Test serial.read(buff, len)
        // uint32_t len;
        // do {
        //     len = serial.available();
        // } while (!len);
        // if (len > BUFF_SIZE-2) {
        //     len = BUFF_SIZE-2;
        // }
        // serial.read(buff, len);
        // buff[len++] = '\n';
        // buff[len++] = '\r';
        // serial_tx(buff, len);
        // osDelay(250);


        // Test serial.readline(buff, max)
        uint32_t len = serial.readline(buff, 32);
        serial.print("\n> ");
        serial.write(buff, len);
        serial.println();
        osDelay(10);

    }

}
