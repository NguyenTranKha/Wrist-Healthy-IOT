#include <U8g2lib.h>
#include <Wire.h>
#include "ESP8266WiFi.h"

#define REPORTING_PERIOD_MS     500
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);
bool initialized=false;
void display_WARNING()
{
  if (not initialized) 
  {
    u8g2.clearBuffer();
    u8g2.setCursor(7,20);
    u8g2.setFont(u8g2_font_px437wyse700a_tf);
    u8g2.print("WARNING");  
    u8g2.sendBuffer(); 
    initialized=true;
  }
}

void display_DANGER()
{
    u8g2.clearBuffer();
    u8g2.setCursor(14,20);
    u8g2.setFont(u8g2_font_px437wyse700a_tf);
    u8g2.print("DANGER");  
    u8g2.sendBuffer(); 
    initialized=true;
}

void display_result()
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_smart_patrol_nbp_tf);
  u8g2.setCursor(5,12);  
  u8g2.print("a");
  u8g2.print(" Bpm");
  u8g2.setCursor(0,30);
  u8g2.print("SpO2 ");
  u8g2.setCursor(65,30);  
  u8g2.print("b");
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

void setup()
{
    Serial.begin(115200);
    u8g2.begin();
}

void loop()
{
  display_CONNECTION();
  delay(1000);
  display_CONNECTED();
  delay(1000);
}
