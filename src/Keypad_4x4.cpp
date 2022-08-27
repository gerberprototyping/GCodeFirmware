#include "Keypad_4x4.h"
#include "clock.h"
#include "GPIO.h"


Keypad_4x4::Keypad_4x4(gpio_t row1, gpio_t row2, gpio_t row3, gpio_t row4, gpio_t col1, gpio_t col2, gpio_t col3, gpio_t col4) {
    
    row[0] = row1;
    row[1] = row2;
    row[2] = row3;
    row[3] = row4;
    col[0] = col1;
    col[1] = col2;
    col[2] = col3;
    col[3] = col4;

    for (int i=0; i<4; i++) {

        enableGPIO(row[i]);
        setMODER(row[i], OUTPUT);
        setOTYPER(row[i], OPEN_DRAIN);

        enableGPIO(col[i]);
        setMODER(col[i], INPUT);

    }

}


char Keypad_4x4::getKeyPress(void) {
    for (int r=0; r<4; r++) {
        for (int i=0; i<4; i++) {
            digitalWrite(row[i], !(i==r));
        }
        delay(1);
        for (int c=0; c<4; c++) {
            if (!digitalRead(col[c])) {
                delay(10);
                if (!digitalRead(col[c])) {
                    return char_arr[r][c];
                }
            }
        }
    }
    return '\0';
}