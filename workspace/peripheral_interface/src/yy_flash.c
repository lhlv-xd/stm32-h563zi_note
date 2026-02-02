/*
 * yy_flash.c
 *
 *  Created on: Jan 4, 2026
 *      Author: Yuyi
 */


#include "yy_flash.h"

/* Static Function Prototype */
static uint32_t __combine_u8_to_u32(uint8_t* u8_data);
static yy_peripheral_StatusTypeDef __check_flash_address_range(uint32_t addr, uint32_t size);
static yy_peripheral_StatusTypeDef __check_flash_value_eq_0xff(uint32_t addr, uint32_t size);
static yy_peripheral_StatusTypeDef __flash_erase(uint32_t addr, uint32_t size);
static yy_peripheral_StatusTypeDef __flash_write_16bytes(uint32_t addr, uint8_t* data);



/**************************************************************************
 * Function
 **************************************************************************/

yy_peripheral_StatusTypeDef yy_flash_fw_write(uint32_t addr, uint8_t* data, uint32_t size)
{
	/* Check if address within the flash range */
	if (__check_flash_address_range(addr, size) != YY_PERIPHERAL_OK) {
		return YY_PERIPHERAL_ERROR;
	}

	/* Aligned to 16 bytes */
	if ((addr & 0xf) != 0) {
		printf("The Address need to be aligned to 16 bytes.");
		return YY_PERIPHERAL_ERROR;
	}

	if (size % 16 != 0) {
		printf("The size needs to be divisible by 16.");
		return YY_PERIPHERAL_ERROR;
	}

	/* Erase sector */
	if (addr % FLASH_SECTOR_SIZE == 0) {
		__flash_erase(addr, size);
	}

	/* make sure operation address was erased */
	if (__check_flash_value_eq_0xff(addr, size) != YY_PERIPHERAL_OK) {
		printf("you need to erase to erase flash\r\n");
		return YY_PERIPHERAL_ERROR;
	}



	uint32_t flash_data[4];
	uint32_t times = (size + 16 - 1) / 16;
	yy_peripheral_StatusTypeDef status;
	for (int i = 0; i < times; i++) {
		for (int j = 0; j < 4; j++) {
			flash_data[j] = __combine_u8_to_u32(data+ 16*i + 4*j);
		}
		/* Write data */
		status = __flash_write_16bytes(addr + (16*i), flash_data);
	}



	return YY_PERIPHERAL_OK;
}


yy_peripheral_StatusTypeDef yy_flash_write()//uint32_t addr, uint8_t* data, uint32_t data_size)
{

}

/**************************************************************************
 * Example Code
 **************************************************************************/
yy_peripheral_StatusTypeDef yy_example_flash_fw_write()
{
	printf("\r\nExample Flash Write: 0x8100000 - 0x810400f\r\n");
	uint32_t size = 16*1024+16;
	uint8_t data[16*1024+16];
	for (int i = 0; i < size; i++) {
		data[i] = i & 0xff;
	}

	yy_peripheral_StatusTypeDef status;
	status = yy_flash_fw_write(0x8100000, data, size);
	printf("\tstatus: %d\r\n", status);
}



/**************************************************************************
 * Static Function
 **************************************************************************/
static uint32_t __combine_u8_to_u32(uint8_t* u8_data)
{
	uint32_t ret = 0;
	for (int i = 0; i < 4; i++) {
		ret += (u8_data[i] << (i*8));
	}
	return ret;
}

static yy_peripheral_StatusTypeDef __check_flash_address_range(uint32_t addr, uint32_t size)
{
	/* out of the flash range */
	if (addr < FLASH_BANK1_BASE || addr >= (FLASH_BANK2_BASE + FLASH_BANK2_SIZE)) {
		printf("Address is out of the flash range.\r\n");
		return YY_PERIPHERAL_ERROR;
	}

	/* address across to another bank */
	if (addr < FLASH_BANK2_BASE && (addr +size) > FLASH_BANK2_BASE) {
		printf("Address across to another bank.\r\n");
		return YY_PERIPHERAL_ERROR;
	}

	/* End of address is out of the flash range */
	if ((addr+size) >= (FLASH_BANK2_BASE + FLASH_BANK2_SIZE)) {
		printf("End of address is out of the flash range.\r\n");
		return YY_PERIPHERAL_ERROR;
	}

	return YY_PERIPHERAL_OK;
}

