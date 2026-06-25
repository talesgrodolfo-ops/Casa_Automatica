// Teste isolado de Wi-Fi — placa so com USB (sem fios nos GPIO)
// Serial Monitor: 9600 baud

#include <WiFi.h>
#include <esp_wifi.h>

#define WIFI_SSID "amorzito"
#define WIFI_PASSWORD "donagolda"

void conectar() {
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  WiFi.disconnect(true);
  delay(200);
  esp_wifi_set_ps(WIFI_PS_NONE);

  wifi_config_t cfg = {};
  strlcpy(reinterpret_cast<char*>(cfg.sta.ssid), WIFI_SSID, sizeof(cfg.sta.ssid));
  strlcpy(reinterpret_cast<char*>(cfg.sta.password), WIFI_PASSWORD, sizeof(cfg.sta.password));
  cfg.sta.threshold.authmode = WIFI_AUTH_WPA_PSK;
  cfg.sta.pmf_cfg.capable = true;
  cfg.sta.pmf_cfg.required = false;

  esp_wifi_set_config(WIFI_IF_STA, &cfg);
  esp_wifi_connect();
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println();
  Serial.println("=== TESTE WIFI ESP32 ===");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  Serial.println("Escaneando...");
  int n = WiFi.scanNetworks(false, true);
  for (int i = 0; i < n && i < 12; i++) {
    Serial.print("  ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" ch");
    Serial.print(WiFi.channel(i));
    Serial.print(" ");
    Serial.print(WiFi.RSSI(i));
    Serial.println(" dBm");
  }

  Serial.println("Conectando (WPA2 + PMF opcional)...");
  conectar();

  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 45000) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED && WiFi.localIP()[0] != 0) {
    Serial.print("OK IP ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("FALHOU - senha errada ou roteador WPA3");
    Serial.println("No roteador: rede 2.4GHz -> WPA2-PSK");
  }
}

void loop() {
  delay(15000);
}
