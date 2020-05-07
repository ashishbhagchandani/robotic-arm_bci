#include<Servo.h>
#define LED 13 
#define powercontrol 10 
#define LED6   7
#define LED5   8
#define LED4   9
#define LED3   10
#define LED2   11
#define LED1   12

Servo wrist;
Servo finger;
Servo elv;
Servo elf;
Servo shoulder;

char val;
int angle=0;

byte generatedChecksum = 0;
byte checksum = 0; 
int payloadLength = 0;
byte payloadData[64] = {0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;

long lastReceivedPacket = 0;
boolean bigPacket = false;

void setup()
{
wrist.attach(5);
//wrist.write(0);

finger.attach(6);
//finger.write(0);

elv.attach(3);
//elv.write(0);

elf.attach(4);
//elf.write(0);

shoulder.attach(2);
//shoulder.write(0);

Serial.begin(57600);
//Serial.println("We Work");  
}

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
            delay(5000);           
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
            Serial.print("PoorQuality: ");
            Serial.print(poorQuality, DEC);
            Serial.print(" Attention: ");
            Serial.print(attention, DEC);
            Serial.print(" Time since last packet: ");
            Serial.print(millis() - lastReceivedPacket, DEC);
            lastReceivedPacket = millis();
            Serial.print("\n"); 
            digitalWrite(LED, LOW);    
            switch(attention / 10) 
            {
             case 0:
              digitalWrite(LED1, HIGH);  
              rotation1();
              delay(100);
              break;
             case 1:
              digitalWrite(LED2, HIGH);
              rotation2();
              delay(100);
              break;
             case 2:
              digitalWrite(LED3, HIGH);
              rotation3();
              delay(100);
              break;
             case 3:              
              digitalWrite(LED4, HIGH);
              rotation4();
              delay(100); 
              break;
             case 4:
              digitalWrite(LED5, HIGH);
              rotation5();
              delay(100);             
              break;
             case 5:
              digitalWrite(LED6, HIGH);           
              break;
             case 6:              
              digitalWrite(LED1, HIGH);
              digitalWrite(LED2, HIGH);
              delay(100); 
              break;
             case 7:
              digitalWrite(LED1, HIGH);
              digitalWrite(LED3, HIGH);
              delay(100);               
              break;    
             case 8:
              digitalWrite(LED1, HIGH); 
              digitalWrite(LED4, HIGH);
              delay(100);
              break;
             case 9:
              digitalWrite(LED1, HIGH); 
              digitalWrite(LED5, HIGH);
              delay(100);              
              break;
             case 10:
              digitalWrite(LED1, HIGH);
              digitalWrite(LED6, HIGH);
              delay(100); 
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

void rotation1(){
  for(angle = 0; angle < 90; angle++)  
  {                                  
    finger.write(angle);               
    delay(70);                   
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 90; angle > 0; angle--)    
  {                                
    finger.write(angle);           
    delay(70);       
  } 
}

void rotation2(){
  for(angle = 0; angle < 90; angle++)  
  {                                  
    wrist.write(angle);               
    delay(70);                   
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 90; angle > 0; angle--)    
  {                                
    wrist.write(angle);           
    delay(70);       
  } 
}

void rotation3(){
  for(angle = 0; angle < 90; angle++)  
  {                                  
    elv.write(angle);               
    delay(70);                   
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 90; angle > 0; angle--)    
  {                                
    delay(70);       
  } 
}

void rotation4(){
  for(angle = 0; angle < 90; angle++)  
  {                                  
    elf.write(angle);               
    delay(70);                   
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 90; angle > 0; angle--)    
  {                                
    elf.write(angle);           
    delay(70);       
  } 
}

void rotation5(){
  for(angle = 0; angle < 90; angle++)  
  {                                  
    shoulder.write(angle);               
    delay(70);                    
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 90; angle > 0; angle--)    
  {                                
    shoulder.write(angle);           
    delay(70);        
  } 
}
