
#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

typedef struct SBITMAP {
	const uint8_t  w;
	const uint8_t  h;
	const uint8_t* map;
} sBITMAP;

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

#endif
