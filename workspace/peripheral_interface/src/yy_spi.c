/*
 * yy_spi.c
 *
 *  Created on: Feb 6, 2026
 *      Author: Yuyi_Chen
 */

#include "yy_spi.h"

/**
 * @brief [Customization] SPI Init
 */
void yy_spi_init()
{
	MX_SPI1_Init();
}

/**
 * @brief [Customization] SPI Deinit
 */
void yy_spi_deinit()
{
	HAL_SPI_MspDeInit(&spiHdl);
}


/**
 * @brief Transmit and Receive an amount of data in blocking mode.
 * @note tx size is equal to rx size
 */
yy_peripheral_StatusTypeDef yy_spi_TransmitReceive(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,
		const uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
{
	HAL_StatusTypeDef status;
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
	status = HAL_SPI_TransmitReceive(&spiHdl, pTxData, pRxData, Size, Timeout);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);

	return status;
}


/**************************************************************************
 * Example Code
 **************************************************************************/
/**
 * @brief SPI Transmit and Receive Example
 */
void yy_example_spi_transmitreceive()
{
	printf("\r\nExample SPI Transmit and Receive.\r\n");
	uint8_t txbuf[1024];
	uint8_t rxbuf[1024] = {0};

	for(int i = 0; i < 1024; i++) {
		txbuf[i] = i & 0xff;
	}

	yy_peripheral_StatusTypeDef status;
	status = yy_spi_TransmitReceive(GPIOB, GPIO_PIN_6, &txbuf, &rxbuf, 1024, 10);
	yy_info("status: %d", status);

	printf("\r\nRx Received data:\r\n");
	for(int i = 0; i < 1024; i++) {
		printf("0x%02x, ", rxbuf[i]);
		if ((i+1) % 16 == 0) {
			printf("\r\n");
		}
	}
}
