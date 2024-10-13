/*
 * encoder.c
 */

#include "main.h"
#include "encoder.h"

/**
 * @brief Read the ADC value from the encoder.
 *
 * This function starts the ADC, waits for the conversion to complete,
 * retrieves the converted ADC value, and then stops the ADC.
 * It returns the ADC value as a 16-bit unsigned integer.
 */

uint16_t ReadEncoder(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    uint16_t adcValue = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return adcValue;
}

/**
 * @brief Convert the ADC value to a corresponding voltage.
 *
 * This function takes a 16-bit ADC value as input and converts it to
 * its equivalent voltage based on a reference voltage of 3.3V and
 * a maximum encoder value of 4077 (for a 12-bit ADC). It returns the voltage
 * as a floating-point number.
 */

float ConvertToVoltage(uint16_t adcValue)
{
    float V_ref = 3.3;
    float maxEncoderValue = 4077.0;

    return (adcValue / maxEncoderValue) * V_ref;
}

/**
 * @brief Convert the ADC value to an angle.
 *
 * This function takes a 16-bit ADC value and converts it to the corresponding
 * angle in degrees since a full rotation maps to 360 degrees.
 * It returns the angle as an integer.
 */

uint16_t ConvertToAngle(uint16_t adcValue)
{
    float maxEncoderValue = 4077.0;
    float fullRotationAngle = 360.0;

    return (uint16_t)((adcValue / maxEncoderValue) * fullRotationAngle);
}
