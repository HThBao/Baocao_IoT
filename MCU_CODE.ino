#include <SoftwareSerial.h>
#define header 0x26
int incomingByte = 0;
int data[10];
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
      data[i] = incomingByte;
      if(data[0] !=  header)
      {
        Serial.print("\nWrong header");
        break;
      }
      flag = 1;
      i++;
    }
    // Transmit via UART
    if (flag){
      for(int a = 0; a < i; a++)
      {
      Serial1.write(data[a]);
      Serial.print("I received: ");
      Serial.println(data[a], HEX);
      }
      
      for(int a = 0; a < i; a++)
      {
      Serial1.write(data[a]);
      Serial.print(" I send  ");
      Serial.println(data[a], HEX);
      }
      flag = 0;
    }
}
