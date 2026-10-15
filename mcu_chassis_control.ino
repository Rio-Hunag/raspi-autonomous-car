#include <Arduino.h>

// Hardware Pin Assignments
const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int ENA = 5;
const int IN1 = 3;
const int IN2 = 4;
const int ENB = 6;
const int IN3 = 7;
const int IN4 = 8;

const int SAFE_DISTANCE_CM = 15;
unsigned long lastCmdTime = 0;
const unsigned long WATCHDOG_TIMEOUT_MS = 500;

void setMotors(int leftSpeed, int rightSpeed) {
  digitalWrite(IN1, leftSpeed >= 0 ? HIGH : LOW);
  digitalWrite(IN2, leftSpeed >= 0 ? LOW : HIGH);
  analogWrite(ENA, constrain(abs(leftSpeed), 0, 255));

  digitalWrite(IN3, rightSpeed >= 0 ? HIGH : LOW);
  digitalWrite(IN4, rightSpeed >= 0 ? LOW : HIGH);
  analogWrite(ENB, constrain(abs(rightSpeed), 0, 255));
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 15000);
  return (duration == 0) ? 999 : (duration * 0.034 / 2);
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  setMotors(0, 0);
}

void loop() {
  // Safe Emergency Stop
  if (getDistance() < SAFE_DISTANCE_CM) {
    setMotors(0, 0);
    return;
  }

  // Serial Fail-Safe Watchdog
  if (millis() - lastCmdTime > WATCHDOG_TIMEOUT_MS) {
    setMotors(0, 0);
  }

  // Handle Incoming Serial Commands
  if (Serial.available() > 0) {
    String rx = Serial.readStringUntil('\n');
    rx.trim();
    if (rx.startsWith("CMD,")) {
      int idx1 = rx.indexOf(',');
      int idx2 = rx.indexOf(',', idx1 + 1);
      if (idx1 != -1 && idx2 != -1) {
        int l = rx.substring(idx1 + 1, idx2).toInt();
        int r = rx.substring(idx2 + 1).toInt();
        setMotors(l, r);
        lastCmdTime = millis();
      }
    }
  }
}
