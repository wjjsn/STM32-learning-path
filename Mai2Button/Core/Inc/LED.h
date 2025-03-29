/*
 * LED.h
 *
 *  Created on: Jan 15, 2025
 *      Author: LLL
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "string.h"
#include "main.h"
#include "tim.h"
extern uint8_t LEDData[64];

void copeLED();
void setLedGs8Bit();
void setLedGs8BitMulti();
void setLedGs8BitMultiFade();
void setLedFet();
void SetLedGsUpdate();
void GetEEPRom();
void getBoardInfo();
void getBoardStatus();
void getProtocolVersion();
void default_REP();

#endif /* INC_LED_H_ */
