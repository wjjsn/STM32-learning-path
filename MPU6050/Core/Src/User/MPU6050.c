/*
 * MPU6050.c
 *
 *  Created on: Mar 16, 2025
 *      Author: 28584
 */

#include "MPU6050.h"

/*
 * 显示总线上的所有设备的地址到OLED屏幕上（未左移）
 * */
void Find_I2C_Device()
{
	for (uint8_t addr = 0x08; addr <= 0x77; addr++)
    {
	// 将7位地址左移1位，转换为HAL库所需的格式（写模式）
	HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(&hi2c1, addr << 1, 3, 10);
	if (status == HAL_OK)
		{
			char buffer[50];
			sprintf(buffer, "Found On:0x%x", addr);
			OLED_showString(0,0,buffer,8);
		}
		HAL_Delay(20); // 短延时避免总线冲突
	}
}

void MPU6050_Init()
{
	//writeRegister(PWR_MGMT_1, 0x80);//重置
	while( readRegister8(PWR_MGMT_1)>>7 )//等待重置完成
	writeRegister(PWR_MGMT_1, 0x01);//关闭睡眠模式，不启用循环模式，不禁用温度传感器，时钟源来自x轴陀螺仪
	writeRegister(PWR_MGMT_2, 0x00);//不设置唤醒频率，不设置待机
	writeRegister(SMPLRT_DIV, 0x00);//采样率分频：对数据输出速率进行分频（减速）
	writeRegister(CONFIG, 0x06);//配置外部同步和低通滤波
	configGYRO_and_ACCEL(GYRO2000, ACCEL2g);//配置量程
}

void configGYRO_and_ACCEL(uint8_t GYRO_RANGE, uint8_t ACCEL_RANGE)
{
	writeRegister(GYRO_CONFIG, GYRO_RANGE);
	writeRegister(ACCEL_CONFIG, ACCEL_RANGE);
}

MPU6050_Data_framework getMPU6050RAWData()
{
	MPU6050_Data_framework MPU6050_Data;
	MPU6050_Data.ACCEL_XOUT = readRegister8(ACCEL_XOUT_H)<<8 | readRegister8(ACCEL_XOUT_L);
	MPU6050_Data.ACCEL_YOUT = readRegister8(ACCEL_YOUT_H)<<8 | readRegister8(ACCEL_YOUT_L);
	MPU6050_Data.ACCEL_ZOUT = readRegister8(ACCEL_ZOUT_H)<<8 | readRegister8(ACCEL_ZOUT_L);
	MPU6050_Data.GYRO_XOUT = readRegister8(GYRO_XOUT_H)<<8 | readRegister8(GYRO_XOUT_L);
	MPU6050_Data.GYRO_YOUT = readRegister8(GYRO_YOUT_H)<<8 | readRegister8(GYRO_YOUT_L);
	MPU6050_Data.GYRO_ZOUT = readRegister8(GYRO_ZOUT_H)<<8 | readRegister8(GYRO_ZOUT_L);
	MPU6050_Data.Temperature = readRegister8(TEMP_OUT_H)<<8 | readRegister8(TEMP_OUT_L);
	return MPU6050_Data;
}

