//Include Libraries
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define t1 "PIPE1"    
#define t2 "PIPE2"

//create an RF24 object
RF24 radio(5, 4);  // CE, CSN

const byte adresses[][6] = {t1, t2};

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int SW1_pin = 2; 
const int X1_pin = A3; 
const int Y1_pin = A2; 
const int SW2_pin = 3;
const int X2_pin = A1;
const int Y2_pin = A0; 

 int PosX1 = 0;
 int PosY1 = 0;
 int PosX2 = 0;
 int PosY2 = 0;
 int buzzer = 1;
 
void setup()
{
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
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

  PosX1 = analogRead(X1_pin);
  PosY1 = analogRead(Y1_pin);
  PosX2 = analogRead(X2_pin);
  PosY2 = analogRead(Y2_pin);

  int Pos[] = {PosX1,PosY1,PosX2,PosY2};
  
  Pos[0] = map(PosX1, 0,1023,0,180);
  Pos[1] = map(PosY1, 0,1023,0,180);
  Pos[2] = map(PosX2, 0,1023,0,180);
  Pos[3] = map(PosY2, 0,1023,0,180);

  //Serial.println(Pos[0]);
  //Serial.println(Pos[1]);
  //Serial.println(Pos[2]);
  //Serial.println(Pos[3]);

  display.clearDisplay();
  display.setCursor(10,10);
  display.println(Pos[0]);
  display.setCursor(10,30);
  display.println(Pos[2]);
  display.setCursor(60,10);
  display.println(Pos[1]);
  display.setCursor(60,30);
  display.println(Pos[3]);
  display.display();
  radio.write(Pos, sizeof(Pos));

  delay(5);

  radio.startListening();
  if(radio.available()) { 
  while (radio.available()) {                                           
      radio.read(&buzzer, sizeof(buzzer)); 
      Serial.println(buzzer); 
        if(digitalRead(buzzer)==1){
  analogWrite(6,200);
  }else {
  analogWrite(6,0);
  }
  
    }
    delay(20);
}
 delay(5);
}
