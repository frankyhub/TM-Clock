/*************************************************************************************************
                                      PROGRAMMINFO
**************************************************************************************************
                                     TM-CLOCK 
  Funktion: Uhrzeit über 4 Digit Display, 
 
**************************************************************************************************
  Version: 27.02.2026   Github TM-Clock 
  
  ---------------------------------------------------------------

*************************************************************************************************
  Board: ESP32vn IoT UNO Board Version 1.0.6!
**************************************************************************************************
  C++ Arduino IDE V1.8.19
**************************************************************************************************
  Einstellungen:
  https://dl.espressif.com/dl/package_esp32_index.json
  http://dan.drown.org/stm32duino/package_STM32duino_index.json
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
**************************************************************************************************
  Inbetriebnahme:

  1. Versorge deine TM-Clock mit Strom
  2. Warte bis das "TM-Clock" WLAN erscheint (Einstellungen, WLAN, Meine Netzwerke)
  3. Verbinde dich mit dem "TM-Clock" WLAN und gebe 192.168.4.1 in der Adresszeile deines Webbrowsers ein (KEINE WEB/GOOGLE SUCHE)
  4. Du solltest jetzt ein Webinterface sehen in dem du den Namen und das Passwort deines Heimnetzwerks eingeben kannst
  5. Wenn du jetzt "Speichern" drückst, sollte sich deine Cloudya mit deinem Heimnetzwerk verbinden
  
  ### Electronik Verbindungen
  4-stellige LED 0.56 Display: CLK an D18, DIO an D5, 5V an 5V, GND an GND

 **************************************************************************************************/
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <TM1637Display.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// TM1637 Pins
#define CLK 18
#define DIO 5
TM1637Display display(CLK, DIO);

// Access Point Daten
const char* apName = "TM-Clock";
const char* apPass = "";

// NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600);

// Speicherung
String ssid = "";
String password = "";

// Server
AsyncWebServer server(80);

// Prüfen ob Wifi Daten existieren
bool loadWifiData() {
  if (!SPIFFS.exists("/wifi.txt")) return false;

  File f = SPIFFS.open("/wifi.txt", "r");
  ssid = f.readStringUntil('\n');
  password = f.readStringUntil('\n');
  ssid.trim();
  password.trim();
  f.close();
  return true;
}

// Wifi Daten speichern
void saveWifiData(String s, String p) {
  File f = SPIFFS.open("/wifi.txt", "w");
  f.println(s);
  f.println(p);
  f.close();
}

  // HTML-Formular für die WLAN-Eingabe
  const char *htmlForm = R"(
  <!DOCTYPE html>
  <html lang="de">
  <head>
    <meta charset="UTF-8">
    <title>WLAN Konfiguration</title>
    <style>
      body { font-family: Arial, sans-serif; text-align: center; }
      input { padding: 10px; margin: 10px; font-size: 18px; }
      button { padding: 10px 20px; font-size: 18px; cursor: pointer; }
    </style>
  </head>
  <body>
    <h2>Geben Sie die WLAN-Daten ein</h2>
    <form action="/configure" method="POST">
      <label for="ssid">SSID:</label><br>
      <input type="text" id="ssid" name="ssid" required><br><br>
      <label for="password">Passwort:</label><br>
      <input type="password" id="password" name="password" required><br><br>
      <button type="submit">Speichern und verbinden</button>
    </form>
  </body>
  </html>
  )";

void startAPMode() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apName, apPass);
  Serial.println("AP gestartet: " + WiFi.softAPIP().toString());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", htmlForm);
  });

  server.on("/save", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("ssid") && request->hasParam("pass")) {
      saveWifiData(
        request->getParam("ssid")->value(),
        request->getParam("pass")->value()
      );
      request->send(200, "text/html", "Gespeichert! Neustart...");
      delay(1500);
      ESP.restart();
    }
  });

  server.begin();
}

void setup() {
  Serial.begin(115200);
  SPIFFS.begin(true);
  display.setBrightness(1);

  if (!loadWifiData()) {
    Serial.println("Keine WLAN-Daten -> AP Mode");
    startAPMode();
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.println("Verbinde mit WLAN...");

  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 15000) {
    delay(200);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WLAN fehlgeschlagen -> AP Mode");
    startAPMode();
    return;
  }

  Serial.println("WLAN verbunden: " + WiFi.localIP().toString());

  timeClient.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) return;

  timeClient.update();

  int h = timeClient.getHours();
  int m = timeClient.getMinutes();

  int displayTime = h * 100 + m;
  display.showNumberDecEx(displayTime, 0b01000000, true); // Doppelpunkt blinkt

  delay(500);
}

