#include "platform.h"

#if G_LOG_ENABLED == 1
	char log_cache[128];
	char log_buffer[512];
	uint16_t log_ptr = 0;
	int16_t log_length;
#endif





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
	static uint8_t display_num_oled = 0;
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
				oled_draw(0, 0, font_5H_num_array[display_num_oled]);
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
				oled_draw(0, 0, font_5H_num_array[display_num_oled]);
				oled_draw(2, dino_2_h, &dino_back_leg_bmp);
				oled_update();
			}
			break;
		case UEVT_TIMER_100MS:

			break;
		case BUTTON1_ON:
			dino_1_h = 0;
			display_num_oled = 1;
			break;
		case BUTTON2_ON:
			dino_2_h = 0;
			display_num_oled = 2;
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
			led_init();
			user_uart_init();
			flash_id = W25Q128_ReadID(); //读取FLASH ID.
			uart_printf("%x\n", flash_id);
			// LOG_RAW("%x\n", flash_id);
			// tusb_init();
			// cdc_log_init();

			break;
		case UEVT_TIMER_10MS:
			t_10ms++;
			if(t_10ms % 3 == 0 && tft_state == 0) {
				// LCD_ShowPicture(0, 0, charge_array[t_10ms / 3 % 30]);
				// lcd_draw_flash(25600 * (t_10ms / 3 % 125));
				// lcd_draw_flash2(&flash_charge_array[t_10ms / 100 % 26]);
				static int x = 0;
				static int y = 0;
				static int vx = 2;  // 水平方向速度
				static int vy = 1;  // 垂直方向速度
				uint8_t	index = 0;
				 // 更新位置
				x += vx;
				y += vy;
				// 边界检测和反射
				if (x <= 0 || x >= 80 - flash_letter_array[0].w) {
					vx = -vx;
					x += vx;  // 防止越界
				}
				if (y <= 0 || y >= 160 - flash_letter_array[0].h) {
					vy = -vy;
					y += vy;  // 防止越界
				}

				
				// index = display_num(index, 8,  100, torbo_num_bitmap, old_key_value);
				index = set_display_component(index, 0, 0, &flash_timeout_array[t_10ms / 3 % 30]);
				index = display_num(index,0,0,flash_nos_9_12_array,t_10ms / 3 % 999);
				index = set_display_component(index, x, y, &flash_letter_array[t_10ms / 30 % 26]);
				index = set_display_component(index, 0, 0, NULL);
				display_component(default_component);
			}
			// if(t_10ms % 100 == 0) {
			// 	LCD_ShowPicture(0, 0, nos_9_12_array[t_10ms / 100 % 10]);
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
				// LOG_RAW("line1 = %d\r\n", t_10ms / 100);
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
				uart_printf("flash id error  = 0x%x\n",flash_id);
			}
			LED_OFF();
			// flash_write_bitmap_array(charge_array);
			// flash_write_bitmap_array(timeout_array);
			// flash_write_bitmap_array(lock_array);
			// flash_write_bitmap_array(lowpower_array);
			// flash_write_bitmap_array(nopod_array);
			// flash_write_bitmap_array(power_on_array);
			// flash_write_bitmap_array(smoke_array);
 			// flash_write_bitmap_array(power_off_array);
			// flash_write_bitmap_array(letter_array);
			// flash_write_bitmap_array(turbo_9_12_array);
			// flash_write_bitmap_array(nos_9_12_array);
			LED_ON();
			flash_address = 0;
			uart_printf("flash burn end! flash id  = 0x%x\n",flash_id);
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

