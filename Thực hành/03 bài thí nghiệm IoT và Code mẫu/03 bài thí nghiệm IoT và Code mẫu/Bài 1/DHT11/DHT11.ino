#include "DHT.h"    // Thư viện cảm biến
#include <Wire.h>   // Thư viện cho chuẩn giao tiếp One-Wire 
#define DHTPIN 4    // Digital pin (IO4) nối với DHT sensor 
#define DHTTYPE DHT11             // DHT 11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Wire.begin();                   //khởi động One-Wire
  dht.begin();                    //khởi động DHT11
  Serial.begin(115200);           //đặt tốc độ truyền cổng serial
                                  //serial=115200 baud
  Serial.println("Thí nghiệm môn IoT và ứng dụng.....");
  Serial.println("DHT11 reading.....");
  delay(2000);
//********************************************************
// Hiển thị thông tin ban đầu
//********************************************************
   }
void loop() {
  float h = dht.readHumidity();           // đọc độ ẩm
  float t = dht.readTemperature();        // đọc nhiệt độ dưới dạng độ C (Celsius) - mặc định
  float f = dht.readTemperature(true);    // đọc nhiệt độ dưới dạng độ F (Fahrenheit)                          
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!")); // Thông báo nếu lỗi đọc dữ liệu xảy ra
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("%"); // in thông báo độ ẩm trên terminal
  
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" do-C  ");     // in thông báo nhiệt độ trên terminal dưới dạng độ C
  Serial.print("Temperature: ");
  Serial.print(f);
  Serial.println(" do-F");       //  in thông báo nhiệt độ trên terminal dưới dạng độ F 
  delay(2000);                   // trễ để đọc thông báo
  }
