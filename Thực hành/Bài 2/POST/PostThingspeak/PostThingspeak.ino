#include "DHT.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <string.h>
const char* ssid ="Therainman";     //"PTIT_LAB DAO TAO";
const char* password ="0123456789";   //"buuchinh@" ;
#define DHTPIN 14     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
 
DHT dht(DHTPIN, DHTTYPE);
String UrlThingspeak = "https://api.thingspeak.com/update?api_key=1C98661JAE1N60BM";
String httpGETRequest(const char* Url);
//************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial.println(F("ESP32-U get data from DHT11 & send to Thingspeak!"));
  dht.begin();
  //***********************setup wifi
  WiFi.begin(ssid,password);
  Serial.println("conecting");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}
//************************************************************* 
void loop() {
  // Wait a few seconds between measurements.
  delay(10000);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.println("%");
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println("ºC");     //ALT+167
  char para[50];
  sprintf(para,"&field1=%2.2f&field2=%2.2f",t,h);
  String Url = UrlThingspeak + String(para);
  Serial.println(Url);
 
  httpGETRequest(Url.c_str());
}
 
String httpGETRequest(const char* Url)
{
  HTTPClient http;
  http.begin(Url);
  int responseCode = http.GET();
  String responseBody = "{}";
  if(responseCode > 0)
  {
    Serial.print("responseCode:");
    Serial.println(responseCode);
    responseBody = http.getString();
  }
  else
  {
    Serial.print("Error Code: ");
    Serial.println(responseCode);
  }
  http.end();
  return responseBody;
}
  
