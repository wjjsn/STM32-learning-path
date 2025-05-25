/*
 * menu.h
 *
 *  Created on: Mar 23, 2025
 *      Author: 28584
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include "OLED.h"

typedef struct
{
	uint8_t option1[25];
	uint8_t option2[25];
	uint8_t option3[25];
	uint8_t option4[25];
	uint8_t option5[25];
	uint8_t option6[25];
	uint8_t option7[25];
	uint8_t option8[25];

	uint8_t sursor_point;
}
menu;

#endif /* INC_MENU_H_ */
