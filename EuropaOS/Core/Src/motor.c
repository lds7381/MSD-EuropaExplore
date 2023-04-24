/*
 * motor.c
 *
 *  Created on: Apr 12, 2023
 *      Author: liams
 */

#include "motor.h"

uint8_t count = 0, last_count = 0;
uint8_t tx;

void motor_task(motor_info_t *motor_info) {
	while (1) {
		if (last_count != count) {
			switch ((char)tx):
				case 'W':
				case 'w':
					motor_instruction(motor_info, MOTOR_FORWARD);
					break;
				case 'S':
				case 's':
					motor_instruction(motor_info, MOTOR_REVERSE);
					break;
				case 'X':
				case 'x':
					motor_instruction(motor_info, MOTOR_ACTIVE_STOP);
					break;
				case '0':
					servo_move_pos(0);
					break;
				case '1':
					servo_move_pos(1);
					break;
				case '2':
					servo_move_pos(0);
					break;
				case '3':
					servo_move_pos(1);
					break;
				case '3':
					servo_move_pos(1);
					break;
				default:
					break;
		}
		else {
			osDelay(100);
		}
	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	print(&huart, "H\r\n", 4);
	HAL_UART_Receive_IT(&huart, &tx, 1);
	count++;
}

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
