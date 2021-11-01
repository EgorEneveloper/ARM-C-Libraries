#include "stm32f10x.h"
#include "utils.h"
#include "ADC.h"

//Important to set reqired ADC pin as Analog Input

void ADC_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;     //ADC1 Clock ENABLE
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;     //GPIO A Clock ENABLE
    
    ADC1->CR1 = ADC_CR1_SCAN;               //Scan mode ENABLE
    ADC1->CR2 = ADC_CR2_CONT;               //Continious conversion
    
    ADC1->SMPR2 = ADC_SMPR2_SMP5_0;         //7.5 samples
    ADC1->SQR1 &= ~ADC_SQR1_L;              //Only 1 channel is used
    
    ADC1->CR2 |= ADC_CR2_ADON;              //ADC1 ENABLE
    _delay_cycles(10000);
}
void ADC_Disable(void) {
    ADC1->CR2 &= !ADC_CR2_ADON;
}
static void ADC_StartConversion(unsigned channel) {
    ADC1->SQR3 = channel;                   //Set channel in ADC sequnce
    ADC1->SR = 0;                           //Clear the status register
    ADC1->CR2 |= ADC_CR2_ADON;              //Start the conversion
}
unsigned ADC_Read(unsigned channel) {
    ADC_StartConversion(channel);
    while(!(ADC1->SR & ADC_SR_EOC));
    return ADC1->DR;
}
