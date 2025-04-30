#include "tft_drv.h"

// 寄存器
#define ILI9341_NOP         0x00 // No-op register
#define ILI9341_SWRESET     0x01 // Software reset register
#define ILI9341_RDDID       0x04 // Read display identification information
#define ILI9341_RDDST       0x09 // Read Display Status
#define ILI9341_SLPIN       0x10 // Enter Sleep Mode
#define ILI9341_SLPOUT      0x11 // Sleep Out
#define ILI9341_PTLON       0x12 // Partial Mode ON
#define ILI9341_NORON       0x13 // Normal Display Mode ON

#define ILI9341_RDMODE      0x0A // Read Display Power Mode
#define ILI9341_RDMADCTL    0x0B // Read Display MADCTL
#define ILI9341_RDPIXFMT    0x0C // Read Display Pixel Format
#define ILI9341_RDIMGFMT    0x0D // Read Display Image Format
#define ILI9341_RDSELFDIAG  0x0F // Read Display Self-Diagnostic Result

#define ILI9341_INVOFF      0x20 // Display Inversion OFF
#define ILI9341_INVON       0x21 // Display Inversion ON
#define ILI9341_GAMMASET    0x26 // Gamma Set
#define ILI9341_DISPOFF     0x28 // Display OFF
#define ILI9341_DISPON      0x29 // Display ON

#define ILI9341_CASET       0x2A // Column Address Set
#define ILI9341_PASET       0x2B // Page Address Set
#define ILI9341_RAMWR       0x2C // Memory Write
#define ILI9341_RAMRD       0x2E // Memory Read

#define ILI9341_PTLAR       0x30 // Partial Area
#define ILI9341_VSCRDEF     0x33 // Vertical Scrolling Definition
#define ILI9341_MADCTL      0x36 // Memory Access Control
#define ILI9341_VSCRSADD    0x37 // Vertical Scrolling Start Address
#define ILI9341_PIXFMT      0x3A // Pixel Format Set

#define ILI9341_FRMCTR1     0xB1 // Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9341_FRMCTR2     0xB2 // Frame Rate Control (In Idle Mode/8 colors)
#define ILI9341_FRMCTR3     0xB3 // Frame Rate control (In Partial Mode/Full Colors)
#define ILI9341_INVCTR      0xB4 // Display Inversion Control
#define ILI9341_DFUNCTR     0xB6 // Display Function Control

#define ILI9341_PWCTR1      0xC0 // Power Control 1
#define ILI9341_PWCTR2      0xC1 // Power Control 2
#define ILI9341_PWCTR3      0xC2 // Power Control 3
#define ILI9341_PWCTR4      0xC3 // Power Control 4
#define ILI9341_PWCTR5      0xC4 // Power Control 5
#define ILI9341_VMCTR1      0xC5 // VCOM Control 1
#define ILI9341_VMCTR2      0xC7 // VCOM Control 2

#define ILI9341_RDID1       0xDA // Read ID 1
#define ILI9341_RDID2       0xDB // Read ID 2
#define ILI9341_RDID3       0xDC // Read ID 3
#define ILI9341_RDID4       0xD3 // Read ID 4

#define ILI9341_GMCTRP1     0xE0 // Positive Gamma Correction
#define ILI9341_GMCTRN1     0xE1 // Negative Gamma Correction

#define ILI9341_PWCTLA      0xCB // Power control A
#define ILI9341_PWCTLB      0xCF // Power control B
#define ILI9341_DTCTLA      0xE8 // Driver timing control A
#define ILI9341_DTCTLB      0xEA // Driver timing control B
#define ILI9341_PWONSEQCTL  0xED // Power on sequence control
#define ILI9341_ENABLE3G    0xF2 // Enable 3 gamma control
#define ILI9341_PUMPRTCTL   0xF7 // Pump ratio control



