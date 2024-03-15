#include "STM32_SPITFT.h"


STM32_SPITFT::STM32_SPITFT(uint16_t w, uint16_t h, SPI_HandleTypeDef* spi,
                            gpio_t dc, gpio_t cs)
    : Adafruit_GFX(w, h)
{
    _spi = spi;
    _dc = dc;
    _cs = cs;
}


void STM32_SPITFT::initSPI(uint32_t freq, uint8_t spiMode) {
    // already initialized by IDE generated code
    _freq = freq;
    endWrite(); // reset CS
    spiDataMode();
}


void STM32_SPITFT::sendCommand(uint8_t cmd, const uint8_t* data, uint8_t len) {
    startWrite();

    spiCmdMode();
    SPI_WRITE8(cmd);
    spiDataMode();
    if (len) {
        SPI_WRITE(data, len);
    }

    endWrite();
}


void STM32_SPITFT::writeCommand(uint8_t cmd) {
    spiCmdMode();
    SPI_WRITE8(cmd);
    spiDataMode();
}




// Adafruit GFX function overrides
void STM32_SPITFT::startWrite() {
    if (_cs != GPIO_NULL) {
        digitalWrite(_cs, 0);
    }
}

void STM32_SPITFT::endWrite() {
    if (_cs != GPIO_NULL) {
        digitalWrite(_cs, 1);
    }
}

void STM32_SPITFT::writePixel(int16_t x, int16_t y, uint16_t color) {
    if ((x >= 0) && (x < _width) && (y >= 0) && (y < _height)) {
        setAddrWindow(x, y, 1, 1);
        SPI_WRITE16(color);
    }
}


void STM32_SPITFT::writePixels(uint16_t *colors, uint32_t len, bool block, bool bigEndian) {
    if (!len) {
        return;
    }
    // avoid parameter-not-used complaints
    (void)block;
    (void)bigEndian;
    SPI_WRITE((uint8_t*) colors, len*sizeof(uint16_t));
}


void STM32_SPITFT::writeColor(uint16_t color, uint32_t len) {
    if (!len) {
        return;
    }
    for (uint32_t i=0; (i<len) && (i<SPITFT_COLOR_BUFF_SIZE); i++) {
        color_buff[i] = color;
    }
    #ifdef SPITFT_DEBUG
        serial.printf("Writing color %d times\r\n", len);
    #endif
    while (true) {
        if (len <= SPITFT_COLOR_BUFF_SIZE) {
            SPI_WRITE((uint8_t*) color_buff, sizeof(color)*len);
            break;
        } else {
            SPI_WRITE((uint8_t*) color_buff, sizeof(color)*SPITFT_COLOR_BUFF_SIZE);
            len -= SPITFT_COLOR_BUFF_SIZE;
        }
        #ifdef SPITFT_DEBUG
            serial.printf("  %d remaining\r", len);
        #endif
    }
    #ifdef SPITFT_DEBUG
        serial.println("  done");
    #endif
}


void STM32_SPITFT::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (w && h) {   // nonzero width and height
        if (w < 0) {    // if negative width
            x += w + 1;
            w = -1;
        }
        if (x < _width) {   // not off right
            if (h < 0) {    // if negative height
                y += h + 1;
                h = -h;
            }
            if (y < _height) {  // not off bottom
                int16_t x2 = x + w - 1;
                if (x2 >= 0) {  // not off left
                    int16_t y2 = y + h - 1;
                    if (y2 >= 0) {  // not off top
                        // Rectangle partly or fully overlaps screen
                        if (x < 0) {    // clip left
                            x = 0;
                            w = x2 + 1;
                        }
                        if (y < 0) {    // clip top
                            y = 0;
                            h = y2 + 1;
                        }
                        if (x2 >= _width) {     // clip right
                            w = _width - x;
                        }
                        if (y2 >= _height) {    // clip bottom
                            h = _height - y;
                        }
                        setAddrWindow(x, y, w, h);
                        writeColor(color, (uint32_t)w * h);
                    }
                }
            }
        }
    }
}


