#include <ESP8266WiFi.h>
#include "MAX30100.h"
#include "heartRate.h"
#include <Wire.h>

MAX30100 sensor;

String apiKey = "OM0SY65RKLH3DNZP";
const char *ssid =  "dit_cu";
const char *pass =  "khongcopass";
const char *server = "api.thingspeak.com";

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

void setup() 
{
      Wire.begin();
  
       Serial.begin(115200);
       delay(10);
       //dht.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
       
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");

       while(!Serial);
       sensor.begin(pw1600, i50, sr100 );
 
}

int number = 5;
int counter = 5;
void loop()
{
   sensor.readSensor();
  long irValue = sensor.IR;
  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }
    Serial.print("IR=");
    Serial.print(irValue);
    Serial.print(", BPM=");
    Serial.print(beatsPerMinute);
    Serial.print(", Avg BPM=");
    Serial.print(beatAvg);

    if (irValue < 50000)
      Serial.print(" No finger?");

    Serial.println();
    
   WiFiClient client;
  
   if(client.connect(server, 80))
   {
      String postStr = apiKey;
      postStr +="&field1=";
      postStr += String(irValue);
      
      if(number >= 255 || number <= 0) counter = counter*-1;
      
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);

      Serial.print("irValue: ");
      Serial.print(irValue);
      Serial.println("%. Send to Thingspeak.");
   }
   client.stop();
   Serial.println("Waiting");
}
