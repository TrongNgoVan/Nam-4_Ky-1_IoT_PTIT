#define BLYNK_TEMPLATE_ID "TMPL6csRfUsNM"
#define BLYNK_TEMPLATE_NAME "ThinghiemIoT"
#define BLYNK_AUTH_TOKEN "F7Pa3540cE80hGWmMxGCyRF6n2Ww3mKg"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define APP_DEBUG
#define USE_ESP32_DEV_MODULE
int R_EN = 14;
int RPWM = 27;
char ssid[] = "Therainman";
char pass[] = "0123456789";

// Hàm đồng bộ dữ liệu từ Virtual pin V0 sang biến p
//sau đó được ghi lên chân 27 để điều khiển động cơ
BLYNK_WRITE(V0) 
{
int p=param.asInt();
digitalWrite(R_EN,p);
if(p==LOW) 
    
    Serial.println(" Động cơ dừng chạy: Switch OFF");
    else 
    Serial.println("Động cơ đang chạy: Switch ON");
}

void setup() {
Serial.begin(115200);
pinMode(R_EN, OUTPUT);
pinMode(RPWM, OUTPUT);
digitalWrite(RPWM, HIGH);   //chân 27 PWM luôn ở mức cao
WiFi.begin(ssid,pass);
Serial.println("conecting");
while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
Serial.println("");
Serial.print("Connected to WiFi network with IP Address: ");
Serial.println(WiFi.localIP());
Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
Blynk.run();
}
