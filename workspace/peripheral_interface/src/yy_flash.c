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
/**
 * @brief Flash write
 * @param[in] address aligned to 16 bytes
 * @param[in] data    data
 * @param[in] size    size of data, it need to require a multiple of 16.
 */
yy_peripheral_StatusTypeDef yy_flash_write(uint32_t addr, uint8_t* data, uint32_t size)
{
	/* Check if address within the flash range */
	if (__check_flash_address_range(addr, size) != YY_PERIPHERAL_OK) {
		return YY_PERIPHERAL_ERROR;
	}

	/* Aligned to 16 bytes */
	if ((addr & 0xf) != 0) {
		yy_err("The Address need to be aligned to 16 bytes.");
		return YY_PERIPHERAL_ERROR;
	}

	if (size % 16 != 0) {
		yy_err("The size needs to be divisible by 16.");
		return YY_PERIPHERAL_ERROR;
	}

	yy_peripheral_StatusTypeDef status;


	/* Erase sector */
	if (addr % FLASH_SECTOR_SIZE == 0) {
		if (__check_flash_value_eq_0xff(addr, size) != YY_PERIPHERAL_OK) {
			status = __flash_erase(addr, size);
			if (status != YY_PERIPHERAL_OK) {
				yy_err("flash erase is failed,(status: %d)", status);
				return YY_PERIPHERAL_ERROR;
			}
		}
	}

	/* make sure operation address was erased */
	if (__check_flash_value_eq_0xff(addr, size) != YY_PERIPHERAL_OK) {
		yy_err("Some address is not 0xff\r\n");
		return YY_PERIPHERAL_ERROR;
	}

	/* Flash Write */
	uint32_t times = (size + 16 - 1) / 16;

	for (int i = 0; i < times; i++) {
		/* Write data */
		status = __flash_write_16bytes(addr + (16*i), data + (16*i));
		if (status != YY_PERIPHERAL_OK) {
			yy_err("flash write is failed,(status: %d)", status);
			return YY_PERIPHERAL_ERROR;
		}
	}

	return YY_PERIPHERAL_OK;
}

/**
 * @brief Read specific flash address.
 */
yy_peripheral_StatusTypeDef yy_flash_read(uint32_t addr, uint8_t* data, uint32_t size)
{
	if (__check_flash_address_range(addr, size) != YY_PERIPHERAL_OK) {
		return YY_PERIPHERAL_ERROR;
	}

	uint8_t* ptr = (uint8_t *) addr;
	for (int i = 0; i < size; i++) {
		data[i] = ptr[i];
	}

	return YY_PERIPHERAL_OK;
}

/**
 * @brief Erase the "sectors" containing the specified range of flash address
 */
yy_peripheral_StatusTypeDef yy_flash_erase(uint32_t addr, uint32_t size)
{
	return __flash_erase(addr, size);
}


/**************************************************************************
 * Example Code
 **************************************************************************/
/**
 * @brief Example flash operation
 */
yy_peripheral_StatusTypeDef yy_example_flash_write_read()
{
	yy_info("Example Flash Write: 0x8100000 - 0x810400f\r\n");
	uint32_t addr = 0x8100000;
	uint32_t size = 16*1024+16;
	uint8_t data[size];
	for (int i = 0; i < size; i++) {
		data[i] = i & 0xff;
	}

	yy_peripheral_StatusTypeDef status;
	status = yy_flash_write(addr, data, size);
	yy_info("\tstatus: %d\r\n", status);

	/* Read data */
	for (int i = 0; i < size; i++) {
		data[i] = 0;
	}
	yy_flash_read(addr, data, size);
	for (int i = 0; i < size; i++) {
		if (data[i] != (i & 0xff)) {
			yy_err("data value is error!(addr: 0x%x, data: 0x%x, value: 0x%x)", addr+i, data[i], (i & 0xff));
		}
	}
	yy_info("Read is succeed!");

	return YY_PERIPHERAL_OK;
}


