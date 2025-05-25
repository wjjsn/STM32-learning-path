/*
 * qnzm.c
 *
 *  Created on: May 10, 2025
 *      Author: 28584
 */

#include "qnzm.h"
#include "usbd_customhid.h"

void ifkey(int command);
extern uint8_t report[15];

void Transmit_Touch_Data(const mpr121_t *mpr121)
{
	uint16_t data=touched(mpr121);
	switch(mpr121->i2cAddr)
	{
		case 0x5A<<1:
			if(data & ((uint16_t)0x01<<0)){ifkey(TOUCH11_TRIG);}else{ifkey(TOUCH11_RELEASE);}
			if(data & ((uint16_t)0x01<<1)){ifkey(TOUCH12_TRIG);}else{ifkey(TOUCH12_RELEASE);}
			if(data & ((uint16_t)0x01<<2)){ifkey(TOUCH9_TRIG);}else{ifkey(TOUCH9_RELEASE);}
			if(data & ((uint16_t)0x01<<3)){ifkey(TOUCH10_TRIG);}else{ifkey(TOUCH10_RELEASE);}
			if(data & ((uint16_t)0x01<<4)){ifkey(TOUCH7_TRIG);}else{ifkey(TOUCH7_RELEASE);}
			if(data & ((uint16_t)0x01<<5)){ifkey(TOUCH8_TRIG);}else{ifkey(TOUCH8_RELEASE);}
			if(data & ((uint16_t)0x01<<6)){ifkey(TOUCH6_TRIG);}else{ifkey(TOUCH6_RELEASE);}
			if(data & ((uint16_t)0x01<<7)){ifkey(TOUCH5_TRIG);}else{ifkey(TOUCH5_RELEASE);}
			if(data & ((uint16_t)0x01<<8)){ifkey(TOUCH4_TRIG);}else{ifkey(TOUCH4_RELEASE);}
			if(data & ((uint16_t)0x01<<9)){ifkey(TOUCH3_TRIG);}else{ifkey(TOUCH3_RELEASE);}
			if(data & ((uint16_t)0x01<<10)){ifkey(TOUCH2_TRIG);}else{ifkey(TOUCH2_RELEASE);}
			if(data & ((uint16_t)0x01<<11)){ifkey(TOUCH1_TRIG);}else{ifkey(TOUCH1_RELEASE);}
			break;
			
		case 0x5C<<1:
//			if(data & ((uint16_t)0x01<<0)){ifkey(TOUCH25_TRIG);}else{ifkey(TOUCH25_RELEASE);}
//			if(data & ((uint16_t)0x01<<1)){ifkey(TOUCH26_TRIG);}else{ifkey(TOUCH26_RELEASE);}
			if(data & ((uint16_t)0x01<<2)){ifkey(TOUCH31_TRIG);}else{ifkey(TOUCH31_RELEASE);}
			if(data & ((uint16_t)0x01<<3)){ifkey(TOUCH32_TRIG);}else{ifkey(TOUCH32_RELEASE);}
			if(data & ((uint16_t)0x01<<4)){ifkey(TOUCH29_TRIG);}else{ifkey(TOUCH29_RELEASE);}
			if(data & ((uint16_t)0x01<<5)){ifkey(TOUCH30_TRIG);}else{ifkey(TOUCH30_RELEASE);}
			if(data & ((uint16_t)0x01<<6)){ifkey(TOUCH28_TRIG);}else{ifkey(TOUCH28_RELEASE);}
			if(data & ((uint16_t)0x01<<7)){ifkey(TOUCH27_TRIG);}else{ifkey(TOUCH27_RELEASE);}
			if((data & ((uint16_t)0x01<<8))){ifkey(TOUCH26_TRIG);}else{ifkey(TOUCH26_RELEASE);}
			if((data & ((uint16_t)0x01<<9))){ifkey(TOUCH25_TRIG);}else{ifkey(TOUCH25_RELEASE);}
//			if((data & ((uint16_t)0x01<<10))){ifkey(TOUCH23_TRIG);}else{ifkey(TOUCH23_RELEASE);}
//			if((data & ((uint16_t)0x01<<11))){ifkey(TOUCH24_TRIG);}else{ifkey(TOUCH24_RELEASE);}
			break;

		case 0x5D<<1:
				if(data & ((uint16_t)0x01<<0)){ifkey(TOUCH23_TRIG);}else{ifkey(TOUCH23_RELEASE);}
				if(data & ((uint16_t)0x01<<1)){ifkey(TOUCH24_TRIG);}else{ifkey(TOUCH24_RELEASE);}
				if(data & ((uint16_t)0x01<<2)){ifkey(TOUCH21_TRIG);}else{ifkey(TOUCH21_RELEASE);}
				if(data & ((uint16_t)0x01<<3)){ifkey(TOUCH22_TRIG);}else{ifkey(TOUCH22_RELEASE);}
				if(data & ((uint16_t)0x01<<4)){ifkey(TOUCH19_TRIG);}else{ifkey(TOUCH19_RELEASE);}
				if(data & ((uint16_t)0x01<<5)){ifkey(TOUCH20_TRIG);}else{ifkey(TOUCH20_RELEASE);}
				if(data & ((uint16_t)0x01<<6)){ifkey(TOUCH18_TRIG);}else{ifkey(TOUCH18_RELEASE);}
				if(data & ((uint16_t)0x01<<7)){ifkey(TOUCH17_TRIG);}else{ifkey(TOUCH17_RELEASE);}
				if(data & ((uint16_t)0x01<<8)){ifkey(TOUCH16_TRIG);}else{ifkey(TOUCH16_RELEASE);}
				if(data & ((uint16_t)0x01<<9)){ifkey(TOUCH15_TRIG);}else{ifkey(TOUCH15_RELEASE);}
				if(data & ((uint16_t)0x01<<10)){ifkey(TOUCH14_TRIG);}else{ifkey(TOUCH14_RELEASE);}
				if(data & ((uint16_t)0x01<<11)){ifkey(TOUCH13_TRIG);}else{ifkey(TOUCH13_RELEASE);}
			break;
	}
	extern USBD_HandleTypeDef hUsbDeviceFS;
	USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report,sizeof(report));
}
