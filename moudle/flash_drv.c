#include "flash_drv.h"

uint8_t sector_data[4096];
uint16_t flash_id = W25Q64; //默认就是25Q64


// // uint8_t SPI_ReadWriteByte(const uint8_t data) {
// // 	SPI_FLASH_CS_LOW();
// // 	spi_write_blocking(SPI_PORT, &data, 1);
// // 	SPI_FLASH_CS_HIGH();
// // }

// uint8_t SPI_ReadWriteByte(const uint8_t data) {
// 	static uint8_t received = 0;
// 	// SPI_FLASH_CS_LOW();
// 	spi_write_read_blocking(SPI_PORT, &data, &received, 1);
// 	// SPI_FLASH_CS_HIGH();
// 	return received;
// }

void flash_gpio_init(void) {


	gpio_init(FLASH_CS_PIN);
	gpio_set_dir(FLASH_CS_PIN, true);


	SPI_FLASH_CS_HIGH();
}


// uint16_t flash_reas_id(void) {
// 	uint16_t Temp = 0x00;
// 	SPI_FLASH_CS_LOW();
// 	SPI_ReadWriteByte(W25X_MANUFACT_DEVICE_ID);//发送读取ID命令
// 	SPI_ReadWriteByte(0x00);
// 	SPI_ReadWriteByte(0x00);
// 	SPI_ReadWriteByte(0x00);
// 	Temp |= SPI_ReadWriteByte(0xff) << 8;
// 	Temp |= SPI_ReadWriteByte(0xff);
// 	SPI_FLASH_CS_HIGH();
// 	return Temp;
// }

// uint8_t flash_read_sr(void) {
// 	uint8_t byte = 0;
// 	SPI_FLASH_CS_LOW();                          //使能器件
// 	SPI_ReadWriteByte(W25X_READ_STATU_TEG);    //发送读取状态寄存器命令
// 	byte = SPI_ReadWriteByte(RANDOM_BYTE);           //读取一个字节
// 	SPI_FLASH_CS_HIGH();                          //取消片选
// 	return byte;
// }

// //等待空闲
// void flash_wait_busy(void) {
// 	while((flash_read_sr() & 0x01) == 0x01); // 等待BUSY位清空
// }

// //SPI_FLASH写使能
// //将WEL置位
// void W25Q128_WriteEnable(void) {
// 	SPI_FLASH_CS_LOW();                          //使能器件
// 	SPI_ReadWriteByte(W25X_WRITE_ENABLE);      //发送写使能
// 	SPI_FLASH_CS_HIGH();                          //取消片选
// }

// void flash_erase(void) {
// 	W25Q128_WriteEnable();                  //SET WEL
// 	flash_wait_busy();
// 	SPI_FLASH_CS_LOW();                          //使能器件
// 	SPI_ReadWriteByte(W25X_CHIP_ERASE);        //发送片擦除命令
// 	SPI_FLASH_CS_HIGH();                          //取消片选
// 	flash_wait_busy();   				   //等待芯片擦除结束
// }



// //读取SPI FLASH
// //在指定地址开始读取指定长度的数据
// //pBuffer:数据存储区
// //ReadAddr:开始读取的地址(24bit)
// //NumByteToRead:要读取的字节数(最大65535)
// void SpiFlashRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead) {
// 	uint16_t i;
// 	SPI_FLASH_CS_LOW();                          //使能器件
// 	SPI_ReadWriteByte(W25X_READ_DATA);         //发送读取命令
// 	SPI_ReadWriteByte((uint8_t)((ReadAddr) >> 16)); //发送24bit地址
// 	SPI_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
// 	SPI_ReadWriteByte((uint8_t)ReadAddr);
// 	for(i = 0; i < NumByteToRead; i++) {
// 		pBuffer[i] = SPI_ReadWriteByte(0XFF); //循环读数
// 	}
// 	SPI_FLASH_CS_HIGH();                          //取消片选
// }


