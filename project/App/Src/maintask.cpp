/*
 * maintask.cpp
 *
 *  Created on: Feb 24, 2024
 *      Author: Andrew Gerber
 */

#include "maintask.h"


#define BUFF_SIZE       32

#define SPI2_DC ((gpio_t) {.port=SPI2_DC_GPIO_Port, .pin=SPI2_DC_Pin})
Adafruit_ST7735 display = Adafruit_ST7735(&SPI2_Handle, SPI2_DC);

char buff[BUFF_SIZE];
// char str[32];


void gfx_test();


extern "C" void StartMainTask(void *argument) {
    builtin_led_set(1);

    // Init USB
    serial.init(RXBuffLockHandle);
    serial.println("USB init complete");

    // Wait for input
    // serial.print("Waiting for input to continue...");
    // serial.discardall();
    // serial.read(); // wait for input
    // serial.write('\n');

    // Init LCD
    display.initR(INITR_BLACKTAB);
    serial.println("LCD init complete");
    gfx_test();


    while (1) {

        // Test serial.read(buff, len)
        // uint32_t len;
        // do {
        //     len = serial.available();
        // } while (!len);
        // if (len > BUFF_SIZE-2) {
        //     len = BUFF_SIZE-2;
        // }
        // serial.read(buff, len);
        // buff[len++] = '\n';
        // buff[len++] = '\r';
        // serial_tx(buff, len);
        // osDelay(250);


        // Test serial.readline(buff, max)
        // uint32_t len = serial.readline(buff, 32);
        // serial.print("\n> ");
        // serial.write(buff, len);
        // serial.println();
        // osDelay(10);

    }

}


float p = 3.1415926;
char lorem_ipsum[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ";
void gfx_test() {
    serial.println("Hello! ST77xx TFT Test");

    uint32_t time = HAL_GetTick();
    display.fillScreen(ST77XX_BLACK);
    time = HAL_GetTick() - time;
    serial.printf("Screen fill took %ld ms\r\n", time);
    delay(500);

    // single pixel
    serial.println("Drawing single pixel");
    display.drawPixel(display.width()/2, display.height()/2, ST77XX_GREEN);
    delay(1000);

    // large block of text
    serial.println("Printing 'Lorem ipsum'");
    display.fillScreen(ST77XX_BLACK);
    display.setCursor(0, 0);
    display.setTextColor(ST77XX_WHITE);
    display.setTextWrap(true);
    display.print(lorem_ipsum);
    delay(3000);

    serial.println("Printing stuff");
    // print function
    display.setTextWrap(false);
    display.fillScreen(ST77XX_BLACK);
    display.setCursor(0, 30);
    display.setTextColor(ST77XX_RED);
    display.setTextSize(1);
    display.println("Hello World!");
    display.setTextColor(ST77XX_YELLOW);
    display.setTextSize(2);
    display.println("Hello World!");
    display.setTextColor(ST77XX_GREEN);
    display.setTextSize(3);
    display.println("Hello World!");
    display.setTextColor(ST77XX_BLUE);
    display.setTextSize(4);
    display.print(1234.567);
    delay(1500);
    display.setCursor(0, 0);
    display.fillScreen(ST77XX_BLACK);
    display.setTextColor(ST77XX_WHITE);
    display.setTextSize(0);
    display.println("Hello World!");
    display.setTextSize(1);
    display.setTextColor(ST77XX_GREEN);
    display.print(p, 6);
    display.println(" Want pi?");
    display.println(" ");
    display.print(8675309, HEX); // print 8,675,309 out in HEX!
    display.println(" Print HEX!");
    display.println(" ");
    display.setTextColor(ST77XX_WHITE);
    display.println("Sketch has been");
    display.println("running for: ");
    display.setTextColor(ST77XX_MAGENTA);
    display.print(millis() / 1000);
    display.setTextColor(ST77XX_WHITE);
    display.print(" seconds.");
    delay(4000);

    // line draw test
    display.fillScreen(ST77XX_BLACK);
    for (int16_t x=0; x < display.width(); x+=6) {
        display.drawLine(0, 0, x, display.height()-1, ST77XX_YELLOW);
        delay(0);
    }
    for (int16_t y=0; y < display.height(); y+=6) {
        display.drawLine(0, 0, display.width()-1, y, ST77XX_YELLOW);
        delay(0);
    }

    display.fillScreen(ST77XX_BLACK);
    for (int16_t x=0; x < display.width(); x+=6) {
        display.drawLine(display.width()-1, 0, x, display.height()-1, ST77XX_YELLOW);
        delay(0);
    }
    for (int16_t y=0; y < display.height(); y+=6) {
        display.drawLine(display.width()-1, 0, 0, y, ST77XX_YELLOW);
        delay(0);
    }

    display.fillScreen(ST77XX_BLACK);
    for (int16_t x=0; x < display.width(); x+=6) {
        display.drawLine(0, display.height()-1, x, 0, ST77XX_YELLOW);
        delay(0);
    }
    for (int16_t y=0; y < display.height(); y+=6) {
        display.drawLine(0, display.height()-1, display.width()-1, y, ST77XX_YELLOW);
        delay(0);
    }

    display.fillScreen(ST77XX_BLACK);
    for (int16_t x=0; x < display.width(); x+=6) {
        display.drawLine(display.width()-1, display.height()-1, x, 0, ST77XX_YELLOW);
        delay(0);
    }
    for (int16_t y=0; y < display.height(); y+=6) {
        display.drawLine(display.width()-1, display.height()-1, 0, y, ST77XX_YELLOW);
        delay(0);
    }
    delay(500);

    serial.println("ST77xx test complete.");

    while (1) {
        display.invertDisplay(true);
        delay(500);
        display.invertDisplay(false);
        delay(500);
    }
}
