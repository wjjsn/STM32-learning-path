/*
 * reDefineIT.c
 *
 *  Created on: Apr 13, 2025
 *      Author: 28584
 */

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "adc.h"
#include "OLED.h"

extern uint16_t RPValue[4];
extern float kp,ki,kd;
extern int SetSpeed;
extern osSemaphoreId_t OLEDupdateSemHandle;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == ADC1)
	{
//		if(RPValue[0]&&RPValue[1]&&RPValue[2]&&RPValue[3])return;
		for (uint8_t i = 0; i < 4; ++i) {RPValue[i]/=128;}
		kp=RPValue[3];
		ki=RPValue[0];
		kd=RPValue[1];
		SetSpeed=RPValue[2]*128/88;
		OLED_showNUM(2, 0, RPValue[3], 8);//RP1
		OLED_showNUM(4, 0, RPValue[0], 8);//RP2
		OLED_showNUM(6, 0, RPValue[1], 8);//RP3
//		OLED_showNUM(6, 0, RPValue[2], 8);//RP4
		osSemaphoreRelease (OLEDupdateSemHandle);
	}
}
