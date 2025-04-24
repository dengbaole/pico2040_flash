#include "spi_drv.h"

void user_spi_init(void) {
	spi_init(SPI_PORT, SPI_BAUDRATE);
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
	// 设置SPi的功能管脚
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
	gpio_set_function(PIN_SCL, GPIO_FUNC_SPI);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

}