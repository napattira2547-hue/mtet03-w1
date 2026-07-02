#include "BluetoothSerial.h"

// ตรวจสอบว่าบอร์ดรองรับและเปิดใช้งาน Bluetooth ไว้หรือไม่
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// สร้างตัวแปรสำหรับใช้งาน Bluetooth
BluetoothSerial SerialBT;

void setup() {
  // ตั้งความเร็ว Serial Monitor เหมือนเดิม
  Serial.begin(115200);
  
  // ⚠️ ตั้งชื่อ Bluetooth ของเรา (นายเปลี่ยนชื่อตรงนี้ได้เลย)
  SerialBT.begin("ESP32_BuaIoT"); 
  
  Serial.println("Bluetooth Started! Ready to pair...");
  Serial.println("รอการเชื่อมต่อและพร้อมรับ-ส่งข้อมูลครับ!");
}

void loop() {
  // ส่วนที่ 1: ถ้ารับข้อความมาจากมือถือ (Bluetooth) -> ให้นำมาแสดงบนจอคอมพิวเตอร์
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  
  // ส่วนที่ 2: ถ้าพิมพ์ข้อความจากจอคอมพิวเตอร์ (Serial Monitor) -> ให้ส่งไปที่มือถือ
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
}
