/*************************************************************************************************
                                      PROGRAMMINFO
**************************************************************************************************
Funktion: TM-Clock, Internetzeit Abruf, WLAN-Manager, WLAN-Daten im SPIFFS gespeichert

**************************************************************************************************
Version: 29.03.2026
**************************************************************************************************
Board: ESP32vn IoT UNO
**************************************************************************************************
Libraries:
WiFiManager (von tzapu) https://github.com/tzapu/wifimanager
TM1637Display (von avishorp)
FS und SPIFFS (standardmäßig in ESP32 Core enthalten)
**************************************************************************************************
C++ Arduino IDE V1.8.19

**************************************************************************************************
Einstellungen:
https://dl.espressif.com/dl/package_esp32_index.json
http://dan.drown.org/stm32duino/package_STM32duino_index.json
http://arduino.esp8266.com/stable/package_esp8266com_index.json
**************************************************************************************************/
#include <Arduino.h>
#include <WiFiManager.h>         // für Access Point Manager https://github.com/tzapu/wifimanager
#include <time.h>                // für NTP und Zeitzonen
#include <TM1637Display.h>       // für TM1637 4-stellige Anzeige
#include <FS.h>
#include <SPIFFS.h>

#define CLK 18   // TM1637 CLK Pin
#define DIO 5    // TM1637 DIO Pin

TM1637Display display(CLK, DIO);

// Zeitzone für Mitteleuropa mit Sommer-/Winterzeit-Umstellung
#define TZ_EU "CET-1CEST,M3.5.0/02,M10.5.0/03"

const char* ntpServer = "pool.ntp.org";
bool timeInitialized = false;

void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS konnte nicht initialisiert werden!");
  } else {
    Serial.println("SPIFFS gestartet.");
  }
}

void connectWiFi() {
  WiFiManager wm;

  if (!wm.autoConnect("TM-Cock")) {
    Serial.println("Keine Verbindung möglich, starte neu...");
    delay(3000);
    ESP.restart();
  }

  Serial.print("Verbunden mit: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());
}

void setupTime() {
  configTzTime(TZ_EU, ntpServer);  // Zeitzone + NTP konfigurieren
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Fehler: Keine Zeitdaten verfügbar!");
  } else {
    Serial.println("Zeit erfolgreich synchronisiert.");
    timeInitialized = true;
  }
}

void displayTime() {
  if (!timeInitialized) return;

  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    int hours = timeinfo.tm_hour;
    int minutes = timeinfo.tm_min;

    // Zeit als hh:mm anzeigen
    int displayTime = hours * 100 + minutes;
    display.showNumberDecEx(displayTime, 0b01000000, true); // Doppelpunkt an

  } else {
    Serial.println("Zeit kann nicht gelesen werden!");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  initSPIFFS();
  connectWiFi();
  setupTime();

  display.setBrightness(0); // Helligkeit
  Serial.println("ESP32 Internetuhr gestartet.");
}

void loop() {
  displayTime();
  delay(1000);
}

