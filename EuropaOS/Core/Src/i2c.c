/*
 * i2c.c
 *
 *  Created on: Mar 27, 2023
 *      Author: liams
 */
#include "i2c.h"

// 8 INDICATING THAT THE ADDRESS IS 8 BIT

int i2c_wr_8(I2C_HandleTypeDef *i2c, uint8_t reg, uint8_t val) {
	uint8_t err;
	err = HAL_I2C_Master_Transmit(i2c, reg, &(val), 1, TIMEOUT);
	if ( err != HAL_OK ) {
		return err;
	} else {
		return HAL_OK;
	}
}

int i2c_wr_8_reg(I2C_HandleTypeDef *i2c, sensor_reg sensor_reg) {
	uint8_t val = sensor_reg.val;
	if ( HAL_I2C_Master_Transmit(i2c, sensor_reg.reg, &(val), 1, TIMEOUT) != HAL_OK ) {
		return 1;
	} else {
		return HAL_OK;
	}
}

int i2c_wr_8_regs(I2C_HandleTypeDef *i2c, const sensor_reg sensor_regs[]) {
	int err = 0;
	uint16_t reg_addr = 0;
	uint16_t reg_val = 0;
	const struct sensor_reg *next = sensor_regs;
	while((reg_addr != 0xff) | (reg_val != 0xff)) {
		reg_addr = next->reg;
		reg_val = next->val;
		err = i2c_wr_8(i2c, (uint8_t)reg_addr, (uint8_t)reg_val);
		if (err != HAL_OK) {
			return 1;
		}
	}
	return HAL_OK;
}

// 16 INDICATING THAT THE ADDRESS IS 16 BIT

int i2c_wr_16(I2C_HandleTypeDef *i2c, uint16_t reg, uint8_t val) {
	if ( HAL_I2C_Master_Transmit(i2c, reg, &(val), 2, TIMEOUT) != HAL_OK ) {
		return 1;
	} else {
		return HAL_OK;
	}
}

int i2c_wr_16_reg(I2C_HandleTypeDef *i2c,  sensor_reg sensor_reg) {
	uint8_t val = sensor_reg.val;
	if ( HAL_I2C_Master_Transmit(i2c, sensor_reg.reg, &(val), 2, TIMEOUT) != HAL_OK ) {
		return 1;
	} else {
		return HAL_OK;
	}
}

int i2c_wr_16_regs(I2C_HandleTypeDef *i2c, const sensor_reg sensor_regs[]) {
	int err = 0;
	uint16_t reg_addr = 0;
	uint16_t reg_val = 0;
	const struct sensor_reg *next = sensor_regs;
	while((reg_addr != 0xffff) | (reg_val != 0xff)) {
		reg_addr = next->reg;
		reg_val = next->val;
		err = i2c_wr_16(i2c, reg_addr, reg_val);
		if (err != HAL_OK) {
			return 1;
		}
	}
	return HAL_OK;
}
