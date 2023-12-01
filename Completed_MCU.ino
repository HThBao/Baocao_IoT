#include <EEPROM.h>
#include <SoftwareSerial.h>

#define BUF_SIZE 256


#define RET_FAIL -1
#define RET_NONE 0
#define RET_SUCCESS 1

#define OFFSET_HEADER_1 0
#define OFFSET_HEADER_2 1
#define OFFSET_CMD 2
#define OFFSET_DATALENGTH 3
#define OFFSET_DATA 4
#define OFFSET_TRAILER_1 5
#define OFFSET_TRAILER_2 6

#define HEADER_1 0xAB
#define HEADER_2 0xCD
#define TRAILER_1 0xE1
#define TRAILER_2 0xE2

#define MAX_TABLE 4

#define DOOR_CONTROL 0x00 
#define DOOR_A_ID 0x00
#define DOOR_B_ID 0x01
#define DOOR_OPEN 0x00
#define DOOR_CLOSE 0x01
//Define for Light
#define LIGHT_CONTROL 01
#define LIGHT_A_ID 00
#define LIGHT_B_ID 01
#define LIGHT_ON 00
#define LIGHT_OFF 01
//Define for Get door status
#define READ_STATUS_DOOR 02
#define READ_STATUS_DOOR_OPEN  00
#define READ_STATUS_DOOR_CLOSE  01

//Define for Led
#define LED_ID 03
//Define for flash
#define FLASH_ID 04


uint8_t status;
uint8_t rx_buf [BUF_SIZE], rx_pack[BUF_SIZE];
uint8_t tx_buf [BUF_SIZE], tx_pack[BUF_SIZE];
uint8_t rx_buf_len;
uint8_t tx_buf_len;
uint8_t cmd;
uint8_t address;
uint8_t rw = 0;
uint8_t state_receive = OFFSET_HEADER_1;
int data_receive[10];

SoftwareSerial Serial1(10, 11);  //SoftwareSerial mySerial (rxPin, txPin);


int uart_transmit() {
    for (int i = 0; i < tx_buf_len + 6; i++){
      Serial1.write(tx_pack[i]);
    }

    Serial.print("\nTransmited packet:\t");
    for (int j = 0; j < tx_buf_len + 6; j++) {
        Serial.print(tx_pack[j], HEX);
        Serial.print("\t");
    }
    return RET_SUCCESS;
}

