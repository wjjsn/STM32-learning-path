/*
 * motor.h
 *
 *  Created on: Mar 28, 2025
 *      Author: 28584
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"
#include "tim.h"

enum
{
	MOTOR_A = 1,
	MOTOR_B = 2
};

enum
{
	MOTOR_FORWARD = 1,
	MOTOR_BACKWARD = 2,
	MOTOR_BREAK = 3,
	MOTOR_STOP = 4,
};

void motorSet(uint8_t motor, uint8_t dir, uint8_t speed);
void Set_PWM_Frequency(uint32_t freq_hz);
void Set_PWM_DutyCycle(uint16_t duty);

#endif /* INC_MOTOR_H_ */
