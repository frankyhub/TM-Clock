<a name="oben"></a>




<div align="center">

|[:skull:ISSUE](https://github.com/frankyhub/TM-Clock/issues?q=is%3Aissue)|[:speech_balloon: Forum /Discussion](https://github.com/frankyhub/TM-Clock/discussions)|[:grey_question:WiKi](https://github.com/frankyhub/TM-Clock/wiki)||
|--|--|--|--|
| | | | |
|![Static Badge](https://img.shields.io/badge/RepoNr.:-%20115-blue)|<a href="https://github.com/frankyhub/TM-Clock/issues">![GitHub issues](https://img.shields.io/github/issues/frankyhub/TM-Clock)![GitHub closed issues](https://img.shields.io/github/issues-closed/frankyhub/TM-Clock)|<a href="https://github.com/frankyhub/TM-Clock/discussions">![GitHub Discussions](https://img.shields.io/github/discussions/frankyhub/TM-Clock)|<a href="https://github.com/frankyhub/TM-Clock/releases">![GitHub release (with filter)](https://img.shields.io/github/v/release/frankyhub/TM-Clock)|
|![GitHub Created At](https://img.shields.io/github/created-at/frankyhub/TM-Clock)| <a href="https://github.com/frankyhub/TM-Clock/pulse" alt="Activity"><img src="https://img.shields.io/github/commit-activity/m/badges/shields" />| <a href="https://github.com/frankyhub/TM-Clock/graphs/traffic"><img alt="ViewCount" src="https://views.whatilearened.today/views/github/frankyhub/github-clone-count-badge.svg">  |<a href="https://github.com/frankyhub?tab=stars"> ![GitHub User's stars](https://img.shields.io/github/stars/frankyhub)|
</div>



# TM-Clock
Internetuhr mit AP für die WLAN-Daten

## Story
Das Repo beinhaltet eine Uhr mit Internetzeit, Sommer/Winterzeitumstellung. Die Zeitanzeige erfolgt über ein 4-Digit TM1637 Display. 

Für die Einstellung der WLAN-Daten steht ein WLAN-Manager zur Verfügung. Nachdem in den Netzwerkeinstellungen des Handys/Tablet die "TM-Clock" ausgewählt wurde, kann über die IP-Adresse 192.168.4.1 der WLAN-Manger gestartet und die WLAN-Zugangsdaten eingetragen werden. Sind die WLAN-Zugangsdaten korrekt, startet die TM-Clock automatisch. 


![pic](/pic/TM3.png)



## Montage

#### Den ESP32 auf den Gehäusedeckel kleben (Verwende den PIN5x5)
![pic](/pic/TM1.png)

#### Das Display in den Durchbruch kleben (Verwende den PIN-TM)
![pic](/pic/TM2.png)


## Hardware:

| Anzahl | Bezeichnung | 
| -------- | -------- | 
| 1        | ESP32vn IoT UNO         |
| 1        | ESP32 Shield        | 
| 1        | TM1637 Display        | 
| 1        | 5V Netzteil        |
| 1        | Schaltdraht        |
| 1        | 3D-Druck Gehäuse        |

## Verdrahtung:

| ESP32 | TM1637 | 
| -------- | -------- | 
| GPIO 18        | CLK |
| GPIO 5        | DIO  | 
| GND        | GND     | 
| 5V        | 5V       |


## Library

https://github.com/tzapu/WiFiManager



  ## Inbetriebnahme:

  1. Versorge deine TM-Clock mit Strom
  2. Warte bis "TM-Clock" in den WLAN-Verbindungen (Einstellungen, WLAN, Meine Netzwerke)
  3. Verbinde dich mit dem "TM-Clock" WLAN und gebe 192.168.4.1 in der Adresszeile deines Webbrowsers ein (KEINE WEB/GOOGLE SUCHE)
  4. Du solltest jetzt ein Webinterface sehen in dem du den Namen und das Passwort deines Heimnetzwerks eingeben kannst
  5. Wenn du jetzt "Speichern" drückst, sollte sich deine TM-Clock mit deinem Heimnetzwerk verbinden
  6. Die TM-Clock startet jetzt


<div style="position:absolute; left:2cm; ">   
<ol class="breadcrumb" style="border-top: 2px solid black;border-bottom:2px solid black; height: 45px; width: 900px;"> <p align="center"><a href="#oben">nach oben</a></p></ol>
</div> 





