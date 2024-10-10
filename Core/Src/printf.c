/*
 * printf.c
 */

#include "main.h"
#include "printf.h"

// Retargets the C library printf function to the USART

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}
