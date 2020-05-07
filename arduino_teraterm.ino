 
#define BAUDRATE 57600
#define DEBUGOUTPUT 0
#define powercontrol 10

// checksum variables
byte generatedChecksum = 0;
byte checksum = 0; 
int payloadLength = 0;
byte payloadData[64] = {0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;

// system variables
long lastReceivedPacket = 0;
boolean bigPacket = false;

//////////////////////////
// Microprocessor Setup //
//////////////////////////
void setup() 

{
  Serial.begin(BAUDRATE);           // USB
  Serial.println("Poor Quality, Attention, Meditation, Time Since Last Packet, Case_1, Case_2, Case_3, Case_4, Case_5");
}

////////////////////////////////
// Read data from Serial UART //
////////////////////////////////
byte ReadOneByte() 

{
  int ByteRead;
  while(!Serial.available());
  ByteRead = Serial.read();

#if DEBUGOUTPUT  
  Serial.print((char)ByteRead);   // echo the same byte out the USB serial (for debug purposes)
#endif

  return ByteRead;
}

/////////////
//MAIN LOOP//
/////////////
void loop() 

{
  
  // Look for sync bytes
  if(ReadOneByte() == 170) 
  {
    if(ReadOneByte() == 170) 
    {
        payloadLength = ReadOneByte();
      
        if(payloadLength > 169)                      //Payload length can not be greater than 169
        return;
        generatedChecksum = 0;        
        for(int i = 0; i < payloadLength; i++) 
        {  
        payloadData[i] = ReadOneByte();            //Read payload into memory
        generatedChecksum += payloadData[i];
        }   

        checksum = ReadOneByte();                      //Read checksum byte from stream      
        generatedChecksum = 255 - generatedChecksum;   //Take one's compliment of generated checksum

        if(checksum == generatedChecksum) 
        {    
          poorQuality = 200;
          attention = 0;
          meditation = 0;

          for(int i = 0; i < payloadLength; i++) 
          {                                          // Parse the payload
          switch (payloadData[i]) 
          {
          case 2:
            i++;            
            poorQuality = payloadData[i];
            bigPacket = true;  
            //delay(5000);          
            break;
          case 4:
            i++;
            attention = payloadData[i];                        
            break;
          case 5:
            i++;
            meditation = payloadData[i];
            break;
          case 0x80:
            i = i + 3;
            break;
          case 0x83:
            i = i + 25;      
            break;
          default:
            break;
          } // switch
        } // for loop

#if !DEBUGOUTPUT

        // *** Add your code here ***

        if(bigPacket) 
        {
          Serial.print(poorQuality, DEC);
          Serial.print(",");
          Serial.print(attention, DEC);
          Serial.print(",");
          Serial.print(meditation, DEC);
          Serial.print(",");
          Serial.print(millis() - lastReceivedPacket, DEC);
          lastReceivedPacket = millis();
          Serial.print(",");
          if(poorQuality!=0)                 //case_5
          { Serial.print("1"); //5
            Serial.print(","); 
            Serial.print("0"); //1 
            Serial.print(",");
            Serial.print("0"); //2
            Serial.print(",");
            Serial.print("0"); //3
            Serial.print(",");
            Serial.print("0"); //4
            Serial.print("\n");
          }
          else
          {
            Serial.print("0");
            Serial.print(",");                    
            if(attention>=65&&meditation>=65) //case_1
            { Serial.print("1");
            }
            else
            {Serial.print("0");
            }
            Serial.print(",");
            if(attention<=35&&meditation>=75)  //case_2
            { Serial.print("1");
            }
            else
            {Serial.print("0");
            }
            Serial.print(",");
            if(attention>=75&&meditation<=50)  //case_3
            { Serial.print("1");
            }
            else
            {Serial.print("0");
            }
            Serial.print(",");
            if(attention<=50&&meditation<=65)  //case_4
            { Serial.print("1");
            }
            else
            {Serial.print("0");
            }
            Serial.print("\n");                                      
          }
         }                     
        }
#endif        
        bigPacket = false;        
      }
      else {
        // Checksum Error
      }  // end if else for checksum
    } // end if read 0xAA byte
  } // end if read 0xAA byte




