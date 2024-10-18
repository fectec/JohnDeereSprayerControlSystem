# John Deere Sprayer Control System

Main project in collaboration with *John Deere* for the undergrad course “**Analysis of Control Systems**”, which delves mainly into *Classic and Modern Control*.

<p align="center">
  <img src="https://github.com/user-attachments/assets/d9fcd6d7-fb5b-44bf-84cf-5ca00228ec28" alt = "NUCLEO-F103RB" width="200" height="200"/>
</p>

The **John Deere Sprayer Control System** is a computerized irrigation management solution designed to stabilize the operation of a 3D-printed sprayer model. Utilizing the *NUCLEO-F103RB* development board equipped with the *STM32F103RB MCU*, this project provides control over a DC motor to which the sprayer model is attached.

## PID Controller using Analog Electronics

As a first approach before computerization, a PID controller was implemented using analog electronics. The physical configuration consists of a *12V DC motor* placed on a 3D-printed base that allows aligning its rotation axis in front of an *AS5600 magnetic encoder*, in order to know the position of the shaft and close the loop.

<p align="center">
  <img src="https://github.com/user-attachments/assets/a7d2a1ad-4e19-43e9-a170-1fcfd81d4bbb" alt = "Base 3D Model" width="150" height="150"/>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/30881e0d-5c65-45e7-87dd-a92824c35ff9" alt = "Analog PID Schematic"/>
</p>

In the initial stage, the circuit receives the *reference signal* (desired value, adjusted with a potentiometer) and the signal coming from the encoder (*feedback*, output of the system). Both are subtracted to obtain the *error signal*, which is then fed to the PID. The *proportional gain* is intended to correct the current error, the *integral gain* to eliminate the error accumulated over time, and the *derivative gain* to anticipate future errors and smooth the system response. These three terms are summed to generate a fine-tuned control signal that feeds the motor. Finally, the *output signal* feeds back to the comparator, thus closing the loop. 

<p align="center">
  <img src="https://github.com/user-attachments/assets/8d734614-c554-4a87-bc00-cfb34d40f9cc" alt = "Testing Analog PID With Oscilloscope" width="300" height="180"/>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/c54bf791-f1bb-480d-95e6-29f38e368ad9" alt = "Testing Analog PID With Oscilloscope (Reference and Output Signals)" width="300" height="150"/>
</p>

As we can see, the output signal follows closely the reference signal, which means there is little error between them, so the controller is working just fine. Even though the circuit was tested on a breadboard, a *PCB* was also designed.

<p align="center">
  <img src="https://github.com/user-attachments/assets/25caaf8f-be7e-47f5-bdd8-f101287ffd0a" alt = "Analog PID PCB Model" width="300" height="300"/>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/89f89972-e9fc-4157-9c29-223d8cb35f64" alt = "Analog PID PCB" width="300" height="180"/>

## Digital PID Controller

For the PID computerization process, first of all, it is necessary to carry out the modeling of the DC motor without knowing its specific parameters, i.e., using *system identification*. A mathematical model of a system must be built through different techniques, since it allows analyzing the performance of a controller on it, however, this is not usually easy. 

Here lies the importance of generating an approximate mathematical model when analyzing an unknown system, which can describe it by means of an experimental approach, which consists of introducing a certain input signal and detecting the output of the device. To optimize a good parameter identification system, the input signal must have the capacity to contain a wide range of frequencies that facilitate the identification of the entire spectrum of plant dynamics.

In this case, this is achieved by means of a *PRBS* (*Pseudo-Random Binary Sequence*) input, a periodic and deterministic signal with properties similar to white noise, in the sense that it changes between two values. Thus, capturing the dynamics of the system with the help of PRBS is more efficient, since it comprises both positive and negative changes within the input sequence. 

<p align="center">
  <img src="https://github.com/user-attachments/assets/8749c2ac-4104-4de0-bf5c-70d8bc2c49f8" alt = "PRBS" width="300" height="180"/>
</p>

