#pragma once

#include <esp_wifi.h>

struct CandidatoAp {
  int canal;
  int rssi;
  uint8_t bssid[6];
  wifi_auth_mode_t modo;
};
