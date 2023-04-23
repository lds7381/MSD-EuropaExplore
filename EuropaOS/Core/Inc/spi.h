/*
 * spi.h
 *
 *  Created on: Apr 5, 2023
 *      Author: liams
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "stm32l4xx_hal.h"
#include "uart.h"
#include <stdio.h>
#include "arducam_reg.h"

int spi_cam_write(SPI_HandleTypeDef *spi, uint8_t camera_id, uint8_t address, uint8_t val);
uint8_t spi_cam_read(SPI_HandleTypeDef *spi, uint8_t camera_id, uint8_t address);

#endif /* INC_SPI_H_ */
