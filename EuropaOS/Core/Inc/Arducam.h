/*
 * Arducam.h
 *
 *  Created on: Mar 6, 2023
 *      Author: liams
 */

#ifndef INC_ARDUCAM_H_
#define INC_ARDUCAM_H_

#include "ov2640_reg.h"
#include "stm32l4xx_hal.h"
#include "i2c.h"
#include "uart.h"
#include <stdio.h>

#define SIZE_8 (8)

#define DEF_TIMEOUT (100)

#define I2C_SLAVE_WRITE_ADDR (0x60)
#define I2C_SLAVE_READ_ADDR  (0x61)

#define OV2640_160x120          0    //160x120
#define OV2640_176x144          1    //176x144
#define OV2640_320x240          2    //320x240
#define OV2640_352x288          3    //352x288
#define OV2640_640x480          4    //640x480
#define OV2640_800x600          5    //800x600
#define OV2640_1024x768         6    //1024x768
#define OV2640_1280x1024    	7    //1280x1024
#define OV2640_1600x1200    	8    //1600x1200

typedef struct arducam_info_t {
	SPI_HandleTypeDef *spi;
	I2C_HandleTypeDef *i2c;
	uint8_t resolution;
} arducam_info_t;

#endif /* INC_ARDUCAM_H_ */
