/*
 * read.h
 */

#ifndef INC_READ_H_
#define INC_READ_H_

extern ADC_HandleTypeDef hadc1;

// Function prototypes

uint16_t ReadEncoder(void);

float ConvertToVoltage(uint16_t);
float ConvertToAngle(uint16_t);

#endif /* INC_READ_H_ */
