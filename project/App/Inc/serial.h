/*
 * serial.h
 *
 *  Created on: Feb 24, 2024
 *      Author: Andrew Gerber
 */

#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdint.h>

#include "config.h"

#include "cmsis_os.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"


#ifdef __cplusplus
extern "C" {
#endif

    int8_t serial_rx_callback(uint8_t* buff, uint32_t len);

    static inline void serial_tx(uint8_t* buff, uint16_t len) {
        if (len) {
            int8_t status;
            do {
                status = CDC_Transmit_FS(buff, len);
            } while (status != USBD_OK);
        }
    }

#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
#include "iostream.h"
#include "serial_iterator.h"


class Serial : public InputStream, public OutputStream {

    public:

        Serial() {}
        void init(osMutexId_t RXBuffLock);

        // InputStream functions
        uint32_t available() const;

        uint8_t read();
        void read(uint8_t* const buff, const uint32_t n);
        uint32_t readline(uint8_t* const buff, const uint32_t nmax);

        serial_iterator peek();

        void discard(const uint32_t n);
        void discardall();
        uint32_t discardline();

        // OutputStream functions
        void write(const uint8_t x);
        void write(const uint8_t* const buff, const uint32_t n);

        void flush();

        friend int8_t serial_rx_callback(uint8_t* buff, uint32_t len);
        friend serial_iterator;

    private:

        static bool is_init;

        // rx buffer
        static osMutexId_t RXBuffLock;
        static uint8_t rx_buff[SERIAL_RX_BUFF_SIZE];
        static volatile serial_iterator rx_front;
        static volatile serial_iterator rx_back;
        static volatile bool rx_empty;

        static inline uint32_t rx_size() {
            osMutexAcquire(RXBuffLock, 0);
                serial_iterator front = rx_front.volatile_read();
                serial_iterator back  = rx_back.volatile_read();
                bool empty = rx_empty;
            osMutexRelease(RXBuffLock);
            uint32_t rval;
            if (back == front) {
                rval = (empty) ? 0 : SERIAL_RX_BUFF_SIZE;
            }
            else if (back < front) {
                rval = serial_iterator::diff((back+SERIAL_RX_BUFF_SIZE), front);
            }
            else {
                rval = serial_iterator::diff(back, front);
            }
            return rval;
        }

        static inline uint32_t rx_space() {
            return SERIAL_RX_BUFF_SIZE - rx_size();
        }

};


extern Serial serial;

#endif // _cplusplus


#endif // __SERIAL_H
