/*
 * yy_uart.h
 *
 *  Created on: Nov 24, 2025
 *      Author: Yuyi_Chen
 */

#ifndef YY_UART_H_
#define YY_UART_H_

#include "yy_hal_peripheral_interface.h"

/* Extern Variable */
extern UART_HandleTypeDef huart3;
#define defaultUartHdl huart3

/* Definition */
// Printf
#ifdef __GNUC__
#ifndef PUTCHAR_PROTOTYPE
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
#endif

/* Prototype */
void yy_uart_init(void);
void yy_uart_deinit(void);
yy_peripheral_StatusTypeDef yy_uart_tx(void* uartHandler, uint8_t* message, uint32_t size);

/* Example Prototype */
void yy_example_uart_show(void);
void yy_example_uart_poll(void);

#endif /* YY_UART_H_ */