static yy_peripheral_StatusTypeDef __check_flash_value_eq_0xff(uint32_t addr, uint32_t size)
{
	uint8_t* ptr = (uint8_t *) addr;
	for (int i = 0; i < size; i++) {
		if (ptr[i] != 0xff) {
			return YY_PERIPHERAL_ERROR;
		}
	}
	return YY_PERIPHERAL_OK;
}



/**
 * @brief erase flash area sector by sector
 * @ note addr must be start of sector
 * @ note You need to add HAL_FLASH_Lock();
 */
static yy_peripheral_StatusTypeDef __flash_erase(uint32_t addr, uint32_t size)
{
	/* Check if address within the flash range */
	if (__check_flash_address_range(addr, size) != YY_PERIPHERAL_OK) {
		return YY_PERIPHERAL_ERROR;
	}

	/* address is not start of sector */
	if (addr % FLASH_SECTOR_SIZE != 0) {
		return YY_PERIPHERAL_ERROR;
	}

	HAL_StatusTypeDef status;
	uint32_t sector_error;
	FLASH_EraseInitTypeDef flash_erase_definitions;
	uint32_t nbsectors = (size + FLASH_TYPEERASE_SECTORS -1) / (FLASH_TYPEERASE_SECTORS);


	/* Setting Parameters */
	flash_erase_definitions.TypeErase   = FLASH_TYPEERASE_SECTORS;  // 8K
	flash_erase_definitions.NbSectors   = nbsectors;

	/* Bank1 */
	if (addr > FLASH_BANK1_BASE && addr < FLASH_BANK2_BASE) {
		flash_erase_definitions.Banks       = FLASH_BANK_1;
		flash_erase_definitions.Sector      = (uint32_t) ((addr - FLASH_BANK1_BASE) / FLASH_SECTOR_SIZE);
	}
	/* Bank2 */
	else {
		flash_erase_definitions.Banks       = FLASH_BANK_2;
		flash_erase_definitions.Sector      = (uint32_t) ((addr - FLASH_BANK2_BASE) / FLASH_SECTOR_SIZE);
	}

	/* Unlock Flash */
	HAL_FLASH_Unlock();

	/* Erase Flash */
	status = HAL_FLASHEx_Erase(&flash_erase_definitions, &sector_error);
	if (status != HAL_OK) {
		/* Erase Failed */
		HAL_FLASH_Lock();
		return YY_PERIPHERAL_ERROR;
	}
	return YY_PERIPHERAL_OK;
}



/**
 * @brief write 16bytes to flash
 * @note  Data must be aligned to 16 bytes.
 * @note  You need to add HAL_FLASH_Lock();
 */
static yy_peripheral_StatusTypeDef __flash_write_16bytes(uint32_t addr, uint8_t* data)
{
	/* Check if address within the flash range */
	if (__check_flash_address_range(addr, 16) != YY_PERIPHERAL_OK) {
		return YY_PERIPHERAL_ERROR;
	}

	/* Aligned to 16 bytes */
	if (addr & 0xf != 0) {
		return YY_PERIPHERAL_ERROR;
	}

	HAL_FLASH_Unlock();

	HAL_StatusTypeDef status;
	status = HAL_FLASH_Program(
								0x02U,          // FLASH_TYPEPROGRAM_WORD,
								addr,           // Flash address
								(uint32_t)data  // In fact, it's  address that pointer to data
							  );
	if (status != HAL_OK)
	{
		HAL_FLASH_Lock();
		return YY_PERIPHERAL_ERROR;
	}

	return YY_PERIPHERAL_OK;
}
