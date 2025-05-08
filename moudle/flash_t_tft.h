#ifndef _FLASH_T_TFT_H
#define _FLASH_T_TFT_H

#include "platform.h"



extern FLASH_sBITMAP flash_charge_array[30];
extern FLASH_sBITMAP flash_letter_array[26];
extern FLASH_sBITMAP flash_timeout_array[30];
extern FLASH_sBITMAP flash_lock_array[30];
extern FLASH_sBITMAP flash_lowpower_array[15];
extern FLASH_sBITMAP flash_nopod_array[25];
extern FLASH_sBITMAP flash_power_on_array[35];
extern FLASH_sBITMAP flash_smoke_array[54];

extern FLASH_sBITMAP_TABLE default_component[25];
uint8_t set_display_component(uint8_t index, uint8_t x, uint8_t y, FLASH_sBITMAP* bitmap);

//没有边界检测
uint8_t display_num(uint8_t index, uint8_t x, uint8_t y, FLASH_sBITMAP* bitmap, uint16_t num);

#endif // !1