/*
 * font.h
 *
 *  Created on: Feb 16, 2025
 *      Author: Administrator
 */

#ifndef INC_FONT_H_
#define INC_FONT_H_

#include "main.h"

struct chinese
{
	uint8_t ch_utf8[3];
	uint8_t ch_dt1[16];
	uint8_t ch_dt2[16];
};

extern struct chinese cn16_16[];
extern uint8_t EN8_16[][16];
extern uint8_t EN6_12[][12];
extern uint8_t CN16_16[][36];

#endif /* INC_FONT_H_ */
