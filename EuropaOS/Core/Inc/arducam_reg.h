/*
 * arducam_reg.h
 *
 *  Created on: Apr 5, 2023
 *      Author: liams
 */

#ifndef INC_ARDUCAM_REG_H_
#define INC_ARDUCAM_REG_H_

#define RWBIT                       0x80  //READ AND WRITE BIT IS BIT[7]

#define ARDUCHIP_TEST1          	0x00  //TEST register

#define ARDUCHIP_FIFO             	0x04  //FIFO and I2C control
#define FIFO_CLEAR_MASK           	0x01
#define FIFO_START_MASK            	0x02
#define FIFO_RDPTR_RST_MASK     	0x10
#define FIFO_WRPTR_RST_MASK     	0x20

#define BURST_FIFO_READ            	0x3C  //Burst FIFO read operation
#define SINGLE_FIFO_READ        	0x3D  //Single FIFO read operation

#define ARDUCHIP_REV               	0x40  //ArduCHIP revision
#define VER_LOW_MASK               	0x3F
#define VER_HIGH_MASK              	0xC0

#define FIFO_SIZE1                	0x42  //Camera write FIFO size[7:0] for burst to read
#define FIFO_SIZE2                	0x43  //Camera write FIFO size[15:8]
#define FIFO_SIZE3                	0x44  //Camera write FIFO size[18:16]

#endif /* INC_ARDUCAM_REG_H_ */
