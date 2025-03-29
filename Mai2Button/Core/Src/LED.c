/*
 * LED.c
 *
 *  Created on: Jan 15, 2025
 *      Author: LLL
 */

#include "LED.h"
#define BT_LED_NUM_light 16
#define BT_LED_NUM 8


uint8_t LEDData[64]={0};

uint8_t LED_bead[BT_LED_NUM_light*24]={0};

uint8_t r_start,g_start,b_start;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim1)
	  {
		  HAL_GPIO_WritePin(BT_LED_GPIO_Port, BT_LED_Pin, SET);
		  HAL_TIM_Base_Stop_IT(&htim1);
	  }
}

void copeLED()
{
	switch(LEDData[4])
	{
		case 0x31:setLedGs8Bit();default_REP();break;//设置单个灯颜色，需要收到 SetLedGsUpdate 才生效
		case 0x32:setLedGs8BitMulti();default_REP();break;//设置多个灯颜色，需要收到 SetLedGsUpdate 才生效
		case 0x33:setLedGs8BitMultiFade();break;//设置多个灯颜色渐变，需要收到 SetLedGsUpdate 才开始执行，渐变的起始色是前一个 setLedGs8BitMulti 设置的
		case 0x39:setLedFet();break;//设置框体灯光，会连续多次发送实现闪烁，需要立刻生效
		case 0x3c:SetLedGsUpdate();default_REP();break;//刷新灯效
		case 0x7c:GetEEPRom();break;//读取 EEPRom 的值
		case 0xf0:getBoardInfo();break;//获取设备固件号
		case 0xf1:getBoardStatus();break;//获取设备状态
		case 0xf3:getProtocolVersion();break;//未知，回复即可
		default:default_REP();
	}
}

void set_one_light(uint8_t LED_serial_NUM, uint8_t g, uint8_t r, uint8_t b)
{
	uint8_t LED_bit_first=(BT_LED_NUM_light/BT_LED_NUM*(LED_serial_NUM-1)+1)*24-24;//写入第几个bit。“BT_LED_NUM_light/8*（n-1）+1”确定首个灯珠序号。*24确定第几bit位。-24移至数组的对应的位数。
	uint8_t max=BT_LED_NUM_light/BT_LED_NUM;
	uint8_t LED_bit_current=LED_bit_first;
	for (int var = 0; var < max; ++var)
		{
			for (int i = 0; i < 8; ++i)
			{
				LED_bead[LED_bit_current]&=g>>(7-i);
				(LED_bit_current)++;
			}
			for (int i = 0; i < 8; ++i)
			{
				LED_bead[LED_bit_current]&=r>>(7-i);
				(LED_bit_current)++;
			}
			for (int i = 0; i < 8; ++i)
			{
				LED_bead[LED_bit_current]&=b>>(7-i);
				(LED_bit_current)++;
			}
		}
}

void set_Multi_light(uint8_t g, uint8_t r, uint8_t b)
{
	uint8_t LED_start_serial_NUM;
	if (LEDData[5]==0){LED_start_serial_NUM=LEDData[5]+1;}else{LED_start_serial_NUM=LEDData[5];}//设置从第几个灯开始亮,起始灯序号是0
	uint8_t LED_end_serial_NUM=LEDData[6];//设置到第几个灯结束,最大灯序号是8
	uint8_t LED_Multi_NUM=LED_end_serial_NUM-LED_start_serial_NUM;//计算要亮几个灯
	for (int var1 = 0; var1 < LED_Multi_NUM; ++var1)
		{
			set_one_light(LED_start_serial_NUM,g,r,b);
		}
}

void setLedGs8Bit()
{
	uint8_t LED_serial_NUM=LEDData[5]+1;//设置第几个灯亮,起始灯序号是0
	uint8_t r=LEDData[6];
	uint8_t g=LEDData[7];
	uint8_t b=LEDData[8];
	set_one_light(LED_serial_NUM,g,r,b);
}



void setLedGs8BitMulti()
{
	uint8_t r=LEDData[8];
	uint8_t g=LEDData[9];
	uint8_t b=LEDData[10];
	r_start=LEDData[8];
	g_start=LEDData[9];
	b_start=LEDData[10];
	set_Multi_light(g,r,b);
}

