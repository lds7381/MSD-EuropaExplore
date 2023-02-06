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

void start_va_sensors(ADC_HandleTypeDef* adc_handle){
	uint32_t vernier_values[3];

	// Select pH Channel
	adc_select_pH(adc_handle);
	while(1) {
		// Start the collection of Values from the ADC
		if ( HAL_ADC_Start(adc_handle) != HAL_OK) {
			Error_Handler();
		}
		// Poll for a conversion
		if ( HAL_ADC_PollForConversion(adc_handle, ADC_TIMEOUT)!= HAL_OK) {
			Error_Handler();
		}
		// Set the value
		vernier_values[0] = HAL_ADC_GetValue(adc_handle);

		// Would then stop the adc, change channels, start again, poll,get value and repeat for all sensors
	}
}

void adc_select_pH(ADC_HandleTypeDef* adc_handle){
	// Create the ADC channel configuration
	ADC_ChannelConfTypeDef sConfig = {0};

	// Populate the configuration to select channel 3 (pH Sensor)
	sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;

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
    sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;

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
    sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;

    // Configure the adc to select channel 3
    if (HAL_ADC_ConfigChannel(adc_handle, &sConfig) != HAL_OK)
    {
	  Error_Handler();
    }
}


