#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>

// Thông tin Wi-Fi (chỉ giữ lại một mạng)
const char* WIFI_SSID = "Ngọ Văn Trọng";
const char* WIFI_PASS = "13082003";

// Khởi tạo WebServer trên cổng 80
WebServer server(80);

// Định nghĩa các độ phân giải khác nhau
static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);

// Hàm chụp và gửi ảnh JPG
void serveJpg() {
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %d\n", frame->getWidth(), frame->getHeight(), static_cast<int>(frame->size()));

  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
}

// Hàm xử lý yêu cầu hình ảnh với độ phân giải thấp
void handleJpgLo() {
  if (!esp32cam::Camera.changeResolution(loRes)) {
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}

// Hàm xử lý yêu cầu hình ảnh với độ phân giải cao
void handleJpgHi() {
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}

// Hàm xử lý yêu cầu hình ảnh với độ phân giải trung bình
void handleJpgMid() {
  if (!esp32cam::Camera.changeResolution(midRes)) {
    Serial.println("SET-MID-RES FAIL");
  }
  serveJpg();
}

// Cấu hình và khởi động server
void setup() {
  Serial.begin(115200);
  Serial.println();

  // Cấu hình camera
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }

  // Kết nối Wi-Fi
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("Camera Stream URL: http://");
  Serial.println(WiFi.localIP());
  Serial.println("  /cam-lo.jpg");
  Serial.println("  /cam-hi.jpg");
  Serial.println("  /cam-mid.jpg");

  // Định tuyến URL
  server.on("/cam-lo.jpg", handleJpgLo);
  server.on("/cam-hi.jpg", handleJpgHi);
  server.on("/cam-mid.jpg", handleJpgMid);

  // Bắt đầu server
  server.begin();
}

// Vòng lặp chính
void loop() {
  server.handleClient();
}
