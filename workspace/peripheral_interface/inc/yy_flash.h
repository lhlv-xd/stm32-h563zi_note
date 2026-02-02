/*
 * yy_flash.h
 *
 *  Created on: Jan 4, 2026
 *      Author: Yuyi
 */

#ifndef PERIPHERAL_INTERFACE_INC_YY_FLASH_H_
#define PERIPHERAL_INTERFACE_INC_YY_FLASH_H_

#include "yy_hal_peripheral_interface.h"

/* Definition */
#define FLASH_SECTOR_SIZE 0x2000
#define FLASH_BANK1_BASE 0x8000000
#define FLASH_BANK1_SIZE 0x100000

#define FLASH_BANK2_BASE 0x8100000
#define FLASH_BANK2_SIZE 0x100000


/* Prototype */
yy_peripheral_StatusTypeDef yy_flash_fw_write(uint32_t addr, uint8_t* data, uint32_t size);
yy_peripheral_StatusTypeDef yy_flash_write();//uint32_t addr, uint8_t* data, uint32_t data_size);

/* Example */
yy_peripheral_StatusTypeDef yy_example_flash_fw_write();

#endif /* PERIPHERAL_INTERFACE_INC_YY_FLASH_H_ */
