#pragma once
#include "stm32f10x.h"

void I2C1_Init(void);
void I2C2_Init(void);
void I2C_Disable(I2C_TypeDef* I2C_Num);
void I2C_Start(I2C_TypeDef* I2C_Num);
void I2C_Stop(I2C_TypeDef* I2C_Num);
void I2C_WriteByte(I2C_TypeDef* I2C_Num, uint8_t data);
void I2C_Address(I2C_TypeDef* I2C_Num, uint8_t device_addr);
void I2C_ReadByte(I2C_TypeDef* I2C_Num, uint8_t device_addr, uint8_t* data);
void I2C_ReadBytes(I2C_TypeDef* I2C_Num, uint8_t device_addr, uint8_t* data, uint16_t len);
