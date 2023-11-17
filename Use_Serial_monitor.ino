/*Example read data from serial monitor
char buffer[4]= {}; // Biến đệm lưu giá trị
void setup() {
        Serial.begin(9600);     // mở serial với baudrate 9600
}

void loop() {

        // nếu còn có thể đọc được 
        if (Serial.available() >= 3) {
                // đọc chữ liệu
                Serial.readBytes(buffer,3);
                //Xuất dữ liệu
                Serial.print("Toi nhan duoc: ");
                Serial.println(buffer);
        }
} 
*/
/*
 * This code use serial monitor to control MCU 
 * Data may be char or string 
 * 
 */
void setup()
{
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW); // lúc đầu ta để LED tắt
}
void loop()
{
  if(Serial.available()) { // nếu có dữ liệu gửi đến
   String text = Serial.readStringUntil('\n'); // đọc giá trị gửi đến cho đến khi gặp kí tự xuống dòng \n
    
    if(text == "ON") {
        digitalWrite(8,HIGH); // bật LED
        Serial.println("LED is turned ON");  // gửi lại thông báo lên Serial Monitor
    }
    
    if(text == "OFF") {
        digitalWrite(8,LOW); // tắt LED
        Serial.println("LED is turned OFF");
    }
   }
 }
