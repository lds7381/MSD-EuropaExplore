/*
 * uart.h
 *
 *  Created on: Feb 19, 2023
 *      Author: liams
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32l4xx.h"
#include "VASensors.h"
#include "main.h"
#include <stdio.h>

#define transmit_timeout (100)

void transmit_va_data (UART_HandleTypeDef *uart, uint8_t *sensor_data, int size);
void print(UART_HandleTypeDef *uart, char *msg, int size);

#endif /* INC_UART_H_ */
