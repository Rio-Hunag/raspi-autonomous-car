import cv2
import numpy as np
import serial
import time

class PIDController:
    def __init__(self, Kp, Ki, Kd):
        self.Kp = Kp
        self.Ki = Ki
        self.Kd = Kd
        self.prev_error = 0
        self.integral = 0

    def compute(self, error):
        self.integral += error
        self.integral = max(min(self.integral, 100), -100) # Anti-windup
        derivative = error - self.prev_error
        output = (self.Kp * error) + (self.Ki * self.integral) + (self.Kd * derivative)
        self.prev_error = error
        return output

class VisionProcessor:
    def __init__(self):
        self.pid = PIDController(Kp=0.45, Ki=0.0005, Kd=0.08)
        self.base_speed = 120

    def process_frame(self, frame):
        h, w, _ = frame.shape
        roi = frame[int(h * 0.65):h, :]
        gray = cv2.cvtColor(roi, cv2.COLOR_BGR2GRAY)
        blur = cv2.GaussianBlur(gray, (5, 5), 0)
        _, thresh = cv2.threshold(blur, 100, 255, cv2.THRESH_BINARY_INV)

        M = cv2.moments(thresh)
        if M["m00"] > 0:
            cx = int(M["m10"] / M["m00"])
            error = cx - (w // 2)
            steering = self.pid.compute(error)
            
            l_speed = int(np.clip(self.base_speed + steering, 0, 255))
            r_speed = int(np.clip(self.base_speed - steering, 0, 255))
            return l_speed, r_speed, thresh
        return 0, 0, thresh

def main():
    try:
        ser = serial.Serial('/dev/ttyACM0', 115200, timeout=0.1)
        time.sleep(2)
    except Exception as e:
        print(f"Serial port connection error: {e}")
        ser = None

    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)

    processor = VisionProcessor()

    try:
        while cap.isOpened():
            ret, frame = cap.read()
            if not ret:
                break

            l_speed, r_speed, mask = processor.process_frame(frame)
            
            if ser and ser.is_open:
                msg = f"CMD,{l_speed},{r_speed}\n"
                ser.write(msg.encode('utf-8'))

            cv2.imshow("Lane Tracking Mask", mask)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
    finally:
        if ser and ser.is_open:
            ser.write(b"CMD,0,0\n")
            ser.close()
        cap.release()
        cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
