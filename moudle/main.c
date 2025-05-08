#include "platform.h"






int main(void) {
	stdio_init_all();
	app_sched_init();
	user_event_init();


	moudle_init();
	uevt_bc_e(UEVT_SYS_BOOT);
	while (true) {
		app_sched_execute();
		// tud_task();
		// cdc_task();
		__wfi();
	}
}
