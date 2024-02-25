/*
 * maintask.cpp
 *
 *  Created on: Feb 24, 2024
 *      Author: Andrew Gerber
 */

#include "maintask.h"


void StartMainTask(void *argument) {

	// Init USB
	MX_USB_DEVICE_Init();

	builtin_led_set(1);

	while (1) {
		// infinite loop
	}

}
