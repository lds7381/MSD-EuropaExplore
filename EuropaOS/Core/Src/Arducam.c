/*
 * Arducam.c
 *
 *  Created on: Mar 6, 2023
 *      Author: liams
 */

#include "Arducam.h"

// I2C Initalization Function for Arducam
void init_arducam(arducam_info_t *cam_info){
	// Get ODVA2640 Chip ready for register change for JPEG Initialization
	i2c_wr_8(cam_info->i2c, 0xFF, 0x01);
	i2c_wr_8(cam_info->i2c, 0x12, 0x80);
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
}

void capture_arducam(arducam_info_t *cam_info){

}
