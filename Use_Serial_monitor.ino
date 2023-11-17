#define LED_ID_A "00"
#define LED_ID_B "01"

#define LED_TURN_ON "00"
#define LED_TURN_OFF "01"

void setup()
{
  Serial.begin(9600); //Set up UART 
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  //Turn off two LED
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
}

void loop()
{
    if(Serial.available()) { // nếu có dữ liệu gửi đến
    String data = Serial.readStringUntil('\n'); // đọc giá trị gửi đến cho đến khi gặp kí tự xuống dòng \n
    String data_id = data.substring(0,2);
    String data_id_device = data.substring(2,4);
    String data_status = data.substring(4,6);

    /*
    //Check data 
    Serial.print("data_id: ");
    Serial.println(data_id);

    Serial.print("data_id_device: ");
    Serial.println(data_id_device);

    Serial.print("data_status: ");
    Serial.println(data_status);
    */
 
    if(data_id == "00")
    {
      LED_CONTROL(data_id_device,data_status);
    }

    //if(data_id == "01")



      
    
   }
 }

void LED_CONTROL(String led_id, String led_status)
{
      if(led_id == "00") {
        if( led_status == LED_TURN_ON )
        {
          digitalWrite(8,HIGH); // bật LED
          Serial.println("LED A is turned ON");  // gửi lại thông báo lên Serial Monitor
        }
        else
        {
          digitalWrite(8,LOW); // tắt LED
          Serial.println("LED A is turned OFF");
        }
    }
    
      if(led_id == "01") {
        if( led_status == LED_TURN_ON )
        {
          digitalWrite(9,HIGH); // bật LED
          Serial.println("LED B is turned ON"); 
        }
        else
        {
          digitalWrite(9,LOW); // tắt LED
          Serial.println("LED B is turned OFF");
        }
    }
}  
