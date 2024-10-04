#include "DHT.h"      // Thư viện cảm biến DHT
#include <Wire.h>     // Thư viện cho chuẩn giao tiếp One-Wire

#define DHTPIN 2     // Pin nối với DHT sensor
#define DHTTYPE DHT11 // Định nghĩa loại cảm biến DHT11
DHT dht(DHTPIN, DHTTYPE);

// Định nghĩa chân LED
#define LEDPIN1 16
#define LEDPIN2 17
#define LEDPIN3 25

// Định nghĩa chân của cảm biến siêu âm
#define trigger 12
#define echo 13

float time_go = 0, distance = 0;

void setup() {
  // Khởi động DHT11
  Wire.begin();
  dht.begin();
  
  // Khởi động LED
  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  pinMode(LEDPIN3, OUTPUT);
  
  // Khởi động cảm biến siêu âm
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  
  // Khởi động Serial
  Serial.begin(74880);
  Serial.println("Thí nghiệm môn IoT và ứng dụng...");
  Serial.println("Khởi động DHT11, LED, và cảm biến siêu âm...");
  delay(2000);
}

void loop() {
  // Đọc cảm biến DHT11 (nhiệt độ và độ ẩm)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  // Đo khoảng cách bằng cảm biến siêu âm
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  time_go = pulseIn(echo, HIGH);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  } else {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.println("%");

    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" do-C");

    Serial.print("Temperature: ");
    Serial.print(f);
    Serial.println(" do-F");
  }

 
  distance = time_go * 346.6 * 100 / 1000000 / 2;  // Tính khoảng cách
  Serial.print("Khoang Cach: ");
  Serial.print(distance);
  Serial.println(" cm");

digitalWrite(LEDPIN1, HIGH);
  Serial.println("LED 1 bật");
  digitalWrite(LEDPIN2, HIGH);
  Serial.println("LED 2 bật");
  digitalWrite(LEDPIN3, HIGH);
  Serial.println("LED 3 bật");
  delay(500);

  digitalWrite(LEDPIN2, LOW);
  Serial.println("LED 2 tắt");

  delay(500);
  digitalWrite(LEDPIN1, LOW);
  Serial.println("LED 1 tắt");
  digitalWrite(LEDPIN3, LOW);
  Serial.println("LED 3 tắt");
  digitalWrite(LEDPIN2, HIGH);
  Serial.println("LED 2 bật");

  delay(1000);  // Trễ trước khi lặp lại vòng lặp
}
