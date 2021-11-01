#include "stm32f10x.h"
#include "USART.h"

void USART1_Init(long baud) { 
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;     //GPIO A Clock ENABLE
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;   //USART1 Clock ENABLE
    
    //SETUP PINA9 as TX and PINA10 as RX
    //Reset pin A9 and A10 bits
    GPIOA->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9 | GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
    //Set PINA9 Output Alternate push-pull 10MHz and PINA10 Input Floating
    GPIOA->CRH |= GPIO_CRH_MODE9_0 | GPIO_CRH_CNF9_1 | GPIO_CRH_CNF10_0;
    
    //SETUP BAUDRATE
    USART1->BRR = 72000000L/baud;                   //Set baudrate
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;     //TX1 and RX1 ENABLE
    USART1->CR1 |= USART_CR1_UE;                    //USART1 ENABLE
}
void USART2_Init(long baud) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;     //GPIO A Clock ENABLE
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;   //USART2 Clock ENABLE
    
    //SETUP PINA2 as TX and PINA3 as RX
    //Reset pin A2 and A3 bits
    GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2 | GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
    //Set PINA2 Output Alternate push-pull 10MHz and PINA3 Input Floating
    GPIOA->CRL |= GPIO_CRL_MODE2_0 | GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_0;
    
    //SETUP BAUDRATE
    USART2->BRR = 36000000L/baud;                   //Set baudrate
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;     //TX2 and RX2 ENABLE
    USART2->CR1 |= USART_CR1_UE;                    //USART2 ENABLE
}
void USART3_Init(long baud) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;     //GPIO B Clock ENABLE
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;   //USART3 Clock ENABLE
    
    //SETUP PINB10 as TX and PINB11 as RX
    //Reset pin B10 and B11 bits
    GPIOB->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF11);
    //Set PINB10 Output Alternate push-pull 10MHz and PINB11 Input Floating
    GPIOB->CRH |= GPIO_CRH_MODE10_0 | GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_0;
    
    //SETUP BAUDRATE
    USART3->BRR = 36000000L/baud;                   //Set baudrate
    USART3->CR1 |= USART_CR1_TE | USART_CR1_RE;     //TX3 and RX3 ENABLE
    USART3->CR1 |= USART_CR1_UE;                    //USART3 ENABLE
}
char USART_Available(USART_TypeDef* USART_Num) {
    return (USART_Num->SR & USART_SR_RXNE) ? 1 : 0;
}
char USART_ReadChar(USART_TypeDef* USART_Num) {
    while(!(USART_Num->SR & USART_SR_RXNE));        //Wait till RX buffer not empty
    return USART1->DR & 0xFF;
}
void USART_WriteChar(USART_TypeDef* USART_Num, char data) {
    while(!(USART_Num->SR & USART_SR_TXE));         //Wait till TX buffer is empty
    USART1->DR = data;
}
void USART_WriteNL(USART_TypeDef* USART_Num) {
    USART_WriteChar(USART_Num, 13);
    USART_WriteChar(USART_Num, 10);
}
static void NumOut(USART_TypeDef* USART_Num, long num) {
    if (num) { NumOut(USART_Num, num / 10); USART_WriteChar(USART_Num, (num % 10) + '0'); }
}
void USART_WriteNum(USART_TypeDef* USART_Num, long num) {
    if (num < 0) { USART_WriteChar(USART_Num, '-'); num = -num; }
    if (num) { NumOut(USART_Num, num); return; }
    USART_WriteChar(USART_Num, '0');
}
long USART_ReadNum(USART_TypeDef* USART_Num, char separator, uint8_t maxnums) {
    long num = 0;
    while(maxnums) {
        char nchar = USART_ReadChar(USART_Num);
        if (nchar == separator) { return num; }
        num *= 10;
        num += nchar & 0x0F;
        --maxnums;
    }
    return num;
}
