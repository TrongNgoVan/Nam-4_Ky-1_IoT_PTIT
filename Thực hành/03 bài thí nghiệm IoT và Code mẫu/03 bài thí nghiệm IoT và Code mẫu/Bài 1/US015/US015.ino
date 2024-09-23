#define trigger 2               // chân trig cua US-015
#define echo 4                  // chân echo cua US-015
float time_go=0,distance=0;
void setup()
{
pinMode(trigger,OUTPUT);
pinMode(echo,INPUT);
Serial.begin(115200);           // Khoi tao cong noi tiep
}
void loop()
{
digitalWrite(trigger,LOW);
delayMicroseconds(2);
digitalWrite(trigger,HIGH);    // chân trigger lên mức cao
delayMicroseconds(10);         // trễ 10µs
digitalWrite(trigger,LOW);     // chân trigger xuống thấp
delayMicroseconds(2);
time_go=pulseIn(echo,HIGH);  
//hàm tính độ rộng xung thu chân ECHO theo thời gian
distance=time_go*346.5*100/1000000/2;  
// vận tốc sóng siêu âm cỡ ~344 m/s
  Serial.print("Khoang Cach:  ");
  Serial.print(distance);
  Serial.println("cm");
 delay(1000);
}
