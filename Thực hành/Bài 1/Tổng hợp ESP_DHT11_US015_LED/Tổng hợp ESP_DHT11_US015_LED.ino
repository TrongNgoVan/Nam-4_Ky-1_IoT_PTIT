#define LEDPIN1 16  // Pin nối với đèn LED (IO13)
#define LEDPIN2 17
#define LEDPIN3 25

void setup() {
  pinMode(LEDPIN1, OUTPUT);
     
  pinMode(LEDPIN2, OUTPUT);
  pinMode(LEDPIN3, OUTPUT);
  Serial.begin(115200);      // Đặt tốc độ truyền cổng serial
  Serial.println("Thí nghiệm môn IoT và ứng dụng.....");
  Serial.println("LED hiển thị.....");
  delay(1000);               // Trễ để hiển thị thông báo ban đầu
}

void loop() {
  digitalWrite(LEDPIN1, HIGH);  // Bật đèn LED
  Serial.println("LED 1 bật");
  digitalWrite(LEDPIN2, HIGH);  // Bật đèn LED
  Serial.println("LED 2 bật");
  digitalWrite(LEDPIN3, HIGH);  // Bật đèn LED
  Serial.println("LED 3 bật");
  delay(1000);                 // Chờ 1 giây

   
}
