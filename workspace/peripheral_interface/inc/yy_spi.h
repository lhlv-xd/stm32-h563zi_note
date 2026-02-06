/*
 * yy_spi.h
 *
 *  Created on: Feb 6, 2026
 *      Author: Yuyi_Chen
 */

#ifndef PERIPHERAL_INTERFACE_INC_YY_SPI_H_
#define PERIPHERAL_INTERFACE_INC_YY_SPI_H_

#include "yy_hal_peripheral_interface.h"

/* Extern Variable */
extern SPI_HandleTypeDef hspi1;
#define spiHdl hspi1

/* Definition */


/* Prototype */
void yy_spi_init();
void yy_spi_deinit();
yy_peripheral_StatusTypeDef yy_spi_TransmitReceive(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, const uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout);
void yy_example_spi_transmitreceive();

/* Example Prototype */



#endif /* PERIPHERAL_INTERFACE_INC_YY_SPI_H_ */
