#include "IOStream.h"


int InputStream::read(uint8_t *buffer, uint32_t n) {
  for (uint32_t i=0; i<n; i++) {
    //try {
      buffer[i] = read();
    //} catch (EOFException ex) {
      //return i;
    //}
  }
  return n;
}


int InputStream::read(char *buffer, uint32_t n) {
  for (uint32_t i=0; i<n; i++) {
    //try {
      buffer[i] = (char) read();
    //} catch (EOFException ex) {
      //return i;
    //}
  }
  return n;
}




void OutputStream::write(uint8_t *buffer, uint32_t n) {
  for (uint32_t i=0; i<n; i++) {
    write(buffer[i]);
  }
}

void OutputStream::write(char *buffer, uint32_t n) {
  for (uint32_t i=0; i<n; i++) {
    write((uint8_t) buffer[i]);
  }
}


void OutputStream::print(char *str) {
  for (uint32_t i=0; str[i]!='\0'; i++) {
    write((uint8_t) str[i]);
  }
}

void OutputStream::println(char *str) {
  print(str);
  write('\r');
  write('\n');
}


void OutputStream::println() {
  write('\r');
  write('\n');
}
