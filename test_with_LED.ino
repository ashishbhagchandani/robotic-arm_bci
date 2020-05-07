#define LED 13
#define BAUDRATE 57600
#define DEBUGOUTPUT 0


#define GREEN3    2
#define YELLOW4   3
#define GREEN2    4
#define YELLOW3   5
#define GREEN1    6
#define YELLOW2   7
#define YELLOW1   8
#define RED       9

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
  pinMode(GREEN3, OUTPUT);
  pinMode(YELLOW4, OUTPUT);
  pinMode(GREEN2, OUTPUT);
  pinMode(YELLOW3, OUTPUT);
  pinMode(GREEN1, OUTPUT);
  pinMode(YELLOW2, OUTPUT);
  pinMode(YELLOW1, OUTPUT);
  pinMode(RED, OUTPUT);

  pinMode(LED, OUTPUT);
  Serial.begin(BAUDRATE);           // USB
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
          if(poorQuality == 0)
             digitalWrite(LED, HIGH);
          else
            digitalWrite(LED, LOW);
          
          Serial.print("PoorQuality: ");
          Serial.print(poorQuality, DEC);
          Serial.print(" Attention: ");
          Serial.print(attention, DEC);
          Serial.print(" Time since last packet: ");
          Serial.print(millis() - lastReceivedPacket, DEC);
          lastReceivedPacket = millis();
          Serial.print("\n");

          switch(attention / 10) 
          {
          case 0:
            digitalWrite(GREEN3, LOW);
            digitalWrite(YELLOW4, LOW);
            digitalWrite(GREEN2, LOW);
            digitalWrite(YELLOW3, LOW);
            digitalWrite(GREEN1, LOW);
            digitalWrite(YELLOW2, LOW);
            digitalWrite(YELLOW1, LOW);
            digitalWrite(RED, LOW);           
            break;
          case 1:
            digitalWrite(GREEN3, HIGH);
            digitalWrite(YELLOW4, LOW);
            digitalWrite(GREEN2, HIGH);
            digitalWrite(YELLOW3, LOW);
            digitalWrite(GREEN1, HIGH);
            digitalWrite(YELLOW2, LOW);
            digitalWrite(YELLOW1, HIGH);
            digitalWrite(RED, LOW);
            break;
          case 2:
            digitalWrite(GREEN3, HIGH);
            digitalWrite(YELLOW4, LOW);
            digitalWrite(GREEN2, LOW);
            digitalWrite(YELLOW3, HIGH);
            digitalWrite(GREEN1, LOW);
            digitalWrite(YELLOW2, LOW);
            digitalWrite(YELLOW1, LOW);
            digitalWrite(RED, LOW);
            break;
          case 3:              
            digitalWrite(GREEN3, LOW);
            digitalWrite(YELLOW4, LOW);
            digitalWrite(GREEN2, LOW);
            digitalWrite(YELLOW3, LOW);
            digitalWrite(GREEN1, HIGH);
            digitalWrite(YELLOW2, LOW);
            digitalWrite(YELLOW1, LOW);
            digitalWrite(RED, LOW);             
            break;
          case 4:
            digitalWrite(GREEN3, LOW);
            digitalWrite(YELLOW4, HIGH);
            digitalWrite(GREEN2, LOW);
            digitalWrite(YELLOW3, LOW);
            digitalWrite(GREEN1, LOW);
            digitalWrite(YELLOW2, HIGH);
            digitalWrite(YELLOW1, LOW);
            digitalWrite(RED, LOW);            
            break;
          case 5:
            digitalWrite(GREEN3, LOW);
            digitalWrite(YELLOW4, HIGH);
            digitalWrite(GREEN2, LOW);
            digitalWrite(YELLOW3, LOW);
            digitalWrite(GREEN1, LOW);
            digitalWrite(YELLOW2, LOW);
            digitalWrite(YELLOW1, LOW);
            digitalWrite(RED, HIGH);             
            break;
          case 6:              
            digitalWrite(GREEN3, LOW);
            digitalWrite(YELLOW4, LOW);
            digitalWrite(GREEN2, HIGH);
            digitalWrite(YELLOW3, LOW);
            digitalWrite(GREEN1, LOW);
            digitalWrite(YELLOW2, HIGH);
            digitalWrite(YELLOW1, LOW);
            digitalWrite(RED, LOW);            
            break;
          case 7:
            digitalWrite(GREEN3, LOW);
            digitalWrite(YELLOW4, HIGH);
            digitalWrite(GREEN2, HIGH);
            digitalWrite(YELLOW3, HIGH);
            digitalWrite(GREEN1, LOW);
            digitalWrite(YELLOW2, LOW);
            digitalWrite(YELLOW1, LOW);
            digitalWrite(RED, LOW);            
            break;    
          case 8:
            digitalWrite(GREEN3, LOW);
            digitalWrite(YELLOW4, LOW);
            digitalWrite(GREEN2, LOW);
            digitalWrite(YELLOW3, HIGH);
            digitalWrite(GREEN1, HIGH);
            digitalWrite(YELLOW2, LOW);
            digitalWrite(YELLOW1, LOW);
            digitalWrite(RED, LOW);
            break;
          case 9:
            digitalWrite(GREEN3, LOW);
            digitalWrite(YELLOW4, LOW);
            digitalWrite(GREEN2, HIGH);
            digitalWrite(YELLOW3, HIGH);
            digitalWrite(GREEN1, HIGH);
            digitalWrite(YELLOW2, HIGH);
            digitalWrite(YELLOW1, LOW);
            digitalWrite(RED, LOW);
            break;
          case 10:
            digitalWrite(GREEN3, LOW);
            digitalWrite(YELLOW4, LOW);
            digitalWrite(GREEN2, LOW);
            digitalWrite(YELLOW3, LOW);
            digitalWrite(GREEN1, HIGH);
            digitalWrite(YELLOW2, HIGH);
            digitalWrite(YELLOW1, HIGH);
            digitalWrite(RED, HIGH);
            break;           
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
}



