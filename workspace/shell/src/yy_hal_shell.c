/*
 * yy_hal_shell.c
 *
 *  Created on: Dec 7, 2025
 *      Author: Yuyi
 */

#include "yy_hal_shell.h"

/* Static Variable */
// for interrupt
static uint8_t rxbuf[1];


/*************************************************************************
 *  Init / Deinit
 *************************************************************************/
/**
 * @brief [Customization] Shell Init
 * @note  Customized implementation function need to depend on the platform
 */
void yy_hal_shell_init()
{
	/* You need to call uart init function */
	MX_USART3_UART_Init();

	/* First receive data for interrupt */
	HAL_UART_Receive_IT(&shellHdl, rxbuf, sizeof(rxbuf));
}

/**
 * @brief [Customization] Shell Deinit
 * @note  Customized implementation function need to depend on the platform
 */
void yy_hal_shell_deinit()
{
	/* You need to call uart deinit function */
	HAL_UART_MspDeInit(&shellHdl);
}


/*************************************************************************
 *  Uart Handler
 *************************************************************************/
UART_INTERRUPT_HANDLER
{
	/* USART3 */
	if (huart->Instance == USART3) {
		// transmit the message received
		yysh_shell_interface(*rxbuf);
		// set the next receiver
		HAL_UART_Receive_IT(&shellHdl, rxbuf, sizeof(rxbuf));
	}
}


/*************************************************************************
 *  Uart Show
 *************************************************************************/
/**
 * @brief [Customization] Shell show
 * @note  Customized implementation function need to depend on the platform
 */
void yy_hal_shell_show(const char* format, ...)
{
	char buf[1024];

	va_list args;
	va_start(args, format);
	vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);

	//HAL_UART_Transmit_IT(&shellHdl, buf, strlen(buf));
	/* Poll is show good. */
	HAL_UART_Transmit(&shellHdl, buf, strlen(buf), 100);
}

/*************************************************************************
 *  Flash Write
 *************************************************************************/
/**
 * @brief [Customization] Shell Flash Write
 * @note  Customized implementation function need to depend on the platform
 */
uint8_t yy_hal_shell_flash_write(uint32_t addr, uint8_t* data, uint32_t data_size)
{
	uint8_t status;
	status = yy_flash_write(addr, data, data_size);
	return status;
}

uint32_t yy_hal_shell_flash_read(uint32_t addr, uint8_t* data, uint32_t data_size)
{
	uint8_t status;
	status = yy_flash_read(addr, data, data_size);
	return status;
}

uint32_t yy_hal_shell_flash_erase(uint32_t addr, uint32_t data_size)
{
	uint8_t status;
	status = yy_flash_erase(addr, data_size);
	return status;
}
