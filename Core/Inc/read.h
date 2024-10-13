/*
 * read.h
 */

#ifndef INC_READ_H_
#define INC_READ_H_

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

// Function prototypes

uint16_t ReadEncoder(void);
uint16_t ReadSetpoint(void);

float ConvertToVoltage(uint16_t);
float ConvertToAngle(uint16_t);

#endif /* INC_READ_H_ */
