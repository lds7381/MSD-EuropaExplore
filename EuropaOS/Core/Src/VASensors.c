/*
 * VASensors.c
 *
 *  Created on: Feb 6, 2023
 *      Author: Liam Sullivan
 *
 *  Description:
 *
 */

#include "VASensors.h"

ADC_ChannelConfTypeDef sConfig = {0};

void start_va_sensors(ADC_HandleTypeDef* adc_handle, UART_HandleTypeDef* uart, uint32_t *buff){
	uint32_t vernier_values[3];

	char str[50] = "Vernier Sensor Collection Started\r\n";
	char reading[20];
	print(uart, str, sizeof(str));

	// Select pH Channel
	adc_select_pH(adc_handle);
//	// Start the collection of Values from the ADC
//	if ( HAL_ADC_Start(adc_handle) != HAL_OK) {
//		Error_Handler();
//	}

	//HAL_ADC_Start_DMA(adc_handle, buff, 1);

	while(1) {
		// Poll for a conversion
		HAL_ADC_Start(adc_handle);
		HAL_ADC_PollForConversion(adc_handle, 1000);
		buff[0] = HAL_ADC_GetValue(adc_handle);
		HAL_ADC_Stop(adc_handle);

		sprintf(reading, "ADC VALUE: %ld\r\n", buff[0]);
		print(uart, reading, sizeof(reading));
		HAL_Delay(500);
	}

}

void adc_select_pH(ADC_HandleTypeDef* adc_handle){
	// Create the ADC channel configuration
	ADC_ChannelConfTypeDef sConfig = {0};

	// Populate the configuration to select channel 3 (pH Sensor)
	sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;

    // Configure the adc to select channel 3
    if (HAL_ADC_ConfigChannel(adc_handle, &sConfig) != HAL_OK){
    	Error_Handler();
    }
}

void adc_select_salinity(ADC_HandleTypeDef* adc_handle){
	// Create the ADC channel configuration
	ADC_ChannelConfTypeDef sConfig = {0};

	// Populate the configuration to select channel 3 (pH Sensor)
	sConfig.Channel = ADC_CHANNEL_5;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;

    // Configure the adc to select channel 3
    if (HAL_ADC_ConfigChannel(adc_handle, &sConfig) != HAL_OK)
    {
	  Error_Handler();
    }
}

void adc_select_dissolved_oxygen(ADC_HandleTypeDef* adc_handle){
	// Create the ADC channel configuration
	ADC_ChannelConfTypeDef sConfig = {0};

	// Populate the configuration to select channel 3 (pH Sensor)
	sConfig.Channel = ADC_CHANNEL_7;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;

    // Configure the adc to select channel 3
    if (HAL_ADC_ConfigChannel(adc_handle, &sConfig) != HAL_OK)
    {
	  Error_Handler();
    }
}

void mux_select(enum mux_vsel_t sel) {
	if ((sel & 0b01) > 0) {
		HAL_GPIO_WritePin(MUX_SEL0_GPIO_Port, MUX_SEL0_Pin, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(MUX_SEL0_GPIO_Port, MUX_SEL0_Pin, GPIO_PIN_RESET);
	}

	if ((sel & 0b10) > 0) {
		HAL_GPIO_WritePin(MUX_SEL1_GPIO_Port, MUX_SEL1_Pin, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(MUX_SEL1_GPIO_Port, MUX_SEL1_Pin, GPIO_PIN_RESET);
	}
}