// //SPI在一页(0~65535)内写入少于256个字节的数据
// //在指定地址开始写入最大256字节的数据
// //pBuffer:数据存储区
// //WriteAddr:开始写入的地址(24bit)
// //NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
// void SpiFlashWritePage(const uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) {
// 	uint16_t i;
// 	W25Q128_WriteEnable();                  //SET WEL
// 	SPI_FLASH_CS_LOW();                          //使能器件
// 	SPI_ReadWriteByte(W25X_PAGE_PROGRAM);      //发送写页命令
// 	SPI_ReadWriteByte((uint8_t)((WriteAddr) >> 16)); //发送24bit地址
// 	SPI_ReadWriteByte((uint8_t)((WriteAddr) >> 8));
// 	SPI_ReadWriteByte((uint8_t)WriteAddr);
// 	for(i = 0; i < NumByteToWrite; i++) {
// 		SPI_ReadWriteByte(pBuffer[i]); //循环写数
// 	}
// 	SPI_FLASH_CS_HIGH();                          //取消片选
// 	flash_wait_busy();					   //等待写入结束
// }
// //无检验写SPI FLASH
// //必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
// //具有自动换页功能
// //在指定地址开始写入指定长度的数据,但是要确保地址不越界!
// //pBuffer:数据存储区
// //WriteAddr:开始写入的地址(24bit)
// //NumByteToWrite:要写入的字节数(最大65535)
// //CHECK OK
// void SpiFlashWriteNoCheck(const uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) {
// 	uint16_t pageremain;
// 	pageremain = 256 - WriteAddr % 256; //单页剩余的字节数
// 	if(NumByteToWrite <= pageremain) {
// 		pageremain = NumByteToWrite;
// 	}
// 	while(1) {
// 		SpiFlashWritePage(pBuffer, WriteAddr, pageremain);
// 		if(NumByteToWrite == pageremain) {
// 			break; //写入结束了
// 		} else { //NumByteToWrite>pageremain
// 			pBuffer += pageremain;
// 			WriteAddr += pageremain;
// 			NumByteToWrite -= pageremain;			 //减去已经写入了的字节数
// 			if(NumByteToWrite > 256) {
// 				pageremain = 256; //一次可以写入256个字节
// 			} else {
// 				pageremain = NumByteToWrite; 	 //不够256个字节了
// 			}
// 		}
// 	};
// }

// //擦除一个扇区
// //Dst_Addr:扇区地址 0~511 for w25x16
// //擦除一个山区的最少时间:150ms
// void SpiFlashEraseSector(uint32_t Dst_Addr) {
// 	Dst_Addr *= 4096;
// 	W25Q128_WriteEnable();                  //SET WEL
// 	flash_wait_busy();
// 	SPI_FLASH_CS_LOW();                          //使能器件
// 	SPI_ReadWriteByte(W25X_SECTOR_ERASE);      //发送扇区擦除指令
// 	SPI_ReadWriteByte((uint8_t)((Dst_Addr) >> 16)); //发送24bit地址
// 	SPI_ReadWriteByte((uint8_t)((Dst_Addr) >> 8));
// 	SPI_ReadWriteByte((uint8_t)Dst_Addr);
// 	SPI_FLASH_CS_HIGH();                          //取消片选
// 	flash_wait_busy();   				   //等待擦除完成
// }

// //写SPI FLASH
// //在指定地址开始写入指定长度的数据
// //该函数带擦除操作!
// //pBuffer:数据存储区
// //WriteAddr:开始写入的地址(24bit)
// //NumByteToWrite:要写入的字节数(最大65535)

// void SpiFlashWrite(const uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) {
// 	uint32_t secpos;
// 	uint16_t secoff;
// 	uint16_t secremain;
// 	uint16_t i;

// 	secpos = WriteAddr / 4096; //定位到这个扇区
// 	secoff = WriteAddr % 4096; //在扇区内的偏移
// 	secremain = 4096 - secoff; //扇区剩余空间大小

