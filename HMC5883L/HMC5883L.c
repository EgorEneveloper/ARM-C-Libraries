#include "stm32f10x.h"
#include "I2C.h"
#include "HMC5883L.h"

static short HMC5883L_Measure(I2C_TypeDef* I2C_Num, uint8_t MSB_Register_Axis) {
    uint8_t buffer[2];
    
    I2C_Start(I2C_Num);
    I2C_Address(I2C_Num, HMC5883L_ADDR_WRITE);
    I2C_WriteByte(I2C_Num, MSB_Register_Axis);
    I2C_Start(I2C_Num);
    I2C_ReadBytes(I2C_Num, HMC5883L_ADDR_READ, buffer, 2);
    I2C_Stop(I2C_Num);
    
    return ((short)buffer[0] << 8) | (short)buffer[1];
}
void HMC5883L_Begin(I2C_TypeDef* I2C_Num) {
    I2C_Start(I2C_Num);
    I2C_Address(I2C_Num, HMC5883L_ADDR_WRITE);
    I2C_WriteByte(I2C_Num, 0x00);               //Write start address
    I2C_WriteByte(I2C_Num, 0x70);               //Configure register A as 8-average, 15 Hz default, normal measurement
    I2C_WriteByte(I2C_Num, 0xA0);               //Configure register B for gain
    I2C_WriteByte(I2C_Num, 0x00);               //Configure register Mode to continuous measurement
    I2C_Stop(I2C_Num);
}
void HMC5883L_ReadXZY(I2C_TypeDef* I2C_Num, short* datain) {
    uint8_t buffer[6];
    
    I2C_Start(I2C_Num);
    I2C_Address(I2C_Num, HMC5883L_ADDR_WRITE);
    I2C_WriteByte(I2C_Num, HMC5883L_REGISTER_MSB_X);
    I2C_Start(I2C_Num);
    I2C_ReadBytes(I2C_Num, HMC5883L_ADDR_READ, buffer, 6);
    I2C_Stop(I2C_Num);
    
    datain[0] = ((short)buffer[4] << 8) | (short)buffer[5];
    datain[1] = ((short)buffer[2] << 8) | (short)buffer[3];
    datain[2] = ((short)buffer[0] << 8) | (short)buffer[1];
}
short HMC5883L_ReadX(I2C_TypeDef* I2C_Num) { return HMC5883L_Measure(I2C_Num, HMC5883L_REGISTER_MSB_X); }
short HMC5883L_ReadY(I2C_TypeDef* I2C_Num) { return HMC5883L_Measure(I2C_Num, HMC5883L_REGISTER_MSB_Y); }
short HMC5883L_ReadZ(I2C_TypeDef* I2C_Num) { return HMC5883L_Measure(I2C_Num, HMC5883L_REGISTER_MSB_Z); }
