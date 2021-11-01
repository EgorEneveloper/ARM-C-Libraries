#include "stm32f10x.h"
#include "SPI.h"

//Important to set manually needed CSN pin as General Push pull

static int temp;

void SPI1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;         //GPIO A Clock ENABLE
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;         //SPI1 Clock Enable
    
    //SETUP PINA5-SCK, PINA6-MISO, PINA7-MOSI
    //Reset the GPIOA pins bits
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_MODE6 | GPIO_CRL_CNF7);
    //Set pins SCK and MOSI Alternate Push Pull, MISO Floating input, 10MHz
    GPIOA->CRL |= (GPIO_CRL_MODE5 | GPIO_CRL_MODE7 | GPIO_CRL_CNF6_0);
    
    //SPI settings: Full duplex, 8bit data, MSB first (+ below)
    SPI1->CR1 = (SPI_CR1_CPOL | SPI_CR1_CPHA |  //Polarity=1 Phase=1
                 SPI_CR1_MSTR |                 //Master mode
                 SPI_CR1_BR_1 | SPI_CR1_BR_0 |  //CLKDIV=16
                 SPI_CR1_SSM | SPI_CR1_SSI);    //Software slave management
    SPI1->CR2 = 0;
    
    SPI1->CR1 |= SPI_CR1_SPE;                   //ENABLE SPI
}
void SPI_Disable(SPI_TypeDef* SPI_Num) {
    SPI_Num->CR1 &= ~SPI_CR1_SPE;
}
uint8_t SPI_SendRecvByte(SPI_TypeDef* SPI_Num, uint8_t data) {
    while(!(SPI_Num->SR & SPI_SR_TXE));         //Wait till empty buffer
    SPI_Num->DR = data;
    while(!(SPI_Num->SR & SPI_SR_TXE));         //Wait till send
    while(SPI_Num->SR & SPI_SR_BSY);            //Wait till SPI not busy
    
    data = SPI_Num->DR & 0xFF;                  //Get Received data
    temp = SPI_Num->SR;                         //Clear flag by reading DR and SR
    
    return data;
}
void SPI_WriteArrw(SPI_TypeDef* SPI_Num, uint8_t* datain, uint8_t len) {
    while (len--) { SPI_SendRecvByte(SPI_Num, *datain++); }
}
void SPI_ReadArrw(SPI_TypeDef* SPI_Num, uint8_t* dataout, uint8_t len) {
    while (len--) { *dataout++ = SPI_SendRecvByte(SPI_Num, 0xFF); }
}
