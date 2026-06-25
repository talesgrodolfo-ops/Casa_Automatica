#include <MFRC522.h>
#include <SPI.h>

// Mesma fiacao do firmware: 5 SS | 18 SCK | 19 MISO | 22 RST | 23 MOSI
#define PIN_RFID_SS 5
#define PIN_RFID_RST 22
#define PIN_RFID_SCK 18
#define PIN_RFID_MISO 19
#define PIN_RFID_MOSI 23

MFRC522 mfrc522(PIN_RFID_SS, PIN_RFID_RST);

void setup() {
  Serial.begin(9600);
  SPI.begin(PIN_RFID_SCK, PIN_RFID_MISO, PIN_RFID_MOSI, PIN_RFID_SS);
  mfrc522.PCD_Init();

  byte versao = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  if (versao == 0x00 || versao == 0xFF) {
    Serial.println("ERRO: RFID nao detectado");
  } else {
    Serial.print("RFID ok versao=0x");
    Serial.println(versao, HEX);
  }
  Serial.println("Aproxime o cartao...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println("Cartao lido");

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(1000);
}
