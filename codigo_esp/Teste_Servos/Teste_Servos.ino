#include <ESP32Servo.h>

struct ServoTeste {
  int pin;
  const char* nome;
};

const ServoTeste servos[] = {
  {32, "janela sala 1"},
  {33, "janela sala 2"},
  {26, "garagem 1"},
  {27, "garagem 2"},
  {12, "portao"},
  {14, "porta rfid"},
};

const int QTD = sizeof(servos) / sizeof(servos[0]);

const int ANGULO_MIN = 0;
const int ANGULO_MAX = 180;
const int PASSO = 1;             // graus por etapa
const int DELAY_PASSO_MS = 40;   // velocidade da varredura (maior = mais lento, mais facil de marcar)

Servo motor;

void limparSerial() {
  while (Serial.available()) {
    Serial.read();
  }
}

void esperarTecla(const char* msg) {
  Serial.println(msg);
  limparSerial();
  while (!Serial.available()) {
    delay(10);
  }
  limparSerial();
}

// Gira de "de" ate "ate", parando se o usuario apertar uma tecla.
// Retorna o angulo em que parou (a marca do usuario, ou o limite se ninguem marcou).
int varrer(int de, int ate) {
  int passo = (ate >= de) ? PASSO : -PASSO;
  int ang = de;
  motor.write(ang);
  delay(DELAY_PASSO_MS);

  while (ang != ate) {
    if (Serial.available()) {
      limparSerial();
      return ang;
    }
    ang += passo;
    motor.write(ang);
    Serial.print(ang);
    Serial.println(" graus");
    delay(DELAY_PASSO_MS);
  }
  return ate;
}

void testar(const ServoTeste& s) {
  motor.attach(s.pin);
  motor.write(ANGULO_MIN);

  Serial.println();
  Serial.print("=== ");
  Serial.print(s.nome);
  Serial.print(" (GPIO ");
  Serial.print(s.pin);
  Serial.println(") ===");

  esperarTecla("Aperte uma tecla para iniciar a IDA...");
  Serial.println("IDA: girando... aperte quando TRAVAR.");
  int ida = varrer(ANGULO_MIN, ANGULO_MAX);
  Serial.print(">>> IDA marcada em ");
  Serial.print(ida);
  Serial.println(" graus");

  delay(500);

  esperarTecla("Aperte uma tecla para iniciar a VOLTA...");
  Serial.println("VOLTA: girando... aperte quando TRAVAR.");
  int volta = varrer(ida, ANGULO_MIN);
  Serial.print(">>> VOLTA marcada em ");
  Serial.print(volta);
  Serial.println(" graus");

  Serial.println();
  Serial.print("RESULTADO ");
  Serial.print(s.nome);
  Serial.print(" (GPIO ");
  Serial.print(s.pin);
  Serial.print("): IDA = ");
  Serial.print(ida);
  Serial.print(" graus | VOLTA = ");
  Serial.print(volta);
  Serial.println(" graus");

  motor.detach();
}

void setup() {
  Serial.begin(9600);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  Serial.println();
  Serial.println("=== Teste interativo de limite dos servos ===");
}

void loop() {
  for (int i = 0; i < QTD; i++) {
    testar(servos[i]);
    delay(500);
  }
  Serial.println();
  Serial.println("=== Todos os servos testados. Reinicie a placa para repetir. ===");
  while (true) {
    delay(1000);
  }
}
