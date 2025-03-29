/*
 * ws2812.c
 *
 *  Created on: Mar 18, 2025
 *      Author: 28584
 */

#include "ws2812.h"

uint16_t data[DATA_ARRAY_NUM] = {0};
static uint32_t colorTable[360];

void ws2812Updata()
{
	for (int i = 0; i < DATA_ARRAY_NUM; ++i)
	{
		if(data[i]==1){data[i]=code1;}
		if(data[i]==0){data[i]=code0;}
	}
	data[DATA_ARRAY_NUM-1] = reset;
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*)data, sizeof(data)/sizeof(uint16_t) );
}

void initColorTable()
{
    const uint8_t saturation = 255; // 固定饱和度
    const uint8_t value = 50;       // 固定明度

    for(int h=0; h<360; h++) {
        colorTable[h] = hsv2rgb(h, saturation, value);
    }
}

void ws2812SetOneLED(uint16_t LED_bit, uint32_t color)//RGB
{
	uint8_t G=0, R=0, B=0, count=0;
	B |= color & 0xFF;
	G |= (color >> 8) & 0xFF;
	R |= (color >> 16) & 0xFF;

	for (uint64_t var = (LED_bit-1)*24; var < (LED_bit-1)*24+24 && count<24 ; ++var)
	{
		if(count<8){data[var]=(G >> (7 - count)) & 1;}
		if(count>=8 && count<16){data[var]=(R >> (15 - count)) & 1;}
		if(count>=16 && count<24){data[var]=(B >> (23 - count)) & 1;}
		count++;
	}
}

void ws2812SetMultipleLED(uint16_t Start_LED_bit, uint16_t Stop_LED_bit, uint32_t color)
{
	for (uint16_t var = Start_LED_bit; var < Stop_LED_bit+1; ++var)
	{
		ws2812SetOneLED(var, color);
	}
}

uint32_t hsv2rgb(uint16_t h, uint8_t s, uint8_t v) {
    uint8_t r, g, b;
    uint8_t region, remainder;
    uint16_t p, q, t;

    if (s == 0) {
        r = v;
        g = v;
        b = v;
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    }

    region = h / 60;
    remainder = (h % 60) * 4;

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region) {
        case 0:  r = v; g = t; b = p; break;
        case 1:  r = q; g = v; b = p; break;
        case 2:  r = p; g = v; b = t; break;
        case 3:  r = p; g = q; b = v; break;
        case 4:  r = t; g = p; b = v; break;
        default: r = v; g = p; b = q; break;
    }

    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

// 彩虹流水灯效果
void rainbowFlowOptimized(uint16_t numLEDs, uint8_t speed) {
    static uint16_t hue = 0;

    for(uint16_t i=0; i<numLEDs; i++) {
        uint16_t ledHue = (hue + (i * 10)) % 360; // 计算偏移后的色相
        uint32_t color = colorTable[ledHue];       // 直接查表获取颜色
        ws2812SetOneLED(i+1, color);
    }

    hue = (hue + speed) % 360; // 更新基准色相
    ws2812Updata();
}
