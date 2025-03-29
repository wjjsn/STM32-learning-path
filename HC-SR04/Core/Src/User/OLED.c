/*
 * OLED.c
 *
 *  Created on: Feb 15, 2025
 *      Author: Administrator
 */

/*
 * OLED.c
 *
 *  Created on: Dec 19, 2024
 *      Author: Administrator
 */
#include "OLED.h"
#include "string.h"

#define OLED_ADDRESS 0x78
#define OLED_CHN_CHAR_WIDTH 3

void OLED_WriteCommand(uint8_t cmd)
{
	uint8_t sendBuffer[2];
	sendBuffer[0]=0x00;
	sendBuffer[1]=cmd;
	HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, sendBuffer, 2, HAL_MAX_DELAY);
}

void OLED_WriteData(uint8_t dat)
{
	uint8_t sendBuffer[2];
	sendBuffer[0]=0x40;
	sendBuffer[1]=dat;
	HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, sendBuffer, 2, HAL_MAX_DELAY);
}

void OLED_SetCursor(uint8_t Page, uint8_t X)
{
	/*通过指令设置页地址和列地址*/
	OLED_WriteCommand(0xB0 | Page);					//设置页位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

void OLED_init(void)
{
	HAL_Delay(100);
	OLED_WriteCommand(0xAE);	//设置显示开启/关闭，0xAE关闭，0xAF开启

	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);	//0x00~0xFF

	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);	//0x0E~0x3F

	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);	//0x00~0x7F

	OLED_WriteCommand(0x40);	//设置显示开始行，0x40~0x7F

	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常，0xA0左右反置

	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常，0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);

	OLED_WriteCommand(0x81);	//设置对比度
	OLED_WriteCommand(0xCF);	//0x00~0xFF

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/反色显示，0xA6正常，0xA7反色

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
}

void OLED_chear(void)
{
	for (int i = 0; i < 8; ++i)
	{
		OLED_SetCursor(i, 0);
		for (int j = 0; j < 128; ++j)
		{
			OLED_WriteData(0x00);
		}
	}
}

void OLED_showChar(uint8_t Page, uint8_t X,char a,uint8_t fontSize)
{
	OLED_SetCursor(Page, X);
	if (fontSize == 8)
	{
		for (int i = 0; i < fontSize; ++i)
		{
			OLED_WriteData(EN8_16[ a - ' ' ][i]);
		}
		OLED_SetCursor(Page+1, X);
		for (int i = 0; i < fontSize; ++i)
		{
			OLED_WriteData(EN8_16[ a - ' ' ][i+8]);
		}
	}
	else if (fontSize == 6)
	{
		for (int i = 0; i < fontSize; ++i)
		{
			OLED_WriteData(EN6_12[ a - ' ' ][i]);
		}
		OLED_SetCursor(Page+1, X);
		for (int i = 0; i < fontSize; ++i)
		{
			OLED_WriteData(EN6_12[ a - ' ' ][i+6]);
		}
	}
}

void OLED_showString(uint8_t Page, uint8_t X,char *String,uint8_t fontSize)
{
	for (int i = 0; String[i] !='\0' ; ++i)
	{
		OLED_showChar(Page, X+i*fontSize ,String[i],fontSize);
	}
}

void OLED_showImage(uint8_t Page, uint8_t X, uint8_t Width, uint8_t Height,uint8_t *Image)
{
	for (int j = 0; j < Height; ++j)
	{
		OLED_SetCursor( Page+j , X);
		for (int i = 0; i < Width; ++i)
		{
			OLED_WriteData(Image[Width*j+i]);
		}
	}
}

//void OLED_showChinese(uint8_t Page, uint8_t X, char *Chinese)
//{
//	uint8_t pChinese = 0;
//	uint8_t pIndex;
//	uint8_t i;
//	char SingleChinese[OLED_CHN_CHAR_WIDTH + 1] = {0};
//
//	for (i = 0; Chinese[i] != '\0'; i ++)		//遍历汉字串
//	{
//		SingleChinese[pChinese] = Chinese[i];	//提取汉字串数据到单个汉字数组
//		pChinese ++;							//计次自增
//
//		/*当提取次数到达OLED_CHN_CHAR_WIDTH时，即代表提取到了一个完整的汉字*/
//		if (pChinese >= OLED_CHN_CHAR_WIDTH)
//		{
//			pChinese = 0;		//计次归零
//
//			/*遍历整个汉字字模库，寻找匹配的汉字*/
//			/*如果找到最后一个汉字（定义为空字符串），则表示汉字未在字模库定义，停止寻找*/
//			for (pIndex = 0; strcmp(OLED_CF16x16[pIndex].Index, "") != 0; pIndex ++)
//			{	/*找到匹配的汉字*/
//				if (strcmp(OLED_CF16x16[pIndex].Index, SingleChinese) == 0)
//				{
//					break;		//跳出循环，此时pIndex的值为指定汉字的索引
//				}
//			}
//			/*将汉字字模库OLED_CF16x16的指定数据以16*16的图像格式显示*/
//			OLED_ShowImage( Page, X + ((i + 1) / OLED_CHN_CHAR_WIDTH - 1) * 16, 16, 2, OLED_CF16x16[pIndex].Data);
//		}
//	}
//}
