#ifndef __INPUT_STREAM_H
#define __INPUT_STREAM_H

#include <stdint.h>

#include "serial_iterator.h"


// InputStream interface class
class InputStream {

    public:

        virtual uint32_t available() const = 0;

        virtual uint8_t read() = 0;
        virtual void read(uint8_t* const buff, const uint32_t n) = 0;
        virtual uint32_t readline(uint8_t* const buff, const uint32_t nmax) = 0;

        virtual serial_iterator peek() = 0;

        virtual void discard(const uint32_t n) = 0;
        virtual void discardall() = 0;
        virtual uint32_t discardline() = 0;

};


// OutputStream interface class
class OutputStream {

    public:

        virtual void write(const uint8_t x) = 0;
        virtual void write(const uint8_t* const buff, const uint32_t n) = 0;

        virtual void flush() = 0;

        virtual void printf(const char* format, ...);

        // Inline aliases and variants
        inline void print(const char c) { // single char literal
            write((uint8_t) c);
            }
        inline void print(const char* str) { // null terminated string
            uint32_t n=0;
            for (; str[n]!='\0'; n++);
            write((uint8_t*) str, n);
        }
        inline void println() { // line ending
            uint8_t buff[2] = {'\r','\n'};
            write((uint8_t*) buff, 2);
            flush();
        }
        inline void println(const char* str) { // null terminated string with line ending
            print(str);
            println();
        }

};


#endif
