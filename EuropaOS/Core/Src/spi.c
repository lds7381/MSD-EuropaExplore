/*
 * spi.c
 *
 *  Created on: Apr 5, 2023
 *      Author: liams
 */
#include "spi.h"

// SPI Write function, based off of cameras (different SPIs), will return 0 if ok
int spi_cam_write(SPI_HandleTypeDef *spi, uint8_t camera_id, uint8_t address, uint8_t val) {
	// Camera 0
	switch (camera_id) {
	case 0:
		// Set CS Low
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
		// Transfer address
		if (HAL_SPI_Transmit(spi, &address, sizeof(address), TIMEOUT) != HAL_OK) {
			return 1;
		}
		// Transfer data
		if (HAL_SPI_Transmit(spi, &val, sizeof(val), TIMEOUT) != HAL_OK ) {
			return 1;
		}
		// Set CS High
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
		return HAL_OK;
		break;
	// Camera 1
	case 1:
		/*
		// Set CS Low
		HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET);
		// Transfer address
		if (HAL_SPI_Transmit(spi, &address, sizeof(address), TIMEOUT) != HAL_OK) {
			return 1;
		}
		// Transfer data
		if (HAL_SPI_Transmit(spi, &val, sizeof(val), TIMEOUT) != HAL_OK ) {
			return 1;
		}
		// Set CS High
		HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
		return HAL_OK;
		*/
		break;
	default:
		return 1;
		break;
	}
	return 1;
}

uint8_t spi_cam_read(SPI_HandleTypeDef *spi, uint8_t camera_id, uint8_t address) {
	uint8_t val;

	switch (camera_id) {
		case 0:
			// Set CS Low
			HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
			// Transmit Receive Data
			if (HAL_SPI_TransmitReceive(spi, &address, &val, sizeof(address), TIMEOUT) != HAL_OK) {
				return 0;
			}
			// Set CS High
			HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
			return val;
			break;
		case 1:
			/*
			// Set CS Low
			HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
			// Transmit Receive Data
			if (HAL_SPI_TransmitReceive(spi, &address, &val, sizeof(address), TIMEOUT) != HAL_OK) {
				return 0;
			}
			// Set CS High
			HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
			return val;
			*/
			break;
		default:
			return 0;
			break;
	}
	return 0;
}

