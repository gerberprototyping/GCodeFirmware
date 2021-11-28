#include "GPIO.h"
#include "stm32l476xx.h"


void enableGPIOA() {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
}
void enableGPIOB() {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
}
void enableGPIOC() {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
}
void enableGPIOD() {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
}
void enableGPIOE() {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
}
void enableGPIOF() {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
}
void enableGPIOG() {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOGEN;
}
void enableGPIOH() {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN;
}


void enableGPIO(gpio_t gpio) {
    if (gpio.port == GPIOA) {
      enableGPIOA();
    } else if (gpio.port == GPIOB) {
      enableGPIOB();
    } else if (gpio.port == GPIOC) {
      enableGPIOC();
    } else if (gpio.port == GPIOD) {
      enableGPIOD();
    } else if (gpio.port == GPIOE) {
      enableGPIOE();
    } else if (gpio.port == GPIOF) {
      enableGPIOF();
    } else if (gpio.port == GPIOG) {
      enableGPIOG();
    } else if (gpio.port == GPIOH) {
      enableGPIOH();
    }
}


void setMODER(gpio_t gpio, gpio_mode_t mode) {
  uint32_t x = gpio.port->MODER;
  x &= ~(0x03 << (gpio.pin*2));
  x |= mode << (gpio.pin*2);
  gpio.port->MODER = x;
}


void setPUPDR(gpio_t gpio, gpio_pupd_t pupd) {
  uint32_t x = gpio.port->PUPDR;
  x &= ~(0x03 << (gpio.pin*2));
  x |= pupd << (gpio.pin*2);
  gpio.port->PUPDR = x;
}

void setOTYPER(gpio_t gpio, gpio_otype_t otype) {
  uint32_t x = gpio.port->OTYPER;
  x &= ~(0x01 << gpio.pin);
  x |= otype << gpio.pin;
  gpio.port->OTYPER = x;
}

void setOSPEEDR(gpio_t gpio, gpio_ospeed_t ospeed) {
  uint32_t x = gpio.port->OSPEEDR;
  x &= ~(0x03 << (gpio.pin*2));
  x |= ospeed << (gpio.pin*2);
  gpio.port->OSPEEDR = x;
}


void setAFR(gpio_t gpio, uint8_t af) {
  af &= 0x0F;
  volatile uint32_t *AFR = (gpio.pin < 8) ? &(gpio.port->AFR[0]) : &(gpio.port->AFR[1]);
  uint32_t x = *AFR;
  uint32_t shift = (gpio.pin % 8) * 4;
  x &= ~(0x0F << shift);
  x |= af << shift;
  *AFR = x;
}


void digitalWrite(gpio_t gpio, bool state) {
  uint32_t x = gpio.port->ODR;
  x &= ~(0x01 << gpio.pin);
  x |= state << gpio.pin;
  gpio.port->ODR = x;
}


bool digitalRead(gpio_t gpio) {
  uint32_t x = gpio.port->IDR;
  x &= 0x01 << gpio.pin;
  return (bool) x;
}



