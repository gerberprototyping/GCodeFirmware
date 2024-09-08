#include "iostream.h"

#include <stdio.h>
#include <stdarg.h>


char printf_buff[256];
void OutputStream::printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(printf_buff, format, args);
    print(printf_buff);
    va_end(args);
}
