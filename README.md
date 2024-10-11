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

As we can see, the output signal follows closely the reference signal, which means there is little error between them, so the controller is working just fine. Even though the circuit was tested on a breadboard, a PCB was also designed.

<p align="center">
  <img src="https://github.com/user-attachments/assets/25caaf8f-be7e-47f5-bdd8-f101287ffd0a" alt = "Analog PID PCB" width="300" height="300"/>
</p>
