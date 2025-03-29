/*
 * HC-SR04.c
 *
 *  Created on: Feb 19, 2025
 *      Author: Administrator
 */

#include "HC-SR04.h"

float measure()
{
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	__HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_CC1);
	__HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_CC2);
	HAL_TIM_IC_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_IC_Start(&htim1, TIM_CHANNEL_2);
	HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, 1);
	for (int var = 0; var < 15; ++var)
	HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, 0);
	uint32_t cc1Flag =__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_CC1);
	uint32_t cc2Flag =__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_CC2);
	while(1)
	{
		cc1Flag =__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_CC1);
		cc2Flag =__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_CC2);
		if(cc1Flag!=0 || cc2Flag!=0){break;}
	}
	HAL_TIM_IC_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_IC_Stop(&htim1, TIM_CHANNEL_2);
	uint16_t ccr1 =__HAL_TIM_GET_COMPARE(&htim1,TIM_CHANNEL_1);
	uint16_t ccr2 =__HAL_TIM_GET_COMPARE(&htim1,TIM_CHANNEL_2);
	float distance = (ccr2-ccr1) * 0.034;
	return distance;
}
