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
#include "stdio.h"

void OLED_WriteCommand(uint8_t cmd);
void OLED_init(void);
void OLED_chear(void);
void OLED_showChar(uint8_t Page, uint8_t X,char a,uint8_t fontSize);
void OLED_showString(uint8_t Page, uint8_t X,char *String,uint8_t fontSize);
void OLED_showImage(uint8_t Page, uint8_t X, uint8_t Width, uint8_t Height,uint8_t *Image);
void OLED_showChinese(uint8_t Page, uint8_t X, char *Chinese);
void OLED_showNUM(uint8_t Page, uint8_t X, uint32_t num, uint8_t fontSize);
void OLED_showSignedNUM(uint8_t Page, uint8_t X, int32_t num, uint8_t fontSize);
void OLED_showSignedFlout(uint8_t Page, uint8_t X, float num, uint8_t fontSize);
void OLED_showHEX(uint8_t Page, uint8_t X, uint32_t, uint8_t fontSize);

#endif /* USER_INC_OLED_H_ */
