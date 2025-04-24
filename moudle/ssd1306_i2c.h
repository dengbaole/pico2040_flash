#ifndef _SSD1306_IIC_H
#define _SSD1306_IIC_H

#include "platform.h"
#include "bitmap.h"

#define SSD1306_HEIGHT              64
#define SSD1306_WIDTH               128

#define SSD1306_I2C_ADDR            _u(0x3C)

// 400 is usual, but often these can be overclocked to improve display response.
// Tested at 1000 on both 32 and 84 pixel height devices and it worked.
#define SSD1306_I2C_CLK             400
// #define SSD1306_I2C_CLK             1000


// commands (see datasheet)
#define SSD1306_SET_MEM_MODE        _u(0x20)
#define SSD1306_SET_COL_ADDR        _u(0x21)
#define SSD1306_SET_PAGE_ADDR       _u(0x22)
#define SSD1306_SET_HORIZ_SCROLL    _u(0x26)
#define SSD1306_SET_SCROLL          _u(0x2E)

#define SSD1306_SET_DISP_START_LINE _u(0x40)

#define SSD1306_SET_CONTRAST        _u(0x81)
#define SSD1306_SET_CHARGE_PUMP     _u(0x8D)

#define SSD1306_SET_SEG_REMAP       _u(0xA0)
#define SSD1306_SET_ENTIRE_ON       _u(0xA4)
#define SSD1306_SET_ALL_ON          _u(0xA5)
#define SSD1306_SET_NORM_DISP       _u(0xA6)
#define SSD1306_SET_INV_DISP        _u(0xA7)
#define SSD1306_SET_MUX_RATIO       _u(0xA8)
#define SSD1306_SET_DISP            _u(0xAE)
#define SSD1306_SET_COM_OUT_DIR     _u(0xC0)
#define SSD1306_SET_COM_OUT_DIR_FLIP _u(0xC0)

#define SSD1306_SET_DISP_OFFSET     _u(0xD3)
#define SSD1306_SET_DISP_CLK_DIV    _u(0xD5)
#define SSD1306_SET_PRECHARGE       _u(0xD9)
#define SSD1306_SET_COM_PIN_CFG     _u(0xDA)
#define SSD1306_SET_VCOM_DESEL      _u(0xDB)

#define SSD1306_PAGE_HEIGHT         _u(8)
#define SSD1306_NUM_PAGES           (SSD1306_HEIGHT >> 3)
#define SSD1306_BUF_LEN             (SSD1306_NUM_PAGES * SSD1306_WIDTH)

#define SSD1306_WRITE_MODE         _u(0xFE)
#define SSD1306_READ_MODE          _u(0xFF)


typedef struct render_area {
	uint8_t start_col;
	uint8_t end_col;
	uint8_t start_page;
	uint8_t end_page;
	int buflen;
} render_area;


extern render_area frame_area;
extern uint8_t ssd1306_buff[SSD1306_BUF_LEN];

void calc_render_area_buflen(render_area* area);
void SSD1306_send_cmd(uint8_t cmd);
void SSD1306_send_cmd_list(uint8_t* buf, int num);
void SSD1306_send_buf(uint8_t buf[], int buflen);
void SSD1306_init(void);
void frame_area_init(void);
void SSD1306_scroll(bool on);
void render(uint8_t* buf, render_area* area);
void SetPixel(uint8_t* buf, int x, int y, bool on);
// Basic Bresenhams.
void DrawLine(uint8_t* buf, int x0, int y0, int x1, int y1, bool on);
uint8_t reverse(uint8_t b);
void FillReversedCache();
void WriteChar(uint8_t* buf, int16_t x, int16_t y, uint8_t ch) ;
void WriteString(uint8_t* buf, int16_t x, int16_t y, char* str);
void oled_clean_buff(void);
void oled_draw(uint8_t x, uint8_t y, const sBITMAP* P);
void oled_update(void);

#endif // !1