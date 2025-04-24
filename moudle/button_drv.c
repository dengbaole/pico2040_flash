#include "button_drv.h"


void button_callback(unsigned int gpio, uint32_t events) {
    if (gpio == KEY2_PIN && (events & GPIO_IRQ_EDGE_FALL)) {
        // 按键2按下
        uevt_bc_e(BUTTON2_ON);
    }
    if (gpio == KEY1_PIN && (events & GPIO_IRQ_EDGE_FALL)) {
        // 按键1按下
        uevt_bc_e(BUTTON1_ON);
    }
}


void button_init(void) {
	gpio_init(KEY2_PIN);
	gpio_set_dir(KEY2_PIN, GPIO_IN);
	gpio_pull_up(KEY2_PIN);
	gpio_init(KEY2_GND_PIN);
	gpio_set_dir(KEY2_GND_PIN, GPIO_OUT);
	gpio_put(KEY2_GND_PIN, 0);
	// 设置中断
	gpio_set_irq_enabled_with_callback(KEY2_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);

	gpio_init(KEY1_PIN);
	gpio_set_dir(KEY1_PIN, GPIO_IN);
	gpio_pull_up(KEY1_PIN);
	gpio_init(KEY1_GND_PIN);
	gpio_set_dir(KEY1_GND_PIN, GPIO_OUT);
	gpio_put(KEY1_GND_PIN, 0);

	// 设置中断
	gpio_set_irq_enabled_with_callback(KEY1_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);

}



void button_scan(void) {
    // 检查按键状态
    if (gpio_get(KEY2_PIN) == 0) {
        // 按键2按下
        uevt_bc_e(BUTTON2_ON);
    }
    if (gpio_get(KEY1_PIN) == 0) {
        // 按键1按下
        uevt_bc_e(BUTTON1_ON);
    }
}