void STM32_SPITFT::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    if ((y >= 0) && (y < _height) && w) {   // Y on screen, nonzero width
        if (w < 0) {    // if negative width
            x += w + 1;
        }
        if (x < _width) {   // not off right
            int16_t x2 = x + w - 1;
            if (x2 >= 0) {  // not off left
                // Line partly or fully overlaps screen
                if (x < 0) {    // clip left
                    x = 0;
                    w = x2 + 1;
                }
                if (x2 >= _width) { // clip right
                    w = _width - x;
                }
                setAddrWindow(x, y, w, 1);
                writeColor(color, (uint32_t)w);
            }
        }
    }
}


void STM32_SPITFT::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    if ((x >= 0) && (x < _width) && h) {    // X on screen, nonzero height
        if (h < 0) {    // if negative height
            y += h + 1;
            h = -h;
        }
        if (y < _height) {  // not off bottom
            int16_t y2 = y + h - 1;
            if (y2 >= 0) {  // not off top
                // Line partly or fully overlaps screen
                if (y < 0) {    // clip top
                    y = 0;
                    h = y2 + 1;
                }
                if (y2 >= _height) {    // clip bottom
                    h = _height - y;
                }
                setAddrWindow(x, y, 1, h);
                writeColor(color, (uint32_t)h);
            }
        }
    }
}


void STM32_SPITFT::swapBytes(uint16_t *src, uint32_t len, uint16_t* dest) {
    if (!dest) {
        dest = src;
    }
    for (uint32_t i=0; i< len; i++) {
        dest[i] = __builtin_bswap16(src[i]);
    }
}


void STM32_SPITFT::drawPixel(int16_t x, int16_t y, uint16_t color) {
    // Clip first...
    if ((x >= 0) && (x < _width) && (y >= 0) && (y < _height)) {
        startWrite();
        setAddrWindow(x, y, 1, 1);
        SPI_WRITE16(color);
        endWrite();
    }
}


void STM32_SPITFT::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    startWrite();
    writeFillRect(x, y, w, h, color);
    endWrite();
}


void STM32_SPITFT::fillScreen(uint16_t color) {
    startWrite();
    #ifdef SPITFT_DEBUG
        serial.print("Fill screen...Setting window...");
    #endif
    setAddrWindow(0, 0, _width, _height);
    #ifdef SPITFT_DEBUG
        serial.println("Writing color...  ");
    #endif
    writeColor(color, (uint32_t)_width * _height);
    #ifdef SPITFT_DEBUG
        serial.println("done");
    #endif
    endWrite();
}


void STM32_SPITFT::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    startWrite();
    writeFastHLine(x, y, w, color);
    endWrite();
}


void STM32_SPITFT::drawFastVLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    startWrite();
    writeFastVLine(x, y, w, color);
    endWrite();
}


void STM32_SPITFT::drawRGBBitmap(int16_t x, int16_t y, uint16_t* colors, int16_t w, int16_t h) {
    int16_t x2, y2; // Lower-right coord
    if (    (x >= _width) ||            // Off-edge right
            (y >= _height) ||           // " top
            ((x2 = (x + w - 1)) < 0) || // " left
            ((y2 = (y + h - 1)) < 0))   // " bottom
        return;

    int16_t bx1 = 0, by1 = 0, // Clipped top-left within bitmap
        saveW = w;            // Save original bitmap width value
    if (x < 0) {              // Clip left
        w += x;
        bx1 = -x;
        x = 0;
    }
    if (y < 0) { // Clip top
        h += y;
        by1 = -y;
        y = 0;
    }
    if (x2 >= _width)
        w = _width - x; // Clip right
    if (y2 >= _height)
        h = _height - y; // Clip bottom

    colors += by1 * saveW + bx1; // Offset bitmap ptr to clipped top-left
    startWrite();
    setAddrWindow(x, y, w, h); // Clipped area
    while (h--) {              // For each (clipped) scanline...
        writePixels(colors, w);     // Push one (clipped) row
        colors += saveW;            // Advance pointer by one full (unclipped) line
    }
    endWrite();
}


void STM32_SPITFT::invertDisplay(bool i) {
    startWrite();
    writeCommand( (i) ? invertOnCommand : invertOffCommand);
    endWrite();
}


uint16_t STM32_SPITFT::color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC)) << 3 | (b >> 3);
}


uint16_t STM32_SPITFT::color565(uint32_t rgb) {
    return ((rgb >> 8) & 0xF800) | ((rgb >> 5) & 0x7E0) | ((rgb >> 3) & 0x1F);
}
