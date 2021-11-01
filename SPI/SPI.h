#pragma once
#include "stm32f10x.h"

void SPI1_Init(void);
void SPI2_Init(void);
void SPI_Disable(SPI_TypeDef* SPI_Num);
void SPI_ReadArrw(SPI_TypeDef* SPI_Num, uint8_t* dataout, uint8_t len);
void SPI_WriteArrw(SPI_TypeDef* SPI_Num, uint8_t* datain, uint8_t len);
uint8_t SPI_SendRecvByte(SPI_TypeDef* SPI_Num, uint8_t data);
