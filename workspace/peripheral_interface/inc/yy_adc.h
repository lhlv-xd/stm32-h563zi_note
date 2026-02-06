/*
 * yy_adc.h
 *
 *  Created on: Feb 4, 2026
 *      Author: Yuyi_Chen
 */

#ifndef PERIPHERAL_INTERFACE_INC_YY_ADC_H_
#define PERIPHERAL_INTERFACE_INC_YY_ADC_H_

#include "yy_hal_peripheral_interface.h"

/* Extern Variable */
extern ADC_HandleTypeDef hadc1;
#define adcHdl hadc1

/* Definition */
#define ADC_TIMEOUT 10

/* Prototype */
void yy_adc_init();
void yy_adc_deinit();
void yy_adc_single_transform();
void yy_adc_with_dma();

/* Example Prototype */



#endif /* PERIPHERAL_INTERFACE_INC_YY_ADC_H_ */
