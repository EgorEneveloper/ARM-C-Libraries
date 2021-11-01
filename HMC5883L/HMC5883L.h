#pragma once
#include "stm32f10x.h"

#define HMC5883L_ADDR_READ			0x3D
#define HMC5883L_ADDR_WRITE			0x3C

#define HMC5883L_REGISTER_STATUS	0x09
#define HMC5883L_REGISTER_MSB_X		0x03
#define HMC5883L_REGISTER_MSB_Z		0x05
#define HMC5883L_REGISTER_MSB_Y		0x08
#define HMC5883L_REGISTER_IDENTY	0x0A

void HMC5883L_Begin(I2C_TypeDef* I2C_Num);
short HMC5883L_ReadX(I2C_TypeDef* I2C_Num);
short HMC5883L_ReadY(I2C_TypeDef* I2C_Num);
short HMC5883L_ReadZ(I2C_TypeDef* I2C_Num);
void HMC5883L_ReadXZY(I2C_TypeDef* I2C_Num, short* datain); //data order: X -> Z -> Y
