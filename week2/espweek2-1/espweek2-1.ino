#include <WiFi.h>
#include <WebServer.h>

// ⚠️ ใส่ชื่อ Wi-Fi และรหัสผ่านของนายตรงนี้
const char* ssid = "Babybua";
const char* password = "10000000";

// สร้างออบเจกต์ WebServer ที่พอร์ต 80 (พอร์ตมาตรฐานของเว็บ)
WebServer server(80);

// กำหนดขา Pin 1 (ตามที่อาจารย์สั่ง)
const int outputPin = 1; 

void setup() {
  // เริ่มต้น Serial Monitor (ความเร็ว 115200) เพื่อดู IP Address
  Serial.begin(115200);
  
  // ตั้งค่าให้ Pin 1 เป็น Output และสั่งปิดไว้ก่อน
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, LOW); 

  // --- เริ่มกระบวนการเชื่อมต่อ Wi-Fi ---
  Serial.println();
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  // รอจนกว่าจะเชื่อมต่อสำเร็จ
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // เชื่อมต่อสำเร็จ! พิมพ์ IP Address ออกมาทางหน้าจอ
  Serial.println();
  Serial.print("Connected! Your ESP32 IP Address is: ");
  Serial.println(WiFi.localIP()); 
  // 📌 เอาเลข IP ตรงนี้แหละ ไปใส่ในโค้ด HTML บรรทัดที่ 40!

  // --- สร้างเส้นทางรอรับคำสั่งจากหน้าเว็บ HTML ---

  // 1. ถ้าหน้าเว็บส่งคำสั่งมาที่ /pin1/on
  server.on("/pin1/on", []() {
    digitalWrite(outputPin, LOW);                 // เปิดอุปกรณ์ (ไฟติด)
    server.send(200, "text/plain", "Pin 1 is ON"); // ส่งข้อความตอบกลับไปหาเว็บ
    Serial.println("Command Received: TURN ON");
  });

  // 2. ถ้าหน้าเว็บส่งคำสั่งมาที่ /pin1/off
  server.on("/pin1/off", []() {
    digitalWrite(outputPin, HIGH);                  // ปิดอุปกรณ์ (ไฟดับ)
    server.send(200, "text/plain", "Pin 1 is OFF");
    Serial.println("Command Received: TURN OFF");
  });

  // 3. จัดการกรณีพิมพ์ URL ผิด
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not Found");
  });

  // สั่งให้ Server เริ่มทำงาน
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // สั่งให้ ESP32 คอยฟังคำสั่งจากผู้ใช้ตลอดเวลา
  server.handleClient();
}