MPU6050_float_Data_framework getMPU6050WorkedData()
{
	MPU6050_Data_framework MPU6050_RAW_Data_result = getMPU6050RAWData();
	MPU6050_float_Data_framework MPU6050_Worked_Data;
	uint8_t GYRO_RANGE = ( readRegister8(GYRO_CONFIG) & 0x18 ) >>3;
	uint8_t ACCEL_RANGE = ( readRegister8(ACCEL_CONFIG) & 0x18 ) >>3;
	switch(GYRO_RANGE)//根据量程计算
	{
	case 0:	MPU6050_Worked_Data.GYRO_XOUT = MPU6050_RAW_Data_result.GYRO_XOUT / 131.07;//250量程，量程计算公式为：32768（int16总共有32768*2个数，因为量程是250+-，所以除以2再除以正的量程所得到的就是一个LSB对应的实际物理量值）/250
			MPU6050_Worked_Data.GYRO_YOUT = MPU6050_RAW_Data_result.GYRO_YOUT / 131.07;
			MPU6050_Worked_Data.GYRO_ZOUT = MPU6050_RAW_Data_result.GYRO_ZOUT / 131.07;break;
	case 1:	MPU6050_Worked_Data.GYRO_XOUT = MPU6050_RAW_Data_result.GYRO_XOUT / 65.536;
			MPU6050_Worked_Data.GYRO_YOUT = MPU6050_RAW_Data_result.GYRO_YOUT / 65.536;
			MPU6050_Worked_Data.GYRO_ZOUT = MPU6050_RAW_Data_result.GYRO_ZOUT / 65.536;break;
	case 2:	MPU6050_Worked_Data.GYRO_XOUT = MPU6050_RAW_Data_result.GYRO_XOUT / 32.768;
			MPU6050_Worked_Data.GYRO_YOUT = MPU6050_RAW_Data_result.GYRO_YOUT / 32.768;
			MPU6050_Worked_Data.GYRO_ZOUT = MPU6050_RAW_Data_result.GYRO_ZOUT / 32.768;break;
	case 3:	MPU6050_Worked_Data.GYRO_XOUT = MPU6050_RAW_Data_result.GYRO_XOUT / 16.384;
			MPU6050_Worked_Data.GYRO_YOUT = MPU6050_RAW_Data_result.GYRO_YOUT / 16.384;
			MPU6050_Worked_Data.GYRO_ZOUT = MPU6050_RAW_Data_result.GYRO_ZOUT / 16.384;break;
	}
	switch(ACCEL_RANGE)
	{
	case 0:	MPU6050_Worked_Data.ACCEL_XOUT = MPU6050_RAW_Data_result.ACCEL_XOUT / 16384.0;
			MPU6050_Worked_Data.ACCEL_YOUT = MPU6050_RAW_Data_result.ACCEL_YOUT / 16384.0;
			MPU6050_Worked_Data.ACCEL_ZOUT = MPU6050_RAW_Data_result.ACCEL_ZOUT / 16384.0;break;
	case 1:	MPU6050_Worked_Data.ACCEL_XOUT = MPU6050_RAW_Data_result.ACCEL_XOUT / 8192.0;
			MPU6050_Worked_Data.ACCEL_YOUT = MPU6050_RAW_Data_result.ACCEL_YOUT / 8192.0;
			MPU6050_Worked_Data.ACCEL_ZOUT = MPU6050_RAW_Data_result.ACCEL_ZOUT / 8192.0;break;
	case 2:	MPU6050_Worked_Data.ACCEL_XOUT = MPU6050_RAW_Data_result.ACCEL_XOUT / 4096.0;
			MPU6050_Worked_Data.ACCEL_YOUT = MPU6050_RAW_Data_result.ACCEL_YOUT / 4096.0;
			MPU6050_Worked_Data.ACCEL_ZOUT = MPU6050_RAW_Data_result.ACCEL_ZOUT / 4096.0;break;
	case 3:	MPU6050_Worked_Data.ACCEL_XOUT = MPU6050_RAW_Data_result.ACCEL_XOUT / 2048.0;
			MPU6050_Worked_Data.ACCEL_YOUT = MPU6050_RAW_Data_result.ACCEL_YOUT / 2048.0;
			MPU6050_Worked_Data.ACCEL_ZOUT = MPU6050_RAW_Data_result.ACCEL_ZOUT / 2048.0;break;
	}
	MPU6050_Worked_Data.Temperature = 36.53 + MPU6050_RAW_Data_result.Temperature/340.0;
	return MPU6050_Worked_Data;
}

uint8_t readRegister8(uint8_t reg)
{
	uint8_t readedBuffer=0;
	status = HAL_I2C_Mem_Read(&hi2c1, MPU6050_I2CADDR, reg, I2C_MEMADD_SIZE_8BIT, &readedBuffer, 1, HAL_MAX_DELAY);
	if (status != HAL_OK){return 1;}
	return readedBuffer;
}

void writeRegister(uint8_t reg, uint8_t value)
{
	if( readRegister8(PWR_MGMT_1)>>6 & 1 )//检测sleep位是否为1，若是则清零
	{
		uint8_t value = readRegister8(PWR_MGMT_1) & 0xBF;//sleep位为bit6
		status = HAL_I2C_Mem_Write(&hi2c1, MPU6050_I2CADDR, PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &value, 1, HAL_MAX_DELAY);//清除sleep位
		if (status != HAL_OK){return;}
	}
	status = HAL_I2C_Mem_Write(&hi2c1, MPU6050_I2CADDR, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, HAL_MAX_DELAY);//开始写入目标寄存器
	if (status != HAL_OK){return;}

}
