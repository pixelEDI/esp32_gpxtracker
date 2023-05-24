/*
        _          _ ______ _____ _____
       (_)        | |  ____|  __ \_   _|
  _ __  ___  _____| | |__  | |  | || |
 | '_ \| \ \/ / _ \ |  __| | |  | || |
 | |_) | |>  <  __/ | |____| |__| || |_
 | .__/|_/_/\_\___|_|______|_____/_____|
 | |
 |_|

www.pixeledi.eu | https://linktr.ee/pixeledi
ESP32 GPS Tracker
RYS8833 GPS Modul | V1.0 | 05.2023

*/

#include <Arduino.h>
#include <TinyGPS++.h>
#include <SD.h>
#include <SPI.h>
#include "button.h"
#include "gpx.h"
#include "oled.h"

TinyGPSPlus gps;
HardwareSerial ss(2); // RX TX

File gpxFile;
Button btn(4);
myoled oled;

void initGNSS()
{
  ss.begin(115200);
  delay(200);
  ss.print("@GSTP\r\n"); // positioning stop
  delay(200);
  ss.print("@GNS 01\r\n"); // Positioning-use satellite use GPS + GLONASS + SBAS + QZSS L1-CA + QZSS L1-S
  delay(200);
  // ss.print("@GSR\r\n"); // hot start
  ss.print("@GCD\r\n"); // cold start

  delay(200);
}

void startGPXTrack()
{
  gpxFile = SD.open("/reyax.gpx", FILE_WRITE);
  gpxFile.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
  gpxFile.println("<gpx version=\"1.1\" xmlns=\"http://www.topografix.com/GPX/1/1\">");
  gpxFile.println("<trk><name>pixelEDI Route</name><trkseg>");
  gpxFile.close();
}

void endGPXTrack()
{
  gpxFile = SD.open("/reyax.gpx", FILE_WRITE);
  gpxFile.println("</trkseg></trk></gpx>");
  gpxFile.close();
}

void checkButton()
{

  // one button 3 features
  // depends on how long you click
  
  switch (btn.checkBTN())
  {
  case 1:
    //short press you can add here display on / off
    break;
  case 2:
    startGPXTrack();
    Serial.println("GPX Started");
    oled.clrDisplay();
    oled.setText("GPX Started", 20, 10);
    break;

  case 3:
    oled.clrDisplay();
    endGPXTrack();
    oled.setText("GPX saved", 20, 10);
    break;
  }
}

void setup()
{
  Serial.begin(115200); // Initialize Serial port for debugging
  initGNSS();
  btn.start();
  oled.starten();
  if (!SD.begin())
  {
    Serial.println("Card Mount Failed");
    return;
  }
  Serial.print("Setup finished");
  // oled.oledDisplayCenter("Setup finished");
}

void loop()
{

  checkButton();

  while (ss.available() > 0)
  {

    // check the raw output
    // int incomingByte = ss.read();
    // Serial.print(char(incomingByte));
    
    if (gps.encode(ss.read()))
    {

      if (gps.location.isValid())
      {
        String lat = String(gps.location.lat());
        String lng = String(gps.location.lng());

        Serial.print("Latitude: ");
        Serial.print(gps.location.lat(), 6);
        Serial.print(", Longitude: ");
        Serial.println(gps.location.lng(), 6);

        oled.clrDisplay();
        oled.setText(lat, 40, 40);
        oled.setText(lng, 40, 60);

        gpxFile = SD.open("/reyax.gpx", FILE_APPEND);
        gpxFile.print("<trkpt lat=\"");
        gpxFile.print(gps.location.lat(), 6);
        gpxFile.print("\" lon=\"");
        gpxFile.print(gps.location.lng(), 6);
        gpxFile.print("\">");
        gpxFile.print("</trkpt>");

        gpxFile.close();
        delay(2000);
      }
      else
      {
        Serial.println("No GPS data available");
        oled.setText("No GPS data available", 10, 40);
      }
    }
  }
}
