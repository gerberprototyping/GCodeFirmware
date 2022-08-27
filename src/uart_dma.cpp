#include "uart.h"


UART_DMA::UART_DMA(USART_TypeDef *uart_ptr, uint32_t baud_rate) {
    uart = uart_ptr;
    baud = baud_rate;
    init();
}


void UART_DMA::init() {
    RCC->CR |= RCC_CR_HSION;                                    // Turn on High Speed Internal 16 MHz clock
    while( !(RCC->CR & RCC_CR_HSIRDY) );                        // Wait for HSI ready

    uint8_t uart_n;
    gpio_t pin[2];
    uint8_t af_val;
    uint8_t dma_ch_n[2];
    IRQn_Type dma_ch_IRQn[2];
    if (uart == USART1) {
        RCC->APB2ENR &= RCC_APB2ENR_USART1EN;                   // Enable USART1 clock
        uart_n = 1;
        dma = UART1_DMA;
        dma_ch_n[0] = UART1_DMA_RX_CH_N;
        dma_ch_n[1] = UART1_DMA_TX_CH_N;
        dma_ch[0] = UART1_DMA_RX_CH;
        dma_ch[1] = UART1_DMA_TX_CH;
        dma_ch_IRQn[0] = UART1_DMA_RX_CH;
        dma_ch_IRQn[0] = UART1_DMA_TX_CH;
        pin[0] = UART1_RX_PIN;
        pin[1] = UART1_TX_PIN;
        af_val = UART1_ALT_FUNC_VAL;
    }
    else if (uart == USART2) {
        RCC->APB1ENR1 &= RCC_APB1ENR1_USART2EN;                 // Enable USART2 clock
        uart_n = 2;
        dma = UART2_DMA;
        dma_ch_n[0] = UART2_DMA_RX_CH_N;
        dma_ch_n[1] = UART2_DMA_TX_CH_N;
        dma_ch[0] = UART2_DMA_RX_CH;
        dma_ch[1] = UART2_DMA_TX_CH;
        dma_ch_IRQn[0] = UART2_DMA_RX_CH;
        dma_ch_IRQn[0] = UART2_DMA_TX_CH;
        pin[0] = UART2_RX_PIN;
        pin[1] = UART2_TX_PIN;
        af_val = UART2_ALT_FUNC_VAL
    }
    else if (uart == USART3) {
        RCC->APB1ENR1 &= RCC_APB1ENR1_USART3EN;                 // Enable USART3 clock
        uart_n = 3;
        dma = UART3_DMA;
        dma_ch_n[0] = UART3_DMA_RX_CH_N;
        dma_ch_n[1] = UART3_DMA_TX_CH_N;
        dma_ch[0] = UART3_DMA_RX_CH;
        dma_ch[1] = UART3_DMA_TX_CH;
        dma_ch_IRQn[0] = UART3_DMA_RX_CH;
        dma_ch_IRQn[0] = UART3_DMA_TX_CH;
        pin[0] = UART3_RX_PIN;
        pin[1] = UART3_TX_PIN;
        af_val = UART3_ALT_FUNC_VAL
    }
    else if (uart == UART4) {
        RCC->APB1ENR1 &= RCC_APB1ENR1_UART4EN;                  // Enable USART4 clock
        uart_n = 4;
        dma = UART4_DMA;
        dma_ch_n[0] = UART4_DMA_RX_CH_N;
        dma_ch_n[1] = UART4_DMA_TX_CH_N;
        dma_ch[0] = UART4_DMA_RX_CH;
        dma_ch[1] = UART4_DMA_TX_CH;
        dma_ch_IRQn[0] = UART4_DMA_RX_CH;
        dma_ch_IRQn[0] = UART4_DMA_TX_CH;
        pin[0] = UART4_RX_PIN;
        pin[1] = UART4_TX_PIN;
        af_val = UART4_ALT_FUNC_VAL
    }
    else if (uart == UART5) {
        RCC->APB1ENR1 &= RCC_APB1ENR1_UART5EN;                  // Enable USART5 clock
        uart_n = 5;
        dma = UART1_DMA;
        dma_ch_n[0] = UART5_DMA_RX_CH_N;
        dma_ch_n[1] = UART5_DMA_TX_CH_N;
        dma_ch[0] = UART5_DMA_RX_CH;
        dma_ch[1] = UART5_DMA_TX_CH;
        dma_ch_IRQn[0] = UART5_DMA_RX_CH;
        dma_ch_IRQn[0] = UART5_DMA_TX_CH;
        pin[0] = UART5_RX_PIN;
        pin[1] = UART5_TX_PIN;
        af_val = UART5_ALT_FUNC_VAL
    }
    else {
            return;
    }

    // Configure GPIO pins
    for (int i=0; i<2; i++) {
        enableGPIO(pin[i]);
        setMODER(pin[i], ALT_FUNC);
        setAFR(pin[i], af_val);
        setOSPEEDR(pin[i], HIGH_SPEED);
        setPUPDR(pin[i], PULLUP);
        setOTYPER(pin[i], OPEN_DRAIN);
    }

    // Configure UART
    RCC->CCIPR &= ~(0b11 << (uart_n*2));
    RCC->CCIPR |= 0b10 << (uart_n*2);                           // Set UART clock source to HSI16
    uart->CR1 &= ~USART_CR1_UE;                                 // Disable UART
    uart->CR1 &= ~USART_CR1_OVER8;                              // Oversample by 16
    uart->CR1 &= ~USART_CR1_M;                                  // Set 1 start bit, 8 data bits
    uart->CR1 &= ~USART_CR1_PCE;                                // Disable parity bit
    uart->CR1 |= USART_CR1_TE | USART_CR1_RE;                   // Enable TX
    uart->CR2 &= ~USART_CR2_ABREN;                              // Disable auto baud rate
    uart->CR2 &= ~(USART_CR2_STOP_0 | USART_CR2_STOP_1);        // Set stop bits to 1
    uart->CR2 &= ~USART_CR2_CLKEN;                              // Disable clock pin
    uart->CR3 &= ~USART_CR3_CTSE;                               // Disable hardware flow control
    uart->BRR &= 0;
    uart->BRR |= IN_CLOCK_SPEED / baud;                         // Set baud rate
    uart->GTPR &= ~USART_GTPR_PSC;
    uart->GTPR |= 1;                                            // Set prescaler value
    uart->CR1 |= USART_CR1_UE;                                  // Turn on UART
    while ( (uart->ISR & USART_ISR_TEACK) == 0 );               // Wait for tx ready
    while ( (uart->ISR & USART_ISR_REACK) == 0 );               // Wait for rx ready

    // Configure DMA
    RCC->AHB1ENR |= (DMA1 == dma) ? RCC_AHB1ENR_DMA1EN : RCC_AHB1ENR_DMA2EN;        // Enable DMA clock
    for (int i=0; i<2; i++) {
        dma_ch[i]->CCR &= ~DMA_CCR_EN;                          // Disable DMA channel
        dma_ch[i]->CCR &= ~DMA_CCR_PSIZE;                       // Peripheral data size: 00 = 8 bits
        dma_ch[i]->CCR &= ~DMA_CCR_MSIZE;                       // Memory data size: 00 = 8 bits
        dma_ch[i]->CCR &= ~DMA_CCR_PINC                         // Disable peripheral increment mode
        dma_ch[i]->CCR |=  DMA_CCR_MINC                         // Enable memory increment mode
        if (i = RX_INDEX) {     // rx channel
            dma_ch[i]->CCR &= ~DMA_CCR_DIR;                     // Transfer direction: to memory
            NVIC_EnableIRQ(dma_ch_IRQn[i]);                     // Enable NVIC interrupt
            dma_ch[i]->CCR |=  DMA_CCR_TCIE;                    // Enable transfer complete interrupt
        } else {                // tx channel
            dma_ch[i]->CCR |=  DMA_CCR_DIR;                     // Transfer direction: to peripheral
            dma_ch[i]->CCR &= ~DMA_CCR_TCIE;                    // Disable transfer complete interrupt
        }
        dma_ch[i]->CCR = BUFFER_SIZE                            // Number of transfers to complete
        dma_ch[i]->CCR = &(uart->RDR)                           // Peripheral address
        dma_ch[i]->CCR = buff[i]                                // Receive buffer address
        dma->CSELR &= ~( 0x0F << (dma_ch_n[i]-1) );
        dma->CSELR |= DMA_CH_SEL_UART_VAL << (dma_ch_n[i]-1)    // Map channel to UART
        dma_ch[i]->CCR &= ~DMA_CCR_HTIE;                        // Disable half tranfer interrupt
        if (i = RX_INDEX) {
            dma_ch[i]->CCR |=  DMA_CCR_EN;                      // Enable channel
        }
    }    

}


void UART_DMA::read(uint8_t *buffer, uint32_t n) {
    //TODO
}


void UART_DMA::write(uint8_t *buffer, uint32_t n) {
    //TODO
}

void UART_DMA::print(char *str) {
    uint32_t n = 0;
    while (str[n] != '\0') n++;
    write((uint8_t*)str, n);
}

void UART_DMA::flush() {
    //TODO
}
