/*
 * serial.cpp
 *
 *  Created on: Feb 25, 2024
 *      Author: Andrew Gerber
 */

#include "serial.h"

#include "usb_device.h"


Serial serial = Serial();

bool Serial::is_init = false;
osMutexId_t Serial::RXBuffLock;
uint8_t Serial::rx_buff[SERIAL_RX_BUFF_SIZE];
volatile serial_iterator Serial::rx_front = serial_iterator();
volatile serial_iterator Serial::rx_back  = serial_iterator();
volatile bool Serial::rx_empty = true;


void Serial::init(osMutexId_t RXBuffLock) {
    if (!is_init) {
        is_init = true;
        MX_USB_DEVICE_Init();
        this->RXBuffLock = RXBuffLock;
    }
}


uint32_t Serial::available() const {
    return rx_size();
}


uint8_t Serial::read() {
    while (rx_empty); // wait for data
    serial_iterator front = rx_front.volatile_read();
    uint8_t rval = *front;
    osMutexAcquire(RXBuffLock, 0);
        serial_iterator back = rx_back.volatile_read();
        ++front;
        rx_front.volatile_write(front);
        if (front == back) {
            rx_empty = true;
        }
    osMutexRelease(RXBuffLock);
    return rval;
}


void Serial::read(uint8_t* const buff, const uint32_t n) {
    if (n <= SERIAL_RX_BUFF_SIZE) { // avoid lockup
        while (available() < n); // wait for data
        serial_iterator front = rx_front.volatile_read();
        serial_iterator src = front;
        for (uint32_t i=0; i<n; i++) {
            buff[i] = *src;
            src++;
        }
        osMutexAcquire(RXBuffLock, 0);
            serial_iterator back = rx_back.volatile_read();
            front += n;
            rx_front.volatile_write(front);
            if (front == back) {
                rx_empty = true;
            }
        osMutexRelease(RXBuffLock);
    }
}


uint32_t Serial::readline(uint8_t* const buff, const uint32_t nmax) {
    serial_iterator front = rx_front.volatile_read();
    serial_iterator src = front;
    uint32_t avail = available();
    uint32_t n = 0;
    uint32_t flush_count = 0;
    while (n < nmax+1) { // +1 because whitespace is not copied to buffer
        if (avail <= n) {
            while (available() <= n); // wait for more data
        }
        if ('\r' == *src) {
            flush_count++;
            if ((available() > flush_count) && ('\n' == src[1])) {
                flush_count++;
            }
            break;
        }
        else if ('\n' == *src) {
            flush_count++;
            break;
        }
        ++src;
        n++;
        flush_count++;
        if (n >= SERIAL_RX_BUFF_SIZE) { // avoid lockup
            return 0;
        }
    }
    if (n != flush_count) {
        // not equal when a line ending was found
        // copy data
        src = front;
        uint8_t* dest = buff;
        for (uint32_t i=0; i<n; i++) {
            *dest = *src;
            ++src;
            ++dest;
        }
        // update values
        osMutexAcquire(RXBuffLock, 0);
            serial_iterator back = rx_back.volatile_read();
            front += flush_count;
            rx_front.volatile_write(front);
            if (front == back) {
                rx_empty = true;
            }
        osMutexRelease(RXBuffLock);
    } else {
        // no line ending found
        n = 0;
    }
    return n;
}


serial_iterator Serial::peek() {
    return rx_front.volatile_read();
}


void Serial::discard(const uint32_t n) {
    if (n >= available()) {
        discardall();
    } else {
        serial_iterator front = rx_front.volatile_read();
        front += n;
        rx_front.volatile_write(front); // guaranteed not to empty, so no mutex needed
    }
}

void Serial::discardall() {
    serial_iterator front = rx_front.volatile_read();
    osMutexAcquire(RXBuffLock, 0);
        front = rx_back.volatile_read();
        rx_front.volatile_write(front);
        rx_empty = true;
    osMutexRelease(RXBuffLock);
}


uint32_t Serial::discardline() {
    uint32_t avail = available();
    serial_iterator front = rx_front.volatile_read();
    serial_iterator src = front;
    uint32_t bytes_flushed = 0;
    while (bytes_flushed < avail) {
        if ('\r' == *src) {
            if ((bytes_flushed+1 < avail) && ('\n' == src[1]) ) {
                ++bytes_flushed;
            }
            break;
        }
        else if ('\n' == *src) {
            break;
        }
        ++src;
        ++bytes_flushed;
    }
    osMutexAcquire(RXBuffLock, 0);
        serial_iterator back = rx_back.volatile_read();
        front += bytes_flushed;
        rx_front.volatile_write(front);
        if (front == back) {
            rx_empty = true;
        }
    osMutexRelease(RXBuffLock);
    return bytes_flushed;
}




void Serial::write(const uint8_t x) {
    serial_tx((uint8_t*) &x, 1);
}


void Serial::write(const uint8_t* const buff, const uint32_t n) {
    serial_tx((uint8_t*) buff, n);
}


void Serial::flush() {
    // do nothing
}



int8_t serial_rx_callback(uint8_t* buff, uint32_t len) {
    #if SERIAL_ECHO
        serial_tx(buff, len);
    #endif
    uint32_t total_count = 0;
    uint8_t* src = buff;
    serial_iterator dest = Serial::rx_back.volatile_read();
    while (total_count < len) {
        // Determine number of bytes to copy this loop
        uint32_t byte_count = len - total_count;
        uint32_t curr_space = Serial::rx_space();
        if (byte_count > curr_space) {
            byte_count = curr_space;
        }
        // Copy bytes
        for (uint32_t i=0; i<byte_count; i++) {
            *dest = *src;
            ++src;
            ++dest;
        }
        // Update values
        total_count += byte_count;
        osMutexAcquire(Serial::RXBuffLock, 0);
            Serial::rx_back.volatile_write( dest );
            Serial::rx_empty = false;
        osMutexRelease(Serial::RXBuffLock);
    }
    return USBD_OK;
}
