#ifndef _ILL9341_DRV_H
#define _ILL9341_DRV_H

#include "platform.h"


//屏幕状态
typedef struct {
	uint16_t width;
	uint16_t height;
} sTftDevice;

extern sTftDevice tftDevice;

//屏幕方向
typedef enum {
	ILI9341_DIRECTION_0,
	ILI9341_DIRECTION_90,
	ILI9341_DIRECTION_180,
	ILI9341_DIRECTION_270
} etftdirection;

void tftInit(void);
void tftSetWindows(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end);
void tftSetDirection(etftdirection dir);
void tftClear(uint16_t color);

void tftplot(uint16_t x, uint16_t y, uint16_t color);
void tftdrawarray(uint16_t* src, size_t len);

void tft_set_bl_brightness(uint16_t brightness);
#endif // !1