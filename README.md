Autonomous Vision Vehicle (Raspberry Pi & Arduino)

A dual-board self-driving car that tracks lanes in real-time and automatically adjusts its speed.

System Architecture

Raspberry Pi 4: Handles camera input, uses OpenCV (ROI + thresholding) for lane detection, runs a PID controller to calculate steering error, and sends motor controls to the Arduino via Serial UART.

Arduino: Receives serial commands, drives the DC motors using an L298N driver, checks distance with an ultrasonic sensor, and handles hard braking if an obstacle is too close.

Key Features

OpenCV lane detection using binary inverse thresholding and image moments.

PID-based steering control to keep cornering smooth.

Built-in safety features: auto-stops if serial connection drops for over 500ms, plus ultrasonic obstacle detection.

Requirements

Python 3

OpenCV (pip install opencv-python)

PySerial (pip install pyserial)

NumPy (pip install numpy)
