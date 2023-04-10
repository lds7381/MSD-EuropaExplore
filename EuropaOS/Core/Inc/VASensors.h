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
#include "uart.h"
#include <math.h>

#define ADC_TIMEOUT (1000)

#define MAX_VOLT (3.3)
#define MIN_VOLT (0.0)

#define MAX_ADC_READ (4095)
#define MIN_ADC_READ (0)

#define PH_EN (1)
#define SALINITY_EN (1)
#define DO_EN (1)
#define TEMP_EN (1)

#define DO_MGL_MODE (1)
#define DO_PERCENT_MODE (0)

#define VS_VERBOSE (1)

#define SALINITY_VOLT_SLOPE (16.3)
#define DO_MGL_VOLT_SLOPE (4.444)
#define DO_MGL_VOLT_INTERCEPT (0.444)
#define DO_PERCENT_VOLT_SLOPE (66.666)
#define DO_PERCENT_VOLT_INERCEPT (0.666)

#define TEMP_RESISTANCE (335600.0)
#define TEMP_MIN_RESISTANCE (85.5)
#define TEMP_MAX (302.0)
#define TEMP_MIN (-40.0)


/*	id_t Enum
 * 	Description: This Enum will hold all of the IDs for the different types of Vernier analog sensors
 */
enum v_net_id{
	thermistor 			= 0b000,
	dissolved_oxygen 	= 0b001,
	ph 					= 0b010,
	salinity 			= 0b100
};

// Vernier Multiplexer selects
enum mux_vsel_t {
	sel_do 					= 0b00,
	sel_ph 					= 0b01,
	sel_salinity			= 0b10
};


/* va_data_t Struct
 * Description: This struct will hold all of the necessary information read from the Vernier analog sensors
 */
typedef struct {
	id_t sensor_type;
	time_t timestamp;
	int	data;
} va_data_t;

void start_va_sensors(ADC_HandleTypeDef* adc_handle, UART_HandleTypeDef* uart, uint32_t *buff);
void adc_select_pH(ADC_HandleTypeDef* adc_handle);
void adc_select_salinity(ADC_HandleTypeDef* adc_handle);
void adc_select_dissolved_oxygen(ADC_HandleTypeDef* adc_handle);
void adc_select_thermistor(ADC_HandleTypeDef* adc_handle);
double conv_adc_volt(uint32_t adc_reading);
uint32_t conv_volt_ph(double volts);
double conv_volt_salinity(double volts);
double conv_volt_do_mgl(double volts);
uint32_t conv_volt_do_percent(double volts);
double conv_adc_temp(uint32_t reading);
double conv_res_temp(uint32_t res);
void mux_select(enum mux_vsel_t sel);
void print_values(double *vernier_values, UART_HandleTypeDef* uart);

#endif /* INC_VASENSORS_H_ */