// 	if(NumByteToWrite <= secremain) {
// 		secremain = NumByteToWrite; //不大于4096个字节
// 	}
// 	while(1) {
// 		SpiFlashRead(sector_data, secpos * 4096, 4096); //读出整个扇区的内容
// 		for(i = 0; i < secremain; i++) { //校验数据
// 			if(sector_data[secoff + i] != 0XFF) {
// 				break; //需要擦除
// 			}
// 		}
// 		if(i < secremain) { //需要擦除
// 			SpiFlashEraseSector(secpos);//擦除这个扇区
// 			for(i = 0; i < secremain; i++) { //复制
// 				sector_data[i + secoff] = pBuffer[i];
// 			}
// 			SpiFlashWriteNoCheck(pBuffer, WriteAddr, secremain); //写入整个扇区
// 		} else {
// 			SpiFlashWriteNoCheck(pBuffer, WriteAddr, secremain); //写已经擦除了的,直接写入扇区剩余区间.
// 		}
// 		if(NumByteToWrite == secremain) {
// 			break; //写入结束了
// 		} else { //写入未结束
// 			secpos++;//扇区地址增1
// 			secoff = 0; //偏移位置为0
// 			pBuffer += secremain; //指针偏移
// 			WriteAddr += secremain; //写地址偏移
// 			NumByteToWrite -= secremain;				//字节数递减
// 			if(NumByteToWrite > 4096) {
// 				secremain = 4096;	//下一个扇区还是写不完
// 			} else {
// 				secremain = NumByteToWrite;			//下一个扇区可以写完了
// 			}
// 		}
// 	};
// }


// // void SpiFlashWrite(const uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) {
// // 	//找到地址所在的扇区
// // 	uint32_t sec_pos;     //扇区地址
// // 	uint16_t sec_remaim;  //扇区剩余空间大小
// // 	uint16_t sec_off;      //扇区内的偏移
// // 	uint16_t i;
// // 	sec_pos = WriteAddr / 4096;
// // 	sec_off = WriteAddr % 4096;
// // 	sec_remaim = 4096 - sec_off;
// // 	if(NumByteToWrite <= sec_remaim) {
// // 		sec_remaim = NumByteToWrite; //不大于4096个字节
// // 	}
// // 	while(1) {
// // 	    SpiFlashRead(sector_data, sec_pos * 4096, 4096);
// // 		for(i = 0; i < sec_remaim; i++) { //校验数据
// // 			if(sector_data[sec_off + i] != 0XFF) {
// // 				break; //需要擦除
// // 			}
// // 		}
// // 		if(i < sec_remaim) { //扇区剩余空间需要擦除
// // 			SpiFlashEraseSector(sec_pos);//擦除这个扇区
// // 			for(i = 0; i < sec_remaim; i++) { //复制
// // 				sector_data[i + sec_off] = pBuffer[i];
// // 			}
// // 			SpiFlashWriteNoCheck(pBuffer, WriteAddr, sec_remaim); //写入整个扇区
// // 		}else{
// // 			SpiFlashWriteNoCheck(pBuffer, WriteAddr, sec_remaim); //写已经擦除了的,直接写入扇区剩余区间.
// // 		}
// // 		if(NumByteToWrite == sec_remaim) {
// // 			break; //写入结束了
// // 		} else { //写入未结束
// // 			sec_pos++;//扇区地址增1
// // 			sec_off = 0; //偏移位置为0
// // 			pBuffer += sec_remaim; //指针偏移
// // 			WriteAddr += sec_remaim; //写地址偏移
// // 			NumByteToWrite -= sec_remaim;				//字节数递减
// // 			if(NumByteToWrite > 4096) {
// // 				sec_remaim = 4096;	//下一个扇区还是写不完
// // 			} else {
// // 				sec_remaim = NumByteToWrite;			//下一个扇区可以写完了
// // 			}
// // 		}

// // 	}

// // }

// 发送写使能命令
void W25Q128_WriteEnable(void) {
	SPI_FLASH_CS_LOW();
	uint8_t cmd = W25Q128_WRITE_ENABLE;
	spi_write_blocking(SPI_PORT, &cmd, 1);
	SPI_FLASH_CS_HIGH();
}

// 读取状态寄存器
uint8_t W25Q128_ReadStatusReg(void) {
	uint8_t status = 0;
	uint8_t cmd = W25Q128_READ_STATUS_REG1;

	SPI_FLASH_CS_LOW();
	spi_write_blocking(SPI_PORT, &cmd, 1);
	spi_read_blocking(SPI_PORT, 0xFF, &status, 1);
	SPI_FLASH_CS_HIGH();

	return status;
}

// 等待Flash操作完成
void W25Q128_WaitForWriteEnd(void) {
	while ((W25Q128_ReadStatusReg() & 0x01) == 0x01); // 检查BUSY位
}

