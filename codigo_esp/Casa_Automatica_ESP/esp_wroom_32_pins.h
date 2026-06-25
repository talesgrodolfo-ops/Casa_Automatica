#pragma once

/*
 * ESP-WROOM-32 — restricoes do modulo + pinagem deste projeto:
 *
 *   12 porta RFID    14 portao API    26/27 garagens
 *   32/33 janelas      13 chuva POWER   35 chuva DO
 *   25/15/2/4 lampadas   5/18/19/22/23 RFID
 *
 * PROIBIDO: GPIO 6-11 (flash) | SOMENTE ENTRADA: 34-39
 * STRAPPING boot: 0, 2, 12, 15 — GPIO 12 e 2 usados na sua fiacao (aviso no Serial)
 */

inline bool wroomPinFlashInterna(int pin) {
  return pin >= 6 && pin <= 11;
}

inline bool wroomPinSomenteEntrada(int pin) {
  return pin >= 34 && pin <= 39;
}

inline bool wroomPinStrapping(int pin) {
  return pin == 0 || pin == 2 || pin == 12 || pin == 15;
}

inline bool wroomPinUartUsb(int pin) {
  return pin == 1 || pin == 3;
}

inline bool wroomPinPermiteSaida(int pin) {
  if (pin < 0 || pin > 33) {
    return false;
  }
  if (wroomPinFlashInterna(pin) || wroomPinSomenteEntrada(pin)) {
    return false;
  }
  return true;
}

inline bool wroomPinPermiteEntrada(int pin) {
  return pin >= 0 && pin <= 39 && !wroomPinFlashInterna(pin);
}

inline bool wroomPinOkServo(int pin) {
  return wroomPinPermiteSaida(pin);
}
