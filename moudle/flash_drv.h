#ifndef __FLASH_DRV_H
#define __FLASH_DRV_H

#include "platform.h"


#define W25Q80 	0XEF13
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128 0XEF17

#define RANDOM_BYTE 0XFF


// W25Q128指令集
#define W25Q128_WRITE_ENABLE     0x06
#define W25Q128_WRITE_DISABLE    0x04
#define W25Q128_READ_DATA        0x03
#define W25Q128_PAGE_PROGRAM     0x02
#define W25Q128_SECTOR_ERASE     0x20
#define W25Q128_BLOCK32_ERASE    0x52
#define W25Q128_BLOCK64_ERASE    0xD8
#define W25Q128_CHIP_ERASE       0xC7
#define W25Q128_READ_ID          0x90
#define W25Q128_READ_STATUS_REG1 0x05

// Flash参数
#define W25Q128_PAGE_SIZE        256
#define W25Q128_SECTOR_SIZE      4096
#define W25Q128_BLOCK32_SIZE     32768
#define W25Q128_BLOCK64_SIZE     65536
#define W25Q128_CHIP_SIZE        0x1000000 // 16MB








#define FLASH_CS_PIN 		17




#define SPI_FLASH_CS_HIGH() gpio_put(FLASH_CS_PIN, true);
#define SPI_FLASH_CS_LOW()  gpio_put(FLASH_CS_PIN, false);


extern uint16_t flash_id;

extern uint8_t sector_data[4096];


void flash_gpio_init(void);
uint16_t W25Q128_ReadID(void);
void W25Q128_WriteEnable(void);
void W25Q128_WaitForWriteEnd(void);
uint8_t W25Q128_ReadStatusReg(void);
void W25Q128_EraseSector(uint32_t addr);
void W25Q128_EraseBlock32(uint32_t addr);
void W25Q128_EraseBlock64(uint32_t addr);
void W25Q128_ChipErase(void);
void W25Q128_WriteData(const uint8_t* data, uint32_t addr, uint32_t len);
void W25Q128_ReadData(uint8_t* data, uint32_t addr, uint32_t len);
uint8_t W25Q128_CheckErased(uint32_t addr, uint32_t len);
void W25Q128_EnsureErased(uint32_t addr, uint32_t len);


#endif




