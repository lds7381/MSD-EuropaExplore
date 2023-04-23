/*
 * Arducam.h
 *
 *  Created on: Mar 6, 2023
 *      Author: liams
 */

#ifndef INC_ARDUCAM_H_
#define INC_ARDUCAM_H_

//#include "ov2640_reg.h"
#include "stm32l4xx_hal.h"
#include "i2c.h"
#include "uart.h"
#include <stdio.h>
#include "arducam_reg.h"
#include "spi.h"

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
	UART_HandleTypeDef *uart;
	uint8_t resolution;
	uint8_t id;
} arducam_info_t;

void arducam_task(arducam_info_t *cam_info);
void init_arducam(arducam_info_t *cam_info);
uint8_t arducam_read_reg(arducam_info_t *cam_info, uint8_t addr);
void arducam_write_reg(arducam_info_t *cam_info, uint8_t addr, uint8_t val);
void arducam_set_bit(arducam_info_t *cam_info, uint8_t addr, uint8_t bit);
void arducam_clear_bit(arducam_info_t *cam_info, uint8_t addr, uint8_t bit);
uint8_t arducam_get_bit(arducam_info_t *cam_info, uint8_t addr, uint8_t bit);
void arducam_start_capture(arducam_info_t *cam_info);
uint8_t arducam_read_fifo(arducam_info_t *cam_info);
void arducam_clear_fifo(arducam_info_t *cam_info);
uint32_t arducam_read_fifo_len(arducam_info_t *cam_info);

#endif /* INC_ARDUCAM_H_ */
