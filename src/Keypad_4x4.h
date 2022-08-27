#ifndef __KEYPAD_H
#define __KEYPAD_H

#include "GPIO.h"


class Keypad_4x4 {

    public:
        Keypad_4x4(gpio_t row1, gpio_t row2, gpio_t row3, gpio_t row4, gpio_t col1, gpio_t col2, gpio_t col3, gpio_t col4);
        char getKeyPress(void);

        const char char_arr[4][4] = { {'1','2','3','A'}, {'4','5','6','B'}, {'7','8','9','C'}, {'*','0','#','D'} };

    private:
        gpio_t row[4];
        gpio_t col[4];

};


#endif