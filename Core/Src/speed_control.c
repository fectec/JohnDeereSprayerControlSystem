/*
 * speed_control.c
 */

#include <main.h>
#include <speed_control.h>

void SetMotorSpeed(uint8_t speed)
{
    // Calculate the compare value based on the speed

    uint32_t autoreload_register_value = __HAL_TIM_GET_AUTORELOAD(&htim2);

    // Max compare value for 100% duty cycle (ARR + 1)

    uint32_t maxValue = autoreload_register_value + 1;

    // Calculate the compare value based on the speed percentage

    uint32_t compareValue = (uint32_t)((speed / 100.0f) * maxValue);

    // Set the PWM duty cycle for TIM_CHANNEL_1 of the timer

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, compareValue);

    // Start the PWM signal generation on TIM_CHANNEL_1

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}
