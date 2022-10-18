//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(5, 4);  // CE, CSN

const int SW1_pin = 2; 
const int X1_pin = A3; 
const int Y1_pin = A2; 
const int SW2_pin = 3;
const int X2_pin = A1;
const int Y2_pin = A0; 

const byte address[6] = "00001";

void setup()
{
  Serial.begin(9600);

  
  int p1=0;
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();
}
void loop()
{
 
 int PosX1 = 0;
 int PosY1 = 0;
 int PosX2 = 0;
 int PosY2 = 0;
 
  if(digitalRead(SW2_pin)==0){
  analogWrite(6,200);
  }else {
  analogWrite(6,0);
  }
  PosX1 = analogRead(X1_pin);
  PosY1 = analogRead(Y1_pin);
  PosX2 = analogRead(X2_pin);
  PosY2 = analogRead(Y2_pin);

  int Pos[] = {PosX1,PosY1,PosX2,PosY2};
  
  Pos[0] = map(PosX1, 0,1023,0,180);
  Pos[1] = map(PosY1, 0,1023,0,180);
  Pos[2] = map(PosX2, 0,1023,0,180);
  Pos[3] = map(PosY2, 0,1023,0,180);

  
  Serial.println(PosX1);
  Serial.println(PosY1);
  Serial.println(PosX2);
  Serial.println(PosY2);

  bool rep = radio.write(Pos, sizeof(Pos));
  delay(100);
}
