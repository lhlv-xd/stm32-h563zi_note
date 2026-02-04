/*
 * yy_hal_shell.h
 *
 *  Created on: Dec 7, 2025
 *      Author: Yuyi
 */

#ifndef SHELL_INC_YY_HAL_SHELL_H_
#define SHELL_INC_YY_HAL_SHELL_H_

/* [Customization] Depend on Platform Header */
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "yy_flash.h"

/* Standard Header */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/* Shell Header */
#include "yy_shell_interface.h"


/* Extern Variable */
extern UART_HandleTypeDef huart3;
#define shellHdl huart3

#define UART_INTERRUPT_HANDLER void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)

/* Prototype */
void yy_hal_shell_init();
void yy_hal_shell_deinit();
void yy_hal_shell_show(const char* format, ...);

uint8_t yy_hal_shell_flash_write(uint32_t addr, uint8_t* data, uint32_t data_size);
uint32_t yy_hal_shell_flash_read(uint32_t addr, uint8_t* data, uint32_t data_size);
uint32_t yy_hal_shell_flash_erase(uint32_t addr, uint32_t data_size);


#endif /* SHELL_INC_YY_HAL_SHELL_H_ */
