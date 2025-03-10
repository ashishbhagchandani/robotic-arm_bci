#include <Servo.h>
#define LED 13 //resistor 22 ohm
#define BAUDRATE 57600
#define DEBUGOUTPUT 0

Servo finger;
Servo wrist;
Servo elf;
Servo elv;
Servo shoulder;

#define powercontrol 10
int servopos =0;
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
  finger.attach(6);
  wrist.attach(5);
  elf.attach(4);
  elv.attach(3);
  shoulder.attach(2);

   //set all servos to starting position
  finger.write(0);
  wrist.write(0);
  elf.write(0);
  elv.write(0);
  shoulder.write(0);

  pinMode(LED, OUTPUT);
  Serial.begin(BAUDRATE);           // USB
 // Serial.println("WE WORK!!");
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
          if(poorQuality != 0){
             digitalWrite(LED, HIGH);
          }
          else{
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
              for(servopos=0;servopos<90;servopos++)
              {
                delay(400);
                finger.write(servopos);
                delay(700);
              }
              for(servopos=90;servopos>0;servopos--)
              {
                delay(400);
                finger.write(servopos);
                delay(700);
              }         
              break;
            case 1:
             for(servopos=0;servopos<90;servopos++)
              {
                delay(400);
                wrist.write(servopos);
                delay(700);
              }
              for(servopos=90;servopos>0;servopos--)
              {
                delay(400);
                wrist.write(servopos);
                delay(700);
              } 
              break;
            case 2:
             for(servopos=0;servopos<90;servopos++)
              {
                delay(400);
                elf.write(servopos);
                delay(700);
              }
              for(servopos=90;servopos>0;servopos--)
              {
                delay(400);
                elf.write(servopos);
                delay(700);
              } 
              break;
            case 3:              
             for(servopos=0;servopos<90;servopos++)
              {
                delay(400);
                elv.write(servopos);
                delay(700);
              }
              for(servopos=90;servopos>0;servopos--)
              {
                delay(400);
                elv.write(servopos);
                delay(700);
              }             
              break;
            case 4:
             for(servopos=0;servopos<90;servopos++)
              {
                delay(400);
                shoulder.write(servopos);
                delay(700);
              }
              for(servopos=90;servopos>0;servopos--)
              {
                delay(400);
                shoulder.write(servopos);
                delay(700);
              }              
              break;
            case 5:
             for(servopos=0;servopos<90;servopos++)
              {
                delay(400);
                finger.write(servopos);
                shoulder.write(servopos);
                delay(700);
              }
              for(servopos=90;servopos>0;servopos--)
              {
                delay(400);
                finger.write(servopos);
                shoulder.write(servopos);
                delay(700);
              }            
            break;
          case 6:              
             for(servopos=0;servopos<90;servopos++)
              {
                delay(400);
                wrist.write(servopos);
                shoulder.write(servopos);
                delay(700);
              }
              for(servopos=90;servopos>0;servopos--)
              {
                delay(400);
                wrist.write(servopos);
                shoulder.write(servopos);
                delay(700);
              }  
            break;
          case 7:
             for(servopos=0;servopos<90;servopos++)
              {
                delay(400);
                wrist.write(servopos);
                finger.write(servopos);
                delay(700);
              }
              for(servopos=90;servopos>0;servopos--)
              {
                delay(400);
                wrist.write(servopos);
                finger.write(servopos);
                delay(700);
              } 
            break;    
          case 8:
              for(servopos=0;servopos<90;servopos++)
              {
                delay(400);
                elf.write(servopos);
                elv.write(servopos);
                delay(700);
              }
              for(servopos=90;servopos>0;servopos--)
              {
                delay(400);
                elf.write(servopos);
                elv.write(servopos);
                delay(700);
              } 
            break;
          case 9:
              for(servopos=0;servopos<90;servopos++)
              {
                delay(400);
                elf.write(servopos);
                shoulder.write(servopos);
                delay(700);
              }
              for(servopos=90;servopos>0;servopos--)
              {
                delay(400);
                elf.write(servopos);
                shoulder.write(servopos);
                delay(700);
              } 
            break;
          case 10:
             for(servopos=0;servopos<90;servopos++)
              {
                delay(400);
                elv.write(servopos);
                shoulder.write(servopos);
                delay(700);
              }
              for(servopos=90;servopos>0;servopos--)
              {
                delay(400);
                elv.write(servopos);
                shoulder.write(servopos);
                delay(700);
              } 
            break;             
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
}