int uart_receive(unsigned int* data_temp) {
  int incomingByte;
  uint8_t readbuf; 
  int readbytes = 0; 
  int i;
  i = 0;
  int count = 0;
  while (Serial1.available() > 0){
    incomingByte = Serial1.read();
    data_temp[count] = incomingByte;
    count ++;
    readbuf = incomingByte & 0xFF;
    if (state_receive == OFFSET_HEADER_1){
      Serial.print("\nReceived packet:\n");
    }
    Serial.print(readbuf, HEX);
    Serial.print("\t");
    switch (state_receive) {
      case OFFSET_HEADER_1:
          if (readbuf == HEADER_1) {
            state_receive = OFFSET_HEADER_2;
            Serial.println("HEADER 1");
          }
          else {
              Serial.println("[ERROR]: Wrong header 1");
              return RET_FAIL;
          }
          break;
      case OFFSET_HEADER_2:
          if (readbuf == HEADER_2) {
            state_receive = OFFSET_CMD;
            Serial.println("HEADER 2");
          }  
          else {
              Serial.println("[ERROR]: Wrong header 2");
              state_receive = OFFSET_HEADER_1;
              return RET_FAIL;
          }
          break;
      case OFFSET_CMD:
          cmd = readbuf;
          state_receive = OFFSET_DATALENGTH;
          Serial.println("CMD");
          break;
      case OFFSET_DATALENGTH:
          rx_buf_len = readbuf;
          state_receive = OFFSET_DATA;
          Serial.println("DATALENGTH");
          break;
      case OFFSET_DATA:
          if (i < rx_buf_len-1){
            rx_buf[i++] = readbuf;
          }
          else{
            rx_buf[i++] = readbuf;
            state_receive = OFFSET_TRAILER_1;
          }
          Serial.println("DATA");
          break;
      case OFFSET_TRAILER_1:
          if (readbuf == TRAILER_1){
            state_receive = OFFSET_TRAILER_2;
            Serial.println("TRAILER_1");
          }
          else {
              Serial.println("[ERROR]: Wrong trailer 1");
              state_receive = OFFSET_HEADER_1;
              return RET_FAIL;
          }
          break;
      case OFFSET_TRAILER_2:
          state_receive = OFFSET_HEADER_1;
          if (readbuf == TRAILER_2){
            Serial.println("TRAILER_2");
            return RET_SUCCESS;
          }
          else {
              Serial.println("[ERROR]: Wrong trailer 2");
              return RET_FAIL;
          }
          break;
      default:
          break;
      }
  }
  return RET_FAIL;
}

 void compose_packet() {
  int i = 4;
  tx_pack[0] = HEADER_1;
  tx_pack[1] = HEADER_2;
  tx_pack[2] = cmd;
  tx_pack[3] = tx_buf_len;
  for (i; i < tx_buf_len+4; ++i)
      tx_pack[i] = tx_buf[i-4];
  tx_pack[i] = TRAILER_1;
  tx_pack[i + 1] = TRAILER_2;
}
//Control Door -----------------------------
void doorcontrol_function(int data[])
{
  Serial.print("\n Doing: Control Door ");
  if(data[4] == DOOR_A_ID)
  {
    if(data[5] == DOOR_OPEN)
    {
      //DOOR A
      Serial.print("OPEN DOOR A");
      //code to open door A
    }
    else
    {
      Serial.print("CLOSE DOOR A");
      //code to close door A
    }
  }
  else
  {
    if(data[5] == DOOR_OPEN)
    {
      //DOOR B
      Serial.print("\nOPEN DOOR B");
      //code to open door B
    }
    else
    {
      Serial.print("\nCLOSE DOOR B");
      //code to close door B
    }
  }

}
//lIGHT CONTROL ----------------------------
void lightcontrol_function(int data[])
{
  Serial.print("\n Doing: Control Light ");
  if(data[4] == LIGHT_A_ID)
  {
    if(data[5] == LIGHT_ON)
    {
      //LIGHT A
      Serial.print("\nTURN ON LIGHT A");
      //code to turn on light A
    }
    else
    {
      Serial.print("\nTURN OFF LIGHT A");
      //code to turn off light A
    }
  }
  else
  {
    //LIGHT B
      if(data[5] == LIGHT_ON)
    {
      Serial.print("\nTURN ON LIGHT B");
      //code to turn on light B
    }
    else
    {
      Serial.print("\nTURN OFF LIGHT B");
      //code to turn off light B
    }
  }
}
//Read Status Door ---------------------------
void readstatusdoor_function(int data[])
{
  //Call function read status of door
  //Describe this finction: This function is requested feedback data which about status door 
  //It receive data about id door then check status that door, final it resend data to PC
  
  tx_buf_len =  2; //It include DoorID and DoorStatus
  //Example: Read status door is Closing now.
  cmd = data[2];        //data[2] is CMD from PC send to MCU.
  tx_buf[0] = data[4]; //data[4] is DoorID which PC need check status
  tx_buf[1] = READ_STATUS_DOOR_CLOSE;
  compose_packet();
  uart_transmit();
  Serial.print("\n Reading status of door ");
}
//Setup time for LED --------------------------
void led_control_function(int data[])
{
  //Call function to setup time to led
  
  
  Serial.print("\n setup time to led ");
  Serial.print("\nHour: ");Serial.println(data[4]);
  Serial.print("\nMin: ");Serial.println(data[5]);
  Serial.print("\nSec: ");Serial.println(data[6]);

}
//Read Temperature -----------------------------
void read_temperature_function(int data[])
{
  //Call function read sensor then return data to PC
  Serial.print("\n Read Temperature from sensor ");
  tx_buf_len =  1; //Only return data of temperature
  //Example: Temperature is 32oC now.
  int data_temperature = 50;
  cmd = FLASH_ID;        //data[2] is CMD from PC send to MCU.
  tx_buf[0] = data_temperature;
  compose_packet();
  uart_transmit();
  Serial.print("\nReturn temperature data to PC ");
}


void setup() {
  Serial.begin(9600);                       
  Serial1.begin(9600); 
}
void loop() {  
    status = RET_FAIL;
    status = uart_receive(data_receive);
    if(status ==  RET_SUCCESS)
    {
      int process = data_receive[2]; //CMD here. It will be decide on the operation of device
      switch(process)
      {
        case DOOR_CONTROL:
          doorcontrol_function(data_receive);
        break;
        
        case LIGHT_CONTROL:
          lightcontrol_function(data_receive);
        break;

        case READ_STATUS_DOOR:
          readstatusdoor_function(data_receive);
        break;
        
        case LED_ID:
          led_control_function(data_receive);
        break;

        case FLASH_ID:
          read_temperature_function(data_receive);
        break;


    default:
      Serial.print("\nWrong data, It will not active");
      }
      
    }

    
}
