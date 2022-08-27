#include "LCD_2x16.h"
#include "clock.h"
#include "GPIO.h"
#include "stm32l476xx.h"




LCD_2x16::LCD_2x16(gpio_t rs_pin, gpio_t rw_pin, gpio_t e_pin, gpio_t db4_pin, gpio_t db5_pin, gpio_t db6_pin, gpio_t db7_pin) {

    rs = rs_pin;
    rw = rw_pin;
    e  = e_pin;
    db[0] = db4_pin;
    db[1] = db5_pin;
    db[2] = db6_pin;
    db[3] = db7_pin;

    display_ctrl_state = 0;

    enableGPIO(rs);
    enableGPIO(rw);
    enableGPIO(e);
    setMODER(rs, OUTPUT);
    setMODER(rw, OUTPUT);
    setMODER(e, OUTPUT);
    for (int i=0; i<4; i++) {
        enableGPIO(db[i]);
        //setMODER(db[i], OUTPUT);
    }

    _wait();
    _write_cmd(0x22);
    set_ddram_addr(0x00);
    bool correct = ( get_addr().val == 0x00 );
    set_ddram_addr(0x07);
    if (!correct || ( get_addr().val != 0x07 )) {
        _align();
    }

    set_function(false, true, false);
    display_on();
    set_input_mode(true, false);
    clear();

}








void LCD_2x16::clear(void) {
    _write_cmd(0x01);
}


void LCD_2x16::home(void) {
    _write_cmd(0x02);
}


void LCD_2x16::set_line(int line) {
    uint8_t addr = (line > 0) ? 0x40 : 0x00;
    set_ddram_addr(addr);
}


void LCD_2x16::display(bool enable) {
    enable ? display_on() : display_off();
}


void LCD_2x16::display_on(void) {
    display_ctrl_state |= 0x04;
    _write_cmd(0x08 | display_ctrl_state);
}


void LCD_2x16::display_off(void) {
    display_ctrl_state &= 0x03;
    _write_cmd(0x08 | display_ctrl_state);
}


void LCD_2x16::cursor(bool enable, bool blink) {
    if (enable) {
        display_ctrl_state |= 0x02;
    } else {
        display_ctrl_state &= 0x05;
    }
    if (blink) {
        display_ctrl_state |= 0x01;
    } else {
        display_ctrl_state &= 0x06;
    }
    _write_cmd(0x08 | display_ctrl_state);
}







void LCD_2x16::cursor_right() {
    move_cursor(true, false);
}


void LCD_2x16::cursor_left() {
    move_cursor(false, false);
}


void LCD_2x16::scroll_right() {
    move_cursor(true, true);
}


void LCD_2x16::scroll_left() {
    move_cursor(false, true);
}








void LCD_2x16::print(char c) {
    _write_data((uint8_t) c);
}


void LCD_2x16::print(int c) {
    _write_data((uint8_t) c);
}


void LCD_2x16::print(char *str) {
    for (uint32_t i=0; str[i]!='\0'; i++) {
        _write_data((uint8_t) str[i]);
    }
}


void LCD_2x16::print(int line, char *str) {
    set_line(line);
    print(str);
}








void LCD_2x16::set_custom(uint8_t id, uint8_t char_data[8]) {
    if (id < 8) {
        addr_t old_addr = get_addr();
        set_cgram_addr(id << 3);
        for (int i=0; i<8; i++) {
            _write_data(char_data[i]);
        }
        set_addr(old_addr);
    }
}


uint8_t LCD_2x16::get_data() {
    return _read(HIGH);
}


uint8_t LCD_2x16::get_data(LCD_2x16::addr_t addr) {
    addr_t old_addr = get_addr();
    set_addr(addr);
    uint8_t data = _read();
    set_addr(old_addr);
    return data;
}


LCD_2x16::addr_t LCD_2x16::get_addr() {
    uint8_t raw_addr = _read(LOW);
    return {.type = (bool)(raw_addr & 0x40), .val = (uint8_t)(raw_addr & 0x3F)};
}


