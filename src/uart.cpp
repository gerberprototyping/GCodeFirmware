#include "uart.h"

#define IN_CLOCK_SPEED      16000000


USART_TypeDef* UART::uart;
uint32_t UART::baud;

uint8_t* volatile UART::rx_front;
uint8_t* volatile UART::rx_back;
volatile bool UART::rx_empty;
uint8_t* volatile UART::tx_front;
uint8_t* volatile UART::tx_back;
volatile bool UART::tx_empty;

uint8_t UART::rx_buff[UART_RX_BUFF_SIZE];
uint8_t UART::tx_buff[UART_TX_BUFF_SIZE];


UART::UART(USART_TypeDef *uart_ptr, uint32_t baud_rate) {
    uart = uart_ptr;
    baud = baud_rate;
    init();
}


void UART::init() {
    rx_front = rx_buff;
    rx_back = rx_buff;
    rx_empty = true;
    tx_front = tx_buff;
    tx_back = tx_buff;
    tx_empty = true;

    RCC->CR |= RCC_CR_HSION;                                        // Turn on High Speed Internal 16 MHz clock
    while( !(RCC->CR & RCC_CR_HSIRDY) );                            // Wait for HSI ready

    if (uart == USART1) {
        RCC->APB2ENR &= RCC_APB2ENR_USART1EN;
    }
    else if (uart == USART2) {
        enableGPIOA();
        setMODER(A2, ALT_FUNC);
        setMODER(A3, ALT_FUNC);
        GPIOA->AFR[0] |= 0x77 << (4*2);                         // Set alternate function 7 on both pins
        GPIOA->OSPEEDR |= 0x0F << (2*2);                        // Set ospeed to High Speed!
        setPUPDR(A2, PULLUP);
        setPUPDR(A3,PULLUP);
        setOTYPER(A2, OPEN_DRAIN);
        setOTYPER(A3, OPEN_DRAIN);
        RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;                 // Enable USART2 clock
        RCC->CCIPR &= ~(RCC_CCIPR_USART2SEL);
        RCC->CCIPR |= RCC_CCIPR_USART2SEL_1;                    // Set USART2 clock source to HSI16
    }
    else if (uart == USART3) {
        RCC->APB1ENR1 &= RCC_APB1ENR1_USART3EN;
    }
    else if (uart == UART4) {
            RCC->APB1ENR1 &= RCC_APB1ENR1_UART4EN;
    }
    else if (uart == UART5) {
        RCC->APB1ENR1 &= RCC_APB1ENR1_UART5EN;
    }
    else {
            return;
    }

    uart->CR1 &= ~USART_CR1_UE;                                 // Disable UART

    uart->CR1 &= ~USART_CR1_OVER8;                              // Oversample by 16
    uart->CR1 &= ~USART_CR1_M;                                  // Set 1 start bit, 8 data bits
    uart->CR1 &= ~USART_CR1_PCE;                                // Disable parity bit
    uart->CR1 |=  USART_CR1_TE | USART_CR1_RE;                  // Enable TX & RX
    uart->CR1 |=  USART_CR1_RXNEIE;                             // RX not empty interrupts
    uart->CR2 &= ~USART_CR2_ABREN;                              // Disable auto baud rate
    uart->CR2 &= ~(USART_CR2_STOP_0 | USART_CR2_STOP_1);        // Set stop bits to 1
    uart->CR2 &= ~USART_CR2_CLKEN;                              // Disable clock pin
    uart->CR3 &= ~USART_CR3_CTSE;                               // Disable hardware flow control
    uart->BRR  =  0;
    uart->BRR |=  IN_CLOCK_SPEED / baud;                        // Set baud rate
    uart->GTPR &= ~USART_GTPR_PSC;
    uart->GTPR |= 1;                                            // Set prescaler value

    uart->CR1 |= USART_CR1_UE;                                  // Turn on UART
    while ( (uart->ISR & USART_ISR_TEACK) == 0 );               // Wait for tx ready
    while ( (uart->ISR & USART_ISR_REACK) == 0 );               // Wait for rx ready
}


uint8_t UART::read() {
    // while(!(uart->ISR & USART_ISR_RXNE));
    // uint8_t c = uart->RDR;
    // #if UART_ECHO
    //     //write(c);
    //     //if (c == '\r') {
    //     //    write('\n');
    //     //}
    // #endif
    // return c;
    while (rx_empty);                                           // wait for data in rx buff
    uint8_t c = *rx_front;
    uint8_t* ptr = rx_front + 1;
    if (ptr >= rx_buff+UART_RX_BUFF_SIZE) {
        ptr = rx_buff;
    }
    clear_rxneie();
    rx_front = ptr;
    if (rx_front == rx_back) {
        rx_empty = true;
    }
    set_rxneie();
    return c;
}


void UART::write(uint8_t x) {
    // while(!(uart->ISR & USART_ISR_TXE));                        // Wait for tx data reg to clear
    // uart->TDR = x;
    while (!tx_empty && tx_front == tx_back);                   // wait for space in tx buff
    *tx_back = x;
    uint8_t* ptr = tx_back + 1;
    if (ptr >= tx_buff+UART_TX_BUFF_SIZE) {
        ptr = tx_buff;
    }
    clear_txeie();
    tx_back = ptr;
    tx_empty = false;
    set_txeie();
}


void UART::flush() {
    while (!(uart->ISR & USART_ISR_TC));                        // Wait for tx complete (TC)
    uart->ICR |= USART_ICR_TCCF;                                // Clear TC flag
}


extern "C" void USART2_IRQHandler() {
    uint32_t ISR = UART::uart->ISR;
    if (ISR & USART_ISR_RXNE) {
        *UART::rx_back = UART::uart->RDR;
        ++UART::rx_back;
        if (UART::rx_back >= UART::rx_buff+UART_RX_BUFF_SIZE) {
            UART::rx_back = UART::rx_buff;
        }
        UART::rx_empty = false;
    }
    if (ISR & USART_ISR_TXE) {
        UART::uart->TDR = *UART::tx_front;
        ++UART::tx_front;
        if (UART::tx_front >= UART::tx_buff+UART_TX_BUFF_SIZE) {
            UART::tx_front = UART::tx_buff;
        }
        if (UART::tx_front == UART::tx_back) {
            UART::tx_empty = true;
            UART::clear_txeie();
        }
    }
    // RXNE and TXE interrupt bits cleared be read or write respectively
}
