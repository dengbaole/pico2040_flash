#ifndef _UART_DRV_H
#define _UART_DRV_H

#include "platform.h"


#define UART_ID uart1
#define BAUD_RATE 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 4
#define UART_RX_PIN 5


void user_uart_init(void);
void uart_printf(const char* fmt, ...);

#endif // !1