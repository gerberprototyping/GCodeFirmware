/*
 * builtin.h
 *
 *  Created on: Feb 24, 2024
 *      Author: Andrew Gerber
 */

#ifndef INC_BUILTIN_H_
#define INC_BUILTIN_H_


#include "main.h"


static inline void builtin_led_toggle() {
	HAL_GPIO_TogglePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin);
}
static inline void builtin_led_set(bool state) {
	HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, (GPIO_PinState) state);
}


static inline bool builtin_btn_read() {
	return HAL_GPIO_ReadPin(BUILTIN_BTN_GPIO_Port, BUILTIN_BTN_Pin);
}


#endif /* INC_BUILTIN_H_ */
