/*
 * motor.c
 *
 *  Created on: Mar 28, 2025
 *      Author: 28584
 */

#include "motor.h"

void motorSet(uint8_t motor, uint8_t dir, uint16_t speed)
{
	GPIO_PinState in1 = GPIO_PIN_RESET, in2 = GPIO_PIN_RESET;
	switch(dir)
	{
		case MOTOR_FORWARD:
			in1 = GPIO_PIN_RESET;//0
			in2 = GPIO_PIN_SET;//1
			break;
		case MOTOR_BACKWARD:
			in1 = GPIO_PIN_SET;//1
			in2 = GPIO_PIN_RESET;//0
			break;
		case MOTOR_BREAK:
			in1 = GPIO_PIN_SET;//0
			in2 = GPIO_PIN_SET;//1
			break;
		case MOTOR_STOP:
			in1 = GPIO_PIN_RESET;//0
			in2 = GPIO_PIN_RESET;//0
			speed = 0;
	}
	if(motor == MOTOR_A)
	{
		HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, in1);
		HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, in2);
		//__HAL_TIM_SET_COMPARE(MOTOR_PWM_TIM, TIM_CHANNEL_1, speed);
	}
	if(motor == MOTOR_B)
	{
		HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, in1);
		HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, in2);
		//__HAL_TIM_SET_COMPARE(MOTOR_PWM_TIM, TIM_CHANNEL_2, speed);
	}
	Set_PWM_DutyCycle(speed);
//	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

void Set_PWM_DutyCycle(uint16_t duty)
{
    // 参数限制：1~3600
    if (duty < 1) duty = 1;
    if (duty > 3600) duty = 3600;

    // 更新CCR
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty);
}


