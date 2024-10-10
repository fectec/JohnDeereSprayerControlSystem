/*
 * motor.c
 */

#include "main.h"
#include "motor.h"
#include "speed_control.h"

void Motor_Stop(void)
{
	HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_RESET);	// IN1 LOW
	HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_RESET);	// IN2 LOW

	SetMotorSpeed(0);
}

void Motor_Forward(uint8_t speed)
{
	// Check if the speed is within the valid range (0 to 100)

	if (speed < 0 || speed > 100)
	{
		return;
	}

	HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_SET);	// IN1 HIGH
	HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_RESET);	// IN2 LOW

	SetMotorSpeed(speed);
}

void Motor_Backward(uint8_t speed)
{
	// Check if the speed is within the valid range (0 to 100)

	if (speed < 0 || speed > 100)
	{
		return;
	}

	HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_RESET);	// IN1 LOW
	HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_SET);	// IN2 HIGH

	SetMotorSpeed(speed);
}