/**************************************************************************
 * Static Function
 **************************************************************************/
/**
 * @brief Combine four uint8_t values ​​into one uint32_t.
 */
static uint32_t __combine_u8_to_u32(uint8_t* u8_data)
{
	uint32_t ret = 0;
	for (int i = 0; i < 4; i++) {
		ret += (u8_data[i] << (i*8));
	}
	return ret;
}

/**
 * @brief Check if addr and (addr+size) is within flash range
 */
static yy_peripheral_StatusTypeDef __check_flash_address_range(uint32_t addr, uint32_t size)
{
	/* addr is out of the flash range */
	if (addr < FLASH_BANK1_BASE || addr > (FLASH_BANK2_BASE + FLASH_BANK2_SIZE - 1)) {
		yy_err("Address is out of the flash range.\r\n");
		return YY_PERIPHERAL_ERROR;
	}

	/* address across to another bank */
	if (addr < FLASH_BANK2_BASE && (addr + size - 1) >= FLASH_BANK2_BASE) {
		yy_err("Address across to another bank.\r\n");
		return YY_PERIPHERAL_ERROR;
	}

	/* End of address is out of the flash range */
	if ((addr + size) >= (FLASH_BANK2_BASE + FLASH_BANK2_SIZE)) {
		yy_err("End of address is out of the flash range.\r\n");
		return YY_PERIPHERAL_ERROR;
	}

	return YY_PERIPHERAL_OK;
}

/**
 * @brief Check if the specific address range has been filled with 0xff.
 */
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
 */
static yy_peripheral_StatusTypeDef __flash_erase(uint32_t addr, uint32_t size)
{
	/* Check if address within the flash range */
	if (__check_flash_address_range(addr, size) != YY_PERIPHERAL_OK) {
		return YY_PERIPHERAL_ERROR;
	}

	/* address is not start of sector */
	if (addr % FLASH_SECTOR_SIZE != 0) {
		yy_err("The address needs to be the starting address of the sector.");
		return YY_PERIPHERAL_ERROR;
	}

	HAL_StatusTypeDef status;
	uint32_t sector_error;
	FLASH_EraseInitTypeDef flash_erase_definitions;
	uint32_t nbsectors = (size + FLASH_SECTOR_SIZE -1) / (FLASH_SECTOR_SIZE);


	/* Setting Parameters */
	flash_erase_definitions.TypeErase   = FLASH_TYPEERASE_SECTORS;
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
	yy_debug("nbsector: %d, sector nums: %d", nbsectors, flash_erase_definitions.Sector);

	HAL_FLASH_Unlock();

	/* Erase Flash */
	status = HAL_FLASHEx_Erase(&flash_erase_definitions, &sector_error);
	if (status != HAL_OK) {
		/* Erase Failed */
		yy_err("status: %d, sector_error: %d", status, sector_error);
		HAL_FLASH_Lock();
		return YY_PERIPHERAL_ERROR;
	}

	while (FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE) != HAL_OK);
	HAL_FLASH_Lock();
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
	if ((addr & 0xf) != 0) {
		yy_err("Address need to aligned to 16 bytes.");
		return YY_PERIPHERAL_ERROR;
	}

	HAL_StatusTypeDef status;
	uint32_t flash_data[4];
	for (int i = 0; i < 4; i++) {
		flash_data[i] = __combine_u8_to_u32(data + 4*i);
	}

	HAL_FLASH_Unlock();

	while (FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE) != HAL_OK);
	status = HAL_FLASH_Program(
								0x02U,                // FLASH_TYPEPROGRAM_WORD,
								addr,                 // Flash address
								(uint32_t)flash_data  // In fact, it's  address that pointer to data
							  );
	if (status != HAL_OK)
	{
		yy_err("addr: 0x%x, status: %d", addr, status);
		HAL_FLASH_Lock();
		return YY_PERIPHERAL_ERROR;
	}

	HAL_FLASH_Lock();
	return YY_PERIPHERAL_OK;
}
