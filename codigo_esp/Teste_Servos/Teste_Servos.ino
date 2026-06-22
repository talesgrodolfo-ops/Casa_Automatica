#include <ESP32Servo.h>


const int servos[][3] = {
  {12, 150,  80},
  //{32, 150,  20},
  //{33,  40, 150},
	//{26, 0, 150},
  //{27, 170, 10}
};

const int QTD = sizeof(servos) / sizeof(servos[0]);
Servo motor[QTD];

void setup() {
  for (int i = 0; i < QTD; i++)
    motor[i].attach(servos[i][0]);

  pinMode(23, OUTPUT);
}

void mover(int pos) {
  for (int i = 0; i < QTD; i++)
    motor[i].write(servos[i][pos]);
}

void loop() {
  digitalWrite(23, HIGH);

  mover(1);
  delay(3000);

  mover(2);
  delay(3000);
}