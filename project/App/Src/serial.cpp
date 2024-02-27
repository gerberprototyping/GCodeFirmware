/*
 * serial.cpp
 *
 *  Created on: Feb 25, 2024
 *      Author: Andrew Gerber
 */

#include "serial.h"

#include "usb_device.h"
#include "builtin.h"


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
    return SERIAL_RX_BUFF_SIZE - rx_size();
}


uint8_t Serial::read() {
    serial_iterator front = rx_front.volatile_read();
    uint8_t rval = 0;
    if (!rx_empty) {
        rval = *front;
        osMutexAcquire(RXBuffLock, 0);
            serial_iterator back = rx_back.volatile_read();
            ++front;
            rx_front.volatile_write(front);
            if (front == back) {
                rx_empty = true;
            }
        osMutexRelease(RXBuffLock);
    }
    return rval;
}


serial_iterator Serial::peek() {
    return rx_front.volatile_read();
}

uint32_t Serial::read(uint8_t* const buff, const uint32_t n) {
    // perform bound checking
    uint32_t byte_cnt = n;
    uint32_t avail = available();
    if (byte_cnt > avail) {
        byte_cnt = avail;
    }
    serial_iterator front = rx_front.volatile_read();
    serial_iterator src = front;
    for (uint32_t i=0; i<byte_cnt; i++) {
        buff[i] = *src;
        src++;
    }
    osMutexAcquire(RXBuffLock, 0);
        serial_iterator back = rx_back.volatile_read();
        front += byte_cnt;
        rx_front.volatile_write(front);
        if (front == back) {
            rx_empty = true;
        }
    osMutexRelease(RXBuffLock);
    return byte_cnt;
}


uint32_t Serial::readline(uint8_t* const buff, const uint32_t nmax) {
    // perform bound checking
    uint32_t max_bytes = nmax;
    uint32_t avail = available();
    if (max_bytes > avail) {
        max_bytes = avail;
    }
    serial_iterator front = rx_front.volatile_read();
    serial_iterator src = front;
    uint8_t* dest = buff;
    uint32_t bytes_read = 0;
    uint32_t bytes_flushed = 0;
    while (bytes_read < max_bytes) {
        if ('\r' == *src) {
            if ((bytes_read+1 < avail) && ('\n' == src[1]) ) {
                ++bytes_flushed;
            }
            break;
        }
        else if ('\n' == *src) {
            break;
        }
        *dest = *src;
        ++src;
        ++dest;
        ++bytes_read;
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
    return bytes_read;
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

}


void Serial::write(const uint8_t* const buff, const uint32_t n) {

}


void Serial::flush() {

}




int8_t serial_rx_callback(uint8_t* buff, uint32_t* len) {
    // uint32_t total_cnt = 0;
    // while (total_cnt < *len) {
    //     // Determine number of bytes to copy this loop
    //     uint32_t byte_cnt = *len - total_cnt;
    //     uint32_t curr_space = Serial::rx_space();
    //     if (byte_cnt > curr_space) {
    //         byte_cnt = curr_space;
    //     }
    //     // Copy bytes
    //     uint8_t* src = buff + total_cnt;
    //     serial_iterator dest = Serial::rx_back.volatile_read();
    //     for (uint32_t i=0; i<byte_cnt; i++) {
    //         *dest = *src;
    //         ++src;
    //         ++dest;
    //     }
    //     // Update values
    //     total_cnt += byte_cnt;
    //     osMutexAcquire(Serial::RXBuffLock, 0);
    //         Serial::rx_back.volatile_write( dest );
    //         Serial::rx_empty = false;
    //     osMutexRelease(Serial::RXBuffLock);
    // }
    return USBD_OK;

    // if (SERIAL_RX_BUFF_SIZE-Serial::rx_size() >= *len) {
    //     serial_tx(buff, *len);
    // }
    // return USBD_OK;
}
