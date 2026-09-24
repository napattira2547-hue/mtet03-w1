// WiFi Client Demo1 by Min => Auto Send and Command Receive

#include <WiFi.h>

// กำหนดขาที่ใช้งานสำหรับควบคุมมอเตอร์ (เช่น ต่อเข้ากับ Relay)
const int motorPin = 2; // บนบอร์ด ESP32 หลาย ๆ รุ่น ขา 2 จะเป็นไฟ LED บนบอร์ดด้วย

// ** Start WiFi Config Zone
char ssid[] = "Babybua";
char pass[] = "10000000";

const char* host = "10.221.123.170";
const uint16_t port = 2000;
// ** End WiFi Config Zone

unsigned long previousMillis = 0;
long SendTime = 2000; // ตั้งเวลาส่งทุก 2000 มิลลิวินาที (2 วินาที)
WiFiClient client;

void setup() 
{
  Serial.begin(115200);  //Initialize serial

  // ตั้งค่าขาควบคุมมอเตอร์ให้เป็น OUTPUT
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW); // เริ่มต้นให้มอเตอร์ดับไว้ก่อน

  WiFi.mode(WIFI_STA);

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP()); /*Print the Local IP*/
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

  previousMillis = millis();
}

void SendInfo()
{
  // random(1, 11) จะทำการสุ่มตัวเลขตั้งแต่ 1 ถึง 10 (ไม่รวมค่าสูงสุดคือ 11)
  int randomNumber = random(0, 11); 
  
  String Info = String(randomNumber);
  client.println(Info); // ส่งค่าตัวเลขพร้อมขึ้นบรรทัดใหม่ (\r\n) ไปที่ Server
  
  Serial.print("Sent random number: ");
  Serial.println(Info);
}

void loop() 
{
  Serial.print("Connecting to ");
  Serial.println(host);

  if (client.connect(host, port)) {
    Serial.println("Connected to server");

    String message = "Hello from ESP-32 client!";
    client.println(message);
    Serial.print("Sent: ");
    Serial.println(message);

    while (client.connected() || client.available()) 
    {
      unsigned long currentMillis = millis();
      if ((currentMillis - previousMillis) >= SendTime)
      {
        previousMillis = currentMillis;
        SendInfo(); // เรียกฟังก์ชันสุ่มเลขและส่งทุกๆ 2 วินาที
      }

      if (client.available()) 
      {
        // ฟังก์ชันนี้จะดึงข้อความมาโดยตัด '#' ทิ้งไปโดยอัตโนมัติ
        String response = client.readStringUntil('#'); 
        Serial.print("Received from server: ");
        Serial.println(response);
        client.println("ok");

        // เปลี่ยนเงื่อนไขให้เช็คแบบไม่มี '#'
        if (response == "on") {
          Serial.println("Motor ON");
          digitalWrite(motorPin, HIGH); // สั่งเปิดมอเตอร์ (จ่ายไฟ 3.3V)
        }
        else if (response == "off") {
          Serial.println("Motor OFF");
          digitalWrite(motorPin, LOW);  // สั่งปิดมอเตอร์ (0V)
        }
        else {
          Serial.println("Command Error!");
        }
      }
    }

    client.stop();
    Serial.println("Disconnected from server");
  } 
}