Closed-loop identification methods require the application of excitation signals at the process inputs in such a way as to produce changes in the process outputs. The resulting additional variations in the outputs are subject to compromise. They must be large enough to produce persistent excitation for identification, but also disturb as little as possible the normal operation of the process. 

This procedure can also be applied to design the open-loop experiment. Pseudo-random binary sequences are often used as excitation signals for identification proposals, since they have a finite length that can be synthesized repeatedly with simple generators, while exhibiting favorable spectra. The spectrum at low frequencies is flat and constant, at high frequencies it falls, consequently it has a specific bandwidth that can be used to excite the processes at the required frequencies.

To reiterate, since there will be both positive and negative changes in the input for both the identification and the controller, the DC motor is required to be able to rotate in two directions. For this purpose, an *L298N driver* was used. The latter has three pins for rotation control: *IN1*, *IN2* and *PWM*. If IN1 and IN2 are low, the motor stops. If IN1 is high and IN2 is low, the motor turns forward. If IN1 is low and IN2 is high, the motor rotates backwards. The speed is dictated by the PWM input.

So the physical arrangement consists of the DC motor, connected to the motor output of the H-bridge L298N, which in turn is powered by a DC power supply. IN1, IN2 and PWM are connected to the STM32F103RB microcontroller included in the NUCLEO-F103RB development board.

Therefore, the role of the MCU in the PRBS identification and digital controller is to generate the input or manipulation signal for the motor. Finally, the AS5600 magnetic encoder is in charge of capturing the actuator output, being read by an ADC. 

<p align="center">
  <img src="https://github.com/user-attachments/assets/0810cce3-0f88-45ff-aacd-770d19f6da28" alt = "Digital PID" width="400" height="220"/>
</p>

Here is a description of the main code in *C* (taking advantage of the abstractions provided by the functions). First, the ADC value is read from the encoder and converted to an angle, indicating the value of that variable for the current position of the motor shaft. By comparing the current angle with the past angle (stored in a variable), the complete rotations (360°) performed by the motor are counted. The total angle (from the time the code starts running) is calculated by multiplying the number of rotations plus the current angle.

```c
currentAngle = ConvertToAngle(ReadEncoder());
detectRotations(currentAngle, previousAngle, &rotations);
totalAngle = (rotations * 360.0) + currentAngle;
```

Then, based on a seed, a pseudo-random number is generated, with which the direction of the motor is determined. Once this is done, the seed is updated. This step represents the generation of the PRBS signal, which as specified above, is periodic, given the cyclic nature of the seed.

```c
pseudoRandomNumber = seed ^ (seed >> 3) >> 1;
direction = (pseudoRandomNumber & 0x0001);
seed = (seed >> 1);
seed = (seed + (direction << 31));
```

Depending on the direction value, the motor is controlled and the adjusted velocity, i.e. the value that entered the plant, is stored in a variable. If the generated direction was backward, the adjusted velocity is -100 (full speed backward), if the generated direction was forward, the the adjusted velocity is 100 (full speed forward). In other words, this is the operating range of the PID manipulation signal. 

```c
if (direction == 0)
{
	Motor_Backward(100.0);
	adjustedVelocity = -100;
}
else
{
	Motor_Forward(100.0);
	adjustedVelocity = 100;
}
```

Subsequently, the iteration time is calculated. To conclude, three data are sent through the serial port to specify the identification: Adjusted speed or input signal (in this case, -100 or 100), the angle derived from such input or output signal (in this case, from 0 to infinity, since it is the total or accumulated angle) and the elapsed time per iteration, which symbolizes the sampling time for the controller.

```c
previousTime = currentTime;
currentTime = HAL_GetTick();
elapsedTime = currentTime - previousTime;

printf("%d, %ld, %ld\r\n", adjustedVelocity, (int32_t) totalAngle, elapsedTime);

previousAngle = currentAngle;

HAL_Delay(10);
```

A 10 ms delay is placed to add an extra margin of operation for the PID. That is, by increasing the sampling time of the controller, it is guaranteed to operate correctly in the worst case (the longest operating time).
		
