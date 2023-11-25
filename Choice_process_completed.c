//PC CODE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <windows.h>

#define BUFFER_SIZE 256
#define HEADER 0x26

#define DOOR_CONTROL 0x00
#define DOOR_A_ID 0x00
#define DOOR_B_ID 0x01
#define DOOR_OPEN 0x00
#define DOOR_CLOSE 0x01
//Define for Get door status
#define READ_STATUS_DOOR 00
#define READ_STATUS_DOOR_OPEN  00
#define READ_STATUS_DOOR_CLOSE  01
//Define for Light
#define LIGHT_CONTROL 02
#define LIGHT_A_ID 00
#define LIGHT_B_ID 01
#define LIGHT_ON 00
#define LIGHT_OFF 01
//Define for Led
#define LED_ID 03
//Define for flash
#define FLASH_ID 04

int process = 1;


/*----------------------------------------------------------------------------------------
---------------------Do not change code in there (1) -------------------------------------
-----------------------------------------------------------------------------------------*/

HANDLE open_port(const char* device, unsigned long baud_rate, unsigned char bit_size, unsigned char parity) {
    HANDLE port = CreateFileA(device, GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (port == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE_VALUE;
    }

    // Flush away any bytes previously read or written.
    BOOL success = FlushFileBuffers(port);
    if (!success)
    {
        printf("Failed to flush serial port");
        CloseHandle(port);
        return INVALID_HANDLE_VALUE;
    }

    // Configure read and write operations to time out after 100 ms.
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 0;
    timeouts.ReadTotalTimeoutConstant = 100;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    success = SetCommTimeouts(port, &timeouts);
    if (!success)
    {
        printf("Failed to set serial timeouts");
        CloseHandle(port);
        return INVALID_HANDLE_VALUE;
    }

    // Set the baud rate and other options.
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(DCB);
    dcb.BaudRate = baud_rate;
    dcb.ByteSize = bit_size;
    dcb.Parity = parity;
    dcb.StopBits = ONESTOPBIT;
    success = SetCommState(port, &dcb);
    if (!success)
    {
        printf("Failed to set serial settings");
        CloseHandle(port);
        return INVALID_HANDLE_VALUE;
    }


    // display information
    printf("----------------------------------\n");
    printf("baud rate = %d\n", dcb.BaudRate);
    printf("Parity = %d\n", dcb.Parity);
    printf("Byte Size = %d\n", dcb.ByteSize);
    printf("Stop Bit = %d\n", dcb.StopBits);
    printf("----------------------------------\n");
    return port;
}
// Transmit data
int uart_transmit(HANDLE port, unsigned char* TX_buf, unsigned int TX_buf_len) {
    int writtenbytes = 0;
    if (WriteFile(port, TX_buf, TX_buf_len, (LPDWORD)&writtenbytes, NULL)) {
        if (writtenbytes == 0) {
            printf("WriteFile() timed out\n");
            return -1;
        }
    }
    else {
        printf("WriteFile() failed\n");
        return -1;
    }
    printf("%d bytes were written\n", writtenbytes);
    return 0;
}
/*------------------------------------END (1)----------------------------------------------*/
/*----------------------------------------------------------------------------------------*/
//Receive data warning change correct (*RX_buf_len = ...)
int uart_receive(HANDLE port, int* RX_buf_len, unsigned char* RX_buf,int* leng) {
    char readbuf;
    int nbbytes = 0;
    for (int a = 0; a < leng; a++)
    {
        if (ReadFile(port, &readbuf, 1, (LPDWORD)&nbbytes, NULL)) {
            if (nbbytes == 0) {
                printf("ReadFile() timed out\n");
                return -1;
            }
        }
        else {
            printf("ReadFile() failed\n");
            return -1;
        }
        *RX_buf_len = leng;
        RX_buf[a] = readbuf;

    }
    return 0;
}
//Function Bao buid
void DOOR_CONTROL_FUNCTION(int* TX_buf_len ,unsigned char* TX_buf)
{
    int choosedoor, choosestatus;
    *TX_buf_len = 4;
    printf("\nStart process DOOR CONTROL:\n");
    TX_buf[1] = DOOR_CONTROL;
    //Choose Status
    printf("\nChoose door: ");
    printf("\n1.Door: A");
    printf("\n2.Door: B");
    printf("\nDoor : ");
    scanf("%d", &choosedoor);
    //Choose Status
    printf("\nChoose door status: ");
    printf("\n1.OPEN");
    printf("\n2.CLOSE");
    printf("\nStatus : ");
    scanf("%d", &choosestatus);
    if(choosedoor == 1)
        TX_buf[2] = DOOR_A_ID;
    else 
        TX_buf[2] = DOOR_B_ID;

    if (choosestatus == 1)
        TX_buf[3] = DOOR_OPEN;
    else
        TX_buf[3] = DOOR_CLOSE;
}

void GET_TEMPERATURE(int* TX_buf_len, unsigned char* TX_buf)
{
    *TX_buf_len = 2;
    TX_buf[1] = FLASH_ID;

}


//main
int main()
{
    int leng;
    // configuration parameters
    const char* device = "\\\\.\\COM20";
    unsigned long baud_rate = 9600;
    unsigned char bit_size = 8;
    unsigned char parity = 0;
    // code
    int status = -1;
    unsigned char TX_buf[BUFFER_SIZE], RX_buf[BUFFER_SIZE];
    unsigned int TX_buf_len = 0, RX_buf_len = 0;

    HANDLE port = open_port(device, baud_rate, bit_size, parity);
    if (port == INVALID_HANDLE_VALUE) {
        return -1;
    }
       while (process != 0)
    {
        TX_buf[0] = HEADER;
        printf("\n================================================");
        printf("\nChoice process   ");
        printf("\n1.Control door   ");
        printf("\n2.Control LED   ");
        printf("\n3.Check LED status ");
        printf("\n4.Read temperature");
        printf("\nChoice process:  ");
        scanf("%d", &process);
        if (process == 0)
            break;
        if (process == 1)
        {
            DOOR_CONTROL_FUNCTION(&TX_buf_len,TX_buf);
            leng = 4;
        }


        if (process == 4)
        {
            GET_TEMPERATURE(&TX_buf_len, TX_buf);
            leng = 2;
        }
        



 
        /*----------------------------------------------------------------------------------------
        ---------------------Do not change code in there (2) -------------------------------------
        // ---------------------------------------------------------------------------------------*/
        // transmit data
        status = uart_transmit(port, TX_buf, TX_buf_len );
        if (status != 0)
            return -1;

        // Receive data
        status = uart_receive(port, &RX_buf_len, RX_buf, leng);
        if (status != 0)
            return -1;
        /*------------------------------------END(2)------------------------------------------------*/

        // Display
        printf("Received packet: ");
        for (int j = 0; j < RX_buf_len; j++) {
            printf("%x\t", RX_buf[j]);
        }
    }
    // Close the serial port.
    if (!CloseHandle(port))
    {
        printf("CloseHandle() failed\n");
        return -1;
    }
   

}



//Arduino code
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

//Show data which is received
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
      
//Resend data to PC
      for(int a = 0; a < i; a++)
      {
      Serial1.write(data[a]);
      Serial.print(" I send  ");
      Serial.println(data[a], HEX);
      }
      flag = 0;
    }
     // flag = 2 is WRONG HEADER
    if( flag ==2) 
    {
      Serial.print("\n Wrong header \n");
      flag = 0;
    }
}
