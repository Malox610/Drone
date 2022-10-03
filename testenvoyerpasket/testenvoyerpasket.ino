//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN
const int SW_pin = 7; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

//address through which two modules communicate.
const byte address[6] = "00001";

void setup()
{
  Serial.begin(9600);

  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  int p1=0;
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();
}
void loop()
{
 int position1 = 0;
 if(analogRead(Y_pin)>=800 && analogRead(X_pin) <= 600 && analogRead(X_pin) >= 400 ){
    position1 = 1;
  }else if(analogRead(Y_pin)<=300 && analogRead(X_pin) <= 600 && analogRead(X_pin) >= 400){
    position1 = 2;
  }else if(analogRead(X_pin)>=800 && analogRead(Y_pin) <= 600 && analogRead(Y_pin) >= 400){
    position1 = 3;
  }else if(analogRead(X_pin)<=300 && analogRead(Y_pin) <= 600 && analogRead(Y_pin) >= 400){
    position1 = 4;
  }//test
  bool rep = radio.write(&position1, sizeof(position1));
  Serial.println(position1);
  delay(100);
}
