/*
 * motor.h
 *
 *  Created on: Apr 12, 2023
 *      Author: liams
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stm32l4xx_hal.h"
#include "cmsis_os.h"

typedef struct motor_info_t {
	GPIO_TypeDef *gpio_port;
	uint16_t en_pin;
	uint16_t fwd_pin;
	uint16_t rev_pin;
} motor_info_t;


#define MOTOR_FREE_STOP 	(0)
#define MOTOR_ACTIVE_STOP 	(1)
#define MOTOR_FORWARD 		(2)
#define MOTOR_REVERSE		(3)

#define SERVO_MIN_DUTY_CYCLE 		(400)
#define SERVO_MAX_DUTY_CYCLE 		(2400)
#define SERVO_DEFAULT_DUTY_CYCLE 	(1400)
#define SERVO_POSITION0_DUTY_CYCLE 	(400)
#define SERVO_POSITION1_DUTY_CYCLE 	(900)
#define SERVO_POSITION2_DUTY_CYCLE 	(1400)
#define SERVO_POSITION3_DUTY_CYCLE 	(1900)
#define SERVO_POSITION4_DUTY_CYCLE 	(2400)

void motor_instruction(motor_info_t *motor_info, int instruction);
void motor_free_stop(motor_info_t *motor_info);
void motor_active_stop(motor_info_t *motor_info);
void motor_forward(motor_info_t *motor_info);
void motor_reverse(motor_info_t *motor_info);


#endif /* INC_MOTOR_H_ */
