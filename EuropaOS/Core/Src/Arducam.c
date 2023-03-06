/*
 * Arducam.c
 *
 *  Created on: Mar 6, 2023
 *      Author: liams
 */

#include "Arducam.h"

void init_arducam(arducam_info_t *cam_info){
	// Get ODVA2640 Chip ready for register change for JPEG Initialization
	HAL_I2C_Master_Transmit(cam_info->i2c, 0xFF, 0x01, SIZE_8, DEF_TIMEOUT);
	HAL_I2C_Master_Transmit(cam_info->i2c, 0x12, 0x80, SIZE_8, DEF_TIMEOUT);
	HAL_Delay(100);

	// Initialize for JPEG

}

void capture_arducam(arducam_info_t *cam_info){

}
