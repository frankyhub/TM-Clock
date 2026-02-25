/*************************************************************************************************
                                      PROGRAMMINFO
**************************************************************************************************
Funktion:Internet Uhr mit TM1637Display, AP für die Eingabe der WLAN Daten  

**************************************************************************************************
Version: 25.02.2026
**************************************************************************************************
Board: ESP32vn IoT UNO Version 1.0.6
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
  #include <TM1637Display.h>
  #include <ESPAsyncWebServer.h>
  
  const char *ssid = "ssid";
  const char *password = "password";
  
  // TM1637 Pin Definitionen
  #define CLK_PIN 18  // Clock Pin
  #define DIO_PIN 5  // Data Pin
  
  //TM1637 tm1637(CLK_PIN, DIO_PIN);
  TM1637Display display = TM1637Display(CLK_PIN, DIO_PIN);
  
  #include <NTPClient.h>
  #include <WiFiUdp.h>
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP);
  
  // Variablen
  String formattedDate;
  String dayStamp;
  String timeStamp;
  
  
  // Internetzeit
  #define MY_NTP_SERVER "de.pool.ntp.org"
  #define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03" // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
  time_t now;
  tm tm;
  
  void showTime() {
    time(&now);
    localtime_r(&now, &tm); // update the structure tm with the current time
    /* Serial.print("Jahr:");
      Serial.print(tm.tm_year + 1900); // years since 1900
      Serial.print("\tmonth:");
      Serial.print(tm.tm_mon + 1); // January = 0 (!)
      Serial.print("\tday:");
      Serial.print(tm.tm_mday); // day of month
      Serial.print("\thour:");
      Serial.print(tm.tm_hour); // hours since midnight 0-23
      Serial.print("\tmin:");
      Serial.print(tm.tm_min); // minutes after the hour 0-59
      Serial.print("\tsec:");
      Serial.print(tm.tm_sec); // seconds after the minute 0-61*
      Serial.print("\twday");
      Serial.print(tm.tm_wday); // days since Sunday 0-6
    */
    Serial.println();
  }
  
  // Webserver Setup
  AsyncWebServer server(80);
  
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
  
  // Setup WLAN-Verbindung
  void setupWiFi() {
    Serial.println("Starte AP für WLAN-Konfiguration...");
    WiFi.softAP("TM-Clock", "");  // Erstelle einen Access Point für die WLAN-Konfiguration
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP: ");
    Serial.println(IP);
  
    // Webserver starten
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/html", htmlForm);
    });
  
    server.on("/configure", HTTP_POST, [](AsyncWebServerRequest *request){
      if (request->hasParam("ssid", true) && request->hasParam("password", true)) {
        ssid = request->getParam("ssid", true)->value().c_str();
        password = request->getParam("password", true)->value().c_str();
        Serial.print("SSID: ");
        Serial.println(ssid);
        Serial.print("Passwort: ");
        Serial.println(password);
        
        // Versuche mit den eingegebenen Daten zu verbinden
        WiFi.begin(ssid, password);
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 10) {
          delay(500);
          Serial.print(".");
          attempts++;
        }
        if (WiFi.status() == WL_CONNECTED) {
          request->send(200, "text/html", "<h2>Erfolgreich verbunden! Starte die Uhr...</h2>");
        } else {
          request->send(200, "text/html", "<h2>Fehler beim Verbinden. Versuche es nochmal!</h2>");
        }
      }
    });
  
    server.begin();
  }
  
  void setup() {
    Serial.begin(115200);
   
   // setup display
    display.setBrightness(1); // Helligkeit auf 1 setzen (0-7)!!!!!
    display.clear();          // Clear the display
  
    setupWiFi();  // WLAN Setup und Webserver starten
    timeClient.begin();  // NTP-Client starten
  
    // NTPClient Zeit
    timeClient.begin();
    timeClient.setTimeOffset(3600);
  }
  
  void loop() {
    // Solange wir noch keine WLAN-Verbindung haben, warten wir
    if (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      return;
    }
  
      //----------------Clock----------------------
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    showTime();
    //  Serial.print("Epoch Time: ");
    //  Serial.println(epochTime);
  
    String formattedTime = timeClient.getFormattedTime();
    // Serial.print("Formatted Time: ");
    // Serial.println(formattedTime);
  
    int currentHour = timeClient.getHours();
    char stunden = timeClient.getHours();
  
  
     Serial.print("Hour: ");
     Serial.println(currentHour);
  
    int currentMinute = timeClient.getMinutes();
    Serial.print("Minutes: ");
    Serial.println(currentMinute);
  
    //int currentSecond = timeClient.getSeconds();
    //  Serial.print("Seconds: ");
    //  Serial.println(currentSecond);
  
    //String weekDay = weekDays[timeClient.getDay()];
    //char tag = timeClient.getDay();
    //  Serial.println("**************** Datum und Uhrzeit ********************");
    //  Serial.println(tag);
  
    //  Serial.print("Wochentag: ");
    //  Serial.println(weekDay);
  
    //Get a time structure
    struct tm *ptm = gmtime ((time_t *)&epochTime);
  
    //int monthDay = ptm->tm_mday;
    //  Serial.print("Tag: ");
    //  Serial.println(monthDay);
  
    //int currentMonth = ptm->tm_mon + 1;
    //  Serial.print("Monat: ");
    //  Serial.println(currentMonth);
  
    //String currentMonthName = months[currentMonth - 1];
    //  Serial.print("Monat: ");
    //  Serial.println(currentMonthName);
  
    //int currentYear = ptm->tm_year + 1900;
    //  Serial.print("Jahr: ");
    //  Serial.println(currentYear);
  
    //Print complete date:
    //String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
    //  Serial.print("Datum: ");
    //  Serial.println(currentDate);
  
    //  Serial.println("");
  
    //---------------------------------------------------------
  
    while (!timeClient.update()) {
      timeClient.forceUpdate();
    }
    // Eingehendes Format
    // 2018-05-28T16:00:13Z
    // Extrahiere Zeit und Datum
    //formattedDate = timeClient.getFormattedTime();
    //Serial.println(formattedTime);
  
    // Extrahiere Datum
    int splitT = formattedDate.indexOf("T");
    dayStamp = formattedDate.substring(0, splitT);
    //  Serial.print("Datum: ");
    //  Serial.println(dayStamp);
    // Extrahiere Zeit
    timeStamp = formattedDate.substring(splitT + 1, formattedDate.length() - 1);
      Serial.print("Uhrzeit: ");
      Serial.print(currentHour);
      Serial.print(":");
      Serial.println(currentMinute);

      int displayTime = currentHour * 100 + currentMinute;
      display.showNumberDecEx(displayTime, 0b01000000, true);

    
    delay(1000);
  }