#define ST7735S_SWRESET 0x01
#define ST7735S_SLPOUT  0x11
#define ST7735S_FRMCTR1 0xB1
#define ST7735S_FRMCTR2 0xB2
#define ST7735S_FRMCTR3 0xB3
#define ST7735S_INVCTR  0xB4
#define ST7735S_PWCTR1  0xC0
#define ST7735S_PWCTR2  0xC1
#define ST7735S_PWCTR3  0xC2
#define ST7735S_PWCTR4  0xC3
#define ST7735S_PWCTR5  0xC4
#define ST7735S_VMCTR1  0xC5
#define ST7735S_INVOFF  0x20
#define ST7735S_MADCTL  0x36
#define ST7735S_COLMOD  0x3A
#define ST7735S_DISPON  0x29
#define ST7735S_CASET   0x2A
#define ST7735S_RASET   0x2B
#define ST7735S_RAMWR   0x2C


// 寄存器0x36掩码
#define MADCTL_MY 0x80  // 纵向交换
#define MADCTL_MX 0x40  // 横向交换
#define MADCTL_MV 0x20  // 纵横交换
#define MADCTL_ML 0x10  // 从下到上刷新
#define MADCTL_RGB 0x00 // RGB
#define MADCTL_BGR 0x08 // BGR
#define MADCTL_MH 0x04  // 从右到左刷新

// 全局变量
sTftDevice tftDevice = {
	.width = LCD_WIDTH,
	.height = LCD_HEIGHT
};


static uint blPwmSlice;

static inline void tftsetdcandcs(bool dc, bool cs) {
	// gpio_put_masked((1u << PIN_DC) | (1u << PIN_CS), (dc << PIN_DC) | (cs << PIN_CS));
	gpio_put(PIN_DC, dc);
	gpio_put(PIN_CS, cs);
}

static inline void tft_write_cmd(const uint8_t* cmd, size_t len) {
	// 拉低CS、DC
	tftsetdcandcs(0, 0);
	spi_write_blocking(SPI_PORT, cmd, 1);
	tftsetdcandcs(0, 1);

	// 如果带参数
	if (len > 1) {
		tftsetdcandcs(1, 0);
		spi_write_blocking(SPI_PORT, &cmd[1], len - 1);
		tftsetdcandcs(1, 1);
	}
}


static inline void tft_write_data8(const uint8_t data) {
	tftsetdcandcs(1, 0);  // DC=1表示数据，CS拉低开始传输

	spi_write_blocking(SPI_PORT, &data, 1);

	tftsetdcandcs(1, 1);  // CS拉高结束传输
}


static inline void tft_write_data(const uint16_t data) {
	tftsetdcandcs(1, 0);

	uint8_t dt[2];
	dt[0] = (data >> 8) & 0xff;
	dt[1] = data & 0xff;

	spi_write_blocking(SPI_PORT, dt, 2);

	tftsetdcandcs(1, 1);
}

static void tft_gpio_init(void) {
    gpio_init(PIN_CS);
	gpio_init(PIN_BK);
	gpio_init(PIN_DC);
	gpio_init(PIN_RST);

	gpio_set_dir(PIN_CS, true);
	gpio_set_dir(PIN_BK, true);
	gpio_set_dir(PIN_DC, true);
	gpio_set_dir(PIN_RST, true);

	// CS拉高
	gpio_put(PIN_CS, true);
	gpio_put(PIN_DC, true);
}



