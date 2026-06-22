/*
 * Casa Automática — firmware ESP32
 *
 * Sincroniza com a API:
 *   GET  /api/portao, /api/janelas, /api/quartos (opcional)
 *   POST /api/chuva, /api/portao/toggle (RFID)
 *
 * Copie config.example.h para config.h e preencha Wi-Fi + API.
 */

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

#include "config.h"

#if ENABLE_RFID
#include <SPI.h>
#include <MFRC522.h>
#endif

static const char* HDR_API_KEY = "X-API-Key";
static const char* TAG = "[casa]";

struct JanelaServoConfig {
  const char* nome;
  int pin;
  int anguloAberto;
  int anguloFechado;
};

static const JanelaServoConfig JANELAS_CFG[] = {
  {JANELA_1_NOME, PIN_SERVO_JANELA_1, ANGULO_JANELA_1_ABERTO, ANGULO_JANELA_1_FECHADO},
  {JANELA_2_NOME, PIN_SERVO_JANELA_2, ANGULO_JANELA_2_ABERTO, ANGULO_JANELA_2_FECHADO},
#if ENABLE_JANELA_SALA
  {JANELA_3_NOME, PIN_SERVO_JANELA_3, ANGULO_JANELA_3_ABERTO, ANGULO_JANELA_3_FECHADO},
#endif
};

static const size_t QTD_JANELAS = sizeof(JANELAS_CFG) / sizeof(JANELAS_CFG[0]);

struct JanelaServoEstado {
  Servo motor;
  bool aberta;
};

JanelaServoEstado janelas[QTD_JANELAS];

Servo servoPortao;
WiFiClientSecure secureClient;
bool httpsPronto = false;
bool portaoAberto = false;
bool ultimaChuva = false;
unsigned long ultimoEnvioChuva = 0;
unsigned long ultimaSyncPortao = 0;
unsigned long ultimaSyncJanelas = 0;
unsigned long ultimaVerificacaoWifi = 0;

#if ENABLE_RFID
MFRC522 leitorRfid(PIN_RFID_SS, PIN_RFID_RST);
#endif

void logLinha(const String& msg) {
  Serial.println(String(TAG) + " " + msg);
}

String normalizarNome(const String& nome) {
  String n = nome;
  n.trim();
  n.toLowerCase();
  return n;
}

bool usaHttps() {
  return String(API_BASE_URL).startsWith("https://");
}

void prepararHttps() {
  if (!usaHttps() || httpsPronto) {
    return;
  }
  secureClient.setInsecure();
  httpsPronto = true;
}

bool iniciarHttp(HTTPClient& http, const String& path) {
  String url = String(API_BASE_URL) + path;
  if (usaHttps()) {
    prepararHttps();
    return http.begin(secureClient, url);
  }
  return http.begin(url);
}

bool wifiConectado() {
  return WiFi.status() == WL_CONNECTED;
}