// 读取Flash ID
uint16_t W25Q128_ReadID(void) {
	uint16_t id = 0;
	uint8_t cmd[4] = {W25Q128_READ_ID, 0x00, 0x00, 0x00};
	uint8_t rx_data[2] = {0};

	SPI_FLASH_CS_LOW();
	spi_write_blocking(SPI_PORT, cmd, 4);
	spi_read_blocking(SPI_PORT, 0xFF, rx_data, 2);
	SPI_FLASH_CS_HIGH();

	id = (rx_data[0] << 8) | rx_data[1];
	return id;
}

// 扇区擦除(4KB)
void W25Q128_EraseSector(uint32_t addr) {
	uint8_t cmd[4];

	// 确保地址是4K对齐的
	addr = addr & ~(W25Q128_SECTOR_SIZE - 1);

	W25Q128_WriteEnable();

	cmd[0] = W25Q128_SECTOR_ERASE;
	cmd[1] = (addr >> 16) & 0xFF;
	cmd[2] = (addr >> 8) & 0xFF;
	cmd[3] = addr & 0xFF;

	SPI_FLASH_CS_LOW();
	spi_write_blocking(SPI_PORT, cmd, 4);
	SPI_FLASH_CS_HIGH();

	W25Q128_WaitForWriteEnd();
}




// 32K块擦除
void W25Q128_EraseBlock32(uint32_t addr) {
	uint8_t cmd[4];

	// 确保地址是32K对齐的
	addr = addr & ~(W25Q128_BLOCK32_SIZE - 1);

	W25Q128_WriteEnable();

	cmd[0] = W25Q128_BLOCK32_ERASE;
	cmd[1] = (addr >> 16) & 0xFF;
	cmd[2] = (addr >> 8) & 0xFF;
	cmd[3] = addr & 0xFF;

	SPI_FLASH_CS_LOW();
	spi_write_blocking(SPI_PORT, cmd, 4);
	SPI_FLASH_CS_HIGH();

	W25Q128_WaitForWriteEnd();
}

// 64K块擦除
void W25Q128_EraseBlock64(uint32_t addr) {
	uint8_t cmd[4];

	// 确保地址是64K对齐的
	addr = addr & ~(W25Q128_BLOCK64_SIZE - 1);

	W25Q128_WriteEnable();

	cmd[0] = W25Q128_BLOCK64_ERASE;
	cmd[1] = (addr >> 16) & 0xFF;
	cmd[2] = (addr >> 8) & 0xFF;
	cmd[3] = addr & 0xFF;

	SPI_FLASH_CS_LOW();
	spi_write_blocking(SPI_PORT, cmd, 4);
	SPI_FLASH_CS_HIGH();

	W25Q128_WaitForWriteEnd();
}

// 整片擦除
void W25Q128_ChipErase(void) {
	W25Q128_WriteEnable();

	SPI_FLASH_CS_LOW();
	uint8_t cmd = W25Q128_CHIP_ERASE;
	spi_write_blocking(SPI_PORT, &cmd, 1);
	SPI_FLASH_CS_HIGH();

	W25Q128_WaitForWriteEnd();
}

// 检查指定区域是否已擦除(全为0xFF)
uint8_t W25Q128_CheckErased(uint32_t addr, uint32_t len) {
	uint8_t buf[256];
	uint32_t remaining = len;

	while(remaining > 0) {
		uint32_t read_len = (remaining > sizeof(buf)) ? sizeof(buf) : remaining;
		W25Q128_ReadData(buf, addr, read_len);

		for(uint32_t i = 0; i < read_len; i++) {
			if(buf[i] != 0xFF) {
				return 0; // 未擦除
			}
		}

		addr += read_len;
		remaining -= read_len;
	}

	return 1; // 已擦除
}




