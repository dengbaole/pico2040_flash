#include "platform.h"

#if G_LOG_ENABLED == 1
	char log_cache[128];
	char log_buffer[512];
	uint16_t log_ptr = 0;
	int16_t log_length;
#endif

uint32_t flash_address = 0;
uint32_t picture_address = 0;



static char serial_fifo[16];
static uint8_t serial_wp = 0;
uint8_t serial_got(const char* str) {
	uint8_t len = strlen(str);
	for(uint8_t i = 1; i <= len; i++) {
		if(serial_fifo[(serial_wp + (0x10 - i)) & 0xF] != str[len - i]) {
			return 0;
		}
	}
	return 1;
}
#include "pico/bootrom.h"
void serial_receive(uint8_t const* buffer, uint16_t bufsize) {
	for(uint16_t i = 0; i < bufsize; i++) {
		if((buffer[i] == 0x0A) || (buffer[i] == 0x0D)) {
			if(serial_got("BOOTLOADER")) {
				reset_usb_boot(0, 0);
			}
			if(serial_got("POWERON")) {
				uevt_bc_e(TFT_POWER_ON);
			}
			if(serial_got("SMOKE")) {
				uevt_bc_e(TFT_SMOKE);
			}
			if(serial_got("CHARGE")) {
				uevt_bc_e(TFT_CHARGE);
			}
			if(serial_got("TIMEOUT")) {
				uevt_bc_e(TFT_TIMEOUT);
			}
			if(serial_got("NOPOD")) {
				uevt_bc_e(TFT_NOPOD);
			}
			if(serial_got("LOWPOWER")) {
				uevt_bc_e(TFT_LOWPOWER);
			}
			if(serial_got("LOCK")) {
				uevt_bc_e(TFT_LOCK);
			}
			if(serial_got("FLASH")) {
				uevt_bc_e(FLASH_BURN);
			}
		} else {
			serial_fifo[serial_wp++ & 0xF] = buffer[i];
		}
	}
}


void oled_handle(uevt_t* evt) {
	static uint16_t t_10ms = 0;
	static uint8_t dino_1_h = 8;
	static uint8_t dino_2_h  = 8;
	static uint8_t display_num = 0;
	// 是否在刷新屏幕
	switch(evt->evt_id) {
		case UEVT_SYS_BOOT:
			i2c_init(i2c0, SSD1306_I2C_CLK * 1000);
			gpio_set_function(SSD1306_SDA_PIN, GPIO_FUNC_I2C);
			gpio_set_function(SSD1306_SCL_PIN, GPIO_FUNC_I2C);
			gpio_pull_up(SSD1306_SDA_PIN);
			gpio_pull_up(SSD1306_SCL_PIN);
			gpio_init(SSD1306_GED_PIN);
			gpio_set_dir(SSD1306_GED_PIN, GPIO_OUT);
			gpio_put(SSD1306_GED_PIN, 0);
			gpio_init(SSD1306_VCC_PIN);
			gpio_set_dir(SSD1306_VCC_PIN, GPIO_OUT);
			gpio_put(SSD1306_VCC_PIN, 1);
			SSD1306_init();

			oled_clean_buff();

			oled_update();
			uevt_bc_e(UEVT_APP_NEWSTATE);
			break;
		case UEVT_TIMER_10MS:
			t_10ms++;
			if(t_10ms % 20 == 0) {
				if(dino_1_h < 38) {
					dino_1_h++;
				}
				if(dino_2_h < 38) {
					dino_2_h++;
				}
				oled_clean_buff();
				oled_draw(0, 0, font_5H_num_array[display_num]);
				oled_draw(2, dino_2_h, &dino_front_leg_bmp);
				oled_update();
			}
			if(t_10ms % 20 == 10) {
				if(dino_1_h < 38) {
					dino_1_h++;
				}
				if(dino_2_h < 38) {
					dino_2_h++;
				}
				oled_clean_buff();
				oled_draw(0, 0, font_5H_num_array[display_num]);
				oled_draw(2, dino_2_h, &dino_back_leg_bmp);
				oled_update();
			}
			break;
		case UEVT_TIMER_100MS:

			break;
		case BUTTON1_ON:
			dino_1_h = 0;
			display_num = 1;
			break;
		case BUTTON2_ON:
			dino_2_h = 0;
			display_num = 2;
			break;
	}
}


