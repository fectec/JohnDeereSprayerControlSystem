/*
 * encoder.h
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

extern ADC_HandleTypeDef hadc1;

// Function prototypes

uint16_t ReadEncoder(void);
float ConvertToVoltage(uint16_t);

#endif /* INC_ENCODER_H_ */
