#include <SoftwareSerial.h>
int incomingByte = 0;
int temp[10];
int flag = 0;
SoftwareSerial Serial1(10, 11);  //SoftwareSerial mySerial (rxPin, txPin);
void setup() {
  // initialize UART with baud rate of 9600, 8 bit payload, 0 parity
  Serial.begin(9600);                       
  Serial1.begin(9600);  
}

void loop() {  
  int i=0;
    // Receive via UART
    while (Serial1.available() > 0){
      incomingByte = Serial1.read();
      temp[i] = incomingByte;
      Serial.print("I received: ");
      Serial.println(incomingByte, HEX);
      flag = 1;
      i++;
    }
    // Transmit via UART
    if (flag){
      for(int a = 0; a < i; a++)
      {
      Serial1.write(temp[a]);
      Serial.print("\n I send  ");
      Serial.println(temp[a], HEX);
      }
      flag = 0;
    }
}
