

//Include Libraries
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <RF24.h>
#include <MPU6500_WE.h>

#define t1 "PIPE1"    
#define t2 "PIPE2"
#define MPU9250_ADDR 0x68 //gyrospcope I2C adress
#define OLED_ADDR 0x3C

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

MPU6500_WE myMPU9250 = MPU6500_WE( MPU9250_ADDR);
//create an RF24 object
RF24 radio(5, 4);  // CE, CSN

const byte adresses[][6] = {t1, t2};

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const byte SW1_pin = 2; //D2  INT0
const byte X1_pin = A3;//A3 
const byte Y1_pin = A2; //A2
const byte SW2_pin = 3;//D3 INT1
const byte X2_pin = A1;
const byte Y2_pin = A0; //A0

byte PSW1 = 1;
byte PSW2 = 1;
byte buzzer = 1;
byte mdp =25;

 byte mode =0 ;

  int x =0 ;
  int y=0;
  int z=0 ;


void interruptILS1()
{
  if(mode==0)
  {
    mode =1 ;
    delay(400);

  }else if(mode==1)
  {
    mode =0;
     delay(400);
    }
    delay(50);
}

void interruptILS2()
{
  if(mode == 0)
  {
    mode = 1 ;
    delay(400);
  }else if(mode == 1)
  {
    mode = 0 ;
    delay(400);
    }
    delay(50);
     
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
  radio.setChannel(120);


  Wire.beginTransmission(0x3C); // start of the display 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Position you MPU9250 flat and don't move it - calibrating...");
  Wire.endTransmission();

  
  //ecrire sur l'ecran la calibration 
  //calibration
   Wire.beginTransmission(MPU9250_ADDR);//debut recuperation donné gyroscope
   
    delay(1000);
   myMPU9250.autoOffsets();

     myMPU9250.enableGyrDLPF();
    myMPU9250.setGyrDLPF(MPU9250_DLPF_6);
     myMPU9250.setSampleRateDivider(99);
     myMPU9250.setGyrRange(MPU9250_GYRO_RANGE_250);
   
  Wire.endTransmission(MPU9250_ADDR);//fin recuperation donnée gyroscope

  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
   
  delay(1000);
}
void modeJoystick()
{
    radio.stopListening();   

  PSW1 = digitalRead(SW1_pin);
  PSW2 = digitalRead(SW2_pin);

mdp = 25;

  int Pos[] = {analogRead(X1_pin),analogRead(Y1_pin),analogRead(X2_pin),analogRead(Y2_pin),mdp,mode};

  
   long start = micros();
   int value = analogRead(X1_pin);
   long stop = micros();
   Serial.println(stop-start);

   
  Pos[0] = map(analogRead(X1_pin), 0,1023,0,180);
  Pos[1] = map(analogRead(Y1_pin), 0,1023,0,180);
  Pos[2] = map(analogRead(X2_pin), 0,1023,0,180);
  Pos[3] = map(analogRead(Y2_pin), 0,1023,0,180);
  

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

  display.display();
  Wire.endTransmission(); //end of the display 
  radio.write(&Pos, sizeof(Pos));
  delay(5);
}

void modeIMU()
{
  radio.stopListening();   
  Wire.beginTransmission(MPU9250_ADDR);//debut recuperation donné gyroscope
  
  xyzFloat Angle=myMPU9250.getAngles(); //in degrees/s
  x=Angle.x ;
   y=Angle.y ;
    z=Angle.z ;

delay(500);
Wire.endTransmission(MPU9250_ADDR);

 Wire.beginTransmission(0x3C); // start of the display 
  display.clearDisplay();
  display.setCursor(10,10);
  display.println(x);
  display.setCursor(10,20);
  display.println(y);
  
  display.setCursor(10,30);
  display.println(z);
  
  display.display();
   int Pos[] = {x,y,z,0,mdp,mode};
  radio.write(&Pos, sizeof(Pos));
  Wire.endTransmission(0x3C); //end of the display 
  }

void loop()
{ 
  if(mode==1)
  {
   modeIMU(); 
    }
    else
    {
      modeJoystick();
      }   
      
  radio.startListening();
  if(radio.available()) { 
  while (radio.available()) {                                           
  radio.read(&buzzer, sizeof(buzzer));
      
  //Serial.println(buzzer); 
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
