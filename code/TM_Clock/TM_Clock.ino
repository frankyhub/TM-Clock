/*************************************************************************************************
                                      PROGRAMMINFO
**************************************************************************************************
Funktion: TM-Clock, Internetzeit Abruf, WLAN-Manager, WLAN-Daten im SPIFFS gespeichert

**************************************************************************************************
Version: 21.03.2026
**************************************************************************************************
Board: ESP32vn IoT UNO
**************************************************************************************************
Libraries:
https://github.com/espressif/arduino-esp32/tree/master/libraries
C:\Users\User\Documents\Arduino
D:\gittemp\Arduino II\A156_Wetterdaten_V3
**************************************************************************************************
C++ Arduino IDE V1.8.19

**************************************************************************************************
Einstellungen:
https://dl.espressif.com/dl/package_esp32_index.json
http://dan.drown.org/stm32duino/package_STM32duino_index.json
http://arduino.esp8266.com/stable/package_esp8266com_index.json
**************************************************************************************************/
#include <WiFi.h>
#include <WebServer.h>
#include <TM1637Display.h>
#include <time.h>
#include <FS.h>
#include <SPIFFS.h>

#define CLK 18
#define DIO 5
TM1637Display display(CLK, DIO);

WebServer server(80);

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;     // 3600 z. B. MEZ = +1 h
const int daylightOffset_sec = 3600; // Sommerzeit

String ssid;
String password;

// ----------------------------------------------------------------
// Hilfsfunktionen für SPIFFS
// ----------------------------------------------------------------
void saveCredentials(const String& ssid, const String& password) {
  File file = SPIFFS.open("/wifi.txt", FILE_WRITE);
  if(!file) {
    Serial.println("Fehler beim Speichern der WLAN Daten!");
    return;
  }
  file.println(ssid);
  file.println(password);
  file.close();
}

bool loadCredentials(String& ssid, String& password) {
  File file = SPIFFS.open("/wifi.txt");
  if(!file) {
    Serial.println("Keine WLAN Daten vorhanden.");
    return false;
  }
  ssid = file.readStringUntil('\n');
  password = file.readStringUntil('\n');
  ssid.trim();
  password.trim();
  file.close();
  return (ssid.length() > 0);
}

// ----------------------------------------------------------------
// Webserver zum Einstellen der WLAN-Daten
// ----------------------------------------------------------------
void handleRoot() {
  String html = R"rawliteral(
    <html>
    <body style='font-family:sans-serif;text-align:center;'>
    <h2>ESP32 WLAN Konfiguration</h2>
    <form action="/save" method="post">
      SSID:<br><input name="ssid"><br>
      Passwort:<br><input name="pass" type="password"><br><br>
      <input type="submit" value="Speichern & Neustarten">
    </form>
    </body>
    </html>
  )rawliteral";
  server.send(200, "text/html", html);
}

void handleSave() {
  ssid = server.arg("ssid");
  password = server.arg("pass");
  saveCredentials(ssid, password);
  server.send(200, "text/html", "<h3>Gespeichert! Neustart...</h3>");
  delay(2000);
  ESP.restart();
}

// ----------------------------------------------------------------
// WLAN-Verbindung aufbauen oder Webserver starten
// ----------------------------------------------------------------
void connectWiFi() {
  if (!loadCredentials(ssid, password)) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("TM-Clock");
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP gestartet. IP: ");
    Serial.println(IP);
    server.on("/", handleRoot);
    server.on("/save", HTTP_POST, handleSave);
    server.begin();
    while (true) {
      server.handleClient();
      delay(10);
    }
  } else {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.printf("Verbinde mit %s", ssid.c_str());
    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 20) {
      delay(1000);
      Serial.print(".");
      retry++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWLAN verbunden!");
    } else {
      Serial.println("\nKeine Verbindung. Öffne Setup-Modus...");
      SPIFFS.remove("/wifi.txt");
      ESP.restart();
    }
  }
}

// ----------------------------------------------------------------
// NTP Synchronisierung
// ----------------------------------------------------------------
void initTime() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Zeit wird synchronisiert...");
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Zeit konnte nicht abgerufen werden.");
    return;
  }
  Serial.println(&timeinfo, "Aktuelle Zeit: %H:%M:%S");
}

// ----------------------------------------------------------------
// Arduino Setup / Loop
// ----------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  if(!SPIFFS.begin(true)) {
    Serial.println("SPIFFS konnte nicht gestartet werden!");
    return;
  }

  display.setBrightness(0);
  display.clear();

  connectWiFi();
  initTime();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) return;
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    int hour = timeinfo.tm_hour;
    int minute = timeinfo.tm_min;
    int displayTime = hour * 100 + minute;
    display.showNumberDecEx(displayTime, 0b01000000, true); // ':' aktivieren
  }
  delay(1000);
}