void W25Q128_EnsureErased(uint32_t addr, uint32_t len) {
	if (len == 0) return;
	const uint32_t sector_size = W25Q128_SECTOR_SIZE;
	uint32_t start_sector = addr / sector_size;
	uint32_t end_sector = (addr + len - 1) / sector_size;

	uint32_t start_offset = addr % sector_size;
	uint32_t end_offset = (addr + len - 1) % sector_size;

	// 如果整个区域已经擦除，直接返回
	if (W25Q128_CheckErased(addr, len)) {
		return;
	}

	// 静态缓冲区，避免栈溢出
	static uint8_t sector_data[W25Q128_SECTOR_SIZE];

	for (uint32_t sector = start_sector; sector <= end_sector; sector++) {
		uint32_t sector_addr = sector * sector_size;

		// 计算本扇区需要擦除的范围
		uint32_t erase_start = 0;
		uint32_t erase_len = sector_size;

		if (sector == start_sector) {
			erase_start = start_offset;
			if (start_sector == end_sector) {
				erase_len = end_offset - start_offset + 1;
			} else {
				erase_len = sector_size - start_offset;
			}
		} else if (sector == end_sector) {
			erase_start = 0;
			erase_len = end_offset + 1;
		} else {
			// 中间完整扇区，直接擦除
			erase_start = 0;
			erase_len = sector_size;
		}

		// 检查本扇区需要擦除的区域是否已擦除
		if (W25Q128_CheckErased(sector_addr + erase_start, erase_len)) {
			// 本区域已擦除，跳过
			continue;
		}

		// 如果是整扇区擦除（擦除范围覆盖整个扇区），直接擦除
		if (erase_start == 0 && erase_len == sector_size) {
			W25Q128_EraseSector(sector_addr);
		} else {
			// 读-改-写保护
			W25Q128_ReadData(sector_data, sector_addr, sector_size);
			memset(sector_data + erase_start, 0xFF, erase_len);
			W25Q128_EraseSector(sector_addr);
			W25Q128_WriteData(sector_data, sector_addr, sector_size);
		}
	}
}

// 带擦除检查的连续写入
void W25Q128_WriteData(const uint8_t* data, uint32_t addr, uint32_t len) {
	uint8_t cmd[4];

	// 确保要写入的区域已擦除
	W25Q128_EnsureErased(addr, len);

	// 由于Flash需要按页写入(256字节一页)，所以需要分多次写入
	while(len > 0) {
		uint32_t page_remaining = W25Q128_PAGE_SIZE - (addr % W25Q128_PAGE_SIZE);
		uint32_t write_len = (len < page_remaining) ? len : page_remaining;

		// 发送写使能
		W25Q128_WriteEnable();

		// 发送页编程命令和地址
		cmd[0] = W25Q128_PAGE_PROGRAM;
		cmd[1] = (addr >> 16) & 0xFF;
		cmd[2] = (addr >> 8) & 0xFF;
		cmd[3] = addr & 0xFF;

		SPI_FLASH_CS_LOW();
		spi_write_blocking(SPI_PORT, cmd, 4);
		spi_write_blocking(SPI_PORT, data, write_len);
		SPI_FLASH_CS_HIGH();

		// 等待写入完成
		W25Q128_WaitForWriteEnd();

		addr += write_len;
		data += write_len;
		len -= write_len;
	}
}

// 从Flash连续读取数据
void W25Q128_ReadData(uint8_t* data, uint32_t addr, uint32_t len) {
	uint8_t cmd[4];

	cmd[0] = W25Q128_READ_DATA;
	cmd[1] = (addr >> 16) & 0xFF;
	cmd[2] = (addr >> 8) & 0xFF;
	cmd[3] = addr & 0xFF;

	SPI_FLASH_CS_LOW();
	spi_write_blocking(SPI_PORT, cmd, 4);
	spi_read_blocking(SPI_PORT, 0xFF, data, len);
	SPI_FLASH_CS_HIGH();
}

uint32_t flash_address = 0;
void flash_write_bitmap_array_impl(const char* name, const void* array[], uint8_t count) {
	
	uart_printf("FLASH_sBITMAP flash_%s[%d] = {\n", name, count);
	for (uint8_t i = 0; i < count; i++) {
		const sBITMAP* bmp = array[i];
		if (!bmp) continue;

		const uint32_t data_size = (uint32_t)(bmp->h) * bmp->w * 2;
		const uint32_t current_addr = flash_address;

		W25Q128_WriteData(bmp->map, current_addr, data_size);
		flash_address += data_size;

		uart_printf("%d, %d, 0x%x,\n",
					bmp->w, bmp->h, current_addr);
	}
	uart_printf("};\n");
}

// 对外暴露的宏定义
#define flash_write_bitmap_array(array) \
	flash_write_bitmap_array_impl(#array, array, sizeof(array)/sizeof(array[0]))
