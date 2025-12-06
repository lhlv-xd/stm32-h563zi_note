/*
 * yy_uart.c
 *
 *  Created on: Nov 24, 2025
 *      Author: Yuyi_Chen
 */


#include "yy_uart.h"

/* Function */
/**
 * @brief printf
 */
__WEAK PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&defaultUartHdl, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

/**
 * @brief [Customization] Initialize Uart.
 */
void yy_uart_init(void)
{
	/* Customization API */
	MX_USART3_UART_Init();
}

/**
 * @brief [Customization] Deinitialize Uart.
 */
void yy_uart_deinit(void)
{
	/* Customization API */
	HAL_UART_MspDeInit(&defaultUartHdl);
}

/**************************************************************************
 * UART POLL
 **************************************************************************/
/**
 * @brief Sending messages through a specific UART handler.
 */
yy_peripheral_StatusTypeDef yy_uart_tx(void* uartHandler, uint8_t* message, uint32_t size)
{
	uint32_t Timeout = 100;

	return HAL_UART_Transmit(uartHandler, message, (uint16_t)size, Timeout);
}


/**
 * @brief Example Code: Send hello message by defaultUartHdl(Tx: PD8)
 */
void yy_example_uart_show(void)
{
	uint8_t msg[] = "hello\r\n";
	yy_uart_tx(&defaultUartHdl, msg, sizeof(msg));
}

/**
 * @brief Example Code: Receive and send a byte by defaultUartHdl(Tx: PD8, Rx: PD9)
 */
void yy_example_uart_poll(void)
{
	uint32_t Timeout = 100;
	uint8_t ch;

	while(1) {
		if (HAL_UART_Receive(&defaultUartHdl, &ch, 1, Timeout) == 0) {
			HAL_UART_Transmit(&defaultUartHdl, &ch, 1, Timeout);
			continue;
		}
		/* Timeout */

	}
}
