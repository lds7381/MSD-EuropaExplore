/*
 * uart.c
 *
 *  Created on: Feb 19, 2023
 *      Author: Liam Sullivan
 */

#include "uart.h"

void transmit_va_data (UART_HandleTypeDef *uart, uint8_t *sensor_data, int size) {
	HAL_UART_Transmit(uart, (uint8_t *)sensor_data, sizeof(size), transmit_timeout);
}

void print(UART_HandleTypeDef *uart, char *msg, int size) {
	HAL_UART_Transmit(uart, (uint8_t *)msg, size, transmit_timeout);
}

