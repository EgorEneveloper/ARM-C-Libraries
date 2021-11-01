#include "stm32f10x.h"
#include "I2C.h"

static int temp;

void I2C1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;     //GPIO B Clock ENABLE
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;     //I2C1 Clock ENABLE
    
    //SETUP PINB6 as SCL and PINB7 as SDA
    //Reset pin B6 and B7 mode bits
    GPIOB->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_MODE7);
    //Set pins Output Alternate open drain 2MHz
    GPIOB->CRL |= GPIO_CRL_MODE6_1 | GPIO_CRL_CNF6 | GPIO_CRL_MODE7_1 | GPIO_CRL_CNF7;
    
    //SETUP I2C
    I2C1->CR1 |= I2C_CR1_SWRST;     //Software reset I2C
    I2C1->CR1 &= ~I2C_CR1_SWRST;    //Clear SW reset bit
    
    I2C1->CR2 |= 36;                //PCLK1 frequency in MHz
    I2C1->CCR = 180;                //Setup clock control
    I2C1->TRISE = 37;               //Setup rise time
    
    I2C1->CR1 |= I2C_CR1_PE;        //ENABLE I2C1
}
void I2C_Disable(I2C_TypeDef* I2C_Num) {
    I2C_Num->CR1 &= !I2C_CR1_PE;
}
void I2C_Start(I2C_TypeDef* I2C_Num) {
    I2C_Num->CR1 |= I2C_CR1_START | I2C_CR1_ACK;//Generate START condition
    while (!(I2C_Num->SR1 & I2C_SR1_SB));       //Wait till start set
}
void I2C_Stop(I2C_TypeDef* I2C_Num) {
    I2C_Num->CR1 |= I2C_CR1_STOP;               //Generate STOP condition
}
void I2C_WriteByte(I2C_TypeDef* I2C_Num, uint8_t data) {
    while(!(I2C_Num->SR1 & I2C_SR1_TXE));       //Wait till empty buffer
    I2C_Num->DR = data;                         //Send data
    while(!(I2C_Num->SR1 & I2C_SR1_BTF));       //Wait till finish transfer
}
static void I2C_WriteAddr(I2C_TypeDef* I2C_Num, uint8_t device_addr) {
    I2C_Num->DR = device_addr;                  //Send addr
    while(!(I2C_Num->SR1 & I2C_SR1_ADDR));      //Wait till ADDR bit is set
}
void I2C_Address(I2C_TypeDef* I2C_Num, uint8_t device_addr) {
    I2C_WriteAddr(I2C_Num, device_addr);        //Write addr
    temp = I2C_Num->SR1 | I2C_Num->SR2;         //Clear ADDR by reading SR1 and SR2
}
static void I2C_GetData(I2C_TypeDef* I2C_Num, uint8_t* data) {
    while(!(I2C_Num->SR1 & I2C_SR1_RXNE));      //Wait till receive data
    *data = I2C_Num->DR & 0xFF;                 //Write data
}
void I2C_ReadByte(I2C_TypeDef* I2C_Num, uint8_t device_addr, uint8_t* data) {
    I2C_WriteAddr(I2C_Num, device_addr);        //Write addr
    
    I2C_Num->CR1 &= ~I2C_CR1_ACK;               //Clear ACK
    temp = I2C_Num->SR1 | I2C_Num->SR2;         //Clear ADDR by reading SR1 and SR2 
    I2C_Stop(I2C_Num);                          //Send STOP contidion
    I2C_GetData(I2C_Num, data);                 //Get data
}
void I2C_ReadBytes(I2C_TypeDef* I2C_Num, uint8_t device_addr, uint8_t* data, uint16_t len) {
    I2C_WriteAddr(I2C_Num, device_addr);        //Write addr
    temp = I2C_Num->SR1 | I2C_Num->SR2;         //Clear ADDR by reading SR1 and SR2 
    
    while (len > 2) {
        I2C_GetData(I2C_Num, data++);           //Get data
        I2C_Num->CR1 |= I2C_CR1_ACK;            //Set ACK
        --len;
    }
    I2C_GetData(I2C_Num, data++);               //Get data
    I2C_Num->CR1 &= ~I2C_CR1_ACK;               //Clear ACK
    I2C_Stop(I2C_Num);                          //Send STOP contidion
    I2C_GetData(I2C_Num, data);                 //Get data
}
