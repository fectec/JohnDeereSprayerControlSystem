/*
 * PRBS.c
 */

#include "main.h"
#include "PRBS.h"

/**
 * @brief Detects and counts complete rotations based on the current and previous angle measurements.
 *
 * This function compares the current angle with the previous angle to determine
 * if a full rotation has occurred. It uses a threshold of 180 degrees to account
 * for the circular nature of angle measurement.
 *
 * When the difference exceeds 180 degrees, it is interpreted as a full
 * rotation backward, and the rotation count is decremented. Conversely,
 * if the difference is less than -180 degrees, it indicates a full rotation
 * forward, and the rotation count is incremented.
 *
 * @param currentAngle The current angle measurement (in degrees).
 * @param previousAngle The previous angle measurement (in degrees).
 * @param rotations A pointer to an integer that keeps track of the total
 *                  number of rotations. This value will be modified by the function.
 */

void detectRotations(float currentAngle, float previousAngle, int32_t* rotations)
{
	float difference = currentAngle - previousAngle;

	if (difference > 180)
	{
		(*rotations)--;
	}
	else if (difference < -180)
	{
		(*rotations)++;
	}
}
