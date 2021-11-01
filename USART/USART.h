#pragma once
#include "stm32f10x.h"

void USART1_Init(long baud);
void USART2_Init(long baud);
void USART3_Init(long baud);
char USART_Available(USART_TypeDef* USART_Num);
char USART_ReadChar(USART_TypeDef* USART_Num);
void USART_WriteChar(USART_TypeDef* USART_Num, char data);
void USART_WriteNL(USART_TypeDef* USART_Num);
void USART_WriteNum(USART_TypeDef* USART_Num, long num);
long USART_ReadNum(USART_TypeDef* USART_Num, char separator, uint8_t maxnums);
