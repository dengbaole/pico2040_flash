#ifndef _PLATFORM_H
#define _PLATFORM_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>



#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/spi.h"
#include "hardware/pwm.h"
#include "hardware/uart.h"

#include "scheduler/uevent.h"
#include "scheduler/scheduler.h"


#include "tusb.h"
#include "usb_func.h"
#include "tusb_config.h"

#include "bitmap.h"
#include "flash_t_tft.h"
#include "tft_drv.h"
#include "gpio_drv.h"
#include "ssd1306_i2c.h"
#include "iic_drv.h"
#include "ssd1306_font.h"
#include "spi_drv.h"
#include "timer_drv.h"


#include "oled_image.h"
#include "tft_image.h"
#include "flash_drv.h"
#include "button_drv.h"
#include "uart_drv.h"


extern uint32_t flash_address;
extern uint32_t picture_address;

void moudle_init(void);


#if G_LOG_ENABLED == 1
extern char log_cache[128];
extern char log_buffer[512];
extern uint16_t log_ptr;
extern int16_t log_length;
#define LOG_RAW(...) \
	do { \
		log_length = sprintf(log_cache, __VA_ARGS__) + 1; \
		if(log_length > 1) { \
			if(log_ptr + log_length >= 512) { \
				log_ptr = 0; \
			} \
			memcpy(log_buffer + log_ptr, log_cache, log_length); \
			cdc_log_print(log_buffer + log_ptr); \
			log_ptr += log_length; \
		} \
	} while(0);
#else
#define LOG_RAW(...)
#endif

#define UEVT_APP_BASE (0xF500)
#define UEVT_APP_NEWSTATE (UEVT_APP_BASE | 0x01)
#define UEVT_APP_STATELEAVE (UEVT_APP_BASE | 0x02)

#define UEVT_SYS_BASE (0xAE00)
#define UEVT_SYS_POWERUP (UEVT_SYS_BASE | 0x01)
#define UEVT_SYS_BOOT (UEVT_SYS_BASE | 0x0B)
#define UEVT_SYS_SETUP (UEVT_SYS_BASE | 0x02)
#define UEVT_SYS_START (UEVT_SYS_BASE | 0x03)
#define UEVT_SYS_BEFORE_SLEEP (UEVT_SYS_BASE | 0x0E)
#define UEVT_SYS_SLEEP (UEVT_SYS_BASE | 0x0C)
#define UEVT_SYS_WAKE (UEVT_SYS_BASE | 0x0D)
#define UEVT_SYS_RENDER (UEVT_SYS_BASE | 0xED)
#define UEVT_SYS_PRINT_NEXT_FREQ (UEVT_SYS_BASE | 0xF1)

#define UEVT_RTC_BASE (0x0000)
#define UEVT_TIMER_4HZ (UEVT_RTC_BASE | 0x10)
#define UEVT_TIMER_FPS (UEVT_RTC_BASE | 0x1F)
#define UEVT_TIMER_10MS (UEVT_RTC_BASE | 0x20)
#define UEVT_TIMER_100MS (UEVT_RTC_BASE | 0x2F)

#define UEVT_BUTTON_BASE (0x0100)
#define BUTTON1_ON (UEVT_BUTTON_BASE | 0x10)
#define BUTTON2_ON (UEVT_BUTTON_BASE | 0x20)


#define UEVT_TFT_BASE (0x0400)
#define TFT_POWER_ON (UEVT_TFT_BASE | 0x10)
#define TFT_SMOKE (UEVT_TFT_BASE | 0x21)
#define TFT_CHARGE (UEVT_TFT_BASE | 0x40)
#define TFT_TIMEOUT (UEVT_TFT_BASE | 0x80)
#define TFT_NOPOD (UEVT_TFT_BASE | 0x11)
#define TFT_LOWPOWER (UEVT_TFT_BASE | 0x12)
#define TFT_LOCK (UEVT_TFT_BASE | 0x14)

#define UEVT_FLASH_BASE (0x0500)
#define FLASH_BURN (UEVT_FLASH_BASE | 0x10)

#define UEVT_ADC_BASE (0x0200)
#define UEVT_ADC_TEMPERATURE_RESULT (UEVT_ADC_BASE | 0x01)



#endif // !1