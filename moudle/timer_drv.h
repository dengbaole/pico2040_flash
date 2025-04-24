#ifndef _TIMER_DRV_H
#define _TIMER_DRV_H

#include "platform.h"

bool timer_10ms_callback(struct repeating_timer* t);
bool timer_100ms_callback(struct repeating_timer* t);


#endif // !1