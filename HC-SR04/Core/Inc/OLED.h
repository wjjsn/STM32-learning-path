/*
 * OLED.h
 *
 *  Created on: Feb 15, 2025
 *      Author: Administrator
 */

#ifndef USER_INC_OLED_H_
#define USER_INC_OLED_H_

#include "i2c.h"
#include "main.h"
#include "font.h"

void OLED_WriteCommand(uint8_t cmd);
void OLED_init(void);
void OLED_chear(void);
void OLED_showChar(uint8_t Page, uint8_t X,char a,uint8_t fontSize);
void OLED_showString(uint8_t Page, uint8_t X,char *String,uint8_t fontSize);
void OLED_showImage(uint8_t Page, uint8_t X, uint8_t Width, uint8_t Height,uint8_t *Image);
void OLED_showChinese(uint8_t Page, uint8_t X, char *Chinese);

#endif /* USER_INC_OLED_H_ */
