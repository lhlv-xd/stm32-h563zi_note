/*
 * yy_i2c_slave.h
 *
 *  Created on: Dec 11, 2025
 *      Author: Yuyi
 */

#ifndef PERIPHERAL_INTERFACE_INC_YY_I2C_SLAVE_H_
#define PERIPHERAL_INTERFACE_INC_YY_I2C_SLAVE_H_

#include "yy_hal_peripheral_interface.h"


/* Definition */
#define I2C_TIMEOUT 100
#define PAGE_SIZE   128


/* Extern Variable */
extern I2C_HandleTypeDef hi2c1;;
#define i2cslaveHdl hi2c1


/* Prototype */
void yy_i2c_slave_init();


#endif /* PERIPHERAL_INTERFACE_INC_YY_I2C_SLAVE_H_ */