void LCD_2x16::set_addr(LCD_2x16::addr_t addr) {
    if (addr.type == CGRAM) {
        set_cgram_addr(addr.val);
    } else {
        set_ddram_addr(addr.val);
    }
}


void LCD_2x16::set_cgram_addr(uint8_t addr) {
    addr &= 0x3F;
    addr |= 0x40;
    _write_cmd(addr);
}


void LCD_2x16::set_ddram_addr(uint8_t addr) {
    addr |= 0x80;
    _write_cmd(addr);
}








void LCD_2x16::set_input_mode(bool cursor_dir_right, bool scroll) {
    uint8_t cmd = 0x04;
    if (cursor_dir_right) {
        cmd |= 0x2;
    }
    if (scroll) {
        cmd |= 0x1;
    }
    _write_cmd(cmd);
}


void LCD_2x16::move_cursor(bool move_dir_right, bool move_display) {
    uint8_t cmd = 0x10;
    if (move_dir_right) {
        cmd |= 0x08;
    }
    if (move_display) {
        cmd |= 0x04;
    }
    _write_cmd(cmd);
}

void LCD_2x16::set_function(bool bus_width_8, bool dual_line, bool font_5x11) {
    uint8_t cmd = 0x20;
    if (bus_width_8) {
        cmd |= 0x10;
    }
    if (dual_line) {
        cmd |= 0x08;
    }
    if (font_5x11) {
        cmd |= 0x04;
    }
    _write_cmd(cmd);
}








void LCD_2x16::_write_cmd(uint8_t cmd) {
    _write(LOW, cmd);
    //delay(10);
    _wait();
}


void LCD_2x16::_write_data(uint8_t data) {
    _write(HIGH, data);
}


void LCD_2x16::_write(bool _rs, uint8_t data) {
    for (int i=0; i<4; i++) {
        setMODER(db[i], OUTPUT);
    }
    digitalWrite(rw, LOW);
    digitalWrite(rs, _rs);
    digitalWrite(e, HIGH);
    digitalWrite(db[0], data & 0x10);
    digitalWrite(db[1], data & 0x20);
    digitalWrite(db[2], data & 0x40);
    digitalWrite(db[3], data & 0x80);
    digitalWrite(e, LOW);
    delay(1);
    digitalWrite(e, HIGH);
    digitalWrite(db[0], data & 0x01);
    digitalWrite(db[1], data & 0x02);
    digitalWrite(db[2], data & 0x04);
    digitalWrite(db[3], data & 0x08);
    digitalWrite(e, LOW);
    delay(1);
}


uint8_t LCD_2x16::_read(bool _rs) {
    for (int i=0; i<4; i++) {
        setMODER(db[i], INPUT);
    }
    uint8_t data = 0;
    digitalWrite(rw, HIGH);
    digitalWrite(rs, _rs);
    digitalWrite(e, HIGH);
    data |= digitalRead(db[0]) << 4;
    data |= digitalRead(db[1]) << 5;
    data |= digitalRead(db[2]) << 6;
    data |= digitalRead(db[3]) << 7;
    digitalWrite(e, LOW);
    delay(1);
    digitalWrite(e, HIGH);
    data |= digitalRead(db[0]);
    data |= digitalRead(db[1]) << 1;
    data |= digitalRead(db[2]) << 2;
    data |= digitalRead(db[3]) << 3;
    digitalWrite(e, LOW);
    delay(1);
    return data;
}


void LCD_2x16::_wait(void) {
    while( (0x80 & _read(false)) );
}


void LCD_2x16::_align() {
    for (int i=0; i<4; i++) {
        setMODER(db[i], OUTPUT);
    }
    digitalWrite(rw, LOW);
    digitalWrite(rs, LOW);
    delay(1);
    digitalWrite(e, HIGH);
    delay(1);
    digitalWrite(db[0], LOW);
    digitalWrite(db[1], LOW);
    digitalWrite(db[2], LOW);
    digitalWrite(db[3], LOW);
    delay(1);
    digitalWrite(e, LOW);
    delay(1);
}

