//Include Libraries
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
//#include <nRF24L01.h>
#include <RF24.h>
#include <MPU9250_WE.h>


#define t1 "PIPE1"    
#define t2 "PIPE2"
#define MPU9250_ADDR 0x68 //gyrospcope I2C adress
#define OLED_ADDR 0x3C

MPU9250_WE myMPU9250 = MPU9250_WE( MPU9250_ADDR);
//create an RF24 object
RF24 radio(5, 4);  // CE, CSN

const byte adresses[][6] = {t1, t2};

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int SW1_pin = 2; //D2  INT0
const byte X1_pin = A3;//A3 
const int Y1_pin = A2; //A2
const int SW2_pin = 3;//D3 INT1
const int X2_pin = A1;
const int Y2_pin = A0; //A0

 int PosX1 = 0;
 int PosY1 = 0;
 int PosX2 = 0;
 int PosY2 = 0;
 int PSW1 = 1 ;
 int PSW2 = 1;
 int buzzer = 1;
 int mdp =2534;

 byte mode =0 ;

  float x =0 ;
  float y=0;
  float z=0 ;


void interruptILS1()
{
  Serial.println("Interrupt 1" ) ;
  if(mode==0)
  {
    mode =1 ;
    
    
  }else if(mode==1)
  {
    mode =0;
    }
    delay(5);
}

void interruptILS2()
{
   Serial.println("Interrupt 2" ) ;
  if(mode==0)
  {
    mode =1 ;
    
  }else if(mode==1)
  {
    mode =0;
    }
    delay(5);
  }
  
void setup()
{
  Serial.begin(115200);
//interrupt

 pinMode(SW1_pin, INPUT_PULLUP);
 pinMode(SW2_pin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SW1_pin), interruptILS1, RISING);
  attachInterrupt(digitalPinToInterrupt(SW2_pin), interruptILS2, RISING);

 

//radio 
  radio.begin();                           
  radio.openWritingPipe(adresses[0]);      
  radio.openReadingPipe(1, adresses[1]);
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(96);

 // Wire.beginTransmission(OLED_ADDR);

  Wire.beginTransmission(0x3C); // start of the display 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Position you MPU9250 flat and don't move it - calibrating...");
  Wire.endTransmission();

  
  //ecrire sur l'ecran la calibration 
  //calibration
   Wire.beginTransmission(MPU9250_ADDR);//debut recuperation donné gyroscope
   
    delay(1000);
   myMPU9250.autoOffsets();
    Serial.println("Done!");

     myMPU9250.enableGyrDLPF();
    myMPU9250.setGyrDLPF(MPU9250_DLPF_6);
     myMPU9250.setSampleRateDivider(99);
     myMPU9250.setGyrRange(MPU9250_GYRO_RANGE_250);
   
  Wire.endTransmission(MPU9250_ADDR);//fin recuperation donnée gyroscope


   
  delay(1000);
}
void modeJoystick()
{
    radio.stopListening();   

  PosX1 = analogRead(X1_pin);
  PosY1 = analogRead(Y1_pin);
  PosX2 = analogRead(X2_pin);
  PosY2 = analogRead(Y2_pin);
  PSW1 = digitalRead(SW1_pin);
  PSW2 = digitalRead(SW2_pin);
Serial.println(PSW1);
Serial.println(" ");
Serial.println(PSW2);
Serial.println("\n");


mdp = 2534;

  int Pos[] = {PosX1,PosY1,PosX2,PosY2,mdp};
  
  Pos[0] = map(PosX1, 0,1023,0,180);
  Pos[1] = map(PosY1, 0,1023,0,180);
  Pos[2] = map(PosX2, 0,1023,0,180);
  Pos[3] = map(PosY2, 0,1023,0,180);
  

  //Serial.println(Pos[0]);
  //Serial.println(Pos[1]);
  //Serial.println(Pos[2]);
  //Serial.println(Pos[3]);
  Wire.beginTransmission(0x3C); // start of the display 
  display.clearDisplay();
  display.setCursor(10,10);
  display.println(Pos[0]);
  display.setCursor(10,30);
  display.println(Pos[2]);
  
  display.setCursor(40,10);
  display.println(Pos[1]);
  display.setCursor(40,30);
  display.println(Pos[3]);
  
 /* display.setCursor(70,10);
  display.println(PSW1);
  display.setCursor(70,30);
  display.println(PSW2);*/
  display.display();
  Wire.endTransmission(); //end of the display 
  radio.write(&Pos, sizeof(Pos));

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

void modeIMU()
{
  Wire.beginTransmission(MPU9250_ADDR);//debut recuperation donné gyroscope
  
  xyzFloat Gyr_value=myMPU9250.getGyrValues(); //in degrees/s
  x=Gyr_value.x ;
   y=Gyr_value.y ;
    z=Gyr_value.z ;
Serial.print("x =");
Serial.print(x,3);
Serial.print(" y =");
Serial.print(y,3);
Serial.print(" z =");
Serial.print(z,3);
Serial.print("\n");
delay(500);
Wire.endTransmission(MPU9250_ADDR);
  
  }

void loop()
{ 
Serial.println(mode);
  if(mode==1)
  {
    Serial.println("Mode IMU" ) ;
   modeIMU(); 
   
    }
    else
    {
      Serial.println("Mode Joystick" ) ;
      modeJoystick();
      }
 /* radio.stopListening();   

  PosX1 = analogRead(X1_pin);
  PosY1 = analogRead(Y1_pin);
  PosX2 = analogRead(X2_pin);
  PosY2 = analogRead(Y2_pin);
  PSW1 = digitalRead(SW1_pin);
  PSW2 = digitalRead(SW2_pin);
Serial.println(PSW1);
Serial.println(" ");
Serial.println(PSW2);
Serial.println("\n");
  //gyroscope 
  Wire.beginTransmission(MPU9250_ADDR);//debut recuperation donné gyroscope
  
  xyzFloat Gyr_value=myMPU9250.getGyrValues(); //in degrees/s
  x=Gyr_value.x ;
   y=Gyr_value.y ;
    z=Gyr_value.z ;
/*Serial.print("x =");
Serial.print(x,3);
Serial.print(" y =");
Serial.print(y,3);
Serial.print(" z =");
Serial.print(z,3);
Serial.print("\n");*/
//delay(900);

    
  /*Wire.endTransmission(MPU9250_ADDR);//fin recuperation donnée gyroscope
  
  
  mdp = 2534;

  int Pos[] = {PosX1,PosY1,PosX2,PosY2,mdp};
  
  Pos[0] = map(PosX1, 0,1023,0,180);
  Pos[1] = map(PosY1, 0,1023,0,180);
  Pos[2] = map(PosX2, 0,1023,0,180);
  Pos[3] = map(PosY2, 0,1023,0,180);
  

  //Serial.println(Pos[0]);
  //Serial.println(Pos[1]);
  //Serial.println(Pos[2]);
  //Serial.println(Pos[3]);
  Wire.beginTransmission(0x3C); // start of the display 
  display.clearDisplay();
  display.setCursor(10,10);
  display.println(Pos[0]);
  display.setCursor(10,30);
  display.println(Pos[2]);
  
  display.setCursor(40,10);
  display.println(Pos[1]);
  display.setCursor(40,30);
  display.println(Pos[3]);
  
 /* display.setCursor(70,10);
  display.println(PSW1);
  display.setCursor(70,30);
  display.println(PSW2);*/
/*  display.display();
  Wire.endTransmission(); //end of the display 
  radio.write(&Pos, sizeof(Pos));

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
 delay(5);*/
}
