/*
 * yy_hal_interface.h
 *
 *  Created on: Nov 24, 2025
 *      Author: Yuyi_Chen
 */

#ifndef YY_HAL_PERIPHERAL_INTERFACE_H_
#define YY_HAL_PERIPHERAL_INTERFACE_H_

#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "adc.h"
#include "i2c.h"

#include "yy_debug.h"

/* Enumeration */
typedef enum
{
  YY_PERIPHERAL_OK       = 0x00,
  YY_PERIPHERAL_ERROR    = 0x01,
  YY_PERIPHERAL_BUSY     = 0x02,
  YY_PERIPHERAL_TIMEOUT  = 0x03
} yy_peripheral_StatusTypeDef;

#endif /* YY_HAL_PERIPHERAL_INTERFACE_H_ */
