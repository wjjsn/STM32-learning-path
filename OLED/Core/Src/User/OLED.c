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


#define OLED_ADDRESS 0x78
#define OLED_CHN_CHAR_WIDTH 3

uint8_t DisplayBuf1[8][128] = {0};
uint8_t DisplayBuf2[8][128] = {0};
uint8_t dataBuf1[8][129]={0};
uint8_t dataBuf2[8][129]={0};
uint8_t CommandBuffer[2]={0x00};

void OLED_WriteCommand(uint8_t cmd)
{
	CommandBuffer[1]=cmd;
	HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, CommandBuffer, 2, HAL_MAX_DELAY);
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
	OLED_chear();
	for (uint8_t i = 0; i < 8; ++i)
	{
		dataBuf1[i][0]=0x40;
		dataBuf2[i][0]=0x40;
	}
}

void OLED_chear(void)
{
	for (int i = 0; i < 8; ++i)
	{
//		OLED_SetCursor(i, 0);
		for (int j = 0; j < 128; ++j)
		{
//			OLED_WriteData(0x00);
			DisplayBuf1[i][j] = 0x00;
		}
	}
}

void OLED_updata()
{
	for (uint8_t i = 0; i < 8; ++i)
	{
		memcpy( (&dataBuf1[i][0])+1, DisplayBuf1[i], 128);
		memcpy( (&dataBuf2[i][0])+1, DisplayBuf2[i], 128);
	}
	for (uint8_t i = 0; i < 8; ++i)
	{
		if(memcmp(&dataBuf1[i][0],&dataBuf2[i][0],129) != 0)
		{
			OLED_SetCursor(i, 0);
			HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, &dataBuf1[i][0], 129, HAL_MAX_DELAY);
			memcpy(DisplayBuf2[i], DisplayBuf1[i], 128);
		}
	}
}

void OLED_reverse(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
	uint8_t i, j;

	if (X > 127) {return;}
	if (Y > 63) {return;}
	if (X + Width > 128) {Width = 128 - X;}
	if (Y + Height > 64) {Height = 64 - Y;}

	for (j = Y; j < Y + Height; j ++)		//遍历指定页
	{
		for (i = X; i < X + Width; i ++)	//遍历指定列
		{
			DisplayBuf1[j / 8][i] ^= 0x01 << (j % 8);	//将显存数组指定数据取反
		}
	}
}

void OLED_animation_down(uint8_t Y)
{
	for (uint8_t var = 0; var < 16; ++var)
	{
		OLED_reverse(0, Y, 127, 1);
		OLED_reverse(0, Y+16, 127, 1);
		OLED_updata();
		++Y;
	}
}

void OLED_animation_up(uint8_t Y)
{
	for (uint8_t var = 0; var < 16; ++var)
	{
		OLED_reverse(0, Y-1, 127, 1);
		OLED_reverse(0, Y+15, 127, 1);
		OLED_updata();
		--Y;
	}
}

void OLED_showChar(uint8_t Page, uint8_t X,char a,uint8_t fontSize)
{
	OLED_SetCursor(Page, X);
	if (fontSize == 8)
	{
		OLED_showImage(Page, X, 8, 2, EN8_16[ a - ' ' ]);
	}
	else if (fontSize == 6)
	{
		OLED_showImage(Page, X, 6, 1, EN6_8[ a - ' ' ]);
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
//		OLED_SetCursor( Page+j , X);
		for (int i = 0; i < Width; ++i)
		{
//			OLED_WriteData(Image[Width*j+i]);
			DisplayBuf1[Page+j][X+i] = Image[Width*j+i];
		}
	}
}

void OLED_showChinese(uint8_t Page, uint8_t X, char *Chinese)
{
	uint8_t perchinese[3];
	uint8_t pointAllChinese = 0;
	uint8_t stopseek[3]={0xff,0xff,0xff};
	uint8_t seek = 0;
	uint8_t locateX = X;
	while (1)
	{
		seek = 0;
		for(uint8_t i=0;i<3 && Chinese[i]!='\0';i++)//utf8的编码下，一个汉字占用3字节，这个循环取出字符串中的一个汉字的utf8编码
		{
			perchinese[i] = Chinese[pointAllChinese];
			pointAllChinese++;
		}
		for (; memcmp(stopseek,cn16_16[seek].ch_utf8,3) != 0 /*字库中最后一个字是stopseek，代表没有找到，此时跳出循环*/; ++seek)
		{
			if( memcmp(perchinese, cn16_16[seek].ch_utf8 ,3) == 0 )//将取出的3字节与字库中的字对比
			{
				break;
			}
		}
		OLED_showImage(Page, locateX, 16, 1, cn16_16[seek].ch_dt1);//显示汉字第一部分
		OLED_showImage(Page+1, locateX, 16, 1, cn16_16[seek].ch_dt2);//显示汉字第二部分
		locateX += 16;
		if(Chinese[pointAllChinese]==0x00){break;}
	}
}

void OLED_showNUM(uint8_t Page, uint8_t X, uint32_t num, uint8_t fontSize)
{
	char buffer[10];
	sprintf(buffer, "%lu", num);
	OLED_showString(Page, X, buffer, fontSize);
}

void OLED_showSignedNUM(uint8_t Page, uint8_t X, int32_t num, uint8_t fontSize)
{
	char buffer[11];
	sprintf(buffer, "%ld", num);
	OLED_showString(Page, X, buffer, fontSize);
}

//void OLED_showSignedFlout(uint8_t Page, uint8_t X, float num, uint8_t fontSize)
//{
//	char buffer[6];
//	sprintf(buffer, "%5.2f", num);
//	OLED_showString(Page, X, buffer, fontSize);
//}

void OLED_showHEX(uint8_t Page, uint8_t X, uint32_t num, uint8_t fontSize)
{
	char buffer[10];
	sprintf(buffer, "0x%lx", num);
	OLED_showString(Page, X, buffer, fontSize);
}
