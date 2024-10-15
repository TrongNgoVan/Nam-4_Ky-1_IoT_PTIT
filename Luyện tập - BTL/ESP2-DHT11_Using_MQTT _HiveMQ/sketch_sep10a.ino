#include <WiFi.h>
#include <WiFiClientSecure.h> // Sử dụng SSL
#include <PubSubClient.h>
#include "DHT.h"
#include <ArduinoJson.h> // Thêm thư viện xử lý JSON

#define DHTPIN 4        // Chân tín hiệu với GPIO4 (D4)
#define DHTTYPE DHT11   // Kiểu cảm biến

DHT dht(DHTPIN, DHTTYPE);



// Thông tin Wi-Fi
const char* ssid = "Ngọ Văn Trọng";         // Thay đổi YOUR_SSID với tên mạng Wi-Fi của bạn
const char* password = "13082003"; // Thay đổi YOUR_PASSWORD với mật khẩu mạng Wi-Fi của bạn

// Thông tin MQTT
const char* mqttServer = "14d84e2f560e4c91bc725916ed6511e8.s1.eu.hivemq.cloud"; // Địa chỉ server HiveMQ
const int mqttPort =8883;                     // Cổng SSL
const char* mqttUser = "ngovantrong";  // Client ID của bạn
const char* mqttPassword = "123";                  // Mật khẩu, để trống nếu không cần
    

WiFiClientSecure espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
//------------Connect to MQTT Broker-----------------------------
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientID =  "ESPClient-";
    clientID += String(random(0xffff),HEX);
    if (client.connect(clientID.c_str(),  mqttUser ,mqttPassword)) {
      Serial.println("connected");
      client.subscribe("esp32_dht11_Ngọ Văn Trọng/client");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
//-----Call back Method for Receiving MQTT massage---------
void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for(int i=0; i<length;i++) incommingMessage += (char)payload[i];
  Serial.println("Massage arived ["+String(topic)+"]"+incommingMessage);
}
//-----Method for Publishing MQTT Messages---------
void publishMessage(const char* topic, String payload, boolean retained){
  if(client.publish(topic,payload.c_str(),true))
    Serial.println("Message published ["+String(topic)+"]: "+payload);
}


void setup() {
  Serial.begin(9600);
  while(!Serial) delay(1);

 

  setup_wifi();
  espClient.setInsecure();
  client.setServer(mqttServer,mqttPort );
  client.setCallback(callback);
}
unsigned long timeUpdata=millis();
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //read DHT11
  if(millis()-timeUpdata>5000){
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    DynamicJsonDocument doc(1024);
    doc["humidity"]=h;
    doc["temperature"]=t;
    char mqtt_message[128];
    serializeJson(doc,mqtt_message);
    publishMessage("esp32/ddht11/Ngọ Văn Trọng", mqtt_message, true);

    timeUpdata=millis();
  }
}