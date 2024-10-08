/*
 * speed_control.h
 */

#ifndef INC_SPEED_CONTROL_H_
#define INC_SPEED_CONTROL_H_

extern TIM_HandleTypeDef htim2;

// Function prototypes

void SetMotorSpeed(uint8_t);	// Function to set motor speed based on
								// input scale from 0 to 100

#endif /* INC_SPEED_CONTROL_H_ */
