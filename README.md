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
  <img src="https://github.com/user-attachments/assets/25caaf8f-be7e-47f5-bdd8-f101287ffd0a" alt = "Analog PID PCB" width="300" height="300"/>
</p>

# Digital PID Controller

For the PID computerization process, first of all, it is necessary to carry out the modeling of the DC motor without knowing its specific parameters, i.e., using system identification.

A mathematical model of a system must be built through different techniques, since it allows analyzing the performance of a controller on it, however, this is not usually easy. 

Here lies the importance of generating an approximate mathematical model when analyzing an unknown system, which can describe it by means of an experimental approach, which consists of introducing a certain input signal and detecting the output of the device.

To optimize a good parameter identification system, the input signal must have the capacity to contain a wide range of frequencies that facilitate the identification of the entire spectrum of plant dynamics.

In this case, this is achieved by means of a PRBS (Pseudo-Random Binary Sequence) input, a periodic and deterministic signal with properties similar to white noise, in the sense that it changes between two values. Thus, capturing the dynamics of the system with the help of PRBS is more efficient, since it comprises both positive and negative changes within the input sequence. 

Closed-loop identification methods require the application of excitation signals at the process inputs in such a way as to produce changes in the process outputs. The resulting additional variations in the outputs are subject to compromise. They must be large enough to produce persistent excitation for identification, but also disturb as little as possible the normal operation of the process. 

This procedure can also be applied to design the open-loop experiment. Pseudo-random binary sequences are often used as excitation signals for identification proposals, since they have a finite length that can be synthesized repeatedly with simple generators, while exhibiting favorable spectra.

The spectrum at low frequencies is flat and constant, at high frequencies it falls, consequently it has a specific bandwidth that can be used to excite the processes at the required frequencies.

To reiterate, since there will be both positive and negative changes in the input for both the identification and the controller, the DC motor is required to be able to rotate in two directions. For this purpose, an L298N driver was used.

The latter has three pins for rotation control: IN1, IN2 and PWM. If IN1 and IN2 are low, the motor stops. If IN1 is high and IN2 is low, the motor turns forward. If IN1 is low and IN2 is high, the motor rotates backwards. The speed is dictated by the PWM input.

So the physical arrangement consists of the DC motor, connected to the motor output of the H-bridge L298N, which in turn is powered by a 6V DC power supply. IN1, IN2 and PWM are connected to the STM32F103RB microcontroller included in the NUCLEO-F103RB development board.

Therefore, the role of the MCU in the PRBS identification and digital controller is to generate the input or manipulation signal for the motor. Finally, the AS5600 magnetic encoder is in charge of capturing the actuator output, being read by an ADC. 

Here is a description of the main code in C (taking advantage of the abstractions provided by the functions). First, the ADC value is read from the encoder and converted to an angle, indicating the value of that variable for the current position of the motor shaft. By comparing the current angle with the past angle (stored in a variable), the complete rotations (360°) performed by the motor are counted. The total angle (from the time the code starts running) is calculated by multiplying the number of rotations plus the current angle.

```c
currentAngle = ConvertToAngle(ReadEncoder());
detectRotations(currentAngle, previousAngle, &rotations);
totalAngle = (rotations * 360.0) + currentAngle;
```
