/*
 * MPU6050.h
 *
 *  Created on: Mar 16, 2025
 *      Author: 28584
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "main.h"

#define MPU6050_I2CADDR 0x68<<1

typedef struct
{
	int16_t ACCEL_XOUT;
	int16_t ACCEL_YOUT;
	int16_t ACCEL_ZOUT;
	int16_t GYRO_XOUT;
	int16_t GYRO_YOUT;
	int16_t GYRO_ZOUT;
	int16_t Temperature;
}MPU6050_Data_framework;

typedef struct
{
	float ACCEL_XOUT;
	float ACCEL_YOUT;
	float ACCEL_ZOUT;
	float GYRO_XOUT;
	float GYRO_YOUT;
	float GYRO_ZOUT;
	float Temperature;
}MPU6050_float_Data_framework;

void Find_I2C_Device();
void MPU6050_Init();
void configGYRO_and_ACCEL(uint8_t GYRO_RANGE, uint8_t ACCEL_RANGE);
MPU6050_Data_framework getMPU6050RAWData();
MPU6050_float_Data_framework getMPU6050WorkedData();
uint8_t readRegister8(uint8_t reg);
void writeRegister(uint8_t reg, uint8_t value);

enum GYRO_and_ACCEL_RANGE
{
	GYRO250 = 0x00,//+-250
	GYRO500 = 0x08,//+-500
	GYRO1000 = 0x10,//+-1000
	GYRO2000 = 0x18,//+-2000
	ACCEL2g = 0x00,//+-2g
	ACCEL4g = 0x08,	//+-4g
	ACCEL8g	= 0x10,	//+-8g
	ACCEL16g = 0x18//+-16g
};

enum
{
	SMPLRT_DIV = 0x19,
	CONFIG = 0x1A,
	GYRO_CONFIG = 0x1B,
	ACCEL_CONFIG = 0x1C,
	ACCEL_XOUT_H = 0x3B,
	ACCEL_XOUT_L = 0x3C,
	ACCEL_YOUT_H = 0x3D,
	ACCEL_YOUT_L = 0x3E,
	ACCEL_ZOUT_H = 0x3F,
	ACCEL_ZOUT_L = 0x40,
	TEMP_OUT_H = 0x41,
	TEMP_OUT_L = 0x42,
	GYRO_XOUT_H = 0x43,
	GYRO_XOUT_L = 0x44,
	GYRO_YOUT_H = 0x45,
	GYRO_YOUT_L = 0x46,
	GYRO_ZOUT_H = 0x47,
	GYRO_ZOUT_L = 0x48,
	PWR_MGMT_1 = 0x6B,
	PWR_MGMT_2 = 0x6C,
	WHO_AM_I = 0x75,

};

#endif /* INC_MPU6050_H_ */