void tftInit(void) {
	// 初始化SPI
	tft_gpio_init();
	// 复位屏幕
	gpio_put(PIN_RST, false);
	sleep_ms(100);
	gpio_put(PIN_RST, true);
	sleep_ms(50);

	// tft_write_cmd(&cmd,1); // SWRESET Software reset
	uint8_t cmd = 0x11;
	tft_write_cmd(&cmd, 1); //Sleep out
	sleep_ms(120);
	// cmd = 0x21;
	// tft_write_cmd(&cmd, 1);
	// cmd = 0x21;
	// tft_write_cmd(&cmd, 1);
	//----ST7735S Frame Rate---------------------//
	cmd = 0xB1;
	tft_write_cmd(&cmd, 1); //Frame rate 80Hz Frame rate=333k/((RTNA + 20) x (LINE + FPA + BPA))
	tft_write_data8(0x05);
	tft_write_data8(0x3A);
	tft_write_data8(0x3A);
	cmd = 0xB2;
	tft_write_cmd(&cmd, 1); //Frame rate 80Hz
	tft_write_data8(0x05);
	tft_write_data8(0x3A);
	tft_write_data8(0x3A);
	cmd = 0xB3;
	tft_write_cmd(&cmd, 1);; //Frame rate 80Hz
	tft_write_data8(0x05);
	tft_write_data8(0x3A);
	tft_write_data8(0x3A);
	tft_write_data8(0x05);
	tft_write_data8(0x3A);
	tft_write_data8(0x3A);

	//------------------------------------Display Inversion Control-----------------------------------------//
	cmd = 0xB4;
	tft_write_cmd(&cmd, 1);
	tft_write_data8(0x03);

	//------------------------------------ST7735S Power Sequence-----------------------------------------//
	cmd = 0xC0;
	tft_write_cmd(&cmd, 1);
	// tft_write_cmd(0xC0,1);
	tft_write_data8(0x62);
	tft_write_data8(0x02);
	tft_write_data8(0x04);
	cmd = 0xC1;
	tft_write_cmd(&cmd, 1);
	// tft_write_cmd(0xC1,1);
	tft_write_data8(0xC0);
	cmd = 0xC2;
	tft_write_cmd(&cmd, 1);
	// tft_write_cmd(0xC2,1);
	tft_write_data8(0x0D);
	tft_write_data8(0x00);
	cmd = 0xC3;
	tft_write_cmd(&cmd, 1);
	// tft_write_cmd(0xC3,1);
	tft_write_data8(0x8D);
	tft_write_data8(0x6A);
	cmd = 0xC4;
	tft_write_cmd(&cmd, 1);
	// tft_write_cmd(0xC4,1);
	tft_write_data8(0x8D);
	tft_write_data8(0xEE);
	//---------------------------------End ST7735S Power Sequence---------------------------------------//
	cmd = 0xC5;
	tft_write_cmd(&cmd, 1);
	// tft_write_cmd(0xC5,1); //VCOM
	tft_write_data8(0x0E);
	cmd = 0xE0;
	tft_write_cmd(&cmd, 1);
	// tft_write_cmd(0xE0);
	tft_write_data8(0x10);
	tft_write_data8(0x0E);
	tft_write_data8(0x02);
	tft_write_data8(0x03);
	tft_write_data8(0x0E);
	tft_write_data8(0x07);
	tft_write_data8(0x02);
	tft_write_data8(0x07);
	tft_write_data8(0x0A);
	tft_write_data8(0x12);
	tft_write_data8(0x27);
	tft_write_data8(0x37);
	tft_write_data8(0x00);
	tft_write_data8(0x0D);
	tft_write_data8(0x0E);
	tft_write_data8(0x10);

	cmd = 0xE1;
	tft_write_cmd(&cmd, 1);
	// tft_write_cmd(0xE1);
	tft_write_data8(0x10);
	tft_write_data8(0x0E);
	tft_write_data8(0x03);
	tft_write_data8(0x03);
	tft_write_data8(0x0F);
	tft_write_data8(0x06);
	tft_write_data8(0x02);
	tft_write_data8(0x08);
	tft_write_data8(0x0A);
	tft_write_data8(0x13);
	tft_write_data8(0x26);
	tft_write_data8(0x36);
	tft_write_data8(0x00);
	tft_write_data8(0x0D);
	tft_write_data8(0x0E);
	tft_write_data8(0x10);

	cmd = 0x3a;
	tft_write_cmd(&cmd, 1);
	// tft_write_cmd(0x3A);
	tft_write_data8(0x05);

	// cmd = 0x36;
	// tft_write_cmd(&cmd, 1);
	// // tft_write_cmd(0x36);
	// tft_write_data8(0x08);//
	// sleep_ms(50);
	cmd = 0x29;
	tft_write_cmd(&cmd, 1);
	// tft_write_cmd(0x29);
	sleep_ms(100);


	// 设置屏幕方向
	tftSetDirection(ILI9341_DIRECTION_0);

	// 清除屏幕
	tftClear(COLOR_BLACK);

	// 打开背光
	// gpio_put(PIN_BK, true);
	// 初始化背光
	gpio_set_function(PIN_BK, GPIO_FUNC_PWM);

	blPwmSlice = pwm_gpio_to_slice_num(PIN_BK);

	float blPwmDivider = 50.0f;
	pwm_set_clkdiv(blPwmSlice, blPwmDivider);

	// 设置背光范围 0-100
	// 初始化背光 100
	pwm_set_wrap(blPwmSlice, 99);
	pwm_set_chan_level(blPwmSlice, PWM_CHAN_B, 100);
	pwm_set_enabled(blPwmSlice, true);
}

