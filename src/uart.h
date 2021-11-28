#ifndef __UART_H
#define __UART_H

#include "Nucleo-L476RG.h"
#include "IOStream.h"

#define DMA_CH_SEL_UART_VAL     0b0010

#define UART1_ALT_FUNC_VAL      7
#define UART1_RX_PIN            A10
#define UART1_TX_PIN            A9
#define UART1_DMA               DMA1
#define UART1_DMA_RX_CH_N       4
#define UART1_DMA_TX_CH_N       5
#define UART1_DMA_RX_CH         DMA1_Channel4
#define UART1_DMA_TX_CH         DMA1_Channel5
#define UART1_DMA_RX_CH_IRQn    DMA1_Channel4_IRQn
#define UART1_DMA_TX_CH_IRQn    DMA1_Channel5_IRQn

#define UART2_ALT_FUNC_VAL      7
#define UART2_RX_PIN            A3
#define UART2_TX_PIN            A2
#define UART1_DMA               DMA1
#define UART2_DMA_RX_CH_N       6
#define UART2_DMA_TX_CH_N       7
#define UART2_DMA_RX_CH         DMA1_Channel6
#define UART2_DMA_TX_CH         DMA1_Channel7
#define UART2_DMA_RX_CH_IRQn    DMA1_Channel6_IRQn
#define UART2_DMA_TX_CH_IRQn    DMA1_Channel7_IRQn

#define UART3_ALT_FUNC_VAL      7
#define UART3_RX_PIN            B11
#define UART3_TX_PIN            B10
#define UART3_DMA               DMA1
#define UART3_DMA_RX_CH_N       3
#define UART3_DMA_TX_CH_N       2
#define UART3_DMA_RX_CH         DMA1_Channel3
#define UART3_DMA_TX_CH         DMA1_Channel2
#define UART3_DMA_RX_CH_IRQn    DMA1_Channel3_IRQn
#define UART3_DMA_TX_CH_IRQn    DMA1_Channel2_IRQn

#define UART4_ALT_FUNC_VAL      8
#define UART4_RX_PIN            A1
#define UART4_TX_PIN            A0
#define UART4_DMA               DMA2
#define UART4_DMA_RX_CH_N       5
#define UART4_DMA_TX_CH_N       3
#define UART4_DMA_RX_CH         DMA2_Channel5
#define UART4_DMA_TX_CH         DMA2_Channel3
#define UART4_DMA_RX_CH_IRQn    DMA2_Channel5_IRQn
#define UART4_DMA_TX_CH_IRQn    DMA2_Channel3_IRQn

#define UART5_ALT_FUNC_VAL      8
#define UART5_RX_PIN            D2
#define UART5_TX_PIN            C12
#define UART5_DMA               DMA2
#define UART5_DMA_RX_CH_N       2
#define UART5_DMA_TX_CH_N       1
#define UART5_DMA_RX_CH         DMA2_Channel2
#define UART5_DMA_TX_CH         DMA2_Channel1
#define UART5_DMA_RX_CH_IRQn    DMA2_Channel2_IRQn
#define UART5_DMA_TX_CH_IRQn    DMA2_Channel1_IRQn

#define RX_INDEX                0
#define TX_INDEX                1

#define BUFFER_SIZE          2048


class UART: public InputStream, public OutputStream {

    public:

        UART(USART_TypeDef *uart_ptr, uint32_t baud_rate);

        uint8_t read();
        int read(char *buffer, uint32_t n);

        void write(uint8_t x);
        void flush();

    private:

        USART_TypeDef *uart;
        uint32_t baud;

        void init();

};


/*class UART_DMA {

    public:

        UART(USART_TypeDef *uart_ptr, uint32_t baud_rate);
        void read(char *buffer, uint32_t n);
        void write(char *buffer, uint32_t n);
        void write(char *str);

    private:

        USART_TypeDef *uart;
        uint32_t baud;

        DMA_TypeDef *dma;
        DMA_Channel_TypeDef *dma_ch[2];

        uint8_t buff[2][RX_BUFFER_SIZE];

        void init();
}*/

#endif