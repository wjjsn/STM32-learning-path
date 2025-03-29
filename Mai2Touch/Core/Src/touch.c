/*
 * touch.c
 *
 *  Created on: Dec 30, 2024
 *      Author: Administrator
 */
#include "main.h"
#include "gpio.h"



	uint8_t touchdata[] = {0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29};

	void doingnothing(){}

	void ReadTouchData()
{
	if(HAL_GPIO_ReadPin(A1_GPIO_Port, A1_Pin)==SET){touchdata[1]=touchdata[1]|0x01;}else{touchdata[1]=touchdata[1]&0xFE;}//A1
	if(HAL_GPIO_ReadPin(A2_GPIO_Port, A2_Pin)==SET){touchdata[1]=touchdata[1]|0x02;}else{touchdata[1]=touchdata[1]&0xFD;}//A2
	if(HAL_GPIO_ReadPin(A3_GPIO_Port, A3_Pin)==SET){touchdata[1]=touchdata[1]|0x04;}else{touchdata[1]=touchdata[1]&0xFB;}//A3
	if(HAL_GPIO_ReadPin(A4_GPIO_Port, A4_Pin)==SET){touchdata[1]=touchdata[1]|0x08;}else{touchdata[1]=touchdata[1]&0xF7;}//A4
	if(HAL_GPIO_ReadPin(A5_GPIO_Port, A5_Pin)==SET){touchdata[1]=touchdata[1]|0x10;}else{touchdata[1]=touchdata[1]&0xEF;}//A5
	if(HAL_GPIO_ReadPin(A6_GPIO_Port, A6_Pin)==SET){touchdata[2]=touchdata[2]|0x01;}else{touchdata[2]=touchdata[2]&0xFE;}//A6
	if(HAL_GPIO_ReadPin(A7_GPIO_Port, A7_Pin)==SET){touchdata[2]=touchdata[2]|0x02;}else{touchdata[2]=touchdata[2]&0xFD;}//A7
	if(HAL_GPIO_ReadPin(A8_GPIO_Port, A8_Pin)==SET){touchdata[2]=touchdata[2]|0x04;}else{touchdata[2]=touchdata[2]&0xFB;}//A8
	if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)==SET){touchdata[2]=touchdata[2]|0x08;}else{touchdata[2]=touchdata[2]&0xF7;}//B1
	if(HAL_GPIO_ReadPin(B2_GPIO_Port, B2_Pin)==SET){touchdata[2]=touchdata[2]|0x10;}else{touchdata[2]=touchdata[2]&0xEF;}//B2
	if(HAL_GPIO_ReadPin(B3_GPIO_Port, B3_Pin)==SET){touchdata[3]=touchdata[3]|0x01;}else{touchdata[3]=touchdata[3]&0xFE;}//B3
	if(HAL_GPIO_ReadPin(B4_GPIO_Port, B4_Pin)==SET){touchdata[3]=touchdata[3]|0x02;}else{touchdata[3]=touchdata[3]&0xFD;}//B4
	if(HAL_GPIO_ReadPin(B5_GPIO_Port, B5_Pin)==SET){touchdata[3]=touchdata[3]|0x04;}else{touchdata[3]=touchdata[3]&0xFB;}//B5
	if(HAL_GPIO_ReadPin(B6_GPIO_Port, B6_Pin)==SET){touchdata[3]=touchdata[3]|0x08;}else{touchdata[3]=touchdata[3]&0xF7;}//B6
	if(HAL_GPIO_ReadPin(B7_GPIO_Port, B7_Pin)==SET){touchdata[3]=touchdata[3]|0x10;}else{touchdata[3]=touchdata[3]&0xEF;}//B7
	if(HAL_GPIO_ReadPin(B8_GPIO_Port, B8_Pin)==SET){touchdata[4]=touchdata[4]|0x01;}else{touchdata[4]=touchdata[4]&0xFE;}//B8
	if(HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin)==SET){touchdata[4]=touchdata[4]|0x02;}else{touchdata[4]=touchdata[4]&0xFD;}//C1
	if(HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin)==SET){touchdata[4]=touchdata[4]|0x04;}else{touchdata[4]=touchdata[4]&0xFB;}//C2
	if(HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin)==SET){touchdata[4]=touchdata[4]|0x08;}else{touchdata[4]=touchdata[4]&0xF7;}//D1
	if(HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin)==SET){touchdata[4]=touchdata[4]|0x10;}else{touchdata[4]=touchdata[4]&0xEF;}//D2
	if(HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin)==SET){touchdata[5]=touchdata[5]|0x01;}else{touchdata[5]=touchdata[5]&0xFE;}//D3
	if(HAL_GPIO_ReadPin(D4_GPIO_Port, D4_Pin)==SET){touchdata[5]=touchdata[5]|0x02;}else{touchdata[5]=touchdata[5]&0xFD;}//D4
	if(HAL_GPIO_ReadPin(D5_GPIO_Port, D5_Pin)==SET){touchdata[5]=touchdata[5]|0x04;}else{touchdata[5]=touchdata[5]&0xFB;}//D5
	if(HAL_GPIO_ReadPin(D6_GPIO_Port, D6_Pin)==SET){touchdata[5]=touchdata[5]|0x08;}else{touchdata[5]=touchdata[5]&0xF7;}//D6
	if(HAL_GPIO_ReadPin(D7_GPIO_Port, D7_Pin)==SET){touchdata[5]=touchdata[5]|0x10;}else{touchdata[5]=touchdata[5]&0xEF;}//D7
	if(HAL_GPIO_ReadPin(D8_GPIO_Port, D8_Pin)==SET){touchdata[6]=touchdata[6]|0x01;}else{touchdata[6]=touchdata[6]&0xFE;}//D8
	if(HAL_GPIO_ReadPin(E1_GPIO_Port, E1_Pin)==SET){touchdata[6]=touchdata[6]|0x02;}else{touchdata[6]=touchdata[6]&0xFD;}//E1
	if(HAL_GPIO_ReadPin(E2_GPIO_Port, E2_Pin)==SET){touchdata[6]=touchdata[6]|0x04;}else{touchdata[6]=touchdata[6]&0xFB;}//E2
	if(HAL_GPIO_ReadPin(E3_GPIO_Port, E3_Pin)==SET){touchdata[6]=touchdata[6]|0x08;}else{touchdata[6]=touchdata[6]&0xF7;}//E3
	if(HAL_GPIO_ReadPin(E4_GPIO_Port, E4_Pin)==SET){touchdata[6]=touchdata[6]|0x10;}else{touchdata[6]=touchdata[6]&0xEF;}//E4
	if(HAL_GPIO_ReadPin(E5_GPIO_Port, E5_Pin)==SET){touchdata[7]=touchdata[7]|0x01;}else{touchdata[7]=touchdata[7]&0xFE;}//E5
	if(HAL_GPIO_ReadPin(E6_GPIO_Port, E6_Pin)==SET){touchdata[7]=touchdata[7]|0x02;}else{touchdata[7]=touchdata[7]&0xFD;}//E6
	if(HAL_GPIO_ReadPin(E7_GPIO_Port, E7_Pin)==SET){touchdata[7]=touchdata[7]|0x04;}else{touchdata[7]=touchdata[7]&0xFB;}//E7
	if(HAL_GPIO_ReadPin(E8_GPIO_Port, E8_Pin)==SET){touchdata[7]=touchdata[7]|0x08;}else{touchdata[7]=touchdata[7]&0xF7;}//E8
}

void SendTouchData()
{
	ReadTouchData();
	CDC_Transmit_FS(touchdata,  sizeof(touchdata));
}
