#ifndef _FLASH_T_TFT_H
#define _FLASH_T_TFT_H

#include "platform.h"

typedef struct FLASH_SBITMAP {
	const uint8_t w;
	const uint8_t h;
	const uint32_t map_address;
} FLASH_sBITMAP;

typedef struct FLASH_sBITMAP_TABLE {
	uint8_t x;
	uint8_t y;
	FLASH_sBITMAP* bitmap;
} FLASH_sBITMAP_TABLE;

extern FLASH_sBITMAP flash_charge_array[30];
extern FLASH_sBITMAP flash_letter_array[26];
extern FLASH_sBITMAP flash_timeout_array[30];
extern FLASH_sBITMAP flash_lock_array[30];
extern FLASH_sBITMAP flash_lowpower_array[15];
extern FLASH_sBITMAP flash_nopod_array[25];
extern FLASH_sBITMAP flash_power_on_array[35];
extern FLASH_sBITMAP flash_smoke_array[54];



#endif // !1