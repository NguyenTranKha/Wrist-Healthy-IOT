/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "TinyGPS++.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);
const char *ssid =  "Hackathon 2017";     // replace with your wifi ssid and wpa2 key
const char *pass =  "www.eiu.edu.vn";
const char *server = "54.201.74.103";
const char *url = "/esppost.php";
static const int RXPin = D4, TXPin = D3;
static const uint32_t GPSBaud = 9600;
char ln[50];
char lt[50];
float bPm = 0;
float SpO2 = 0;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

#define REPORTING_PERIOD_MS     1000

uint32_t tsLastReport = 0;


void setup()
{
  set();
       Serial.begin(115200);
       delay(10);
       u8g2.begin();
       delay(10);
       WiFi.begin(ssid, pass);
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            display_CONNECTION();
     }
      display_CONNECTED();
      delay(1000);
      ss.begin(GPSBaud);
            
}

void loop()
{
    //code calculator heart rate, SpO2, GPS;


    Sensor_Update();
    bPm = get_HR();
    SpO2 = get_spO();
    dtostrf(gps.location.lat(),9,6,lt);
    dtostrf(gps.location.lng(),11,7,ln);
    
    
   
    
    //updata server;
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect("54.201.74.103", httpPort)) {
    return;
  }
  delay(1000);


//display oled;
 if(bPm < 50 || bPm >95) display_WARNING();
 if(bPm < 30 || bPm >100) display_DANGER();
 if(bPm > 60 && bPm < 90) display_result(bPm, SpO2);

 String data = "ID=HK1&temperature=";
 data+="37";
 data+="&SPO2=";
 data+=(String)SpO2;
 data+="&BEAT=";
 data+=(String)bPm;
 data+="&LONG=";
 data+=(String)ln;
 data+="&LAT=";
 data+=(String)lt;

   client.println("POST /POST_DATA.php HTTP/1.1");
   client.println("Host: 54.201.74.103");
   client.println("Content-Type: application/x-www-form-urlencoded");
   client.print("Content-Length: ");
   client.println(data.length());
   client.println();
   client.print(data);

       delay(500); // Can be changed
      while (client.connected())
    {
      if ( client.available() )
      {
        char str=client.read();
       Serial.print(str);
      }      
    }
        smartDelay(1000);
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

void display_WARNING()
{
    u8g2.clearBuffer();
    u8g2.setCursor(7,20);
    u8g2.setFont(u8g2_font_px437wyse700a_tf);
    u8g2.print("WARNING");  
    u8g2.sendBuffer(); 
}

void display_DANGER()
{
    u8g2.clearBuffer();
    u8g2.setCursor(14,20);
    u8g2.setFont(u8g2_font_px437wyse700a_tf);
    u8g2.print("DANGER");  
    u8g2.sendBuffer(); 
}

void display_result(int bPm, int SpO2)
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_smart_patrol_nbp_tf);
  u8g2.setCursor(5,12);  
  u8g2.print(bPm);
  u8g2.print(" Bpm");
  u8g2.setCursor(0,30);
  u8g2.print("SpO2 ");
  u8g2.setCursor(65,30);  
  u8g2.print(SpO2);
  u8g2.print("%"); 
  u8g2.sendBuffer();
}

void display_CONNECTION()
{
    u8g2.clearBuffer();
    u8g2.setCursor(0,12);
    u8g2.setFont(u8g2_font_smart_patrol_nbp_tf);
    u8g2.print("CONNECTION...");  
    u8g2.sendBuffer();
}

void display_DISCONNECTED()
{
    u8g2.clearBuffer();
    u8g2.setCursor(0,12);
    u8g2.setFont(u8g2_font_smart_patrol_nbp_tf);
    u8g2.print("DISCONNECTED");  
    u8g2.sendBuffer(); 
}

void display_CONNECTED()
{
    u8g2.clearBuffer();
    u8g2.setCursor(0,12);
    u8g2.setFont(u8g2_font_smart_patrol_nbp_tf);
    u8g2.print("CONNECTED");  
    u8g2.sendBuffer(); 
}
