#include <SoftwareSerial.h>
#define header 0x26
int incomingByte = 0;
int data[10];
int data_temp[10]; // use data temp to check data recieve then save it to main data array
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
      data_temp[i] = incomingByte;
      if(data_temp[0] != header)  // use data temp array to check header
      {
          flag = 2;
      }else
      {
        flag = 1;
      }
      i++;
      
      delay(2);
    }
    // If true HEADER program will be Transmit via UART to PC
    if (flag == 1){
      for(int a = 0; a < i; a++)
      {
      Serial.print("I received: ");
      Serial.println(data_temp[a], HEX);
      }
      // Save data from data temp array to main data array
      for(int a = 0; a < i; a++)
      {
          data[a] = data_temp[a];
      }
      for(int a = 0; a < i; a++)
      {
      Serial1.write(data[a]);
      Serial.print(" I send  ");
      Serial.println(data[a], HEX);
      }
      flag = 0;
    }
    // flag = 1 is WRONG HEADER
    if( flag ==2) 
    {
      Serial.print("\n Wrong header \n");
      flag = 0;
    }
}
