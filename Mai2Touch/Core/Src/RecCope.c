/*
 * RecCope.c
 *
 *  Created on: Dec 30, 2024
 *      Author: Administrator
 */

#include "main.h"
extern uint8_t state;

void RecCope(uint8_t* Buf)
{
	uint8_t recdatapacket[4]={Buf[1],Buf[2],Buf[3],Buf[4]};
	uint8_t replydata[6]={0x28,Buf[1],Buf[2],Buf[3],Buf[4],0x29};
	switch(recdatapacket[2])
	{
	case 0x45:reset();break;//Reset初始化指令，无需回复
	case 0x4c:state=0;break;//Start Conditioning Mode进入配置模式，无需回复，如果在发送触摸数据，需要停止
	case 0x72:CDC_Transmit_FS(replydata, 6);break;//Set Touch Panel Ratio设置 Ratio
	case 0x6b:CDC_Transmit_FS(replydata, 6);break;//Set Touch Panel Sensitivity设置 Sensitivity
	case 0x41:state=1;break;//End Conditioning Mode退出配置模式，无需回复，可以开始发送触摸数据
	}
}

void reset()
{

}

void set_Ratio()
{

}

void set_Sensitivity()
{

}
