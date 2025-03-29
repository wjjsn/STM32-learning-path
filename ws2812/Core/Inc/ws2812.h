/*
 * ws2812.h
 *
 *  Created on: Mar 18, 2025
 *      Author: 28584
 */

#ifndef INC_WS2812_H_
#define INC_WS2812_H_

#include "main.h"
#include "tim.h"

#define LED_NUM 10
#define DATA_ARRAY_NUM LED_NUM*24+1

enum
{
	code0 = 30,
	code1 = 60,
	reset = 0
};

void ws2812Updata();
void initColorTable();
void ws2812SetOneLED(uint16_t LED_bit, uint32_t color);
void ws2812SetMultipleLED(uint16_t Start_LED_bit, uint16_t Stop_LED_bit, uint32_t color);
uint32_t hsv2rgb(uint16_t h, uint8_t s, uint8_t v);
void rainbowFlowOptimized(uint16_t numLEDs, uint8_t speed);

#endif /* INC_WS2812_H_ */
