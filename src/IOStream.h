#ifndef __INPUT_STREAM_H
#define __INPUT_STREAM_H

#include <stdint.h>
#include <exception>


struct IOException: public std::exception  {
  const char* what() {
    return "General IO error occured";
  }
};

struct EOFException: public IOException  {
  const char* what() {
    return "Reached end of file or stream";
  }
};


// Abstract InputStream class
class InputStream {

  public:

    virtual uint8_t read() = 0;
    int read(uint8_t *buffer, uint32_t n);
    int read(char *buffer, uint32_t);

};


// Abstract OutputStream class
class OutputStream {

  public:

    virtual void write(uint8_t x) = 0;
    virtual void flush() = 0;
    void write(uint8_t *buffer, uint32_t n);
    void write(char *buffer, uint32_t n);

};


#endif