void tftSetWindows(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end) {
	uint16_t temp;

	// 横向
	uint8_t cmd = 0x2a;
	tft_write_cmd(&cmd, 1);
	tft_write_data(x_start + 24);
	tft_write_data(x_end + 24);

	// 纵向
	cmd = 0x2b;
	tft_write_cmd(&cmd, 1);
	tft_write_data(y_start + 0);
	tft_write_data(y_end + 0);

	// 开始写入屏幕
	cmd = 0x2c;
	tft_write_cmd(&cmd, 1);
}

void tftSetDirection(etftdirection dir) {
	uint8_t temp = 0;

	switch (dir) {
		case ILI9341_DIRECTION_0:
			tftDevice.width = LCD_WIDTH;
			tftDevice.height = LCD_HEIGHT;
			temp = MADCTL_MX | MADCTL_MY;
			break;

		case ILI9341_DIRECTION_90:
			tftDevice.width = LCD_HEIGHT;
			tftDevice.height = LCD_WIDTH;
			temp = MADCTL_MV;
			break;

		case ILI9341_DIRECTION_180:
			tftDevice.width = LCD_WIDTH;
			tftDevice.height = LCD_HEIGHT;
			temp = MADCTL_MY;
			break;

		case ILI9341_DIRECTION_270:
			tftDevice.width = LCD_HEIGHT;
			tftDevice.height = LCD_WIDTH;
			temp = MADCTL_MY | MADCTL_MX | MADCTL_MV;
			break;

		default:
			break;
	}

	temp |= MADCTL_BGR;

	uint8_t cmd[2] = { 0x36, temp };
	tft_write_cmd(cmd, 2);

	tftSetWindows(0, 0, tftDevice.width, tftDevice.height);
}

void tftClear(uint16_t color) {
	size_t fillSize = ILI9341_SIZE;
	tftSetWindows(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	for (size_t index = 0; index < fillSize; ++index) {
		tft_write_data(color);
	}
}

void tftplot(uint16_t x, uint16_t y, uint16_t color) {
	tftSetWindows(x, y, 1, 1);
	tft_write_data(color);
}

void tftdrawarray(uint16_t* src, size_t len) {
	for (size_t index = 0; index < len; ++index) {
		tft_write_data(src[index]);
	}
}

void tft_set_bl_brightness(uint16_t brightness) {
	pwm_set_chan_level(blPwmSlice, PWM_CHAN_B, brightness);
}


void LCD_ShowPicture(uint16_t x, uint16_t y, const sBITMAP* pic) {
	uint16_t i, j;
	uint32_t k = 0;
	tftSetWindows(x, y, x + pic->w - 1, y + pic->h - 1);

	tftsetdcandcs(1, 0);
	spi_write_blocking(SPI_PORT, pic->map, pic->h * pic->w * 2);
	tftsetdcandcs(1, 1);
}