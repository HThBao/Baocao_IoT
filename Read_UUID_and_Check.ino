// (c) Michael Schoeffler 2014, http://www.mschoeffler.de
/*
 *  * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 * 
 */
#include "SPI.h" // SPI library
#include "MFRC522.h" // RFID library (https://github.com/miguelbalboa/rfid)

#define take_UUID 1
#define check_UUID 2
#define true_UUID 3
#define false_UUID 4
//WARNING This port follow table at the comment
//This code use MEGA 1650 for run
const int pinRST = 5;     
const int pinSDA = 53;

static uint16_t customer_UUID[4];  
bool access = false;
int status_run = take_UUID;
MFRC522 mfrc522(pinSDA, pinRST); // Set up mfrc522 on the Arduino

void setup() {
  SPI.begin(); // open SPI connection
  mfrc522.PCD_Init(); // Initialize Proximity Coupling Device (PCD)
  Serial.begin(9600); // open serial connection
}
void Check_UUID()
{
  uint16_t user1_UUID[3][4] = {{ 250 , 176 , 246 , 218 }, { 163 , 19 , 140 , 16 }, { 227 , 100 , 129 , 16 }};
              for(int x = 0; x < 3; x++){
              for(int i = 0; i < 4; i++ ){
                  if(customer_UUID[i] != user1_UUID[x][i]) {
                      access = false;
                      break;
                  } else {
                      access = true;
                  }
              }
              if(access) break;
            }
}
void loop() {
  switch(status_run)
  {
    case take_UUID:
    Serial.print(" \n Wating for card \n"); 
    delay(1000);
    if (mfrc522.PICC_IsNewCardPresent()) { // (true, if RFID tag/card is present ) PICC = Proximity Integrated Circuit Card
      if(mfrc522.PICC_ReadCardSerial()) { // true, if RFID tag/card was read
        Serial.print("RFID TAG ID:");
        for (byte i = 0; i < mfrc522.uid.size; ++i) { // read id (in parts)
          Serial.print(mfrc522.uid.uidByte[i], HEX); // print id as hex values
          customer_UUID[i] = (mfrc522.uid.uidByte[i]);
          Serial.print(" "); // add space between hex blocks to increase readability
        }
        
        Serial.println(); // Print out of id is complete.
      }
      status_run = check_UUID;
    }
    break;

    case check_UUID:
    {
        Check_UUID();
        if(access == true)
        {
          status_run = true_UUID;
        }
        else 
        {
           status_run = false_UUID;
        }
    }
    break;
    
    case true_UUID:
        Serial.print(" \n Your UUID true\n Going to program"); 
        status_run = take_UUID;
        delay(5000);
    break;
    
    case false_UUID:
        Serial.print(" \n Wrong UUID \n Please take your card again"); 
        status_run = take_UUID;
        delay(5000);
    break;
  
    default:
        Serial.print(" \n Wrong status_run\n");
        status_run = take_UUID; 
        break;
    }    
}
 
