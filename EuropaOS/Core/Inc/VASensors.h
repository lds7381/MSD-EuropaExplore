/*
 * VASensors.h
 *
 *  Created on: Feb 6, 2023
 *      Author: Liam Sullivan
 *
 *  Description:
 *
 */

#ifndef INC_VASENSORS_H_
#define INC_VASENSORS_H_

#include "stm32l4xx.h"
#include "main.h"

#define ADC_TIMEOUT (1000)


/*	id_t Enum
 * 	Description: This Enum will hold all of the IDs for the different types of Vernier analog sensors
 */
typedef enum id_t{
	thermistor 			= 0b000,
	dissolved_oxygen 	= 0b001,
	ph 					= 0b010,
	salinity 			= 0b100
};


/* va_data_t Struct
 * Description: This struct will hold all of the necessary information read from the Vernier analog sensors
 */
typedef struct {
	id_t sensor_type;
	time_t timestamp;
	int	data;
} va_data_t;

void start_va_sensors(ADC_HandleTypeDef* adc_handle);
void adc_select_pH(ADC_HandleTypeDef* adc_handle);
void adc_select_salinity(ADC_HandleTypeDef* adc_handle);
void adc_select_dissolved_oxygen(ADC_HandleTypeDef* adc_handle);

#endif /* INC_VASENSORS_H_ */
