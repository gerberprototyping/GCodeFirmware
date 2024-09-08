/*
 * serial_iterator.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: Andrew Gerber
 */

#include "serial_iterator.h"

#include "serial.h"

#define BASE    (Serial::rx_buff)


serial_iterator::serial_iterator() {
    ptr = BASE;
}
serial_iterator::serial_iterator(const serial_iterator &it) {
    ptr = it.ptr;
}
serial_iterator::serial_iterator(uint8_t* const p) {
    ptr = p;
}

serial_iterator serial_iterator::volatile_read() volatile {
    uint8_t* _ptr = ptr;
    return serial_iterator(_ptr);
}
void serial_iterator::volatile_write(const serial_iterator &it) volatile {
    ptr = it.ptr;
}


uint8_t* serial_iterator::get_raw() const {
    return ptr;
}

uint32_t serial_iterator::diff(const serial_iterator &lhs, const serial_iterator &rhs) {
    return ((uint32_t) lhs.ptr) - ((uint32_t) rhs.ptr);
}

bool serial_iterator::overflow() const {
    return ptr > BASE + SERIAL_RX_BUFF_SIZE;
}

bool serial_iterator::underflow() const {
    return ptr < BASE;
}


// Arithmetic operators
serial_iterator serial_iterator::operator+(const uint32_t x) const {
    serial_iterator rval = serial_iterator(ptr + x);
    while (rval.overflow()) {
        rval.ptr = rval.ptr - SERIAL_RX_BUFF_SIZE;
    }
    return rval;
}
serial_iterator serial_iterator::operator-(const uint32_t x) const {
    serial_iterator rval = serial_iterator(ptr - x);
    while (rval.underflow()) {
        rval.ptr = rval.ptr + SERIAL_RX_BUFF_SIZE;
    }
    return rval;
}
serial_iterator& serial_iterator::operator+=(const uint32_t x) {
    ptr = ptr + x;
    while (overflow()) {
        ptr = ptr - SERIAL_RX_BUFF_SIZE;
    }
    return *this;
}
serial_iterator& serial_iterator::operator-=(const uint32_t x) {
    ptr = ptr - x;
    while (underflow()) {
        ptr = ptr + SERIAL_RX_BUFF_SIZE;
    }
    return *this;
}


// Incr/Dec operators
serial_iterator& serial_iterator::operator++() { // ++ptr
    ptr++;
    if (overflow()) {
        ptr = BASE;
    }
    return *this;
}
serial_iterator  serial_iterator::operator++(int) { // ptr++
    serial_iterator old = *this;
    operator++();
    return old;
}


// Comparison operators
bool serial_iterator::operator==(const serial_iterator &rhs) const {return ptr == rhs.ptr;}
bool serial_iterator::operator!=(const serial_iterator &rhs) const {return ptr != rhs.ptr;}
bool serial_iterator::operator< (const serial_iterator &rhs) const {return ptr <  rhs.ptr;}
bool serial_iterator::operator> (const serial_iterator &rhs) const {return ptr <  rhs.ptr;}
bool serial_iterator::operator<=(const serial_iterator &rhs) const {return ptr <= rhs.ptr;}
bool serial_iterator::operator>=(const serial_iterator &rhs) const {return ptr >= rhs.ptr;}


// Array subscript operator
uint8_t& serial_iterator::operator[](uint32_t i) {
    serial_iterator p = operator+(i);
    return *p;
}
const uint8_t& serial_iterator::operator[](uint32_t i) const {
    serial_iterator p = operator+(i);
    return *p;
}


// Dereference operator
uint8_t& serial_iterator::operator*() {return *ptr;}
const uint8_t& serial_iterator::operator*() const {return *ptr;}
