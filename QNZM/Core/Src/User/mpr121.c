/*
 * mpr121.c
 *
 *  Created on: Mar 12, 2025
 *      Author: 28584
 */

#include "mpr121.h"

const mpr121_t mpr121_1={0x5A<<1, &hi2c1};
const mpr121_t mpr121_2={0x5c<<1, &hi2c1};
const mpr121_t mpr121_3={0x5D<<1, &hi2c1};

// uncomment to use autoconfig !
#define AUTOCONFIG // use autoconfig (Yes it works pretty well!)

HAL_StatusTypeDef status;

uint8_t mpr121_init(const mpr121_t *mpr121)
{
	writeRegister(mpr121, MPR121_SOFTRESET, 0x63);//软重置
	writeRegister(mpr121, MPR121_ECR, 0x00);

	uint8_t c = readRegister8(mpr121, MPR121_CONFIG2);
	if (c != 0x24){return 1;}

	setThresholds(mpr121, MPR121_TOUCH_THRESHOLD_DEFAULT, MPR121_RELEASE_THRESHOLD_DEFAULT);
	writeRegister(mpr121, MPR121_MHDR, 0x01);
	writeRegister(mpr121, MPR121_NHDR, 0x01);
	writeRegister(mpr121, MPR121_NCLR, 0x0E);
	writeRegister(mpr121, MPR121_FDLR, 0x00);

	writeRegister(mpr121, MPR121_MHDF, 0x01);
	writeRegister(mpr121, MPR121_NHDF, 0x05);
	writeRegister(mpr121, MPR121_NCLF, 0x01);
	writeRegister(mpr121, MPR121_FDLF, 0x00);

	writeRegister(mpr121, MPR121_NHDT, 0x00);
	writeRegister(mpr121, MPR121_NCLT, 0x00);
	writeRegister(mpr121, MPR121_FDLT, 0x00);

	writeRegister(mpr121, MPR121_DEBOUNCE, 0);
	writeRegister(mpr121, MPR121_CONFIG1, 0x10); // default, 16uA charge current
	writeRegister(mpr121, MPR121_CONFIG2, 0x20); // 0.5uS encoding, 1ms period

	#ifdef AUTOCONFIG
	  writeRegister(mpr121, MPR121_AUTOCONFIG0, 0x0B);

	  // correct values for Vdd = 3.3V
	  writeRegister(mpr121, MPR121_UPLIMIT, 200);     // ((Vdd - 0.7)/Vdd) * 256
	  writeRegister(mpr121, MPR121_TARGETLIMIT, 180); // UPLIMIT * 0.9
	  writeRegister(mpr121, MPR121_LOWLIMIT, 130);    // UPLIMIT * 0.65
	#endif

	uint8_t ECR_SETTING = 0x8C;//配置ECR寄存器，位的作用在数据手册的第16页
	writeRegister(mpr121, MPR121_ECR, ECR_SETTING); // start with above ECR setting
	return 0;
}

void setThresholds(const mpr121_t *mpr121, uint8_t touch, uint8_t release)
{
	// set all thresholds (the same)
	for (uint8_t i = 0; i < 12; i++)
	{
		writeRegister(mpr121, MPR121_TOUCHTH_0 + 2 * i, touch);
		writeRegister(mpr121, MPR121_RELEASETH_0 + 2 * i, release);
	}
}

uint16_t touched(const mpr121_t *mpr121)
{
	uint16_t data = ( readRegister8(mpr121, MPR121_TOUCHSTATUS_H)<<8 | readRegister8(mpr121, MPR121_TOUCHSTATUS_L) );
	return data;
}

uint8_t readRegister8(const mpr121_t *mpr121, uint8_t reg)
{
	uint8_t readedBuffer=0;
	status = HAL_I2C_Mem_Read(mpr121->hi2c, mpr121->i2cAddr, reg, I2C_MEMADD_SIZE_8BIT, &readedBuffer, 1, HAL_MAX_DELAY);
	if (status != HAL_OK){return 0;}
	return readedBuffer;
}

void writeRegister(const mpr121_t *mpr121, uint8_t reg, uint8_t value)
{
	uint8_t stop_required = 1;//判断是否需要进入停止状态指示位
	uint8_t ecr_backup = readRegister8(mpr121, MPR121_ECR);//备份电极配置寄存器(ERC寄存器)
	uint8_t ecr_chear = 0x00;
	if( (reg == MPR121_ECR) || ((0x73 <= reg) && (reg <= 0x7A)) ){stop_required = 0;}//如果成立则无需进入停止状态
	if (stop_required)//清除ERC寄存器，进入停止状态
	{
		status = HAL_I2C_Mem_Write(mpr121->hi2c, mpr121->i2cAddr, MPR121_ECR, I2C_MEMADD_SIZE_8BIT, &ecr_chear, 1, HAL_MAX_DELAY);
		if (status != HAL_OK){return;}
	}
	status = HAL_I2C_Mem_Write(mpr121->hi2c, mpr121->i2cAddr, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, HAL_MAX_DELAY);//开始写入目标寄存器
	if (status != HAL_OK){return;}
	if (stop_required) //还原ERC寄存器的值
	{
		status = HAL_I2C_Mem_Write(mpr121->hi2c, mpr121->i2cAddr, MPR121_ECR, I2C_MEMADD_SIZE_8BIT, &ecr_backup, 1, HAL_MAX_DELAY);
		if (status != HAL_OK){return;}
	}
}
