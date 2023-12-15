void Check_UUID_function(int data[])
{
  int count_uuid = 0;
  do{
  // take UUID and return true or false UUID to PC
  switch(status_run)
    {
      case take_UUID:
//      Serial.print(" \n case take_UUID\n");
//      Serial.print(" \n Wating for card \n");
      count_uuid ++;
      //delay(1000);
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
        //Serial.print("\nHello dont run me\n");
        status_run = check_UUID;
      }
      break;
  
      case check_UUID:
      {
        //Serial.print(" \n case check_UUID\n");
          Check_UUID();
          if(access_uuid == true)
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
          //Serial.print(" \n Your UUID true\n Going to program"); 
          status_run = take_UUID;
          tx_buf_len =  1;
            tx_buf[0] = 1; //Return data = 1 is true UUID
            cmd = CHECK_UUID;
            compose_packet();
            uart_transmit();
      break;
      
      case false_UUID:
          //Serial.print(" \n Wrong UUID \n Please take your card again"); 
          status_run = take_UUID;
          tx_buf_len =  1;
          tx_buf[0] = 0; //Return data = 1 is wrong UUID
          cmd = CHECK_UUID;
          compose_packet();
          uart_transmit();
          delay(1000);
      break;
    
      default:
          Serial.print(" \n Wrong status_run\n");
          status_run = take_UUID; 
          break;
      } 
  }while(count_uuid < 2);
}
