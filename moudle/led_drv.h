#ifndef _LED_DRV_H
#define _LED_DRV_H

#include "platform.h"

#define LED_PIN 25

void led_init(void);


#define LED_ON()  gpio_put(LED_PIN, true)
#define LED_OFF() gpio_put(LED_PIN, false)

#endif // !1