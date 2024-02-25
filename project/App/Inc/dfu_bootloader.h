/*
 * dfu_bootloader.h
 *
 *  Created on: Feb 24, 2024
 *      Author: Andrew Gerber
 */

#ifndef __DFU_BOOTLOADER_H
#define __DFU_BOOTLOADER_H

#define BOOTLOADER_ROM_ADDR        0x1FFF0000


// https://stm32f4-discovery.net/2017/04/tutorial-jump-system-memory-software-stm32/
// https://github.com/markusgritsch/SilF4ware/blob/94bb679119a0b9879fedc62a5e22f40623433242/SilF4ware/drv_reset.c
void jump_to_bootloader() {
    __enable_irq();
    HAL_RCC_DeInit();
    SysTick->CTRL = SysTick->LOAD = SysTick->VAL = 0;
    __HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH();

    // device specific bootloader ROM address
    const uint32_t bootloader_ptr = (* ((uint32_t *)  BOOTLOADER_ROM_ADDR));
    __set_MSP(bootloader_ptr);

    void (*SysMemBootJump)(void);
    SysMemBootJump = (void (*)(void)) (* ((uint32_t *)  BOOTLOADER_ROM_ADDR+4));
    SysMemBootJump();

    while(true) {
        // dead loop
    }
}


#endif /* __DFU_BOOTLOADER_H */
