/*
 * motor.c
 *
 *  Created on: Mar 28, 2025
 *      Author: 28584
 */

#include "motor.h"

void motorSet(uint8_t motor, uint8_t dir, uint16_t speed)
{
	GPIO_PinState in1, in2;
	switch(dir)
	{
		case MOTOR_FORWARD:
			in1 = GPIO_PIN_SET;//1
			in2 = GPIO_PIN_RESET;//0
			break;
		case MOTOR_BACKWARD:
			in1 = GPIO_PIN_RESET;//0
			in2 = GPIO_PIN_SET;//1
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
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

void Set_PWM_Frequency(uint32_t freq_hz) //1.1k~72kHz
{
    // 计算ARR（确保ARR≥999以维持0.1%精度）
    uint32_t new_arr = (72000000 / freq_hz) - 1;
    if (new_arr < 999) new_arr = 999; // 强制最低ARR=999
    if (new_arr > 65535)
    {
    	new_arr = 65535;

    }

    // 更新ARR（自动保持当前占空比）
    uint32_t current_ccr = __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1);
    uint32_t new_ccr = (current_ccr * (new_arr + 1)) / (htim1.Instance->ARR + 1);

    __HAL_TIM_SET_AUTORELOAD(&htim1, new_arr);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, new_ccr);
}

void Set_PWM_DutyCycle(uint16_t duty)
{
    // 参数限制：1~1000（对应0.1%~100.0%）
    if (duty < 1) duty = 1;
    if (duty > 1000) duty = 1000;

    // 计算CCR（四舍五入）
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim1);
    uint32_t ccr = (duty * (arr + 1) + 500) / 1000; // +500实现四舍五入

    // 更新CCR
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ccr);
}


