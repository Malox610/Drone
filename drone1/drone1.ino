//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h> 

#define t1 "PIPE2"     
#define t2 "PIPE1"     
RF24 radio(9, 8);  // CE, CSN

const byte adresses[][6] = {t1, t2};

Servo myServoX1;
Servo myServoY1;
Servo myServoX2;
Servo myServoY2;

const int bouton = 2;

int PosX1 = 90;
int PosY1 = 90;
int PosX2 = 90;
int PosY2 = 90;
const int mdp =2534;


void setup()
{
    Serial.begin(115200);
    myServoX1.attach(7);
    myServoY1.attach(6);
    myServoX2.attach(4);
    myServoY2.attach(3);
    pinMode(bouton, INPUT);
  radio.begin();                          
  radio.openWritingPipe(adresses[0]);      
  radio.openReadingPipe(1, adresses[1]);   
  radio.setPALevel(RF24_PA_MIN);   
  radio.setChannel(96); 

  delay(2000);
}

void loop()
{
 radio.stopListening();  
 int buzzer = digitalRead(bouton);  
 radio.write(&buzzer, sizeof(buzzer));
 delay(5);

radio.startListening();  
if (radio.available()){
  while (radio.available()) {  
    
int Pos[] = {PosX1,PosY1,PosX2,PosY2,mdp};
    int value = 3;
    radio.read(Pos, sizeof(Pos));
    Serial.println(value);
      if(Pos[5]==mdp)
      {
    PosX1 = Pos[0];
    PosY1 = Pos[1];
    PosX2 = Pos[2];
    PosY2 = Pos[3];
    
    myServoX1.write(PosX1);
    myServoY1.write(PosY1);  
    myServoX2.write(PosX2);
    myServoY2.write(PosY2);

  //Serial.println(Pos[0]);
  //Serial.println(PosY1);
  //Serial.println(PosX2);
  //Serial.println(PosY2);  
      }else
      {
        Serial.println("mauvaise commande");
        }
  }
  delay(20);
  }
  delay(5);
}
