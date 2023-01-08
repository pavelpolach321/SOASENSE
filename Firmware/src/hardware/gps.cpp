/*
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include "hardware.h"
#include "gps.h"

*/
/*
   This sample sketch demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
/*
static const int RXPin = GPS_RX_PIN, TXPin = GPS_TX_PIN;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

int gpsInit()
{
  //Serial.begin(115200);
  ss.begin(GPSBaud, SWSERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN, false, 512);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPSPlus with an attached GPS module"));
  Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
  return 0;
}

void gpsUpdate(node_object_t *handle)
{

Serial.println(F("3s to enable gps"));
Serial.println(F("......."));
  delay(3000);
  gpio_pulldown_en(ENABLE_GPS_PIN);
  gpio_set_direction(ENABLE_GPS_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level(ENABLE_GPS_PIN, 1);
  delay(1000);
Serial.println(F("gps enabled"));
Serial.println(F("......."));
  

  */


  // This sketch displays information every time a new sentence is correctly encoded.
/*
https://forum.arduino.cc/t/gps-module-power-management/477859/15

The RXM-PMREQ message puts the GPS in the inactive state for the period specified and is supported on Ublox6, 7, 8;

If you send it with a zero period as below the GPS goes inactive, but retains settings, so you can hot-fix it.

0xB5, 0x62, 0x02, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x4D, 0x3B

This is the measured power consumption in inactive mode;

Ubloc Neo 6M - 75uA
Ublox Max 8Q - 35uA

These are the bare GPS, no extra stuff connected.

Did not have a bare bones 7 to hand to test.
*/
//char buf[] = {0xB5, 0x62, 0x02, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x4D, 0x3B};

  
  //ss.write(buf);
  /*
  while (ss.available() == 0)
    Serial.print(".");
  while (ss.available() > 0)
    if (gps.encode(ss.read())){
      Serial.println("ajajaja");
      displayInfo();
    }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  Serial.print(" Satelites ");
  if (gps.satellites.isValid())
  {
    Serial.println(gps.satellites.value());
  }
  else {
    Serial.print(F("INVALID"));
  }
  Serial.println();

    
  gpio_pulldown_en(ENABLE_GPS_PIN);
  gpio_set_direction(ENABLE_GPS_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level(ENABLE_GPS_PIN, 0);
  Serial.println(F("gps disabled"));
//Serial.println(F("......."));
  delay(3000);

}
*/
