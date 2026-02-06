/*
 * yy_adc.c
 *
 *  Created on: Feb 4, 2026
 *      Author: Yuyi_Chen
 */

#include "yy_adc.h"

/**
 * @brief [Customization] ADC Init
 */
void yy_adc_init()
{
	MX_GPDMA1_Init();
	MX_ADC1_Init();
}

/**
 * @brief [Customization] ADC Deinit
 */
void yy_adc_deinit()
{
	HAL_ADC_MspDeInit(&adcHdl);
}


/**
 * @brief Only single ADC transform
 */
void yy_adc_single_transform()
{
	uint32_t value = 0;
	float vol = 0;
	while (1) {
		value= 0;
		HAL_ADC_Start(&adcHdl);
		HAL_ADC_PollForConversion(&adcHdl, ADC_TIMEOUT);
		value = HAL_ADC_GetValue(&adcHdl);
		printf("value: %d\r\n", value);
		HAL_Delay(1000);
		HAL_ADC_Stop(&adcHdl);
	}

}


static uint32_t adcbuf[3] = {0};
/**
 * @brief Use adc by dma.
 */
void yy_adc_with_dma()
{
	/* Start adc + dma  */
	HAL_ADCEx_Calibration_Start(&adcHdl, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&adcHdl, (uint32_t *)adcbuf, 3);

	printf("\r\nstart\r\n");
	while(1) {
		printf("%d, %d, %d\r\n", adcbuf[0], adcbuf[1], adcbuf[2]);
		HAL_Delay(1000);
	}

}
