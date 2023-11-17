
  char check_ID[3];
  char Led_ID[3];
  char status_led[3];
void setup()
{
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW); // lúc đầu ta để LED tắt
}

void loop()
{
    if(Serial.available()) { // nếu có dữ liệu gửi đến
    String data = Serial.readStringUntil('\n'); // đọc giá trị gửi đến cho đến khi gặp kí tự xuống dòng \n
    String data_id = data.substring(0,2);
    String data_id_device = data.substring(2,4);
    Serial.print("data_id: ");
    Serial.println(data_id);

      Serial.print("data_id_device: ");
    Serial.println(data_id_device);
    
//    if(text == "0000") {
//        digitalWrite(8,HIGH); // bật LED
//        Serial.println("LED Green is turned ON");  // gửi lại thông báo lên Serial Monitor
//    }
//    
//    if(text == "0001") {
//        digitalWrite(8,LOW); // tắt LED
//        Serial.println("LED Green is turned OFF");
//    }
//
//      if(text == "0100") {
//        digitalWrite(9,HIGH); // bật LED
//        Serial.println("LED Red is turned ON");  // gửi lại thông báo lên Serial Monitor
//    }
//    
//    if(text == "0101") {
//        digitalWrite(9,LOW); // tắt LED
//        Serial.println("LED Red is turned OFF");
//    }
//        if(text == "0200") {
//        digitalWrite(10,HIGH); // bật LED
//        Serial.println("LED Yellow is turned ON");  // gửi lại thông báo lên Serial Monitor
//    }
//    
//    if(text == "0201") {
//        digitalWrite(10,LOW); // tắt LED
//        Serial.println("LED Yellow is turned OFF");
//    }
    
   }
 }
