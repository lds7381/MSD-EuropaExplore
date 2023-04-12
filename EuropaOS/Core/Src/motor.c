/*
 * motor.c
 *
 *  Created on: Apr 12, 2023
 *      Author: liams
 */

#include "motor.h"

void motor_instruction(motor_info_t *motor_info, int instruction) {
	switch(instruction) {
	case MOTOR_FREE_STOP:
		motor_free_stop(motor_info);
		break;
	case MOTOR_ACTIVE_STOP:
		motor_active_stop(motor_info);
		break;
	case MOTOR_FORWARD:
		motor_forward(motor_info);
		break;
	case MOTOR_REVERSE:
		motor_reverse(motor_info);
		break;
	default:
		break;
	}
}

void servo_move_pos(int position){
	switch (position) {
	case 0:
		TIM3->CCR1 = SERVO_POSITION0_DUTY_CYCLE;
		break;
	case 1:
		TIM3->CCR1 = SERVO_POSITION1_DUTY_CYCLE;
		break;
	case 2:
		TIM3->CCR1 = SERVO_POSITION2_DUTY_CYCLE;
		break;
	case 3:
		TIM3->CCR1 = SERVO_POSITION3_DUTY_CYCLE;
		break;
	case 4:
		TIM3->CCR1 = SERVO_POSITION4_DUTY_CYCLE;
		break;
	default:
		TIM3->CCR1 = SERVO_DEFAULT_DUTY_CYCLE;
		break;
	}
}

void motor_free_stop(motor_info_t *motor_info){
	// Clear pins
	HAL_GPIO_WritePin(motor_info->gpio_port, motor_info->fwd_pin|motor_info->en_pin|motor_info->rev_pin, GPIO_PIN_RESET);
	// Set EN Pin only
	HAL_GPIO_WritePin(motor_info->gpio_port, motor_info->en_pin, GPIO_PIN_SET);

}

void motor_active_stop(motor_info_t *motor_info) {
	// Clear pins
	HAL_GPIO_WritePin(motor_info->gpio_port, motor_info->fwd_pin|motor_info->en_pin|motor_info->rev_pin, GPIO_PIN_RESET);
}

void motor_forward(motor_info_t *motor_info) {
	// Clear pins
	HAL_GPIO_WritePin(motor_info->gpio_port, motor_info->fwd_pin|motor_info->en_pin|motor_info->rev_pin, GPIO_PIN_RESET);
	// Set Forward Pin only
	HAL_GPIO_WritePin(motor_info->gpio_port, motor_info->fwd_pin, GPIO_PIN_SET);

}

void motor_reverse(motor_info_t *motor_info) {
	// Clear pins
	HAL_GPIO_WritePin(motor_info->gpio_port, motor_info->fwd_pin|motor_info->en_pin|motor_info->rev_pin, GPIO_PIN_RESET);
	// Set Reverse Pin only
	HAL_GPIO_WritePin(motor_info->gpio_port, motor_info->fwd_pin, GPIO_PIN_SET);
}
