/*
 * Arducam.c
 *
 *  Created on: Mar 6, 2023
 *      Author: Liam Sullivan
 */

#include "Arducam.h"

void arducam_task(arducam_info_t *cam_info) {
	uint32_t fifo_len;
	char str[50];
	int size;

	print(cam_info->uart, "Starting Camera Initialization\r\n", 33);

	// initialize arducam
	init_arducam(cam_info);

	print(cam_info->uart, "Camera Initialized\r\n", 21);

	arducam_start_capture(cam_info);

	HAL_Delay(TIMEOUT * 10);

	fifo_len = arducam_read_fifo_len(cam_info);

	size = sprintf(str, "FIFO Size: %ld\r\n", fifo_len);
	print(cam_info->uart, str, size);

	return;
}

// I2C Initalization Function for Arducam
void init_arducam(arducam_info_t *cam_info){
	// Get ODVA2640 Chip ready for register change for JPEG Initialization
	uint8_t num = i2c_wr_8(cam_info->i2c, 0xFF, 0x01);
	if ( num != HAL_OK) {
		print(cam_info->uart, "Something Wrong\r\n", 16);
	}
	if (i2c_wr_8(cam_info->i2c, 0x12, 0x80) != HAL_OK) {
		print(cam_info->uart, "Something Wrong\r\n", 16);
	}
	HAL_Delay(100);

	// Initialize for JPEG
	i2c_wr_8_regs(cam_info->i2c, OV2640_JPEG_INIT);
	i2c_wr_8_regs(cam_info->i2c, OV2640_YUV422);
	i2c_wr_8_regs(cam_info->i2c, OV2640_JPEG);
	i2c_wr_8(cam_info->i2c, 0xFF, 0x01);
	i2c_wr_8(cam_info->i2c, 0x15, 0x00);
	switch(cam_info->resolution) {
		case OV2640_640x480:
			i2c_wr_8_regs(cam_info->i2c, OV2640_640x480_JPEG);
			break;
		case OV2640_800x600:
			i2c_wr_8_regs(cam_info->i2c, OV2640_800x600_JPEG);
			break;
		case OV2640_1024x768:
			i2c_wr_8_regs(cam_info->i2c, OV2640_1024x768_JPEG);
			break;
		case OV2640_1280x1024:
			i2c_wr_8_regs(cam_info->i2c, OV2640_1280x1024_JPEG);
			break;
		default:
			i2c_wr_8_regs(cam_info->i2c, OV2640_640x480_JPEG);
			break;
	}

	// Set the CS Pin High
	if (cam_info->id == 0) {
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
	}
	else if (cam_info->id == 1) {
		//HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
	}
}

// Arducam Register Functions

uint8_t arducam_read_reg(arducam_info_t *cam_info, uint8_t addr) {
	uint8_t data;
	data = spi_cam_read(cam_info->spi, cam_info->id, addr);
	return data;
}

void arducam_write_reg(arducam_info_t *cam_info, uint8_t addr, uint8_t val) {
	if (spi_cam_write(cam_info->spi, cam_info->id, addr | RWBIT, val) != HAL_OK) {
		//print(hlpuart1, "Warning: Arducam register failed to write!\r\n");
	}
}

void arducam_set_bit(arducam_info_t *cam_info, uint8_t addr, uint8_t bit) {
	uint8_t temp;
	temp = arducam_read_reg(cam_info, addr);
	arducam_write_reg(cam_info, addr, (temp | bit));
}

void arducam_clear_bit(arducam_info_t *cam_info, uint8_t addr, uint8_t bit) {
	uint8_t temp;
	temp = arducam_read_reg(cam_info, addr);
	arducam_write_reg(cam_info, addr, (temp & (~bit)));
}

uint8_t arducam_get_bit(arducam_info_t *cam_info, uint8_t addr, uint8_t bit) {
	uint8_t temp;
	temp = arducam_read_reg(cam_info, addr);
	temp &= bit;
	return temp;
}

// FIFO Functions

void arducam_start_capture(arducam_info_t *cam_info) {
	arducam_write_reg(cam_info, ARDUCHIP_FIFO, FIFO_START_MASK);
}

uint8_t arducam_read_fifo(arducam_info_t *cam_info) {
	uint8_t data;
	data = arducam_read_reg(cam_info, SINGLE_FIFO_READ);
	return data;
}

void arducam_clear_fifo(arducam_info_t *cam_info) {
	arducam_write_reg(cam_info, ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

uint32_t arducam_read_fifo_len(arducam_info_t *cam_info) {
	uint32_t len1,len2,len3,length=0;
	len1 = arducam_read_reg(cam_info, FIFO_SIZE1);
	len2 = arducam_read_reg(cam_info, FIFO_SIZE2);
	len3 = arducam_read_reg(cam_info, FIFO_SIZE3) & 0x7f;
	length = ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff;
	return length;
}

