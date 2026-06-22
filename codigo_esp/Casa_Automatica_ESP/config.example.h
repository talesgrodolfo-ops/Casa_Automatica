#pragma once

// Copie este arquivo para config.h e preencha Wi-Fi + API.

#define WIFI_SSID "SUA_REDE_WIFI"
#define WIFI_PASSWORD "SUA_SENHA_WIFI"

// Producao (Cloudflare Tunnel)
#define API_BASE_URL "https://lar-doce-lar.coresystemstgr.com"
#define API_KEY "local-device-api-key-32-characters-ok"

#define INTERVALO_CHUVA_MS 2000
#define INTERVALO_PORTAO_MS 1000
#define INTERVALO_JANELAS_MS 1000
#define INTERVALO_WIFI_CHECK_MS 15000

// --- Portao (1 servo) — GPIO 12 ---
#define PIN_SERVO_PORTAO 12
#define ANGULO_PORTAO_ABERTO 150
#define ANGULO_PORTAO_FECHADO 80

// --- Janela cozinha — nome igual ao campo "window" na API ---
#define JANELA_1_NOME "cozinha"
#define PIN_SERVO_JANELA_1 32
#define ANGULO_JANELA_1_ABERTO 150
#define ANGULO_JANELA_1_FECHADO 20

// --- Janela quarto ---
#define JANELA_2_NOME "quarto"
#define PIN_SERVO_JANELA_2 33
#define ANGULO_JANELA_2_ABERTO 150
#define ANGULO_JANELA_2_FECHADO 40

// --- Janela sala (opcional — API tem 3 janelas; ative se tiver o 3o servo) ---
#define ENABLE_JANELA_SALA false
#define JANELA_3_NOME "sala"
#define PIN_SERVO_JANELA_3 26
#define ANGULO_JANELA_3_ABERTO 150
#define ANGULO_JANELA_3_FECHADO 0

#define PIN_SAIDA_AUX 23

// Sensor de chuva (POWER + DO)
#define PIN_CHUVA_POWER 19
#define PIN_CHUVA_DO 21

// RFID — SS em GPIO 5 evita conflito com chuva (GPIO 21)
#define ENABLE_RFID true
#define PIN_RFID_SS 5
#define PIN_RFID_RST 22

// LEDs de teste das luzes (cozinha/quarto/sala) — desligado em producao
#define ENABLE_LEDS_LUZES false
#define PIN_LED_COZINHA 2
#define PIN_LED_QUARTO 4
#define PIN_LED_SALA 5
