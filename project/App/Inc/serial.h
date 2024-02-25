/*
 * serial.h
 *
 *  Created on: Feb 24, 2024
 *      Author: Andrew Gerber
 */

#ifndef __SERIAL_H
#define __SERIAL_H
#ifdef __cplusplus
extern "C" {
#endif

#include "usbd_cdc_if.h"


static inline uint8_t serial_tx(uint8_t* buff, uint16_t len) {
    return CDC_Transmit_FS(buff, len);
}


static inline void serial_rx_callback(uint8_t* buff, uint32_t *len) {
    serial_tx(buff, *len);
}


#ifdef __cplusplus
}
#endif
#endif /* __SERIAL_H */
