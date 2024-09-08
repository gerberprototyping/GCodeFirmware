/*
 * serial_iterator.h
 *
 *  Created on: Feb 26, 2024
 *      Author: Andrew Gerber
 */

#ifndef __SERIAL_ITERATOR_H
#define __SERIAL_ITERATOR_H

#include <stddef.h>
#include <stdint.h>

#include "config.h"


class serial_iterator {

    public:

        serial_iterator();
        serial_iterator(const serial_iterator &it);
        explicit serial_iterator(uint8_t* const p);

        serial_iterator volatile_read() volatile;
        void volatile_write(const serial_iterator &it) volatile;

        uint8_t* get_raw() const;
        static uint32_t diff(const serial_iterator &lhs, const serial_iterator &rhs);

        bool overflow() const;
        bool underflow() const;

        // Arithmetic operators
        serial_iterator operator+(const uint32_t x) const;
        serial_iterator operator-(const uint32_t x) const;
        serial_iterator& operator+=(const uint32_t x);
        serial_iterator& operator-=(const uint32_t x);

        // Incr/Dec operators
        serial_iterator& operator++();// ++ptr
        serial_iterator  operator++(int); // ptr++

        // Comparison operators
        bool operator==(const serial_iterator &rhs) const;
        bool operator!=(const serial_iterator &rhs) const;
        bool operator< (const serial_iterator &rhs) const;
        bool operator> (const serial_iterator &rhs) const;
        bool operator<=(const serial_iterator &rhs) const;
        bool operator>=(const serial_iterator &rhs) const;

        // Array subscript operator
        uint8_t& operator[](uint32_t i);
        const uint8_t& operator[](uint32_t i) const;

        // Dereference operator
        uint8_t& operator*();
        const uint8_t& operator*() const;

    private:

        uint8_t* ptr;

};


#endif // __SERIAL_ITERATOR_H