void setLedGs8BitMultiFade()
{
	uint8_t setLedGs8BitMultiFade_REP[8]={0xe0, LEDData[2], LEDData[1], 0x03, 0x01, 0x33, 0x01, 0x4a};
	CDC_Transmit_FS(setLedGs8BitMultiFade_REP,  sizeof(setLedGs8BitMultiFade_REP));
	uint8_t fade_time=4095 / LEDData[11] * 8;
	uint8_t r_end=LEDData[8];
	uint8_t g_end=LEDData[9];
	uint8_t b_end=LEDData[10];
	int r_change=r_start-r_end;
	int g_change=g_start-g_end;
	int b_change=b_start-b_end;
    // 计算每一步的变化量和余数
    int r_step = r_change / fade_time;
    int r_remainder = r_change % fade_time;
    int g_step = g_change / fade_time;
    int g_remainder = g_change % fade_time;
    int b_step = b_change / fade_time;
    int b_remainder = b_change % fade_time;

    // 用于存储当前的 r, g, b 值
    int r_current = r_start;
    int g_current = g_start;
    int b_current = b_start;

    for (uint8_t i = 0; i < fade_time; ++i)
    {
        // 更新当前 r, g, b 值
        r_current -= r_step;// 因为change是start-end所以使用-=，反之则+=
        g_current -= g_step;
        b_current -= b_step;

		if ( i%4 == 0 )
		{
			// 处理余数
			if (r_remainder > 0) {r_current--;r_remainder--;}
			if (g_remainder > 0) {g_current--;g_remainder--;}
			if (b_remainder > 0) {b_current--;b_remainder--;}
		}

        // 确保值在 0 到 255 范围内
        if (r_current < 0) r_current = 0;if (r_current > 255) r_current = 255;
        if (g_current < 0) g_current = 0;if (g_current > 255) g_current = 255;
        if (b_current < 0) b_current = 0;if (b_current > 255) b_current = 255;

        set_Multi_light(g_current,r_current,b_current);
        HAL_Delay(1);
    }
}



void setLedFet()
{
	uint8_t setLedFet_REP[8]={0xe0, LEDData[2], LEDData[1], 0x03, 0x01, 0x39, 0x01, 0x50};
	CDC_Transmit_FS(setLedFet_REP,  sizeof(setLedFet_REP));

}

void SetLedGsUpdate()
{
	HAL_GPIO_WritePin(BT_LED_GPIO_Port, BT_LED_Pin, RESET);
	HAL_TIM_Base_Start_IT(&htim1);
}

void GetEEPRom()
{
	uint8_t EEPRom[9]={0xe0, LEDData[2], LEDData[1], 0x04, 0x01, 0x7c, 0x01, LEDData[5], 0x94};
	CDC_Transmit_FS(EEPRom,  sizeof(EEPRom));
}

void getBoardInfo()
{
	uint8_t BoardInfo[18]={0xe0, LEDData[2], LEDData[1], 0x0d, 0x01, 0xf0, 0x01, 0x31, 0x35, 0x30, 0x37, 0x30, 0x2d, 0x30, 0x34, 0xff, 0x90, 0x2e};
	CDC_Transmit_FS(BoardInfo,  sizeof(BoardInfo));
}

void getBoardStatus()
{
	uint8_t BoardStatus[12]={0xe0, LEDData[2], LEDData[1], 0x07, 0x01, 0xf1, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0c};
	CDC_Transmit_FS(BoardStatus,  sizeof(BoardStatus));
}

void getProtocolVersion()
{
	uint8_t ProtocolVersion[11]={0xe0, LEDData[2], LEDData[1], 0x06, 0x01, 0xf3, 0x01, 0x01, 0x00, 0x00, 0x0e};
	CDC_Transmit_FS(ProtocolVersion,  sizeof(ProtocolVersion));
}

void default_REP()
{
	uint8_t default_REP[8]={0xe0, LEDData[2], LEDData[1], 0x03, 0x01, LEDData[4], 0x01, 0x00};
	default_REP[7]=default_REP[1]+default_REP[2]+default_REP[3]+default_REP[4]+default_REP[5]+default_REP[6];
	if(default_REP[7]>0xff){default_REP[7]=default_REP[7];}
	CDC_Transmit_FS(default_REP,  sizeof(default_REP));
}

