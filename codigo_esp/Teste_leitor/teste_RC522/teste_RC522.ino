#include <MFRC522.h>
#include <SPI.h>

// Mesma fiacao do firmware: 5 SS | 18 SCK | 19 MISO | 22 RST | 23 MOSI
#define PIN_RFID_SS 5 //certo
#define PIN_RFID_RST 22 
#define PIN_RFID_SCK 18 //certo
#define PIN_RFID_MISO 19 
#define PIN_RFID_MOSI 23 //certo

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

  Serial.println("------------------------------");
  Serial.println("Cartao lido");

  // UID em hexadecimal
  Serial.print("UID (hex): ");
  String uidHex = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      Serial.print("0");
      uidHex += "0";
    }
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    uidHex += String(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) {
      Serial.print(":");
    }
  }
  Serial.println();

  uidHex.toUpperCase();
  Serial.print("UID (sem separador): ");
  Serial.println(uidHex);

  // UID em decimal
  Serial.print("UID (dec): ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i]);
    if (i < mfrc522.uid.size - 1) {
      Serial.print(" ");
    }
  }
  Serial.println();

  // Tamanho do UID
  Serial.print("Tamanho do UID: ");
  Serial.print(mfrc522.uid.size);
  Serial.println(" bytes");

  // SAK
  Serial.print("SAK: 0x");
  if (mfrc522.uid.sak < 0x10) {
    Serial.print("0");
  }
  Serial.println(mfrc522.uid.sak, HEX);

  // Tipo do cartao (PICC)
  MFRC522::PICC_Type tipo = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print("Tipo: ");
  Serial.println(mfrc522.PICC_GetTypeName(tipo));

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(1000);
}
