#include <ESP8266WiFi.h>

String apiKey = "OM0SY65RKLH3DNZP";
const char *ssid =  "dit_cu";
const char *pass =  "khongcopass";
const char *server = "api.thingspeak.com";

void setup() 
{
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
 
}

int number = 5;
int counter = 5;
void loop()
{
   WiFiClient client;
  
   if(client.connect(server, 80))
   {
      String postStr = apiKey;
      postStr +="&field1=";
      postStr += String(number);
      
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

      Serial.print("Number: ");
      Serial.print(number);
      Serial.println("%. Send to Thingspeak.");
   }
   client.stop();
   Serial.println("Waiting");
   number = number + 5;
}
