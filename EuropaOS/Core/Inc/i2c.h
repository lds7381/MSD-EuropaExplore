/*
 * i2c.h
 *
 *  Created on: Mar 27, 2023
 *      Author: liams
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "ov2640_reg.h"
#include "stm32l4xx_hal.h"
#include "uart.h"
#include <stdio.h>

#define TIMEOUT (100)

typedef struct sensor_reg {
	uint16_t reg;
	uint16_t val;
} sensor_reg;

int i2c_wr_8(I2C_HandleTypeDef *i2c, uint8_t reg, uint8_t val);
int i2c_wr_8_reg(I2C_HandleTypeDef *i2c, sensor_reg reg_val);
int i2c_wr_8_regs(I2C_HandleTypeDef *i2c, const sensor_reg sensor_regs[]);

int i2c_wr_16(I2C_HandleTypeDef *i2c, uint16_t reg, uint8_t val);
int i2c_wr_16_reg(I2C_HandleTypeDef *i2c, sensor_reg reg_val);
int i2c_wr_16_regs(I2C_HandleTypeDef *i2c, const sensor_reg sensor_regs[]);


#endif /* INC_I2C_H_ */