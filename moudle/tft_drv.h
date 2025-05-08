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


//芯片sram 8k
#define DISPLAY_WIDTH 80
#define DISPLAY_HEIGHT 160
#define SPLIT_SCREEN 16   //DISPLAY_WIDTH * DISPLAY_HEIGHT*2/SPLIT_SCREEN 需要整除
#define DISPLAY_BUF_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT*2/SPLIT_SCREEN)
extern uint8_t display_buff[DISPLAY_BUF_SIZE];
extern uint8_t rx_buff[DISPLAY_BUF_SIZE];

void tftInit(void);
void tftSetWindows(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);
void tftSetDirection(etftdirection dir);
void tftClear(uint16_t color);

void tftplot(uint16_t x, uint16_t y, uint16_t color);
void tftdrawarray(uint16_t* src, size_t len);

void tft_set_bl_brightness(uint16_t brightness);
void LCD_ShowPicture(uint16_t x, uint16_t y, const sBITMAP* pic);
void lcd_draw_flash(uint32_t addr);
// #include "flash_t_tft.h"
void lcd_draw_flash2(const FLASH_sBITMAP* pic);
void display_component(FLASH_sBITMAP_TABLE* bitmap_table);
#endif // !1