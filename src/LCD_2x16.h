#ifndef __STM32L476R_NUCLEO_LCD_H
#define __STM32L476R_NUCLEO_LCD_H

#include <stdint.h>
#include "GPIO.h"

#define CGRAM   1
#define DDRAM   0


class LCD_2x16 {

    public:

        struct addr_t {
            bool type;
            uint8_t val;
        };

        LCD_2x16(gpio_t rs_pin, gpio_t rw_pin, gpio_t e_pin, gpio_t db4_pin, gpio_t db5_pin, gpio_t db6_pin, gpio_t db7_pin);
        void clear(void);
        void home(void);
        void set_line(int line);
        void display(bool enable = true);
        void display_on(void);
        void display_off(void);
        void cursor(bool enable = true, bool blink = true);

        void cursor_right(void);
        void cursor_left(void);
        void scroll_right(void);
        void scroll_left(void);

        void print(char c);
        void print(int c);
        void print(char* str);
        void print(int line, char* str);

        void set_custom(uint8_t id, uint8_t char_data[8]);
        uint8_t get_data();
        uint8_t get_data(addr_t addr);
        addr_t get_addr();
        void set_addr(addr_t addr);
        void set_cgram_addr(uint8_t addr);
        void set_ddram_addr(uint8_t addr);

    private:

        gpio_t rs;
        gpio_t rw;
        gpio_t e;
        gpio_t db[4];

        uint8_t display_ctrl_state;

        void set_input_mode(bool cursor_dir_right = true, bool scroll = false);
        void move_cursor(bool move_dir_right = true, bool move_display = false);
        void set_function(bool bus_width_8 = false, bool dual_line = true, bool font_5x11 = false);

        void _write_cmd(uint8_t cmd);
        void _write_data(uint8_t data);
        void _write(bool _rs, uint8_t data);
        uint8_t _read(bool _rs = HIGH);
        void _wait(void);
        void _align(void);

};


#endif
