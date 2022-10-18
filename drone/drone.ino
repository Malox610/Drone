//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h> 

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN
Servo myServoX1;
Servo myServoY1;
Servo myServoX2;
Servo myServoY2;

//address through which two modules communicate.
const byte address[6] = "00001";

void setup()
{
  while (!Serial);
    Serial.begin(9600);
    myServoX1.attach(6);
    myServoY1.attach(5);
    myServoX2.attach(4);
    myServoY2.attach(3);
    
  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, address);
  
  //Set module as receiver
  radio.startListening();
}

void loop()
{
int PosX1 = 90;
int PosY1 = 90;
int PosX2 = 90;
int PosY2 = 90;

  //Read the data if available in buffer
  if (radio.available())
  {
    
  int Pos[] = {PosX1,PosY1,PosX2,PosY2};
  
    radio.read(Pos, sizeof(Pos));

    PosX1 = Pos[0];
    PosY1 = Pos[1];
    PosX2 = Pos[2];
    PosY2 = Pos[3];
    
    myServoX1.write(PosX1);
    myServoY1.write(PosY1);  
    myServoX2.write(PosX2);
    myServoY2.write(PosY2);

  Serial.println(PosX1);
  Serial.println(PosY1);
  Serial.println(PosX2);
  Serial.println(PosY2);  }
}
