/*
 * motor.c
 */

#include "main.h"
#include "motor.h"
#include "speed_control.h"

/**
 * @brief Stop the motor.
 *
 * This function sets both IN1 and IN2 pins to LOW, effectively stopping
 * the motor. It also sets the motor speed to zero.
 */

void Motor_Stop(void)
{
	HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_RESET);	// IN1 LOW
	HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_RESET);	// IN2 LOW

	SetMotorSpeed(0);
}

/**
 * @brief Rotate the motor forward at a specified speed.
 *
 * This function checks if the provided speed is within the valid range
 * (0 to 100). If valid, it sets IN1 to HIGH and IN2 to LOW to rotate
 * the motor forward, then sets the motor speed using PWM.
 *
 * @param speed The speed percentage (0 to 100).
 */

void Motor_Forward(float speed)
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

/**
 * @brief Rotate the motor backward at a specified speed.
 *
 * This function checks if the provided speed is within the valid range
 * (0 to 100). If valid, it sets IN1 to LOW and IN2 to HIGH to rotate
 * the motor backward, then sets the motor speed using PWM.
 *
 * @param speed The speed percentage (0 to 100).
 */

void Motor_Backward(float speed)
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
