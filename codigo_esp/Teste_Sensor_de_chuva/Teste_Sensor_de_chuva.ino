// ESP-WROOM-32: POWER D13 | DO D35 (entrada)
#define POWER_PIN 13
#define DO_PIN 35

void setup() {
  Serial.begin(9600);
  pinMode(POWER_PIN, OUTPUT);
  pinMode(DO_PIN, INPUT);
}

void loop() {
  digitalWrite(POWER_PIN, LOW);
  delay(10);
  int rain_state = digitalRead(DO_PIN);
  digitalWrite(POWER_PIN, LOW);

  if (rain_state == HIGH) {
    Serial.println("sem chuva");
  } else {
    Serial.println("chovendo");
  }
  delay(1000);
}
