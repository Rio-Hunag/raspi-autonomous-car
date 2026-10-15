# Autonomous Vision Vehicle (Raspberry Pi & Arduino)

An integrated dual-board system for real-time lane tracking and dynamic velocity control.

## 🚀 System Architecture
- **Host (Raspberry Pi 4)**: Captures video feeds via camera module, applies OpenCV image segmentation (ROI + Thresholding), computes steering error using a tuned PID controller, and sends motor speeds over Serial UART.
- **Microcontroller (Arduino)**: Parses serial packets, drives DC motors via PWM (L298N), monitors ultrasonic distance sensor, and executes hardware-level emergency braking.

## 🛠️ Key Technical Features
- **OpenCV Lane Detection**: Uses binary inverse thresholding and image moments to determine lane centerline.
- **PID Steering Logic**: Closed-loop $P, I, D$ control algorithm to eliminate oscillation during cornering.
- **Safety Watchdog & Filtering**: Built-in 500ms timeout watchdog for serial connection loss, coupled with ultrasonic sensor distance protection.

## 📋 Software Prerequisites
- Python 3.x
- OpenCV (`pip install opencv-python`)
- PySerial (`pip install pyserial`)
- NumPy (`pip install numpy`)
