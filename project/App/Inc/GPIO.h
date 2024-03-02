/*
 * bpio.h
 *
 *  Created on: Feb 24, 2024
 *      Author: Andrew Gerber
 */

#ifndef __GPIO_H
#define __GPIO_H


#include "main.h"


typedef struct {
    GPIO_TypeDef*   port;
    uint16_t        pin;
} gpio_t;

#define GPIO_NULL       ((gpio_t) {.port=NULL, .pin=0})

static inline bool operator==(const gpio_t &lhs, const gpio_t &rhs) {
    return (lhs.pin == rhs.pin) && (lhs.port == rhs.port);
}
static inline bool operator!=(const gpio_t &lhs, const gpio_t &rhs) {
    return (lhs.pin != rhs.pin) || (lhs.port != rhs.port);
}


static inline void digitalWrite(const gpio_t &pin, bool state) {
    HAL_GPIO_WritePin(pin.port, pin.pin, (GPIO_PinState) (state ? 1 : 0));
}

static inline bool digitalRead(const gpio_t &pin) {
    return HAL_GPIO_ReadPin(pin.port, pin.pin);
}


static inline void builtin_led_toggle() {
    HAL_GPIO_TogglePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin);
}
static inline void builtin_led_set(bool state) {
    HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, (GPIO_PinState) (state ? 0 : 1));
}


static inline bool builtin_btn_read() {
    return HAL_GPIO_ReadPin(BUILTIN_BTN_GPIO_Port, BUILTIN_BTN_Pin);
}


#endif /* __GPIO_H */
