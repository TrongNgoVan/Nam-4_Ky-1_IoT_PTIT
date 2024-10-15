// Định nghĩa các thông số liên quan đến dự án Blynk
#define BLYNK_TEMPLATE_ID "TMPL6VXt7MQDH"
#define BLYNK_TEMPLATE_NAME "Control DHT"
#define BLYNK_AUTH_TOKEN "dQ3Obep5wYbyMIcb8zCyaPYKlzZcOJL-"

// Định nghĩa cách bạn muốn ghi log ra Serial Monitor để theo dõi tình trạng hoạt động của chương trình.
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

// Định nghĩa liên quan đến cảm biến DHT22
#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Định nghĩa tên và mật khẩu của mạng Wi-Fi mà ESP32 sẽ kết nối.
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// Định nghĩa một đối tượng BlynkTimer được sử dụng để quản lý thời gian trong chương trình.
BlynkTimer timer;

// Định nghĩa các chân GPIO được sử dụng
#define LED 4

// Hàm callback được gọi khi giá trị nút nhấn trên ứng dụng Blynk thay đổi.
// Nếu giá trị của nút nhấn là 1 (được bật), LED sẽ được bật (HIGH), 
// ngược lại nếu giá trị là 0 (được tắt), LED sẽ được tắt (LOW).
BLYNK_WRITE(V3) {
  int value = param.asInt();
  if (value) {
    Serial.println("Button pressed");
    // Thực hiện hành động khi nút nhấn được bật
    // Ví dụ: bật LED
    digitalWrite(LED, HIGH);
  } else {
    Serial.println("Button released");
    // Thực hiện hành động khi nút nhấn được tắt
    // Ví dụ: tắt LED
    digitalWrite(LED, LOW);
  }
}

// Hàm này đọc dữ liệu từ cảm biến DHT22 và gửi dữ liệu này lên dự án Blynk.
void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Ghi giá trị h lên V1 và t lên V0
  Blynk.virtualWrite(V1, h); // Virtual pin V1 cho độ ẩm
  Blynk.virtualWrite(V0, t); // Virtual pin V0 cho nhiệt độ
}

// Trong hàm setup(), chương trình bắt đầu bằng việc thiết lập kết nối với ứng dụng Blynk,
// khai báo các chân GPIO, khởi động cảm biến DHT22, và thiết lập một bộ định thời (timer)
// để gửi dữ liệu từ cảm biến lên Blynk cứ sau mỗi giây.
void setup() {
  // Debug console
  Serial.begin(115200);
  delay(1000);

  // Kết nối với Wi-Fi và Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Khởi động cảm biến DHT22
  dht.begin();

  // Thiết lập các chân GPIO
  pinMode(LED, OUTPUT);

  // Thiết lập một hàm sẽ được gọi mỗi giây
  timer.setInterval(1000L, sendSensor);
}

// Hàm loop() chứa vòng lặp chính, trong đó Blynk.run() và timer.run() được gọi liên tục
// để duy trì kết nối với Blynk và quản lý thời gian gửi dữ liệu từ cảm biến.
void loop() {
  Blynk.run(); // Chạy Blynk
  timer.run(); // Chạy timer
}
