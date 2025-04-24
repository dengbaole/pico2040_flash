#include "timer_drv.h"

bool timer_10ms_callback(struct repeating_timer* t) {
	uevt_bc_e(UEVT_TIMER_10MS);
	return true;
}

bool timer_100ms_callback(struct repeating_timer* t) {
	uevt_bc_e(UEVT_TIMER_100MS);
	return true;
}


