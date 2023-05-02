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
	double vernier_values[4];

	char str[50] = "Vernier Sensor Collection Started\r\n";
	double volts;
	uint32_t ph;
	double salinity, dis_o, temp;

	tx_sensor_data sensor_data;

	va_data_t ph_data = {
			.data = 0.0,
			.sensor_type = 0b010
	};
	va_data_t do_data = {
			.data = 0.0,
			.sensor_type = dissolved_oxygen
	};
	va_data_t sal_data = {
			.data = 0.0,
			.sensor_type = 0b100
	};
	va_data_t temp_data = {
			.data = 0.0,
			.sensor_type = thermistor
	};

	// Display Sensor Collection Started
	print(uart, str, sizeof(str));

	// ADC STML4 BUG, NEED TO SET DIFFERNTIAL MODE TO FALSE (MIGHT NEED TO DO THIS FOR EVER CHANNEL)
	adc_handle->Instance->DIFSEL = 0;

	// Main Task while loop
	while(1) {
		// If pH Enabled, Switch to channel and get ADC reading
		if (PH_EN){
			// Select pH Channel
			adc_select_pH(adc_handle);
			HAL_ADCEx_Calibration_Start(adc_handle, ADC_SINGLE_ENDED);
			adc_handle->Instance->DIFSEL = 0;

			// Poll for a conversion
			HAL_ADC_Start(adc_handle);
			HAL_ADC_PollForConversion(adc_handle, 1000);
			buff[0] = HAL_ADC_GetValue(adc_handle);
			HAL_ADC_Stop(adc_handle);

			// Convert
			volts = conv_adc_volt(buff[0]);
			ph = conv_volt_ph(volts);
			ph_data.data = (double)ph;
			vernier_values[0] = ph;

		}
		// If Salinity Enabled, Switch to channel and get ADC reading
		if (SALINITY_EN) {
			adc_select_salinity(adc_handle);
			HAL_ADCEx_Calibration_Start(adc_handle, ADC_SINGLE_ENDED);
			adc_handle->Instance->DIFSEL = 0;

			// Poll for a conversion
			HAL_ADC_Start(adc_handle);
			HAL_ADC_PollForConversion(adc_handle, 1000);
			buff[0] = HAL_ADC_GetValue(adc_handle);
			HAL_ADC_Stop(adc_handle);

			// Convert
			volts = conv_adc_volt(buff[0]);
			salinity = conv_volt_salinity(volts);
			sal_data.data = salinity;
			vernier_values[1] = salinity;
		}
		// If Dissolved Oxygen Enabled, Switch to channel and get ADC reading
		if (DO_EN) {
			adc_select_dissolved_oxygen(adc_handle);
			HAL_ADCEx_Calibration_Start(adc_handle, ADC_SINGLE_ENDED);
			adc_handle->Instance->DIFSEL = 0;

			// Poll for a conversion
			HAL_ADC_Start(adc_handle);
			HAL_ADC_PollForConversion(adc_handle, 1000);
			buff[0] = HAL_ADC_GetValue(adc_handle);
			HAL_ADC_Stop(adc_handle);

			// Convert
			volts = conv_adc_volt(buff[0]);
			if (DO_MGL_MODE) {
				dis_o = conv_volt_do_mgl(volts);
			}
			else {
				dis_o = conv_volt_do_percent(volts);
			}
			do_data.data = dis_o;
			vernier_values[2] = dis_o;

		}

		if (TEMP_EN) {
			adc_select_thermistor(adc_handle);
			HAL_ADCEx_Calibration_Start(adc_handle, ADC_SINGLE_ENDED);
			adc_handle->Instance->DIFSEL = 0;

			// Poll for a conversion
			HAL_ADC_Start(adc_handle);
			HAL_ADC_PollForConversion(adc_handle, 1000);
			buff[0] = HAL_ADC_GetValue(adc_handle);
			HAL_ADC_Stop(adc_handle);

			// Convert
			temp = conv_adc_temp(buff[0]);
			temp_data.data = temp;
			vernier_values[3] = temp;
		}

		// Add Vernier Values to FreeRTOS Buffer to be transmitted (TODO)
		sensor_data.ph_data = ph_data;
		sensor_data.do_data = do_data;
		sensor_data.sal_data = sal_data;
		sensor_data.temp_data = temp_data;

		// Print out information to console if verbose
		if (VS_VERBOSE) {
			print_values(vernier_values, uart);
		}
		else {
			transmit_va_data(uart, (uint8_t *)&sensor_data, sizeof(sensor_data));
		}

		// One second delay (TODO: replace this with timer in future)
		HAL_Delay(1000);

	}
	return;

}

