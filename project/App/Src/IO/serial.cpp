/*
 * serial.cpp
 *
 *  Created on: Feb 25, 2024
 *      Author: Andrew Gerber
 */

#include "serial.h"


Serial serial = Serial();

SERIAL_HandleTypeDef* Serial::channel;
bool Serial::is_init = false;

// RX Buffer
uint8_t Serial::rx_buff[SERIAL_RX_BUFF_SIZE];
volatile serial_iterator Serial::rx_front = serial_iterator();
volatile serial_iterator Serial::rx_back  = serial_iterator();
volatile bool Serial::rx_empty = true;

volatile bool Serial::rx_prev_cr = false;
TaskHandle_t Serial::rx_task = NULL;


void Serial::init(SERIAL_HandleTypeDef* channel) {
    if (!is_init) {
        is_init = true;
        this->channel = channel;
        this->rx_task = xTaskGetCurrentTaskHandle();
        #if defined(SERIAL_USB)
            MX_USB_DEVICE_Init();
        #elif defined (SERIAL_UART)
            // Manually set RxISR of HAL driver and enable RX-Not-Empty interrupt
            channel->RxISR = &serial_rx_callback;
            ATOMIC_SET_BIT(channel->Instance->CR1, USART_CR1_RXNEIE);
            //TODO register RxEventCallback to ensure USART_CR1_RXNEIE is never cleared
        #endif
    }
}


uint32_t Serial::available() const {
    return rx_size();
}


uint8_t Serial::read() {
    // wait for data
    while (rx_empty) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
    // read data
    serial_iterator front = rx_front.volatile_read();
    uint8_t rval = *front;
    // update pointer
    inc_rx_front(front, 1);
    rx_prev_cr = '\r' == rval;
    return rval;
}


void Serial::read(uint8_t* const buff, const uint32_t n) {
    if (n > SERIAL_RX_BUFF_SIZE) {
        buff[0] = '\0';
        return;
    }
    // wait for data
    while (rx_size() < n) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
    // read data
    serial_iterator front = rx_front.volatile_read();
    serial_iterator curr = front;
    for (uint32_t i=0; i<n; i++) {
        buff[i] = *curr;
        ++curr;
    }
    // update pointer
    inc_rx_front(front, n);
    rx_prev_cr = '\r' == buff[n-1];
}


uint32_t Serial::readline(uint8_t* const buff, const uint32_t nmax) {
    serial_iterator front = rx_front.volatile_read();

    // ignore leading \n if previously character was \r
    if (rx_prev_cr) {
        // wait for data
        while (rx_size() < 1) { ulTaskNotifyTake(pdTRUE, portMAX_DELAY); }
        if ('\n' == *front) {
            inc_rx_front(front, 1);
            ++front;
        }
    }

    serial_iterator src = front;
    uint8_t* dest = buff;
    uint32_t n = 0;
    while (n+1 < nmax) { // +1 for next byte
        // wait for next byte
        while (rx_size() < n) { ulTaskNotifyTake(pdTRUE, portMAX_DELAY); }

        // check byte
        *dest = *src;
        if ('\r' == *dest || '\n' == *dest) {
            rx_prev_cr = '\r' == *dest;
            *dest = '\0'; // add null termination char
            break;
        }
        ++n;
        ++src;
        ++dest;

        // avoid lockup
        if (n > SERIAL_RX_BUFF_SIZE) {
            return 0;
        }
    }
    if (n+1 == nmax) {
        // no line ending found
        return 0;
    }
    // update pointer
    inc_rx_front(front, n+1); // +1 for line ending char
    return n; // number of bytes copied, not including null char
}


serial_iterator Serial::peek() {
    return rx_front.volatile_read();
}


void Serial::discard(const uint32_t n) {
	uint32_t avail = available();
    if (n >= avail) {
        discardall();
    } else {
        serial_iterator front = rx_front.volatile_read();
        rx_prev_cr = '\r' == *(front + avail);
        front += n;
        rx_front.volatile_write(front); // guaranteed not to be empty
    }
}

void Serial::discardall() {
    serial_iterator front = rx_front.volatile_read();
    uint32_t avail = available();
    rx_prev_cr = '\r' == *(front + avail);
    inc_rx_front(front, avail);
}


uint32_t Serial::discardline() {
    serial_iterator front = rx_front.volatile_read();

    // ignore leading \n if previous character was \r
    if (rx_prev_cr) {
        // wait for data
        while (rx_size() < 1) { ulTaskNotifyTake(pdTRUE, portMAX_DELAY); }
        if ('\n' == *front) {
            inc_rx_front(front, 1);
            ++front;
        }
    }

    serial_iterator src = front;
    uint32_t n = 0;
    while (n+1 < SERIAL_RX_BUFF_SIZE) { // +1 for next byte
        // wait for next byte
        while (rx_size() < n) { ulTaskNotifyTake(pdTRUE, portMAX_DELAY); }

        // check byte
        uint8_t c = *src;
        if ('\r' == c || '\n' == c) {
            rx_prev_cr = '\r' == c;
            break;
        }
        ++n;
        ++src;
    }
    if (n+1 >= SERIAL_RX_BUFF_SIZE) {
        // no line ending found
        return 0;
    }
    // update pointer
    inc_rx_front(front, n+1); // +1 for line ending char
    return n; // number of bytes skipped, not including line ending
}




void Serial::write(const uint8_t x) {
    serial_tx(this->channel, (uint8_t*) &x, 1);
}


void Serial::write(const uint8_t* const buff, const uint32_t n) {
    serial_tx(this->channel, (uint8_t*) buff, n);
}


void Serial::flush() {
    // do nothing, no tx buffer
}



#if defined(SERIAL_USB)
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
#elif defined(SERIAL_UART)
    void serial_rx_callback(UART_HandleTypeDef *uart) {
        __HAL_UART_SEND_REQ(uart, UART_RXDATA_FLUSH_REQUEST);
        serial_iterator dest = Serial::rx_back.volatile_read();
        if (Serial::rx_space()) {
            *dest = (uint8_t) READ_REG(uart->Instance->RDR);
            Serial::rx_back.volatile_write(++dest);
            Serial::rx_empty = false;
            if (NULL != Serial::rx_task) {
                // Notify RX task that new data is available
                BaseType_t higher_priority_task_woken = pdFALSE;
                vTaskNotifyGiveFromISR(Serial::rx_task, &higher_priority_task_woken);
                // context switch if higher priority task was unblocked
                portYIELD_FROM_ISR(higher_priority_task_woken);
            }
        } else {
            //TODO set overflow error sticky bit
        }
    }
#endif
