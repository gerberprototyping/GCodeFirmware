#ifndef __STM32_SPITFT_H
#define __STM32_SPITFT_H

#include "main.h"

#include "Adafruit_GFX.h"
#include "GPIO.h"
#include "serial.h"

// #define SPITFT_DEBUG

#define SPITFT_TIMEOUT          10000
#define SPITFT_COLOR_BUFF_SIZE  1024
// #define SPITFT_USE_DMA


class STM32_SPITFT : public Adafruit_GFX {

    public:

        STM32_SPITFT(uint16_t w, uint16_t h, SPI_HandleTypeDef* spi,
                    gpio_t dc, gpio_t cs = GPIO_NULL);

        // Function overrides from base class
        void startWrite();
        void endWrite();

        // These functions require a chip-select and/or SPI transaction
        // around them.
        void writePixel(int16_t x, int16_t y, uint16_t color);
        void writePixels(uint16_t *colors, uint32_t len, bool block = true, bool bigEndian = false);
        void writeColor(uint16_t color, uint32_t len);
        void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
        void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

        void swapBytes(uint16_t *src, uint32_t len, uint16_t *dest = NULL);

        void drawPixel(int16_t x, int16_t y, uint16_t color);
        void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        void fillScreen(uint16_t color);
        void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
        void drawFastVLine(int16_t x, int16_t y, int16_t w, uint16_t color);

        using Adafruit_GFX::drawRGBBitmap;
        void drawRGBBitmap(int16_t x, int16_t y, uint16_t* colors, int16_t w, int16_t h);

        void invertDisplay(bool i);
        uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
        uint16_t color565(uint32_t rgb);


        // Pure virtual functions
        virtual void begin(uint32_t freq) = 0;
        virtual void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;


        void initSPI(uint32_t freq = 0, uint8_t spiMode = 0);
        void sendCommand(uint8_t cmd, const uint8_t* data = NULL, uint8_t len = 0);

        void writeCommand(uint8_t cmd);

        inline void SPI_WRITE(const uint8_t* data, uint16_t len);
        inline void SPI_WRITE8(uint8_t data);
        inline void SPI_WRITE16(uint16_t data);
        inline void SPI_WRITE32(uint32_t data);

        inline void spiCmdMode();
        inline void spiDataMode();

    protected:

        SPI_HandleTypeDef* _spi;
        gpio_t _dc;
        gpio_t _cs;

        int16_t _xstart = 0;
        int16_t _ystart = 0;
        uint8_t invertOnCommand = 0;
        uint8_t invertOffCommand = 0;

        uint32_t _freq = 0;

        uint16_t color_buff[SPITFT_COLOR_BUFF_SIZE];

};


inline void STM32_SPITFT::SPI_WRITE(const uint8_t* data, uint16_t len) {
    if (!len) {
        return;
    }
    for (uint32_t i=1; i<=3; i++) {
        HAL_SPI_StateTypeDef state = HAL_SPI_GetState(_spi);
        if (HAL_SPI_STATE_READY != state) {
            delay(1);
        } else {
            break;
        }
    }
    delay(100);
    HAL_SPI_StateTypeDef state = HAL_SPI_GetState(_spi);
    if (HAL_SPI_STATE_READY != state) {
        uint32_t error = HAL_SPI_GetError(_spi);
        serial.printf("SPI stuck. state=0x%x  error=0x%lX\r\n", (uint8_t) state, error);
        while (1) {
            builtin_led_set(0);
            delay(3000);
            for (uint32_t i=0; i<(uint8_t)state; i++) {
                builtin_led_set(1);
                delay(500);
                builtin_led_set(0);
                delay(500);
            }
        }
    }
    HAL_SPI_Transmit(_spi, (uint8_t*) data, len, SPITFT_TIMEOUT);
}

inline void STM32_SPITFT::SPI_WRITE8(uint8_t data) {
    SPI_WRITE(&data, sizeof(data));
}

inline void STM32_SPITFT::SPI_WRITE16(uint16_t data) {
    data = __builtin_bswap16(data);
    SPI_WRITE( (uint8_t*) &data, sizeof(data));
}

inline void STM32_SPITFT::SPI_WRITE32(uint32_t data) {
    data = __builtin_bswap32(data);
    SPI_WRITE( (uint8_t*) &data, sizeof(data));
}

inline void STM32_SPITFT::spiCmdMode() { digitalWrite(_dc, 0); }
inline void STM32_SPITFT::spiDataMode() { digitalWrite(_dc, 1); }


// #ifdef SPITFT_USE_DMA
//     #define HAL_SPI_WRITE   HAL_SPI_TRANSMIT
// #else
// #endif


#endif
