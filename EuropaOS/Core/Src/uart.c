/*
 * uart.c
 *
 *  Created on: Feb 19, 2023
 *      Author: Liam Sullivan
 */

#include "uart.h"

void print(UART_HandleTypeDef *uart, char *msg, int size) {
	HAL_UART_Transmit(uart, (uint8_t *)msg, size, transmit_timeout);
}

