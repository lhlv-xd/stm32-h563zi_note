/*
 * yy_i2c_master.c
 *
 *  Created on: Dec 9, 2025
 *      Author: Yuyi
 */

#include "yy_i2c_master.h"


/**
 * @brief [Customization] I2C Master Poll Init
 */
void yy_i2c_master_init()
{
	MX_I2C2_Init();
}

/**
 * @brief [Customization] I2C Master Poll Deinit
 */
void yy_i2c_master_deinit()
{
	HAL_I2C_MspDeInit(&i2cmasterHdl);
}

/**************************************************************************
 * I2C Master POLL
 **************************************************************************/
/**
 * @brief I2C read by poll method
 */
yy_peripheral_StatusTypeDef yy_i2c_master_poll_read(uint8_t slave_address, uint8_t reg_address, uint8_t* recv_data, uint16_t recv_size)
{
	//HAL_I2C_Master_Receive(&i2cmasterHdl, reg_address, recv_data, recv_size, I2C_TIMEOUT);
	return HAL_I2C_Mem_Read(&i2cmasterHdl, slave_address << 1, reg_address, I2C_MEMADD_SIZE_8BIT, recv_data, recv_size, I2C_TIMEOUT);
}

/**
 * @brief I2C write by poll method
 */
yy_peripheral_StatusTypeDef yy_i2c_master_poll_write(uint8_t slave_address, uint8_t reg_address, uint8_t* data, uint16_t size)
{
	//HAL_I2C_Master_Transmit(&i2cmasterHdl, reg_address, data, size, I2C_TIMEOUT);
	return HAL_I2C_Mem_Write(&i2cmasterHdl, slave_address << 1, reg_address, I2C_MEMADD_SIZE_8BIT, data, size, I2C_TIMEOUT);
}


/**
 * @brief Example Code: Read 128 bytes from slave address 0x50.
 */
void yy_example_i2c_master_poll_read()
{
	uint8_t data[128] = {0};
	yy_i2c_master_poll_read(0x50, 0x0, data, sizeof(data));

	for (int i = 0; i < 128; i++) {
		printf("%02x, ", data[i]);
		if ((i+1) % 16 == 0) {
			printf("\r\n");
		}
	}
}

/**
 * @brief Example Code: Write 128 bytes from slave address 0x50.
 */
void yy_example_i2c_master_poll_write()
{
	uint8_t data[128];
	for (int i = 0; i < 128; i++) {
		data[i] = i & 0xff;
	}

	yy_i2c_master_poll_write(0x50, 0x0, data, 128);
}
