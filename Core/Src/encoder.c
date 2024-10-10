/*
 * encoder.c
 */

#include "main.h"
#include "encoder.h"

uint16_t ReadEncoder(void)
{
    HAL_ADC_Start(&hadc1);  								// Start ADC
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);		// Wait for conversion to complete
    uint32_t adcValue = HAL_ADC_GetValue(&hadc1);			// Get the ADC value
    HAL_ADC_Stop(&hadc1);									// Stop ADC
    return adcValue;										// Return the ADC value
}

float ConvertToVoltage(uint16_t adcValue)
{
    float V_ref = 3.3; 										// Reference voltage
    float maxADCValue = 4095.0; 							// Max value for 12-bit ADC
    return (adcValue / maxADCValue) * V_ref;
}
