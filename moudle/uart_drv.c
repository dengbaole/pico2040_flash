#include "uart_drv.h"
#include <stdarg.h>

#define UART_LOG_BUF_SIZE 128


void user_uart_init(void) {
	uart_init(UART_ID, BAUD_RATE);
	gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
	gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}


void uart_printf(const char* fmt, ...) {
	char buffer[UART_LOG_BUF_SIZE];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	uart_puts(UART_ID, buffer);
}