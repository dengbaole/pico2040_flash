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
	.width = ILI9341_WIDTH,
	.height = ILI9341_HEIGHT
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

static inline void tft_write_data(const uint16_t data) {
	tftsetdcandcs(1, 0);

	uint8_t dt[2];
	dt[0] = (data >> 8) & 0xff;
	dt[1] = data & 0xff;

	spi_write_blocking(SPI_PORT, dt, 2);

	tftsetdcandcs(1, 1);
}

void tftInit(void) {
	// 初始化SPI
	user_spi_init();

	// 复位屏幕
	gpio_put(PIN_RST, false);
	sleep_ms(100);
	gpio_put(PIN_RST, true);
	sleep_ms(100);

	// 发送初始化命令
	// uint8_t cmd[] = {0x36, 0x48};
	// tft_write_cmd(cmd, 2);

	// 数量， 命令， 参数...
	// 如果数量为0x81，则延时150ms
	const uint8_t initCmd[] = {
		4, ILI9341_PWCTLB, 0x00, 0xC1, 0x30,
		5, ILI9341_PWONSEQCTL, 0x64, 0x03, 0x12, 0x81,
		4, ILI9341_DTCTLA, 0x85, 0x00, 0x78,
		6, ILI9341_PWCTLA, 0x39, 0x2C, 0x00, 0x34, 0x02,
		2, ILI9341_PUMPRTCTL, 0x20,
		3, ILI9341_DTCTLB, 0x00, 0x00,
		2, ILI9341_PWCTR1, 0x10,
		2, ILI9341_PWCTR2, 0x00,
		3, ILI9341_VMCTR1, 0x30, 0x30,
		2, ILI9341_VMCTR2, 0xB7,
		2, ILI9341_MADCTL, 0x48,
		2, ILI9341_VSCRSADD, 0x00,
		2, ILI9341_PIXFMT, 0x55,
		3, ILI9341_FRMCTR1, 0x00, 0x1B,
		4, ILI9341_DFUNCTR, 0x08, 0x82, 0x27,
		2, ILI9341_ENABLE3G, 0x00,
		2, ILI9341_GAMMASET, 0x01,
		16, ILI9341_GMCTRP1, 0x0F, 0x2A, 0x28, 0x08, 0x0E,
		0x08, 0x54, 0XA9, 0x43, 0x0A, 0x0F, 0x00, 0x00, 0x00, 0x00,
		16, ILI9341_GMCTRN1, 0x00, 0x15, 0x17, 0x07, 0x11,
		0x06, 0x2B, 0x56, 0x3C, 0x05, 0x10, 0x0F, 0x3F, 0x3F, 0x0F,
		0x81, ILI9341_SLPOUT,
		0x81, ILI9341_DISPON,
		0x00
	};

	const uint8_t* cmdIndex = initCmd;
	uint8_t count, temp;

	while (*cmdIndex) {
		temp = *cmdIndex++;
		count = temp & 0x7F;

		tft_write_cmd(cmdIndex, count);
		cmdIndex += count;

		if (0x81 == temp) {
			sleep_ms(150);
		}
	}



	// 设置屏幕方向
	tftSetDirection(ILI9341_DIRECTION_0);

	// 清除屏幕
	tftClear(COLOR_BLUE);


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

void tftSetWindows(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	uint16_t temp;

	// 横向
	temp = x + width;
	if (temp > tftDevice.width) {
		temp = tftDevice.width;
	}
	--temp;

	uint8_t cmd = 0x2a;
	tft_write_cmd(&cmd, 1);
	tft_write_data(x);
	tft_write_data(temp);

	// 纵向
	temp = y + height;
	if (temp > tftDevice.height) {
		temp = tftDevice.height;
	}
	--temp;

	cmd = 0x2b;
	tft_write_cmd(&cmd, 1);
	tft_write_data(y);
	tft_write_data(temp);

	// 开始写入屏幕
	cmd = 0x2c;
	tft_write_cmd(&cmd, 1);
}

void tftSetDirection(etftdirection dir) {
	uint8_t temp = 0;

	switch (dir) {
		case ILI9341_DIRECTION_0:
			tftDevice.width = ILI9341_WIDTH;
			tftDevice.height = ILI9341_HEIGHT;
			temp = MADCTL_MX;
			break;

		case ILI9341_DIRECTION_90:
			tftDevice.width = ILI9341_HEIGHT;
			tftDevice.height = ILI9341_WIDTH;
			temp = MADCTL_MV;
			break;

		case ILI9341_DIRECTION_180:
			tftDevice.width = ILI9341_WIDTH;
			tftDevice.height = ILI9341_HEIGHT;
			temp = MADCTL_MY;
			break;

		case ILI9341_DIRECTION_270:
			tftDevice.width = ILI9341_HEIGHT;
			tftDevice.height = ILI9341_WIDTH;
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