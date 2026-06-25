#include <ESP32Servo.h>

// ESP-WROOM-32 — mesma ordem do firmware Casa Automatica
const int servos[][3] = {
  /*{32, 135, 20},   // D32 janela sala 2
  {33, 40, 150},   // D33 janela sala 1
  *///{26, 15, 175},   // D26 garagem 1
  {27, 130, 15},   // D27 garagem 2
};

const int QTD = sizeof(servos) / sizeof(servos[0]);
Servo motor[QTD];

void setup() {
  Serial.begin(9600);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  for (int i = 0; i < QTD; i++) {
    motor[i].attach(servos[i][0]);
  }
}

void mover(int pos) {
  for (int i = 0; i < QTD; i++) {
    motor[i].write(servos[i][pos]);
  }
}

void loop() {
  mover(1);
  delay(3000);
  mover(2);
  delay(3000);
}
