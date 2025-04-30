#include "spi_drv.h"

void user_spi_init(void) {
	spi_init(SPI_PORT, SPI_BAUDRATE);

	// 设置SPi的功能管脚
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
	gpio_set_function(PIN_SCL, GPIO_FUNC_SPI);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
}