void adc_select_pH(ADC_HandleTypeDef* adc_handle){
	// Create the ADC channel configuration
	ADC_ChannelConfTypeDef sConfig = {0};

	// Populate the configuration to select channel 3 (pH Sensor)
	sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;

    // Configure the adc to select channel 3
    if (HAL_ADC_ConfigChannel(adc_handle, &sConfig) != HAL_OK){
    	Error_Handler();
    }

    // Mux Selection
    mux_select(sel_ph);
}

void adc_select_salinity(ADC_HandleTypeDef* adc_handle){
	// Create the ADC channel configuration
	ADC_ChannelConfTypeDef sConfig = {0};

	// Populate the configuration to select channel 3 (pH Sensor)
	sConfig.Channel = ADC_CHANNEL_6;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;

    // Configure the adc to select channel 3
    if (HAL_ADC_ConfigChannel(adc_handle, &sConfig) != HAL_OK)
    {
	  Error_Handler();
    }

    // Mux Selection
    mux_select(sel_salinity);
}

void adc_select_dissolved_oxygen(ADC_HandleTypeDef* adc_handle){
	// Create the ADC channel configuration
	ADC_ChannelConfTypeDef sConfig = {0};

	// Populate the configuration to select channel 3 (pH Sensor)
	sConfig.Channel = ADC_CHANNEL_12;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;

    // Configure the adc to select channel 3
    if (HAL_ADC_ConfigChannel(adc_handle, &sConfig) != HAL_OK)
    {
	  Error_Handler();
    }

    // Mux Selection
    mux_select(sel_do);
}

void adc_select_thermistor(ADC_HandleTypeDef* adc_handle) {
	// Create the ADC channel configuration
		ADC_ChannelConfTypeDef sConfig = {0};

	// Populate the configuration to select channel 3 (pH Sensor)
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;

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

double conv_adc_volt(uint32_t adc_reading){
	return (double)(((((double)adc_reading - (double)MIN_ADC_READ) * (MAX_VOLT - MIN_VOLT)) / ((double)MAX_ADC_READ - (double)MIN_ADC_READ)) + MIN_VOLT);
}

uint32_t conv_volt_ph(double volts) {
	double ph = (14-(volts/0.25));
	return (uint32_t) round(ph);
}

double conv_volt_salinity(double volts) {
	return volts * SALINITY_VOLT_SLOPE;
}

double conv_volt_do_mgl(double volts) {
	return (volts * DO_MGL_VOLT_SLOPE) - DO_MGL_VOLT_INTERCEPT;
}

uint32_t conv_volt_do_percent(double volts) {
	return (volts * DO_PERCENT_VOLT_SLOPE) - DO_PERCENT_VOLT_INERCEPT;
}

double conv_adc_temp(uint32_t reading) {
	// Convert value to resistance
	uint32_t resistance = 4096 / reading - 1;
	resistance = TEMP_RESISTANCE / resistance;

	return conv_res_temp(resistance);
}

double conv_res_temp(uint32_t res) {
	return (double)(((((double)res - (double)TEMP_MIN_RESISTANCE) * (TEMP_MAX - TEMP_MIN)) / ((double)TEMP_RESISTANCE - (double)TEMP_MIN_RESISTANCE)) + TEMP_MIN);
}

void print_values(double *vernier_values, UART_HandleTypeDef* uart) {
	char msg[26];
	int size;
	if (PH_EN) {
		size = sprintf(msg, "%d,", (int)vernier_values[0]);
		print(uart, msg, size);
	}
	if (DO_EN) {
		if (DO_MGL_MODE) {
	    	size = sprintf(msg, "%0.2f,", vernier_values[2]);
		    print(uart, msg, size);
		}
		else if (DO_PERCENT_MODE) {
			size = sprintf(msg, "%0.2f,", vernier_values[2]);
			print(uart, msg, size);
		}
	}
	if (SALINITY_EN) {
			size = sprintf(msg, "%0.2f,", vernier_values[1]);
			print(uart, msg, size);
		}
	if (TEMP_EN) {
		size = sprintf(msg, "%0.2f", vernier_values[3]);
		print(uart, msg, size);
	}
	print(uart, "\r\n", 3);
}


