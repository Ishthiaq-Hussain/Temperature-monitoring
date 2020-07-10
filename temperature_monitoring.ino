#include <BoltDeviceCredentials.h>
#include <BoltIoT-Arduino-Helper.h>


#include <DHTesp.h>
#include <DHT.h>  // Including library for dht
#define DHTTYPE DHT11
#include <ESP8266WiFi.h>
                         
 
String apiKey = "your api key";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "your ssid";     // replace with your wifi ssid and wpa2 key
const char *pass =  "password";
const char* server = "api.thingspeak.com";

#define DHTPIN 4          //pin where the dht11 is connected
 
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;
 
void setup()
{
       dht.begin();
     
       Serial.println("Humidity and temperature\n\n");
       delay(700);  

       Serial.begin(115200);
       delay(100);
       dht.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
       
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED)
     {
            delay(100);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 
}
 
void loop()
{
 
      float h = dht.readHumidity();
      float t = dht.readTemperature();
     
              if (isnan(h) || isnan(t))
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }

                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                           
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
                                 
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius");
                             
                             Serial.println(". Send to Thingspeak.");
                        }
          client.stop();

  Serial.println("Waiting...");
  delay(100);
}
