#include "led_drv.h"

void led_init(void){
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, true);
	gpio_put(LED_PIN, true);
}