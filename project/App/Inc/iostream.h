#ifndef __INPUT_STREAM_H
#define __INPUT_STREAM_H

#include <stdint.h>

#include "serial_iterator.h"


// Abstract InputStream class
class InputStream {

    public:

        // Pure virtual function
        virtual uint32_t available() const = 0;
        virtual uint8_t read() = 0;
        virtual serial_iterator peek() = 0;

        // Virtual functions with default implementation
        virtual uint32_t read(uint8_t* const buff, const uint32_t n);
        virtual uint32_t readline(uint8_t* const buff, const uint32_t nmax);
        virtual void discard(const uint32_t n);
        virtual void discardall();
        virtual uint32_t discardline();

        // Aliases and variants
        // virtual char read_char() { return (char) read(); }
        // virtual uint32_t readline(char* const buff, const uint32_t nmax) { return readline((uint8_t*)buff, nmax); }
        // virtual uint32_t peekline(const char* buff) const { return peekline((const uint8_t*)buff); }

};


// Abstract OutputStream class
class OutputStream {

    public:

        // Pure virtual functions
        virtual void write(const uint8_t x) = 0;
        virtual void flush() = 0;

        // Virtual functions with default implementation
        virtual void write(const uint8_t* const buff, const uint32_t n);


        // Inline aliases and variants
        inline void print(const char c) { write((uint8_t) c); }
        inline void println() { print('\r'); print('\n'); flush(); }
        inline void println(const char* const str) { print(str); println(); }
        inline void print(const char* const str) {
            uint32_t n=0;
            for (; str[n]!='\0'; n++);
            write((uint8_t*) str, n);
        }

};


#endif
