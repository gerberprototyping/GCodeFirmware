#include "iostream.h"


uint32_t InputStream::read(uint8_t* const buff, const uint32_t n) {
    for (uint32_t i=0; i<n; i++) {
        //try {
            buff[i] = read();
        //} catch (EOFException ex) {
            //return i;
        //}
    }
    return n;
}


uint32_t InputStream::readline(uint8_t* const buff, const uint32_t nmax) {
    uint32_t n = 0;
    while (n < nmax) {
        uint8_t x = read();
        if ('\r' == x) {
            if ('\n' == *peek()) {
                read(); // discard
            }
            break;
        }
        else if ('\n' == x) {
            break;
        }
        buff[n] = x;
        n++;
    }
    return n;
}


void InputStream::discard(const uint32_t n) {
    for (uint32_t i=0; i<n; i++) {
        read(); // discard
    }
}


void InputStream::discardall() {
    uint32_t n = available();
    for (uint32_t i=0; i<n; i++) {
        read(); //discard
    }
}


uint32_t InputStream::discardline() {
    uint32_t n = 0;
    while (true) {
        uint8_t x = read();
        if ('\r' == x) {
            if ('\n' == *peek()) {
                read();
                n++;
            }
            n++;
            break;
        }
        else if ('\n' == x) {
            n++;
            break;
        }
        n++;
    }
    return n;
}




void OutputStream::write(const uint8_t* const buff, const uint32_t n) {
    for (uint32_t i=0; i<n; i++) {
        //try {
            write(buff[i]);
        //} catch (EOFException ex) {
            //return i;
        //}
    }
}
