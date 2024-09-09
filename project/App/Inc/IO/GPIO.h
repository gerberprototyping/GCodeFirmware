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
    #ifdef BUILTIN_LED_Pin
    HAL_GPIO_TogglePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin);
    #endif
}
static inline void builtin_led_set(bool state) {
    #ifdef BUILTIN_LED_Pin
    // HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, (GPIO_PinState) (state ? 0 : 1));
    HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, (GPIO_PinState) (state ? 1 : 0));
    #endif
}


static inline bool builtin_btn_read() {
    #ifdef BUILTIN_BTN_Pin
    return HAL_GPIO_ReadPin(BUILTIN_BTN_GPIO_Port, BUILTIN_BTN_Pin);
    #else
    return false;
    #endif
}




#define HIGH true
#define LOW false

#define GPIO_NULL   ((gpio_t) {.port = ((GPIO_TypeDef *) uint32_t(0)), .pin = 0})

#define A0          ((gpio_t) {.port = GPIOA, .pin = 0})
#define A1          ((gpio_t) {.port = GPIOA, .pin = 1})
#define A2          ((gpio_t) {.port = GPIOA, .pin = 2})
#define A3          ((gpio_t) {.port = GPIOA, .pin = 3})
#define A4          ((gpio_t) {.port = GPIOA, .pin = 4})
#define A5          ((gpio_t) {.port = GPIOA, .pin = 5})
#define A6          ((gpio_t) {.port = GPIOA, .pin = 6})
#define A7          ((gpio_t) {.port = GPIOA, .pin = 7})
#define A8          ((gpio_t) {.port = GPIOA, .pin = 8})
#define A9          ((gpio_t) {.port = GPIOA, .pin = 9})
#define A10         ((gpio_t) {.port = GPIOA, .pin = 10})
#define A11         ((gpio_t) {.port = GPIOA, .pin = 11})
#define A12         ((gpio_t) {.port = GPIOA, .pin = 12})
#define A13         ((gpio_t) {.port = GPIOA, .pin = 13})
#define A14         ((gpio_t) {.port = GPIOA, .pin = 14})
#define A15         ((gpio_t) {.port = GPIOA, .pin = 15})
#define B0          ((gpio_t) {.port = GPIOB, .pin = 0})
#define B1          ((gpio_t) {.port = GPIOB, .pin = 1})
#define B2          ((gpio_t) {.port = GPIOB, .pin = 2})
#define B3          ((gpio_t) {.port = GPIOB, .pin = 3})
#define B4          ((gpio_t) {.port = GPIOB, .pin = 4})
#define B5          ((gpio_t) {.port = GPIOB, .pin = 5})
#define B6          ((gpio_t) {.port = GPIOB, .pin = 6})
#define B7          ((gpio_t) {.port = GPIOB, .pin = 7})
#define B8          ((gpio_t) {.port = GPIOB, .pin = 8})
#define B9          ((gpio_t) {.port = GPIOB, .pin = 9})
#define B10         ((gpio_t) {.port = GPIOB, .pin = 10})
#define B11         ((gpio_t) {.port = GPIOB, .pin = 11})
#define B12         ((gpio_t) {.port = GPIOB, .pin = 12})
#define B13         ((gpio_t) {.port = GPIOB, .pin = 13})
#define B14         ((gpio_t) {.port = GPIOB, .pin = 14})
#define B15         ((gpio_t) {.port = GPIOB, .pin = 15})
#define C0          ((gpio_t) {.port = GPIOC, .pin = 0})
#define C1          ((gpio_t) {.port = GPIOC, .pin = 1})
#define C2          ((gpio_t) {.port = GPIOC, .pin = 2})
#define C3          ((gpio_t) {.port = GPIOC, .pin = 3})
#define C4          ((gpio_t) {.port = GPIOC, .pin = 4})
#define C5          ((gpio_t) {.port = GPIOC, .pin = 5})
#define C6          ((gpio_t) {.port = GPIOC, .pin = 6})
#define C7          ((gpio_t) {.port = GPIOC, .pin = 7})
#define C8          ((gpio_t) {.port = GPIOC, .pin = 8})
#define C9          ((gpio_t) {.port = GPIOC, .pin = 9})
#define C10         ((gpio_t) {.port = GPIOC, .pin = 10})
#define C11         ((gpio_t) {.port = GPIOC, .pin = 11})
#define C12         ((gpio_t) {.port = GPIOC, .pin = 12})
#define C13         ((gpio_t) {.port = GPIOC, .pin = 13})
#define C14         ((gpio_t) {.port = GPIOC, .pin = 14})
#define C15         ((gpio_t) {.port = GPIOC, .pin = 15})
#define D2          ((gpio_t) {.port = GPIOD, .pin = 2})
#define H0          ((gpio_t) {.port = GPIOH, .pin = 0})
#define H1          ((gpio_t) {.port = GPIOH, .pin = 1})


#define ARDUINO_A0    A0
#define ARDUINO_A1    A1
#define ARDUINO_A2    A4
#define ARDUINO_A3    B0
#define ARDUINO_A4    C1
#define ARDUINO_A5    C0
#define ARDUINO_D0    A3
#define ARDUINO_D1    A2
#define ARDUINO_D2    A10
#define ARDUINO_D3    B3
#define ARDUINO_D4    B5
#define ARDUINO_D5    B4
#define ARDUINO_D6    B10
#define ARDUINO_D7    A8
#define ARDUINO_D8    A9
#define ARDUINO_D9    C7
#define ARDUINO_D10   B6
#define ARDUINO_D11   A7
#define ARDUINO_D12   A6
#define ARDUINO_D13   A5
#define ARDUINO_D14   B9
#define ARDUINO_D15   B8


#endif /* __GPIO_H */
