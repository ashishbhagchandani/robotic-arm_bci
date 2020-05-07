#include <SoftwareSerial.h>

SoftwareSerial BTSerial(0, 1); // RX | TX

void setup()
{
  Serial.begin(9600);
  Serial.println("Enter AT commands:");
  BTSerial.begin(38400);       // HC-05 default speed in AT command more
}

void loop()
{
  if (BTSerial.available())    // read from HC-05 and send to Arduino Serial Monitor
  Serial.write(BTSerial.read());

  if (Serial.available())     // Keep reading from Arduino Serial Monitor and send to HC-05
  BTSerial.write(Serial.read());
}

//AT Commands
//AT+NAME=”PANTECH”
//AT+UART=”57600,0,0”
//AT+ROLE=”1”
//AT+PSWD=”1234”
//AT+CMODE=”0”
//AT+BIND=”XXXX,YY,ZZZZZZ”(Brainsense Unique Number)
//AT+IAC=”9E8B33”
//AT+CLASS=”0”
//AT+INQM=”1,9,48”
