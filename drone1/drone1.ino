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

const int Xpositive = 10;
const int Xnegative = 5;
const int Ypositive = A5;
const int Ynegative = A4;

const int bouton = 2;

int PosX1 = 90;
int PosY1 = 90;
int PosX2 = 90;
int PosY2 = 90;
byte mdp =25;
byte mdpcomming = 0;
byte mode =0 ;

void setup()
{
    Serial.begin(115200);
    myServoX1.attach(7);
    myServoY1.attach(6);
    myServoX2.attach(4);
    myServoY2.attach(3);
    pinMode(Xpositive, OUTPUT);
    pinMode(Xnegative, OUTPUT);
    pinMode(Ypositive, OUTPUT);
    pinMode(Ynegative, OUTPUT);
    pinMode(bouton, INPUT);

  radio.begin();                          
  radio.openWritingPipe(adresses[0]);      
  radio.openReadingPipe(1, adresses[1]);   
  radio.setPALevel(RF24_PA_MIN);   
  radio.setChannel(120); 

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
    
int Pos[] = {PosX1,PosY1,PosX2,PosY2,mdp,mode};

    radio.read(Pos, sizeof(Pos));
    PosX1 = Pos[0];
    PosY1 = Pos[1];
    PosX2 = Pos[2];
    PosY2 = Pos[3];
    mdpcomming = Pos[4];
    mode = Pos[5];
    if (mdp == mdpcomming ){
   if (mode == 0) { 
  
  digitalWrite(Xpositive, LOW);
  digitalWrite(Xnegative, LOW);
  digitalWrite(Ypositive, LOW);
  digitalWrite(Ynegative, LOW);

    myServoX1.write(PosX1);
    myServoY1.write(PosY1);  
    myServoX2.write(PosX2);
    myServoY2.write(PosY2);

  Serial.println(PosX1);
  Serial.println(PosY1);
  Serial.println(PosX2);
  Serial.println(PosY2);  
  }
  else if (mode ==1)
  {
    myServoX1.write(0);
    myServoY1.write(0);  
    myServoX2.write(0);
    myServoY2.write(0);

    if (PosX1 > 0){
      digitalWrite(Xpositive, HIGH);
      digitalWrite(Xnegative, LOW);
    }else{
      digitalWrite(Xnegative, HIGH);
      digitalWrite(Xpositive, LOW);
    }
    
      if (PosY1 > 0){
      digitalWrite(Ypositive, HIGH);
      digitalWrite(Ynegative, LOW);
    }else{
      digitalWrite(Ynegative, HIGH);
      digitalWrite(Ypositive, LOW);
    }
    
  Serial.print("x :");
  Serial.println(PosX1);
  Serial.print("y :");
  Serial.println(PosY1);
  Serial.print("z :");
  Serial.println(PosX2);
  }
    }else {
     Serial.println("wrong password"); 
    }
  }
  delay(20);
  }
  delay(5);
}