When performing the PRBS test, a Python script is run that collects data from the serial port and stores it in a *.csv* file. These are entered as workspace variables in MATLAB and exported to the *System Identification* app. Here, the approximate model is estimated as a transfer function, and the number of *zeros* and *poles* can be modified. It is possible to export different models, and to create controllers for each one. 

<p align="center">
  <img src="https://github.com/user-attachments/assets/6589cba0-738c-45c3-8065-649297ac3791" alt = "Exporting Data To System Identification App" width="500" height="380"/>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/4b7cf1c2-4669-4b3b-be67-b322d3df179a" alt = "Estimating Transfer Function Model" width="500" height="580"/>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/62702187-32d4-4e90-a3a0-c1e49562c638" alt = "Transfer Function Models" width="500" height="300"/>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/9ebfecc4-216a-4f0f-84d2-a036c4e994c1" alt = "Transfer Function Models Accuracy" width="500" height="300"/>
</p>

The transfer function is then entered into the PID Tuner application, which is used to generate the PID controller (with the possibility of choosing between other types, such as PI and PD) by altering parameters such as Response Time (slower or faster) or Transient Behavior (more aggressive or more robust). Again, the result is exported to the workspace, from where *Kp*, *Kd* and *Ki* are extracted of the tuned PID. 

<p align="center">
  <img src="https://github.com/user-attachments/assets/66f6c0ac-a6cf-49ca-956b-fd8deefe5fa3" alt = "Transfer Function Models Transient Responses" width="500" height="300"/>
</p>

Below is the description of the discrete PID code, which uses these constants.

```c
currentAngle = ConvertToAngle(ReadEncoder());
detectRotations(currentAngle, previousAngle, &rotations);
totalAngle = (rotations * 360.0) + currentAngle;
```

Identical to the PRBS code, the total motor angle is obtained. Now, the setpoint is read, which like the analog PID is given by a potentiometer and its output voltage, which is read by the ADC of the microcontroller and converted to an angle. An averaging filter was implemented for the setpoint, where up to 20 readings are averaged, using a circular buffer that removes the oldest cost and adds the newest. This is for the purpose of removing noise.

```c
sumReadings -= setpointReadings[readingIndex];
setpointReadings[readingIndex] = ConvertToAngle(ReadSetpoint());
sumReadings += setpointReadings[readingIndex];
readingIndex = (readingIndex + 1) % NUM_READINGS;				

if (readingIndex == 0) bufferFull = true;					

if (bufferFull)
{
	setpoint = (float) sumReadings / NUM_READINGS;
}
else
{
	setpoint = (float) sumReadings / (readingIndex + 1);
}
```

The elapsed time and the error, which is the subtraction between the setpoint and the total angle, are calculated. Using the discrete-time PID formula, the manipulation signal is determined, which depends on the last output, the error two sampling instants ago, the last error, the current error, the sampling time, Kp, Kd and Ki.

```c
currentTime = HAL_GetTick();
elapsedTime = currentTime - previousTime;

lastLastError = lastError;
lastError = error;

error = setpoint - totalAngle;

lastOutput = output;

output = lastOutput + (Kd / samplingTime) * lastLastError + (-Kp - 2.0 * (Kd / samplingTime) + (Ki * samplingTime)) * lastError + (Kp + (Kd / samplingTime)) * error;
```

If the manipulation exceeds the set limits (-100 or 100) it is trimmed. We proceed to move the motor with the output signal from the controller. If it is negative, the function to move the motor backwards is called, if it is positive, the function to move the motor forward is called, if it is 0, the motor is stopped.

```c
if (output < -100) output = -100;
if (output > 100) output = 100;

if (output < 0)
{
	Motor_Backward(fabs(output));
}
else if (output > 0)
{
	Motor_Forward(output);
}
else
{
	Motor_Stop();
}
```

A pause is added to smooth the actuator motion, and the previous time and angle variables are updated for the next iteration.

```c
previousTime = currentTime;

previousAngle = currentAngle;
```