void lcd_handle(uevt_t* evt) {
	static uint16_t t_10ms = 0;
	static uint16_t tft_state = 0;
	// static uint16_t tft_state = 0;
	// 是否在刷新屏幕
	switch(evt->evt_id) {
		case UEVT_SYS_BOOT:
			flash_gpio_init();
			user_spi_init();
			tftInit();

			flash_id = W25Q128_ReadID(); //读取FLASH ID.
			LOG_RAW("%x\n", flash_id);
			tusb_init();
			cdc_log_init();
			break;
		case UEVT_TIMER_10MS:
			t_10ms++;
			if(t_10ms % 3 == 0 && tft_state == 0) {
				// LCD_ShowPicture(0, 0, charge_array[t_10ms / 3 % 30]);
				lcd_draw_flash(25600 * (t_10ms / 3 % 125));
			}
			// if(t_10ms % 3 == 0 && tft_state == 1) {
			// 	LCD_ShowPicture(0, 0, lock_array[t_10ms / 3 % 30]);
			// }
			// if(t_10ms % 3 == 0 && tft_state == 2) {
			// 	LCD_ShowPicture(0, 0, lowpower_array[t_10ms / 3 % 15]);
			// }
			// if(t_10ms % 3 == 0 && tft_state == 3) {
			// 	LCD_ShowPicture(0, 0, nopod_array[t_10ms / 3 % 25]);
			// }
			// if(t_10ms % 3 == 0 && tft_state == 4) {
			// 	LCD_ShowPicture(0, 0, power_on_array[t_10ms / 3 % 35]);
			// }
			// if(t_10ms % 3 == 0 && tft_state == 5) {
			// 	LCD_ShowPicture(0, 0, smoke_array[t_10ms / 3 % 54]);
			// }
			// if(t_10ms % 3 == 0 && tft_state == 6) {
			// 	LCD_ShowPicture(0, 0, timeout_array[t_10ms / 3 % 30]);
			// }
			if(t_10ms % 100 == 0) {
				LOG_RAW("line1 = %d\r\n", t_10ms / 100);
				// LOG_RAW("%x\n", flash_id);
			}
			break;
		case UEVT_TIMER_100MS:
			t_10ms++;
			break;
		case TFT_CHARGE:
			tft_state = 0;
			break;
		case TFT_LOCK:
			tft_state = 1;
			break;
		case TFT_LOWPOWER:
			tft_state = 2;
			break;
		case TFT_NOPOD:
			tft_state = 3;
			break;
		case TFT_POWER_ON:
			tft_state = 4;
			break;
		case TFT_SMOKE:
			tft_state = 5;
			break;
		case TFT_TIMEOUT:
			tft_state = 6;
			break;
		case FLASH_BURN: {
			//todo 从零开始烧录图片
			if (flash_id != 0xEF13 && flash_id != 0xEF14 && flash_id != 0xEF15 && flash_id != 0xEF16 && flash_id != 0xEF17) {
				LOG_RAW("flash id error\n");
				return;
			}

			for(uint8_t i = 0; i < 30; i++) {
				const sBITMAP* charge = charge_array[i];
				W25Q128_WriteData(charge -> map, flash_address, charge -> h * charge -> w * 2);
				picture_address = flash_address;
				flash_address += charge -> h * charge -> w * 2;
			}
			for(uint8_t i = 0; i < 30; i++) {
				const sBITMAP* charge = lock_array[i];
				W25Q128_WriteData(charge -> map, flash_address, charge -> h * charge -> w * 2);
				picture_address = flash_address;
				flash_address += charge -> h * charge -> w * 2;
			}
			for(uint8_t i = 0; i < 30; i++) {
				const sBITMAP* charge = timeout_array[i];
				W25Q128_WriteData(charge -> map, flash_address, charge -> h * charge -> w * 2);
				picture_address = flash_address;
				flash_address += charge -> h * charge -> w * 2;
			}
			for(uint8_t i = 0; i < 35; i++) {
				const sBITMAP* charge = power_on_array[i];
				W25Q128_WriteData(charge -> map, flash_address, charge -> h * charge -> w * 2);
				picture_address = flash_address;
				flash_address += charge -> h * charge -> w * 2;
			}
			// static uint8_t i = 0;

			// const sBITMAP *charge = charge_array[i];
			// W25Q128_WriteData(charge -> map, flash_address, charge -> h * charge -> w * 2);
			// picture_address = flash_address;
			// flash_address += charge -> h * charge -> w * 2;
			// lcd_draw_flash(i * 25600);
			// i++;
		}
		break;
		default:
			break;

	}
}


void timer_handle(uevt_t* evt) {
	// 是否在刷新屏幕
	static struct repeating_timer timer_10ms;
	static struct repeating_timer timer_100ms;
	switch(evt->evt_id) {
		case UEVT_SYS_BOOT:
			add_repeating_timer_ms(10, timer_10ms_callback, NULL, &timer_10ms);
			add_repeating_timer_ms(100, timer_100ms_callback, NULL, &timer_100ms);
			break;
		case UEVT_APP_NEWSTATE:

			// uevt_bc_e(UEVT_APP_NEWSTATE);
			break;
	}
}

void button_handle(uevt_t* evt) {
	// 是否在刷新屏幕
	switch(evt->evt_id) {
		case UEVT_SYS_BOOT:
			button_init();
			break;
		case UEVT_APP_NEWSTATE:

			break;
		case UEVT_SYS_SLEEP:

			break;
	}
}



void moudle_init(void) {
	// user_event_handler_regist(oled_handle);
	user_event_handler_regist(button_handle);
	user_event_handler_regist(lcd_handle);
	user_event_handler_regist(timer_handle);

}

