/*
 * yy_i2c_master.h
 *
 *  Created on: Dec 9, 2025
 *      Author: Yuyi
 */

#ifndef PERIPHERAL_INTERFACE_INC_YY_I2C_MASTER_H_
#define PERIPHERAL_INTERFACE_INC_YY_I2C_MASTER_H_

#include "yy_hal_peripheral_interface.h"

/* Extern Variable */
extern I2C_HandleTypeDef hi2c2;
#define i2cmasterHdl hi2c2

/* Definition */
#define I2C_TIMEOUT 100

/* Prototype */
void yy_i2c_master_init();
void yy_i2c_master_deinit();
yy_peripheral_StatusTypeDef yy_i2c_master_poll_read(uint8_t slave_address, uint8_t reg_address, uint8_t* recv_data, uint16_t recv_size);
yy_peripheral_StatusTypeDef yy_i2c_master_poll_write(uint8_t slave_address, uint8_t reg_address, uint8_t* data, uint16_t size);

/* Example Prototype */
void yy_example_i2c_master_poll_read();
void yy_example_i2c_master_poll_write();
void yy_example_i2c_master_access_slave();

#endif /* PERIPHERAL_INTERFACE_INC_YY_I2C_MASTER_H_ */