void conectarWifi() {
  if (wifiConectado()) {
    return;
  }

  logLinha("wifi: conectando em " + String(WIFI_SSID));
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long inicio = millis();
  while (!wifiConectado() && millis() - inicio < 20000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (wifiConectado()) {
    logLinha("wifi: ok — IP " + WiFi.localIP().toString());
  } else {
    logLinha("wifi: falhou");
  }
}

void aplicarHeadersApi(HTTPClient& http) {
  http.addHeader("Content-Type", "application/json");
  http.addHeader(HDR_API_KEY, API_KEY);
}

bool lerChuva() {
  digitalWrite(PIN_CHUVA_POWER, LOW);
  delay(10);
  int estado = digitalRead(PIN_CHUVA_DO);
  digitalWrite(PIN_CHUVA_POWER, LOW);
  return estado == LOW;
}

int indiceJanelaPorNome(const String& nomeApi) {
  String alvo = normalizarNome(nomeApi);
  for (size_t i = 0; i < QTD_JANELAS; i++) {
    if (normalizarNome(JANELAS_CFG[i].nome) == alvo) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

void moverJanela(size_t indice, bool aberta) {
  if (indice >= QTD_JANELAS) {
    return;
  }

  const JanelaServoConfig& cfg = JANELAS_CFG[indice];
  int angulo = aberta ? cfg.anguloAberto : cfg.anguloFechado;
  janelas[indice].motor.write(angulo);
  janelas[indice].aberta = aberta;

  logLinha(
    String("janela ") + cfg.nome + " " + (aberta ? "aberta" : "fechada") + " (" + angulo + " graus)"
  );
}

void fecharTodasJanelas() {
  for (size_t i = 0; i < QTD_JANELAS; i++) {
    if (janelas[i].aberta) {
      moverJanela(i, false);
    }
  }
}

void moverServoPortao(bool aberto) {
  int angulo = aberto ? ANGULO_PORTAO_ABERTO : ANGULO_PORTAO_FECHADO;
  servoPortao.write(angulo);
  portaoAberto = aberto;
  logLinha(String("portao ") + (aberto ? "aberto" : "fechado") + " (" + angulo + " graus)");
}

bool postChuva(bool chovendo) {
  if (!wifiConectado()) {
    return false;
  }

  HTTPClient http;
  if (!iniciarHttp(http, "/api/chuva")) {
    return false;
  }

  aplicarHeadersApi(http);
  String body = chovendo ? "{\"rain\":\"yes\"}" : "{\"rain\":\"no\"}";
  int code = http.POST(body);
  http.end();

  if (code == HTTP_CODE_OK || code == HTTP_CODE_CREATED) {
    logLinha(String("POST /api/chuva -> ") + (chovendo ? "yes" : "no"));
    if (chovendo) {
      fecharTodasJanelas();
    }
    return true;
  }

  logLinha("POST /api/chuva falhou, HTTP " + String(code));
  return false;
}

bool getPortao(bool& aberto) {
  if (!wifiConectado()) {
    return false;
  }

  HTTPClient http;
  if (!iniciarHttp(http, "/api/portao")) {
    return false;
  }

  http.addHeader(HDR_API_KEY, API_KEY);
  int code = http.GET();
  if (code != HTTP_CODE_OK) {
    logLinha("GET /api/portao falhou, HTTP " + String(code));
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();

  StaticJsonDocument<256> doc;
  if (deserializeJson(doc, payload)) {
    logLinha("JSON invalido em /api/portao");
    return false;
  }

  const char* state = doc["state"] | "closed";
  aberto = String(state) == "open";
  return true;
}

bool sincronizarJanelasDaApi() {
  if (!wifiConectado()) {
    return false;
  }

  HTTPClient http;
  if (!iniciarHttp(http, "/api/janelas")) {
    return false;
  }

  http.addHeader(HDR_API_KEY, API_KEY);
  int code = http.GET();
  if (code != HTTP_CODE_OK) {
    logLinha("GET /api/janelas falhou, HTTP " + String(code));
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();

  StaticJsonDocument<2048> doc;
  if (deserializeJson(doc, payload)) {
    logLinha("JSON invalido em /api/janelas");
    return false;
  }

  bool bloqueadas = doc["windowsBlocked"] | false;
  JsonArray lista = doc["windows"].as<JsonArray>();
  if (lista.isNull()) {
    return false;
  }

  for (JsonObject janela : lista) {
    String nome = janela["window"].as<String>();
    int idx = indiceJanelaPorNome(nome);
    if (idx < 0) {
      continue;
    }

    bool deveAbrir = janela["state"].as<String>() == "open";
    if (bloqueadas) {
      deveAbrir = false;
    }

    if (deveAbrir != janelas[idx].aberta) {
      moverJanela(static_cast<size_t>(idx), deveAbrir);
    }
  }

  return true;
}

bool postTogglePortao() {
  if (!wifiConectado()) {
    return false;
  }

  HTTPClient http;
  if (!iniciarHttp(http, "/api/portao/toggle")) {
    return false;
  }

  aplicarHeadersApi(http);
  int code = http.POST((uint8_t*)nullptr, 0);
  String payload = http.getString();
  http.end();

  if (code != HTTP_CODE_OK) {
    logLinha("POST /api/portao/toggle falhou, HTTP " + String(code));
    return false;
  }

  StaticJsonDocument<256> doc;
  if (deserializeJson(doc, payload)) {
    logLinha("JSON invalido em /api/portao/toggle");
    return false;
  }

  const char* state = doc["state"] | "closed";
  moverServoPortao(String(state) == "open");
  logLinha("portao alternado via RFID");
  return true;
}

#if ENABLE_LEDS_LUZES
void sincronizarLuzes() {
  if (!wifiConectado()) {
    return;
  }

  HTTPClient http;
  if (!iniciarHttp(http, "/api/quartos")) {
    return;
  }

  http.addHeader(HDR_API_KEY, API_KEY);
  int code = http.GET();
  if (code != HTTP_CODE_OK) {
    http.end();
    return;
  }

  String payload = http.getString();
  http.end();

  StaticJsonDocument<2048> doc;
  if (deserializeJson(doc, payload)) {
    return;
  }

  for (JsonObject room : doc["rooms"].as<JsonArray>()) {
    String nome = room["room"].as<String>();
    nome.toLowerCase();
    bool ligado = room["light"].as<String>() == "on";
    int pin = -1;

    if (nome == "cozinha") pin = PIN_LED_COZINHA;
    else if (nome == "quarto") pin = PIN_LED_QUARTO;
    else if (nome == "sala") pin = PIN_LED_SALA;

    if (pin != -1) {
      digitalWrite(pin, ligado ? HIGH : LOW);
    }
  }
}
#endif

void processarChuva() {
  unsigned long agora = millis();
  if (agora - ultimoEnvioChuva < INTERVALO_CHUVA_MS) {
    return;
  }
  ultimoEnvioChuva = agora;

  bool chovendo = lerChuva();
  if (chovendo == ultimaChuva) {
    return;
  }

  if (postChuva(chovendo)) {
    ultimaChuva = chovendo;
  }
}

void sincronizarPortao() {
  unsigned long agora = millis();
  if (agora - ultimaSyncPortao < INTERVALO_PORTAO_MS) {
    return;
  }
  ultimaSyncPortao = agora;

  bool aberto = false;
  if (!getPortao(aberto)) {
    return;
  }

  if (aberto != portaoAberto) {
    moverServoPortao(aberto);
  }
}

void sincronizarJanelas() {
  unsigned long agora = millis();
  if (agora - ultimaSyncJanelas < INTERVALO_JANELAS_MS) {
    return;
  }
  ultimaSyncJanelas = agora;
  sincronizarJanelasDaApi();
}

void sincronizarEstadoInicial() {
  ultimaChuva = lerChuva();
  postChuva(ultimaChuva);

  bool aberto = false;
  if (getPortao(aberto)) {
    moverServoPortao(aberto);
  }

  sincronizarJanelasDaApi();
}

#if ENABLE_RFID
void processarRfid() {
  if (!leitorRfid.PICC_IsNewCardPresent()) {
    return;
  }
  if (!leitorRfid.PICC_ReadCardSerial()) {
    return;
  }

  logLinha("rfid: cartao lido — alternando portao");
  postTogglePortao();

  leitorRfid.PICC_HaltA();
  leitorRfid.PCD_StopCrypto1();
  delay(1000);
}
#endif

void setup() {
  Serial.begin(115200);
  delay(300);

  logLinha("boot Casa Automatica");

  pinMode(PIN_SAIDA_AUX, OUTPUT);
  digitalWrite(PIN_SAIDA_AUX, HIGH);

  pinMode(PIN_CHUVA_POWER, OUTPUT);
  pinMode(PIN_CHUVA_DO, INPUT);

#if ENABLE_LEDS_LUZES
  pinMode(PIN_LED_COZINHA, OUTPUT);
  pinMode(PIN_LED_QUARTO, OUTPUT);
  pinMode(PIN_LED_SALA, OUTPUT);
#endif

  servoPortao.attach(PIN_SERVO_PORTAO);
  moverServoPortao(false);

  for (size_t i = 0; i < QTD_JANELAS; i++) {
    janelas[i].aberta = false;
    janelas[i].motor.attach(JANELAS_CFG[i].pin);
    moverJanela(i, false);
  }

#if ENABLE_RFID
  SPI.begin();
  leitorRfid.PCD_Init();
  logLinha("rfid: leitor pronto");
#endif

  conectarWifi();
  if (wifiConectado()) {
    sincronizarEstadoInicial();
  }

  logLinha("pronto — 1 portao + " + String(QTD_JANELAS) + " janela(s) fisica(s)");
  logLinha("API: " + String(API_BASE_URL));
}

void loop() {
  unsigned long agora = millis();

  if (agora - ultimaVerificacaoWifi >= INTERVALO_WIFI_CHECK_MS) {
    ultimaVerificacaoWifi = agora;
    bool estavaOffline = !wifiConectado();
    conectarWifi();
    if (estavaOffline && wifiConectado()) {
      sincronizarEstadoInicial();
    }
  }

  processarChuva();
  sincronizarPortao();
  sincronizarJanelas();

#if ENABLE_RFID
  processarRfid();
#endif

#if ENABLE_LEDS_LUZES
  sincronizarLuzes();
#endif

  delay(50);
}
