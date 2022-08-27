#include "temp.h"


Temp::Temp() {

    RCC->CR |= RCC_CR_HSION;                                        // Turn on High Speed Internal 16 MHz clock
    while( !(RCC->CR & RCC_CR_HSIRDY) );                            // Wait for HSI ready


    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;                              // 1.   Turn on ADC clock
    ADC1->CR &= ~ADC_CR_ADEN;                                       // 2.   Disable ADC1
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;                          // 3.   Enable I/O analog voltage booster
    ADC123_COMMON->CCR |= ADC_CCR_VREFEN;                           // 4.   Enagle conversion of internal channels
    ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;                           // 5.   Disable prescale clock dividing
    ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;                          // 6.   Set synchronous clock mode (HCLK/1)
    ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;
    ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;                            // 7.   Config all ADCs as independent
    wakeADC1();                                                     // 8.   Wake from deep-power-down
    ADC1->CFGR &= ~ADC_CFGR_RES;                                    // 9.   Set resolution to 12-bits
    ADC1->CFGR &= ~ADC_CFGR_ALIGN;                                  // 10.  Select right alignment
    ADC1->SQR1 &= ~ADC_SQR1_L;                                      // 11.  Select 1 conversion in the regular channel conversion sequence
    ADC1->SQR1 &= ~ADC_SQR1_SQ1;                                    // 12.  Select channel 17 as 1st conversion in conversion sequence
    ADC1->SQR1 |= 17 << 6;
    //ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_17;                        // 13.  Set channel 17 to single-ended (channel 17 is forced single-ended)
    ADC1->SMPR2 |= ADC_SMPR2_SMP17;                                 // 14.  Set channel 17 sample time to max
    ADC1->CFGR &= ~ADC_CFGR_CONT;                                   // 15.  Set discontinuous mode
    ADC1->CFGR &= ~ADC_CFGR_EXTEN;                                  // 16.  Select software trigger
    //ADC1->IER |= ADC_IER_EOCIE;                                   //      Enable end of conversion interupt
    ADC123_COMMON->CCR |= 0x1 << 23;                                //      Enable channel 17 and wake temp sensor
    calibrateADC1();
    ADC1->CR |= ADC_CR_ADEN;                                        // 17.  Enable ADC1
    while( !(ADC1->ISR & ADC_ISR_ADRDY) );                          // 18.  Wait for ADC1 ready

}


void Temp::wakeADC1() {
    if ( (ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD) {
        ADC1->CR &= ~ADC_CR_DEEPPWD;
    }
    ADC1->CR |= ADC_CR_ADVREGEN;
    delay(1);
}


void Temp::calibrateADC1() {
    ADC1->CR &= ~ADC_CR_ADCALDIF;                                   // Select singled-ended intput calibration
    ADC1->CR |=  ADC_CR_ADCAL;                                      // Start calibration
    while( (ADC1->CR & ADC_CR_ADCAL) );                             // Wait for calibration to finish
}


float Temp::read() {
    ADC1->CR |= ADC_CR_ADSTART;                                     // Start conversion
    while( !(ADC123_COMMON->CSR & ADC_CSR_EOC_MST) );               // Wait for conversion to complete
    float ts_data = (float)(ADC1->DR & 0x0FFF);                     // 12-bit raw reading
    ts_data = (3.3/3) * ts_data;                                    // Compensate for Vref difference (3.0V factory, 3.3V Board)
    float TS_CAL1 = (float)( *(uint16_t *)(0x1FFF75A8) );           // Factor temp calibration values
    float TS_CAL2 = (float)( *(uint16_t *)(0x1FFF75CA) );
    return ( (110 - 30)/(TS_CAL2 - TS_CAL1) ) * (ts_data - TS_CAL1) + 30;       // Conversion equation from Reference Manual
}
